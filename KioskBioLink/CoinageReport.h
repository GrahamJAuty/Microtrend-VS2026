#pragma once
/**********************************************************************/
#include "PayLinkAuditRecord.h"
/**********************************************************************/

class CCoinageReport
{
public:
	CCoinageReport ( CString strAuditFilename );
	bool Create();
	void Display(CWnd* pParent);

private:
	bool ReportByDate();
	bool SaveLine ( CPayLinkAuditRecord* pAtc );

private:
	CString m_strAuditFilename = "";
	CString m_strReportName = "";
	CString m_strReportParams = "";
	CString m_strReportKey = "Rep0007";

	CSSDate m_dateFrom;
	CSSDate m_dateTo;

	int m_nErrorNo = REPORT_NOERROR;

	int m_nLastCCNo = -1;
	long m_lRunningGT = 0;
	CString m_strLastUserID = "";
	CString m_strLastName = "";

	CReportFile m_Report;
};

/**********************************************************************/
