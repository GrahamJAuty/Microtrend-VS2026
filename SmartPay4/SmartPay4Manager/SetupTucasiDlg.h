#pragma once
/*******************************************************************/
#include "..\SmartPay4ManagerBgnd\TucasiSoapData.h"
/*******************************************************************/
#include "resource.h"
/*******************************************************************/

class CSetupTucasiDlg : public CSSAutoShutdownDialog
{
public:
	CSetupTucasiDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_SETUP_TUCASI };
	CButton m_buttonDelete;
	CButton m_checkIncludePurse2;
	CButton m_checkKeepXMLFiles;
	CButton m_checkKeepXMLLogin;
	CEdit m_editLastPayment;
	BOOL m_bKeepXmlFiles;
	BOOL m_bKeepXmlLogin;
	CString m_strSchoolID;
	CString m_strUserName;
	CString m_strPassword;
	CSSComboBox m_comboSoap;
	CSSComboBox m_comboAccountPurse;
	CString m_strSoapVersion;
	__int64 m_nLastPaymentID;
	BOOL m_bIncludePurse2;
	BOOL m_bImportPayments;
	BOOL m_bExportUpdates;
	BOOL m_bExportBalances;
	CButton m_buttonAltKey1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnToggleExportBalances();
	afx_msg void OnToggleImportPayments();
	afx_msg void OnClickStaticPayment1();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonAltKey1();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();

private:
	CTucasiSoapData m_data;
};

/*******************************************************************/
