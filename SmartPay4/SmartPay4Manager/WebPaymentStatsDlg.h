#pragma once
//************************************************************
#include "resource.h"
//************************************************************

class CWebPaymentStatsDlg : public CSSAutoShutdownDialog
{
public:
	CWebPaymentStatsDlg(int m_nWebPaymentType, CWnd* pParent = NULL);

	enum { IDD = IDD_WEBPAYMENTSTATS };
	CButton	m_buttonExceptions;
	CEdit m_editExceptions;
	int		m_nExceptions;
	int		m_nRead;
	int		m_nValid;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnButtonExceptions();
	afx_msg void OnButtonView();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CString m_strExceptionFilename;
	CString m_strImportFilename;

private:
	int m_nWebPaymentType;
};

//************************************************************
