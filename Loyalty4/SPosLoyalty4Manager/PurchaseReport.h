#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRecordSetAccountFull.h"
//*******************************************************************
#include "WorkingDlg.h"
#include "RepmanHandler.h"
#include "ReportInfoPurchaseHistory.h"
#include "TLogRecord.h"
//*******************************************************************

struct CPurchaseReportTotals
{
public:
	CPurchaseReportTotals()
	{
		Reset();
	}

	void Reset()
	{
		m_nTotalPoints = 0;
		m_dTotalValue = 0.0;
		m_dTotalCardSpend = 0.0;
	}

public:
	int m_nTotalPoints;
	double m_dTotalValue;
	double m_dTotalCardSpend;
};

//*******************************************************************

class CPurchaseReport
{
public:
	CPurchaseReport(const char* szLabel, CWnd* pParent);
	void Show(__int64 nUserID);

private:
	void ResetTotals(int nLevel);
	int DisplayAll();
	int DisplayOne(__int64 nUserID);
	void AddColumnText();

	int PrintTReport(CReportInfoPurchaseHistory& ReportInfo, __int64 nUserID, CWorkingDlg* pWorkingDlg, bool& bCancelButton);
	void PrintTLogRecord(CTLogRecord* pTLogRecord, CReportInfoPurchaseHistory& ReportInfo);
	bool IsRenewalTLogLine(CTLogRecord* pTLogRecord);
	bool IsRenewalTLogItem(CTLogRecord* pTLogRecord);
	void PrintDetailTotals(int nLevel);

	void AddSummaryColumnText();
	int ConsolidateTReport(CReportInfoPurchaseHistory& ReportInfo, __int64 nUserID, CWorkingDlg* pWorkingDlg, bool& bCancelButton, bool& bFirstAccount);
	void PrintSummaryTotals(int nLevel);

	void AddCardDetails(__int64 nUserID);
	CString GetItemTextForReport(CTLogRecord* pTLogRecord);

private:
	void AddTotalPoints(int n);
	void AddTotalValue(double d);
	void AddTotalCardSpend(double d);

private:
	CString m_strReportLabel;
	CWnd* m_pParent;
	CRepmanHandler m_Repman;

	CReportFile m_Report;
	CString m_strReportTitle;

	CString m_strLastDate;
	bool m_bNewPageReqd;

	int m_nReportLines;
	bool m_bIncludePoints;

private:
	CStringArray m_arrayDbNames;
	CPurchaseReportTotals m_arrayTotals[3];
};

//*******************************************************************
