/**********************************************************************/
#include "EposReportConsolParams.h"
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
#include "EposReportPluVipSalesDept.h"
/**********************************************************************/

CEposReportPluVipSalesDept::CEposReportPluVipSalesDept( CEposSelectArray& SelectArray, int nReportType ) : CEposReport( SelectArray )
{
	switch( nReportType )
	{
	case REPORT_TYPE_PLUWASTE_DEPT_ITEM:
		m_nReportType = nReportType;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = FALSE;
		m_bPremiumDetail = FALSE;
		m_bVIPReport = FALSE;
		break;

	case REPORT_TYPE_VIPSALES_DEPT_ITEM:
		m_nReportType = nReportType;
		m_bAllowValue = EcrmanOptions.GetReportsVIPRetailFlag();
		m_bDiscountDetail = m_bAllowValue && EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = m_bAllowValue && EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = TRUE;
		break;

	case REPORT_TYPE_PLUSALES_DEPT_ITEM:
	default:
		m_nReportType = REPORT_TYPE_PLUSALES_DEPT_ITEM;
		m_bAllowValue = TRUE;
		m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
		m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
		m_bVIPReport = FALSE;
		break;
	}

	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

CEposReportPluVipSalesDept::~CEposReportPluVipSalesDept()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
		delete ( m_arrayReportBlocks[n].m_pItems );
	}
}

/**********************************************************************/

void CEposReportPluVipSalesDept::InitialiseConsolidationBlocks()
{
	CString strParams = "";
	switch( m_nPreferConsolType )
	{
	case EPOSREPORT_CONSOL_SAVED:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSaved();
		break;

	case EPOSREPORT_CONSOL_ADHOC:
		strParams = m_EposReportSelectInfo.GetConsolPrefsAdhoc();
		break;
		
	case EPOSREPORT_CONSOL_DEFAULT:
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		break;
		
	case EPOSREPORT_CONSOL_SYSTEM:
	default:
		strParams = m_EposReportSelectInfo.GetConsolPrefsSummary();
		break;
	}

	CEposReportConsolParamsPluSales Params;
	Params.LoadSettings( strParams );

	if ( ( Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}
		
	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( FALSE );
	m_BlockMap.BuildMap( m_SelectArray );
	
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportPluSalesDeptBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolPluSalesDept>;
		block.m_pItems = new CReportConsolidationArray<CEposReportConsolPluSalesDept>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPluVipSalesDept::Consolidate()
{
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	m_ReportFilters.ResetDbIdx();
	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		m_BlockMap.GetBlockListForTerminal( infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks );

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

				csv.ParseLine ( strBuffer );

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

				//FILTER FOR VIP SALE
				if ( TRUE == m_bVIPReport )
				{
					if ( csv.IsVIPSale() == FALSE )
						continue;
				}

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
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_PROMO_TAXABLE:
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

						if ( infoPluNo.m_nModifier != 0 )
							dMultiplier = DataManager.Modifier.GetMultiplierForQty ( infoPluNo.m_nModifier );

						nPluNo = infoPluNo.m_nBasePluNo;
						nModifier = infoPluNo.m_nModifier;
					}
					break;
				}

				CEposReportConsolPluSalesDept ConsolInfo;
				ConsolInfo.m_nPluNo = nPluNo;
				ConsolInfo.m_nDeptNo = nDeptNo;
				ConsolInfo.m_nTaxDateCode = nTaxDateCode;
				
				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
					ConsolInfo.m_nTaxBand = -1;
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				else
					ConsolInfo.m_nTaxBand = nModifier; //CONVERT TO TAX BAND LATER

				if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
					ConsolInfo.m_nLocIdx = infoFile.m_nLocIdx;
				else
					ConsolInfo.m_nLocIdx = 0;
				
				switch ( nLineType )
				{
				case CSVPLUDATA_PLU_DISCOUNT:
				case CSVPLUDATA_PLU_MIXMATCH:
					ConsolInfo.m_dQty = 0.0;
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);
					break;

				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
					ConsolInfo.m_dQty = csv.GetSaleQty();
					ConsolInfo.m_dValBeforeDiscount = 0.0;
					ConsolInfo.SetDiscountOrPremium(csv, m_bPremiumDetail);					
					break;

				case CSVPLUDATA_PLU_SALE:
					ConsolInfo.m_dQty = csv.GetSaleQty() * dMultiplier;
					ConsolInfo.m_dValBeforeDiscount = csv.GetValue();
					ConsolInfo.m_dDiscountVal = 0.0;
					ConsolInfo.m_dPremiumVal = 0.0;
					break;
				}

				if ( (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE) || ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType )  )
				{
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
					}
				}
				else
				{
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						m_arrayReportBlocks[ nBlock ].m_pItems -> Consolidate( ConsolInfo );
					}
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();

	if ( DealerFlags.GetReportCostPriceFlag() == TRUE )
	{
		CReportCostFinder CostFinder;

		__int64 nProgressTarget = 0;
		__int64 nProgressCount = 0;

		for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
		{
			nProgressTarget += m_arrayReportBlocks[n].m_pSales -> GetSize();
			nProgressTarget += m_arrayReportBlocks[n].m_pItems -> GetSize();
		}

		StatusProgress.Lock( TRUE, "Calculating cost prices" );

		m_ReportFilters.ResetDbIdx();
		for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
		{
			int nDbIdx = m_arrayReportBlocks[n].m_nDbIdx;
			
			if ( m_ReportFilters.SetDatabase( nDbIdx ) == TRUE )
			{
				CostFinder.Init();
			}

			for ( int s = 0; s < m_arrayReportBlocks[n].m_pSales -> GetSize(); s++ )
			{
				StatusProgress.SetPos( nProgressCount++, nProgressTarget );

				CEposReportConsolPluSalesDept Sales;
				m_arrayReportBlocks[n].m_pSales -> GetAt( s, Sales );
				Sales.m_dItemCost = CostFinder.GetPluCostByPluNo( Sales.m_nPluNo, Sales.m_nLocIdx );
				m_arrayReportBlocks[n].m_pSales -> SetAt( s, Sales );
			}

			for ( int s = 0; s < m_arrayReportBlocks[n].m_pItems -> GetSize(); s++ )
			{
				StatusProgress.SetPos( nProgressCount++, nProgressTarget );

				CEposReportConsolPluSalesDept Sales;
				m_arrayReportBlocks[n].m_pItems -> GetAt( s, Sales );
				Sales.m_dItemCost = CostFinder.GetPluCostByPluNo( Sales.m_nPluNo, Sales.m_nLocIdx );
				m_arrayReportBlocks[n].m_pItems -> SetAt( s, Sales );
			}
		}

		StatusProgress.Unlock();
	}
}

/**********************************************************************/

bool CEposReportPluVipSalesDept::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_UnderlineMaker.Reset();

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	AddReportColumn ( "Plu Number", TA_LEFT, 400, FALSE );
	AddReportColumn ( "Description", TA_LEFT, 350, FALSE );
	AddReportColumn ( "Quantity", TA_RIGHT, 250, TRUE );

	if ( TRUE == m_bAllowValue )
	{
		if ( IsWastageReport() == TRUE )
			AddReportColumn ( "Waste", TA_RIGHT, 250, TRUE );
		else
		{
			AddReportColumn ( "Sales", TA_RIGHT, 250, TRUE );

			if ( TRUE == m_bDiscountDetail )
			{
				AddReportColumn ( "Discount", TA_RIGHT, 250, TRUE );

				if (TRUE == m_bPremiumDetail)
				{
					AddReportColumn("Premium", TA_RIGHT, 250, TRUE);
				}

				AddReportColumn ( "Retail", TA_RIGHT, 250, TRUE );
			}
		}

		AddReportColumn ( "Average", TA_RIGHT, 250, TRUE );
		AddReportColumn ( EcrmanOptions.GetTaxBandString(), TA_RIGHT, 200, FALSE );
		AddReportColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250, TRUE );

		if ( TRUE == m_bNetOfTax )
			AddReportColumn ( "Est. Net", TA_RIGHT, 250, TRUE );
	}

	AddReportColumn ( "Est. Cost", TA_RIGHT, 250, TRUE );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPluSalesDeptBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	int nTotalBlocks = 0;
	m_BlockMap.BuildSortArray();

	m_ReportFilters.ResetDbIdx();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPluSalesDeptBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		if ( TRUE == ReportBlock.m_bAddToTotal )
			nTotalBlocks++;

		m_ReportFilters.SetDatabase( ReportBlock.m_nDbIdx );

		m_ReportFile.HandleNewPage();
	
		CString strHeader = "";
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.LockBlankLines( 1 );
	
		CreateSalesSection( ReportBlock );
		WriteBlockTotals( ReportBlock );
	}

	StatusProgress.Unlock();

	if ( nTotalBlocks >= 2 )
	{
		m_ReportFile.RequestBlankLines(2);
		m_ReportFile.WriteReportMiscLine( "<..>Grand Total" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		WriteConsolidatedTotals( m_infoGrandTotals, "Grand", FALSE, TRUE );
	}

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportPluVipSalesDept::CreateSalesSection( CEposReportPluSalesDeptBlock& Block )
{
	for ( int s = Block.m_pItems -> GetSize() - 1; s >= 0; s-- )
	{
		CEposReportConsolPluSalesDept ConsolInfo;
		Block.m_pItems -> GetAt( s, ConsolInfo );
		UpdateCreateReportProgress();
		
		m_PluInfoFinder.SetPluNo( ConsolInfo.m_nPluNo );
		ConsolInfo.m_nTaxBand = m_PluInfoFinder.GetModifierTaxBand( ConsolInfo.m_nTaxBand );

		Block.m_pSales -> Consolidate( ConsolInfo );
		Block.m_pItems -> RemoveAt( s );
	}

	int nCurrentDeptStart = -1;
	int nCurrentItemStart = -1;
	CEposReportConsolPluSalesDept CurrentDept;
	CEposReportConsolPluSalesDept CurrentPluNoTax;

	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolPluSalesDept Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		if ( CurrentDept.CompareDeptNo( Sales ) == FALSE )
		{
			if ( nCurrentItemStart != -1 )
			{
				CalculatePluItemTotals( Block, CurrentPluNoTax.m_nTaxBand, nCurrentItemStart, s - 1 );
				WritePluItemTotals( Block, CurrentPluNoTax.m_nTaxBand );
			}

			if ( nCurrentDeptStart != -1 )
				WriteDepartmentTotal( Block, CurrentDept.m_nDeptNo );
				
			nCurrentDeptStart = s;
			nCurrentItemStart = s;

			Block.m_infoDeptTotals.Clear();

			CurrentDept = Sales;
			CurrentPluNoTax = Sales;

			CString strHeader;
			strHeader.Format( "<..>D%3.3d, %s",
				Sales.m_nDeptNo,
				DataManager.Department.GetReportTextByDeptNo( Sales.m_nDeptNo ) );
			
			m_ReportFile.RequestBlankLines( 2 );
			m_ReportFile.WriteReportMiscLine( strHeader );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
		}

		if ( CurrentPluNoTax.ComparePluNoTax( Sales ) == FALSE )
		{
			if ( nCurrentItemStart != -1 )
			{
				CalculatePluItemTotals( Block, CurrentPluNoTax.m_nTaxBand, nCurrentItemStart, s - 1 );
				WritePluItemTotals( Block, CurrentPluNoTax.m_nTaxBand );
			}
		
			nCurrentItemStart = s;
			CurrentPluNoTax = Sales;
		}
	}

	if ( nCurrentItemStart != -1 )
	{
		CalculatePluItemTotals( Block, CurrentPluNoTax.m_nTaxBand, nCurrentItemStart, Block.m_pSales -> GetSize() - 1 );
		WritePluItemTotals( Block, CurrentPluNoTax.m_nTaxBand );
	}

	if ( nCurrentDeptStart != -1 )
		WriteDepartmentTotal( Block, CurrentDept.m_nDeptNo );
}

/**********************************************************************/

void CEposReportPluVipSalesDept::CalculatePluItemTotals( CEposReportPluSalesDeptBlock& Block, int nTaxBand, int nStart, int nEnd )
{
	Block.m_infoCurrentPluTotals.Clear();

	if ( ( nTaxBand < 0 ) || ( nTaxBand > MAX_TAX_BANDS ) )
		nTaxBand = 0;

	CEposReportConsolPluSalesDept ConsolInfoStart;
	Block.m_pSales -> GetAt( nStart, ConsolInfoStart );
	Block.m_nCurrentPluNo = ConsolInfoStart.m_nPluNo;

	//FETCH PER ITEM COST IF AT DATABASE LEVEL
	double dItemCost = 0.0;
	bool bReportCost = DealerFlags.GetReportCostPriceFlag();
	if ( FALSE == bReportCost )
	{
		m_PluInfoFinder.SetPluNo( Block.m_nCurrentPluNo );
		dItemCost = m_PluInfoFinder.GetCost();
	}

	for ( int i = nStart; i <= nEnd; i++ )
	{		
		CEposReportConsolPluSalesDept ConsolInfoLine;
		Block.m_pSales -> GetAt( i, ConsolInfoLine );

		if ( TRUE == bReportCost )
			dItemCost = ConsolInfoLine.m_dItemCost;
	
		double dNet = ConsolInfoLine.m_dValBeforeDiscount - ConsolInfoLine.m_dDiscountVal + ConsolInfoLine.m_dPremiumVal;
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( ConsolInfoLine.m_nTaxDateCode, nTaxBand );
		double dTax = dNet - ReportHelpers.CalcNonTax ( dNet, dTaxRate );
		double dCost = dItemCost * ConsolInfoLine.m_dQty;	

		CEposReportPluSalesInfo PluSalesInfo;
		PluSalesInfo.m_dQty = ConsolInfoLine.m_dQty;
		PluSalesInfo.m_dValBeforeDiscount = ConsolInfoLine.m_dValBeforeDiscount;
		PluSalesInfo.m_dDiscountAmount = ConsolInfoLine.m_dDiscountVal;
		PluSalesInfo.m_dPremiumAmount = ConsolInfoLine.m_dPremiumVal;
		PluSalesInfo.m_dTax = dTax;
		PluSalesInfo.m_dCost = dCost;
		PluSalesInfo.m_bGotData = TRUE;

		Block.m_infoCurrentPluTotals.Consolidate( PluSalesInfo );
		Block.m_infoDeptTotals.Consolidate( PluSalesInfo );
		Block.m_infoBlockTotals.Consolidate( PluSalesInfo );

		if ( TRUE == Block.m_bAddToTotal )
			m_infoGrandTotals.Consolidate( PluSalesInfo );
	}
}

/**********************************************************************/

void CEposReportPluVipSalesDept::WritePluItemTotals( CEposReportPluSalesDeptBlock& Block, int nTaxBand )
{
	double dQty =		Block.m_infoCurrentPluTotals.m_dQty;
	double dVal =		Block.m_infoCurrentPluTotals.m_dValBeforeDiscount;
	double dDiscount =	Block.m_infoCurrentPluTotals.m_dDiscountAmount;
	double dPremium =	Block.m_infoCurrentPluTotals.m_dPremiumAmount;
	double dTax =		Block.m_infoCurrentPluTotals.m_dTax;
	double dCost =		Block.m_infoCurrentPluTotals.m_dCost;
	double dNet =		dVal - dDiscount + dPremium;
		
	CCSV csv ( '\t' );	
	m_PluInfoFinder.SetPluNo( Block.m_nCurrentPluNo );
	csv.Add ( ReportHelpers.GetDisplayPluNo ( Block.m_nCurrentPluNo, TRUE ) );
	csv.Add ( m_PluInfoFinder.GetPluInfoRepText(TRUE) );			
	csv.Add ( dQty, SysInfo.GetDPQty() );

	if (TRUE == m_bAllowValue)
	{
		if (TRUE == m_bDiscountDetail)
		{
			csv.Add(dVal, SysInfo.GetDPValue());
			csv.Add(dDiscount, SysInfo.GetDPValue());

			if (TRUE == m_bPremiumDetail)
			{
				csv.Add(dPremium, SysInfo.GetDPValue());
			}
		}

		csv.Add(dNet, SysInfo.GetDPValue());
		csv.Add(ReportHelpers.CalcAverage(dQty, dNet), SysInfo.GetDPValue());
		csv.Add(CTaxArray::GetTaxBandFromNumber(nTaxBand));
		csv.Add(dTax, SysInfo.GetDPValue());

		if (TRUE == m_bNetOfTax)
		{
			csv.Add(dNet - dTax, SysInfo.GetDPValue());
		}
	}

	csv.Add ( dCost, SysInfo.GetDPValue() );
	
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPluVipSalesDept::WriteDepartmentTotal ( CEposReportPluSalesDeptBlock& Block, int nDeptNo )
{
	CString strText;
	strText.Format ( "D%3.3d %s", nDeptNo, DataManager.Department.GetReportTextByDeptNo( nDeptNo ) );
	WriteConsolidatedTotals ( Block.m_infoDeptTotals, strText, TRUE, FALSE );	
}

/**********************************************************************/

void CEposReportPluVipSalesDept::WriteBlockTotals( CEposReportPluSalesDeptBlock& Block )
{
	CString strType = GetTotalTypeName( FALSE, Block.m_nEntityType );
	m_ReportFile.RequestBlankLines(1);
	WriteConsolidatedTotals( Block.m_infoBlockTotals, strType, FALSE, FALSE );
}

/**********************************************************************/

void CEposReportPluVipSalesDept::WriteConsolidatedTotals ( CEposReportPluSalesInfo& Totals, const char* szType, bool bDeptTotal, bool bGrandTotal )
{
	if ( FALSE == bGrandTotal )
	{
		m_ReportFile.WriteReportMiscLine( m_UnderlineMaker.GetLine() );
	}

	CString strType = szType;

	if ( FALSE == bDeptTotal )
	{
		if ( strType != "" ) strType += " ";
		strType += "Total";
	}

	double dNet = Totals.m_dValBeforeDiscount - Totals.m_dDiscountAmount + Totals.m_dPremiumAmount;

	CCSV csv ( '\t' );
	csv.Add ( "" );
	csv.Add ( strType );
	csv.Add ( Totals.m_dQty, SysInfo.GetDPQty() );

	if ( TRUE == m_bAllowValue )
	{
		if ( TRUE == m_bDiscountDetail )
		{
			csv.Add ( Totals.m_dValBeforeDiscount, SysInfo.GetDPValue() );
			csv.Add ( Totals.m_dDiscountAmount, SysInfo.GetDPValue() );

			if (TRUE == m_bPremiumDetail)
			{
				csv.Add(Totals.m_dPremiumAmount, SysInfo.GetDPValue());
			}
		}

		csv.Add ( dNet, SysInfo.GetDPValue() );
		csv.Add ( ReportHelpers.CalcAverage ( Totals.m_dQty, dNet ), SysInfo.GetDPValue() );
		csv.Add ( "" );
		csv.Add ( Totals.m_dTax, SysInfo.GetDPValue() );

		if ( TRUE == m_bNetOfTax )
			csv.Add ( dNet - Totals.m_dTax, SysInfo.GetDPValue() );
	}

	csv.Add ( Totals.m_dCost, SysInfo.GetDPValue() );
	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
}

/**********************************************************************/
