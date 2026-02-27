#pragma once
//$$******************************************************************
#define PASSWORD_EVENT_LOGON_SYSTEM 1
#define PASSWORD_EVENT_LOGON_MENU 2
#define PASSWORD_EVENT_LOGOFF_SYSTEM 3
#define PASSWORD_EVENT_LOGOFF_MENU 4
//$$******************************************************************

class CPasswordLogger
{
public:
	CPasswordLogger();
	~CPasswordLogger();

public:
	void LogEvent(int nPasswordNo, int nEventType);

private:
	void LogEventInternal(CString strDateNow, CString strTimeNow, int nPasswordNo, int nEventType);

private:
	bool m_bLogEnabledAtLogon;
	bool m_bPasswordsAtLogon;
	CString m_strLogonDate;
	CString m_strLogonTime;
	int m_nLogonType;

private:
	CSSLogFile* m_pLogFilePassword;
	CString m_strLogFilename;
};

//$$******************************************************************
extern CPasswordLogger PasswordLogger;
//$$******************************************************************
