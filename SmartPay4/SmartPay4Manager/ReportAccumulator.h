#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
//**********************************************************************
#include "AccumulatorReportInfo.h"
//**********************************************************************

class CReportAccumulator
{
public:
	CReportAccumulator(CAccumulatorReportInfo& ReportInfo, const char* szAuditFile, CWnd* pParent);
	bool Show();

private:
	int Create(const char* szReportFile);
	int CompileReport();

	void AddColumnText();
	bool SaveReportLine(CSQLRowAccountFull& RowAccount, bool bExist, int nQty, double dValue, int nPos = 0);

	void ClearTotals(int nIndex);
	void SaveSortOnTotal(const char* szSortOnText);
	void SaveGroupTotal(int nGroupNo);
	void SaveReportTotal(bool bRankReport = FALSE);
	void SaveTotals(int nIndex, const char* szText, bool bRankReport = FALSE);

private:
	CWnd* m_pParent;
	CAccumulatorReportInfo& m_ReportInfo;
	CDbReportHandler m_DbReporter;

	CSmartPayAuditFile m_auditFile;
	CSQLAuditRecord m_atc;
	bool m_bAuditFileChecked;

	CReportFileWithEndText m_Report;
	CString m_strReportLabel;
	int m_nReportType;

	double m_dReportValue[nREPORT_MAX];
	int m_nReportQty[nREPORT_MAX];
	int m_nReportCount[nREPORT_MAX];
};

//**********************************************************************
