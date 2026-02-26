#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "WalletStampRuleCSVArray.h"
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CReportDataTableWalletStampRuleDlg : public CReportDataTableDlg
{
public:
	CReportDataTableWalletStampRuleDlg(CWnd* pParent = NULL);

private:
	void HandleReport1();
	void HandleReport2();
};

/**********************************************************************/
#endif
/**********************************************************************/
