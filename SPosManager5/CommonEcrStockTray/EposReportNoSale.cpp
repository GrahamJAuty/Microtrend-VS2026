/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
#include "LocationCSVArray.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportNoSale.h"
/**********************************************************************/

CEposReportNoSale::CEposReportNoSale( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_NOSALE;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportNoSale::Consolidate()
{
	int nTotalFileSize;
	CReportConsolidationArray<CTermFileListInfo> arrayFileInfo;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetTermFileList ( arrayFileInfo, m_SelectArray, FALSE, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );
	
	int nServer = 0;
	COleDateTime oleTranDate;
	CString strTranDate = "";
	CString strTranTime = "";

	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ); nFileIdx++ )
	{
		CTermFileListInfo infoFile;
		arrayFileInfo.GetAt( nFileIdx, infoFile );
		
		CSSFile fileSales;
		if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
			continue;

		int nDbIdx;
		int nDbNo = dbLocation.GetDbNo( infoFile.m_nLocIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			nDbIdx = 0;

		CDataManagerInfo infoDM;
		DataManager.OpenDatabaseReadOnly( nDbIdx, infoDM, FALSE );

		m_CashRSPVersionChecker.ResetTransactionVersions();

		CString strBuffer;
		while ( ( m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE ) && ( fileSales.ReadString ( strBuffer ) ) )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVEposTermLine csvIn ( strBuffer );
		
			int nCashRSPLineType = csvIn.GetLineTypeInt();
			if ( CASHRSP_LINETYPE_NOSALE == nCashRSPLineType )
			{
				m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

				CString strDate = csvIn.GetNoSaleDate();
				CString strTime = csvIn.GetNoSaleTime();

				//FILTER FOR DATE AND TIME
				if ( strDate.GetLength() != 8 || strTime.GetLength() != 8 )
					continue;
				
				strTranDate.Format ( "20%s%s%s",
					(const char*) strDate.Right(2),
					(const char*) strDate.Mid(3,2),
					(const char*) strDate.Left(2) );

				strTranTime.Format ( "%s%s%s",
					(const char*) strTime.Left(2),
					(const char*) strTime.Mid(3,2),
					(const char*) strTime.Right(2) );

				if ( SimpleTimeCheck ( infoFile.m_nLocIdx, strTranDate, strTranTime ) == FALSE )
					continue;

				nServer = csvIn.GetNoSaleServer();
				
				CEposReportConsolAgeVerifyNoSale infoConsol;
				
				switch( m_nServerReportType )
				{
				case SERVER_REPORT_CODE:
				case SERVER_REPORT_SYS:
					infoConsol.m_nServerLocOrDbSort = 0;
					infoConsol.m_nServerLocOrDbIdx = 0;
					break;

				case SERVER_REPORT_NAME:
					infoConsol.m_nServerLocOrDbSort = 0;
					infoConsol.m_nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( infoFile.m_nLocIdx, nServer );
					break;

				case SERVER_REPORT_DB:
					infoConsol.m_nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx( nDbIdx );
					infoConsol.m_nServerLocOrDbIdx = nDbIdx;
					break;
				
				case SERVER_REPORT_LOC:
					infoConsol.m_nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx( infoFile.m_nLocIdx );
					infoConsol.m_nServerLocOrDbIdx = infoFile.m_nLocIdx;
					break;
				}

				oleTranDate = COleDateTime (
						atoi(strTranDate.Left(4)),
						atoi(strTranDate.Mid(4,2)),
						atoi(strTranDate.Right(2)),
						0, 0, 0 );

				infoConsol.m_nServerNo = nServer;
				infoConsol.m_strDate = strTranDate;
				infoConsol.m_strTime = strTranTime;
				infoConsol.m_nLimit = 0;
				infoConsol.m_nResult = 0;

				{
					CSortedIntByString item;
					item.m_strItem = csvIn.GetNoSaleReasonText();

					int nIdx;
					if ( m_arrayReasonLookup.Find( item, nIdx ) == TRUE )
					{
						m_arrayReasonLookup.GetAt( nIdx, item );
						infoConsol.m_nReasonIdx = item.m_nItem;
					}
					else
					{
						item.m_nItem = m_arrayReasonTexts.GetSize();
						m_arrayReasonLookup.InsertAt( nIdx, item );
						m_arrayReasonTexts.Add( item.m_strItem );
						infoConsol.m_nReasonIdx = item.m_nItem;
					}
				}
					
				m_ConsolArray.Consolidate( infoConsol );
				m_SelectArray.ProcessStartDate ( infoFile.m_nSelectArrayIdx, oleTranDate );
				m_SelectArray.ProcessEndDate( infoFile.m_nSelectArrayIdx, oleTranDate );
			}

			switch( nCashRSPLineType )
			{
			case CASHRSP_LINETYPE_VERSION:
				m_CashRSPVersionChecker.ProcessCashRSPVersionLine( csvIn );
				break;

			case CASHRSP_LINETYPE_APPLICATION:
				m_CashRSPVersionChecker.ProcessAppTypeLine( csvIn );
				break;

			default:
				m_CashRSPVersionChecker.ResetPendingTransactionInfo();
				break;
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportNoSale::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Reason", TA_LEFT, 600 );
	
	CEposReportConsolAgeVerifyNoSale CurrentServer;
	CurrentServer.m_nServerLocOrDbSort = -1;
	CurrentServer.m_nServerLocOrDbIdx = -1;
	
	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolAgeVerifyNoSale infoConsol;
		m_ConsolArray.GetAt( i, infoConsol );

		if ( infoConsol.CompareServer( CurrentServer ) == FALSE )
		{
			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine( GetServerHeader( infoConsol.m_nServerLocOrDbIdx, infoConsol.m_nServerNo ) );
			m_ReportFile.WriteReportMiscLine ( "<LI>" );
			
			CurrentServer = infoConsol;
		}
			
		CString strLineDate;
		strLineDate.Format ( "%s/%s/%s",
			(const char*) infoConsol.m_strDate.Right(2),
			(const char*) infoConsol.m_strDate.Mid(4,2),
			(const char*) infoConsol.m_strDate.Left(4) );

		CString strLineTime;
		strLineTime.Format ( "%s:%s:%s",
			(const char*) infoConsol.m_strTime.Left(2),
			(const char*) infoConsol.m_strTime.Mid(2,2),
			(const char*) infoConsol.m_strTime.Right(2) );

		CCSV csv ( '\t' );
		csv.Add( strLineDate );
		csv.Add( strLineTime );

		int nIdx = infoConsol.m_nReasonIdx;
		if ( ( nIdx >= 0 ) && ( nIdx < m_arrayReasonTexts.GetSize() ) )
		{
			CCSV csvReason( m_arrayReasonTexts.GetAt( nIdx ), ',', '\'' );
			csv.Add( csvReason.GetString(0) );
		}
		else
		{
			csv.Add("");
		}

		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}

	StatusProgress.Unlock();
	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

