//$$******************************************************************
#include "..\SPosLoyalty4Manager\PasswordData.h"
//$$******************************************************************
#include "PasswordLogger.h"
//$$******************************************************************

CPasswordLogger::CPasswordLogger()
{
	m_bLogEnabledAtLogon = FALSE;
	m_strLogonDate = "";
	m_strLogonTime = "";
	m_pLogFilePassword = NULL;
	m_strLogFilename = "";
}

//$$******************************************************************

CPasswordLogger::~CPasswordLogger()
{
	if (m_pLogFilePassword != NULL)
	{
		delete (m_pLogFilePassword);
	}
}

//$$******************************************************************

void CPasswordLogger::LogEvent(int nPasswordNo, int nEventType)
{
	if ((nPasswordNo < 0) || (nPasswordNo > 20))
	{
		return;
	}

	if ((nEventType < 1) || (nEventType > 6))
	{
		return;
	}

	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	CString strDateNow;
	strDateNow.Format("%4.4d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay());

	CString strTimeNow;
	strTimeNow.Format("%2.2d%2.2d%2.2d",
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	switch (nEventType)
	{
	case PASSWORD_EVENT_LOGON_SYSTEM:
	case PASSWORD_EVENT_LOGON_MENU:
		m_strLogonDate = strDateNow;
		m_strLogonTime = strTimeNow;
		m_nLogonType = nEventType;
		m_bLogEnabledAtLogon = (System.GetSignOnMethod() != 0) && Passwords.InUse();
		break;
	}

	switch (nEventType)
	{
	case PASSWORD_EVENT_LOGON_SYSTEM:
	case PASSWORD_EVENT_LOGON_MENU:
		if (TRUE == m_bLogEnabledAtLogon)
		{
			LogEventInternal(strDateNow, strTimeNow, nPasswordNo, nEventType);
		}
		break;

	case PASSWORD_EVENT_LOGOFF_SYSTEM:
	case PASSWORD_EVENT_LOGOFF_MENU:
		if (m_strLogonDate != "")
		{
			bool bLogEnabledNow = (System.GetSignOnMethod() != 0) && Passwords.InUse();

			if (TRUE == bLogEnabledNow )
			{
				if (FALSE == m_bLogEnabledAtLogon )
				{
					LogEventInternal(m_strLogonDate, m_strLogonTime, nPasswordNo, m_nLogonType);
				}
				LogEventInternal(strDateNow, strTimeNow, nPasswordNo, nEventType);
			}
			else
			{
				if (TRUE == m_bLogEnabledAtLogon )
				{
					LogEventInternal(strDateNow, strTimeNow, nPasswordNo, nEventType);
				}
			}
			m_strLogonDate = "";
			m_strLogonTime = "";
			m_bLogEnabledAtLogon = FALSE;
		}
		break;
	}
}

//$$******************************************************************

void CPasswordLogger::LogEventInternal(CString strDate, CString strTime, int nPasswordNo, int nEventType)
{
	CCSV csv;

	CString strUserName = "";
	if (0 == nPasswordNo)
	{
		strUserName = "Master";
	}
	else
	{
		strUserName = Passwords.GetUserName(nPasswordNo);
	}

	CString strLogDate = "";

	switch (nEventType)
	{
	case PASSWORD_EVENT_LOGOFF_SYSTEM:
	case PASSWORD_EVENT_LOGOFF_MENU:
		csv.Add(1);
		csv.Add(m_strLogonDate);
		csv.Add(m_strLogonTime);
		csv.Add(nPasswordNo);
		csv.Add(nEventType);
		csv.Add(strUserName);
		csv.Add(strDate);
		csv.Add(strTime);
		strLogDate = m_strLogonDate;
		break;

	default:
		csv.Add(1);
		csv.Add(strDate);
		csv.Add(strTime);
		csv.Add(nPasswordNo);
		csv.Add(nEventType);
		csv.Add(strUserName);
		strLogDate = strDate;
		break;
	}
	
	CreateSubdirectory(Filenames.GetUserLogFolder());

	CString strFilename = strLogDate.Left(6) + ".log";

	if (strFilename != m_strLogFilename)
	{
		if (m_pLogFilePassword != NULL)
		{
			delete(m_pLogFilePassword);
		}

		m_pLogFilePassword = new (CSSLogFile);

		if (m_pLogFilePassword != NULL)
		{
			m_pLogFilePassword->SetFilename(Filenames.GetUserLogFilename(strFilename));
		}

		m_strLogFilename = strFilename;
	}

	if (m_pLogFilePassword != NULL)
	{
		m_pLogFilePassword->WriteLine(csv.GetLine());
	}
}

//$$******************************************************************

