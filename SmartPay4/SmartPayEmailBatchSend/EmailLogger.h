#pragma once
/**********************************************************************/

struct CEmailLoggerInfo
{
public:
	CEmailLoggerInfo();

public:
	CString m_strFilename;
	CString m_strEmail;
	CString m_strSubject;
	CString m_strResult;
	CString m_strDate;
	CString m_strTime;
	int m_nHideLevel;
};

/**********************************************************************/

class CEmailLogger  
{
public:
	CEmailLogger();
	void Initialise();

public:
	void LogEmailSystem( CEmailLoggerInfo& info );
	const char* GetLastEmailMessage(){ return m_strLastEmailMessage; }
	
public:
	static void CheckMaxLogSysFileSize( CString& strNew, CString& strOld );

public:
	const char* GetEmailLogLine( CEmailLoggerInfo& info );

private:
	CString m_strLastEmailMessage;
	CString m_strEmailLogLine;
		
private:
	CSSLogFile m_LogFileEmail;
};

/**********************************************************************/
extern CEmailLogger EmailLogger;
/**********************************************************************/
