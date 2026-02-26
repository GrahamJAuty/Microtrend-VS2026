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
/**********************************************************************/
#include "EposReportPluSalesTax.h"
/**********************************************************************/

CEposReportPluSalesTax::CEposReportPluSalesTax( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_PLUSALES_TAX_ITEM;
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

CEposReportPluSalesTax::~CEposReportPluSalesTax()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		m_arrayReportBlocks[n].m_pItems -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
		delete ( m_arrayReportBlocks[n].m_pItems );
	}
}

/**********************************************************************/

void CEposReportPluSalesTax::InitialiseConsolidationBlocks()
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

		CEposReportPluSalesTaxBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolPluSalesTax>;
		block.m_pItems = new CReportConsolidationArray<CEposReportConsolPluSalesTax>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPluSalesTax::Consolidate()
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

				int nPriceBand = csv.GetPriceBand();
				if ( ( nPriceBand < 0 ) || ( nPriceBand >= MAX_PLU_PRICES ) )
					nPriceBand = 0;

				CEposReportConsolPluSalesTax ConsolInfo;

				ConsolInfo.m_nPluNo = nPluNo;
				ConsolInfo.m_nTaxDateCode = nTaxDateCode;
				
				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
					ConsolInfo.m_nTaxBand = -1;
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				else
					ConsolInfo.m_nTaxBand = nModifier;	//WILL CONVERT TO TAX BAND LATER

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
		int nLastDbIdx = -1;
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
			
			if ( m_ReportFilters.SetDatabase(nDbIdx) == TRUE )
			{
				CostFinder.Init();
			}

			for ( int s = 0; s < m_arrayReportBlocks[n].m_pItems -> GetSize(); s++ )
			{
				StatusProgress.SetPos( nProgressCount++, nProgressTarget );

				CEposReportConsolPluSalesTax Sales;
				m_arrayReportBlocks[n].m_pItems -> GetAt( s, Sales );
				Sales.m_dItemCost = CostFinder.GetPluCostByPluNo( Sales.m_nPluNo, Sales.m_nLocIdx );
				m_arrayReportBlocks[n].m_pItems -> SetAt( s, Sales );
			}

			for ( int s = 0; s < m_arrayReportBlocks[n].m_pSales -> GetSize(); s++ )
			{
				StatusProgress.SetPos( nProgressCount++, nProgressTarget );

				CEposReportConsolPluSalesTax Sales;
				m_arrayReportBlocks[n].m_pSales -> GetAt( s, Sales );
				Sales.m_dItemCost = CostFinder.GetPluCostByPluNo( Sales.m_nPluNo, Sales.m_nLocIdx );
				m_arrayReportBlocks[n].m_pSales -> SetAt( s, Sales );
			}
		}

		StatusProgress.Unlock();
	}
}

/**********************************************************************/

bool CEposReportPluSalesTax::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Plu Number", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 250 );

	if ( IsWastageReport() == TRUE )
		m_ReportFile.AddColumn ( "Waste", TA_RIGHT, 250 );
	else
	{
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
	}

	m_ReportFile.AddColumn ( "Average", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250 );

	if ( TRUE == m_bNetOfTax )
		m_ReportFile.AddColumn ( "Est. Net", TA_RIGHT, 250 );

	m_ReportFile.AddColumn ( "Est. Cost", TA_RIGHT, 250 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPluSalesTaxBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	int nTotalBlocks = 0;
	m_BlockMap.BuildSortArray();
	for (int n = 0; n < m_BlockMap.GetSortArraySize(); n++)
	{
		m_arrayTaxBandTotals.RemoveAll();

		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPluSalesTaxBlock ReportBlock = m_arrayReportBlocks.GetAt(nIdx);

		if (ReportBlock.GotData() == FALSE)
			continue;

		if (TRUE == ReportBlock.m_bAddToTotal)
			nTotalBlocks++;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly(ReportBlock.m_nDbIdx, info, FALSE);

		m_ReportFile.HandleNewPage();

		CString strHeader = "";
		ReportBlock.GetHeaderText(strHeader);
		m_ReportFile.WriteReportMiscLine(strHeader);
		m_ReportFile.WriteReportMiscLine("<LI>");
		m_ReportFile.LockBlankLines(1);

		CreateSalesSection(ReportBlock);
		
		if (m_arrayTaxBandTotals.GetSize() > 1)
		{
			m_ReportFile.RequestBlankLines(2);

			for (int n = 0; n < m_arrayTaxBandTotals.GetSize(); n++)
			{
				m_ReportFile.WriteReportDataLine(m_arrayTaxBandTotals.GetAt(n));
			}
			m_ReportFile.LockBlankLines(0);
		}
		
		WriteBlockTotals(ReportBlock);
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

void CEposReportPluSalesTax::CreateSalesSection(CEposReportPluSalesTaxBlock& Block)
{
	for (int s = Block.m_pItems->GetSize() - 1; s >= 0; s--)
	{
		CEposReportConsolPluSalesTax ConsolInfo;
		Block.m_pItems->GetAt(s, ConsolInfo);
		UpdateCreateReportProgress();

		m_PluInfoFinder.SetPluNo(ConsolInfo.m_nPluNo);
		ConsolInfo.m_nTaxBand = m_PluInfoFinder.GetModifierTaxBand(ConsolInfo.m_nTaxBand);

		Block.m_pSales->Consolidate(ConsolInfo);
		Block.m_pItems->RemoveAt(s);
	}

	CEposReportConsolPluSalesTax CurrentTaxBand;
	CurrentTaxBand.m_nTaxBand = -2;
	CurrentTaxBand.m_nTaxDateCode = 0;
	CurrentTaxBand.m_nPluNo = 0;

	CEposReportConsolPluSalesTax CurrentPluNo = CurrentTaxBand;

	int nCurrentTaxBandStart = -1;
	int nCurrentItemStart = -1;

	for (int s = 0; s < Block.m_pSales->GetSize(); s++)
	{
		CEposReportConsolPluSalesTax Sales;
		Block.m_pSales->GetAt(s, Sales);
		UpdateCreateReportProgress();

		if (CurrentTaxBand.CompareTaxBand(Sales) == FALSE)
		{
			if (nCurrentItemStart != -1)
			{
				CalculatePluItemTotals(Block, nCurrentItemStart, s - 1);
				WritePluItemTotals(Block);
			}

			if (nCurrentTaxBandStart != -1)
			{
				WriteTaxBandTotal(Block, CurrentTaxBand.m_nTaxBand);
			}

			nCurrentTaxBandStart = s;
			nCurrentItemStart = s;

			Block.m_infoTaxBandTotals.Clear();

			CurrentTaxBand = Sales;
			CurrentPluNo = Sales;

			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo(Sales.m_nTaxBand, TaxRateInfo);

			CString strHeader = "";

			CString strType = "";
			if ( -1 == Sales.m_nTaxBand )
			{
				strType = EcrmanOptions.GetTaxString("Zero %T");
			}
			else
			{
				strType = TaxRateInfo.m_strReportText;
			}

			strHeader.Format("<..>%s", (const char*)strType);

			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine(strHeader);
			m_ReportFile.WriteReportMiscLine("<LI>");
		}

		if (CurrentPluNo.ComparePluNoTax(Sales) == FALSE)
		{
			if (nCurrentItemStart != -1)
			{
				CalculatePluItemTotals(Block, nCurrentItemStart, s - 1);
				WritePluItemTotals(Block);
			}

			nCurrentItemStart = s;
			CurrentPluNo = Sales;
		}
	}

	if (nCurrentItemStart != -1)
	{
		CalculatePluItemTotals(Block, nCurrentItemStart, Block.m_pSales->GetSize() - 1);
		WritePluItemTotals(Block);
	}

	if (nCurrentTaxBandStart != -1)
	{
		WriteTaxBandTotal(Block, CurrentTaxBand.m_nTaxBand);
	}
}

/**********************************************************************/

void CEposReportPluSalesTax::CalculatePluItemTotals(CEposReportPluSalesTaxBlock& Block, int nStart, int nEnd)
{
	CEposReportConsolPluSalesTax ConsolInfoStart;
	Block.m_pSales->GetAt(nStart, ConsolInfoStart);
	Block.m_nCurrentPluNo = ConsolInfoStart.m_nPluNo;
	Block.m_infoCurrentPluTotals.Clear();

	int nTaxBand = ConsolInfoStart.m_nTaxBand;

	double dItemCost = 0.0;
	bool bReportCost = DealerFlags.GetReportCostPriceFlag();

	if (FALSE == bReportCost)
	{
		m_PluInfoFinder.SetPluNo(Block.m_nCurrentPluNo);
		dItemCost = m_PluInfoFinder.GetCost();
	}

	for (int i = nStart; i <= nEnd; i++)
	{
		CEposReportConsolPluSalesTax ConsolInfoLine;
		Block.m_pSales->GetAt(i, ConsolInfoLine);

		if (TRUE == bReportCost)
		{
			dItemCost = ConsolInfoLine.m_dItemCost;
		}

		double dNet = ConsolInfoLine.m_dValBeforeDiscount - ConsolInfoLine.m_dDiscountVal + ConsolInfoLine.m_dPremiumVal;
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate(ConsolInfoLine.m_nTaxDateCode, nTaxBand);
		double dTax = dNet - ReportHelpers.CalcNonTax(dNet, dTaxRate);
		double dCost = dItemCost * ConsolInfoLine.m_dQty;

		Block.m_infoCurrentPluTotals.m_dQty += ConsolInfoLine.m_dQty;
		Block.m_infoCurrentPluTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		Block.m_infoCurrentPluTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		Block.m_infoCurrentPluTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		Block.m_infoCurrentPluTotals.m_dTax += dTax;
		Block.m_infoCurrentPluTotals.m_dCost += dCost;
		Block.m_infoCurrentPluTotals.m_bGotData = TRUE;

		Block.m_infoTaxBandTotals.m_dQty += ConsolInfoLine.m_dQty;
		Block.m_infoTaxBandTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		Block.m_infoTaxBandTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		Block.m_infoTaxBandTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		Block.m_infoTaxBandTotals.m_dTax += dTax;
		Block.m_infoTaxBandTotals.m_dCost += dCost;
		Block.m_infoTaxBandTotals.m_bGotData = TRUE;

		Block.m_infoBlockTotals.m_dQty += ConsolInfoLine.m_dQty;
		Block.m_infoBlockTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		Block.m_infoBlockTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		Block.m_infoBlockTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		Block.m_infoBlockTotals.m_dTax += dTax;
		Block.m_infoBlockTotals.m_dCost += dCost;
		Block.m_infoBlockTotals.m_bGotData = TRUE;

		if (TRUE == Block.m_bAddToTotal)
		{
			m_infoGrandTotals.m_dQty += ConsolInfoLine.m_dQty;
			m_infoGrandTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
			m_infoGrandTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
			m_infoGrandTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
			m_infoGrandTotals.m_dTax += dTax;
			m_infoGrandTotals.m_dCost += dCost;
			m_infoGrandTotals.m_bGotData = TRUE;
		}
	}
}

/**********************************************************************/

void CEposReportPluSalesTax::WritePluItemTotals( CEposReportPluSalesTaxBlock& Block )
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
	csv.Add ( dTax, SysInfo.GetDPValue() );

	if (TRUE == m_bNetOfTax)
	{
		csv.Add(dNet - dTax, SysInfo.GetDPValue());
	}
	
	csv.Add ( dCost, SysInfo.GetDPValue() );
	
	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportPluSalesTax::WriteTaxBandTotal ( CEposReportPluSalesTaxBlock& Block, int nTaxBand )
{
	CTaxRateInfo TaxRateInfo;
	DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );
	
	CString strType = "";
	if ( -1 == nTaxBand )
	{
		strType = EcrmanOptions.GetTaxString("Zero %T");
	}
	else
	{
		strType = TaxRateInfo.m_strReportText;
	}

	m_arrayTaxBandTotals.Add(WriteConsolidatedTotals ( Block.m_infoTaxBandTotals, strType, TRUE, FALSE ));	
}

/**********************************************************************/

void CEposReportPluSalesTax::WriteBlockTotals( CEposReportPluSalesTaxBlock& Block )
{
	CString strType = GetTotalTypeName( FALSE, Block.m_nEntityType );
	m_ReportFile.RequestBlankLines(1);
	WriteConsolidatedTotals( Block.m_infoBlockTotals, strType, FALSE, FALSE );
}

/**********************************************************************/

CString CEposReportPluSalesTax::WriteConsolidatedTotals(CEposReportPluSalesInfo& Totals, const char* szType, bool bTaxBandTotal, bool bGrandTotal )
{
	if (FALSE == bGrandTotal)
	{
		CString strLine = "\t\t<LI>\t<LI>\t<LI>\t<LI>\t<LI>";
		if (TRUE == m_bDiscountDetail) strLine += "\t<LI>\t<LI>";
		if (TRUE == m_bPremiumDetail) strLine += "\t<LI>";
		if (TRUE == m_bNetOfTax) strLine += "\t<LI>";
		m_ReportFile.WriteReportMiscLine(strLine);
	}

	CString strType = szType;

	if (FALSE == bTaxBandTotal)
	{
		if (strType != "") strType += " ";
		strType += "Total";
	}

	double dNet = Totals.m_dValBeforeDiscount - Totals.m_dDiscountAmount + Totals.m_dPremiumAmount;

	CCSV csv('\t');
	csv.Add("");
	csv.Add(strType);
	csv.Add(Totals.m_dQty, SysInfo.GetDPQty());

	if (TRUE == m_bDiscountDetail)
	{
		csv.Add(Totals.m_dValBeforeDiscount, SysInfo.GetDPValue());
		csv.Add(Totals.m_dDiscountAmount, SysInfo.GetDPValue());

		if (TRUE == m_bPremiumDetail)
		{
			csv.Add(Totals.m_dPremiumAmount, SysInfo.GetDPValue());
		}
	}

	csv.Add(dNet, SysInfo.GetDPValue());
	csv.Add(ReportHelpers.CalcAverage(Totals.m_dQty, dNet), SysInfo.GetDPValue());
	csv.Add(Totals.m_dTax, SysInfo.GetDPValue());

	if (TRUE == m_bNetOfTax)
	{
		csv.Add(dNet - Totals.m_dTax, SysInfo.GetDPValue());
	}

	csv.Add(Totals.m_dCost, SysInfo.GetDPValue());

	CString strLine = csv.GetLine();
	m_ReportFile.WriteReportMiscLine(strLine);

	return strLine;
}

/**********************************************************************/
