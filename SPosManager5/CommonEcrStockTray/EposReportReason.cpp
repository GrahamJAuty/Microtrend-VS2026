/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportReason.h"
/**********************************************************************/

CEposReportConsolReason::CEposReportConsolReason()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolReason::Reset()
{
	m_nServerLocOrDbSort = 0;
	m_nServerLocOrDbIdx = 0;
	m_nServerNo = 0;
	m_strDate = "";
	m_strTime = "";
	m_nLineType = 0;
	m_nLineNo = 0;
	m_nLocIdx = 0;
	m_nTrmIdx = 0;
	m_nBasePluNo = 0;
	m_nModifier = 0;
	m_strPluText = "";
	m_nReasonCode = 0;
	m_strReasonText = "";
	m_bGotValue = FALSE;
	m_dValue = 0.0;
}

/**********************************************************************/

void CEposReportConsolReason::Add (CEposReportConsolReason& source )
{
}

/**********************************************************************/

bool CEposReportConsolReason::CompareServer( CEposReportConsolReason& source )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return FALSE;

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return FALSE;

	if ( m_nServerNo != source.m_nServerNo )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

int CEposReportConsolReason::Compare( CEposReportConsolReason& source, int nHint )
{
	if ( m_nServerLocOrDbSort != source.m_nServerLocOrDbSort )
		return ( m_nServerLocOrDbSort > source.m_nServerLocOrDbSort ? 1 : -1 );

	if ( m_nServerLocOrDbIdx != source.m_nServerLocOrDbIdx )
		return ( m_nServerLocOrDbIdx > source.m_nServerLocOrDbIdx ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( m_nServerNo > source.m_nServerNo ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( m_strDate > source.m_strDate ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( m_strTime > source.m_strTime ? 1 : -1 );

	if ( m_nLineType != source.m_nLineType )
		return ( m_nLineType > source.m_nLineType ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( m_nLineNo > source.m_nLineNo ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportReason::CEposReportReason( CEposSelectArray& SelectArray, int nType ) : CEposReport( SelectArray )
{
	m_nReportType = nType;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
}

/**********************************************************************/

void CEposReportReason::Consolidate()
{
	m_nLineNo = 0;

	int nTotalFileSize;
	CArray<CReasonFileListInfo, CReasonFileListInfo> arrayReasonFiles;
	CEposReportFileListMaker FileListMaker( m_bEODMode, m_bSaleTime, m_PMSModes );
	FileListMaker.GetReasonFileList ( arrayReasonFiles, m_SelectArray, nTotalFileSize );

	int nProgress = 0;	
	StatusProgress.Lock( TRUE, "Consolidating report data" );

	int nCurrentDbIdx = -1;

	for ( int nFileIdx = 0; nFileIdx < arrayReasonFiles.GetSize(); nFileIdx++ )
	{
		int nSelIdx =		arrayReasonFiles[nFileIdx].m_nSelIdx;
		int nNetworkIdx =	arrayReasonFiles[nFileIdx].m_nNetworkIdx;
		CString strDate =	arrayReasonFiles[nFileIdx].m_strFilename.Left(8);

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem( nSelIdx, ListItem );
		int nTNo = dbLocation.GetTNo( ListItem.m_nLocIdx,  ListItem.m_nTermIdx );

		CString strFile	=	arrayReasonFiles[nFileIdx].m_strFilename;
		
		CString strPath = "";
		strPath += dbNetwork.GetFolderPathDataEODFiles( nNetworkIdx, "Reasons" );
		strPath += "\\";
		strPath += strFile;

		int nYear = atoi(strDate.Left(4));
		int nMonth = atoi(strDate.Mid(4,2));
		int nDay = atoi(strDate.Right(2));

		CSSFile fileReasons;
		if ( fileReasons.Open( strPath, "rb" ) == FALSE )
			continue;

		if ( ListItem.m_nDbIdx != nCurrentDbIdx )
		{
			CDataManagerInfo infoDM;
			DataManager.OpenDatabaseReadOnly( ListItem.m_nDbIdx, infoDM, FALSE );
			nCurrentDbIdx = ListItem.m_nDbIdx;
		}

		CString strLine;
		while( fileReasons.ReadString( strLine ) == TRUE )
		{
			StatusProgress.SetPos ( nProgress + fileReasons.Tell(), nTotalFileSize );

			int nCause = 0;
			__int64 nPluNo;
			int nServer = 0;
			int nReasonCode = 0;
			CString strCSVTime = "";
			CString strReasonText = "";
			bool bGotValue = FALSE;
			double dValue = 0.0;

			bool bAccept = TRUE;
			
			if ( ( strFile.GetLength() == 12 ) || ( strFile.Mid(9,3) == "con" ) )
			{
				CCSVReasonDataCON	csvReasonData( strLine );
				nCause =			csvReasonData.GetCause();
				nPluNo =			csvReasonData.GetPluNo();
				nServer =			csvReasonData.GetServer();
				nReasonCode =		csvReasonData.GetReasonCode();
				strCSVTime =		csvReasonData.GetTime();
				
				CCSV csvText( ' ' );
				for ( int n = 8; n < csvReasonData.GetSize(); n++ )
					csvText.Add( csvReasonData.GetString(n) );

				strReasonText = csvText.GetLine();
			}
			else
			{
				CCSVReasonDataCSV	csvReasonData( strLine );

				if ( csvReasonData.GetVersion() != 1 )
					bAccept = FALSE;
				else
				{
					nCause =			csvReasonData.GetCause();
					nPluNo =			csvReasonData.GetPluNo();
					nServer =			csvReasonData.GetServer();
					nReasonCode =		csvReasonData.GetReasonCode();
					strCSVTime =		csvReasonData.GetTime();
					strReasonText =		csvReasonData.GetReasonText();
					bGotValue =			TRUE;

					int nValue =		csvReasonData.GetValue();

					//MAKE REFUND AND PROMO VALUES POSITIVE
					switch( nCause )
					{
					case 3:
					case 5:
						nValue *= -1;
						break;
					}

					dValue =			( (double) nValue ) / 100.0;
				}
			}

			if ( FALSE == bAccept )
				continue;

			switch( m_nReportType )
			{
			case REPORT_TYPE_REASON_VOID:
				bAccept = ( ( 1 == nCause ) || ( 4 == nCause ) );
				break;
			
			case REPORT_TYPE_REASON_VIP:
				bAccept = ( 2 == nCause );
				break;
			
			case REPORT_TYPE_REASON_REFUND:
				bAccept = ( 3 == nCause );
				break;

			case REPORT_TYPE_REASON_PROMOTION:
				bAccept = ( 5 == nCause );
				break;

			case REPORT_TYPE_REASON_WASTAGE:
				bAccept = ( 6 == nCause );
				break;

			default:
				bAccept = FALSE;
			}

			if ( FALSE == bAccept )
				continue;

			if ( strCSVTime.GetLength() != 8 )
				continue;

			CString strTime = "";
			strTime += strCSVTime.Left(2);
			strTime += strCSVTime.Mid(3,2);
			strTime += strCSVTime.Right(2);

			if ( SimpleTimeCheck( ListItem.m_nLocIdx, strDate, strTime ) == FALSE )
				continue;

			CSQLPluNoInfo infoPluNo;
			CString strPluText = "";
			
			if ( nCause != 4 )
			{
				infoPluNo.m_nEposPluNo = nPluNo;
				::ProcessPluNo( infoPluNo, TRUE, TRUE );

				if ( FALSE == infoPluNo.m_bValid )
					continue;

				m_PluInfoFinder.SetPluNo( infoPluNo.m_nBasePluNo );

				strPluText = m_PluInfoFinder.GetPluInfoRepText( TRUE, infoPluNo.m_nModifier, TRUE );
				if ( strPluText == "" )
					strPluText = m_PluInfoFinder.GetPluInfoEcrText( TRUE, infoPluNo.m_nModifier, TRUE );
			}

			CEposReportConsolReason infoConsol;

			switch( m_nServerReportType )
			{
			case SERVER_REPORT_CODE:
			case SERVER_REPORT_SYS:
				infoConsol.m_nServerLocOrDbSort = 0;
				infoConsol.m_nServerLocOrDbIdx = 0;
				break;

			case SERVER_REPORT_NAME:
				infoConsol.m_nServerLocOrDbSort = 0;
				infoConsol.m_nServerLocOrDbIdx = DataManagerNonDb.ServerNameTable.GetServerNameIdx( ListItem.m_nLocIdx, nServer );
				break;

			case SERVER_REPORT_DB:
				infoConsol.m_nServerLocOrDbSort = dbDatabase.GetSortIdxFromDbIdx( ListItem.m_nDbIdx );
				infoConsol.m_nServerLocOrDbIdx = ListItem.m_nDbIdx;
				break;
	
			case SERVER_REPORT_LOC:
				infoConsol.m_nServerLocOrDbSort = dbLocation.GetSortIdxFromLocIdx( ListItem.m_nLocIdx );
				infoConsol.m_nServerLocOrDbIdx = ListItem.m_nLocIdx;
				break;
			}

			infoConsol.m_nServerNo = nServer;
			infoConsol.m_strDate = strDate;
			infoConsol.m_strTime = strTime;
			infoConsol.m_nLineType = nCause;
			infoConsol.m_nLineNo = m_nLineNo++;
			infoConsol.m_nLocIdx = ListItem.m_nLocIdx;
			infoConsol.m_nTrmIdx = ListItem.m_nTermIdx;
			infoConsol.m_nBasePluNo = infoPluNo.m_nBasePluNo;
			infoConsol.m_nModifier = infoPluNo.m_nModifier;
			infoConsol.m_strPluText = strPluText;
			infoConsol.m_nReasonCode = nReasonCode;
			infoConsol.m_strReasonText = strReasonText;
			infoConsol.m_bGotValue = bGotValue;
			infoConsol.m_dValue = dValue;
			m_ConsolArray.Consolidate( infoConsol );

			COleDateTime date = COleDateTime (
				nYear,
				nMonth,
				nDay,
				atoi( strTime.Left(2) ),
				atoi( strTime.Mid(2,2) ),
				atoi( strTime.Right(2) ) );

			m_SelectArray.ProcessStartDate( nSelIdx, date );
			m_SelectArray.ProcessEndDate( nSelIdx, date );
		}

		nProgress += fileReasons.GetLength();
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportReason::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	CString strTitle = GetReportTitle( FALSE );
	
	m_ReportFile.SetStyle1 ( strTitle );
	m_ReportFile.AddColumn ( "Date", TA_LEFT, 250 );
	m_ReportFile.AddColumn ( "Time", TA_LEFT, 200 );
	m_ReportFile.AddColumn ( "Location", TA_LEFT, 400 );
	m_ReportFile.AddColumn ( "Plu No", TA_LEFT, 380 );
	m_ReportFile.AddColumn ( "Description", TA_LEFT, 500 );
	m_ReportFile.AddColumn ( "Reason", TA_LEFT, 600 );
	m_ReportFile.AddColumn ( "Value", TA_RIGHT, 250 );
	
	CEposReportConsolReason CurrentServer;
	CurrentServer.m_nServerLocOrDbSort = -1;
	CurrentServer.m_nServerLocOrDbIdx = -1;

	int nSize = m_ConsolArray.GetSize();
	StatusProgress.Lock( TRUE, "Creating report" );

	double dTotalVal = 0.0;
	bool bGotValue = FALSE;

	for ( int i = 0 ; i < nSize ; i++ )
	{
		StatusProgress.SetPos( i, nSize );

		CEposReportConsolReason infoConsol;
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

		CString strLocName;
		strLocName.Format( "%s, %s",
			dbLocation.GetName(infoConsol.m_nLocIdx),
			dbLocation.GetTerminalName(infoConsol.m_nLocIdx, infoConsol.m_nTrmIdx) );

		CString strPluNo = "";
		CString strPluText = "";

		if ( 4 == infoConsol.m_nLineType )
		{
			strPluNo = "Transaction Void";
			strPluText = "N/A";
		}
		
		if ( infoConsol.m_nLineType != 4 )
		{
			strPluNo = ReportHelpers.GetDisplayPluNo( infoConsol.m_nBasePluNo, TRUE );

			if ( infoConsol.m_nModifier != 0 )
			{
				CString strExtra;
				strExtra.Format( " + %d", infoConsol.m_nModifier );
				strPluNo += strExtra;
			}

			strPluText = infoConsol.m_strPluText;
		}
	
		CCSV csv ( '\t' );
		csv.Add( strLineDate );
		csv.Add( strLineTime );
		csv.Add( strLocName );
		csv.Add( strPluNo );
		csv.Add( strPluText );
		csv.Add( infoConsol.m_strReasonText );

		if ( TRUE == infoConsol.m_bGotValue )
		{
			csv.Add( infoConsol.m_dValue, SysInfo.GetDPValue() );
			bGotValue = TRUE;
		}

		m_ReportFile.WriteReportDataLine ( csv.GetLine() );

		dTotalVal += infoConsol.m_dValue;
	}
	StatusProgress.Unlock();

	if ( TRUE == bGotValue )
	{
		m_ReportFile.WriteReportMiscLine ( "\t\t\t\t\t\t<LI>" );

		CCSV csvTotal( '\t' );
		csvTotal.Add( "" );
		csvTotal.Add( "" );
		csvTotal.Add( "" );
		csvTotal.Add( "" );
		csvTotal.Add( "" );
		csvTotal.Add( "Total" );
		csvTotal.Add( dTotalVal, SysInfo.GetDPValue() );
		m_ReportFile.WriteReportMiscLine( csvTotal.GetLine() );
	}

	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

