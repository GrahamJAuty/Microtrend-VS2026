#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CSalesXXXAutoDlg : public CDialog
{
public:
	CSalesXXXAutoDlg(CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CSalesXXXAutoDlg)
	enum { IDD = IDD_SALESXXX_AUTO };
	CSSComboBox	m_comboWait;
	BOOL	m_bAgain;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSalesXXXAutoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesXXXAutoDlg)
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
