/**********************************************************************/
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "PMSReportFileFinder.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportTaxSummaryPrice.h"
/**********************************************************************/

CEposReportTaxSummaryPrice::CEposReportTaxSummaryPrice( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_TAX_SUMMARY_PRICE;
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

CEposReportTaxSummaryPrice::~CEposReportTaxSummaryPrice()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		m_arrayReportBlocks[n].m_pItem -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
		delete ( m_arrayReportBlocks[n].m_pItem );
	}
}

/**********************************************************************/

void CEposReportTaxSummaryPrice::InitialiseConsolidationBlocks()
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

	CEposReportConsolParamsStandard Params;
	Params.LoadSettings( strParams );

	if ( ( Params.m_bConsolSys | Params.m_bConsolDb | Params.m_bConsolLoc | Params.m_bConsolTerm ) == FALSE )
	{
		strParams = m_EposReportSelectInfo.GetConsolPrefsDefault();
		Params.LoadSettings( strParams );
	}
	
	if ( ( NODE_SYSTEM != m_nConLevel ) && ( TRUE == Params.m_bConsolSys ) )
	{
		Params.m_bConsolSys = FALSE;
		Params.m_bConsolDb = TRUE;
	}
		
	m_SelectArray.MakeList();
	m_BlockMap.SetConsolidateByTerminalFlag( Params.m_bConsolTerm );
	m_BlockMap.SetConsolidateByLocationFlag( Params.m_bConsolLoc );
	m_BlockMap.SetConsolidateByDatabaseFlag( Params.m_bConsolDb );
	m_BlockMap.SetConsolidateBySystemFlag( Params.m_bConsolSys );
	m_BlockMap.BuildMap( m_SelectArray );
	
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportTaxSummaryPriceBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolTaxSummaryPrice>;
		block.m_pItem = new CReportConsolidationArray<CEposReportConsolTaxSummaryPriceItem>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportTaxSummaryPrice::Consolidate()
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

				CEposReportConsolTaxSummaryPrice ConsolInfo;

				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
				{
					nPluNo = 0;
					ConsolInfo.m_nTaxBand = 0;
				}
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
				{
					nPluNo = 0;
					ConsolInfo.m_nTaxBand = csv.GetNumericTaxBand();
				}
				else
				{
					ConsolInfo.m_nTaxBand = 0;
				}

				ConsolInfo.m_nPriceLevel = nPriceBand;
				ConsolInfo.m_nTaxDateCode = nTaxDateCode;
				
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

				if ( (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE) || ( 0 == nPluNo ) )
				{
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
					}
				}
				else
				{
					CEposReportConsolTaxSummaryPriceItem ItemInfo;
					ItemInfo.m_nPluNo = nPluNo;
					ItemInfo.m_nModifier = nModifier;
					ItemInfo.m_nPriceLevel = ConsolInfo.m_nPriceLevel;
					ItemInfo.m_nTaxDateCode = ConsolInfo.m_nTaxDateCode;
					ItemInfo.m_dQty = ConsolInfo.m_dQty;
					ItemInfo.m_dValBeforeDiscount = ConsolInfo.m_dValBeforeDiscount;
					ItemInfo.m_dDiscountVal = ConsolInfo.m_dDiscountVal;
					ItemInfo.m_dPremiumVal = ConsolInfo.m_dPremiumVal;

					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						m_arrayReportBlocks[ nBlock ].m_pItem -> Consolidate( ItemInfo );
					}
				}
			}
			while ( ( fileSales.ReadString( strBuffer ) == TRUE ) && ( --nLinesToRead != 0 ) );
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportTaxSummaryPrice::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Price Level", TA_LEFT, 400 );
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
	m_ReportFile.AddColumn ( EcrmanOptions.GetEstTaxString(), TA_RIGHT, 250 );

	if ( TRUE == m_bNetOfTax )
		m_ReportFile.AddColumn ( "Est. Net", TA_RIGHT, 250 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportTaxSummaryPriceBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pItem -> GetSize();
		m_nCreateReportTarget += MAX_PLU_PRICES;
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportTaxSummaryPriceBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );

		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		CreateSalesSection( ReportBlock );
		WriteBlockTotals( ReportBlock );
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportTaxSummaryPrice::CreateSalesSection( CEposReportTaxSummaryPriceBlock& Block )
{
	for ( int s = Block.m_pItem -> GetSize() - 1; s >= 0; s-- )
	{
		CEposReportConsolTaxSummaryPriceItem Item;
		Block.m_pItem -> GetAt( s, Item );
		UpdateCreateReportProgress();

		m_PluInfoFinder.SetPluNo( Item.m_nPluNo );
		
		CEposReportConsolTaxSummaryPrice ConsolInfo;
		ConsolInfo.m_nTaxBand = m_PluInfoFinder.GetModifierTaxBand( Item.m_nModifier );
		ConsolInfo.m_nPriceLevel = Item.m_nPriceLevel;
		ConsolInfo.m_nTaxDateCode = Item.m_nTaxDateCode;
		ConsolInfo.m_dQty = Item.m_dQty;
		ConsolInfo.m_dValBeforeDiscount = Item.m_dValBeforeDiscount;
		ConsolInfo.m_dDiscountVal= Item.m_dDiscountVal;
		ConsolInfo.m_dPremiumVal = Item.m_dPremiumVal;
		Block.m_pSales -> Consolidate( ConsolInfo );

		Block.m_pItem -> RemoveAt( s );
	}

	int nCurrentBandStart = -1;
	CEposReportConsolTaxSummaryPrice CurrentBand;
	CurrentBand.m_nPriceLevel = -1;
	
	int nProgressTicks = 0;
	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolTaxSummaryPrice Sales;
		Block.m_pSales -> GetAt( s, Sales );
		
		if ( CurrentBand.m_nPriceLevel != Sales.m_nPriceLevel )
		{
			if ( nCurrentBandStart != -1 )
			{
				CalculatePriceLevelTotals( Block, nCurrentBandStart, s - 1 );
				WritePriceLevelTotals( Block, CurrentBand.m_nPriceLevel );
				UpdateCreateReportProgress();
				nProgressTicks++;
			}
	
			nCurrentBandStart = s;
			CurrentBand = Sales;
		}
	}

	if ( nCurrentBandStart != -1 )
	{
		CalculatePriceLevelTotals( Block, nCurrentBandStart, Block.m_pSales -> GetSize() - 1 );
		WritePriceLevelTotals( Block, CurrentBand.m_nPriceLevel );
	}

	while( nProgressTicks < MAX_PLU_PRICES )
	{
		UpdateCreateReportProgress();
		nProgressTicks++;
	}
}

/**********************************************************************/

void CEposReportTaxSummaryPrice::CalculatePriceLevelTotals( CEposReportTaxSummaryPriceBlock& Block, int nStart, int nEnd )
{
	CEposReportConsolTaxSummaryPrice ConsolInfoStart;
	Block.m_pSales -> GetAt( nStart, ConsolInfoStart );
	Block.m_infoCurrentPriceLevelTotals.Clear();

	for ( int i = nStart; i <= nEnd; i++ )
	{
		CEposReportConsolTaxSummaryPrice ConsolInfoLine;
		Block.m_pSales -> GetAt( i, ConsolInfoLine );
		
		double dNet = ConsolInfoLine.m_dValBeforeDiscount - ConsolInfoLine.m_dDiscountVal + ConsolInfoLine.m_dPremiumVal;
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( ConsolInfoLine.m_nTaxDateCode, ConsolInfoLine.m_nTaxBand );
		double dTax = dNet - ReportHelpers.CalcNonTax ( dNet, dTaxRate );
		
		Block.m_infoCurrentPriceLevelTotals.m_dQty += ConsolInfoLine.m_dQty;
		Block.m_infoCurrentPriceLevelTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		Block.m_infoCurrentPriceLevelTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		Block.m_infoCurrentPriceLevelTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		Block.m_infoCurrentPriceLevelTotals.m_dTax += dTax;
		Block.m_infoCurrentPriceLevelTotals.m_bGotData = TRUE;

		Block.m_infoBlockTotals.m_dQty += ConsolInfoLine.m_dQty;
		Block.m_infoBlockTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		Block.m_infoBlockTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		Block.m_infoBlockTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		Block.m_infoBlockTotals.m_dTax += dTax;
		Block.m_infoBlockTotals.m_bGotData = TRUE;

		if ( TRUE == Block.m_bAddToTotal )
		{
			m_infoGrandTotals.m_dQty += ConsolInfoLine.m_dQty;
			m_infoGrandTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
			m_infoGrandTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
			m_infoGrandTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
			m_infoGrandTotals.m_dTax += dTax;
			m_infoGrandTotals.m_bGotData = TRUE;
		}
	}
}

/**********************************************************************/

void CEposReportTaxSummaryPrice::WritePriceLevelTotals( CEposReportTaxSummaryPriceBlock& Block, int nPriceLevel )
{
	double dQty =		Block.m_infoCurrentPriceLevelTotals.m_dQty;
	double dVal =		Block.m_infoCurrentPriceLevelTotals.m_dValBeforeDiscount;
	double dDiscount =	Block.m_infoCurrentPriceLevelTotals.m_dDiscountAmount;
	double dPremium =	Block.m_infoCurrentPriceLevelTotals.m_dPremiumAmount;
	double dTax =		Block.m_infoCurrentPriceLevelTotals.m_dTax;
	double dNet =		dVal - dDiscount + dPremium;
	
	CCSV csv ( '\t' );	
	csv.Add( DataManager.PriceText.GetReportText( nPriceLevel ) );

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

	m_ReportFile.WriteReportDataLine( csv.GetLine() );
}

/**********************************************************************/

void CEposReportTaxSummaryPrice::WriteBlockTotals( CEposReportTaxSummaryPriceBlock& Block )
{
	CString strType = GetTotalTypeName( FALSE, Block.m_nEntityType );
	WriteConsolidatedTotals( Block.m_infoBlockTotals, strType );
}

/**********************************************************************/

void CEposReportTaxSummaryPrice::WriteConsolidatedTotals ( CEposReportPluSalesInfo& Totals, const char* szType )
{
	CString strLine = "\t<LI>\t<LI>\t<LI>\t<LI>";
	if ( TRUE == m_bDiscountDetail ) strLine += "\t<LI>\t<LI>";
	if ( TRUE == m_bPremiumDetail ) strLine += "\t<LI>";
	if ( TRUE == m_bNetOfTax ) strLine += "\t<LI>";
	m_ReportFile.WriteReportMiscLine( strLine );
	
	CString strType = szType;
	if ( strType != "" ) strType += " ";
	strType += "Total";
	
	double dNet = Totals.m_dValBeforeDiscount - Totals.m_dDiscountAmount + Totals.m_dPremiumAmount;

	CCSV csv ( '\t' );
	csv.Add ( strType );
	csv.Add ( Totals.m_dQty, SysInfo.GetDPQty() );

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
	csv.Add ( Totals.m_dTax, SysInfo.GetDPValue() );

	if (TRUE == m_bNetOfTax)
	{
		csv.Add(dNet - Totals.m_dTax, SysInfo.GetDPValue());
	}

	m_ReportFile.WriteReportMiscLine ( csv.GetLine() );
}

/**********************************************************************/
