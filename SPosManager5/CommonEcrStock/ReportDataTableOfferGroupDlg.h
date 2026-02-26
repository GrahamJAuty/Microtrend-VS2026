#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CReportDataTableOfferGroupDlg : public CReportDataTableDlg
{
public:
	CReportDataTableOfferGroupDlg(CWnd* pParent = NULL);   

private:
	void HandleReport1();
	void HandleReport2();

private:
	void CreateReport( bool bSPOSVersion );
};

/**********************************************************************/
#endif
/**********************************************************************/
