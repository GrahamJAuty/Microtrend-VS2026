#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 #include "SharedResources.h"
/**********************************************************************/

class CPosTrayMonthDaysDlg : public CSSDialog
{
public:
	CPosTrayMonthDaysDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CPosTrayMonthDaysDlg)
	enum { IDD = IDD_POSTRAY_MONTHDAYS };
	CButton m_checkMonthDays[31];
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPosTrayMonthDaysDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPosTrayMonthDaysDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonFirst();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nMonthDays;
};

/**********************************************************************/
#endif
/**********************************************************************/
