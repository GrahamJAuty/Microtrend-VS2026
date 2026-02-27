#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSQLTweakDlg : public CSSAutoShutdownDialog
{
public:
	CSQLTweakDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_SQLDB_TWEAK };
	CButton m_buttonFileSize;
	CButton m_buttonShrinkLog;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonFileSize();
	afx_msg void OnButtonShrinkLog();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownLogOff();
};

//$$******************************************************************

