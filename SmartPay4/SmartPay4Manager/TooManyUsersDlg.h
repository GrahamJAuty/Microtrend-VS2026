#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/

class CTooManyUsersDlg : public CDialog
{
public:
	CTooManyUsersDlg(CWnd* pParent = NULL);   
	virtual ~CTooManyUsersDlg();

	enum { IDD = IDD_TOO_MANY_USERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	CStatic m_staticLine1;
	CStatic m_staticLine2;
	CStatic m_staticLine3;
	CStatic m_staticLine4;

	//{{AFX_MSG(CTooManyUsersDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
