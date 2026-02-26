/**********************************************************************/
#include "EmailOptions.h"
#include "FileRemover.h"
#include "MessageLogger.h"
#include "PosTrayTokenFiles.h"
/**********************************************************************/
#include "PosTrayEmailSender.h"
/**********************************************************************/

CPosTrayEmailSender::CPosTrayEmailSender()
{
	m_bRetryEmails = FALSE;
	m_bLogRetryErrors = FALSE;
	m_bSMTP = FALSE;
}

/**********************************************************************/

void CPosTrayEmailSender::LookForReports( bool bSMTP )
{
	CDataManagerInfo info;
	DataManagerNonDb.OpenEmailAddress( DB_READONLY, info );

	m_bSMTP = bSMTP;
	m_bRetryEmails = CheckRetryToken();
	m_bLogRetryErrors = m_bRetryEmails;
	m_bRetryEmails |= m_bSMTP;
	
	CString strSourceFolder = "PosTray\\Email\\";
	::GetSyssetProgramPath( strSourceFolder );

	if ( TRUE == m_bSMTP )
	{
		CreateSubdirectory( strSourceFolder );
		CreateSubdirectory( strSourceFolder + "\\External" );
	}

	CString strExPrefix = "XXX_";
	
	CStringArray arrayFoldersToDelete;

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
		
		if ( strSubFolderName == "EXTERNAL" )
		{
			if ( TRUE == m_bSMTP )
			{
				ProcessExternalEmails( strSubFolderPath, strSourceFolder + "XXX_EXTERNAL" );
			}
		}
		else if ( strSubFolderName == "XXX_EXTERNAL" )
		{
			if ( ( TRUE == m_bSMTP ) && ( TRUE == m_bRetryEmails ) )
			{
				if ( ProcessExternalExceptions( strSubFolderPath, strSourceFolder + "EXTERNAL" ) == TRUE )
				{
					arrayFoldersToDelete.Add( strSubFolderName );
				}
			}
		}
		else if ( strSubFolderName.Left(4) != strExPrefix )
		{
			CString strExPath = strSourceFolder + strExPrefix + strSubFolderName;
			
			if ( ProcessInternalEmails( strSubFolderPath, strExPath ) == TRUE )
			{
				//arrayFoldersToDelete.Add( strSubFolderName );
			}
		}
		else
		{
			if ( TRUE == m_bRetryEmails )
			{
				if ( ProcessInternalExceptions( strSubFolderPath ) == TRUE )
				{
					//arrayFoldersToDelete.Add( strSubFolderName );
				}
			}
		}
	}

	DeleteFolders( strSourceFolder, arrayFoldersToDelete );
	DataManagerNonDb.CloseEmailAddress( info );
}

/**********************************************************************/

void CPosTrayEmailSender::DeleteFolders( CString& strRoot, CStringArray& array )
{
	for ( int n = 0; n < array.GetSize(); n++ )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFolder ( strRoot + array.GetAt(n) );
	}
}

/**********************************************************************/

void CPosTrayEmailSender::DeleteFiles( CString& strRoot, CStringArray& array )
{
	for ( int n = 0; n < array.GetSize(); n++ )
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile ( strRoot + array.GetAt(n) );
	}
}

/**********************************************************************/

bool CPosTrayEmailSender::ProcessInternalEmails( const char* szSourceFolder, const char* szExFolder )
{
	CStringArray arrayFilesToDelete;
	CStringArray arrayFoldersToDelete;

	CString strSourceFolder = szSourceFolder;
	strSourceFolder += "\\";

	CString strExFolder = szExFolder;
	strExFolder += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourceFolder + "*.*" ) != 0 );

	bool bCanDeleteFolder = FALSE;
	bool bFirstLoopPass = TRUE;

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		if ( TRUE == bFirstLoopPass )
		{
			bCanDeleteFolder = bWorking;
			bFirstLoopPass = FALSE;
		}

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
					MoveToExceptionsFolder( FALSE, strSourceFolder, strExFolder, strFileName, strFileTitle, arrayFilesToDelete, bCanDeleteFolder );
				}
			}
			else
			{
				if  ( ( strFileExt != ".NKN" ) && ( strFileExt != ".OK" ) && ( strFileExt != ".ERR" ) ) 
				{
					LogFileNameError( TRUE, strFileName, arrayFilesToDelete );
				}
				else if ( strFileExt != ".NKN" )
				{
					arrayFilesToDelete.Add( strFileName );
				}
			}
		}
		else
		{
			bool bProcessSubFolder = FALSE;
			bool bExceptionsSubFolder = FALSE;
			
			CString strExPrefix = "XXX_";
			CString strSubFolderName = FileFinder.GetFileName().MakeUpper();
			CString strSubFolderFullPath = strSourceFolder + strSubFolderName;
			CString strSubFolderExPath = strSourceFolder + strExPrefix + strSubFolderName;
			
			if ( strSubFolderName.GetLength() > 3 )
			{
				if ( strSubFolderName.Left(4) == "XXX_" )
				{
					bProcessSubFolder = m_bRetryEmails;
					bExceptionsSubFolder = TRUE;
				}
				else
				{
					bProcessSubFolder = TRUE;
				}
			}

			if ( TRUE == bProcessSubFolder )
			{	
				bool bClearedFolder = FALSE;
				
				if ( TRUE == bExceptionsSubFolder )
				{
					bClearedFolder = ProcessInternalExceptions( strSubFolderFullPath);
				}
				else
				{
					bClearedFolder = ProcessInternalEmails( strSubFolderFullPath, strSubFolderExPath );
				}

				if ( TRUE == bClearedFolder )
				{
					arrayFoldersToDelete.Add( strSubFolderName );
				}
				else
				{
					bCanDeleteFolder = FALSE;
				}
			}
		}
	}	

	DeleteFiles( strSourceFolder, arrayFilesToDelete );
	DeleteFolders( strSourceFolder, arrayFoldersToDelete );

	return bCanDeleteFolder;
}

/**********************************************************************/

bool CPosTrayEmailSender::ProcessInternalExceptions( const char* szSourceFolder )
{
	CStringArray arrayFilesToDelete;

	CString strSourceFolder = szSourceFolder;
	strSourceFolder += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourceFolder + "*.*" ) != 0 );

	bool bCanDeleteFolder = FALSE;
	bool bFirstLoopPass = TRUE;
	
	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		if ( TRUE == bFirstLoopPass )
		{
			bCanDeleteFolder = bWorking;
			bFirstLoopPass = FALSE;
		}

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
				bCanDeleteFolder = FALSE;
			}
		}
		else
		{
			if  ( ( strFileExt != ".NKN" ) && ( strFileExt != ".OK" ) && ( strFileExt != ".ERR" ) )
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
	return bCanDeleteFolder;
}

/**********************************************************************/

void CPosTrayEmailSender::ProcessExternalEmails( const char* szSourceFolder, const char* szExFolder )
{
	CStringArray arrayFilesToDelete;
	CStringArray arrayFoldersToDelete;

	CString strSourceFolder = szSourceFolder;
	strSourceFolder += "\\";

	CString strExFolder = szExFolder;
	strExFolder += "\\";

	CreateSubdirectory( strSourceFolder + "BodyText" );

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
			if ( ValidateExternalFileName( FileFinder, strFileName, strFileTitle, strFileExt ) == TRUE )
			{
				CString strInfo = "";
				CString strSubject = "";
				CString strErrorText = "";		
				int nResult = EmailExternalFile( strSourceFolder, strFileTitle, strFileExt, strSourceFolder, strInfo, strSubject, strErrorText );

				CString strErrorLogText = "";
				GetErrorLogText( nResult, strErrorText, strErrorLogText );

				if ( EMAIL_ERROR_NONE == nResult )
				{
					LogSendOK( strFileName, strFileTitle, strInfo, strSubject, arrayFilesToDelete );
				}
				else
				{
					bool bDummy;
					LogSendFail( TRUE, nResult, strSourceFolder, strFileName, strFileTitle, strInfo, strSubject, strErrorLogText );
					MoveToExceptionsFolder( TRUE, strSourceFolder, strExFolder, strFileName, strFileTitle, arrayFilesToDelete, bDummy );
				}
			}
			else
			{
				if ( ( strFileName != "TEMPLATE.NKN" ) && ( strFileExt != ".OK" ) && ( strFileExt != ".ERR" ) )
				{
					LogFileNameError( TRUE, strFileName, arrayFilesToDelete );
				}
				else if ( strFileName != "TEMPLATE.NKN" )
				{
					arrayFilesToDelete.Add( strFileName );
				}
			}
		}
		else
		{
			bool bProcessSubFolder = FALSE;
			bool bExceptionsSubFolder = FALSE;
			
			CString strExPrefix = "XXX_";
			CString strSubFolderName = FileFinder.GetFileName().MakeUpper();
			CString strSubFolderFullPath = strSourceFolder + strSubFolderName;
			CString strSubFolderExPath = strSourceFolder + strExPrefix + strSubFolderName;
			CString strNKNFolderFullPath = "";
			
			if ( ( strSubFolderName.GetLength() > 3 ) && ( strSubFolderName != "BODYTEXT" ) )
			{
				if ( strSubFolderName.Left(4) == "XXX_" )
				{
					bProcessSubFolder = m_bRetryEmails;
					bExceptionsSubFolder = TRUE;
					strNKNFolderFullPath = strSourceFolder + strSubFolderName.Right( strSubFolderName.GetLength() - 4 ); 
				}
				else
				{
					bProcessSubFolder = TRUE;
				}
			}

			if ( TRUE == bProcessSubFolder )
			{			
				if ( TRUE == bExceptionsSubFolder )
				{
					if ( ProcessExternalExceptions( strSubFolderFullPath, strNKNFolderFullPath ) == TRUE )
					{
						arrayFoldersToDelete.Add( strSubFolderName );
					}
				}
				else
				{
					ProcessExternalEmails( strSubFolderFullPath, strSubFolderExPath );
				}
			}
		}
	}	

	DeleteFiles( strSourceFolder, arrayFilesToDelete );
	DeleteFolders( strSourceFolder, arrayFoldersToDelete );
}

/**********************************************************************/

bool CPosTrayEmailSender::ProcessExternalExceptions( const char* szSourceFolder, const char* szNKNSourceFolder )
{
	CStringArray arrayFilesToDelete;

	CString strSourceFolder = szSourceFolder;
	strSourceFolder += "\\";

	CString strNKNSourceFolder = szNKNSourceFolder;
	strNKNSourceFolder += "\\";

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourceFolder + "*.*" ) != 0 );

	bool bCanDeleteFolder = FALSE;
	bool bFirstLoopPass = TRUE;
	
	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		if ( TRUE == bFirstLoopPass )
		{
			bCanDeleteFolder = bWorking;
			bFirstLoopPass = FALSE;
		}

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//NO SUBDIRECTORIES FOR EXCEPTIONS
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFileName, strFileTitle, strFileExt;
		if ( ValidateExternalFileName( FileFinder, strFileName, strFileTitle, strFileExt ) == TRUE )
		{
			CString strInfo = "";
			CString strSubject = "";
			CString strErrorText = "";		
			int nResult = EmailExternalFile( strSourceFolder, strFileTitle, strFileExt, strNKNSourceFolder, strInfo, strSubject, strErrorText );

			CString strErrorLogText = "";
			GetErrorLogText( nResult, strErrorText, strErrorLogText ); 

			if ( EMAIL_ERROR_NONE == nResult )
			{
				LogSendOK( strFileName, strFileTitle, strInfo, strSubject, arrayFilesToDelete );
			}
			else
			{
				LogSendFail( m_bLogRetryErrors, nResult, strSourceFolder, strFileName, strFileTitle, strInfo, strSubject, strErrorLogText );
				bCanDeleteFolder = FALSE;
			}
		}
		else if ( ( strFileExt != ".OK" ) && ( strFileExt != ".ERR" ) )
		{
			LogFileNameError( m_bLogRetryErrors, strFileName, arrayFilesToDelete );
		}
		else
		{
			arrayFilesToDelete.Add( strFileName );
		}
	}	

	DeleteFiles( strSourceFolder, arrayFilesToDelete );
	return bCanDeleteFolder;
}

/**********************************************************************/

void CPosTrayEmailSender::GetFileNameParts( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt )
{
	strFileName = FileFinder.GetFileName();
	strFileTitle = FileFinder.GetFileTitle();
	strFileExt = strFileName.Right( strFileName.GetLength() - strFileTitle.GetLength() );
	strFileName.MakeUpper();
	strFileTitle.MakeUpper();
	strFileExt.MakeUpper();
}

/**********************************************************************/

bool CPosTrayEmailSender::ValidateInternalFileName( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt )
{
	GetFileNameParts( FileFinder, strFileName, strFileTitle, strFileExt );

	if ( strFileTitle.GetLength() < 19 )
	{
		return FALSE;
	}

	if ( ( strFileExt != ".CSV" ) && ( strFileExt != ".HTM" ) )
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

bool CPosTrayEmailSender::ValidateExternalFileName( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt )
{
	GetFileNameParts( FileFinder, strFileName, strFileTitle, strFileExt );
	
	if ( strFileTitle.GetLength() < 1 )
	{
		return FALSE;
	}

	if ( ( strFileExt == "" ) || ( strFileExt == ".NKN" ) || ( strFileExt == ".OK" ) || ( strFileExt == ".ERR" ) )
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CPosTrayEmailSender::GetErrorLogText( int nResult, CString& strErrorText, CString& strErrorLogText )
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

void CPosTrayEmailSender::LogFileNameError( bool bLogError, CString& strFileName, CStringArray& arrayFilesToDelete )
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

void CPosTrayEmailSender::LogSendOK( CString& strFileName, CString& strFileTitle, CString& strInfo, CString& strSubject, CStringArray& arrayFilesToDelete  )
{
	LogEmailJobSystem( strFileName, strInfo, strSubject, "OK" );
	arrayFilesToDelete.Add( strFileName );
	arrayFilesToDelete.Add( strFileTitle + ".NKN" );
	arrayFilesToDelete.Add( strFileTitle + ".OK" );
}

/**********************************************************************/

void CPosTrayEmailSender::LogSendFail( bool bLogError, int nResult, CString& strSourceFolder, CString& strFileName, CString& strFileTitle, CString& strInfo, CString& strSubject, CString& strErrorLog )
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

void CPosTrayEmailSender::MoveToExceptionsFolder( bool bExternalFiles, CString& strSourceFolder, CString& strExFolder, CString& strFileName, CString& strFileTitle, CStringArray& arrayFilesToDelete, bool& bCanDeleteFolder )
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

	if ( TRUE == bCopyFail )
	{
		bCanDeleteFolder = FALSE;
	}
	else
	{
		for ( int n = 0; n < arrayFilesToCopy.GetSize(); n++ )
		{
			arrayFilesToDelete.Add( arrayFilesToCopy.GetAt(n) );
		}
	}
}

/**********************************************************************/

int CPosTrayEmailSender::EmailPosTrayReport( CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strInfo, CString& strSubject, CString& strErrorText )
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

	if ( FALSE == m_bSMTP )
	{
		char cBuffer [ _MAX_PATH ];
		cBuffer[0] = 0;
		strcat_s ( cBuffer, strSourceFolder + strFileTitle + strFileExt );
		return EmailMAPI( arrayEmail, strSubject, cBuffer, bHTML, strErrorText );
	}
	else
	{
		CString strAttach = (bHTML) ? "0" : "1";
		CString strBodyTextPath = "";
		return EmailSMTP( arrayEmail, strSubject, strSourceFolder, strFileTitle, strFileExt, strAttach, strBodyTextPath, strErrorText );
	}
}

/**********************************************************************/

int CPosTrayEmailSender::GetExternalNKNDetails( bool bWantRule, const char* szFileTitle, const char* szFileExt, CSSFile& fileInfo, CString& strNickname, CString& strSubject, CString& strBodyTextFile )
{
	bool bMatchRule = TRUE;

	CString strBuffer = "";
	while ( strBuffer == "" )
	{
		if ( fileInfo.ReadString( strBuffer ) == FALSE )
		{
			return EMAIL_ERROR_INFOFILE_NICKNAME;
		}
	}

	if ( TRUE == bWantRule )
	{
		CString strFileTitle = szFileTitle;
		CString strFileName = strFileTitle + szFileExt;
		strFileTitle.MakeUpper();
		strFileName.MakeUpper();

		CCSV csv( strBuffer );
		CString strRule = csv.GetString(0);
		CString strMatch = csv.GetString(1);
		strRule.MakeUpper();
		strMatch.MakeUpper();

		if ( strRule == "FILENAME=" )
		{
			bMatchRule = ( strFileName == strMatch );
		}
		else if ( strRule == "FILENAME" )
		{
			bMatchRule = ( strFileName.Find( strMatch ) >= 0 );
		}
		else if ( strRule == "FILETITLE=" )
		{
			bMatchRule = ( strFileTitle == strMatch );
		}
		else if ( strRule == "FILETITLE" )
		{
			bMatchRule = ( strFileTitle.Find( strMatch ) >= 0 );
		}
		else
		{
			bMatchRule = FALSE;
		}

		if ( fileInfo.ReadString( strBuffer ) == FALSE )
		{
			return EMAIL_ERROR_INFOFILE_NICKNAME;
		}
	}

	strNickname = strBuffer;

	{
		if ( fileInfo.ReadString( strBuffer ) == FALSE )
			return EMAIL_ERROR_INFOFILE_SUBJECT;

		CCSV csv( strBuffer );
		strSubject = csv.GetString(0);
		strBodyTextFile = csv.GetString(1);
	}

	return ( bMatchRule ) ? EMAIL_ERROR_NONE : EMAIL_ERROR_RULE;
}

/**********************************************************************/

int CPosTrayEmailSender::EmailExternalFile( CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strNKNTemplateFolder, CString& strInfo, CString& strSubject, CString& strErrorText )
{
	CString strEmailFilename = strFileTitle + strFileExt;
	strEmailFilename.MakeUpper();
	
	CString strNKNTemplateFile = strNKNTemplateFolder + "TEMPLATE.NKN"; 
	if ( ::FileExists( strNKNTemplateFile ) == FALSE )
		return EMAIL_ERROR_INFOFILE_FIND;

	CSSFile fileInfo;
	if ( fileInfo.Open( strNKNTemplateFile, "rb" ) == FALSE )
		return EMAIL_ERROR_INFOFILE_READ;

	CString strNickname = "";
	CString strBodyTextFile = "";
	
	{
		int nResult = GetExternalNKNDetails( FALSE, strFileTitle, strFileExt, fileInfo, strNickname, strSubject, strBodyTextFile );
		
		if ( nResult != EMAIL_ERROR_NONE )
		{
			fileInfo.Close();
			return nResult;
		}
	}

	do
	{
		CString strNicknameRule;
		CString strSubjectRule;
		CString strBodyTextFileRule;
		
		int nResult = GetExternalNKNDetails( TRUE, strFileTitle, strFileExt, fileInfo, strNicknameRule, strSubjectRule, strBodyTextFileRule );

		if ( EMAIL_ERROR_NONE == nResult )
		{
			strNickname = strNicknameRule;
			strSubject = strSubjectRule;
			strBodyTextFile = strBodyTextFileRule;
		}

		if ( nResult != EMAIL_ERROR_RULE )
		{
			break;
		}
	}
	while (TRUE);

	fileInfo.Close();

	CStringArray arrayEmail;
	int nResult = GetEmailAddresses( strNickname, arrayEmail, strInfo );
	if ( 0 != nResult )
		return nResult;

	if ( strSubject.Left(1) == "#" )
	{
		strSubject = strSubject.Right( strSubject.GetLength() - 1 );
	}
	else
	{
		CString strExtra;
		strExtra.Format( " (%s%s)", (const char*) strFileTitle, (const char*) strFileExt ); 
		strSubject += strExtra;
	}
	
	strInfo = "";
	if ( arrayEmail.GetSize() > 0 )
	{
		strInfo = arrayEmail.GetAt(0);
	}

	CString strBodyTextPath = "";

	if ( strBodyTextFile != "" )
	{
		strBodyTextPath += strNKNTemplateFolder;
		strBodyTextPath += "BODYTEXT\\";
		strBodyTextPath += strBodyTextFile;
	}

	CString strAttach = "1";
	return EmailSMTP( arrayEmail, strSubject, strSourceFolder, strFileTitle, strFileExt, strAttach, strBodyTextPath, strErrorText );
}

/**********************************************************************/

int CPosTrayEmailSender::EmailMAPI( CStringArray& arrayEmail, CString& strSubject, char* cBuffer, bool bHTML, CString& strErrorText  )
{
	int nReturnCode = EMAIL_ERROR_NONE;

	bool bResult = FALSE;
	CSSMapi32 EmailHandler;
		
	if ( FALSE == bHTML )
		bResult = EmailHandler.SendAttachment( arrayEmail, strSubject, cBuffer, TRUE );
	else	
		bResult = EmailHandler.SendMessage( arrayEmail, strSubject, cBuffer, TRUE );
			
	if ( FALSE == bResult )
	{
		strErrorText = EmailHandler.GetError();
		nReturnCode = EMAIL_ERROR_MAPI;
	}

	return nReturnCode;
}

/**********************************************************************/

int CPosTrayEmailSender::EmailSMTP( CStringArray& arrayEmail, CString& strSubject, CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strAttach, CString& strBodyTextPath, CString& strErrorText )
{
	int nReturnCode = EMAIL_ERROR_NONE;

	{
		CCSV csvEmailAddresses;
		for ( int n = 0; n < arrayEmail.GetSize(); n++ )
		{
			CString strAddress = arrayEmail.GetAt(n);
			if (strAddress.Left(4) == "CWT_")
			{
				strAddress = strAddress.Mid(4);
			}

			csvEmailAddresses.Add( strAddress );
		}

		CEmailOptions EmailOptionsLocal;
		EmailOptionsLocal.Read();
			
		CString strServer = EmailOptionsLocal.GetEmailSMTPServerToUse();
		CString strUsername = EmailOptionsLocal.GetEmailSMTPUsernameToUse();
		CString strPassword = EmailOptionsLocal.GetEmailSMTPPasswordToUse();
			
		CString strFrom = EmailOptionsLocal.GetEmailSMTPFromAddressToUse();
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
				EmailOptionsLocal.GetEmailSMTPPortToUse(),
				EmailOptionsLocal.GetEmailSMTPUseSSLFlagToUse(),
				strSubject,
				strBodyTextPath,
				strSourceFolder + strFileTitle,
				strDot + strExt,
				strAttach == "1",
				strFrom,
				EmailOptionsLocal.GetEmailSMTPReplyToToUse(),
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

const char* CPosTrayEmailSender::GetEmailErrorText( int nError )
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

void CPosTrayEmailSender::LogEmailJobSystem( const char* szFilename, const char* szEmail, const char* szSubject, const char* szResult )
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

	MessageLogger.LogEmailSystem( info );
}

/**********************************************************************/

void CPosTrayEmailSender::LogEmailJobSystem( const char* szExceptionText )
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
	
	MessageLogger.LogEmailSystem( info );
}

/**********************************************************************/

int CPosTrayEmailSender::GetEmailAddresses(CString& strNickname, CStringArray& arrayEmail, CString& strInfo)
{
	strInfo = "";
	arrayEmail.RemoveAll();

	if (strNickname.Find("@", 0) != -1)
	{
		arrayEmail.Add(strNickname);
		return 0;
	}

	bool bFound = FALSE;
	CReportConsolidationArray<CSortedStringItem> arraySortedEmail;

	{
		CReportConsolidationArray<CSortedStringItem> arrayUsedNicknames;
		CReportConsolidationArray<CSortedStringItem> arrayNewNicknames;

		CSortedStringItem itemNN;
		itemNN.m_strItem = strNickname;
		arrayNewNicknames.Consolidate(itemNN);
		arrayUsedNicknames.Consolidate(itemNN);

		while (arrayNewNicknames.GetSize() > 0)
		{
			ProcessNickname(arrayNewNicknames, arrayUsedNicknames, arraySortedEmail, bFound);
		}
	}

	for (int n = 0; n < arraySortedEmail.GetSize(); n++)
	{
		CSortedStringItem itemNN;
		arraySortedEmail.GetAt(n, itemNN);
		arrayEmail.Add(itemNN.m_strItem);
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

void CPosTrayEmailSender::ProcessNickname(
	CReportConsolidationArray<CSortedStringItem>& arrayNewNicknames,
	CReportConsolidationArray<CSortedStringItem>& arrayUsedNicknames,
	CReportConsolidationArray<CSortedStringItem>& arraySortedEmail,
	bool& bFound)
{
	if (arrayNewNicknames.GetSize() == 0)
	{
		return;
	}

	CSortedStringItem itemNN;
	arrayNewNicknames.GetAt(0, itemNN);
	arrayNewNicknames.RemoveAt(0);

	for (int n = 0; n < DataManagerNonDb.EmailAddress.GetSize(); n++)
	{
		CString strArrayNickname = DataManagerNonDb.EmailAddress.GetNickname(n);

		if (strArrayNickname == itemNN.m_strItem)
		{
			bFound = TRUE;

			for (int e = 0; e < DataManagerNonDb.EmailAddress.GetEmailAddressCount(n); e++)
			{
				CString strAddress = DataManagerNonDb.EmailAddress.GetEmailAddress(n, e);

				if (strAddress.Find("@", 0) != -1)
				{
					CSortedStringItem itemEmail;
					itemEmail.m_strItem = strAddress;
					arraySortedEmail.Consolidate(itemEmail);
				}
				else
				{
					CSortedStringItem itemNew;
					itemNew.m_strItem = strAddress;

					int nPos = 0;
					if (arrayUsedNicknames.Find(itemNew, nPos) == FALSE)
					{
						arrayUsedNicknames.InsertAt(nPos, itemNew);
						arrayNewNicknames.Consolidate(itemNew);
					}
				}
			}

			break;
		}
	}
	
}

/**********************************************************************/

bool CPosTrayEmailSender::CheckRetryToken()
{
	if ( CPosTrayTokenFiles::RetryEmailFileExists() == FALSE )
		return FALSE;

	CPosTrayTokenFiles::DeleteRetryEmailFile();

	if ( CPosTrayTokenFiles::RetryEmailFileExists() == TRUE )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
