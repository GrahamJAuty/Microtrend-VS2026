//$$******************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
//$$******************************************************************
#include "PurchaseControlReportConfig.h"
#include "PurchaseControlReportDlg.h"
//$$******************************************************************
#include "PurchaseControlReportWrapper.h"
//$$******************************************************************

CPurchaseControlReportWrapper::CPurchaseControlReportWrapper( CString strLabel, __int64 nUserID, CWnd* pParent)
{
	m_strLabel = strLabel;
	m_nUserID = nUserID;
	m_pParent = pParent;
}

//$$******************************************************************

void CPurchaseControlReportWrapper::ShowReport()
{
	//SINGLE CARD AUDIT
	if (m_strLabel == "R143")
	{
		CPurchaseControlReportConfig ReportConfig(m_strLabel);

		CString strCardNo = SimpleFormatInt64Value(m_nUserID);
		ReportConfig.SetSingleCardFlag(TRUE);
		ReportConfig.SetStringCardNoFrom(strCardNo);
		ReportConfig.SetStringCardNoTo(strCardNo);
		ReportConfig.UpdateInt64CardNo(TRUE);
		ReportConfig.SetSingleGroupFlag(FALSE);
		ReportConfig.SetSingleGroupNo(0);
		ReportConfig.SetSortByCardNoFlag(TRUE);

		CPurchaseControlReportDlg dlg(ReportConfig, m_pParent);
		dlg.DoModal();
	}
	else
	{
		CPurchaseControlReportConfig ReportConfig(m_strLabel);
		ReportConfig.SetSingleCardFlag(FALSE);
		CPurchaseControlReportDlg dlg(ReportConfig, m_pParent);
		dlg.DoModal();
	}
}

//$$******************************************************************


		

		

