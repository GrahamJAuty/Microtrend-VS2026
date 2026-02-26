/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
//#include "DataManagerNonDb.h"
#include "LocationCSVArray.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportAgeVerify.h"
/**********************************************************************/

CEposReportAgeVerify::CEposReportAgeVerify( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_AGEVERIFY;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportAgeVerify::Consolidate()
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

	for ( int nFileIdx = 0; ( nFileIdx < arrayFileInfo.GetSize() ) && ( FALSE == m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() ); nFileIdx++ )
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
		while ( fileSales.ReadString ( strBuffer ) )
		{
			StatusProgress.SetPos ( nProgress + fileSales.Tell(), nTotalFileSize );

			CCSVEposTermLine csvIn ( strBuffer );
			
			int nCashRSPLineType = csvIn.GetLineTypeInt();
			if ( CASHRSP_LINETYPE_AGEVERIFY == nCashRSPLineType )
			{
				m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

				CString strDate = csvIn.GetAgeVerifyDate();
				CString strTime = csvIn.GetAgeVerifyTime();

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

				if ( SimpleTimeCheck( infoFile.m_nLocIdx, strTranDate, strTranTime ) == FALSE )
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
				infoConsol.m_nLimit = csvIn.GetAgeVerifyLimit();
				infoConsol.m_nResult = csvIn.GetAgeVerifyResult();
					
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

bool CEposReportAgeVerify::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Age", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Result", TA_LEFT, 1200 );
	
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
			if ( CurrentServer.m_nServerLocOrDbIdx != -1 )
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

		switch( infoConsol.m_nLimit )
		{
		case 1:		csv.Add(16);	break;
		case 2:		csv.Add(18);	break;
		case 3:		csv.Add(21);	break;
		case 4:		csv.Add(25);	break;
		default:	csv.Add("??");	break;
		}

		switch( infoConsol.m_nResult )
		{
		case 0:		csv.Add( "Declined" );	break;
		case 1:		csv.Add( "OK" );		break;
		default:	csv.Add( "Unknown" );	break;
		}

		m_ReportFile.WriteReportDataLine ( csv.GetLine() );
	}
	StatusProgress.Unlock();

	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

