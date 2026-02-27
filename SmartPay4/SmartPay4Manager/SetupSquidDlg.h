#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\SquidData.h"
//*******************************************************************

class CSetupSquidDlg : public CSSAutoShutdownDialog
{
public:
	CSetupSquidDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	enum { IDD = IDD_SETUP_SQUID };
	CButton m_buttonAltKey1;
	CButton m_buttonDelete;
	CButton m_checkKeepXMLFiles;
	CButton m_checkKeepXMLLogin;
	BOOL m_bKeepXmlFiles;
	BOOL m_bKeepXmlLogin;
	BOOL m_bKeepLinkLog;
	CString m_strIPAddress;
	CString m_strMerchantID;
	CString m_strIssuerID;
	CString m_strPassword;
	int m_nMaxTransactionLines;
	CSSComboBox m_comboAccountPurse;
	CSSComboBox m_comboSoap;
	CString m_strSoapVersion;
	CSSComboBox m_comboExternalRef;
	CSSComboBox m_comboSquidPupilID;
	CSSComboBox m_comboSquidCard;
	CSSComboBox m_comboSquidCVV;
	int m_nSquidCardHolderField;
	int m_nSquidExternalRefField;
	int m_nSquidCardField;
	int m_nSquidCVVField;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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
	CSquidData m_data;
	int m_nAccountPurse;
};

//*******************************************************************
