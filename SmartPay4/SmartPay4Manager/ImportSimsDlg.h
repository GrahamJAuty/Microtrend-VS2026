#pragma once
//********************************************************************
#include "resource.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\SimsData.h"
//********************************************************************

class CImportSimsDlg : public CSSAutoShutdownDialog
{
public:
	CImportSimsDlg( CWnd* pParent = NULL);   // standard constructor

public:
	enum { IDD = IDD_IMPORTSIMS };
	CButton m_buttonView;
	CButton m_buttonView2;
	CButton m_buttonView3;
	CButton m_buttonExecute1;
	CButton m_buttonExecute2;
	CButton m_checkDeleteFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	afx_msg void OnButtonView();
	afx_msg void OnButtonView2();
	afx_msg void OnButtonView3();
	afx_msg void OnButtonExecute1();
	afx_msg void OnButtonExecute2();
	afx_msg void OnButtonBrowse();
	afx_msg void OnKillFocusImportFilename();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();

public:
	CString m_strImportFilename;
	BOOL m_bDeleteFile;
	
private:
	CSimsData m_sims;
};

//********************************************************************
