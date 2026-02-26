#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CReportDataTableTimePlanDlg : public CReportDataTableDlg
{
public:
	CReportDataTableTimePlanDlg(CWnd* pParent = NULL);   

public:
	void HandleReport1();
	void HandleReport2();
	void HandleReport3();
};

/**********************************************************************/

class CTimePlanTypeSelectDlg : public CReportDataTableDlg
{
public:
	CTimePlanTypeSelectDlg(CWnd* pParent = NULL);

public:
	void HandleReport1() { m_nType = 1; }
	void HandleReport2() { m_nType = 2; }

public:
	int m_nType = 0;
};

/**********************************************************************/
#endif
/**********************************************************************/
