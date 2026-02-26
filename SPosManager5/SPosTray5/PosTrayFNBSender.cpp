/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
#include "FNBDatabaseOptions.h"
#include "FTPConnect.h"
//#include "globalfunctions.h"
#include "MessageLogger.h"
/**********************************************************************/
#include "PosTrayFNBSender.h"
/**********************************************************************/

CPosTrayFNBSender::CPosTrayFNBSender(void)
{
}

/**********************************************************************/

CPosTrayFNBSender::~CPosTrayFNBSender(void)
{
}

/**********************************************************************/

void CPosTrayFNBSender::SendExports()
{
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		m_strSentFolder = "";
		m_strSentFolder += dbDatabase.GetFolderPathSysset( nDbIdx );
		m_strSentFolder += "\\FNB";
		::CreateSubdirectory( m_strSentFolder );
		m_strSentFolder += "\\SENT";
		::CreateSubdirectory( m_strSentFolder );

		CFNBDatabaseOptions FNBOptions;
		CStringArray arrayFilenames;
		GetFileList( nDbIdx, FNBOptions, arrayFilenames );
		
		m_strExtraErrorInfo = "";
		int nResult = FTPTransfer( nDbIdx, FNBOptions, arrayFilenames );
		
		if ( FNB_EXPORT_ERROR_NONE != nResult )
		{
			CMessageLoggerInfo info;
			info.m_bAddDate = TRUE;
			info.m_bAddTimeOfDay = TRUE;
			info.m_bFollowingLineNetwork = FALSE;
			info.m_bFollowingLineSystem = FALSE;
			info.m_bLeadingLineNetwork = FALSE;
			info.m_bLeadingLineSystem = FALSE;
			info.m_bLogSystemStartStop = FALSE;
			info.m_nHideLevel = 0;
			info.m_nNetworkIdx = 0;
			info.m_bAddNetworkName = FALSE;

			CString strExtra = "";
			if ( m_strExtraErrorInfo != "" )
			{
				strExtra += " (";
				strExtra += m_strExtraErrorInfo;
				strExtra += ")";
			}
	
			switch( nResult )
			{
			case FNB_EXPORT_ERROR_FTP_CONNECT:
				info.m_bLeadingLineNetwork = TRUE;
				info.m_strMessage = "FTP Connection Error : ";
				break;

			case FNB_EXPORT_ERROR_FTP_PUTFILE:	
				info.m_strMessage = "FTP Send File Error : ";
				break;

			default:
				info.m_strMessage = "Unknown Error : ";
				break;
			}

			info.m_strMessage += strExtra;
			MessageLogger.LogFNBSystem( info );
		}
	}
}

/**********************************************************************/

void CPosTrayFNBSender::GetFileList( int nDbIdx, CFNBDatabaseOptions& FNBOptions, CStringArray& arrayFilenames )
{
	CString strFolder = "";
	strFolder += dbDatabase.GetFolderPathSysset( nDbIdx );
	strFolder += "\\FNB\\EXPORTS\\";

	CString strFilenameOptions = FNBOptions.GetFilename( nDbIdx, FALSE );

	if ( ::FileExists( strFilenameOptions ) == FALSE )
		return;
		
	if ( FNBOptions.Read( nDbIdx, FALSE ) == FALSE )
		return;

	if ( FNBOptions.GetEnable() == FALSE )
		return;

	CStringArray arrayAlreadySent;

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strFolder + "*.dat" );

	while ( TRUE == bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
			
		if ( strFilename.GetLength() != 12 )
			continue;

		if ( ::TestNumeric( strFilename.Left(8) ) == FALSE )
			continue;

		CString strSentFile = m_strSentFolder;
		strSentFile += "\\";
		strSentFile += strFilename.Left(6);
		strSentFile += ".DAT";

		bool bAlreadySent = FALSE;
		
		CSSFile fileSent;
		if ( fileSent.Open( strSentFile, "rb" ) == TRUE )
		{
			CString strDay = strFilename.Mid(6,2);

			CString strBuffer;
			while( fileSent.ReadString( strBuffer ) == TRUE )
			{
				if ( strBuffer == strDay )
				{
					bAlreadySent = TRUE;
					break;
				}
			}
		}

		if ( FALSE == bAlreadySent )
			arrayFilenames.Add( strFilename );
		else
		{
			CMessageLoggerInfo info;
			info.m_bAddDate = TRUE;
			info.m_bAddTimeOfDay = TRUE;
			info.m_bFollowingLineNetwork = FALSE;
			info.m_bFollowingLineSystem = FALSE;
			info.m_bLeadingLineNetwork = ( arrayAlreadySent.GetSize() == 0 );
			info.m_bLeadingLineSystem = FALSE;
			info.m_bLogSystemStartStop = FALSE;
			info.m_nHideLevel = 0;
			info.m_nNetworkIdx = 0;
			info.m_bAddNetworkName = FALSE;
			
			info.m_strMessage = "";
			info.m_strMessage += "File Already Sent (";
			info.m_strMessage +=  strFilename;
			info.m_strMessage +=  ")";
		
			MessageLogger.LogFNBSystem( info );

			arrayAlreadySent.Add( strFilename );
		}
	}

	for ( int n = 0; n < arrayAlreadySent.GetSize(); n++ )
		CFileRemover FileRemover( strFolder + arrayAlreadySent.GetAt(n) );
}

/**********************************************************************/

int CPosTrayFNBSender::FTPTransfer( int nDbIdx, CFNBDatabaseOptions& FNBOptions, CStringArray& arrayFilenames )
{
	int nResult = FNB_EXPORT_ERROR_NONE;

	if ( arrayFilenames.GetSize() == 0 )
		return nResult;

	CString strFolder = "";
	strFolder += dbDatabase.GetFolderPathSysset( nDbIdx );
	strFolder += "\\FNB\\EXPORTS\\";

	CFtpConnect FtpConnect;
	if ( FtpConnect.ConnectFNBExport( FNBOptions ) == TRUE )
	{
		{
			CMessageLoggerInfo info;
			info.m_bAddDate = TRUE;
			info.m_bAddNetworkName = FALSE;
			info.m_bAddTimeOfDay = TRUE;
			info.m_bFollowingLineNetwork = FALSE;
			info.m_bFollowingLineSystem = FALSE;
			info.m_bLeadingLineNetwork = TRUE;
			info.m_bLeadingLineSystem = FALSE;
			info.m_bLogSystemStartStop = FALSE;
			info.m_nHideLevel = 0;
			info.m_strMessage = "Connect : ";
			info.m_strMessage += FNBOptions.GetFTPServer();
			info.m_strStatus = "OK";
			MessageLogger.LogFNBSystem( info );
		}

		for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
		{
			CString strFilename = arrayFilenames.GetAt(n);

			CString strSource = "";
			strSource += strFolder;
			strSource += strFilename;

			CString strDest = FNBOptions.GetFTPFolder();
			if ( strDest.GetLength() != 0 )
				if ( strDest.Right(1) != "\\" )
					strDest += "\\";

			strDest += strFilename;

			if ( FtpConnect.PutFile( strSource, strDest ) == TRUE )
			{
				{
					CMessageLoggerInfo info;
					info.m_bAddDate = TRUE;
					info.m_bAddNetworkName = FALSE;
					info.m_bAddTimeOfDay = TRUE;
					info.m_bFollowingLineNetwork = FALSE;
					info.m_bFollowingLineSystem = FALSE;
					info.m_bLeadingLineNetwork = FALSE;
					info.m_bLeadingLineSystem = FALSE;
					info.m_bLogSystemStartStop = FALSE;
					info.m_nHideLevel = 0;
					info.m_strMessage = "Sent File : ";
					info.m_strMessage += strSource.Right(30);
					info.m_strStatus = "OK";
					MessageLogger.LogFNBSystem( info );
				}

				CString strSentFile = m_strSentFolder;
				strSentFile += "\\";
				strSentFile += strFilename.Left(6);
				strSentFile += ".DAT";

				CSSFile fileSent;
				if ( fileSent.Open( strSentFile, "ab" ) == TRUE )
				{
					fileSent.WriteLine( strFilename.Mid(6,2) );
					fileSent.Close();
				}

				CFileRemover FileRemover( strSource );
			}
			else
			{
				m_strExtraErrorInfo = strSource.Right(30);
				nResult = FNB_EXPORT_ERROR_FTP_PUTFILE;
				break;
			}
		}
	}
	else
	{
		m_strExtraErrorInfo = FNBOptions.GetFTPServer();
		nResult = FNB_EXPORT_ERROR_FTP_CONNECT;
	}

	FtpConnect.Disconnect();

	return nResult;
}

/**********************************************************************/
