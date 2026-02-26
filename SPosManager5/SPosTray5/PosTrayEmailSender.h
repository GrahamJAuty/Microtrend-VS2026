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
#ifdef POSTRAY_UTILITY
/**********************************************************************/

class CPosTrayEmailSender  
{
public:
	CPosTrayEmailSender();

public:
	void LookForReports( bool bSMTP );
	bool CheckRetryToken();

private:
	int GetEmailAddresses( CString& strNickname, CStringArray& arrayEmail, CString& strInfo );
		
private:
	bool ProcessInternalEmails( const char* szSourceFolder, const char* szExFolder );
	bool ProcessInternalExceptions( const char* szSourceFolder );
	void ProcessExternalEmails( const char* szSourceFolder, const char* szExFolder );
	bool ProcessExternalExceptions( const char* szSourceFolder, const char* szNKNSourceFolder );

private:
	void MoveToExceptionsFolder( bool bExternalFiles, CString& strSourceFolder, CString& strExFolder, CString& strFileName, CString& strFileTitle, CStringArray& arrayFilesToDelete, bool& bCanDeleteFolder );

private:
	void GetFileNameParts( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt );
	bool ValidateInternalFileName( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt );
	bool ValidateExternalFileName( CFileFind& FileFinder, CString& strFileName, CString& strFileTitle, CString& strFileExt );
	void DeleteFolders( CString& strRoot, CStringArray& array );
	void DeleteFiles( CString& strRoot, CStringArray& array );	
	
private:
	int GetExternalNKNDetails( bool bWantRule, const char* szFileTitle, const char* szFileExt, CSSFile& fileInfo, CString& strNickname, CString& strSubject, CString& strBodyTextFile );
	int EmailPosTrayReport( CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strInfo, CString& strSubject, CString& strErrorText );
	int EmailExternalFile( CString& strSourceFolder, CString& strFileTitle, CString& strFileExt, CString& strNKNTemplateFolder, CString& strInfo, CString& strSubject, CString& strErrorText );

private:
	int EmailMAPI( CStringArray& arrayEmail, CString& strSubject, char* cBuffer, bool bHTML, CString& strErrorText );
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
	void ProcessNickname(
		CReportConsolidationArray<CSortedStringItem>& arrayNewNicknames,
		CReportConsolidationArray<CSortedStringItem>& arrayUsedNicknames,
		CReportConsolidationArray<CSortedStringItem>& arraySortedEmail,
		bool& bFound);

private:
	bool m_bRetryEmails;
	bool m_bLogRetryErrors;
	bool m_bSMTP;
	
private:
	CString m_strEmailErrorText;
};

/**********************************************************************/
#endif
/**********************************************************************/
