#pragma once
//******************************************************************
#include "..\SmartPay4Shared\EmailAlertHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//******************************************************************

class CEmailLowBalances
{

//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	CEmailLowBalances ();

//******************************************************************
#else
//******************************************************************

public:
	CEmailLowBalances (CWnd* pParent );

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

	bool CheckLowBalances(CSQLRowAccountFull& RowAccount);
	bool CheckNewEmailRequired(CSQLRowAccountFull& RowAccount);
	CString SendEmail(CSQLRowAccountFull& RowAccount);
	void LogEmailSent(CSQLRowAccountFull& RowAccount);

	CString GetBalanceText( int nPurse );
	CString GetValueText( int nPurse, double dPurse );

private:
	CString m_strTitle;
	int m_nExportCount;
	
	CEmailAlertHandler m_data;
	CString m_strEmailLogDatabase;
};

//******************************************************************
