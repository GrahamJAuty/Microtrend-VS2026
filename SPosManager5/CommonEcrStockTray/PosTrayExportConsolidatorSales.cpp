/**********************************************************************/
#include "DateRangeHelpers.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "PluSalesHistoryFileArray.h"
#include "PMSReportFileFinder.h"
#include "..\CommonEcrStock\PosTrayManualExportDlg.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "ReportPointCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayExportConsolidatorSales.h"
/**********************************************************************/

CPosTrayExportConsolidatorSales::CPosTrayExportConsolidatorSales( int nReportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg ) : CPosTrayExportConsolidator( nReportDateType, SelectArray, bQuitNow, pHostDlg )
{
	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
	m_bDepartmentByItem = FALSE;
	m_ReportFilters.PluFilter.SetMasterFilterNo(0);
	m_bSaleTime = FALSE;
	m_nTaxFilter = TAXBAND_FILTER_ALL;
	m_nPriceLevelFilter = PRICELEVEL_FILTER_ALL;
	m_nModifierFilter = MODIFIER_FILTER_ALL;

	m_bEposSales = TRUE;
	m_bManualSales = FALSE;
	m_bImportSales = FALSE;
	m_bGuestToTerminal = FALSE;

	m_bEODMode = FALSE;
	m_TimeSliceMap.SetReportType( SH_TIMESLICE_SECOND );
	m_TimeSliceMap.SetTimeSliceFileNo(0);
	
	m_bItemRange = FALSE;
	m_nItemFrom = 0;
	m_nItemTo = 0;
	m_bCustomer = FALSE;
	m_bVIPSales = FALSE;

	AddAcceptedField ( POSTRAY_EXPORTFIELD_DBNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_LOCNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_RPNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TRMNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TBLGRPNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TBLNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_CAL );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_BUS );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_YEARNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MONTHNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DATE_WEEK );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKDAYNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TIME_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TIME_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PLUNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MODNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNO_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNO_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNO_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNO_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNO_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNO_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SERVER_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SERVER_ITEM );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TAXNO );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PBANDNO );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DBNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_LOCNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_RPNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TBLGRPNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TBLNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_REPTEXT_BASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_REPTEXT_MOD );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MODNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SUPPREF );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SORTCODE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_STOCKCODE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNAME_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DPTNAME_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNAME_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_GRPNAME_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNAME_DBASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_ACATNAME_SALE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SRVNAME_TRAN );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_SRVNAME_ITEM );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TAXNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PBANDNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_MONTHNAME );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WEEKDAYNAME );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TRAN_SEQNO );
	/**********/
	AddAcceptedField ( POSTRAY_EXPORTFIELD_QTY_BASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_QTY_MOD );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_FULL );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_DISCOUNTONLY );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_PREMIUMONLY);
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_DISC );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_TAX );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_VALUE_NET );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_COST );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WASTE_QTY_BASE );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WASTE_QTY_MOD );
	AddAcceptedField ( POSTRAY_EXPORTFIELD_WASTE_COST );
}

/**********************************************************************/

void CPosTrayExportConsolidatorSales::ResetTaskSpecific( CPosTrayTask& EmailTask )
{
	if ( m_arrayOutputFields.GetSize() == 0 )
	{
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_PLUNO );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_REPTEXT_BASE );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_QTY_BASE );
		m_arrayOutputFields.Add( POSTRAY_EXPORTFIELD_VALUE_DISC );
		m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_PLUNO );
		m_bWantSales = TRUE;
	}

	if ( m_arrayConsolidationFields.GetSize() == 0 )
		m_arrayConsolidationFields.Add( POSTRAY_EXPORTFIELD_DUMMY );

	m_ReportFilters.DeptFilter.SetDeptFilterNo( DEPARTMENTSET_ALL );
	m_bEposSales = EmailTask.GetBatchEposSalesFlag();
	m_bManualSales = EmailTask.GetBatchManualSalesFlag();
	m_bImportSales = EmailTask.GetBatchImportSalesFlag();
	m_bGuestToTerminal = EmailTask.GetBatchPMSTerminalFlag();

	if ( ( FALSE == m_bEposSales ) && ( FALSE == m_bManualSales ) && ( FALSE == m_bImportSales ) )
		m_bEposSales = TRUE;

	m_nTaxDecimalPlaces = EmailTask.GetBatchTaxDecimalPlaces();
	if ( ( m_nTaxDecimalPlaces < 0 ) || ( m_nTaxDecimalPlaces > 8 ) )
		m_nTaxDecimalPlaces = SysInfo.GetDPValue();
	
	CLocationSelectorLookup lookup( EmailTask );
	CLocationSelectorEntity entity( lookup );
	entity.GetEPOSSelectArray( m_SelectArray );

	int nDbIdx;
	int nConLevel = lookup.GetConType();
	int nTableNo = lookup.GetEntityNo();
	dbDatabase.FindDatabaseByNumber( lookup.GetDbNo(), nDbIdx );
	
#ifdef STOCKMAN_SYSTEM

	if ( m_bManualSales == TRUE || m_bImportSales == TRUE )
	{
		switch ( nConLevel )
		{
		case NODE_DATABASE:
			m_SelectArray.SelectDatabaseForNonEposSales( nDbIdx, ( m_bManualSales != 0 ), ( m_bImportSales != 0 ) );
			break;

		case NODE_SYSTEM:
			{
				for ( int n = 0; n < dbDatabase.GetSize(); n++ )
					m_SelectArray.SelectDatabaseForNonEposSales( n, ( m_bManualSales != 0 ), ( m_bImportSales != 0 ) );
			}
			break;
		}
	}
	
#endif

	m_SelectArray.MakeList( TRUE );

	m_bItemRange = FALSE;
	m_nItemFrom = 0;
	m_nItemTo = 0;
	m_bCustomer = FALSE;
	m_bVIPSales = FALSE;

	m_SalesTree.ClearConsolidatedLevels();
	m_SalesTree.ClearFieldStructure();
}

/**********************************************************************/

bool CPosTrayExportConsolidatorSales::PrepareFields()
{
	bool bResult = CPosTrayExportConsolidator::PrepareFields( m_SalesTree );

	if ( m_TableFilter.m_nType != TABLEFILTER_ALL )
	{
		m_bWantTable = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CPosTrayExportConsolidatorSales::Consolidate() 
{
	dbDatabase.BuildSortIdxTables();
	dbLocation.BuildSortIdxTables();
	dbLocation.ResetRpNoReport();
	dbReportpoint.BuildSortIdxTables();
	DataManagerNonDb.CustomerNameTable.BuildSortOrder();

	if ( m_nTaxFilter != TAXBAND_FILTER_ALL )
		m_bWantPluInfo = TRUE;
	
	if ( m_bDepartmentByItem == TRUE )
		m_bWantPluInfo = TRUE;

	m_SalesTree.ClearConsolidatedLevels();
	
	CPluSalesHistoryFileArray arrayFileInfo( m_SelectArray );
	arrayFileInfo.SetEposSalesFlag( m_bEposSales );
	arrayFileInfo.SetImportSalesFlag( m_bImportSales );
	arrayFileInfo.SetManualSalesFlag( m_bManualSales );
	arrayFileInfo.SetEODModeFlag( m_bEODMode );
	arrayFileInfo.SetGuestToTerminalFlag( m_bGuestToTerminal );
	arrayFileInfo.SetSaleTimeFlag( m_bSaleTime );
	arrayFileInfo.BuildFileList();

	int nProgress = 0;	
	
	if ( NULL == m_pHostDialog )
		StatusProgress.Lock( TRUE, "Consolidating report data" );

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetFileCount() ) && ( FALSE == m_bQuitNow ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetFileInfo( nFileIdx, infoFile );

		int nDbIdx;
		int nDbNo;
		{
			CEposSelectArrayListItem ListItem;
			m_SelectArray.GetListItem( infoFile.m_nSelectArrayIdx, ListItem );
			nDbIdx = ListItem.m_nDbIdx;
			nDbNo = dbDatabase.GetDbNo( nDbIdx );
		}

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

		CPluSalesLine csvSales( infoFile.IsLegacySalesFolder() );

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( nDbIdx );
		int nSortLocIdx = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
		
		int nTNo = 0;
		if ( infoFile.m_nTermIdx != 0 )
			nTNo = dbLocation.GetTNo( infoFile.m_nLocIdx, infoFile.m_nTermIdx - 1 );

		//THESE VALUES STAY THE SAME WITHIN EACH FILE
		for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
		{
			switch ( m_arrayConsolidationFields.GetAt( n ) )
			{
			case POSTRAY_EXPORTFIELD_DUMMY:	m_SalesTree.SetBufferInt( n, 0 );				break;
			case POSTRAY_EXPORTFIELD_DBNO:	m_SalesTree.SetBufferInt( n, nSortDbIdx );		break;
			case POSTRAY_EXPORTFIELD_LOCNO:	m_SalesTree.SetBufferInt( n, nSortLocIdx );		break;
			case POSTRAY_EXPORTFIELD_TRMNO:	m_SalesTree.SetBufferInt( n, nTNo );			break;
			case POSTRAY_EXPORTFIELD_RPNO:	m_SalesTree.SetBufferInt( n, nRpIdx );			break;	
			}
		}

		//MAKE SURE THAT THE RIGHT DATABASE IS LOADED FOR TAX AND GROUP INFO
		m_ReportFilters.SetDatabase( nDbIdx );

		bool bIsPMSLocation = dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx );

#ifdef COMPILE_PMS
		CPMSReportFileFinder PMSReportFileFinder;
		PMSReportFileFinder.SetPMSDateMode( PMS_DATEMODE_ACTUAL, "", "", TRUE );
#endif

		CString strThisFilterDate = "";
		CString strLastFilterDate = "";
		CString strThisFilterTime = "";
		COleDateTime oleThisFilterDate = COleDateTime( 2000, 1, 1, 0, 0, 0 );
		
		if ( FALSE == bIsPMSLocation )
		{
			if ( TRUE == m_bSaleTime )
			{
				strThisFilterDate = infoFile.m_strDateSale;
				strLastFilterDate = infoFile.m_strDateSale;
			}
			else
			{
				strThisFilterDate = infoFile.m_strDateTran;
				strLastFilterDate = infoFile.m_strDateTran;
			}

			oleThisFilterDate = COleDateTime( atoi( strThisFilterDate.Left(4) ), atoi( strThisFilterDate.Mid(4,2) ), atoi( strThisFilterDate.Mid(6,2) ), 0, 0, 0 );
		}

		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );

		if ( TRUE == m_bWantTable )
		{
			m_TableReportHelpers.SetLocIdx( infoFile.m_nLocIdx );
			m_TableReportHelpers.BuildGroupMap( m_TableFilter );
		}

		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion( infoFile.m_nLocIdx, nDummy, nConnectionType );
		}

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			bool bPMSInclude = FALSE;

			do
			{
				ShowProgress( nProgress + fileSales.Tell(), arrayFileInfo.GetTotalFileSize() );

#ifdef COMPILE_PMS
				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if ( TRUE == bIsPMSLocation )
				{
					if ( strBuffer.Left(9) == "#PMS_TIME" )
					{
						int nDummy;
						PMSReportFileFinder.ProcessPMSTimeLine( strBuffer, nDummy, oleThisFilterDate, strThisFilterDate, strThisFilterTime );
						bPMSInclude = DataManagerNonDb.SessionDateTimeFilter.CheckDate( strThisFilterDate );
						continue;
					}

					if ( FALSE == bPMSInclude )
						continue;
				}
#endif

				csvSales.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPES
				int nLineType = csvSales.GetLineType();
				{
					bool bAllowType = FALSE;
					
					switch( nLineType )
					{
					case CSVPLUDATA_PROMO_TAXABLE:
					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
						bAllowType = ( DealerFlags.GetHoltsDiscountFlag() == FALSE ); 
						bAllowType &= ( m_ReportFilters.PluFilter.GetCurrentFilterNo() == 0 );
						break;

					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
						bAllowType = ( DealerFlags.GetHoltsDiscountFlag() == FALSE ); 
						break;

					case CSVPLUDATA_PLU_SALE:
						bAllowType = TRUE;
						break;

					case CSVPLUDATA_PLU_CHEAPESTFREE:
					default:
						bAllowType = FALSE;
						break;
					}

					if ( FALSE == bAllowType )
						continue;
				}

				if (csvSales.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				if (csvSales.GetPluSaleWastageFlag() == TRUE)
				{
					if (FALSE == m_bWantWaste)
						continue;
				}
				else
				{
					if (FALSE == m_bWantSales)
						continue;
				}

				//FILTER FOR POST TO ROOM
				if ( SysInfo.IsPMSSystem() == TRUE )
					if ( ( FALSE == m_bGuestToTerminal ) && ( FALSE == bIsPMSLocation ) )
						if ( DataManager.Payment.IsPMSPaymentType( csvSales.GetPluPaymentNo() ) )
							continue;

				int nLevel = csvSales.GetPriceBand() + 1;
				if ( CONNECTION_TYPE_SPTBOOK_NONE != nConnectionType )
				{
					if ( ( nLevel < 1 ) || ( nLevel >= MAX_PLU_PRICES ) )
						nLevel = 1;
				}
				else
				{
					nLevel = DataManager.SBPriceText.GetReportMapPriceBand( nLevel - 2 ) + 1;
				}

				//THROW OUT NON LEVEL 3 SALES FOR VIP MODE
				if ( TRUE == m_bVIPSales )
				{
					if ( ( CONNECTION_TYPE_STANDARD_NONE != nConnectionType ) || ( csvSales.IsVIPSale() == FALSE ) )
						continue;
				}

				//TRANSLATE SERVER INTO CONSOLIDATION STRING
				CString strTranServer = "";
				if ( TRUE == m_bWantTranServer )
					DataManagerNonDb.ServerNameTable.GetServerConsolidationString( nDbIdx, infoFile.m_nLocIdx, csvSales.GetTranServerNo(), strTranServer );

				CString strItemServer = "";
				if ( TRUE == m_bWantItemServer )
					DataManagerNonDb.ServerNameTable.GetServerConsolidationString( nDbIdx, infoFile.m_nLocIdx, csvSales.GetItemServerNo(), strItemServer );

				//TRANSLATE CUSTOMER INTO CONSOLIDATION STRING
				int nCustomerID = csvSales.GetCustomerID();
				if ( TRUE == m_bCustomer )
					if ( 0 == nCustomerID )
						continue;

				CString strCustomer = "";
				if ( TRUE == m_bWantCustomer )
				{
					if ( 0 == nCustomerID )
						strCustomer = "3";
					else
					{
						CCustomerSelectionListAccountInfo infoCustomer;
						infoCustomer.m_nLocIdx = infoFile.m_nLocIdx;
						infoCustomer.m_nCustomerID = nCustomerID;

						int nNodeType, nPos;
						DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoCustomer, nNodeType, nPos );
						switch( nNodeType )
						{
						case NODE_CUSTOMER_ACCOUNT:
							strCustomer.Format ( "0%8.8d%8.8d", infoCustomer.m_nSortIdx, nPos );
							break;

						case NODE_CUSTOMER_ONEOFF:
							strCustomer.Format ( "1%8.8d%8.8d", infoCustomer.m_nSortIdx, nPos );
							break;

						case NODE_PLACEHOLDER:
							strCustomer.Format ( "2%8.8d", nCustomerID );
							break;
						}
					}
				}

				CString strTranTime = "";
				CString strSaleTime = "";
			
				//FILTER BY DATE AND TIME
				if ( FALSE == bIsPMSLocation )
				{
					strTranTime = csvSales.GetTranTime();
					strSaleTime = csvSales.GetSaleTime();
				}
				else
				{
					strTranTime = strThisFilterTime;
					strSaleTime = strThisFilterTime;
				}

				ExpandTimeString( strTranTime );
				ExpandTimeString( strSaleTime );

				strThisFilterTime = ( m_bSaleTime ) ? strSaleTime : strTranTime;

				if ( ( TRUE == bIsPMSLocation ) && ( strThisFilterDate != strLastFilterDate ) )
				{	
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisFilterDate );
					oleThisFilterDate = COleDateTime( atoi( strThisFilterDate.Left(4) ), atoi( strThisFilterDate.Mid(4,2) ), atoi( strThisFilterDate.Mid(6,2) ), 0, 0, 0 );
					strLastFilterDate = strThisFilterDate;
				}

				if ( oleThisFilterDate.m_status != COleDateTime::valid )
					continue;

				//CHECK FOR PREVIOUS DATE
				if ( m_BusinessDateInfo.ValidateBusinessDate( infoFile.m_nLocIdx, m_bEODMode, strThisFilterDate, strThisFilterTime ) == FALSE )
					continue;

				CreateTimeLabel( strTranTime, ( FALSE == m_bSaleTime ) );
				CreateTimeLabel( strSaleTime, ( TRUE == m_bSaleTime ) );

				//FILTER BY DEPT NO OF SALE
				int nDeptNoSale = csvSales.GetDeptNo();

				if ( m_bDepartmentByItem == FALSE )
					if ( m_ReportFilters.CheckDepartment( nDeptNoSale ) == FALSE )
						continue;

				int nDeptGrpSale = 0;
				if ( m_bWantDeptGrpSale == TRUE )
				{
					nDeptGrpSale = csvSales.GetGroupNo() - 1;

					if ( nDeptGrpSale < 0 )
						nDeptGrpSale = 0;
				}

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

						if ( m_ReportFilters.CheckPlu( infoPluNo ) == FALSE )
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
				int nCategorySale = 0;
				int nCategoryItem = 0;
				int nPluSupp = 0;
				int nDeptNoItem = 0;
				double dCostPerPlu = 0.0;
				
				if ( m_bWantPluInfo == TRUE )
				{
					nCategorySale = csvSales.GetAnalysisNo();
					nCategoryItem = 0;

					int nPluIdx;
					if ( DataManager.Plu.FindPluByNumber( nBasePluNo, nPluIdx ) == TRUE )
					{
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt( nPluIdx, PluRecord );
						nTaxBand = PluRecord.GetModifierTaxBandIntForReport(nModifier);
						nPluSupp = PluRecord.GetSupplierNo();
						nCategoryItem = PluRecord.GetModifierAnalysisCategoryForReport(nModifier);

						if ( 0 == nCategorySale )
							nCategorySale = nCategoryItem;

						nDeptNoItem = PluRecord.GetModifierDeptNoForReport(nModifier);

						if ( TRUE == m_bWantCost )
							dCostPerPlu = DataManager.Plu.GetPluPurchaseCost( PluRecord );
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

				int nDeptGrpItem = 0;
				if ( m_bWantDeptInfo == TRUE )
				{
					int nDeptIdx;
					if ( DataManager.Department.FindDeptByNumber( nDeptNoItem, nDeptIdx ) == TRUE )
					{
						CDepartmentCSVRecord deptRecord;
						DataManager.Department.GetAt( nDeptIdx, deptRecord );
						nDeptGrpItem = deptRecord.GetEposGroup();
					}
				}

				//FILTER BY DEPARTMENT NUMBER OF ITEM
				if ( m_bDepartmentByItem == TRUE )
					if ( m_ReportFilters.CheckDepartment( nDeptNoItem ) == FALSE )
						continue;
		
				//FILTER BY PRICE LEVEL
				if ( PRICELEVEL_FILTER_ALL != m_nPriceLevelFilter )
				{
					int nMask = 0;
						
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
					
					if ( ( nMask != 0 ) && ( ( m_nPriceLevelFilter & nMask ) == 0 ) )
						continue;
				}

				if ( MODIFIER_FILTER_ALL != m_nModifierFilter )
				{
					int nMask = 0;
						
					switch ( nModifier )
					{
					case 0:	nMask = 1;		break;
					case 1:	nMask = 2;		break;
					case 2:	nMask = 4;		break;
					case 3:	nMask = 8;		break;
					case 4:	nMask = 16;		break;
					case 5:	nMask = 32;		break;
					case 6:	nMask = 64;		break;
					case 7:	nMask = 128;	break;
					case 8:	nMask = 256;	break;
					case 9:	nMask = 512;	break;
					}
					
					if ( ( nMask != 0 ) && ( ( m_nModifierFilter & nMask ) == 0 ) )
						continue;
				}

				if ( 0 == nDeptNoSale )		nDeptNoSale = 1000;
				if ( 0 == nDeptNoItem )		nDeptNoItem = 1000;
				if ( 0 == nDeptGrpSale )	nDeptGrpSale = 1000;
				if ( 0 == nDeptGrpItem )	nDeptGrpItem = 1000;

				//FILTER BY TABLE	;
				int nTableGrp = 0;
				int nTableNo = csvSales.GetTranTableNo();
				if ( TRUE == m_bWantTable )
				{
					if ( m_TableReportHelpers.CheckTableGroup( nTableNo, nTableGrp ) == FALSE )
						continue;
				}
				
				CDateRangeHelpers DateRangeHelpers;

				//THESE VALUES CHANGE ON A LINE-BY-LINE BASIS
				for ( int n = 0; n < m_arrayConsolidationFields.GetSize(); n++ )
				{
					switch ( m_arrayConsolidationFields.GetAt( n ) )
					{
					case POSTRAY_EXPORTFIELD_DATE_BUS:		m_SalesTree.SetBufferDate( n, m_BusinessDateInfo.GetBusinessDate() );	 break;
					case POSTRAY_EXPORTFIELD_DATE_CAL:		m_SalesTree.SetBufferDate( n, oleThisFilterDate ); break;
					case POSTRAY_EXPORTFIELD_YEARNO:		m_SalesTree.SetBufferInt( n, m_BusinessDateInfo.GetBusinessDate().GetYear() ); break;
					case POSTRAY_EXPORTFIELD_MONTHNO:		m_SalesTree.SetBufferInt( n, m_BusinessDateInfo.GetBusinessDate().GetMonth() ); break;
					case POSTRAY_EXPORTFIELD_DATE_WEEK:		m_SalesTree.SetBufferDate( n, m_BusinessDateInfo.GetBusinessDateWeek() );	break;
					case POSTRAY_EXPORTFIELD_WEEKNO:		m_SalesTree.SetBufferInt( n, DateRangeHelpers.GetWeekNo( m_BusinessDateInfo.GetBusinessDateWeek() ) );	break; 
					case POSTRAY_EXPORTFIELD_WEEKDAYNO:		m_SalesTree.SetBufferInt( n, m_BusinessDateInfo.GetWeekday() ); break;
					case POSTRAY_EXPORTFIELD_TIME_TRAN:		m_SalesTree.SetBufferString( n, strTranTime ); break;
					case POSTRAY_EXPORTFIELD_TIME_SALE:		m_SalesTree.SetBufferString( n, strSaleTime ); break;
					case POSTRAY_EXPORTFIELD_S4LABOUR_HOUR:	m_SalesTree.SetBufferString( n, strSaleTime.Mid(1,2) ); break;
					case POSTRAY_EXPORTFIELD_SERVER_TRAN:	m_SalesTree.SetBufferString( n, strTranServer ); break;
					case POSTRAY_EXPORTFIELD_SERVER_ITEM:	m_SalesTree.SetBufferString( n, strItemServer ); break;
					case POSTRAY_EXPORTFIELD_ACATNO_SALE:	m_SalesTree.SetBufferInt( n, nCategorySale ); break;
					case POSTRAY_EXPORTFIELD_ACATNO_DBASE:	m_SalesTree.SetBufferInt( n, nCategoryItem ); break;
					case POSTRAY_EXPORTFIELD_DPTNO_SALE:	m_SalesTree.SetBufferInt( n, nDeptNoSale ); break;
					case POSTRAY_EXPORTFIELD_DPTNO_DBASE:	m_SalesTree.SetBufferInt( n, nDeptNoItem ); break;
					case POSTRAY_EXPORTFIELD_GRPNO_SALE:	m_SalesTree.SetBufferInt( n, nDeptGrpSale );	break;
					case POSTRAY_EXPORTFIELD_GRPNO_DBASE:	m_SalesTree.SetBufferInt( n, nDeptGrpItem );	break;				
					case POSTRAY_EXPORTFIELD_PLUNO:			m_SalesTree.SetBufferInt64( n, nBasePluNo ); break;
					case POSTRAY_EXPORTFIELD_MODNO:			m_SalesTree.SetBufferInt( n, nModifier ); break;
					case POSTRAY_EXPORTFIELD_TAXNO:			m_SalesTree.SetBufferInt( n, nTaxBand ); break;
					case POSTRAY_EXPORTFIELD_PBANDNO:		m_SalesTree.SetBufferInt( n, nLevel ); break;
					case POSTRAY_EXPORTFIELD_TRAN_SEQNO:	m_SalesTree.SetBufferInt( n, csvSales.GetTranSeqNo() ); break;
					case POSTRAY_EXPORTFIELD_TBLGRPNO:		m_SalesTree.SetBufferInt( n, nTableGrp ); break;
					case POSTRAY_EXPORTFIELD_TBLNO:			m_SalesTree.SetBufferInt( n, nTableNo ); break;
					}
				}
				
				double dLineBaseQty = 0.0;
				double dLineModQty = 0.0;
				double dLineVal = 0.0;
				double dLineTax = 0.0;
				double dLineWaste = 0.0;
				double dLineDiscountVal = 0.0;
				double dLinePremiumVal = 0.0;
				double dLineCost = 0.0;
				double dLineWasteBaseQty = 0.0;
				double dLineWasteModQty = 0.0;
				double dLineWasteCost = 0.0;

				bool bConsolidateLine = FALSE;
				
				if ( csvSales.GetPluSaleWastageFlag() == TRUE )
				{
					dLineWasteModQty = csvSales.GetSaleQty();
					dLineWasteBaseQty = dLineWasteModQty * dMultiplier;
					dLineWasteCost = dLineWasteBaseQty * dCostPerPlu;
					bConsolidateLine = TRUE;
				}
				else
				{
					switch (nLineType)
					{
					case CSVPLUDATA_PLU_DISCOUNT:
					case CSVPLUDATA_PLU_MIXMATCH:
					{
						bConsolidateLine = TRUE;
						double dRawDiscountVal = csvSales.GetValue();

						if (csvSales.IsPremium())
						{
							dLineDiscountVal = 0.0;
							dLinePremiumVal = dRawDiscountVal * (-1);
						}
						else
						{
							dLineDiscountVal = dRawDiscountVal;
							dLinePremiumVal = 0.0;
						}

						if (TRUE == m_bWantTax)
						{
							double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
							dLineTax = dRawDiscountVal - ReportHelpers.CalcNonTax(dRawDiscountVal, dTaxRate);
							dLineTax *= -1;
							dLineBaseQty = 0.0;
							dLineModQty = 0.0;
						}
					}
					break;

					case CSVPLUDATA_PROMO_TAXABLE:
					{
						bConsolidateLine = TRUE;
						double dRawDiscountVal = csvSales.GetValue();
						dLineBaseQty = csvSales.GetSaleQty();
						dLineModQty = dLineBaseQty;

						if (csvSales.IsPremium())
						{
							dLineDiscountVal = 0.0;
							dLinePremiumVal = dRawDiscountVal * (-1);
						}
						else
						{
							dLineDiscountVal = dRawDiscountVal;
							dLinePremiumVal = 0.0;
						}

						if (TRUE == m_bWantTax)
						{
							double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
							dLineTax = dRawDiscountVal - ReportHelpers.CalcNonTax(dRawDiscountVal, dTaxRate);
							dLineTax *= -1;
						}
					}
					break;

					case CSVPLUDATA_PROMO_NONTAXABLE:
					case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					{
						bConsolidateLine = TRUE;
						double dRawDiscountVal = csvSales.GetValue();
						dLineTax = 0.0;
						dLineBaseQty = csvSales.GetSaleQty();
						dLineModQty = dLineBaseQty;

						if (csvSales.IsPremium())
						{
							dLineDiscountVal = 0.0;
							dLinePremiumVal = dRawDiscountVal * (-1);
						}
						else
						{
							dLineDiscountVal = dRawDiscountVal;
							dLinePremiumVal = 0.0;
						}
					}
					break;

					case CSVPLUDATA_PLU_SALE:
						if (csvSales.GetPluSaleVoidFlag() == FALSE)
						{
							bConsolidateLine = TRUE;

							dLineModQty = csvSales.GetSaleQty();
							dLineBaseQty = dLineModQty * dMultiplier;
							dLineVal = csvSales.GetValue();

							if (TRUE == m_bWantTax)
							{
								double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(nTaxDateCode, nTaxBand);
								dLineTax = dLineVal - (ReportHelpers.CalcNonTax(dLineVal, dTaxRate));
							}

							if (TRUE == m_bWantCost)
							{
								dLineCost = dCostPerPlu * dLineModQty * dMultiplier;
							}
						}
						break;
					}
				}

				if ( TRUE == bConsolidateLine )
				{
					CEntityTreeExportSalesNode Node;
					Node.SetBaseQty( dLineBaseQty );
					Node.SetModQty( dLineModQty );
					Node.SetVal( dLineVal );
					Node.SetDiscountVal( dLineDiscountVal );
					Node.SetPremiumVal( dLinePremiumVal );
					Node.SetTaxAmount( dLineTax );
					Node.SetCost( dLineCost );
					Node.SetWasteBaseQty( dLineWasteBaseQty );
					Node.SetWasteModQty( dLineWasteModQty );
					Node.SetWasteCost( dLineWasteCost );
					m_SalesTree.Consolidate( Node );
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) && ( FALSE == m_bQuitNow ) );
		}
		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	if ( NULL == m_pHostDialog )
		StatusProgress.Unlock();
}

/**********************************************************************/

__int64 CPosTrayExportConsolidatorSales::CreateExport( const char* szFilename )
{
	bool bResult;
	return CPosTrayExportConsolidator::CreateExport( &m_SalesTree, "Creating Sales Export", szFilename, bResult, FALSE );
}

/**********************************************************************/

void CPosTrayExportConsolidatorSales::GetNodeTotals( int nBranchIdx, int nLeafIdx )
{
	CEntityTreeExportSalesNode SalesNode;
	m_SalesTree.GetNode( nBranchIdx, nLeafIdx, SalesNode );
	m_NodeTotals.m_dCost = SalesNode.GetCost();
	m_NodeTotals.m_dQtyBase = SalesNode.GetBaseQty();
	m_NodeTotals.m_dQtyMod = SalesNode.GetModQty();
	m_NodeTotals.m_dValueFull = SalesNode.GetVal();
	m_NodeTotals.m_dDiscountVal = SalesNode.GetDiscountVal();
	m_NodeTotals.m_dPremiumVal = SalesNode.GetPremiumVal();
	m_NodeTotals.m_dValueAfterDiscount = m_NodeTotals.m_dValueFull - m_NodeTotals.m_dDiscountVal + m_NodeTotals.m_dPremiumVal;
	m_NodeTotals.m_dTaxAmount = SalesNode.GetTaxAmount();
	m_NodeTotals.m_dValueNet = m_NodeTotals.m_dValueAfterDiscount - m_NodeTotals.m_dTaxAmount;
	m_NodeTotals.m_dWasteQtyBase = SalesNode.GetWasteBaseQty();
	m_NodeTotals.m_dWasteQtyMod = SalesNode.GetWasteModQty();
	m_NodeTotals.m_dWasteCost = SalesNode.GetWasteCost();
}

/**********************************************************************/