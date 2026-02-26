/**********************************************************************/
 
/**********************************************************************/
#include "FileRemover.h"
#include "FTPConnect.h"
#include "MessageLogger.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayFTPSender.h"
/**********************************************************************/

CPosTrayFTPSender::CPosTrayFTPSender(void)
{
}

/**********************************************************************/

CPosTrayFTPSender::~CPosTrayFTPSender(void)
{
}

/**********************************************************************/

void CPosTrayFTPSender::SendExports( CPosTrayTask& Task )
{
	CStringArray arrayFilenames;
	GetFileList( Task, arrayFilenames );
		
	m_strExtraErrorInfo = "";
	int nResult = FTPTransfer( Task, arrayFilenames );
	
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
		MessageLogger.LogFTPSystem( info );
	}
}

/**********************************************************************/

void CPosTrayFTPSender::GetFileList( CPosTrayTask& Task, CStringArray& arrayFilenames )
{
	CString strPath = Task.GetLiveTaskFolder();
	strPath += "\\FTP\\";

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strPath + "*.csv" );

	while ( TRUE == bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
			
		if ( strFilename.GetLength() >= 5 )
			arrayFilenames.Add( strFilename );
	}
}

/**********************************************************************/

int CPosTrayFTPSender::FTPTransfer( CPosTrayTask& Task, CStringArray& arrayFilenames )
{
	int nResult = FNB_EXPORT_ERROR_NONE;

	if ( arrayFilenames.GetSize() == 0 )
		return nResult;

	CString strFolder = "";
	strFolder += Task.GetLiveTaskFolder();
	strFolder += "\\FTP\\";

	CFtpConnect FtpConnect;
	if ( FtpConnect.ConnectPosTrayExport() == TRUE )
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
			info.m_strMessage += PosTrayOptions.GetFTPServer();
			info.m_strStatus = "OK";
			MessageLogger.LogFTPSystem( info );
		}

		{
			CString strDest = Task.GetTaskFTPFolder();

			if ( strDest != "" )
				FtpConnect.CreateDirectory( strDest );
		}

		for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
		{
			CString strFilename = arrayFilenames.GetAt(n);

			CString strSource = "";
			strSource += strFolder;
			strSource += strFilename;

			CString strDest = Task.GetTaskFTPFolder();
			
			if ( strDest != "" )
			{
				if ( PosTrayOptions.GetFTPUnixModeFlag() == TRUE )
				{
					if ( strDest.Right(1) != "/" )
						strDest += "/";
				}
				else
				{
					if ( strDest.Right(1) != "\\" )
						strDest += "\\";
				}
			}
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
					info.m_strMessage = "Source File : ";
					info.m_strMessage += strFilename;
					info.m_strStatus = "OK";
					MessageLogger.LogFTPSystem( info );

					info.m_strMessage = "Dest Folder : ";
					info.m_strMessage += Task.GetTaskFTPFolder();
					info.m_strStatus = "OK";
					MessageLogger.LogFTPSystem( info );
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
		m_strExtraErrorInfo = PosTrayOptions.GetFTPServer();
		nResult = FNB_EXPORT_ERROR_FTP_CONNECT;
	}

	FtpConnect.Disconnect();

	return nResult;
}

/**********************************************************************/
