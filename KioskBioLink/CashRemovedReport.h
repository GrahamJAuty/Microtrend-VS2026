#pragma once
//*******************************************************************
#include "PayLinkAuditRecord.h"
//*******************************************************************

class CCashRemovedReport
{
public:
	CCashRemovedReport ( CString strAuditFilename );
	bool Create();
	void Display(CWnd* pParent);

private:
	bool ReportByDate();
	bool SaveLine ( CPayLinkAuditRecord* pAtc );
	void ShowTotal();

private:
	CString m_strAuditFilename = "";
	CString m_strReportName = "";
	CString m_strReportParams = "";
	CString m_strReportKey = "Rep0005";

	CSSDate m_dateFrom;
	CSSDate m_dateTo;

	int m_nErrorNo = REPORT_NOERROR;
	double m_dTotalRemoved = 0.0;

	CReportFile m_Report;
};

//*******************************************************************