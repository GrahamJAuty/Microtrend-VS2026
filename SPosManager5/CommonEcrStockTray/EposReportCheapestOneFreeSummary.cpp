/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "EposReportConsolParams.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCheapestOneFreeSummary.h"
/**********************************************************************/

CEposReportCheapestOneFreeSummary::CEposReportCheapestOneFreeSummary( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_CHEAPESTONEFREE_SUMMARY;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

CEposReportCheapestOneFreeSummary::~CEposReportCheapestOneFreeSummary()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pPromos -> RemoveAll();
		m_arrayReportBlocks[n].m_pTotals -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pPromos );
		delete ( m_arrayReportBlocks[n].m_pTotals );
	}
}

/**********************************************************************/

void CEposReportCheapestOneFreeSummary::InitialiseConsolidationBlocks()
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

		CEposReportCheapestOneFreeSummaryBlock block;
		block.Copy( entity );
		block.m_pPromos = new CReportConsolidationArray<CEposReportConsolCheapestOneFreeSummary>;
		block.m_pTotals = new CReportConsolidationArray<CEposReportConsolCheapestOneFreeSummary>;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportCheapestOneFreeSummary::Consolidate()
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

		if ( m_PMSModes.GetPMSLocationStatus( infoFile ) != 2 )
			continue;

		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		CString strThisSaleDate = "";
		CString strThisSaleTime = "";
		m_PMSModes.GetInitialSaleDateTime( FALSE, infoFile, m_bSaleTime, strThisSaleDate, strThisSaleTime );
		
		m_ReportFilters.SetDatabase( infoFile.m_nDbIdx );

		CPluSalesLine csv( infoFile.IsLegacySalesFolder() );
		
		int nLinesToRead;
		CString strBuffer;
		if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
		{
			do
			{
				StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );
					
				csv.ParseLine( strBuffer );

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
					if ( m_PMSModes.GetEposFlag() == FALSE )
						if ( DataManager.Payment.IsPMSPaymentType( csv.GetPluPaymentNo() ) )
							continue;
			
				//NORMAL CASHRSP SALES TAKES DATE FROM FILE AND TIME FROM CSV LINE
				{
					COleDateTime oleDummy;
					if ( CheckPluSaleDateTime( FALSE, infoFile, csv, oleDummy, strThisSaleDate, strThisSaleTime ) == FALSE )
						continue;
				}

				//FILTER FOR DEPARTMENT
				int nDeptNo = csv.GetDeptNo();
				if ( m_ReportFilters.CheckDepartment( nDeptNo ) == FALSE )
					continue;

				switch( csv.GetLineType() )
				{
				case CSVPLUDATA_PLU_SALE:
				case CSVPLUDATA_PLU_CHEAPESTFREE:

					if ( csv.GetMixMatchType() == MIXMATCH_ACTION_FREE_ITEM )
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = csv.GetPluNoNew();
						::ProcessPluNo( infoPluNo, FALSE, TRUE );
							
						if ( TRUE == infoPluNo.m_bValid )
						{
							int nMixMatchNo = csv.GetMixMatchNo();
							
							CEposReportConsolCheapestOneFreeSummary infoPromo;
							infoPromo.m_nMixMatchNo = nMixMatchNo;
							infoPromo.m_nSubtotalFlag = 0;
							infoPromo.m_nPluNo = infoPluNo.m_nBasePluNo;
							infoPromo.m_nQty = 1;
							infoPromo.m_dVal = ( csv.GetValue() * -1 );
							
							CEposReportConsolCheapestOneFreeSummary infoTotal = infoPromo;
							infoTotal.m_nSubtotalFlag = 1;
							infoTotal.m_nPluNo = 0;
							
							for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
							{
								int nBlock = m_arrayTerminalBlocks.GetAt(n);
								m_arrayReportBlocks[ nBlock ].m_pPromos -> Consolidate( infoPromo );
								m_arrayReportBlocks[ nBlock ].m_pPromos -> Consolidate( infoTotal );
								infoPromo.m_nMixMatchNo = 0;
								infoTotal.m_nMixMatchNo = 0;
								m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( infoPromo );
								m_arrayReportBlocks[ nBlock ].m_pTotals -> Consolidate( infoTotal );
								infoPromo.m_nMixMatchNo = nMixMatchNo;
								infoTotal.m_nMixMatchNo = nMixMatchNo;
							}
						}
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

bool CEposReportCheapestOneFreeSummary::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Plu No", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 350 );
	m_ReportFile.AddColumn ( "Quantity", TA_RIGHT, 250 );
	m_ReportFile.AddColumn ( "Discount", TA_RIGHT, 250 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportCheapestOneFreeSummaryBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
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
		CEposReportCheapestOneFreeSummaryBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

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
	}

	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCheapestOneFreeSummary::CreateSalesSection( CEposReportCheapestOneFreeSummaryBlock& Block )
{
	ShowPromos( Block, FALSE );
	ShowPromos( Block, TRUE );
}

/**********************************************************************/

void CEposReportCheapestOneFreeSummary::ShowPromos( CEposReportCheapestOneFreeSummaryBlock& Block, bool bTotals )
{
	CReportConsolidationArray<CEposReportConsolCheapestOneFreeSummary>* pArray;
	
	if ( TRUE == bTotals )
		pArray = Block.m_pTotals;
	else
		pArray = Block.m_pPromos;

	bool bWantHeader = TRUE;
	for ( int s = 0; s < pArray -> GetSize(); s++ )
	{
		CEposReportConsolCheapestOneFreeSummary infoConsol;
		pArray -> GetAt( s, infoConsol );
		UpdateCreateReportProgress();

		if ( TRUE == bWantHeader )
		{
			if ( TRUE == bTotals )
			{
				m_ReportFile.RequestBlankLines(1);
				m_ReportFile.WriteReportMiscLine( "Totals" );
				m_ReportFile.WriteReportMiscLine( "<LI>" );
				bWantHeader = FALSE;
			}
			else
			{
				int nMixMatchIdx;
				CString strMixMatchName = "";
				if ( DataManager.MixMatch.FindTableByNumber( infoConsol.m_nMixMatchNo, nMixMatchIdx ) == TRUE )
				{
					CMixMatchCSVRecord MixMatch;
					DataManager.MixMatch.GetAt( nMixMatchIdx, MixMatch );
					strMixMatchName = MixMatch.GetDisplayName();
				}

				if (strMixMatchName == "")
				{
					strMixMatchName.Format("Mix Match %d", infoConsol.m_nMixMatchNo);
				}

				CString strHeader = "";
				strHeader.Format( "<..>M%3.3d: %s", infoConsol.m_nMixMatchNo, (const char*) strMixMatchName );

				m_ReportFile.RequestBlankLines(1);
				m_ReportFile.WriteReportMiscLine( strHeader );
				m_ReportFile.WriteReportMiscLine( "<LI>" );
				bWantHeader = FALSE;
			}
		}

		if ( 1 == infoConsol.m_nSubtotalFlag )
		{
			m_ReportFile.WriteReportMiscLine( "\t\t<LI>\t<LI>" );

			CCSV csvOut( '\t' );
			csvOut.Add( "" );
			csvOut.Add( "Total" );
			csvOut.Add( infoConsol.m_nQty );
			csvOut.Add( infoConsol.m_dVal, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportMiscLine( csvOut.GetLine() );
			bWantHeader = TRUE;
		}
		else
		{
			CString strPluNo = ReportHelpers.GetDisplayPluNo( infoConsol.m_nPluNo, TRUE );
			CString strDescription = "Unknown";

			{
				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( infoConsol.m_nPluNo, nPluIdx ) == TRUE )
				{
					CPluCSVRecord PluRecord;
					DataManager.Plu.GetAt( nPluIdx, PluRecord );
					strDescription = PluRecord.GetEposText();
				}
			}

			CCSV csvOut( '\t' );
			csvOut.Add( strPluNo );
			csvOut.Add( strDescription );
			csvOut.Add( infoConsol.m_nQty );
			csvOut.Add( infoConsol.m_dVal, SysInfo.GetDPValue() );
			m_ReportFile.WriteReportDataLine( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/
