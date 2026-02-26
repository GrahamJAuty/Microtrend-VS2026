#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CAboutDlgPosTray : public CDialog
{
public:
	CAboutDlgPosTray(CWnd* pParent = NULL);   

	//{{AFX_DATA(CAboutDlgPosTray)
	enum { IDD = IDD_ABOUTBOX_POSTRAY };
	CEdit	m_editUsers;
	CStatic	m_staticIcon;
	CComboBox m_comboLibs;
	CStatic	m_staticVersion;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlgPosTray)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlgPosTray)
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/
