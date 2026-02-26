#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CSalesXXXWarnDlg : public CDialog
{
public:
	CSalesXXXWarnDlg(CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CSalesXXXWarnDlg)
	enum { IDD = IDD_SALESXXX_WARN };
	CSSComboBox	m_comboWait;
	BOOL	m_bAgain;
	BOOL	m_bProcess;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesXXXWarnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesXXXWarnDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnToggleAgain();
	//}}AFX_MSG
	afx_msg void OnCancel();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
