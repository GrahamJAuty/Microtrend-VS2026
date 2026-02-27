#pragma once
//******************************************************************
#include "..\SmartPay4Shared\EmailAlertHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//******************************************************************
#include "EmailLogDatabase.h"
//******************************************************************

class CEmailPurchases
{
//******************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	CEmailPurchases();

//******************************************************************
#else
//******************************************************************

public:
	CEmailPurchases (CWnd* pParent );

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
	void CreateEmail( CSQLRowAccountFull& RowAccount, CEmailLogDatabase& db, bool& bCancelButton, CString& strError );
	CString SendEmail( CSQLRowAccountFull& RowAccount, const char* szReportFile );

private:
	CString m_strTitle;
	int m_nExportCount;

	CEmailAlertHandler m_data;
};

//*****************************************************************
