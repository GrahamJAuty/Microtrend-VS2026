#pragma once
//*******************************************************************
#define THREADLOG_REQIN 0
#define THREADLOG_REQOUT 1
#define THREADLOG_RSPIN 2
#define THREADLOG_RSPOUT 3
#define THREADLOG_LOGIN 4
#define THREADLOG_LOGOUT 5
#define THREADLOG_REQTIDYIN 6
#define THREADLOG_REQTIDYOUT 7
//*******************************************************************

class CMessageLogger
{
public:
	CMessageLogger(void);
	~CMessageLogger(void);

public:
	void LogGetPurse2BalanceMessage( CString strMessage );
	void LogWinServiceMessage( CString strMessage );
	void LogSystemMessage( CString strMessage );
	void LogThreadDiagnostic(int nType, int nFolderSet);

private:
	void LogMessage( CString strContext, CString strMessage );

private:
	void CheckMaxLogFileSize( CString& strNew, CString& strOld );
};

//*******************************************************************
extern CMessageLogger MessageLogger;
//*******************************************************************
