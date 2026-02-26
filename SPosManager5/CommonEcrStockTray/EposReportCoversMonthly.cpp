/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
//#include "DealerFlags.h"
//#include "EcrmanOptionsIni.h"
#include "EposReportConsolParams.h"
//#include "globalfunctions.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "PMSReportFileFinder.h"
#include "ReportTypes.h"
#include "SalesHistoryDateInfo.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCoversMonthly.h"
/**********************************************************************/

CEposReportCoversMonthly::CEposReportCoversMonthly( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_COVERS_MONTHLY;
	SetPMSAllowMode( PMS_ALLOWMODE_ACCOUNT );
}

/**********************************************************************/

CEposReportCoversMonthly::~CEposReportCoversMonthly()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

void CEposReportCoversMonthly::InitialiseConsolidationBlocks()
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

		CEposReportCoversMonthlyBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolCoversMonthly>;
		block.m_nCovers = 0;

		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

bool CEposReportCoversMonthly::CheckGraph()
{
	return TRUE;
}

/**********************************************************************/

void CEposReportCoversMonthly::Consolidate()
{
	m_BusinessDateInfo.Reset();
	InitialiseConsolidationBlocks();

	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetFileList ( arrayFileInfo, HISTORYFILE_TYPE_SALES, m_SelectArray, TRUE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	int nLastSelectArrayIdx = -1;

	for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );

		if ( ( infoFile.m_nSelectArrayIdx != nLastSelectArrayIdx ) || ( dbLocation.IsPMSPseudoLocation( infoFile.m_nLocIdx ) ) )
		{
			m_arrayTranMonth.RemoveAll();
			nLastSelectArrayIdx = infoFile.m_nSelectArrayIdx;
		}

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
		CString strLastSaleTime = "";
		CString strLastTranTime = "";

		int nDbNo = 0;
		if ( NODE_SYSTEM == m_nConLevel )
		{
			nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
			
			int nDbIdx;
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
			{
				CDataManagerInfo info;
				DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
			}
		}

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

				//FILTER FOR NON SALE LINES
				if ( csv.GetLineType() != CSVPLUDATA_PLU_SALE )
					continue;

				//FILTER LINES WITH NO COVERS
				int nLineCovers = csv.GetCovers();
				if ( 0 == nLineCovers ) 
				{
					if ( DealerFlags.GetAssumeCoversFlag() == TRUE )
						nLineCovers = 1;
					else
						continue;
				}

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

				int nYear = 0;
				int nMonth = 0;
				GetYearAndMonth( infoFile.m_nLocIdx, strThisSaleDate, strThisSaleTime, nYear, nMonth );

				if ( 0 == nYear )
					continue;

				int nCoversForMonth = 0;
				int nCoversForBlockTotal = 0;
				CString strThisTranTime = csv.GetTranTime();		
				if ( ( strThisTranTime != strLastTranTime ) || ( strThisSaleTime != strLastSaleTime ) )
				{
					CEposReportConsolTranMonth infoTranMonth;
					infoTranMonth.m_strDateTran = infoFile.m_strDateTran;
					infoTranMonth.m_strTimeTran = strThisTranTime;
					infoTranMonth.m_nYear = nYear;
					infoTranMonth.m_nMonth = nMonth;

					//INCREMENT COVERS BY MONTH FOR NEW COMBINATIONS OF TRAN DATE/TIME AND SALE MONTH
					int nCoversPos;
					if ( m_arrayTranMonth.Find( infoTranMonth, nCoversPos ) == FALSE )
					{
						nCoversForMonth = nLineCovers;
						m_arrayTranMonth.InsertAt( nCoversPos, infoTranMonth );
			
						//INCREMENT BLOCK TOTAL COVERS FOR NEW TRAN DATE/TIME ONLY
						if ( strThisTranTime != strLastTranTime )
						{
							nCoversForBlockTotal = nCoversForMonth;

							for ( int x = 0; x <= 1; x++ )
							{
								int nPos = ( nCoversPos - 1 ) + ( x * 2 );
								if ( ( nPos >= 0 ) && ( nPos < m_arrayTranMonth.GetSize() ) )
								{
									CEposReportConsolTranMonth infoTemp;
									m_arrayTranMonth.GetAt( nPos, infoTemp );
								
									if ( infoTemp.TranCompare( infoTranMonth ) == TRUE )
									{
										nCoversForBlockTotal = 0;
										break;
									}
								}
							}
						}
					}

					strLastTranTime = strThisTranTime;
					strLastSaleTime = strThisSaleTime;
				}

				if ( nCoversForMonth != 0 )
				{
					CEposReportConsolCoversMonthly ConsolInfo;
					ConsolInfo.m_nYear = nYear;
					ConsolInfo.m_nMonth = nMonth;
					ConsolInfo.m_nQty = nCoversForMonth;
					
					for ( int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++ )
					{
						int nBlock = m_arrayTerminalBlocks.GetAt(n);
						m_arrayReportBlocks[ nBlock ].m_pSales -> Consolidate( ConsolInfo );
						m_arrayReportBlocks[ nBlock ].m_BlockTotal.Add( ConsolInfo );
						m_arrayReportBlocks[ nBlock ].m_nCovers += nCoversForBlockTotal;
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

void CEposReportCoversMonthly::GetYearAndMonth( int nLocIdx, CString& strDateToCheck, CString& strTimeToCheck, int& nYear, int& nMonth )
{
	nYear = 0;
	nMonth = 0;

	if ( m_BusinessDateInfo.ValidateBusinessDate( nLocIdx, m_bEODMode, strDateToCheck, strTimeToCheck ) == TRUE )
	{
		COleDateTime oleBusinessDate = m_BusinessDateInfo.GetBusinessDate();	
		nYear = oleBusinessDate.GetYear();
		nMonth = oleBusinessDate.GetMonth();
	}
}

/**********************************************************************/

bool CEposReportCoversMonthly::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Month", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Qty", TA_RIGHT, 300 );

	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		CEposReportCoversMonthlyBlock ReportBlock = m_arrayReportBlocks.GetAt( n );
		m_nCreateReportTarget += ReportBlock.m_pSales -> GetSize();
	}
	
	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if ( m_nCreateReportMiniTarget < 1 )
		m_nCreateReportMiniTarget = 1;

	StatusProgress.Lock( TRUE, "Creating report" );

	m_BlockMap.BuildSortArray();
	for ( int n = 0; n < m_BlockMap.GetSortArraySize(); n++ )
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportCoversMonthlyBlock ReportBlock = m_arrayReportBlocks.GetAt( nIdx );

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

void CEposReportCoversMonthly::CreateSalesSection( CEposReportCoversMonthlyBlock& Block )
{
	for ( int s = 0; s < Block.m_pSales -> GetSize(); s++ )
	{
		CEposReportConsolCoversMonthly Sales;
		Block.m_pSales -> GetAt( s, Sales );
		UpdateCreateReportProgress();

		COleDateTime dateReport = COleDateTime( Sales.m_nYear, Sales.m_nMonth, 1, 0, 0, 0 );

		CString strRepMonth = dateReport.Format ( "%B %Y" );

		CCSV csv ( '\t' );
		csv.Add ( strRepMonth );
		csv.Add ( Sales.m_nQty );
		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}
}

/**********************************************************************/

bool CEposReportCoversMonthly::CreateGraph()
{
	CSSFile fileGraph;
	if ( fileGraph.Open ( Super.ReportFilename(), "wb" ) == FALSE )
		return FALSE;

	CCSV csvOut;
	csvOut.Add ( "Month" );
	csvOut.Add ( "Covers Qty" );
	csvOut.Add ( GetReportNameInternal( m_nReportType ) );
	csvOut.Add ( GetReportTitle(TRUE) );
	fileGraph.WriteLine( csvOut.GetLine() );

	CEposReportCoversMonthlyBlock ReportBlock = m_arrayReportBlocks.GetAt( 0 );
	int nSize = ReportBlock.m_pSales -> GetSize();

	for ( int i = 0 ; i < nSize ; i++ )
	{
		CEposReportConsolCoversMonthly ConsolInfo;
		ReportBlock.m_pSales -> GetAt( i, ConsolInfo );

		COleDateTime dateReport = COleDateTime( ConsolInfo.m_nYear, ConsolInfo.m_nMonth, 1, 0, 0, 0 );

		CString strRepMonth = dateReport.Format ( "%B %Y" );

		CCSV csvOut;
		csvOut.Add ( strRepMonth );
		csvOut.Add ( ConsolInfo.m_nQty );
		fileGraph.WriteLine( csvOut.GetLine() );
	}
	return TRUE;
}

/**********************************************************************/

void CEposReportCoversMonthly::WriteBlockTotals( CEposReportCoversMonthlyBlock& Block )
{
	CString strType = GetTotalTypeName( TRUE, Block.m_nEntityType, FALSE, TRUE );
		
	m_ReportFile.WriteReportMiscLine ( "\t<LI>" );
	CCSV csv ( '\t' );
	csv.Add ( strType );
	csv.Add ( Block.m_nCovers );
	m_ReportFile.WriteReportMiscLine( csv.GetLine() );
}

/**********************************************************************/
