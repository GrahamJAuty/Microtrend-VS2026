#pragma once
//$$******************************************************************
#include "../SmartPay4Shared/ServiceOptions.h"
//$$******************************************************************
#include "SmartPay4Manager.h"
//$$******************************************************************

class CServiceLogonDlg : public CSSAutoShutdownDialog
{
public:
	CServiceLogonDlg( bool bBackground, CWnd* pParent = NULL);
	
	enum { IDD = IDD_SERVICE_LOGON };

public:
	CButton m_checkLogon;
	CEdit m_editUsername;
	CEdit m_editPassword;
	CEdit m_buttonPassword;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnToggleLogon();
	afx_msg void OnButtonPassword();
	DECLARE_MESSAGE_MAP()

private:
	void UpdateServiceOptions();

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	
private:
	bool m_bBackground;
	CServiceOptions* m_pServiceOptions;

private:
	bool m_bGotOldPassword;
	CString m_strNewPassword;

private:
	bool m_bLogonState;
	CString m_strLastPassword;
	CString m_strLastUsername;
};

//$$******************************************************************
