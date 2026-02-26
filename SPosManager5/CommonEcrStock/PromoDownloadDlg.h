#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CPromoDownloadDlg : public CReportDataTableDlg
{
public:
	CPromoDownloadDlg(CWnd* pParent = NULL);   

public:
	void HandleReport1();
	void HandleReport2();

public:
	int m_nDownloadType;
};

/**********************************************************************/
#endif
/**********************************************************************/
