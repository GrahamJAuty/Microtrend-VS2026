//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
//$$******************************************************************
#include "AuditReport.h"
#include "AuditReportConfig.h"
#include "PCAuditReportDlg.h"
//$$******************************************************************
#include "PCAuditReportWrapper.h"
//$$******************************************************************

CPCAuditReportWrapper::CPCAuditReportWrapper(CString strAuditFilename, CString strLabel, CWnd* pParent)
{
	m_strAuditFilename = strAuditFilename;
	m_strLabel = strLabel;
	m_pParent = pParent;
}

//$$******************************************************************

void CPCAuditReportWrapper::ShowReport()
{
	CSmartPayAuditFile auditFile(m_strAuditFilename);
	if (auditFile.ValidateFile() == nREPORT_NOERROR)
	{
		CAuditReportConfig ReportConfig(m_strLabel, m_strAuditFilename);
		ReportConfig.SetSingleCardFlag(FALSE);
		CPCAuditReportDlg dlg( ReportConfig, m_pParent);
		dlg.DoModal();
	}
}

//$$******************************************************************
