#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CReportDataTableServerDlg : public CReportDataTableDlg
{
public:
	CReportDataTableServerDlg( int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx, CWnd* pParent = NULL);   

public:
	void HandleReport1();
	void HandleReport2();
	void HandleReport3();

private:
	void CreateServerListReport(bool bSortOrder);

private:
	int m_nServerDbIdx;
	int m_nServerLocIdx;
	int m_nTerminalLocIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
