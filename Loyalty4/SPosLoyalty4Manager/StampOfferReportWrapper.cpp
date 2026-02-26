//$$******************************************************************
#include "StampOfferReportConfig.h"
#include "StampOfferReportDlg.h"
#include "LoyaltyManager.h"
#include "RepmanHandler.h"
//$$******************************************************************
#include "StampOfferReportWrapper.h"
//$$******************************************************************

CStampOfferReportWrapper::CStampOfferReportWrapper( CString strLabel, __int64 nUserID, CWnd* pParent)
{
	m_strLabel = strLabel;
	m_nUserID = nUserID;
	m_pParent = pParent;
}

//$$******************************************************************

void CStampOfferReportWrapper::ShowReport()
{
	//SINGLE CARD AUDIT
	if (m_strLabel == "R130")
	{
		CStampOfferReportConfig ReportConfig(m_strLabel);

		CString strCardNo = SimpleFormatInt64Value(m_nUserID);
		ReportConfig.SetSingleCardFlag(TRUE);
		ReportConfig.SetStringCardNoFrom(strCardNo);
		ReportConfig.SetStringCardNoTo(strCardNo);
		ReportConfig.UpdateInt64CardNo(TRUE);
		ReportConfig.SetSingleGroupFlag(FALSE);
		ReportConfig.SetSingleGroupNo(0);
		ReportConfig.SetSortByCardNoFlag(TRUE);

		CStampOfferReportDlg dlg(ReportConfig, m_pParent);
		dlg.DoModal();
	}
	else
	{
		CStampOfferReportConfig ReportConfig(m_strLabel);
		ReportConfig.SetSingleCardFlag(FALSE);
		CStampOfferReportDlg dlg(ReportConfig, m_pParent);
		dlg.DoModal();
	}
}

//$$******************************************************************


		

		

