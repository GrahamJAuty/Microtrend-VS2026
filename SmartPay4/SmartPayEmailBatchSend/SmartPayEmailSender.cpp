/**********************************************************************/
#include "..\SmartPay4ManagerEmail\DefTextEmailAddress.h"
/**********************************************************************/
#include "..\SmartPay4Shared\EmailOptions.h"
#include "..\SmartPay4Shared\FileRemover.h"
/**********************************************************************/
#include "EmailLogger.h"
/**********************************************************************/
#include "SmartPayEmailSender.h"
/**********************************************************************/

CSmartPayEmailSender::CSmartPayEmailSender()
{
	m_bLogRetryErrors = FALSE;
	m_nPendingEmailCount = 0;
	EmailLogger.Initialise();
}

/**********************************************************************/

void CSmartPayEmailSender::LookForReports()
{
	m_nPendingEmailCount = 0;
	m_bLogRetryErrors = CheckLogRetryToken();

	EmailAddresses.Read( Filenames.GetEmailAddressesFilename() );
	
	CString strSourceFolder = "Email\\";
	::GetSyssetProgramPath( strSourceFolder );

	CString strExPrefix = "XXX_";
	
	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourceFolder + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
		{
			continue;
		}
		
		//IGNORE INDIVIDUAL FILES
		if ( FileFinder.IsDirectory() == FALSE )
		{
			continue;
		}

		CString strSubFolderName = FileFinder.GetFileName();
		strSubFolderName.MakeUpper();
	
		CString strSubFolderPath = strSourceFolder + strSubFolderName;
		
		if ( strSubFolderName == "REPORTS" )
		{
			CString strExPath = strSourceFolder + strExPrefix + strSubFolderName;
			ProcessInternalEmails( strSubFolderPath, strExPath );
		}
		else if ( strSubFolderName == "XXX_REPORTS" )
		{
			ProcessInternalExceptions( strSubFolderPath );
		}
	}
}

/**********************************************************************/

void CSmartPayEmailSender::DeleteFiles( CString& strRoot, CStringArray& array )
{
	for ( int n = 0; n < array.GetSize(); n++ )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile ( strRoot + array.GetAt(n) );
	}
}

/**********************************************************************/

void CSmartPayEmailSender::ProcessInternalEmails( const char* szSourceFolder, const char* szExFolder )
{
	CStringArray arrayFilesToDelete;
	CStringArray arrayFoldersToDelete;

	CString strSourceFolder = szSourceFolder;
	strSourceFolder += "\\";

	CString strExFolder = szExFolder;
	strExFolder += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourceFolder + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		if ( FileFinder.IsDirectory() == FALSE )
		{
			CString strFileName, strFileTitle, strFileExt;
			if ( ValidateInternalFileName( FileFinder, strFileName, strFileTitle, strFileExt ) == TRUE )
			{
				CString strInfo = "";
				CString strSubject = "";
				CString strErrorText = "";
				int nResult = EmailPosTrayReport( strSourceFolder, strFileTitle, strFileExt, strInfo, strSubject, strErrorText );

				CString strErrorLogText = "";
				GetErrorLogText( nResult, strErrorText, strErrorLogText );

				if ( EMAIL_ERROR_NONE == nResult )
				{
					LogSendOK( strFileName, strFileTitle, strInfo, strSubject, arrayFilesToDelete );
				}
				else
				{
					LogSendFail( TRUE, nResult, strSourceFolder, strFileName, strFileTitle, strInfo, strSubject, strErrorLogText );
					MoveToExceptionsFolder( FALSE, strSourceFolder, strExFolder, strFileName, strFileTitle, arrayFilesToDelete );
				}
			}
			else
			{
				if ( ( strFileExt != ".NKN" ) && ( strFileExt != ".OK" ) && ( strFileExt != ".ERR" ) )
				{
					LogFileNameError( TRUE, strFileName, arrayFilesToDelete );
				}
				else if ( strFileExt != ".NKN" )
				{
					arrayFilesToDelete.Add( strFileName );
				}
			}
		}
	}	

	DeleteFiles( strSourceFolder, arrayFilesToDelete );
}

/**********************************************************************/

void CSmartPayEmailSender::ProcessInternalExceptions( const char* szSourceFolder )
{
	CStringArray arrayFilesToDelete;

	CString strSourceFolder = szSourceFolder;
	strSourceFolder += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourceFolder + "*.*" ) != 0 );

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//NO SUBDIRECTORIES FOR EXCEPTIONS
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFileName, strFileTitle, strFileExt;
		if ( ValidateInternalFileName( FileFinder, strFileName, strFileTitle, strFileExt ) == TRUE )
		{
			CString strInfo = "";
			CString strSubject = "";
			CString strErrorText = "";
			int nResult = EmailPosTrayReport( strSourceFolder, strFileTitle, strFileExt, strInfo, strSubject, strErrorText );

			CString strErrorLogText = "";
			GetErrorLogText( nResult, strErrorText, strErrorLogText );

			if ( EMAIL_ERROR_NONE == nResult )
			{
				LogSendOK( strFileName, strFileTitle, strInfo, strSubject, arrayFilesToDelete );
			}
			else
			{
				LogSendFail( m_bLogRetryErrors, nResult, strSourceFolder, strFileName, strFileTitle, strInfo, strSubject, strErrorLogText );
				m_nPendingEmailCount++;
			}
		}
		else
		{
			if ( ( strFileExt != ".NKN" ) && ( strFileExt != ".OK" ) && ( strFileExt != ".ERR" ) )
			{
				LogFileNameError( m_bLogRetryErrors, strFileName, arrayFilesToDelete );
			}
			else if ( strFileExt != ".NKN" )
			{
				arrayFilesToDelete.Add( strFileName );
			}
		}
	}	

	DeleteFiles( strSourceFolder, arrayFilesToDelete );
}

/**********************************************************************/

void CSmartPayEmailSender::GetFileNameParts( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt )
{
	strFileName = FileFinder.GetFileName();
	strFileTitle = FileFinder.GetFileTitle();
	strFileExt = strFileName.Right( strFileName.GetLength() - strFileTitle.GetLength() );
	strFileName.MakeUpper();
	strFileTitle.MakeUpper();
	strFileExt.MakeUpper();
}

/**********************************************************************/

bool CSmartPayEmailSender::ValidateInternalFileName( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt )
{
	GetFileNameParts( FileFinder, strFileName, strFileTitle, strFileExt );

	if ( strFileTitle.GetLength() < 5 )
	{
		return FALSE;
	}

	if ( strFileExt != ".HTM" )
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CSmartPayEmailSender::GetErrorLogText( int nResult, CString& strErrorText, CString& strErrorLogText )
{
	strErrorLogText = "";

	switch( nResult )
	{
	case EMAIL_ERROR_MAPI:
	case EMAIL_ERROR_SMTP_UNKNOWN:
		strErrorLogText = "FAILED : ";
		strErrorLogText += strErrorText;
		break;

	case EMAIL_ERROR_NONE:
		break;

	default:
		strErrorLogText = "FAILED : ";
		strErrorLogText += GetEmailErrorText( nResult );
	}
}

/**********************************************************************/

void CSmartPayEmailSender::LogFileNameError( bool bLogError, CString& strFileName, CStringArray& arrayFilesToDelete )
{
	if ( TRUE == bLogError )
	{
		CString strError = "FAILED : ";
		strError += GetEmailErrorText( EMAIL_ERROR_FILENAME );
		LogEmailJobSystem( strFileName, "", "", strError );
	}
	arrayFilesToDelete.Add( strFileName );
}

/**********************************************************************/

void CSmartPayEmailSender::LogSendOK( CString& strFileName, CString& strFileTitle, CString& strInfo, CString& strSubject, CStringArray& arrayFilesToDelete  )
{
	LogEmailJobSystem( strFileName, strInfo, strSubject, "OK" );
	arrayFilesToDelete.Add( strFileName );
	arrayFilesToDelete.Add( strFileTitle + ".NKN" );
	arrayFilesToDelete.Add( strFileTitle + ".OK" );
}

/**********************************************************************/

void CSmartPayEmailSender::LogSendFail( bool bLogError, int nResult, CString& strSourceFolder, CString& strFileName, CString& strFileTitle, CString& strInfo, CString& strSubject, CString& strErrorLog )
{
	if ( TRUE == bLogError )
	{
		LogEmailJobSystem( strFileName, strInfo, strSubject, strErrorLog );
	}

	if ( EMAIL_ERROR_SMTP_EXCEPTION == nResult )
	{
		CString strErrorPath = "";
		strErrorPath += strSourceFolder + strFileTitle;
		strErrorPath += ".ERR";
		
		if ( TRUE == bLogError )
		{
			CSSFile fileError;
			if ( fileError.Open( strErrorPath, "rb" ) == TRUE )
			{
				CString strBuffer;
				while ( fileError.ReadString( strBuffer ) == TRUE )
					LogEmailJobSystem( strBuffer );
				fileError.Close();
			}
		}

		CFileRemover FileRemover( strErrorPath );
	}
}

/**********************************************************************/

void CSmartPayEmailSender::MoveToExceptionsFolder( bool bExternalFiles, CString& strSourceFolder, CString& strExFolder, CString& strFileName, CString& strFileTitle, CStringArray& arrayFilesToDelete )
{
	::CreateSubdirectory( strExFolder );

	CStringArray arrayFilesToCopy;
	
	if ( FALSE == bExternalFiles )
	{
		CString strInfoFile = "";
		strInfoFile += strFileTitle;
		strInfoFile += ".NKN";
		arrayFilesToCopy.Add( strInfoFile );
	}

	arrayFilesToCopy.Add( strFileName );

	bool bCopyFail = FALSE;
	for ( int n = 0; n < arrayFilesToCopy.GetSize(); n++ )
	{
		CString strFileToCopy = arrayFilesToCopy.GetAt(n);

		if ( CopyFile( strSourceFolder + strFileToCopy, strExFolder + strFileToCopy, FALSE ) == FALSE )
		{
			bCopyFail = TRUE;
			break;
		}
	}

	if ( FALSE == bCopyFail )
	{
		for ( int n = 0; n < arrayFilesToCopy.GetSize(); n++ )
		{
			arrayFilesToDelete.Add( arrayFilesToCopy.GetAt(n) );
		}
	}
}

/**********************************************************************/

int CSmartPayEmailSender::EmailPosTrayReport( CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strInfo, CString& strSubject, CString& strErrorText )
{
	CString strEmailFilename = strFileTitle + strFileExt;
	strEmailFilename.MakeUpper();
	
	CString strInfoFilename = strFileTitle;
	strInfoFilename += ".NKN";
	
	if ( ::FileExists( strSourceFolder + strInfoFilename ) == FALSE )
		return EMAIL_ERROR_INFOFILE_FIND;

	CSSFile fileInfo;
	if ( fileInfo.Open( strSourceFolder + strInfoFilename, "rb" ) == FALSE )
		return EMAIL_ERROR_INFOFILE_READ;

	CString strNickname = "";
	if ( fileInfo.ReadString( strNickname ) == FALSE )
		return EMAIL_ERROR_INFOFILE_NICKNAME;

	if ( fileInfo.ReadString( strSubject ) == FALSE )
		return EMAIL_ERROR_INFOFILE_SUBJECT;

	fileInfo.Close();

	CStringArray arrayEmail;
	int nResult = GetEmailAddresses( strNickname, arrayEmail, strInfo );
	if ( 0 != nResult )
		return nResult;

	bool bHTML = ( strFileExt == ".HTM" );

	strInfo = "";
	if ( arrayEmail.GetSize() > 0 )
	{
		strInfo = arrayEmail.GetAt(0);
	}

	CString strAttach = (bHTML) ? "0" : "1";
	CString strBodyTextPath = "";
	return EmailSMTP( arrayEmail, strSubject, strSourceFolder, strFileTitle, strFileExt, strAttach, strBodyTextPath, strErrorText );
}

/**********************************************************************/

int CSmartPayEmailSender::EmailSMTP( CStringArray& arrayEmail, CString& strSubject, CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strAttach, CString& strBodyTextPath, CString& strErrorText )
{
	int nReturnCode = EMAIL_ERROR_NONE;

	{
		CCSV csvEmailAddresses;
		for ( int n = 0; n < arrayEmail.GetSize(); n++ )
		{
			csvEmailAddresses.Add( arrayEmail.GetAt(n) );
		}

		CEmailOptions EmailOptions;
		EmailOptions.Read();
			
		CString strServer = EmailOptions.GetEmailSMTPServerToUse();
		CString strUsername = EmailOptions.GetEmailSMTPUsernameToUse();
		CString strPassword = EmailOptions.GetEmailSMTPPasswordToUse();
				
		CString strFrom = EmailOptions.GetEmailSMTPFromAddressToUse();
		CString strTo = csvEmailAddresses.GetLine();
		
		if ( ( strServer == "" ) || ( strUsername == "" ) || ( strPassword == "" ) || ( strFrom == "" ) )
		{
			nReturnCode = EMAIL_ERROR_SMTP_CONFIG;
		}
		else
		{
			CFileRemover FileRemover( strSourceFolder + strFileTitle + ".ERR" );

			CString strExt = "";
			if ( strFileExt.GetLength() > 0 )
			{
				strExt = strFileExt.Right( strFileExt.GetLength() - 1 );
			}

			strBodyTextPath.MakeUpper();
			if ( strBodyTextPath.Right(5) == "#FILE" )
			{
				strAttach = "0";
				strBodyTextPath = "";
			}

			CString strDot = ".";
			
			CChilkatSendMail SendMail;

			int nResult = SendMail.SendMail(
				strServer,
				strUsername,
				strPassword,
				EmailOptions.GetEmailSMTPPortToUse(),
				EmailOptions.GetEmailSMTPUseSSLFlagToUse(),
				strSubject,
				strBodyTextPath,
				strSourceFolder + strFileTitle,
				strDot + strExt,
				strAttach == "1",
				strFrom,
				EmailOptions.GetEmailSMTPReplyToToUse(),
				strTo,
				FALSE);

			switch( nResult )
			{
			case 4:	
				nReturnCode = EMAIL_ERROR_SMTP_EXCEPTION;	
				break;

			case 0:	
				break;
				
			default:
				strErrorText.Format ( "SMTP Error %d", nResult );
				nReturnCode = EMAIL_ERROR_SMTP_UNKNOWN;
				break;
			}
		}
	}

	return nReturnCode;
}

/**********************************************************************/

const char* CSmartPayEmailSender::GetEmailErrorText( int nError )
{
	m_strEmailErrorText = "";

	switch( nError )
	{
	case EMAIL_ERROR_FILENAME:				m_strEmailErrorText = "Invalid filename";					break;
	case EMAIL_ERROR_INFOFILE_FIND:			m_strEmailErrorText = "Missing info file";					break;
	case EMAIL_ERROR_INFOFILE_READ:			m_strEmailErrorText = "Unable to open info file";			break;
	case EMAIL_ERROR_INFOFILE_NICKNAME:		m_strEmailErrorText = "No email address specified";			break;
	case EMAIL_ERROR_INFOFILE_SUBJECT:		m_strEmailErrorText = "No subject specified";				break;
	case EMAIL_ERROR_NICKNAME_FIND:			m_strEmailErrorText = "Unknown nickname";					break;
	case EMAIL_ERROR_NICKNAME_EMAIL:		m_strEmailErrorText = "No address list for nickname";		break;
	case EMAIL_ERROR_SMTP_CONFIG:			m_strEmailErrorText = "SMTP Options not configured";		break;
	case EMAIL_ERROR_SMTP_EXCEPTION:		m_strEmailErrorText = "SMTP exception";						break;
	default:
		m_strEmailErrorText.Format ( "Unknown Error Code (%d)", nError ); 
	}

	return m_strEmailErrorText;
}

/**********************************************************************/

void CSmartPayEmailSender::LogEmailJobSystem( const char* szFilename, const char* szEmail, const char* szSubject, const char* szResult )
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CEmailLoggerInfo info;

	info.m_strDate.Format( "%2.2d/%2.2d/%4.4d", 
		timeNow.GetDay(),
		timeNow.GetMonth(),
		timeNow.GetYear() );

	info.m_strTime.Format( "%2.2d:%2.2d:%2.2d", 
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond() );

	info.m_strFilename = szFilename;
	info.m_strEmail = szEmail;
	info.m_strSubject = szSubject;
	info.m_strResult = szResult;

	EmailLogger.LogEmailSystem( info );
}

/**********************************************************************/

void CSmartPayEmailSender::LogEmailJobSystem( const char* szExceptionText )
{
	CEmailLoggerInfo info;
	info.m_strDate = "";
	info.m_strTime = "";
	info.m_strEmail = "";
	info.m_strSubject = "";
	info.m_strResult = "";

	info.m_strFilename = "";	
	info.m_strFilename += "<..>";
	info.m_strFilename += szExceptionText;
	
	EmailLogger.LogEmailSystem( info );
}

/**********************************************************************/

int CSmartPayEmailSender::GetEmailAddresses( CString& strNickname, CStringArray& arrayEmail, CString& strInfo )
{
	strInfo = "";
	arrayEmail.RemoveAll();
	
	if ( strNickname.Find( "@", 0 ) != -1 )
	{
		arrayEmail.Add( strNickname );
		return 0;
	}

	bool bFound = FALSE;
	for ( int n = 0; n < EmailAddresses.GetSize(); n++ )
	{
		CString strArrayNickname = EmailAddresses.GetNickname(n);

		if ( strArrayNickname == strNickname )
		{
			bFound = TRUE;
			
			for ( int e = 0; e < EmailAddresses.GetEmailAddressCount(n); e++ )
			{
				CString strAddress = EmailAddresses.GetEmailAddress(n,e);

				if ( strAddress.Find( "@", 0 ) != -1 )
					arrayEmail.Add( strAddress );
			}
			
			break;
		}
	}

	if ( FALSE == bFound )
	{
		strInfo = strNickname;
		return EMAIL_ERROR_NICKNAME_FIND;
	}

	if ( arrayEmail.GetSize() == 0 )
	{
		strInfo = strNickname;
		return EMAIL_ERROR_NICKNAME_EMAIL;
	}

	return 0;
}

/**********************************************************************/

bool CSmartPayEmailSender::CheckLogRetryToken()
{
	if ( ::FileExists( Filenames.GetEmailLogRetryTokenFilename() ) == FALSE )
		return FALSE;

	CFileRemover FileRemover( Filenames.GetEmailLogRetryTokenFilename() );

	if ( ::FileExists( Filenames.GetEmailLogRetryTokenFilename() ) == TRUE )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
