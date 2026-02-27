#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CPINAdminDlg : public CSSAutoShutdownDialog
{
public:
	CPINAdminDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_PINADMIN };
	CButton m_checkExclude;
	CButton m_checkRemove;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonOrphan();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownLogOff();
};

//$$******************************************************************

