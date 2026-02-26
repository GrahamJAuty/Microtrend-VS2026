#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
//*******************************************************************
#include "AuditRecord.h"
#include "ReportGroupPeriodSalesDlg.h"
#include "WorkingDlg.h"
#include "ReportInfoPeriodSales.h"
//*******************************************************************

struct CGroupPeriodSalesTotals
{
public:
	void Reset()
	{
		m_nCardCount = 0;
		m_nTransCount = 0;
		m_dPurse1 = 0.0;
		m_dPurse2 = 0.0;
		m_dCash = 0.0;
		m_dDiscount = 0.0;
	};

	void Add ( const CGroupPeriodSalesTotals& totals )
	{
		m_nCardCount += totals.m_nCardCount;
		m_nTransCount += totals.m_nTransCount;
		m_dPurse1 += totals.m_dPurse1;
		m_dPurse2 += totals.m_dPurse2;
		m_dCash += totals.m_dCash;
		m_dDiscount += totals.m_dDiscount;
	};

public:
	int m_nCardCount = 0;
	int m_nTransCount = 0;
	double m_dPurse1 = 0.0;
	double m_dPurse2 = 0.0;
	double m_dCash = 0.0;
	double m_dDiscount = 0.0;
};

//*******************************************************************

class CGroupPeriodSalesReport
{
public:
	CGroupPeriodSalesReport ( const char* szLabel, const char* szAuditFile, CWnd* pParent );
	~CGroupPeriodSalesReport();

public:
	bool Show();

private:
	void Reset();
	int Create ( const char* szReportFile );
	void AddColumnText();
	int ReportByDate ( bool bSummaryReport );
	void GroupByPeriod ( CWorkingDlg* pWorkingDlg, bool bSummaryReport);
	bool Validate ( bool bSummaryReport );
	void ShowGroupTotal ( int nPeriodIndex, int nGroupNo );
	void ShowPeriodTotal ( int nPeriodIndex );
	void ShowDayTotal ( const char* szDate, bool bSummary );
	void ShowReportTotal ( const char* szText );

	void ShowGroupGrandTotals ( int n, const char* szText );
	void ShowTotals ( int n, const char* szTitle = "");
	void ClearReportTotals ( int n );
	void ClearGroupDateTotals();
	void ClearGroupGrandTotals();
	void ClearGroupTotals(int n);

	void AddTotals(CCSV& csv, CGroupPeriodSalesTotals& totals);

private:
	CWnd* m_pParent = nullptr;
	CString m_strReportLabel = "";
	CLoyaltyAuditFile m_auditFile;
	CReportGroupPeriodSalesDlg m_dlgFilter;
	CReportInfoPeriodSales m_ReportInfo;

	CAuditRecord m_atc;
	CReportFile m_Report;
	bool m_bCancelButton = FALSE;;

	CString m_strTitle;

	int m_nFilePos = 0;
	int m_nRestartPos = 0;
	CString m_strLastDate = "";

	CGroupPeriodSalesTotals m_SectionTotals[4];
	CGroupPeriodSalesTotals m_GroupTotals[2][100];

	bool m_bPeriodTextReqd = FALSE;
	int m_nDayCounter = 0;
};

//*******************************************************************
