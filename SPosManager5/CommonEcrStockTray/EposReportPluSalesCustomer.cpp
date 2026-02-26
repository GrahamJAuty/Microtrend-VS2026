/**********************************************************************/
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "PMSReportFileFinder.h"
#include "ReportCostFinder.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "UnderlineMaker.h"
/**********************************************************************/
#include "EposReportPluSalesCustomer.h"
/**********************************************************************/

CEposReportPluSalesCustomer::CEposReportPluSalesCustomer( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_PLUSALES_CUSTOMER;
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

void CEposReportPluSalesCustomer::Consolidate()
{
	DataManagerNonDb.CustomerNameTable.BuildSortOrder();
	
	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus( infoFile );
		
		if ( 0 == nPMSStatus)
			continue;

		bool bIsPMSLocation = ( 1 == nPMSStatus );

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( bIsPMSLocation, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
		int nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( infoFile.m_strDateTran );
		CString strLastSaleDate = strThisSaleDate;

		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );

		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			COleDateTime oleDatePMS = COleDateTime::GetCurrentTime();
			bool bPMSInclude = FALSE;

			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

				//EXTRACT DATE AND TIME INFORMATION FROM #PMS_TIME LINE 
				if ( m_PMSModes.CheckPMSTimeLine( bIsPMSLocation, strBuffer, strThisSaleDate, strThisSaleTime, oleDatePMS, bPMSInclude ) == FALSE )
					continue;
						
				csv.ParseLine( strBuffer );

				//CHECK FOR RELEVANT LINE TYPE
				int nLineType = csv.GetLineType();
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

				//FILTER FOR WASTAGE
				if (csv.GetPluSaleWastageFlag() != IsWastageReport())
				{
					continue;
				}

				if (csv.GetPluSaleVoidFlag() == TRUE)
				{
					continue;
				}

				//FILTER FOR POST TO ROOM
				if ( SysInfo.IsPMSSystem() == TRUE )
					if ( ( m_PMSModes.GetEposFlag() == FALSE ) && ( FALSE == bIsPMSLocation ) )
						if ( DataManager.Payment.IsPMSPaymentType( csv.GetPluPaymentNo() ) )
							continue;

				//FORCE NON CUSTOMER SALES TO BOTTOM OF REPORT
				int nCustomerID = csv.GetCustomerID();
				int nSortType = ( 0 == nCustomerID ) ? 1 : 0;
				
				//CHECK SALE DATE AND TIME
				if ( CheckPluSaleDateTime( bIsPMSLocation, infoFile, csv, oleDatePMS, strThisSaleDate, strThisSaleTime ) == FALSE )
					continue;

				if ( ( TRUE == bIsPMSLocation ) && ( strThisSaleDate != strLastSaleDate ) )
				{
					nTaxDateCode = DataManager.HistoricalTaxRates.GetDateCode( strThisSaleDate );
					strLastSaleDate = strThisSaleDate;
				}

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				double dMultiplier = 1.0;
				__int64 nPluNo = 0;
				int nModifier = 0;

				switch( nLineType )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					nPluNo = GetSeparatedPromoPluNo( csv.GetPromoNoSeparate() );
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					nPluNo = GetSeparatedMixMatchPluNo( csv.GetMixMatchNo() );
					break;
			
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );
						
						if ( FALSE == infoPluNo.m_bValid )
							continue;

						if ( m_ReportFilters.CheckPlu( infoPluNo ) == FALSE )
							continue;
			
						dMultiplier = 1.0;
						if ( infoPluNo.m_nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( infoPluNo.m_nModifier );

						nPluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;
					}
					break;
				}

				int nPriceBand = csv.GetPriceBand();
				if ( ( nPriceBand < 0 ) || ( nPriceBand >= MAX_PLU_PRICES ) )
					nPriceBand = 0;

				CEposReportConsolPluSalesCustomerID ConsolInfo;
				ConsolInfo.m_nLocIdx = infoFile.m_nLocIdx;
				ConsolInfo.m_nSortType = nSortType;
				ConsolInfo.m_nCustomerID = nCustomerID;
				ConsolInfo.m_nPluNo = nPluNo;
				ConsolInfo.m_nDeptNo = nDeptNo;
				ConsolInfo.m_nPriceBand = nPriceBand;
				ConsolInfo.m_nTaxDateCode = nTaxDateCode;

				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
					ConsolInfo.m_nTaxBand = -1;
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				else
					ConsolInfo.m_nTaxBand = nModifier; //WILL CONVERT TO TAX BAND LATER

				switch ( nLineType )
				{
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_dQty = 0.0;
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					m_ConsolArrayID.Consolidate( ConsolInfo );
					break;

				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					ConsolInfo.m_dQty = csv.GetSaleQty();
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					m_ConsolArrayID.Consolidate( ConsolInfo );
					break;

				case CSVPLUDATA_PLU_SALE:
					ConsolInfo.m_dQty = csv.GetSaleQty() * dMultiplier;
					ConsolInfo.m_dValBeforeDiscount = csv.GetValue();
					ConsolInfo.m_dDiscountVal = 0.0;
					ConsolInfo.m_dPremiumVal = 0.0;
					m_ConsolArrayID.Consolidate( ConsolInfo );
					break;
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();

	//SORT BY NAME AND GET DATABASE TAX BANDS

	int nSize = m_ConsolArrayID.GetSize();
	
	int nCurrentLocIdx = -1;
	int nCurrentDbIdx = -1;
	__int64 nCurrentPluNo = 0;
	bool bCurrentPluExists = FALSE;
	CPluCSVRecord CurrentPluRecord;

	StatusProgress.Lock( TRUE, "Sorting by name" );

	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos( n, nSize );

		int nIdx = ( nSize - n ) - 1;

		CEposReportConsolPluSalesCustomerID infoDetailID;
		m_ConsolArrayID.GetAt( nIdx, infoDetailID );

		int nSortIdxType = 0;
		int nSortIdxNum = 0;

		if ( 1 == infoDetailID.m_nSortType )
		{
			nSortIdxType = 3;
			nSortIdxNum = 0;
		}
		else
		{
			CCustomerSelectionListAccountInfo infoListSelect;
			infoListSelect.m_nLocIdx = infoDetailID.m_nLocIdx;
			infoListSelect.m_nCustomerID = infoDetailID.m_nCustomerID;

			int nNodeType, nDummy;
			DataManagerNonDb.CustomerNameTable.GetCustomerStatus( infoListSelect, nNodeType, nDummy );
			switch( nNodeType )
			{
			case NODE_CUSTOMER_ACCOUNT:
				nSortIdxType = 0;
				nSortIdxNum = infoListSelect.m_nSortIdx;
				break;

			case NODE_CUSTOMER_ONEOFF:
				nSortIdxType = 1;
				nSortIdxNum = infoListSelect.m_nSortIdx;
				break;

			case NODE_PLACEHOLDER:
			default:
				nSortIdxType = 2;
				nSortIdxNum = infoDetailID.m_nCustomerID;
				break;
			}
		}

		int nLocIdx = infoDetailID.m_nLocIdx;
		__int64 nPluNo = infoDetailID.m_nPluNo;

		if (DealerFlags.GetUseDatabaseVATBandFlag() == TRUE)
		{
			if ( nLocIdx != nCurrentLocIdx )
			{
				int nDbIdx;
				if ( dbDatabase.FindDatabaseByNumber( dbLocation.GetDbNo(nLocIdx), nDbIdx ) == TRUE )
				{
					if ( nDbIdx != nCurrentDbIdx )
					{
						CDataManagerInfo info;
						DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
						nCurrentDbIdx = nDbIdx;
						nCurrentPluNo = 0;
					}
				}
				nCurrentLocIdx = nLocIdx;
			}
		
			if ( nPluNo != nCurrentPluNo )
			{
				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
				{
					bCurrentPluExists = TRUE;
					DataManager.Plu.GetAt( nPluIdx, CurrentPluRecord );
				}
				else
				{
					bCurrentPluExists = FALSE;
				}

				nCurrentPluNo = nPluNo;
			}

			if ( TRUE == bCurrentPluExists )
				infoDetailID.m_nTaxBand = CurrentPluRecord.GetModifierTaxBandIntForReport( infoDetailID.m_nTaxBand );
			else
				infoDetailID.m_nTaxBand = 0;
		}

		CEposReportConsolPluSalesCustomerSortIdx infoDetailSortIdx;
		infoDetailSortIdx.m_nLocSortIdx = dbLocation.GetSortIdxFromLocIdx( nLocIdx );
		infoDetailSortIdx.m_nSortIdxType = nSortIdxType;
		infoDetailSortIdx.m_nSortIdxNum = nSortIdxNum;
		infoDetailSortIdx.m_nCustomerID = infoDetailID.m_nCustomerID;
		infoDetailSortIdx.m_nPluNo = nPluNo;
		infoDetailSortIdx.m_nDeptNo = infoDetailID.m_nDeptNo;
		infoDetailSortIdx.m_nTaxBand = infoDetailID.m_nTaxBand;
		infoDetailSortIdx.m_nPriceBand = infoDetailID.m_nPriceBand;
		infoDetailSortIdx.m_nTaxDateCode = infoDetailID.m_nTaxDateCode;
		infoDetailSortIdx.m_dQty = infoDetailID.m_dQty;
		infoDetailSortIdx.m_dValBeforeDiscount = infoDetailID.m_dValBeforeDiscount;
		infoDetailSortIdx.m_dDiscountVal = infoDetailID.m_dDiscountVal;
		infoDetailSortIdx.m_dPremiumVal = infoDetailID.m_dPremiumVal;
		infoDetailSortIdx.m_nLocIdx = infoDetailID.m_nLocIdx;
		
		m_ConsolArraySortIdx.Consolidate( infoDetailSortIdx );

		m_ConsolArrayID.RemoveAt( nIdx );
	}

	if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
	{
		int nLastDbIdx = -1;
		CReportCostFinder CostFinder;

		__int64 nProgressTarget = m_ConsolArraySortIdx.GetSize();
		__int64 nProgressCount = 0;

		StatusProgress.Lock( TRUE, "Calculating cost prices" );

		for ( int n = 0; n < m_ConsolArraySortIdx.GetSize(); n++ )
		{
			CEposReportConsolPluSalesCustomerSortIdx Sales;
			m_ConsolArraySortIdx.GetAt( n, Sales );
	
			int nDbIdx;
			int nDbNo = dbLocation.GetDbNo( Sales.m_nLocIdx );
			dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx );

			if ( nDbIdx != nLastDbIdx )
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
				nLastDbIdx = nDbIdx;
				CostFinder.Init();
			}

			Sales.m_dItemCost = CostFinder.GetPluCostByPluNo( Sales.m_nPluNo, Sales.m_nLocIdx );
			m_ConsolArraySortIdx.SetAt( n, Sales );

			StatusProgress.SetPos( nProgressCount++, nProgressTarget );
		}

		StatusProgress.Unlock();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportPluSalesCustomer::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Dept", TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Sales", TA_RIGHT, 250 );

	if ( TRUE == m_bDiscountDetail )
	{
		m_ReportFile.AddColumn ( "Discount", TA_RIGHT, 250 );

		if (TRUE == m_bPremiumDetail)
		{
			m_ReportFile.AddColumn("Premium", TA_RIGHT, 250);
		}

		m_ReportFile.AddColumn ( "Retail", TA_RIGHT, 250 );
	}
	
	m_ReportFile.AddColumn ( "Average", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT, 200 );
	m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250 );

	if ( TRUE == m_bNetOfTax )
		m_ReportFile.AddColumn ( "Est. Net", TA_RIGHT, 250 );

	m_ReportFile.AddColumn ( "Est. Cost", TA_RIGHT, 250 );

	int nCurrentBlockStart = -1;
	
	CEposReportConsolPluSalesCustomerSortIdx CurrentLocation;
	CEposReportConsolPluSalesCustomerSortIdx CurrentCustomer;
	CEposReportConsolPluSalesCustomerSortIdx CurrentPluDeptTax;
	
	int nSize = m_ConsolArraySortIdx.GetSize();
	StatusProgress.Lock( TRUE, "Creating Report" );

	CEposReportPluSalesInfo arrayCustomerTotals[MAX_PLU_PRICES];
	CEposReportPluSalesInfo arrayGrandTotalsCustomer[MAX_PLU_PRICES];
	CEposReportPluSalesInfo arrayGrandTotalsOther[MAX_PLU_PRICES];

	m_ReportFilters.ResetDbIdx();
	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolPluSalesCustomerSortIdx ConsolInfo;
		m_ConsolArraySortIdx.GetAt( i, ConsolInfo );

		//LOCATION HAS CHANGED, SO WE MAY NEED TO READ THE DATABASE FILES
		if ( ConsolInfo.CompareLocation( CurrentLocation ) == FALSE )
		{
			int nDbIdx;
			int nDbNo = dbLocation.GetDbNo( dbLocation.GetLocIdxFromSortIdx ( CurrentLocation.m_nLocSortIdx ) );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			{
				m_ReportFilters.SetDatabase( nDbIdx );
			}
			
			CurrentLocation = ConsolInfo;
		}

		//CUSTOMER HAS CHANGED SO WRITE TOTALS FOR OLD CUSTOMER & HEADER FOR NEW ONE
		if ( ConsolInfo.CompareCustomer( CurrentCustomer ) == FALSE )
		{
			if ( nCurrentBlockStart != -1 )
			{
				if ( CurrentCustomer.m_nCustomerID == 0 )
					WritePluItemTotals( CurrentPluDeptTax.m_nTaxBand, nCurrentBlockStart, i - 1, arrayCustomerTotals, arrayGrandTotalsOther, FALSE );
				else
				{
					WritePluItemTotals( CurrentPluDeptTax.m_nTaxBand, nCurrentBlockStart, i - 1, arrayCustomerTotals, arrayGrandTotalsCustomer, TRUE );
					WriteConsolidatedTotals( "Customer Total", arrayCustomerTotals, TRUE, FALSE );
				}

				for ( int p = 0 ; p < MAX_PLU_PRICES ; p++ )
					arrayCustomerTotals[p].Clear();
			}

			nCurrentBlockStart = i;
			CurrentCustomer = ConsolInfo;
			CurrentPluDeptTax = ConsolInfo;

			if ( 0 != CurrentCustomer.m_nCustomerID )
			{
				int nLocIdx = dbLocation.GetLocIdxFromSortIdx( ConsolInfo.m_nLocSortIdx );

				CString strHeader = "";
				if ( EcrmanOptions.GetFeaturesCustomerBalanceFlag() == TRUE )
				{
					if ( 0 == ConsolInfo.m_nSortIdxType )
					{
						CCustomerSelectionListAccountInfo infoSelect;
						infoSelect.m_nLocIdx = nLocIdx ;
						infoSelect.m_nCustomerID = ConsolInfo.m_nCustomerID;
						DataManagerNonDb.CustomerNameTable.Account.GetCustomerStatus( infoSelect );

						strHeader.Format( "<..>%s, Account Customer %8.8d, %s",
							dbLocation.GetName( nLocIdx ), 
							ConsolInfo.m_nCustomerID,
							infoSelect.GetReportName() );
					}
					else if ( 1 == ConsolInfo.m_nSortIdxType )
					{
						CCustomerSelectionListAccountInfo infoSelect;
						infoSelect.m_nLocIdx = nLocIdx ;
						infoSelect.m_nCustomerID = ConsolInfo.m_nCustomerID;
						DataManagerNonDb.CustomerNameTable.OneOff.GetCustomerStatus( infoSelect );

						strHeader.Format( "<..>%s, One Off Customer %8.8d, %s",
							dbLocation.GetName( nLocIdx ), 
							ConsolInfo.m_nCustomerID,
							infoSelect.GetReportName() );
					}
				}
					
				if ( strHeader == "" )
				{
					strHeader.Format( "<..>%s, Unknown Customer %8.8d",
						dbLocation.GetName( nLocIdx ), 
						ConsolInfo.m_nCustomerID );
				}

				m_ReportFile.RequestBlankLines(2);
				m_ReportFile.WriteReportMiscLine( strHeader );
				m_ReportFile.WriteReportMiscLine( "<LI>" );
			}
		}

		if ( CurrentPluDeptTax.ComparePluDeptTax( ConsolInfo ) == FALSE )
		{
			if ( nCurrentBlockStart != -1 )
			{
				if ( 0 == CurrentCustomer.m_nCustomerID )
					WritePluItemTotals( CurrentPluDeptTax.m_nTaxBand, nCurrentBlockStart, i - 1, arrayCustomerTotals, arrayGrandTotalsOther, FALSE );
				else
					WritePluItemTotals( CurrentPluDeptTax.m_nTaxBand, nCurrentBlockStart, i - 1, arrayCustomerTotals, arrayGrandTotalsCustomer, TRUE );
			}

			nCurrentBlockStart = i;
			CurrentPluDeptTax = ConsolInfo;
		}
	}

	StatusProgress.Unlock();

	if ( nCurrentBlockStart != -1 )
	{
		if ( 0 == CurrentCustomer.m_nCustomerID )
			WritePluItemTotals( CurrentPluDeptTax.m_nTaxBand, nCurrentBlockStart, nSize - 1, arrayCustomerTotals, arrayGrandTotalsOther, FALSE );
		else
			WritePluItemTotals( CurrentPluDeptTax.m_nTaxBand, nCurrentBlockStart, nSize - 1, arrayCustomerTotals, arrayGrandTotalsCustomer, TRUE );
	}

	if ( CurrentCustomer.m_nCustomerID > 0 )
		WriteConsolidatedTotals( "Customer Total", arrayCustomerTotals, TRUE, FALSE );
	
	CEposReportPluSalesInfo arrayGrandTotals[MAX_PLU_PRICES];
	
	for ( int nLevel = 0; nLevel < MAX_PLU_PRICES; nLevel++ )
	{
		arrayGrandTotals[ nLevel ].m_dQty =					arrayGrandTotalsCustomer[ nLevel ].m_dQty;
		arrayGrandTotals[ nLevel ].m_dValBeforeDiscount =	arrayGrandTotalsCustomer[ nLevel ].m_dValBeforeDiscount;
		arrayGrandTotals[ nLevel ].m_dDiscountAmount =		arrayGrandTotalsCustomer[ nLevel ].m_dDiscountAmount;
		arrayGrandTotals[ nLevel ].m_dPremiumAmount =		arrayGrandTotalsCustomer[ nLevel ].m_dPremiumAmount;
		arrayGrandTotals[ nLevel ].m_dTax =					arrayGrandTotalsCustomer[ nLevel ].m_dTax;
		arrayGrandTotals[ nLevel ].m_dCost =				arrayGrandTotalsCustomer[ nLevel ].m_dCost;
		arrayGrandTotals[ nLevel ].m_bGotData =				arrayGrandTotalsCustomer[ nLevel ].m_bGotData;

		arrayGrandTotals[ nLevel ].m_dQty +=				arrayGrandTotalsOther[ nLevel ].m_dQty;
		arrayGrandTotals[ nLevel ].m_dValBeforeDiscount +=	arrayGrandTotalsOther[ nLevel ].m_dValBeforeDiscount;
		arrayGrandTotals[ nLevel ].m_dDiscountAmount +=		arrayGrandTotalsOther[ nLevel ].m_dDiscountAmount;
		arrayGrandTotals[ nLevel ].m_dPremiumAmount +=		arrayGrandTotalsOther[ nLevel ].m_dPremiumAmount;
		arrayGrandTotals[ nLevel ].m_dTax +=				arrayGrandTotalsOther[ nLevel ].m_dTax;
		arrayGrandTotals[ nLevel ].m_dCost +=				arrayGrandTotalsOther[ nLevel ].m_dCost;
		arrayGrandTotals[ nLevel ].m_bGotData |=			arrayGrandTotalsOther[ nLevel ].m_bGotData;
	}

	m_ReportFile.RequestBlankLines(2);
	WriteConsolidatedTotals( "Sales Total (Customer)", arrayGrandTotalsCustomer, TRUE, FALSE );
	WriteConsolidatedTotals( "Sales Total (Other)", arrayGrandTotalsOther, FALSE, TRUE );
	WriteConsolidatedTotals( "Grand Total", arrayGrandTotals, TRUE, TRUE  );
	
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportPluSalesCustomer::WritePluItemTotals( int nTaxBand, int nStart, int nEnd, CEposReportPluSalesInfo arrayConsolidatedTotals1[], CEposReportPluSalesInfo arrayConsolidatedTotals2[], bool bShowInReport )
{
	if ( ( nTaxBand < 0 ) || ( nTaxBand > MAX_TAX_BANDS ) )
		nTaxBand = 0;
		
	CEposReportPluSalesInfo arrayPluItemTotals[MAX_PLU_PRICES];

	CEposReportConsolPluSalesCustomerSortIdx ConsolInfoStart;
	m_ConsolArraySortIdx.GetAt( nStart, ConsolInfoStart );

	bool bReportCost = DealerFlags.GetReportCostPriceFlag();
	double dItemCost = 0.0;
	
	CString strReportText = "";
	{
		m_PluInfoFinder.SetPluNo( ConsolInfoStart.m_nPluNo );
		if ( FALSE == bReportCost ) dItemCost = m_PluInfoFinder.GetCost();
		strReportText = m_PluInfoFinder.GetPluInfoRepText(TRUE);
	}
	
	for ( int i = nStart; i <= nEnd; i++ )
	{
		CEposReportConsolPluSalesCustomerSortIdx ConsolInfoLine;
		m_ConsolArraySortIdx.GetAt( i, ConsolInfoLine );

		if ((ConsolInfoLine.m_nPriceBand < 0) || (ConsolInfoLine.m_nPriceBand >= MAX_PLU_PRICES))
		{
			ConsolInfoLine.m_nPriceBand = 0;
		}

		if (TRUE == bReportCost)
		{
			dItemCost = ConsolInfoLine.m_dItemCost;
		}

		double dNet = ConsolInfoLine.m_dValBeforeDiscount - ConsolInfoLine.m_dDiscountVal + ConsolInfoLine.m_dPremiumVal;
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( ConsolInfoLine.m_nTaxDateCode, nTaxBand );
		double dTax = dNet - ReportHelpers.CalcNonTax ( dNet, dTaxRate );
		double dCost = dItemCost * ConsolInfoLine.m_dQty;
		
		arrayPluItemTotals[ ConsolInfoLine.m_nPriceBand ].m_dQty += ConsolInfoLine.m_dQty;
		arrayPluItemTotals[ ConsolInfoLine.m_nPriceBand ].m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		arrayPluItemTotals[ ConsolInfoLine.m_nPriceBand ].m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		arrayPluItemTotals[ ConsolInfoLine.m_nPriceBand ].m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		arrayPluItemTotals[ ConsolInfoLine.m_nPriceBand ].m_dTax += dTax;
		arrayPluItemTotals[ ConsolInfoLine.m_nPriceBand ].m_dCost += dCost;
		arrayPluItemTotals[ ConsolInfoLine.m_nPriceBand ].m_bGotData = TRUE;

		arrayConsolidatedTotals1[ ConsolInfoLine.m_nPriceBand ].m_dQty += ConsolInfoLine.m_dQty;
		arrayConsolidatedTotals1[ ConsolInfoLine.m_nPriceBand ].m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		arrayConsolidatedTotals1[ ConsolInfoLine.m_nPriceBand ].m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		arrayConsolidatedTotals1[ ConsolInfoLine.m_nPriceBand ].m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		arrayConsolidatedTotals1[ ConsolInfoLine.m_nPriceBand ].m_dTax += dTax;
		arrayConsolidatedTotals1[ ConsolInfoLine.m_nPriceBand ].m_dCost += dCost;
		arrayConsolidatedTotals1[ ConsolInfoLine.m_nPriceBand ].m_bGotData = TRUE;

		arrayConsolidatedTotals2[ ConsolInfoLine.m_nPriceBand ].m_dQty += ConsolInfoLine.m_dQty;
		arrayConsolidatedTotals2[ ConsolInfoLine.m_nPriceBand ].m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		arrayConsolidatedTotals2[ ConsolInfoLine.m_nPriceBand ].m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		arrayConsolidatedTotals2[ ConsolInfoLine.m_nPriceBand ].m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		arrayConsolidatedTotals2[ ConsolInfoLine.m_nPriceBand ].m_dTax += dTax;
		arrayConsolidatedTotals2[ ConsolInfoLine.m_nPriceBand ].m_dCost += dCost;
		arrayConsolidatedTotals2[ ConsolInfoLine.m_nPriceBand ].m_bGotData = TRUE;
	}

	if ( TRUE == bShowInReport )
	{
		for ( int p = 0; p < MAX_PLU_PRICES; p++ )
		{
			if ( ( 0 == p ) || ( TRUE == arrayPluItemTotals[p].m_bGotData ) )
			{
				CCSV csv ( '\t' );
			
				if ( 0 == p )
				{
					csv.Add ( ReportHelpers.GetDisplayPluNo ( ConsolInfoStart.m_nPluNo, TRUE ) );
					csv.Add ( strReportText );
					csv.Add ( ConsolInfoStart.m_nDeptNo );
				}
				else
				{
					CString strPriceLevel;
					strPriceLevel.Format ( "<..>--> %s", DataManager.PriceText.GetReportText(p) );
					csv.Add ( "" );
					csv.Add ( strPriceLevel );
					csv.Add ( "" );
				}

				if ( TRUE == arrayPluItemTotals[p].m_bGotData )
				{
					double dQty = arrayPluItemTotals[p].m_dQty;
					double dVal = arrayPluItemTotals[p].m_dValBeforeDiscount;
					double dDiscount = arrayPluItemTotals[p].m_dDiscountAmount;
					double dPremium = arrayPluItemTotals[p].m_dPremiumAmount;
					double dTax = arrayPluItemTotals[p].m_dTax;
					double dCost = arrayPluItemTotals[p].m_dCost;
					double dNet = dVal - dDiscount + dPremium;

					csv.Add( dQty, SysInfo.GetDPQty() );

					if ( TRUE == m_bDiscountDetail )
					{
						csv.Add ( dVal, SysInfo.GetDPValue() );
						csv.Add ( dDiscount, SysInfo.GetDPValue() );

						if (TRUE == m_bPremiumDetail)
						{
							csv.Add(dPremium, SysInfo.GetDPValue());
						}
					}

					csv.Add ( dNet, SysInfo.GetDPValue() );
					csv.Add ( ReportHelpers.CalcAverage ( dQty, dNet ), SysInfo.GetDPValue() );
					csv.Add ( CTaxArray::GetTaxBandFromNumber( nTaxBand ) );
					csv.Add ( dTax, SysInfo.GetDPValue() );

					if (TRUE == m_bNetOfTax)
					{
						csv.Add(dNet - dTax, SysInfo.GetDPValue());
					}

					csv.Add ( dCost, SysInfo.GetDPValue() );
				}

				m_ReportFile.WriteReportDataLine( csv.GetLine() );
			}
		}
	}
}

/**********************************************************************/

void CEposReportPluSalesCustomer::WriteConsolidatedTotals( const char* szType, CEposReportPluSalesInfo arrayConsolidatedTotals[], bool bUnderline, bool bGrandTotalOnly )
{
	CString strType = "<..>";
	strType += szType;

	CUnderlineMaker UnderlineMaker;
	UnderlineMaker.AddBlanks(3);
	UnderlineMaker.AddTotals( m_bDiscountDetail ? 5 : 3 );
	UnderlineMaker.AddTotals( m_bPremiumDetail ? 1 : 0);
	UnderlineMaker.AddBlanks(1);
	UnderlineMaker.AddTotals( m_bNetOfTax ? 3 : 2 );

	if (TRUE == bUnderline)
	{
		m_ReportFile.WriteReportMiscLine(UnderlineMaker.GetLine());
	}

	CEposReportPluSalesInfo GrandTotals;
	bool bDispGrandTotal = bGrandTotalOnly;

	for ( int p = 0 ; p < MAX_PLU_PRICES ; p++ )
	{
		if (p != 0)
		{
			if (TRUE == arrayConsolidatedTotals[p].m_bGotData)
			{
				bDispGrandTotal = TRUE;
			}
		}

		GrandTotals.m_dQty += arrayConsolidatedTotals[p].m_dQty;
		GrandTotals.m_dValBeforeDiscount += arrayConsolidatedTotals[p].m_dValBeforeDiscount;
		GrandTotals.m_dDiscountAmount += arrayConsolidatedTotals[p].m_dDiscountAmount;
		GrandTotals.m_dPremiumAmount += arrayConsolidatedTotals[p].m_dPremiumAmount;
		GrandTotals.m_dTax += arrayConsolidatedTotals[p].m_dTax;
		GrandTotals.m_dCost += arrayConsolidatedTotals[p].m_dCost;
	}

	if ( FALSE == bGrandTotalOnly )
	{
		for ( int p = 0 ; p < MAX_PLU_PRICES ; p++ )
		{
			double dQty = arrayConsolidatedTotals[p].m_dQty;
			double dVal = arrayConsolidatedTotals[p].m_dValBeforeDiscount;
			double dDiscount = arrayConsolidatedTotals[p].m_dDiscountAmount;
			double dPremium = arrayConsolidatedTotals[p].m_dPremiumAmount;
			double dTax = arrayConsolidatedTotals[p].m_dTax;
			double dCost = arrayConsolidatedTotals[p].m_dCost;
			double dNet = dVal - dDiscount + dPremium;

			if ( ( 0 == p ) || ( dQty != 0.0 ) || ( dVal != 0.0 ) )
			{
				CCSV csv ( '\t' );

				if ((p != 0) || (TRUE == bDispGrandTotal))
				{
					csv.Add(DataManager.PriceText.GetReportText(p));
				}
				else
				{
					csv.Add("");
				}

				csv.Add ( strType );
				csv.Add ( "" );
				csv.Add ( dQty, SysInfo.GetDPQty() );

				if ( TRUE == m_bDiscountDetail )
				{
					csv.Add ( dVal, SysInfo.GetDPValue() );
					csv.Add ( dDiscount, SysInfo.GetDPValue() );

					if (TRUE == m_bPremiumDetail)
					{
						csv.Add(dPremium, SysInfo.GetDPValue());
					}
				}
					
				csv.Add ( dNet, SysInfo.GetDPValue() );
				csv.Add ( ReportHelpers.CalcAverage ( dQty, dNet ), SysInfo.GetDPValue() );
				csv.Add ( "" );
				csv.Add ( dTax, SysInfo.GetDPValue() );

				if (TRUE == m_bNetOfTax)
				{
					csv.Add(dNet - dTax, SysInfo.GetDPValue());
				}

				csv.Add ( dCost, SysInfo.GetDPValue() );
				m_ReportFile.WriteReportDataLine ( csv.GetLine() );
			}
		}
	}

	if ( bDispGrandTotal == TRUE )
	{
		if (FALSE == bGrandTotalOnly)
		{
			m_ReportFile.WriteReportMiscLine(UnderlineMaker.GetLine());
		}

		double dQty = GrandTotals.m_dQty;
		double dVal = GrandTotals.m_dValBeforeDiscount;
		double dDiscount = GrandTotals.m_dDiscountAmount;
		double dPremium = GrandTotals.m_dPremiumAmount;
		double dTax = GrandTotals.m_dTax;
		double dCost = GrandTotals.m_dCost;
		double dNet = dVal - dDiscount + dPremium;
		
		CCSV csv ( '\t' );
		csv.Add ( "" );
		csv.Add ( strType );
		csv.Add ( "" );
		csv.Add ( dQty, SysInfo.GetDPQty() );

		if ( TRUE == m_bDiscountDetail )
		{
			csv.Add ( dVal, SysInfo.GetDPValue() );
			csv.Add ( dDiscount, SysInfo.GetDPValue() );

			if (TRUE == m_bPremiumDetail)
			{
				csv.Add(dPremium, SysInfo.GetDPValue());
			}
		}
		
		csv.Add ( dNet, SysInfo.GetDPValue() );
		csv.Add ( ReportHelpers.CalcAverage ( dQty, dNet ), SysInfo.GetDPValue() );
		csv.Add ( "" );
		csv.Add ( dTax, SysInfo.GetDPValue() );

		if (TRUE == m_bNetOfTax)
		{
			csv.Add(dNet - dTax, SysInfo.GetDPValue());
		}

		csv.Add ( dCost, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
	}
}

/**********************************************************************/

