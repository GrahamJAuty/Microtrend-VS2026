#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL); 
	virtual ~CAboutDlg() {};

	enum { IDD = IDD_ABOUTBOX };
	CStatic m_staticVersion;
	CStatic m_staticSerialNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CSSComboBox m_comboLibs;
};

//$$******************************************************************
