/**********************************************************************/
 
/**********************************************************************/
#include "CashRSPImporterDiscountHelpers.h"
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportPromotionSummary.h"
/**********************************************************************/

CEposReportPromotionSummary::CEposReportPromotionSummary( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_PROMOTION_SUMMARY;
}

/**********************************************************************/

CEposReportPromotionSummary::~CEposReportPromotionSummary()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pPromos -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pPromos );
	}
}

/**********************************************************************/

void CEposReportPromotionSummary::InitialiseConsolidationBlocks()
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

		CEposReportPromotionSummaryBlock block;
		block.Copy( entity );
		block.m_pPromos = new CReportConsolidationArray<CEposReportConsolPromotionSummary>;
		
		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportPromotionSummary::Consolidate()
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

				//FILTER FOR WASTAGE
				if (csv.GetPluSaleWastageFlag() == TRUE)
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

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				CEposReportConsolPromotionSummary infoPromo;
				infoPromo.m_nSection = 0;
									
				switch( csv.GetLineType() )
				{
				case CSVPLUDATA_PROMO_TAXABLE:
				case CSVPLUDATA_PROMO_NONTAXABLE:
					infoPromo.m_nSection = 1;
					infoPromo.m_nOfferNo = csv.GetPromoNoSeparate();
					break;

				case CSVPLUDATA_MIXMATCH_NONTAXABLE:
				case CSVPLUDATA_PLU_MIXMATCH:
					infoPromo.m_nSection = 2;
					infoPromo.m_nMixMatchType = csv.GetMixMatchType();
					infoPromo.m_nOfferNo = csv.GetMixMatchNo();
					break;

				case CSVPLUDATA_PLU_DISCOUNT:
					infoPromo.m_nSection = 1;
					infoPromo.m_nOfferNo = csv.GetPromoNoForPlu();
					break;
				}

				if ( infoPromo.m_nSection != 0 )
				{
					if ( 1 == infoPromo.m_nSection )
					{
						switch( csv.GetDiscountMode() )
						{
						case DISCOUNT_MODE_SUBTOTAL:	infoPromo.m_nDiscType = 0;	break;
						case DISCOUNT_MODE_LOYALTY:		infoPromo.m_nDiscType = 1;	break;
						case DISCOUNT_MODE_ITEM:		infoPromo.m_nDiscType = 2;	break;
						default:						infoPromo.m_nDiscType = 3;	break;
						}

						CString strDiscAction = csv.GetDiscountType();
						if ( strDiscAction == "M" )			infoPromo.m_nDiscAction = 0;
						else if ( strDiscAction == "D" )	infoPromo.m_nDiscAction = 1;
						else if ( strDiscAction == "P" )	infoPromo.m_nDiscAction = 2;
						else infoPromo.m_nSection = 0;
					}
				}

				if ( infoPromo.m_nSection != 0 )
				{
					infoPromo.m_dAmount = csv.GetValue();
					
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						
						infoPromo.m_bOfferTotal = FALSE;
						m_arrayReportBlocks[ nBlock ].m_pPromos -> Consolidate( infoPromo );

						infoPromo.m_bOfferTotal = TRUE;
						m_arrayReportBlocks[ nBlock ].m_pPromos -> Consolidate( infoPromo );
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

bool CEposReportPromotionSummary::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Type", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Action", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Amount", TA_RIGHT, 250 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportPromotionSummaryBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pPromos -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportPromotionSummaryBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

		if ( ReportBlock.GotData() == FALSE )
			continue;

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( ReportBlock.m_nDbIdx, info, FALSE );
		
		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText( strHeader );
		m_ReportFile.WriteReportMiscLine( strHeader );
		m_ReportFile.WriteReportMiscLine( "<LI>" );
		m_ReportFile.LockBlankLines(1);

		CreateSalesSection( ReportBlock );
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportPromotionSummary::CreateSalesSection( CEposReportPromotionSummaryBlock& Block )
{
	CReportConsolidationArray<CEposReportConsolPromotionSummaryTotal> arrayTotals;

	bool bWantHeader = TRUE;
	for ( int s = 0; s < Block.m_pPromos -> GetSize(); s++ )
	{
		CEposReportConsolPromotionSummary infoConsol;
		Block.m_pPromos -> GetAt( s, infoConsol );
		UpdateCreateReportProgress();

		if ( ( infoConsol.m_nSection != 1 ) && ( infoConsol.m_nSection != 2 ) )
			continue;

		if ( TRUE == bWantHeader )
		{
			CString strHeader = "";

			if ( 1 == infoConsol.m_nSection )
			{
				int nPromoIdx;
				CString strPromoName = "";
				if ( DataManager.Promotion.FindTableByNumber( infoConsol.m_nOfferNo, nPromoIdx ) == TRUE )
				{
					CPromotionsCSVRecord Promo;
					DataManager.Promotion.GetAt( nPromoIdx, Promo );
					strPromoName = Promo.GetDisplayName();
				}

				if ( strPromoName == "" )
					strPromoName.Format( "Promo %d", infoConsol.m_nOfferNo );
				
				strHeader.Format( "<..>P%2.2d: %s", infoConsol.m_nOfferNo, (const char*) strPromoName );
			}
			else
			{	
				::GetMixMatchOfferTypeName( infoConsol.m_nMixMatchType, strHeader, TRUE, "Unknown Mix Match Type" );
			}
			
			m_ReportFile.RequestBlankLines( 2 );
			m_ReportFile.WriteReportMiscLine( strHeader );
			m_ReportFile.WriteReportMiscLine( "<LI>" );
			bWantHeader = FALSE;
		}

		CCSV csvOut( '\t' );

		if ( TRUE == infoConsol.m_bOfferTotal )
		{
			csvOut.Add( "" );
			csvOut.Add( "Total" );
			m_ReportFile.WriteReportMiscLine( "\t\t<LI>" );
			bWantHeader = TRUE;
		}
		else 
		{		
			if ( 1 == infoConsol.m_nSection )
			{
				AddDiscountType( infoConsol.m_nDiscType, infoConsol.m_nDiscAction, csvOut );
			}
			else
			{
				int nMixMatchIdx;
				CString strMixMatchName = "<..>";
				if ( DataManager.MixMatch.FindTableByNumber( infoConsol.m_nOfferNo, nMixMatchIdx ) == TRUE )
				{
					CMixMatchCSVRecord MixMatch;
					DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatch );
					strMixMatchName += MixMatch.GetDisplayName();
				}

				if (strMixMatchName == "<..>")
				{
					strMixMatchName.Format("<..>Mix Match %d", infoConsol.m_nOfferNo);
				}

				csvOut.Add( strMixMatchName );
				csvOut.Add( "" );
			}

			CEposReportConsolPromotionSummaryTotal infoTotal;
			infoTotal.Copy( infoConsol );
			arrayTotals.Consolidate( infoTotal );
		}

		csvOut.Add( infoConsol.m_dAmount, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
	}

	if ( arrayTotals.GetSize() > 0 )
	{
		m_ReportFile.RequestBlankLines( 2 );
		m_ReportFile.WriteReportMiscLine( "<..>Totals" );
		m_ReportFile.WriteReportMiscLine( "<LI>" );

		double dBlockTotal = 0.0;

		for ( int s = 0; s < arrayTotals.GetSize(); s++ )
		{
			CEposReportConsolPromotionSummaryTotal infoConsol;
			arrayTotals.GetAt( s, infoConsol );
			dBlockTotal += infoConsol.m_dAmount;

			CCSV csvOut( '\t' );

			if (1 == infoConsol.m_nSection)
			{
				AddDiscountType(infoConsol.m_nDiscType, infoConsol.m_nDiscAction, csvOut);
			}
			else
			{
				CString strType;
				::GetMixMatchOfferTypeName( infoConsol.m_nMixMatchType, strType, FALSE, "Unknown Type" );

				csvOut.Add( "Mix Match" );
				csvOut.Add( strType );		
			}
			
			csvOut.Add( infoConsol.m_dAmount, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		}

		CCSV csvOut( '\t' );
		csvOut.Add( "" );
		csvOut.Add( "" );
		csvOut.Add( dBlockTotal, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportMiscLine( "\t\t<LI>" );
		m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
	}
}	
	
/**********************************************************************/

void CEposReportPromotionSummary::AddDiscountType( int nDiscType, int nDiscAction, CCSV& csvOut )
{
	switch( nDiscType )
	{
	case 0:		csvOut.Add( "Subtotal" );	break;
	case 1:		csvOut.Add( "Loyalty" );	break;
	case 2:		csvOut.Add( "Item" );		break;
	default:	csvOut.Add( "???" );		break;
	}

	switch( nDiscAction )
	{
	case 0:		csvOut.Add( "Markdown" );	break;
	case 1:		csvOut.Add( "Discount" );	break;
	case 2:		csvOut.Add( "Premium" );	break;
	default:	csvOut.Add( "???" );		break;
	}
}

/**********************************************************************/
