#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSalesXXXStatsDiscardDlg : public CDialog
{
public:
	CSalesXXXStatsDiscardDlg(CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CSalesXXXStatsDiscardDlg)
	enum { IDD = IDD_SALESXXX_STATS_DISCARD };
	CStatic	m_staticNumber2;
	CStatic	m_staticNumber1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesXXXStatsDiscardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesXXXStatsDiscardDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nNumber1;
	int m_nNumber2;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif 
/**********************************************************************/
