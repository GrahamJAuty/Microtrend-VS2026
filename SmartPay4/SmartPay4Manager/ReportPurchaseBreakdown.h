#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\TLogRecord.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//**********************************************************************
#include "PurchaseBreakdownDatabase.h"
#include "PurchaseBreakdownPropertySheet.h"
#include "ReportPurchaseHelpers.h"
//**********************************************************************

class CPurchaseBreakdownReport
{
public:
	CPurchaseBreakdownReport(const char* szLabel, CWnd* pParent);
	void Show();

private:
	void ResetReportTotals();
	void ResetTotals(int n);
	void AddColumnText();
	int DisplayReport();

	void ExtractPurchases(CSQLRowAccountFull& RowAccount, CWorkingDlg* pProgress, bool& bCancelButton);
	bool ValidatePlu(const char* szPluNo);

private:
	CWnd* m_pParent;

	CRepmanHandler m_Repman;
	CString m_strReportName;
	CString m_strParamsName;

	CReportFileWithEndText m_Report;

	CPurchaseBreakdownDatabase m_dbSales;

	bool m_bShowGroupColumn;
	void ShowGroupTotals(int nGroupNo);
	void ShowTotals(int n, const char* szTitle = "");

private:
	bool m_bShowPoints;

	double m_dTotalQty[3];
	double m_dTotalRefundQty[3];
	double m_dTotalValue[3];
	int m_nTotalPoints[3];
	int m_nTotalCount[3];

	CReportConsolidationArray<CSortedInt64Item> m_arrayPluNumbers;

private:
	CReportPurchaseHelpers m_ReportPurchaseHelpers;
};

//**********************************************************************
