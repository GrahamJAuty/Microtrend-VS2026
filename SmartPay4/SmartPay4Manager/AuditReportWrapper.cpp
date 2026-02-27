//$$******************************************************************
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//$$******************************************************************
#include "AuditReport.h"
#include "AuditReportConfig.h"
#include "AuditReportDlg.h"
//$$******************************************************************
#include "AuditReportWrapper.h"
//$$******************************************************************

CAuditReportWrapper::CAuditReportWrapper( CString strAuditFilename, CString strLabel, __int64 nSingleUserID, CWnd* pParent)
{
	m_strAuditFilename = strAuditFilename;
	m_strLabel = strLabel;
	m_pParent = pParent;
	m_nSingleUserID = nSingleUserID;
	m_pAuditArray = NULL;
}

//$$******************************************************************

void CAuditReportWrapper::SetAuditArray(CStringArray* pArray)
{
	m_pAuditArray = pArray;
}

//$$******************************************************************

void CAuditReportWrapper::ShowReport()
{
	CSmartPayAuditFile auditFile(m_strAuditFilename);
	if (auditFile.ValidateFile() == nREPORT_NOERROR)
	{
		//SINGLE CARD AUDIT
		if (m_strLabel == "R123")
		{
			CAuditReportConfig ReportConfig(m_strLabel, m_strAuditFilename);
			
			CString strCardNo = ::FormatInt64Value(m_nSingleUserID);
			
			ReportConfig.SetSingleCardFlag(TRUE);
			ReportConfig.SetStringCardNoFrom(strCardNo);
			ReportConfig.SetStringCardNoTo(strCardNo);
			ReportConfig.SetSortByCardNoFlag(TRUE);

			CRepmanHandler RepmanHandler("", m_pParent);
			RepmanHandler.SetTitle(ReportConfig.GetReportTitle());

			CAuditReport AuditReport( ReportConfig);

			int nErrNo = AuditReport.CreateReport();

			if (nREPORT_NOERROR == nErrNo)
			{
				RepmanHandler.DisplayReport(ReportConfig.GetReportFilename(), ReportConfig.GetParamsFilename(), ReportConfig.GetReportKey());
			}
			else
			{
				RepmanHandler.DisplayError(nErrNo);
			}
		}
		else
		{
			CAuditReportConfig ReportConfig(m_strLabel, m_strAuditFilename);

			switch (ReportConfig.GetReportLabelNumber())
			{
			case 111:
			case 141:
				{
					CString strCardNo = ::FormatInt64Value(m_nSingleUserID);

					ReportConfig.SetSingleCardFlag(TRUE);
					ReportConfig.SetStringCardNoFrom(strCardNo);
					ReportConfig.SetStringCardNoTo(strCardNo);
					ReportConfig.SetSortByCardNoFlag(TRUE);
				}
				break;

			default:
				ReportConfig.SetSingleCardFlag(FALSE);
				break;
			}

			CAuditReportDlg dlg( ReportConfig, m_pParent);
			dlg.DoModal();
		}
	}
}

//$$******************************************************************

void CAuditReportWrapper::ShowEditSessionAudit()
{
	CAuditReportConfig ReportConfig(m_strLabel, m_strAuditFilename);

	CString strCardNo = FormatInt64Value(m_nSingleUserID);
	
	ReportConfig.SetSingleCardFlag(TRUE);
	ReportConfig.SetStringCardNoFrom(strCardNo);
	ReportConfig.SetStringCardNoTo(strCardNo);
	ReportConfig.SetSortByCardNoFlag(TRUE);

	CRepmanHandler RepmanHandler("", m_pParent);
	RepmanHandler.SetTitle(ReportConfig.GetReportTitle());

	CAuditReport AuditReport( ReportConfig);

	int nErrNo = AuditReport.CreateReportFromArray( m_pAuditArray );

	if (nREPORT_NOERROR == nErrNo)
	{
		RepmanHandler.DisplayReport(ReportConfig.GetReportFilename(), ReportConfig.GetParamsFilename(), ReportConfig.GetReportKey());
	}
	else
	{
		RepmanHandler.DisplayError(nErrNo);
	}
}

//$$******************************************************************

void CAuditReportWrapper::ShowArchive()
{
	CAuditReportConfig ReportConfig("R120", m_strAuditFilename);
	CCardArchiveDlg dlg(ReportConfig, m_pParent);
	dlg.DoModal();
}

//$$******************************************************************

		

		

