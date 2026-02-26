#pragma once
/**********************************************************************/

class CAboutDlgEcrman : public CDialog
{
public:
	CAboutDlgEcrman();

	//{{AFX_DATA(CAboutDlgEcrman)
	CEdit	m_editUsers;
	CStatic	m_staticIcon;
	CStatic	m_staticVersion;
	CStatic	m_staticSerial;
	CStatic	m_staticLiteType;
	CSSComboBox	m_comboLibs;
	CButton m_buttonUsers;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlgEcrman)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlgEcrman)
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonUsers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
