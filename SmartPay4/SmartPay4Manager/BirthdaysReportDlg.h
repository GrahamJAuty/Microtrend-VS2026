#pragma once
//**********************************************************************
#include "resource.h"
//**********************************************************************

class CBirthdaysReportDlg : public CSSAutoShutdownDialog
{
public:
	CBirthdaysReportDlg( CWnd* pParent = NULL);   // standard constructor
	virtual ~CBirthdaysReportDlg();

	enum { IDD = IDD_BIRTHDAYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonAll();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	BOOL m_bCheckMonth1;
	BOOL m_bCheckMonth2;
	BOOL m_bCheckMonth3;
	BOOL m_bCheckMonth4;
	BOOL m_bCheckMonth5;
	BOOL m_bCheckMonth6;
	BOOL m_bCheckMonth7;
	BOOL m_bCheckMonth8;
	BOOL m_bCheckMonth9;
	BOOL m_bCheckMonth10;
	BOOL m_bCheckMonth11;
	BOOL m_bCheckMonth12;
	CButton m_buttonAll;
	BOOL m_bDobReqd;
	BOOL m_bExpiryReqd;

private:
	int m_nSelectAll;
	int m_nCurrentMonth;
	int m_nNextMonth;

private:
	int MonthsSelected();
};

//**********************************************************************
