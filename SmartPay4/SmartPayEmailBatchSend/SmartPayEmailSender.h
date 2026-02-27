#pragma once
/**********************************************************************/
#define EMAIL_ERROR_NONE 0
/**********************************************************************/
#define EMAIL_ERROR_FILENAME 1
#define EMAIL_ERROR_INFOFILE_FIND 2
#define EMAIL_ERROR_INFOFILE_READ 3
#define EMAIL_ERROR_INFOFILE_NICKNAME 4
#define EMAIL_ERROR_INFOFILE_SUBJECT 5
#define EMAIL_ERROR_NICKNAME_FIND 6
#define EMAIL_ERROR_NICKNAME_EMAIL 7
#define EMAIL_ERROR_MAPI 8
#define EMAIL_ERROR_RULE 9
/**********************************************************************/
#define EMAIL_ERROR_SMTP_CONFIG 11
#define EMAIL_ERROR_SMTP_EXCEPTION 12
#define EMAIL_ERROR_SMTP_UNKNOWN 14
/**********************************************************************/

class CSmartPayEmailSender  
{
public:
	CSmartPayEmailSender();

public:
	void LookForReports();
	bool CheckLogRetryToken();
	int GetPendingEmailCount(){ return m_nPendingEmailCount; }

private:
	int GetEmailAddresses( CString& strNickname, CStringArray& arrayEmail, CString& strInfo );
		
private:
	void ProcessInternalEmails( const char* szSourceFolder, const char* szExFolder );
	void ProcessInternalExceptions( const char* szSourceFolder );
	
private:
	void MoveToExceptionsFolder( bool bExternalFiles, CString& strSourceFolder, CString& strExFolder, CString& strFileName, CString& strFileTitle, CStringArray& arrayFilesToDelete );

private:
	void GetFileNameParts( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt );
	bool ValidateInternalFileName( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt );
	void DeleteFiles( CString& strRoot, CStringArray& array );	
	
private:
	int EmailPosTrayReport( CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strInfo, CString& strSubject, CString& strErrorText );
	
private:
	int EmailSMTP( CStringArray& arrayEmail, CString& strSubject, CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strAttach, CString& strBodyTextPath, CString& strErrorText );

private:
	void GetErrorLogText( int nResult, CString& strErrorText, CString& strErrorLogText );
	void LogFileNameError( bool bLogError, CString& strFileName, CStringArray& arrayFilesToDelete );
	void LogSendOK( CString& strFileName, CString& strFileTitle, CString& strInfo, CString& strSubject, CStringArray& arrayFilesToDelete  );
	void LogSendFail( bool bLogError, int nResult, CString& strSourceFolder, CString& strFileName, CString& strFileTitle, CString& strInfo, CString& strSubject, CString& strErrorLog );
	void LogEmailJobSystem( const char* szFilename, const char* szEmail, const char* szSubject, const char* szResult );
	void LogEmailJobSystem( const char* szExceptionText );
	const char* GetEmailErrorText( int nError );

private:
	bool m_bLogRetryErrors;
	int m_nPendingEmailCount;
	
private:
	CString m_strEmailErrorText;
};

/**********************************************************************/
