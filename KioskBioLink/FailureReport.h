#pragma once
/**********************************************************************/
#include "PayLinkAuditRecord.h"
/**********************************************************************/

typedef struct
{
	double dValue;
	int nCount;
} 
StructTotal;

/**********************************************************************/

class CFailureReport
{
public:
	CFailureReport(CString strAuditFilename);
	bool Create();
	void Display(CWnd* pParent);

private:
	void ClearTotals(int nIndex);
	bool ReportByDate();
	bool SaveLine(CPayLinkAuditRecord* pAtc);
	void ShowTotal(int nIndex);

private:
	CString m_strAuditFilename = "";
	CString m_strReportName = "";
	CString m_strReportParams = "";
	CString m_strReportKey = "Rep0001";
	int m_nErrorNo = REPORT_NOERROR;

	CSSDate m_dateFrom;
	CSSDate m_dateTo;
	CReportFile m_Report;

	StructTotal m_Totals[2];
};

/**********************************************************************/
