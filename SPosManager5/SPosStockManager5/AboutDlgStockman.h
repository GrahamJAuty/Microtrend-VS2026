#pragma once
/**********************************************************************/

class CAboutDlgStockman : public CDialog
{
public:
	CAboutDlgStockman();

	//{{AFX_DATA(CAboutDlgStockman)
	CEdit	m_editUsers;
	CStatic	m_staticIcon;
	CStatic	m_staticVersion;
	CStatic	m_staticSerial;
	CSSComboBox	m_comboLibs;
	CButton m_buttonUsers;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlgStockman)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlgStockman)
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonUsers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
