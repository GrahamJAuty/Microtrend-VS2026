/**********************************************************************/
 
/**********************************************************************/
#include "FileRemover.h"
#include "FTPConnect.h"
//#include "globalfunctions.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayS4LabourSender.h"
/**********************************************************************/

CPosTrayS4LabourSender::CPosTrayS4LabourSender(void)
{
}

/**********************************************************************/

CPosTrayS4LabourSender::~CPosTrayS4LabourSender(void)
{
}

/**********************************************************************/

void CPosTrayS4LabourSender::SendExports()
{
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		CS4LabourLocationOptions S4LabourOptions;
		CStringArray arrayFilenames;
		GetFileList( nLocIdx, S4LabourOptions, arrayFilenames );
		
		m_strExtraErrorInfo = "";
		int nResult = FTPTransfer( nLocIdx, S4LabourOptions, arrayFilenames );
		
		if ( S4LABOUR_EXPORT_ERROR_NONE != nResult )
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
			case S4LABOUR_EXPORT_ERROR_FTP_CONNECT:
				info.m_bLeadingLineNetwork = TRUE;
				info.m_strMessage = "FTP Connection Error : ";
				break;

			case S4LABOUR_EXPORT_ERROR_FTP_PUTFILE:	
				info.m_strMessage = "FTP Send File Error : ";
				break;

			default:
				info.m_strMessage = "Unknown Error : ";
				break;
			}

			info.m_strMessage += strExtra;
			MessageLogger.LogS4LabourSystem( info );
		}
	}
}

/**********************************************************************/

void CPosTrayS4LabourSender::GetFileList( int nLocIdx, CS4LabourLocationOptions& S4LabourOptions, CStringArray& arrayFilenames )
{
	CString strFolder = "";
	strFolder += dbLocation.GetFolderPathSysset( nLocIdx );
	strFolder += "\\S4LABOUR\\EXPORTS\\";

	CString strFilenameOptions = S4LabourOptions.GetFilename( nLocIdx, FALSE );

	if ( ::FileExists( strFilenameOptions ) == FALSE )
		return;
		
	if ( S4LabourOptions.Read( nLocIdx, FALSE ) == FALSE )
		return;

	if ( S4LabourOptions.GetEnable() == FALSE )
		return;

	CStringArray arrayAlreadySent;

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strFolder + "*.CSV" );

	while ( TRUE == bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
			
		{
			int nLength = strFilename.GetLength();
			if ( nLength < 19 )
				continue;
		
			CString strTest = strFilename.MakeUpper();
			strTest.MakeUpper();

			if ( strTest.Left(5) != "CASH-" )
				continue;

			strTest = strTest.Right(13);
			if ( strTest.Left(1) != "-" )
				continue;

			if ( ::TestNumeric( strTest.Mid(1,8) ) == FALSE )
				continue;

			if ( strTest.Right(4) != ".CSV" )
				continue;
		}

		arrayFilenames.Add( strFilename );
	}
}

/**********************************************************************/

int CPosTrayS4LabourSender::FTPTransfer( int nLocIdx, CS4LabourLocationOptions& S4LabourOptions, CStringArray& arrayFilenames )
{
	int nResult = S4LABOUR_EXPORT_ERROR_NONE;

	if ( arrayFilenames.GetSize() == 0 )
		return nResult;

	CString strFolder = "";
	strFolder += dbLocation.GetFolderPathSysset( nLocIdx );
	strFolder += "\\S4LABOUR\\EXPORTS\\";

	CFtpConnect FtpConnect;
	if ( FtpConnect.ConnectS4LabourExport() == TRUE )
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
			info.m_strMessage += PosTrayOptions.GetS4LabourFTPServer();
			info.m_strStatus = "OK";
			MessageLogger.LogS4LabourSystem( info );
		}

		for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
		{
			CString strFilename = arrayFilenames.GetAt(n);

			CString strSource = "";
			strSource += strFolder;
			strSource += strFilename;

			CString strDest = "";
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
					info.m_strMessage += strFilename;
					info.m_strStatus = "OK";
					MessageLogger.LogS4LabourSystem( info );
				}

				CFileRemover FileRemover( strSource );
			}
			else
			{
				m_strExtraErrorInfo = strFilename;
				nResult = S4LABOUR_EXPORT_ERROR_FTP_PUTFILE;
				break;
			}
		}
	}
	else
	{
		m_strExtraErrorInfo = PosTrayOptions.GetS4LabourFTPServer();
		nResult = S4LABOUR_EXPORT_ERROR_FTP_CONNECT;
	}

	FtpConnect.Disconnect();

	return nResult;
}

/**********************************************************************/
