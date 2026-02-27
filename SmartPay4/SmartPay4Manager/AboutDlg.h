#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CAboutDlg : public CSSAutoShutdownDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);   
	virtual ~CAboutDlg() {};

public:
	enum { IDD = IDD_ABOUTBOX };
	CStatic m_staticSerialNo;
	CStatic m_staticVersion;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonUsers();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CSSComboBox m_comboLibs;

public:
	int m_nMaxUsers;
};

//$$******************************************************************
