#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
//**********************************************************************
#include "ReportPurchaseHistory.h"
#include "PurchasePropertySheet.h"
//**********************************************************************

struct StatementFields
{
	double dOpening[3];				// Purse1, Purse2, Purse3
	double dAdditions[3];
	double dBonuses[3];
	double dDeductions[3];
	double dClosing[3];
};

//**********************************************************************

class CReportStatement : CPurchaseReport
{
public:
	CReportStatement ( const char* szAuditFile, const char* szLabel, __int64 nSingleUserID, CWnd* pParent );
	void Show();

private:
	int CreateReport();
	int ReportByCardNo ( CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorking );

	void ResetStatement();
	void ResetTotals();

	void AddColumnText();
	void AddColumnBalanceLine();
	void AddColumnStatementLine();
	void AddEndStatementLine ( int nCount );

	void GetStartingBalance ( CSQLAuditRecord* pAtcRecord );
	void ProcessAuditLine (CSQLAuditRecord* pAtcRecord );

	bool AnythingToReport();
	bool SaveStatement (__int64 nUserID );

	void AddBalanceLine (int nIndex );

	int SingleInactiveBalance ( CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorking );

private:
	CString m_strAuditFilename;
	StatementFields balances;
	bool m_bBonusAllowed;
	__int64 m_nSingleUserID;
};

//**********************************************************************
