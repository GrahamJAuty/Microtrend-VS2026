//$$******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
//$$******************************************************************
#include "AuditReport.h"
#include "AuditReportConfig.h"
#include "AuditReportDlg.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
//$$******************************************************************
#include "AuditReportWrapper.h"
//$$******************************************************************

CAuditReportWrapper::CAuditReportWrapper( CString strAuditFilename, CString strLabel, __int64 nUserID, CWnd* pParent)
{
	m_strAuditFilename = strAuditFilename;
	m_strLabel = strLabel;
	m_nUserID = nUserID;
	m_pParent = pParent;
}

//$$******************************************************************

void CAuditReportWrapper::ShowReport()
{
	CLoyaltyAuditFile auditFile(m_strAuditFilename);
	if (auditFile.ValidateFile() == nREPORT_NOERROR)
	{
		//SINGLE CARD AUDIT
		if ( (m_strLabel == "R123") || ( m_strLabel == "R133") )
		{
			CAuditReportConfig ReportConfig(m_strLabel, m_strAuditFilename);

			CString strCardNo = SimpleFormatInt64Value(m_nUserID);
			ReportConfig.SetSingleCardFlag(TRUE);
			ReportConfig.SetStringCardNoFrom(strCardNo);
			ReportConfig.SetStringCardNoTo(strCardNo);
			ReportConfig.UpdateInt64CardNo(TRUE);
			ReportConfig.SetSingleGroupFlag(FALSE);
			ReportConfig.SetSingleGroupNo(0);
			ReportConfig.SetSortByCardNoFlag(TRUE);
			
			CAuditReportDlg dlg( ReportConfig, m_pParent);
			dlg.DoModal();
		}
		else
		{
			CAuditReportConfig ReportConfig(m_strLabel, m_strAuditFilename);
			ReportConfig.SetSingleCardFlag(FALSE);
			CAuditReportDlg dlg( ReportConfig, m_pParent);
			dlg.DoModal();
		}
	}
}

//$$******************************************************************

void CAuditReportWrapper::ShowArchive()
{
	CAuditReportConfig ReportConfig("R122", m_strAuditFilename);
	CCardArchiveDlg dlg(ReportConfig, m_pParent);
	dlg.DoModal();
}

//$$******************************************************************

		

		

