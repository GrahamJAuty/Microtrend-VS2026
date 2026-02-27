#pragma once
//**********************************************************************
#include "cardlink.h"
#include "afxwin.h"
//**********************************************************************

class CAboutDlg : public CSSColourDialog
{
public:
	CAboutDlg(CWnd* pParent);

	enum { IDD = IDD_ABOUTBOX };
	CComboBox m_comboLibs;
	CComboBox m_comboReports;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonSetup();
	afx_msg void OnButtonLicence();
	afx_msg void OnButtonTerminate();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonReport1();
	afx_msg void OnButtonReport2();
	afx_msg void OnButtonReport3();
	afx_msg void OnButtonTest();

	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();

public:
	bool m_bTerminateReqd = FALSE;

private:
	CSSColourStatic m_staticVersion;
	CSSColourStatic m_staticSerialNo;
	CSSColourStatic m_staticCopyright;
	CSSColourButton m_buttonDone;
	CSSColourButton m_buttonSetup;
	CSSColourButton m_buttonLicence;
	CSSColourButton m_buttonTerminate;
	CSSColourButton m_buttonRefresh;
	CSSColourButton m_buttonReport1;
	CSSColourButton m_buttonReport2;
	CSSColourButton m_buttonReport3;
	CSSColourButton m_buttonTest;
};

//**********************************************************************
