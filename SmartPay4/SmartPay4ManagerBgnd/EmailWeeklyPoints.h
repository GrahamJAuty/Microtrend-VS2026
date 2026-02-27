#pragma once
//******************************************************************
#include "..\SmartPay4Shared\EmailAlertHandler.h"
//******************************************************************

class CEmailWeeklyPoints
{
//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	CEmailWeeklyPoints();

//******************************************************************
#else
//******************************************************************

public:
	CEmailWeeklyPoints(CWnd* pParent );

private:
	CWnd* m_pParent;
	bool m_bCancelButton;

//******************************************************************
#endif
//*****************************************************************

private:
	void Init();

public:
	void Execute();
	int GetExportCount() { return m_nExportCount; }

private:
	void ScanDatabase();
	bool CheckNewEmailRequired(CSQLRowAccountFull& RowAccount);
	CString SendEmail(CSQLRowAccountFull& RowAccount);
	void LogEmailSent(CSQLRowAccountFull& RowAccount);
	CString FormatDateTime ( const char* szDate, const char* szTime );

private:
	CString m_strTitle;
	int m_nExportCount;
	
	CEmailAlertHandler m_data;

	CString m_strEmailLogDatabase;
	CSSDate m_dateToday;

	int m_nLastPoints;
	CSSDate m_dateLastSent;
	CSSTime m_timeLastSent;
};

//******************************************************************
