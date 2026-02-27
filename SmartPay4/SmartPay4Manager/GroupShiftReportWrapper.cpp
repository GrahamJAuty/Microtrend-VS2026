//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//$$******************************************************************
#include "GroupShiftReportConfig.h"
#include "GroupShiftReportDlg.h"
#include "SmartPayGroupShiftFile.h"
//$$******************************************************************
#include "GroupShiftReportWrapper.h"
//$$******************************************************************

CGroupShiftReportWrapper::CGroupShiftReportWrapper( CString strLabel, __int64 nSingleUserID, CWnd* pParent)
{
	m_strLabel = strLabel;
	m_pParent = pParent;
	m_nSingleUserID = nSingleUserID;
}

//$$******************************************************************

void CGroupShiftReportWrapper::ShowReport()
{
	CSmartPayGroupShiftFile auditFile(Filenames.GetGroupShiftLogFilename());

	CGroupShiftReportConfig ReportConfig(m_strLabel);

	if (m_strLabel == "R141")					// single audit report
	{
		CString strCardNo = ::FormatInt64Value(m_nSingleUserID);

		ReportConfig.SetSingleCardFlag(TRUE);
		ReportConfig.SetStringCardNoFrom(strCardNo);
		ReportConfig.SetStringCardNoTo(strCardNo);
		ReportConfig.SetSortByCardNoFlag(TRUE);
	}
	else
	{
		ReportConfig.SetSingleCardFlag(FALSE);
	}

	CGroupShiftReportDlg dlg(ReportConfig, m_pParent);
	dlg.DoModal();
}

//$$******************************************************************



		

		

