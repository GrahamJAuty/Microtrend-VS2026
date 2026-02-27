#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CAlertTextSelectDlg : public CSSAutoShutdownDialog
{
public:
	CAlertTextSelectDlg(CWnd* pParent = NULL);
	
public:
	enum { IDD = IDD_ALERTTEXT };
	CButton m_radioAlert[10];
	CButton m_radioClear;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	int m_nAlertCode;
	BOOL m_bClearAllowed;
};

//$$******************************************************************