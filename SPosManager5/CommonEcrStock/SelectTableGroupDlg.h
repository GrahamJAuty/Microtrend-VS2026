#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CSelectTableGroupDlg : public CReportDataTableDlg
{
public:
	CSelectTableGroupDlg( int nLocIdx, CWnd* pParent = NULL);   

public:
	void HandleInitDialog();
	void HandleReport1();
	void HandleReport2();
	void HandleReport3();

private:
	void EditGroups( int nLevel );

private:
	int m_nLocIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
