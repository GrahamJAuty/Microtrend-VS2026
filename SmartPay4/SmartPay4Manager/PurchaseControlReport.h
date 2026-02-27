#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//$$******************************************************************
#include "PurchaseControlReportConfig.h"
//$$******************************************************************

class CPurchaseControlReport
{
public:
	CPurchaseControlReport(CPurchaseControlReportConfig& ReportConfig);

public:
	int CreateReport();

private:
	void AddColumnText();
	void CreateFileDateArray();

private:
	int PurchaseControlReport(CWorkingDlg* pDlgWorking);
	int PurchaseControlReportByCardNo(CWorkingDlg* pDlgWorking);
	int PurchaseControlReportByDate(CWorkingDlg* pDlgWorking);

private:
	bool IsValid(CString strLine, CSQLPurchaseControlAuditLine& Line);
	bool IsCardInRange(CSQLPurchaseControlAuditLine& Line);
	bool SaveLine(CSQLPurchaseControlAuditLine& Line, bool bExists);
	CString FormatDate(CString strDate, bool bIsExpiry);
	CString FormatTime(CString strTime);

private:
	CReportConsolidationArray<CSortedStringItem> m_arrayFileDates;
	CPurchaseControlReportConfig& m_ReportConfig;
	CReportFile m_Report;

private:
	CString m_strFileDateFrom;
	CString m_strFileDateTo;
	CString m_strFileTimeFrom;
	CString m_strFileTimeTo;
};

//$$******************************************************************