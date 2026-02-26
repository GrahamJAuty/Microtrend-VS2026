#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSalesXXXStatsDlg : public CDialog
{
public:
	CSalesXXXStatsDlg(CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CSalesXXXStatsDlg)
	enum { IDD = IDD_SALESXXX_STATS };
	CStatic	m_staticNumber7;
	CStatic	m_staticNumber6;
	CStatic	m_staticNumber5;
	CStatic	m_staticNumber4;
	CStatic	m_staticNumber3;
	CStatic	m_staticNumber2;
	CStatic	m_staticNumber1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesXXXStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesXXXStatsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nNumber1;
	int m_nNumber2;
	int m_nNumber3;
	int m_nNumber4;
	int m_nNumber5;
	int m_nNumber6;
	int m_nNumber7;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif 
/**********************************************************************/

