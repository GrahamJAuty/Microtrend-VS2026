#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\SchoolCommsData.h"
//*******************************************************************

class CSetupSchoolcommsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupSchoolcommsDlg(CWnd* pParent = NULL);   // standard constructor
	
	enum { IDD = IDD_SETUP_SCHOOLCOMMS };
	BOOL m_bKeepXmlFiles;
	BOOL m_bKeepXmlLogin;
	CString m_strSchoolID;
	CString m_strUserName;
	CString m_strPassword;
	CString m_strMemorable;
	__int64 m_nLastPaymentID;
	int m_nMaxTransactionLines;
	CString m_strSoapVersion;
	BOOL m_bIncludePurse2;
	/*****/
	CSSComboBox m_comboAccountPurse;
	CSSComboBox m_comboSoap;
	CEdit m_editLastPaymentID;
	CButton m_buttonAltKey1;
	CButton m_checkKeepXmlFiles;
	CButton m_checkKeepXmlLogin;
	CButton m_buttonDelete;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClickStaticPayment();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonAltKey1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	CSchoolcommsData m_data;
	int m_nAccountPurse;
};

//*******************************************************************
