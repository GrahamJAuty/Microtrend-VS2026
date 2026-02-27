#pragma once
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\TucasiJSONData.h"
//*******************************************************************
#include "SmartPay4Manager.h"
//*******************************************************************

class CSetupTucasiJsonDlg : public CSSAutoShutdownDialog
{
public:
	CSetupTucasiJsonDlg(CWnd* pParent = NULL);   // standard constructor
	
	enum { IDD = IDD_SETUP_TUCASIV2 };
	CButton m_buttonDeleteJSON;
	CButton m_checkIncludePurse2;
	CButton m_checkKeepXMLFiles;
	CButton m_checkKeepXMLLogin;
	CEdit m_editLastPayment;
	BOOL m_bKeepXmlFiles;
	CString m_strIPAddress;
	CString m_strSchoolID;
	CString m_strUserName;
	CString m_strPassword;
	CSSComboBox m_comboAccountPurse;
	__int64 m_nLastPaymentID;
	int m_nMaxTransactionLines;
	BOOL m_bIncludePurse2;
	BOOL m_bExportBalances;
	CButton m_buttonAltKey1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnToggleExportBalances();
	afx_msg void OnClickStaticPayment1();
	afx_msg void OnButtonAltKey1();
	afx_msg void OnButtonDeleteJSON();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	void CSSAutoShutdownPreInitDialog();
	BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();

private:
	CTucasiJsonData m_data;
	int m_nAccountPurse;
};

//*******************************************************************
