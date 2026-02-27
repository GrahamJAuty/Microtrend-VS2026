#pragma once
//*******************************************************************

class CMessageLogger
{
public:
	CMessageLogger(void);
	~CMessageLogger(void);

public:
	void LogImportDatabaseMessage( CString strMessage );
	void LogWinServiceMessage( CString strMessage );
	void LogSystemMessage( CString strMessage );
	void LogParentMailMessage( CString strMessage );
	void LogParentPayMessage( CString strMessage );
	void LogSchoolCommsMessage( CString strMessage );
	void LogSQuidMessage( CString strMessage );
	void LogTucasiJSONMessage( CString strMessage );
	void LogTucasiSOAPMessage( CString strMessage );
	void LogWisePayMessage( CString strMessage );
	void LogBromComMessage(CString strMessage);
	void LogPebbleMessage(CString strMessage);

private:
	void LogMessage( CString strContext, CString strMessage );

private:
	void CheckMaxLogFileSize( CString& strNew, CString& strOld );
};

//*******************************************************************
extern CMessageLogger MessageLogger;
//*******************************************************************
