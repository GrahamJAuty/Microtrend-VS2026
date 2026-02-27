#pragma once
//******************************************************************
#include "..\SmartPay4Shared\EmailAlertHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//******************************************************************

class CEmailPointsAchievement
{
//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	CEmailPointsAchievement();

//******************************************************************
#else
//******************************************************************

public:
	CEmailPointsAchievement( CWnd* pParent );

private:
	CWnd* m_pParent;
	bool m_bCancelButton;
	
//******************************************************************
#endif
//*****************************************************************

public:
	void Execute();
	int GetExportCount() { return m_nExportCount; }

private:
	void Init();

private:
	void ScanDatabase();

	bool CheckNewEmailRequired(CSQLRowAccountFull& RowAccount);
	CString SendEmail(CSQLRowAccountFull& RowAccount);
	void LogEmailSent(CSQLRowAccountFull& RowAccount);

private:
	CString m_strTitle;
	int m_nExportCount;
	
	int m_nPointsThreshold;

	CEmailAlertHandler m_data;
	CString m_strEmailLogDatabase;
};

//*****************************************************************
