#pragma once
//$$******************************************************************
#include "../SPosLoyalty4Shared/ServiceOptions.h"
//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************

class CServiceLogonDlg : public CSSDialog
{
public:
	CServiceLogonDlg( CWnd* pParent = NULL); 
	
	enum { IDD = IDD_SERVICE_LOGON };

public:
	CButton m_checkLogon;
	CEdit m_editUsername;
	CEdit m_editPassword;
	CEdit m_buttonPassword;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnToggleLogon();
	afx_msg void OnButtonPassword();
	DECLARE_MESSAGE_MAP()

private:
	void UpdateServiceOptions();

private:
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
