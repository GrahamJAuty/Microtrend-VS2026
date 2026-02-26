#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ReportDataTableDlg.h"
/**********************************************************************/

class CSelectTableGroupSetDlg : public CReportDataTableDlg
{
public:
	CSelectTableGroupSetDlg( int nLocIdx, CWnd* pParent = NULL);   

public:
	void HandleInitDialog();
	void HandleReport1();
	void HandleReport2();
	void HandleReport3();

private:
	void EditGroupSets( int nLevel );

private:
	int m_nLocIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
