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
#include "EposReportTaxSummaryTax.h"
/**********************************************************************/

CEposReportTaxSummaryTax::CEposReportTaxSummaryTax( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_TAX_SUMMARY_TAX;
	m_bDiscountDetail = EcrmanOptions.GetReportsDiscountDetailSalesFlag();
	m_bPremiumDetail = EcrmanOptions.AllowPremiumDetailSales();
	m_bNetOfTax = EcrmanOptions.GetReportsNetOfTaxSalesFlag();
}

/**********************************************************************/

CEposReportTaxSummaryTax::~CEposReportTaxSummaryTax()
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

void CEposReportTaxSummaryTax::InitialiseConsolidationBlocks()
{
	m_bAllowGlobal = ( ( NODE_SYSTEM == m_nConLevel ) && ( EcrmanOptions.GetGlobalTaxFlag() == TRUE ) ); 

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

		CEposReportTaxSummaryTaxBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolTaxSummaryTax>;
		block.m_pItem = new CReportConsolidationArray<CEposReportConsolTaxSummaryTaxItem>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportTaxSummaryTax::Consolidate()
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

				CEposReportConsolTaxSummaryTax ConsolInfo;

				if ( ( CSVPLUDATA_PROMO_NONTAXABLE == nLineType ) || ( CSVPLUDATA_MIXMATCH_NONTAXABLE == nLineType ) )
				{
					nPluNo = 0;
					ConsolInfo.m_nTaxBand = REPORT_TAXBAND_NONTAX_DISCOUNT;
				}
				else if (DealerFlags.GetUseDatabaseVATBandFlag() == FALSE)
				{
					nPluNo = 0;
					ConsolInfo.m_nTaxBand = ReportHelpers.GetNumericTaxBandFromString( csv.GetTaxBand() );
				}
				else
				{
					ConsolInfo.m_nTaxBand = 0;
				}

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
					CEposReportConsolTaxSummaryTaxItem ItemInfo;
					ItemInfo.m_nPluNo = nPluNo;
					ItemInfo.m_nModifier = nModifier;
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

bool CEposReportTaxSummaryTax::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( EcrmanOptions.GetTaxBandString(), TA_LEFT, 400 );
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
		CEposReportTaxSummaryTaxBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pItem -> GetSize();
		m_nCreateReportTarget += MAX_TAX_BANDS + 1;
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportTaxSummaryTaxBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );

		m_ReportFile.HandleNewPage();

		if ( ( NODE_SYSTEM == ReportBlock.m_nEntityType ) && ( FALSE == m_bAllowGlobal ) )
		{	
			m_ReportFile.WriteReportMiscLine( "<..>Grand Total" );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			WriteConsolidatedTotals( m_infoGrandTotals, "Grand", TRUE );
		}	
		else
		{
			CString strHeader;
			ReportBlock.GetHeaderText( strHeader );
			m_ReportFile.WriteReportMiscLine( strHeader );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			CreateSalesSection( ReportBlock );
			WriteBlockTotals( ReportBlock );
		}
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportTaxSummaryTax::CreateSalesSection( CEposReportTaxSummaryTaxBlock& Block )
{
	for ( int s = Block.m_pItem -> GetSize() - 1; s >= 0; s-- )
	{
		CEposReportConsolTaxSummaryTaxItem Item;
		Block.m_pItem -> GetAt( s, Item );
		UpdateCreateReportProgress();

		m_PluInfoFinder.SetPluNo( Item.m_nPluNo );

		CEposReportConsolTaxSummaryTax ConsolInfo;
		
		if ( m_PluInfoFinder.PluExists() == TRUE )
		{
			ConsolInfo.m_nTaxBand = m_PluInfoFinder.GetModifierTaxBand( Item.m_nModifier );
		}
		else
		{
			ConsolInfo.m_nTaxBand = REPORT_TAXBAND_UNKNOWN_PLU;
		}
		
		ConsolInfo.m_nTaxDateCode = Item.m_nTaxDateCode;
		ConsolInfo.m_dQty = Item.m_dQty;
		ConsolInfo.m_dValBeforeDiscount = Item.m_dValBeforeDiscount;
		ConsolInfo.m_dDiscountVal = Item.m_dDiscountVal;
		ConsolInfo.m_dPremiumVal = Item.m_dPremiumVal;
		Block.m_pSales -> Consolidate( ConsolInfo );

		Block.m_pItem -> RemoveAt( s );
	}

	int nCurrentBandStart = -1;
	CEposReportConsolTaxSummaryTax CurrentBand;
	CurrentBand.m_nTaxBand = 999;
	
	int nProgressTicks = 0;
	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolTaxSummaryTax Sales;
		Block.m_pSales -> GetAt( s, Sales );
		
		if ( CurrentBand.m_nTaxBand != Sales.m_nTaxBand )
		{
			if ( nCurrentBandStart != -1 )
			{
				CalculateTaxBandTotals( Block, nCurrentBandStart, s - 1 );
				WriteTaxBandTotals( Block, CurrentBand.m_nTaxBand );
				UpdateCreateReportProgress();
				nProgressTicks++;
			}
	
			nCurrentBandStart = s;
			CurrentBand = Sales;
		}
	}

	if ( nCurrentBandStart != -1 )
	{
		CalculateTaxBandTotals( Block, nCurrentBandStart, Block.m_pSales -> GetSize() - 1 );
		WriteTaxBandTotals( Block, CurrentBand.m_nTaxBand );
	}

	while( nProgressTicks < MAX_TAX_BANDS + 1 )
	{
		UpdateCreateReportProgress();
		nProgressTicks++;
	}
}

/**********************************************************************/

void CEposReportTaxSummaryTax::CalculateTaxBandTotals( CEposReportTaxSummaryTaxBlock& Block, int nStart, int nEnd )
{
	CEposReportConsolTaxSummaryTax ConsolInfoStart;
	Block.m_pSales -> GetAt( nStart, ConsolInfoStart );
	Block.m_infoCurrentTaxBandTotals.Clear();

	int nTaxBand = ConsolInfoStart.m_nTaxBand;

	for ( int i = nStart; i <= nEnd; i++ )
	{
		CEposReportConsolTaxSummaryTax ConsolInfoLine;
		Block.m_pSales -> GetAt( i, ConsolInfoLine );
		
		double dNet = ConsolInfoLine.m_dValBeforeDiscount - ConsolInfoLine.m_dDiscountVal + ConsolInfoLine.m_dPremiumVal;
		double dTaxRate = DataManager.HistoricalTaxRates.GetTaxRate( ConsolInfoLine.m_nTaxDateCode, nTaxBand );
		double dTax = dNet - ReportHelpers.CalcNonTax ( dNet, dTaxRate );
	
		Block.m_infoCurrentTaxBandTotals.m_dQty += ConsolInfoLine.m_dQty;
		Block.m_infoCurrentTaxBandTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		Block.m_infoCurrentTaxBandTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		Block.m_infoCurrentTaxBandTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		Block.m_infoCurrentTaxBandTotals.m_dTax += dTax;
		Block.m_infoCurrentTaxBandTotals.m_bGotData = TRUE;

		Block.m_infoBlockTotals.m_dQty += ConsolInfoLine.m_dQty;
		Block.m_infoBlockTotals.m_dValBeforeDiscount += ConsolInfoLine.m_dValBeforeDiscount;
		Block.m_infoBlockTotals.m_dDiscountAmount += ConsolInfoLine.m_dDiscountVal;
		Block.m_infoBlockTotals.m_dPremiumAmount += ConsolInfoLine.m_dPremiumVal;
		Block.m_infoBlockTotals.m_dTax += dTax;
		Block.m_infoBlockTotals.m_bGotData = TRUE;

		if ( ( TRUE == Block.m_bAddToTotal ) && ( FALSE == m_bAllowGlobal ) )
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

void CEposReportTaxSummaryTax::WriteTaxBandTotals( CEposReportTaxSummaryTaxBlock& Block, int nTaxBand )
{
	double dQty =		Block.m_infoCurrentTaxBandTotals.m_dQty;
	double dVal =		Block.m_infoCurrentTaxBandTotals.m_dValBeforeDiscount;
	double dDiscount =	Block.m_infoCurrentTaxBandTotals.m_dDiscountAmount;
	double dPremium	=	Block.m_infoCurrentTaxBandTotals.m_dPremiumAmount;
	double dTax =		Block.m_infoCurrentTaxBandTotals.m_dTax;
	double dNet =		dVal - dDiscount + dPremium;
	
	CCSV csv ( '\t' );	
	csv.Add( ReportHelpers.GetTaxBandString( nTaxBand ) );

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

void CEposReportTaxSummaryTax::WriteBlockTotals( CEposReportTaxSummaryTaxBlock& Block )
{
	CString strType = GetTotalTypeName( FALSE, Block.m_nEntityType );
	WriteConsolidatedTotals( Block.m_infoBlockTotals, strType, FALSE );
}

/**********************************************************************/

void CEposReportTaxSummaryTax::WriteConsolidatedTotals ( CEposReportPluSalesInfo& Totals, const char* szType, bool bGrandTotal )
{
	if ( FALSE == bGrandTotal )
	{
		CString strLine = "\t<LI>\t<LI>\t<LI>\t<LI>";
		if ( TRUE == m_bDiscountDetail ) strLine += "\t<LI>\t<LI>";
		if ( TRUE == m_bPremiumDetail) strLine += "\t<LI>";
		if ( TRUE == m_bNetOfTax ) strLine += "\t<LI>";
		m_ReportFile.WriteReportMiscLine( strLine );
	}

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
