#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\GroupSetText.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//**********************************************************************
#include "ReportGroupPeriodDlg.h"
//**********************************************************************

class CGroupPeriodSalesReport
{
public:
	CGroupPeriodSalesReport ( const char* szAuditFile, CWnd* pParent );
	bool Show();

private:
	void Reset();
	int Create ( const char* szReportFile );
	void AddColumnText();
	bool ReportByDate ( CWorkingDlg* pWorkingDlg, bool bSummaryReport );
	void GroupByPeriod ( CWorkingDlg* pWorkingDlg, bool bSummaryReport);
	bool Validate ( bool bSummaryReport );
	void ShowGroupTotal ( int nPeriodIndex, int nGroupNo );
	void ShowPeriodTotal ( int nPeriodIndex );
	void ShowDayTotal ( const char* szDate, bool bSummary );
	void ShowReportTotal ( const char* szText );

	void ShowGroupGrandTotals ( int n, const char* szText );
	void ShowTotals ( int n, const char* szTitle = "" );
	void ClearTotals ( int n );
	void ClearGroupTotals();

	void AddInt ( CCSV* pCsv, int nValue );
	void AddDouble ( CCSV* pCsv, double dValue );

private:
	CSmartPayAuditFile m_auditFile;
	CWnd* m_pParent;

	CString m_strTitle;
	bool m_bCancelButton;

	CReportGroupPeriodDlg m_dlgFilter;

	CGroupSetText m_GroupSetText;

	CSQLAuditRecord m_atc;
	CReportFileWithEndText m_Report;

	int m_nFilePos;
	int m_nRestartPos;
	CString m_strLastDate;

	int m_nCardCount[4];
	int m_nTransCount[4];
	double m_dTransPurse1[4];
	double m_dTransPurse2[4];
	double m_dTransPurse3[4];
	double m_dTransCash[4];

	int m_nGroupCount    [2][100];
	int m_nGroupTrans    [2][100];
	double m_dGroupPurse1[2][100];
	double m_dGroupPurse2[2][100];
	double m_dGroupPurse3[2][100];
	double m_dGroupCash  [2][100];

	bool m_bPeriodTextReqd;
	int m_nDayCounter;
};

//**********************************************************************
