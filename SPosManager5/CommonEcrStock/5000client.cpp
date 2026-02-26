/**********************************************************************/
#include <share.h>
#include <direct.h>
#include <io.h>
/**********************************************************************/
#include "5000SiteArray.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NetClient.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "5000Client.h"
/**********************************************************************/

C5000ClientPutFileInfo::C5000ClientPutFileInfo()
{
	m_strSourceFolder = "";
	m_strSourceFilename = "";
	m_strDestFolder = "";
	m_strDestFilename = "";
	m_strDestSubFolder = "";
	m_bCreateSubFolders = FALSE;
}

/**********************************************************************/

C5000ClientPutFileInfo::C5000ClientPutFileInfo( CCommsExportBase& commsExport, CJobListCSVArray& jobArray, CJobListCSVRecord& jobRecord )
{
	m_strSourceFolder = commsExport.GetSourceFolder();
	m_strSourceFilename = commsExport.GetSourceFilename();
	m_strDestFolder = commsExport.GetDestFolder();
	m_strDestFilename = "";
	m_strDestSubFolder = jobArray.GetDateFolder( jobRecord );
	m_bCreateSubFolders = TRUE;
}

/**********************************************************************/

void C5000ClientPutFileInfo::GetDownloadTexts( CString& strSource, CString& strTarget )
{
	strSource = "";
	strSource += m_strSourceFilename;

	strTarget = "";
	
	if ( m_strDestSubFolder != "" )
	{
		strTarget += m_strDestSubFolder;
		strTarget += "\\";
	}
	
	if ( m_strDestFilename != "" )
		strTarget += m_strDestFilename;
	else
		strTarget += m_strSourceFilename;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

C5000Client::C5000Client ( int nNetworkIdx, const char* szNetworkPath, CWnd* pStatusWnd )
	: CNetCommsClient ( szNetworkPath )
{
	m_pStatusWnd = pStatusWnd;
	m_nNetworkIdx = nNetworkIdx;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void C5000Client::LogMessage ( const char* szText, const char* szError, bool bHide )
{
	CString strText = "Client5000 ";
	strText += szText;

	CMessageLoggerInfo info;
	info.m_strMessage = szText;
	info.m_strStatus = szError;
	info.m_nHideLevel = ( bHide == TRUE ) ? 1 : 0;
	info.m_nNetworkIdx = m_nNetworkIdx;
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;

	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
}

void C5000Client::LogMessage ( const char* szType, int nCount, const char* szError, bool bHide )
{
	CString strMsg;
	strMsg.Format( "%s (%d)",
		szType,
		nCount );

	LogMessage( strMsg, szError, bHide );
}

/**********************************************************************/

void C5000Client::DisplayStatus ( const char* szText )
{
	if ( m_pStatusWnd != NULL )
		m_pStatusWnd->SetWindowText ( szText );
}

/**********************************************************************/

static int AppendFile ( const char* szSource, const char* szDest, int& nCount )
{
	nCount = 0;

	CSSFile fileSource;

	if ( fileSource.Open ( szSource, "rb" ) == TRUE )
	{
		CSSFile fileDest;

		if ( fileDest.Open ( szDest, "ab", _SH_DENYWR ) == TRUE )
		{
			CString strBuf;

			while ( fileSource.ReadLine ( strBuf ) == TRUE )
			{
				if ( fileDest.WriteString ( strBuf ) == FALSE )
					return 3;		// write error

				nCount++;
			}
			return 0;		// ok
		}
		return 2;			// open for append error
	}
	return 1;				// open for read error
}

/**********************************************************************/

bool C5000Client::Receive ( const char* szRemotePath, const char* szLocalPath )
{
	bool bReply = FALSE;

	DisplayStatus ( "Enquiring" );

	CStringArray FilenameArray;

	if ( TxCommand ( sz5000CmdReceive, FilenameArray ) == TRUE )
	{
		_mkdir ( szLocalPath );

		CString strExportPath = "";
		bool bMakeExportFile = FALSE;
		if ( DealerFlags.GetBatchExportFlag() == TRUE )
		{
			strExportPath = szLocalPath;
			strExportPath.MakeUpper();

			if ( strExportPath.GetLength() > 9 )
			{
				if ( strExportPath.Right(9) == "PENDING1\\" )
				{
					strExportPath.SetAt( strExportPath.GetLength() - 2, '4' );
					_mkdir ( strExportPath );
					bMakeExportFile = TRUE;
				}
			}
		}
		
		CString strMessage;

		if ( FilenameArray.GetSize() == 0 )
		{
			strMessage.Format ( "Receive: (%s) (%s)", szRemotePath, szLocalPath );
			LogMessage ( strMessage, "Ok (no data)" );
		}

		for ( int i = 0 ; i < FilenameArray.GetSize() ; i++ )
		{
			CString strFilename = FilenameArray.GetAt(i);	// send.nnn

			strMessage.Format ( "Receiving %s", strFilename );
			DisplayStatus ( strMessage );

			CString strRemoteFilename = szRemotePath;
			strRemoteFilename += strFilename;				// send.nnn

			CString strTempFilename = szLocalPath;
			strTempFilename += strFilename;				// send.nnn

			strMessage.Format ( "Receive: %s (%s) (%s)", strFilename, szRemotePath, szLocalPath );

			if ( CNetCommsClient::GetFile ( strFilename, szLocalPath, szRemotePath, FALSE, 0 ) == TRUE )	// copy file from server
			{
				CString strLocalFilename = szLocalPath;
				strLocalFilename += sz5000TermFilename;
				strLocalFilename += strFilename.Right( strFilename.GetLength() - 4);	// term.nnn

				int nCount;
				int nError = AppendFile ( strTempFilename, strLocalFilename, nCount );

				if ( nError == 0 )		// ok
				{
					if ( TRUE == bMakeExportFile )
					{
						COleDateTime timeNow = COleDateTime::GetCurrentTime();

						CString strExportFile;
						strExportFile.Format( "%s_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d.dat",
							(const char*) strTempFilename.Right(3),
							timeNow.GetYear(),
							timeNow.GetMonth(),
							timeNow.GetDay(),
							timeNow.GetHour(),
							timeNow.GetMinute(),
							timeNow.GetSecond() );

						CopyFile( strTempFilename, strExportPath + strExportFile, FALSE );
					}

					if ( DelFile ( strRemoteFilename ) == 0 )
						LogMessage ( strMessage, nCount, "Ok" );
					else
						LogMessage ( strMessage, nCount, "Delete error" );
				}
				else
				{
					m_strError.Format ( "Append error %d", nError );
					LogMessage ( strMessage, nCount, m_strError );
				}
			}
			else
			{
				LogMessage ( strMessage, CNetCommsClient::GetError() );
			}

			remove ( strTempFilename );
		}

		bReply = TRUE;
	}
	else
	{
		m_strError = CNetCommsClient::GetError();
		LogMessage ( "Receive:", m_strError );
	}

	return bReply;
}

/**********************************************************************/

bool C5000Client::ReceiveReasonFiles ( const char* szRemotePath, int nNetworkIdx, int nParam1 )
{
	CString strNetworkPath = dbNetwork.GetFolderPathData( nNetworkIdx );
	CString strNetworkHistoryPath = dbNetwork.GetFolderPathDataHistory( nNetworkIdx );
	CString strSlash = "\\";

	bool bReply = FALSE;
	
	DisplayStatus ( "Enquiring" );

	CStringArray FilenameArray;

	CCSV csvCommand;
	csvCommand.Add( sz5000CmdReceiveReasons );
	csvCommand.Add( nParam1 );
	
	if ( TxCommand ( csvCommand.GetLine(), FilenameArray ) == TRUE )
	{
		CString strMessage;

		if ( FilenameArray.GetSize() == 0 )
		{
			strMessage.Format ( "Receive: (%s) (%s)", szRemotePath, (const char*) strNetworkHistoryPath );
			LogMessage ( strMessage, "Ok (no data)" );
		}

		::MakeSubdirectory( strNetworkPath );
		::MakeSubdirectory( strNetworkHistoryPath );
		strNetworkHistoryPath += strSlash;

		CString strSlash = "\\";

		for ( int i = 0 ; i < FilenameArray.GetSize() ; i++ )
		{
			CCSV csv ( FilenameArray.GetAt(i) );

			if ( csv.GetSize() != 2 )
				continue;

			CString strEODFolder = csv.GetString(0);
			CString strReasonFilename = csv.GetString(1);

			CString strReasonFilepath = strEODFolder + strSlash + strReasonFilename;

			MakeSubdirectory ( strNetworkHistoryPath + strEODFolder );
			
			strMessage.Format ( "Receiving %s", strReasonFilepath );
			DisplayStatus ( strMessage );

			strMessage.Format ( "Receive: %s (%s) (%s)", strReasonFilepath, szRemotePath, strNetworkHistoryPath  );

			if ( CNetCommsClient::GetFile ( strReasonFilepath, strNetworkHistoryPath, szRemotePath, FALSE, 0 ) == TRUE )	// copy file from server
			{
				dbNetwork.WriteReasonTokenFile( nNetworkIdx );

				CString strRemoteReasonFilepath = szRemotePath;
				strRemoteReasonFilepath += strReasonFilepath;				

				if ( DelFile ( strRemoteReasonFilepath ) == 0 )
					LogMessage ( strMessage, 0, "Ok" );
				else
					LogMessage ( strMessage, 0, "Delete error" );
			}
			else
			{
				LogMessage ( strMessage, CNetCommsClient::GetError() );
			}
		}
		
		bReply = TRUE;
	}
	else
	{
		m_strError = CNetCommsClient::GetError();
		LogMessage ( "Receive:", m_strError );
	}

	return bReply;
}

/**********************************************************************/

bool C5000Client::ReceiveEODPluFile ( const char* szRemotePath, int nLocIdx )
{
	bool bReply = FALSE;

	DisplayStatus ( "Enquiring" );

	CString strSourcePath = "";
	dbLocation.GetSafeTerminalExportPath( nLocIdx, strSourcePath );
	
	if ( dbLocation.GetProdCSVImportFolderType( nLocIdx ) == EODIMPORT_FOLDERTYPE_INTRADING )
		strSourcePath += "INTRADING\\";

	CString strDestPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( nLocIdx );

	CCSV csvCommand;
	csvCommand.Add( sz5000CmdReceiveEODPlu );
	csvCommand.Add( strSourcePath );
	csvCommand.Add( dbLocation.GetRemoteID( nLocIdx ) );

	CStringArray FilenameArray;
	if ( TxCommand ( csvCommand.GetLine(), FilenameArray ) == TRUE )
	{
		_mkdir ( strDestPath );
		strDestPath += "\\";

		CString strMessage;

		if ( FilenameArray.GetSize() == 0 )
		{
			strMessage.Format ( "Receive: (%s) (%s)", (const char*) strSourcePath, (const char*) strDestPath );
			LogMessage ( strMessage, "Ok (no data)" );
			bReply = TRUE;
		}

		for ( int i = 0 ; i < FilenameArray.GetSize() ; i++ )
		{
			CString strFilename = FilenameArray.GetAt(i);

			if ( strFilename.Left(3) == "###" )
			{
				strSourcePath = strFilename.Right( strFilename.GetLength() - 3 );
				strSourcePath += "\\";
				continue;
			}
			
			strMessage.Format ( "Receiving %s", strFilename );
			DisplayStatus ( strMessage );

			strMessage.Format ( "Receive: %s (%s) (%s)", 
				(const char*) strFilename, 
				(const char*) strSourcePath, 
				(const char*) strDestPath );

			CString strRemotePath = ""; 
			strRemotePath += szRemotePath;
			strRemotePath += strSourcePath;

			if ( CNetCommsClient::GetFile ( strFilename, strDestPath, strRemotePath, FALSE, 0 ) == TRUE )	// copy file from server
			{
				CString strRemoteFilePath = "";
				strRemoteFilePath += strRemotePath;
				strRemoteFilePath += strFilename;

				if ( DelFile ( strRemoteFilePath ) == 0 )
					LogMessage ( strMessage, 0, "Ok" );
				else
					LogMessage ( strMessage, 0, "Delete error" );

				bReply = TRUE;
			}
			else
			{
				LogMessage ( strMessage, CNetCommsClient::GetError() );
			}
		}
	}
	else
	{
		m_strError = CNetCommsClient::GetError();
		LogMessage ( "Receive:", m_strError );
	}

	return bReply;
}

/**********************************************************************/

bool C5000Client::ReceiveEODCustomerFile ( const char* szRemotePath, int nLocIdx )
{
	bool bReply = FALSE;

	DisplayStatus ( "Enquiring" );

	CString strSourcePath = "";
	dbLocation.GetSafeTerminalExportPath( nLocIdx, strSourcePath );

	if ( dbLocation.GetCustomerImportFolderType( nLocIdx ) == EODIMPORT_FOLDERTYPE_INTRADING )
		strSourcePath += "INTRADING\\";

	CString strDestPath = dbLocation.GetFolderPathSyssetPendingCustomerLocal( nLocIdx );

	CCSV csvCommand;
	csvCommand.Add( sz5000CmdReceiveEODCustomer );
	csvCommand.Add( strSourcePath );
	csvCommand.Add( dbLocation.GetRemoteID( nLocIdx ) );

	CStringArray FilenameArray;
	if ( TxCommand ( csvCommand.GetLine(), FilenameArray ) == TRUE )
	{
		_mkdir ( strDestPath );
		strDestPath += "\\";

		CString strMessage;

		if ( FilenameArray.GetSize() == 0 )
		{
			strMessage.Format ( "Receive: (%s) (%s)", (const char*) strSourcePath, (const char*) strDestPath );
			LogMessage ( strMessage, "Ok (no data)" );
			bReply = TRUE;
		}

		for ( int i = 0 ; i < FilenameArray.GetSize() ; i++ )
		{
			CString strFilename = FilenameArray.GetAt(i);

			if ( strFilename.Left(3) == "###" )
			{
				strSourcePath = strFilename.Right( strFilename.GetLength() - 3 );
				strSourcePath += "\\";
				continue;
			}
			
			strMessage.Format ( "Receiving %s", strFilename );
			DisplayStatus ( strMessage );

			strMessage.Format ( "Receive: %s (%s) (%s)", 
				(const char*) strFilename, 
				(const char*) strSourcePath, 
				(const char*) strDestPath );

			CString strRemotePath = ""; 
			strRemotePath += szRemotePath;
			strRemotePath += strSourcePath;

			if ( CNetCommsClient::GetFile ( strFilename, strDestPath, strRemotePath, FALSE, 0 ) == TRUE )	// copy file from server
			{
				CString strRemoteFilePath = "";
				strRemoteFilePath += strRemotePath;
				strRemoteFilePath += strFilename;

				if ( DelFile ( strRemoteFilePath ) == 0 )
					LogMessage ( strMessage, 0, "Ok" );
				else
					LogMessage ( strMessage, 0, "Delete error" );

				bReply = TRUE;
			}
			else
			{
				LogMessage ( strMessage, CNetCommsClient::GetError() );
			}
		}
	}
	else
	{
		m_strError = CNetCommsClient::GetError();
		LogMessage ( "Receive:", m_strError );
	}

	return bReply;
}

/**********************************************************************/

bool C5000Client::PutFile( C5000ClientPutFileInfo& infoPutFile )
{
	CString strDestFilename = infoPutFile.m_strDestFilename;
	if ( strDestFilename == "" )
		strDestFilename = infoPutFile.m_strSourceFilename;

	CString strMessage = "";
	strMessage += "Sending ";
	strMessage += infoPutFile.m_strSourceFilename;
	strMessage += " => ";
	strMessage += strDestFilename;
	DisplayStatus( strMessage );

	CString strDestFolder = infoPutFile.m_strDestFolder;

	if ( infoPutFile.m_strDestSubFolder != "" )
	{
		strDestFolder += infoPutFile.m_strDestSubFolder;

		if ( TRUE == infoPutFile.m_bCreateSubFolders )
			_mkdir( strDestFolder );

		strDestFolder += "\\";
	}
	
	CString strDestFilePath = "";
	strDestFilePath += strDestFolder;
	strDestFilePath += strDestFilename;
	
	CString strSourceFilePath = "";
	strSourceFilePath += infoPutFile.m_strSourceFolder;
	strSourceFilePath += infoPutFile.m_strSourceFilename;
	
	bool bReply;

	if ( CopyFile ( strSourceFilePath, strDestFilePath, FALSE ) != 0 )
	{
		m_strError = "Ok";
		bReply = TRUE;
	}
	else
	{
		m_strError.Format ( "Error: %s", GetLastRasCommsErrorMessage() );
		bReply = FALSE;
	}

	strMessage.Format ( "Send: (%s) => (%s)", (const char*) strSourceFilePath, (const char*) strDestFilePath );

	LogMessage ( strMessage, m_strError );

	return bReply;
}

/**********************************************************************/

bool C5000Client::DeleteFile ( const char* szFilename, const char* szRemotePath )
{
	CString strMessage;
	strMessage.Format ( "Deleting %s", szFilename );
	DisplayStatus ( strMessage );

	CString strRemoteFilename = szRemotePath;

	strRemoteFilename += szFilename;

	bool bReply;

	if ( remove ( strRemoteFilename ) == 0 )
	{
		m_strError = "Ok";
		bReply = TRUE;
	}
	else
	{
		m_strError.Format ( "Error: %s", GetLastRasCommsErrorMessage() );
		bReply = FALSE;
	}

	strMessage.Format ( "Delete: %s (%s) ", szFilename, szRemotePath );

	LogMessage ( strMessage, m_strError );

	return bReply;
}

/**********************************************************************/
