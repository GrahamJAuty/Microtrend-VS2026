/**********************************************************************/
#include <Share.h>
/**********************************************************************/
#include "DateRangeHelpers.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "PriceHelpers.h"
#include "RepCSV.h"
#include "ReportHelpers.h"
#include "ReportpointCSVArray.h"
#include "SalesHistoryReportPlu.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "SalesHistoryReportCustomer.h"
/**********************************************************************/

CAccountHistoryLineInfo::CAccountHistoryLineInfo()
{
	m_nLineType = 0;
	m_nSubType = 0;
	m_dValue = 0.0;
}

/**********************************************************************/

CCustomerHeaderInfo::CCustomerHeaderInfo()
{
	m_strAccount = "";
	m_strAddress = "";
	m_strTotalName = "";
	m_dTotalQty = 0.0;
	m_dTotalVal = 0.0;
	m_bDPQty = FALSE;
}

/**********************************************************************/

CSalesHistoryReportCustomer::CSalesHistoryReportCustomer()
{	
	m_ReportFilters.DisablePluFilter();

	m_bNewPagePerCustomer = TRUE;

	m_strWeekDays[0] = "Sun";
	m_strWeekDays[1] = "Mon";
	m_strWeekDays[2] = "Tue";
	m_strWeekDays[3] = "Wed";
	m_strWeekDays[4] = "Thu";
	m_strWeekDays[5] = "Fri";
	m_strWeekDays[6] = "Sat";
	
	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
	m_bDepartmentByItem = FALSE;

	m_nTaxFilter = TAXBAND_FILTER_ALL;

	m_bShowSales = TRUE;
	m_bShowRA = TRUE;
	m_bShowCharges = TRUE;
	m_bShowPayMeth = TRUE;
	m_bShowTax = TRUE;

	m_nConLevel = NODE_SYSTEM;
	m_nTableNo = 0;
	m_nDbIdx = 0;

	m_bItemRange = FALSE;
	m_nBlankLineCount = -1;
	m_nItemFrom = 0;
	m_nItemTo = 0;

	m_bNewPagePerCustomer = FALSE;
	m_bLastFieldIsCustomer = FALSE;
	m_bFirstCustomer = TRUE;
}

/**********************************************************************/

void CSalesHistoryReportCustomer::Reset( CSalesHistoryCSVRecordCustomer& ReportRecord, CLocationSelectorEntity& LocSelEntity )
{
	int nReportType = ReportRecord.GetReportType();
	if ( nReportType == SH_CUSTOM )
	{
		ReportRecord.GetReportFields( m_arrayReportFields );
		m_arrayReportFields.InsertAt( 0, SH_FIELD_CUSTOMER );
	}
	else
		m_arrayReportFields.RemoveAll();

	m_nReportType = ReportRecord.GetReportType();
	m_strReportName = ReportRecord.GetReportName();
	Reset ( LocSelEntity );
}

/**********************************************************************/

void CSalesHistoryReportCustomer::Reset( CLocationSelectorEntity& LocSelEntity )
{
	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );

	LocSelEntity.GetEPOSSelectArray( m_SelectArray );
	m_nConLevel = LocSelEntity.GetConType();
	m_nTableNo = LocSelEntity.GetEntityNo();
	m_nDbIdx = LocSelEntity.GetDbIdx();

	m_SelectArray.MakeList( TRUE );

	m_bItemRange = FALSE;
	m_nBlankLineCount = -1;
	m_nItemFrom = 0;
	m_nItemTo = 0;
	
	switch ( m_nReportType )
	{
	case SH_CUST_SUMMARY:
		m_arrayReportFields.Add( SH_FIELD_CUSTOMER );
		break;

	case SH_CUST_DETAIL:
		m_arrayReportFields.Add( SH_FIELD_CUSTOMER );
		m_arrayReportFields.Add( SH_FIELD_DATE_BUSINESS );
		m_arrayReportFields.Add( SH_FIELD_TIME );
		m_arrayReportFields.Add( SH_FIELD_TRANSERVER );
		break;
	
	default:
		break;
	}

	m_CustomerSalesTable.ClearConsolidatedLevels();
	m_CustomerSalesTable.ClearFieldStructure();
}

/**********************************************************************/

bool CSalesHistoryReportCustomer::PrepareFields()
{
	bool bGotPluField = FALSE;

	CReportConsolidationArray<CSortedStringItem> FieldsSoFar;
	for ( int nPos = 0; nPos < m_arrayReportFields.GetSize(); nPos++ )
	{		
		int nField = m_arrayReportFields.GetAt( nPos );
		int nFieldType = nField % 100;
		bool bAccept = FALSE;

		switch ( nFieldType )
		{
		case SH_FIELD_MONTH:
		case SH_FIELD_WEEK:
		case SH_FIELD_WEEKDAY:
		case SH_FIELD_DATE_BUSINESS:
		case SH_FIELD_TIME:
		case SH_FIELD_TERMINAL:
		case SH_FIELD_TRANSERVER:
		case SH_FIELD_CUSTOMER:
			{
				int nPos;
				CString strField;
				strField.Format ( "%3.3d", nFieldType );

				CSortedStringItem item;
				item.m_strItem = strField;

				if ( FieldsSoFar.Find( item, nPos ) == FALSE )
				{
					FieldsSoFar.Consolidate( item );
					bAccept = TRUE;
				}
			}
			break;
		}

		if ( bAccept == FALSE )
		{
			m_arrayReportFields.RemoveAt( nPos );
			nPos--;
		}
	}

	m_arrayTotalFlags.RemoveAll();

	//OVERALL REPORT WILL HAVE GRAND TOTAL BUT NO HEADER
	m_arrayTotalFlags.Add(1);
	
	int nFieldCount = m_arrayReportFields.GetSize();
	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		int nField = m_arrayReportFields.GetAt(nPos);
		int nFieldType = nField % 100;
		int nTotalType = nField - nFieldType;
		bool bHeader = ( nTotalType == SH_FIELD_HEADER || nTotalType == SH_FIELD_HEADER + SH_FIELD_TOTAL );
		bool bTotal = ( nTotalType == SH_FIELD_TOTAL || nTotalType == SH_FIELD_HEADER + SH_FIELD_TOTAL );
	
		m_arrayReportFields.SetAt( nPos, nFieldType );
		m_arrayTotalFlags.Add ( bTotal ? 1 : 0 );
	}

	m_arrayTotalFlags.SetAt( nFieldCount, 0 );

	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		switch( m_arrayReportFields.GetAt( nPos ) )
		{
		case SH_FIELD_CUSTOMER:
		case SH_FIELD_TIME:
		case SH_FIELD_TRANSERVER:
			m_CustomerSalesTable.AddField( SORT_TYPE_STRING );
			break;

		case SH_FIELD_WEEKDAY:
		case SH_FIELD_TERMINAL:
			m_CustomerSalesTable.AddField( SORT_TYPE_INT );
			break;

		case SH_FIELD_MONTH:
		case SH_FIELD_WEEK:
		case SH_FIELD_DATE_BUSINESS:
			m_CustomerSalesTable.AddField( SORT_TYPE_DATE );
			break;
		}
	}

	m_bLastFieldIsCustomer = FALSE;
	if ( nFieldCount > 0 )
	{
		if ( m_arrayReportFields.GetAt( nFieldCount - 1 ) == SH_FIELD_CUSTOMER )
			m_bLastFieldIsCustomer = TRUE;
	}

	return ( nFieldCount != 0 );
}

/**********************************************************************/

void CSalesHistoryReportCustomer::Consolidate() 
{
	dbDatabase.BuildSortIdxTables();
	dbLocation.BuildSortIdxTables();
	dbReportpoint.BuildSortIdxTables();
	
	if ( TRUE == m_bShowSales )
		ConsolidatePluSales();
	
	if ( ( TRUE == m_bShowRA ) || ( TRUE == m_bShowCharges ) || ( TRUE == m_bShowPayMeth ) )
		ConsolidateCustomerActivity();
}

/**********************************************************************/

void CSalesHistoryReportCustomer::ConsolidatePluSales()
{
	dbLocation.ResetRpNoReport();

	bool bWantPluInfo = FALSE;
	bool bWantCustomer = FALSE;
	bool bWantTranServer = FALSE;
	
	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		switch ( m_arrayReportFields.GetAt(n) )
		{
		case SH_FIELD_CUSTOMER:
			bWantCustomer = TRUE;
			break;

		case SH_FIELD_TRANSERVER:
			bWantTranServer = TRUE;
			break;
		}
	}

	if ( ( m_nTaxFilter != TAXBAND_FILTER_ALL ) || ( TRUE == m_bShowTax ) )
		bWantPluInfo = TRUE;
	
	if ( m_bDepartmentByItem == TRUE )
		bWantPluInfo = TRUE;

	m_CustomerSalesTable.ClearConsolidatedLevels();
	
	int nTotalFileSize;
	CArray<CTermFileListInfo,CTermFileListInfo> arrayFileInfo;
	GetFileListPluSales ( arrayFileInfo, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile = arrayFileInfo.GetAt( nFileIdx );

		CSalesHistoryDateInfo currentFileDate, currentFileDateOne, currentFileDateTwo;
		currentFileDate.m_Date = COleDateTime ( 
			atoi ( infoFile.m_strDateTran.Left(4) ), 
			atoi ( infoFile.m_strDateTran.Mid ( 4, 2 ) ), 
			atoi ( infoFile.m_strDateTran.Right ( 2 ) ), 
			0, 0, 0 );

		if ( currentFileDate.m_Date.GetStatus() != COleDateTime::valid )
			continue;

		currentFileDateOne.m_Date = currentFileDate.m_Date - COleDateTimeSpan( 1, 0, 0, 0 );
		currentFileDateTwo.m_Date = currentFileDate.m_Date - COleDateTimeSpan( 2, 0, 0, 0 );
		
		currentFileDate.SetDateInfoForReports();
		currentFileDateOne.SetDateInfoForReports();
		currentFileDateTwo.SetDateInfoForReports();

		int nDbNo = dbDatabase.GetDbNo( infoFile.m_nDbIdx );

		int nRpIdx = 0;

		if ( infoFile.m_nTermIdx != 0 )
		{
			int nRpNo = dbLocation.GetRpNoReport( infoFile.m_nLocIdx, infoFile.m_nTermIdx - 1 );
			
			if ( dbReportpoint.FindReportpointByNumber( nDbNo, nRpNo, nRpIdx ) == TRUE )
				nRpIdx = dbReportpoint.GetSortIdxFromRpIdx( nRpIdx ) + 1;
			else
				nRpIdx = 0;
		}

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CPluSalesLine csvSales ( infoFile.IsLegacySalesFolder() );

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( infoFile.m_nDbIdx );
		int nSortLocIdx = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
		
		int nTNo = 0;
		if ( infoFile.m_nTermIdx != 0 )
			nTNo = dbLocation.GetTNo( infoFile.m_nLocIdx, infoFile.m_nTermIdx - 1 );

		//THESE VALUES STAY THE SAME WITHIN EACH FILE
		for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
		{
			switch ( m_arrayReportFields.GetAt( n ) )
			{
			case SH_FIELD_TERMINAL:	m_CustomerSalesTable.SetBufferInt( n, nTNo );			break;
			}
		}
	
		//MAKE SURE THAT THE RIGHT DATABASE IS LOADED FOR TAX AND GROUP INFO
		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );
		
		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );

		int nLinesToRead;
		CString strBuffer;
		bool bGotSalesToday = FALSE;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				csvSales.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csvSales.GetLineType();
				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					break;

				case CSVPLUDATA_PLU_CHEAPESTFREE:
				default:
					continue;
				}

				//FILTER FOR POST TO ROOM
				//if ( SysInfo.IsPMSSystem() == TRUE )
				//	if ( ( FALSE == m_bGuestToTerminal ) && ( FALSE == bIsPMSLocation ) )
				//		if ( DataManager.Payment.IsPMSPaymentType( csvSales.GetPaymentNumber() ) )
				//			continue;

				//THROW OUT NON LEVEL 3 SALES FOR VIP MODE
				int nLevel = csvSales.GetPriceBand() + 1;
				if ( ( nLevel < 1 ) || ( nLevel > MAX_PLU_PRICES ) )
					nLevel = 1;

				//TRANSLATE CUSTOMER INTO CONSOLIDATION STRING
				int nCustomerID = csvSales.GetCustomerID();
				if ( 0 == nCustomerID )
					continue;

				bool bIncludeCustomer = FALSE;
				CString strCustomer = "";
				
				{
					CCustomerSelectionListAccountInfo infoAccount;
					infoAccount.m_nLocIdx = infoFile.m_nLocIdx;
					infoAccount.m_nCustomerID = nCustomerID;

					int nNodeType, nPos;
					DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoAccount, nNodeType, nPos );
					switch( nNodeType )
					{
					case NODE_CUSTOMER_ACCOUNT:
						strCustomer.Format ( "0%8.8d%8.8d", infoAccount.m_nSortIdx, nPos );
						bIncludeCustomer = infoAccount.m_bIsSelected & infoAccount.m_bIsInFilter;
						break;
					
					case NODE_CUSTOMER_ONEOFF:
						strCustomer.Format ( "1%8.8d%8.8d", infoAccount.m_nSortIdx, nPos );
						bIncludeCustomer = DataManagerNonDb.CustomerNameTable.Account.GetOneOffCustomerFlag();
						break;

					case NODE_PLACEHOLDER:
						strCustomer.Format ( "2%8.8d", nCustomerID );
						bIncludeCustomer = DataManagerNonDb.CustomerNameTable.Account.GetUnknownCustomerFlag();
						break;
					}
				}

				if ( FALSE == bIncludeCustomer )
					continue;

				//FILTER BY DATE AND TIME
				CSalesHistoryDateInfo infoLineDate;
				CString strTime = csvSales.GetTranTime();
				switch ( CheckConsolidationDateAndTime( currentFileDate, strTime ) )
				{
				case 1:		infoLineDate = currentFileDate;		break;
				case 2:		infoLineDate = currentFileDateOne;	break;
				case 3:		infoLineDate = currentFileDateTwo;	break;
				default:	continue;
				}

				//FILTER BY DEPT NO OF SALE
				int nDeptNoSale = csvSales.GetDeptNo();

				if ( m_bDepartmentByItem == FALSE )
					if ( m_ReportFilters.CheckDepartment( nDeptNoSale ) == FALSE )
						continue;

				__int64 nBasePluNo = 0;
				int nModifier = 0;
				double dMultiplier = 1.0;

				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					nBasePluNo = CEposReport::GetSeparatedPromoPluNo( csvSales.GetPromoNoSeparate() );
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					nBasePluNo = CEposReport::GetSeparatedMixMatchPluNo( csvSales.GetMixMatchNo() );
					break;

				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csvSales.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );
						
						if ( FALSE == infoPluNo.m_bValid )
							continue;

						nBasePluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;

						if ( nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( nModifier );
					}
					break;
				}

				//FILTER BY PLU RANGE
				if ( m_bItemRange == TRUE )
				{
					if ( ( m_nItemFrom != 0 ) && ( nBasePluNo < m_nItemFrom ) )
						continue;

					if ( ( m_nItemTo != 0 ) && ( nBasePluNo > m_nItemTo ) )
						continue;
				}

				//SORT BY TAX BAND
				int nTaxBand = 0;
				int nCategory = 0;
				int nDeptNoItem = 0;
				
				if ( bWantPluInfo == TRUE )
				{
					int nPluIdx;
					if ( DataManager.Plu.FindPluByNumber( nBasePluNo, nPluIdx ) == TRUE )
					{
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt( nPluIdx, PluRecord );
						nTaxBand = PluRecord.GetModifierTaxBandIntForReport(nModifier);
						nCategory = PluRecord.GetModifierAnalysisCategoryForReport(nModifier);
						nDeptNoItem = PluRecord.GetModifierDeptNoForReport(nModifier);
					}

					if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
						nTaxBand = 0;
					else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
						nTaxBand = csvSales.GetNumericTaxBand();

					//FILTER BY TAX BAND
					if ( m_nTaxFilter != TAXBAND_FILTER_ALL )
					{
						int nMask = 1;

						if ((nTaxBand >= 1) && (nTaxBand <= MAX_TAX_BANDS))
						{
							nMask <<= nTaxBand;
						}

						if ((m_nTaxFilter & nMask) == 0)
						{
							continue;
						}
					}
				}

				//FILTER BY DEPARTMENT NUMBER OF ITEM
				if ( m_bDepartmentByItem == TRUE )
					if ( m_ReportFilters.CheckDepartment( nDeptNoItem ) == FALSE )
						continue;
				
				//FILTER BY PRICE LEVEL
				if ( PRICELEVEL_FILTER_ALL != m_nPriceLevelFilter )
				{
					int nMask = 1;
						
					switch ( nLevel )
					{
					case 1:	nMask = 1;		break;
					case 2:	nMask = 2;		break;
					case 3:	nMask = 4;		break;
					case 4:	nMask = 8;		break;
					case 5:	nMask = 16;		break;
					case 6:	nMask = 32;		break;
					case 7:	nMask = 64;		break;
					case 8:	nMask = 128;	break;
					case 9:	nMask = 256;	break;
					case 10:nMask = 512;	break;
					}
					
					if ( ( m_nPriceLevelFilter & nMask ) == 0 )
						continue;
				}

				CString strTranServer = "";
				if ( TRUE == bWantTranServer )
				{
					int nTranServer = csvSales.GetTranServerNo();
					DataManagerNonDb.ServerNameTable.GetServerConsolidationString( infoFile.m_nDbIdx, infoFile.m_nLocIdx, nTranServer, strTranServer );
				}

				//THESE VALUES CHANGE ON A LINE-BY-LINE BASIS
				for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
				{
					switch ( m_arrayReportFields.GetAt( n ) )
					{
					case SH_FIELD_MONTH:		m_CustomerSalesTable.SetBufferDate( n, infoLineDate.m_DateMonth ); break;
					case SH_FIELD_WEEK:			m_CustomerSalesTable.SetBufferDate( n, infoLineDate.m_DateWeek ); break;
					case SH_FIELD_WEEKDAY:		m_CustomerSalesTable.SetBufferInt( n, infoLineDate.m_nWeekday );	break;
					case SH_FIELD_DATE_BUSINESS:m_CustomerSalesTable.SetBufferDate( n, infoLineDate.m_Date ); break;
					case SH_FIELD_TIME:			m_CustomerSalesTable.SetBufferString( n, strTime ); break;
					case SH_FIELD_TRANSERVER:	m_CustomerSalesTable.SetBufferString( n, strTranServer ); break;
					case SH_FIELD_CUSTOMER:		m_CustomerSalesTable.SetBufferString( n, strCustomer ); break;
					}
				}

				CEposReportConsolCustomerHistory infoSales;
				infoSales.m_dQty = 0.0;
				infoSales.m_dVal = 0.0;
				infoSales.m_nReportLineType = SALESHISTCUST_LINE_PLU;
				infoSales.m_nReportLineSubType = 0;

				CEposReportConsolCustomerHistory infoTax;
				infoTax.m_dQty = 0.0;
				infoTax.m_dVal = 0.0;
				infoTax.m_nReportLineType = SALESHISTCUST_LINE_TAX;
				infoTax.m_nReportLineSubType = 0;

				bool bConsolidateSales = FALSE;
				bool bConsolidateTax = FALSE;
				
				if ((csvSales.GetPluSaleWastageFlag() == FALSE) && (csvSales.GetPluSaleVoidFlag() == FALSE))
				{
					switch( nLineType )
					{
					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
						{
							double dDiscount = csvSales.GetValue();

							bConsolidateSales = TRUE;
							infoSales.m_dVal = -dDiscount;
							infoSales.m_nReportLineType = SALESHISTCUST_LINE_PLU;
							infoSales.m_nReportLineSubType = nBasePluNo;

							if ( TRUE == m_bShowTax )
							{
								bConsolidateTax = TRUE;
								double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nTaxDateCode, nTaxBand );
								infoTax.m_dVal = dDiscount - ReportHelpers.CalcNonTax ( dDiscount, dTaxRate );
								infoTax.m_dVal *= -1;
								infoTax.m_nReportLineSubType = nTaxBand;
							}
						}

						break;

					case CSVPLUDATA_PROMO_TAXABLE:
						{
							double dDiscount = csvSales.GetValue();

							bConsolidateSales = TRUE;
							infoSales.m_dVal = -dDiscount;
							infoSales.m_dQty = csvSales.GetSaleQty();
							infoSales.m_nReportLineType = SALESHISTCUST_LINE_PLU;
							infoSales.m_nReportLineSubType = nBasePluNo;

							if ( TRUE == m_bShowTax )
							{
								bConsolidateTax = TRUE;
								double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nTaxDateCode, nTaxBand );
								infoTax.m_dVal = dDiscount - ReportHelpers.CalcNonTax ( dDiscount, dTaxRate );
								infoTax.m_dVal *= -1;
								infoTax.m_nReportLineSubType = nTaxBand;
							}
						}
						break;

					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						bConsolidateSales = TRUE;
						infoSales.m_dVal = -csvSales.GetValue();
						infoSales.m_dQty = csvSales.GetSaleQty();
						infoSales.m_nReportLineType = SALESHISTCUST_LINE_PLU;
						infoSales.m_nReportLineSubType = nBasePluNo;
						break;

					case CSVPLUDATA_PLU_SALE:
						{
							double dSales = csvSales.GetValue();

							bConsolidateSales = TRUE;
							infoSales.m_dQty = csvSales.GetSaleQty() * dMultiplier;
							infoSales.m_dVal = dSales;
							infoSales.m_nReportLineType = SALESHISTCUST_LINE_PLU;
							infoSales.m_nReportLineSubType = nBasePluNo;

							if ( TRUE == m_bShowTax )
							{
								bConsolidateTax = TRUE;
								double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( nTaxDateCode, nTaxBand );
								infoTax.m_dVal = dSales - ( ReportHelpers.CalcNonTax ( dSales, dTaxRate ) );
								infoTax.m_nReportLineSubType = nTaxBand;
							}
						}
						break;
					}
				}

				if ( TRUE == bConsolidateSales )
				{
					m_CustomerSalesTable.Consolidate( infoSales, ( NODE_SYSTEM == m_nConLevel ) );
					bGotSalesToday = TRUE;
				}

				if ( TRUE == bConsolidateTax )
				{
					m_CustomerSalesTable.Consolidate( infoTax, ( NODE_SYSTEM == m_nConLevel ) );
					bGotSalesToday = TRUE;
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}
		nProgress += fileSales.GetLength();
		fileSales.Close();

		if ( bGotSalesToday == TRUE )
		{
			if ( ( infoFile.m_nSelectArrayIdx >= 0 ) && ( infoFile.m_nSelectArrayIdx < m_SelectArray.GetListSize() ) )
			{
				m_SelectArray.ProcessStartDate( infoFile.m_nSelectArrayIdx, currentFileDate.m_Date );
				m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, currentFileDate.m_Date );
			}
		}
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

void CSalesHistoryReportCustomer::ConsolidateCustomerActivity()
{
	bool bWantTranServer = FALSE;
	
	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		switch ( m_arrayReportFields.GetAt(n) )
		{
		case SH_FIELD_TRANSERVER:
			bWantTranServer = TRUE;
			break;
		}
	}

	int nTotalFileSize;
	CArray<CTermFileListInfo, CTermFileListInfo> arrayFileInfo;
	GetFileListCustomer ( arrayFileInfo, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Checking customer transactions" );
	
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile = arrayFileInfo.GetAt( nFileIdx );

		CSalesHistoryDateInfo currentFileDate, currentFileDateOne, currentFileDateTwo;
		currentFileDate.m_Date = COleDateTime ( 
			atoi ( infoFile.m_strDateTran.Left(4) ), 
			atoi ( infoFile.m_strDateTran.Mid ( 4, 2 ) ), 
			atoi ( infoFile.m_strDateTran.Right ( 2 ) ), 
			0, 0, 0 );

		if ( currentFileDate.m_Date.GetStatus() != COleDateTime::valid )
			continue;

		currentFileDateOne.m_Date = currentFileDate.m_Date - COleDateTimeSpan( 1, 0, 0, 0 );
		currentFileDateTwo.m_Date = currentFileDate.m_Date - COleDateTimeSpan( 2, 0, 0, 0 );
		
		currentFileDate.SetDateInfoForReports();
		currentFileDateOne.SetDateInfoForReports();
		currentFileDateTwo.SetDateInfoForReports();

		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			continue;

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( nDbIdx );
		int nSortLocIdx = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
		
		int nTNo = 0;
		if ( infoFile.m_nTermIdx != 0 )
			nTNo = dbLocation.GetTNo( infoFile.m_nLocIdx, infoFile.m_nTermIdx - 1 );

		//THESE VALUES STAY THE SAME WITHIN EACH FILE
		for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
		{
			switch ( m_arrayReportFields.GetAt( n ) )
			{
			case SH_FIELD_TERMINAL:	m_CustomerSalesTable.SetBufferInt( n, nTNo );			break;
			}
		}

		bool bGotSalesToday = FALSE;
		bool bValidTime = FALSE;
		CSalesHistoryDateInfo infoLineDate;
		CString strTime = "";
		CString strTranServer = "";

		int nCustomerID = 0;
		CArray<CAccountHistoryLineInfo,CAccountHistoryLineInfo> arrayBuffer;
			
		CString strBuffer;
		while( fileSales.ReadString( strBuffer ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVAccountHistory csvIn ( strBuffer );

			int nAction = -1;
			double dLineValue = 0.0;

			switch( csvIn.GetAccountLineType() )
			{
			case ACCOUNT_LINETYPE_HEADER:
				{
					if ( TRUE == bValidTime )
					{
						if ( ConsolidateCustomerActivityBuffer( nCustomerID, strTranServer, infoFile, infoLineDate, strTime, arrayBuffer ) == TRUE )
						{
							if ( ( infoFile.m_nSelectArrayIdx >= 0 ) && ( infoFile.m_nSelectArrayIdx < m_SelectArray.GetListSize() ) )
							{
								m_SelectArray.ProcessStartDate( infoFile.m_nSelectArrayIdx, currentFileDate.m_Date );
								m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, currentFileDate.m_Date );
							}
						}
					}
						
					arrayBuffer.RemoveAll();
					nCustomerID = 0;

					bValidTime = FALSE;
					strTime = csvIn.GetHeaderLineTime();
					switch ( CheckConsolidationDateAndTime( currentFileDate, strTime ) )
					{
					case 1:		infoLineDate = currentFileDate;			bValidTime = TRUE;	break;
					case 2:		infoLineDate = currentFileDateOne;		bValidTime = TRUE;	break;
					case 3:		infoLineDate = currentFileDateTwo;		bValidTime = TRUE;	break;
					default:	continue;
					}

					strTranServer = "";
					if ( ( TRUE == bWantTranServer ) && ( TRUE == bValidTime ) )
					{
						int nTranServer = csvIn.GetHeaderLineServerNo();
						DataManagerNonDb.ServerNameTable.GetServerConsolidationString( nDbIdx, infoFile.m_nLocIdx, nTranServer, strTranServer );
					}
				}
				break;

			case ACCOUNT_LINETYPE_MARKER:
				if ( csvIn.GetMarkerType() == CASHRSP_ITEMPAYTYPE_CUSTOMER )
				{
					if ( 0 == nCustomerID )
						nCustomerID = csvIn.GetMarkerID();
				}
				break;

			case ACCOUNT_LINETYPE_RECEIVE:
				if ( ( TRUE == bValidTime ) && ( TRUE == m_bShowRA ) && ( csvIn.GetReceiveType() == CASHRSP_ITEMPAYTYPE_CUSTOMER ) )
				{
					if ( 0 == nCustomerID ) 
						nCustomerID = csvIn.GetReceiveID();
					
					CAccountHistoryLineInfo info;
					info.m_nLineType = ACCOUNT_LINETYPE_RECEIVE;
					info.m_nSubType = csvIn.GetReceiveType();
					info.m_dValue = csvIn.GetReceiveValue();
					arrayBuffer.Add( info );
				}
				break;

			case ACCOUNT_LINETYPE_CHARGE:
				if ( ( bValidTime ) && ( m_bShowCharges | m_bShowPayMeth ) && ( csvIn.GetChargeType() == CASHRSP_ITEMPAYTYPE_CUSTOMER ) )
				{
					if ( 0 == nCustomerID ) 
						nCustomerID = csvIn.GetChargeID();

					if ( TRUE == m_bShowCharges )
					{
						CAccountHistoryLineInfo info;
						info.m_nLineType = ACCOUNT_LINETYPE_CHARGE;
						info.m_nSubType = csvIn.GetChargeType();
						info.m_dValue = csvIn.GetChargeTender();
						arrayBuffer.Add( info );
					}
				}
				break;

			case ACCOUNT_LINETYPE_PAYMENT_RA:
			case ACCOUNT_LINETYPE_PAYMENT_MIXED:
				if ( ( TRUE == bValidTime ) && ( TRUE == m_bShowPayMeth ) )
				{
					CAccountHistoryLineInfo info;
					info.m_nLineType = ACCOUNT_LINETYPE_PAYMENT_MIXED;
					info.m_nSubType = csvIn.GetPaymentNumber();
					info.m_dValue = csvIn.GetPaymentAmount();
					arrayBuffer.Add( info );
				}
				break;
			}
		}

		if ( TRUE == bValidTime )
		{
			if ( ConsolidateCustomerActivityBuffer( nCustomerID, strTranServer, infoFile, infoLineDate, strTime, arrayBuffer ) == TRUE )
			{
				if ( ( infoFile.m_nSelectArrayIdx >= 0 ) && ( infoFile.m_nSelectArrayIdx < m_SelectArray.GetListSize() ) )
				{
					m_SelectArray.ProcessStartDate( infoFile.m_nSelectArrayIdx, currentFileDate.m_Date );
					m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, currentFileDate.m_Date );
				}
			}
		}
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CSalesHistoryReportCustomer::ConsolidateCustomerActivityBuffer( int nCustomerID, CString& strServer, CTermFileListInfo& infoFile, CSalesHistoryDateInfo& infoDate, CString& strTime, CArray<CAccountHistoryLineInfo,CAccountHistoryLineInfo>& arrayBuffer )
{
	if ( 0 == nCustomerID )
		return FALSE;

	CString strCustomer = "";
	bool bIncludeCustomer = FALSE;
	
	{
		CCustomerSelectionListAccountInfo infoAccount;
		infoAccount.m_nLocIdx = infoFile.m_nLocIdx;
		infoAccount.m_nCustomerID = nCustomerID;

		int nNodeType, nPos;
		DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoAccount, nNodeType, nPos );
		switch( nNodeType )
		{
		case NODE_CUSTOMER_ACCOUNT:
			strCustomer.Format ( "0%8.8d%8.8d", infoAccount.m_nSortIdx, nPos );
			bIncludeCustomer = infoAccount.m_bIsSelected & infoAccount.m_bIsInFilter;
			break;

		case NODE_CUSTOMER_ONEOFF:
			strCustomer.Format ( "1%8.8d%8.8d", infoAccount.m_nSortIdx, nPos );
			bIncludeCustomer = DataManagerNonDb.CustomerNameTable.Account.GetOneOffCustomerFlag();
			break;

		case NODE_PLACEHOLDER:
		default:
			strCustomer.Format ( "2%8.8d", nCustomerID );
			bIncludeCustomer = DataManagerNonDb.CustomerNameTable.Account.GetUnknownCustomerFlag();
			break;
		}
	}

	if ( FALSE == bIncludeCustomer )
		return FALSE;

	double dPaymentValue = 0.0;
	double dChargeValue = 0.0;
	double dReceiveValue = 0.0;
	bool bGotSalesToday = FALSE;

	for ( int n = 0; n < arrayBuffer.GetSize(); n++ )
	{
		CAccountHistoryLineInfo infoLine = arrayBuffer.GetAt(n);

		switch( infoLine.m_nLineType )
		{
		case ACCOUNT_LINETYPE_CHARGE:			dChargeValue += infoLine.m_dValue;		break;
		case ACCOUNT_LINETYPE_PAYMENT_MIXED:	dPaymentValue += infoLine.m_dValue;		break;
		case ACCOUNT_LINETYPE_RECEIVE:			dReceiveValue += infoLine.m_dValue;		break;
		}
	}
	
	//THESE VALUES CHANGE ON A LINE-BY-LINE BASIS
	for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
	{
		switch ( m_arrayReportFields.GetAt( n ) )
		{
		case SH_FIELD_MONTH:		m_CustomerSalesTable.SetBufferDate( n, infoDate.m_DateMonth ); break;
		case SH_FIELD_WEEK:			m_CustomerSalesTable.SetBufferDate( n, infoDate.m_DateWeek ); break;
		case SH_FIELD_WEEKDAY:		m_CustomerSalesTable.SetBufferInt( n, infoDate.m_nWeekday );	break;
		case SH_FIELD_DATE_BUSINESS:m_CustomerSalesTable.SetBufferDate( n, infoDate.m_Date ); break;
		case SH_FIELD_TIME:			m_CustomerSalesTable.SetBufferString( n, strTime ); break;
		case SH_FIELD_TRANSERVER:	m_CustomerSalesTable.SetBufferString( n, strServer ); break;
		case SH_FIELD_CUSTOMER:		m_CustomerSalesTable.SetBufferString( n, strCustomer ); break;
		}
	}

	if ( dChargeValue != 0.0 )
	{
		CEposReportConsolCustomerHistory infoSales;
		infoSales.m_dQty = 1.0;
		infoSales.m_dVal = dChargeValue;
		infoSales.m_nReportLineType = SALESHISTCUST_LINE_CHARGE;
		infoSales.m_nReportLineSubType = 1;

		m_CustomerSalesTable.Consolidate( infoSales, FALSE );
		bGotSalesToday = TRUE;
	}

	if ( dReceiveValue != 0.0 )
	{
		CEposReportConsolCustomerHistory infoSales;
		infoSales.m_dQty = 1.0;
		infoSales.m_dVal = dReceiveValue;
		infoSales.m_nReportLineType = SALESHISTCUST_LINE_RECEIVE;
		infoSales.m_nReportLineSubType = 1;

		m_CustomerSalesTable.Consolidate( infoSales, FALSE );
		bGotSalesToday = TRUE;
	}

	if ( dPaymentValue != 0.0 )
	{
		for ( int n = 0; n < arrayBuffer.GetSize(); n++ )
		{
			CAccountHistoryLineInfo infoLine = arrayBuffer.GetAt(n);

			if ( ACCOUNT_LINETYPE_PAYMENT_MIXED == infoLine.m_nLineType )
			{
				CEposReportConsolCustomerHistory infoSales;
				infoSales.m_dQty = 1.0;
				infoSales.m_dVal = infoLine.m_dValue;
				infoSales.m_nReportLineType = SALESHISTCUST_LINE_PAYMENT;
				infoSales.m_nReportLineSubType = infoLine.m_nSubType;

				m_CustomerSalesTable.Consolidate( infoSales, ( NODE_SYSTEM == m_nConLevel ) );
				bGotSalesToday = TRUE;
			}
		}
	}
	
	return bGotSalesToday;
}

/**********************************************************************/

int CSalesHistoryReportCustomer::CheckConsolidationDateAndTime( CSalesHistoryDateInfo& infoDate, CString& strTime )
{			
	CString strSaleTime = strTime;
	
	switch( strSaleTime.GetLength() )
	{
	case 6:		
		break;
	
	case 4:		
		strSaleTime += "00";		
		break;

	default:	
		strSaleTime = "000000";		
		break;
	}
			
	int	nHour = atoi( strSaleTime.Left(2) );
	int	nMinute = atoi ( strSaleTime.Mid(2,2) );
	int	nSecond = atoi ( strSaleTime.Right(2) );
				
	//ROUND TO TIME SLICE IF REQUIRED
	switch( m_TimeSliceMap.GetReportType() )
	{
	case SH_TIMESLICE_SECOND:
		break;

	case SH_TIMESLICE_MINUTE:
		nSecond = 0;
		break;

	case SH_TIMESLICE_5MIN:
		nSecond = 0;
		nMinute -= ( nMinute % 5 );
		break;

	case SH_TIMESLICE_10MIN:
		nSecond = 0;
		nMinute -= ( nMinute % 10 );
		break;

	case SH_TIMESLICE_15MIN:
		nSecond = 0;
		nMinute -= ( nMinute % 15 );
		break;

	case SH_TIMESLICE_30MIN:
		nSecond = 0;
		nMinute -= ( nMinute % 30 );
		break;

	case SH_TIMESLICE_60MIN:
		nSecond = 0;
		nMinute = 0;
		break;
	}

	//CHECK FOR PREVIOUS DATE
	CString strDate;
	strDate.Format( "%4.4d%2.2d%2.2d",
		infoDate.m_Date.GetYear(),
		infoDate.m_Date.GetMonth(),
		infoDate.m_Date.GetDay() );

	int nResult = ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strDate, strSaleTime ) );
	
	if ( ( nResult < 1 ) || ( nResult > 3 ) ) 
	{
		strTime = "";
		nResult = 0;
	}
	else
	{
		if ( m_TimeSliceMap.GetReportType() == SH_CUSTOM )
			strTime = m_TimeMap.GetSortOrder( nHour, nMinute );
		else
		{
			strTime.Format( "%1.1d%2.2d%2.2d%2.2d",
				nResult - 1,
				nHour,
				nMinute,
				nSecond );
		}
	}

	return nResult;
}

/**********************************************************************/

void CSalesHistoryReportCustomer::WriteReportLine( const char* szLine )
{
	CString strLine = szLine;
	if ( strLine == "" )
		return;

	m_ReportFile.WriteLine( strLine );
	m_nBlankLineCount = 0;
}

/**********************************************************************/

void CSalesHistoryReportCustomer::WriteBlankLines( int nLineCount )
{
	if ( m_nBlankLineCount >= 0 )
	{
		while( m_nBlankLineCount < nLineCount )
		{
			m_ReportFile.WriteLine( "" );
			m_nBlankLineCount++;
		}
	}
}

/**********************************************************************/

bool CSalesHistoryReportCustomer::CreateReport()
{
	m_bFirstCustomer = TRUE;
	m_nBlankLineCount = -1;

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );
	
	StatusProgress.Lock( TRUE, "Creating report" );
	bool bResult = CreateCustomReportInternal();			
	StatusProgress.Unlock();
	
	return bResult;
}

/**********************************************************************/

bool CSalesHistoryReportCustomer::CreateCustomReportInternal()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	bool bGotData = FALSE;

	CString strTitle = m_strReportName;
	::TrimSpaces( strTitle, FALSE );

	if ( strTitle == "" ) 
		strTitle = "Customer Sales History";

	m_ReportFile.SetStyle1 ( GetReportTitle( strTitle ) );
	
	m_ReportFile.AddColumn ( "", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 600 );
	m_ReportFile.AddColumn ( "", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Sales", TA_RIGHT, 200 );
	
	m_nProgress = 0;
	m_nLabelTaxBand = -1;

	CEposReportConsolCustomerHistory infoConsol;
	infoConsol.m_nTransactionNo = 0;
	ProcessCustomerSalesArray( 0, 0, infoConsol );

	m_ReportFile.WriteLine( "" );
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/

void CSalesHistoryReportCustomer::ProcessCustomerSalesArray(int nArray, int nDepth, CEposReportConsolCustomerHistory& infoConsol )
{
	int nFieldType = m_arrayReportFields.GetAt( nDepth );	
	bool bTotal = ( m_arrayTotalFlags.GetAt(nDepth) == 1 );
	
	int nArraySize = m_CustomerSalesTable.GetArraySize(nArray);
	if ( nArraySize == 0 )
		return;

	for ( int nNodeIdx = 0; nNodeIdx < nArraySize; nNodeIdx++ )
	{
		StatusProgress.SetPos( m_nProgress++, m_CustomerSalesTable.GetNodeCount() );

		int nLineDbNo = -1;

		int nNextArrayIdx = m_CustomerSalesTable.GetNextArrayIdx( nArray, nNodeIdx );

		CEposReportConsolCustomerHistory infoNode;
		m_CustomerSalesTable.GetSalesTotalForNode( nArray, nNodeIdx, infoNode );
		
		switch ( nFieldType )
		{
		case SH_FIELD_MONTH:
			m_strLabelMonth = GetRepMonth( m_CustomerSalesTable.GetLabelDate( nArray, nNodeIdx ), FALSE, FALSE );	
			break;

		case SH_FIELD_WEEK:
			{	
				COleDateTime oleWeekStart = m_CustomerSalesTable.GetLabelDate( nArray, nNodeIdx );
				m_strLabelWeek = oleWeekStart.Format ( "%d/%m/%Y" );
				
				CDateRangeHelpers DateRangeHelpers;
				m_nLabelWeekNo = DateRangeHelpers.GetWeekNo( oleWeekStart );
			}
			break;

		case SH_FIELD_DATE_BUSINESS:
			{	
				COleDateTime oleDate = m_CustomerSalesTable.GetLabelDate( nArray, nNodeIdx );
				m_strLabelDate = oleDate.Format ( "%d/%m/%Y" );
			}
			break;
	
		case SH_FIELD_TIME:
			{	
				CString strLabel = m_CustomerSalesTable.GetLabelString( nArray, nNodeIdx );

				if ( m_TimeSliceMap.GetReportType() != SH_CUSTOM )
				{
					m_nLabelTimeHour = atoi( strLabel.Mid(1,2) );
					m_nLabelTimeMinute = atoi( strLabel.Mid(3,2) );
					m_nLabelTimeSecond = atoi( strLabel.Right(2) );
					m_nLabelTimeStars = atoi( strLabel.Left(1) );
				}
				else
					m_nLabelTimeHour = atoi( strLabel );
			}
			break;
	
		case SH_FIELD_WEEKDAY:
			{
				int nWeekDay = m_CustomerSalesTable.GetLabelInt( nArray, nNodeIdx );
				nWeekDay += EcrmanOptions.GetReportsWeekStartDay();
				nWeekDay %= 7;
				m_strLabelWeekDay = m_strWeekDays [ nWeekDay ];
			}
			break;

		case SH_FIELD_TERMINAL:
			m_nLabelTerminal = m_CustomerSalesTable.GetLabelInt( nArray, nNodeIdx );
			break;

		case SH_FIELD_TRANSERVER:
			{
				CString strLabel = m_CustomerSalesTable.GetLabelString( nArray, nNodeIdx );
				DataManagerNonDb.ServerNameTable.ProcessServerLabel( strLabel, m_nLabelTranServerNo, m_strLabelTranServerName );
			}
			break;

		case SH_FIELD_CUSTOMER:
			{
				CString strLabel = m_CustomerSalesTable.GetLabelString( nArray, nNodeIdx );
				int nLen = strLabel.GetLength();

				bool bOneOff = FALSE;
				CCustomerSelectionListAccountInfo infoCustomer;
					
				if ( ( 17 == nLen ) && ( strLabel.Left(1) == "0" ) )
				{
					int nIdx = atoi( strLabel.Right(8) );
					DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( nIdx, infoCustomer );
					m_strLabelCustomerName = infoCustomer.GetReportName();
				}
				else if ( ( 17 == nLen ) && ( strLabel.Left(1) == "1" ) )
				{
					int nIdx = atoi( strLabel.Right(8) );
					DataManagerNonDb.CustomerNameTable.OneOff.GetCustomerStatus( nIdx, infoCustomer );
					m_strLabelCustomerName = infoCustomer.GetReportName();
					bOneOff = TRUE;
				}
				else if ( ( 9 == nLen ) && ( strLabel.Left(1) == "2" ) )
				{
					int nID = atoi( strLabel.Right(8) );
					infoCustomer.m_nCustomerID = nID;
					m_strLabelCustomerName.Format( "Customer %d", nID );
					infoCustomer.m_strName1 = m_strLabelCustomerName;
					infoCustomer.m_strName2 = "";
				}
				else
				{
					m_strLabelCustomerName = "Unknown Customer";
					infoCustomer.m_strName1 = m_strLabelCustomerName;
					infoCustomer.m_strName2 = "";
				}

				WriteCustomerHeader( infoCustomer, infoNode, bOneOff );

				nLineDbNo = dbLocation.GetDbNo( infoCustomer.m_nLocIdx );
			}
			break;
		}

		if ( (nLineDbNo != -1) && ( nLineDbNo != dbDatabase.GetDbNo( DataManager.GetActiveDbIdx() ) ) )
		{
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( nLineDbNo, nDbIdx ) == TRUE )
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			}
		}

		if ( nDepth == m_arrayReportFields.GetSize() - 1 )
		{
			if ( FALSE == m_bLastFieldIsCustomer )
				WriteHeader( nDepth + 1 );

			CreateReportLine( infoNode );		
		}
		else
			ProcessCustomerSalesArray( nNextArrayIdx, nDepth + 1, infoNode );
	}

	if ( bTotal == TRUE )
	{
		if ( ( 0 == nDepth ) &&( TRUE == m_bNewPagePerCustomer ) )
			m_ReportFile.WriteLine ( "<NEWPAGE>" );

		WriteTotals( nDepth, infoConsol );
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomer::CreateReportLine( CEposReportConsolCustomerHistory& infoNode )
{
	int nStart, nEnd;
	m_CustomerSalesTable.FindTransactionItemLines( infoNode.m_nTransactionNo, nStart, nEnd );
	WriteTransactionItems( nStart, nEnd, FALSE );
	WriteBlankLines( 1 );
}

/**********************************************************************/

const char* CSalesHistoryReportCustomer::GetHeaderLine( int nDepth )
{
	CString strSpace = " ";
	CCSV csvHeader;
	
	for ( int nPos = 0; nPos < nDepth; nPos++ )
	{
		int nFieldType = m_arrayReportFields.GetAt( nPos );
		switch ( nFieldType )
		{
		case SH_FIELD_MONTH:
			csvHeader.Add ( strSpace + m_strLabelMonth );
			break;

		case SH_FIELD_WEEK:
			{
				CString strWeekNo;
				strWeekNo.Format ( "Week %d", m_nLabelWeekNo );
				csvHeader.Add ( strSpace + strWeekNo );
				csvHeader.Add ( strSpace + m_strLabelWeek );
			}
			break;

		case SH_FIELD_DATE_BUSINESS:
			csvHeader.Add ( strSpace + m_strLabelDate );
			break;

		case SH_FIELD_TIME:
			csvHeader.Add ( strSpace + GetLabelTimeString() );
			break;

		case SH_FIELD_WEEKDAY:
			csvHeader.Add ( strSpace + m_strLabelWeekDay );
			break;

		case SH_FIELD_TERMINAL:
			csvHeader.Add( strSpace + dbLocation.GetTerminalName( m_nLabelLocIdx, m_nLabelTerminal ) );
			break;

		case SH_FIELD_TRANSERVER:	
			{
				CString strServer;
				strServer.Format( " Server %d : %s",	
					m_nLabelTranServerNo,
					(const char*) m_strLabelTranServerName );

				csvHeader.Add( strServer );
			}
			break;

		case SH_FIELD_CUSTOMER:
			csvHeader.Add ( strSpace + m_strLabelCustomerName );
			break;
		}
	}
	m_strHeader = csvHeader.GetLine();
	return m_strHeader;
}

/**********************************************************************/

void CSalesHistoryReportCustomer::WriteTotals( int nDepth, CEposReportConsolCustomerHistory& infoConsol )
{
	WriteBlankLines( 2 );

	CString strTotal;
	if ( nDepth == 0 )
	{
		WriteReportLine( "<..>Grand Total" );
		WriteReportLine( "<LI>" );
	}
	else
		WriteHeader( nDepth );
	
	int nStart, nEnd;
	m_CustomerSalesTable.FindTransactionItemLines( infoConsol.m_nTransactionNo, nStart, nEnd );
	WriteTransactionItems( nStart, nEnd, ( 0 == nDepth ) );
	WriteBlankLines( 1 );
}

/**********************************************************************/

void CSalesHistoryReportCustomer::WriteHeader( int nDepth )
{
	if ( nDepth > 0 )
	{
		CString strTemp = GetHeaderLine(nDepth);
		if ( strTemp.GetLength() >= 1 ) strTemp = strTemp.Right( strTemp.GetLength() - 1 );
		CString strHeader = "<..>";
		strHeader += strTemp;
		WriteBlankLines( 1 );
		WriteReportLine( strHeader );
	}
	WriteReportLine( "<LI>" );
}

/**********************************************************************/

void CSalesHistoryReportCustomer::WriteTransactionItems( int nStart, int nEnd, bool bGrandTotal )
{
	int nLastType = -1;

	if ( ( TRUE == bGrandTotal ) && ( NODE_SYSTEM == m_nConLevel ) )
	{
		double dSalesTotal = 0.0;

		bool bGotTax = FALSE;
		for ( int n = nStart; n <= nEnd; n++ )
		{
			CEposReportConsolCustomerHistory infoConsol;
			m_CustomerSalesTable.GetSalesTotalForLine( n, infoConsol );

			if ( SALESHISTCUST_LINE_TAX == infoConsol.m_nReportLineType )
			{
				bGotTax = TRUE;
				break;
			}
		}

		for ( int n = nStart; n <= nEnd; n++ )
		{
			CEposReportConsolCustomerHistory infoConsol;
			m_CustomerSalesTable.GetSalesTotalForLine( n, infoConsol );

			switch( infoConsol.m_nReportLineType )
			{
			case SALESHISTCUST_LINE_PLU:
				{
					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "" );
					csvOut.Add ( bGotTax ? "Gross Sales Total" : "Sales Total" );
					csvOut.Add ( infoConsol.m_dQty, SysInfo.GetDPQty() );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					dSalesTotal = infoConsol.m_dVal;
					nLastType = SALESHISTCUST_LINE_PLU;
				}
				break;

			case SALESHISTCUST_LINE_CHARGE:
				{
					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "" );
					csvOut.Add ( "Charge Total" );
					csvOut.Add ( infoConsol.m_dQty, 0 );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					nLastType = SALESHISTCUST_LINE_CHARGE;
				}
				break;

			case SALESHISTCUST_LINE_RECEIVE:
				{
					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "" );
					csvOut.Add ( "RA Total" );
					csvOut.Add ( infoConsol.m_dQty, 0 );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					nLastType = SALESHISTCUST_LINE_RECEIVE;
				}
				break;

			case SALESHISTCUST_LINE_TAX:
				{
					CString strTaxTotal = EcrmanOptions.GetSalesTaxName();
					strTaxTotal += " Total";

					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "" );
					csvOut.Add ( strTaxTotal );
					csvOut.Add ( "" );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );

					csvOut.RemoveAll();	
					csvOut.Add ( "" );
					csvOut.Add ( "" );
					csvOut.Add ( "Net Sales Total" );
					csvOut.Add ( "" );
					csvOut.Add ( dSalesTotal - infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );

					nLastType = SALESHISTCUST_LINE_TAX;
				}
				break;

			case SALESHISTCUST_LINE_PAYMENT:
				{
					WriteBlankLines( 1 );

					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "" );
					csvOut.Add ( "Payment Total" );
					csvOut.Add ( infoConsol.m_dQty, 0 );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					nLastType = SALESHISTCUST_LINE_PAYMENT;
				}
				break;

			default:
				break;
			}
		}
	}
	else
	{
		int nTaxLines = 0;
		double dSalesTotal = 0.0;
		double dTaxTotal = 0.0;

		for ( int n = nStart; n <= nEnd; n++ )
		{
			CEposReportConsolCustomerHistory infoConsol;
			m_CustomerSalesTable.GetSalesTotalForLine( n, infoConsol );

			switch( infoConsol.m_nReportLineType )
			{
			case SALESHISTCUST_LINE_PLU:
				{
					__int64 nPluNo = infoConsol.m_nReportLineSubType;

					CString strDisplayPluNo = "";
					strDisplayPluNo.Format ( "Plu %s", ReportHelpers.GetDisplayPluNo ( nPluNo ), TRUE );
						
					int nPluIdx;
					CString strDescription = "Unknown";
					if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
					{
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt ( nPluIdx, PluRecord );
						strDescription = ReportHelpers.GetReportText ( PluRecord );
					}
				
					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( strDisplayPluNo );
					csvOut.Add ( strDescription );
					csvOut.Add ( infoConsol.m_dQty, SysInfo.GetDPQty() );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					nLastType = SALESHISTCUST_LINE_PLU;

					dSalesTotal += infoConsol.m_dVal;
				}
				break;

			case SALESHISTCUST_LINE_CHARGE:
				if ( FALSE == m_bShowPayMeth )
				{
					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "Charge to Account" );
					csvOut.Add ( "" );
					csvOut.Add ( infoConsol.m_dQty, 0 );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					nLastType = SALESHISTCUST_LINE_CHARGE;
				}
				break;

			case SALESHISTCUST_LINE_RECEIVE:
				{
					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "Received on Account" );
					csvOut.Add ( "" );
					csvOut.Add ( infoConsol.m_dQty, 0 );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					nLastType = SALESHISTCUST_LINE_RECEIVE;
				}
				break;

			case SALESHISTCUST_LINE_TAX:
				{
					if ( nLastType != SALESHISTCUST_LINE_TAX )
					{
						WriteBlankLines( 1 );

						CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
						csvOut.Add ( "" );
						csvOut.Add ( "" );
						csvOut.Add ( "Gross Sales" );
						csvOut.Add ( "" );
						csvOut.Add ( dSalesTotal, SysInfo.GetDPValue() );
						WriteReportLine ( csvOut.GetLine() );
						WriteReportLine ( "\t\t\t\t<LI>" );
					}

					int nTaxBand = (int) infoConsol.m_nReportLineSubType;

					CTaxRateInfo TaxRateInfo;
					DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );
						
					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( EcrmanOptions.GetTaxString( "%T Amount" ) );
					csvOut.Add ( TaxRateInfo.m_strReportText );
					csvOut.Add ( "" );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					
					nTaxLines++;
					dTaxTotal += infoConsol.m_dVal;
					nLastType = SALESHISTCUST_LINE_TAX;
				}
				break;

			case SALESHISTCUST_LINE_PAYMENT:
				{
					if ( nLastType == SALESHISTCUST_LINE_TAX )
						FinishTaxSection( nTaxLines, dSalesTotal, dTaxTotal );

					if ( nLastType != SALESHISTCUST_LINE_PAYMENT )
						WriteBlankLines( 1 );

					int nPaymentNo = (int) infoConsol.m_nReportLineSubType;
					
					CString strPayment = "";
					switch( nPaymentNo )
					{
					case 998:
					case 999:
						strPayment.Format( "Unknown (%d)", nPaymentNo );
						break;

					default:
						strPayment = DataManager.Payment.GetDisplayName( (int) infoConsol.m_nReportLineSubType );
						break;
					}

					CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
					csvOut.Add ( "" );
					csvOut.Add ( "" );
					csvOut.Add ( strPayment );
					csvOut.Add ( infoConsol.m_dQty, 0 );
					csvOut.Add ( infoConsol.m_dVal, SysInfo.GetDPValue() );
					WriteReportLine ( csvOut.GetLine() );
					nLastType = SALESHISTCUST_LINE_PAYMENT;
				}
				break;

			default:
				break;
			}
		}

		if ( nLastType == SALESHISTCUST_LINE_TAX )
			FinishTaxSection( nTaxLines, dSalesTotal, dTaxTotal );
	}
}

/**********************************************************************/

void CSalesHistoryReportCustomer::FinishTaxSection( int nTaxLines, double dSalesTotal, double dTaxTotal )
{
	if ( nTaxLines > 1 )
	{
		CString strTotal = EcrmanOptions.GetSalesTaxName();
		strTotal += " Total";

		CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
		csvOut.Add ( "" );
		csvOut.Add ( "" );
		csvOut.Add ( strTotal );
		csvOut.Add ( "" );
		csvOut.Add ( dTaxTotal, SysInfo.GetDPValue() );
		WriteReportLine ( "\t\t\t\t<LI>" );
		WriteReportLine ( csvOut.GetLine() );
	}

	WriteReportLine ( "\t\t\t\t<LI>" );
	CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
	csvOut.Add ( "" );
	csvOut.Add ( "" );
	csvOut.Add ( "Net Sales" );
	csvOut.Add ( "" );
	csvOut.Add ( dSalesTotal - dTaxTotal, SysInfo.GetDPValue() );
	WriteReportLine ( csvOut.GetLine() );
}

/**********************************************************************/

void CSalesHistoryReportCustomer::WriteCustomerHeader( CCustomerSelectionListAccountInfo& infoCustomer, CEposReportConsolCustomerHistory& infoNode, bool bOneOff )
{
	if ( FALSE == m_bFirstCustomer )
	{
		if ( TRUE == m_bNewPagePerCustomer )
			m_ReportFile.WriteLine ( "<NEWPAGE>" );
		else
			m_ReportFile.WriteLine ( "" );
	}

	m_bFirstCustomer = FALSE;

	int nStart, nEnd;
	m_CustomerSalesTable.FindTransactionBlockLines( infoNode.m_nTransactionNo, nStart, nEnd );

	double dSaleQty = 0.0;
	double dSaleVal = 0.0;
	double dRAQty = 0.0;
	double dRAVal = 0.0;
	double dChargeQty = 0.0;
	double dChargeVal = 0.0;

	for ( int n = nStart; n <= nEnd; n++ )
	{
		CEposReportConsolCustomerHistory infoConsol;
		m_CustomerSalesTable.GetSalesTotalForBlock( n, infoConsol );

		switch( infoConsol.m_nReportLineType)
		{
		case SALESHISTCUST_LINE_PLU:
			dSaleQty = infoConsol.m_dQty;
			dSaleVal = infoConsol.m_dVal;
			break;

		case SALESHISTCUST_LINE_CHARGE:
			dChargeQty = infoConsol.m_dQty;
			dChargeVal = infoConsol.m_dVal;
			break;

		case SALESHISTCUST_LINE_RECEIVE:
			dRAQty = infoConsol.m_dQty;
			dRAVal = infoConsol.m_dVal;
			break;
		}
	}

	CArray<CCustomerHeaderInfo,CCustomerHeaderInfo> arrayHeaderInfo;

	{
		CCustomerHeaderInfo infoHeader;
		infoHeader.m_strAccount.Format( "Account %d", infoCustomer.m_nCustomerID ); 
		infoHeader.m_strAddress = infoCustomer.GetReportName();
		arrayHeaderInfo.Add( infoHeader );
	}

	if ( TRUE == bOneOff )
	{
		CCustomerHeaderInfo infoHeader;
		infoHeader.m_strAddress = "(One off Customer)";
		arrayHeaderInfo.Add( infoHeader );
	}

	if ( infoCustomer.m_strAddress1 != "" )
	{
		CCustomerHeaderInfo infoHeader;
		infoHeader.m_strAddress = infoCustomer.m_strAddress1;
		arrayHeaderInfo.Add( infoHeader );
	}

	if ( infoCustomer.m_strAddress2 != "" )
	{
		CCustomerHeaderInfo infoHeader;
		infoHeader.m_strAddress = infoCustomer.m_strAddress2;
		arrayHeaderInfo.Add( infoHeader );
	}

	if ( infoCustomer.m_strAddress3 != "" )
	{
		CCustomerHeaderInfo infoHeader;
		infoHeader.m_strAddress = infoCustomer.m_strAddress3;
		arrayHeaderInfo.Add( infoHeader );
	}

	if ( infoCustomer.m_strPostcode != "" )
	{
		CCustomerHeaderInfo infoHeader;
		infoHeader.m_strAddress = infoCustomer.m_strPostcode;
		arrayHeaderInfo.Add( infoHeader );
	}

	int nPos = 1;
	if ( TRUE == m_bShowSales )		AddTotalsToCustomerHeader( arrayHeaderInfo, nPos, "Sales Total", dSaleQty, dSaleVal, TRUE );
	if ( TRUE == m_bShowCharges )	AddTotalsToCustomerHeader( arrayHeaderInfo, nPos, "Charge Total", dChargeQty, dChargeVal, FALSE );
	if ( TRUE == m_bShowRA )		AddTotalsToCustomerHeader( arrayHeaderInfo, nPos, "RA Total", dRAQty, dRAVal, FALSE );
	
	WriteReportLine( "<LI>" );

	for ( int n = 0; n < arrayHeaderInfo.GetSize(); n++ )
	{
		CCustomerHeaderInfo infoHeader = arrayHeaderInfo.GetAt( n );

		CCSV csvOut ( '\t', '"', TRUE, FALSE, FALSE );	
		csvOut.Add( infoHeader.m_strAccount );
		csvOut.Add( infoHeader.m_strAddress );
		
		if ( infoHeader.m_strTotalName != "" )
		{
			csvOut.Add( infoHeader.m_strTotalName );

			if ( CPriceHelpers::CompareDoubles( infoHeader.m_dTotalQty, 0.0, 3 ) != 0 )
				csvOut.Add( infoHeader.m_dTotalQty, ( infoHeader.m_bDPQty ) ? SysInfo.GetDPQty() : 0 );
			else
				csvOut.Add( "" );

			csvOut.Add ( infoHeader.m_dTotalVal, SysInfo.GetDPValue() );
		}

		WriteReportLine ( csvOut.GetLine() );
	}

	WriteReportLine( "<LI>" );
	
	if ( FALSE == m_bLastFieldIsCustomer )
		WriteBlankLines( 1 );
}

/**********************************************************************/

void CSalesHistoryReportCustomer::AddTotalsToCustomerHeader( CArray<CCustomerHeaderInfo,CCustomerHeaderInfo>& arrayHeaderInfo, int& nPos, const char* szTotal, double dQty, double dVal, bool bDPQty )
{
	if ( ( nPos >= 0 ) && ( nPos < arrayHeaderInfo.GetSize() ) )
	{
		CCustomerHeaderInfo infoHeader = arrayHeaderInfo.GetAt( nPos );
		infoHeader.m_strTotalName = szTotal;
		infoHeader.m_dTotalQty = dQty;
		infoHeader.m_dTotalVal = dVal;
		infoHeader.m_bDPQty = bDPQty;
		arrayHeaderInfo.SetAt( nPos++, infoHeader );
	}
	else
	{
		CCustomerHeaderInfo infoHeader;
		infoHeader.m_strTotalName = szTotal;
		infoHeader.m_dTotalQty = dQty;
		infoHeader.m_dTotalVal = dVal;
		infoHeader.m_bDPQty = bDPQty;
		arrayHeaderInfo.Add( infoHeader );
		nPos = arrayHeaderInfo.GetSize();
	}
}

/**********************************************************************/

const char* CSalesHistoryReportCustomer::GetLabelTimeString()
{
	m_strLabelTimeString = "";

	switch( m_TimeSliceMap.GetReportType() )
	{
	case SH_TIMESLICE_MINUTE:
		m_strLabelTimeString.Format( "%2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute );
		break;

	case SH_TIMESLICE_5MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 4 );
		break;

	case SH_TIMESLICE_10MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 9 );
		break;

	case SH_TIMESLICE_15MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 14 );
		break;

	case SH_TIMESLICE_30MIN:
		m_strLabelTimeString.Format( "%2.2d:%2.2d - %2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeHour,
			m_nLabelTimeMinute + 29 );
		break;

	case SH_TIMESLICE_60MIN:
		m_strLabelTimeString.Format( "%2.2d:00 - %2.2d:59",
			m_nLabelTimeHour,
			m_nLabelTimeHour );
		break;

	case SH_CUSTOM:
		{
			m_strLabelTimeString.Format( "%4.4d", m_nLabelTimeHour );
			m_strLabelTimeString = m_TimeMap.GetSortOrderName( m_strLabelTimeString );
		}
		break;

	case SH_TIMESLICE_SECOND:
	default:
		m_strLabelTimeString.Format( "%2.2d:%2.2d:%2.2d",
			m_nLabelTimeHour,
			m_nLabelTimeMinute,
			m_nLabelTimeSecond );
		break;
	}

	switch ( m_nLabelTimeStars )
	{
	case 1:
		m_strLabelTimeString += "*";
		break;

	case 2:
		m_strLabelTimeString += "**";
		break;
	}

	return m_strLabelTimeString;
}

/**********************************************************************/
