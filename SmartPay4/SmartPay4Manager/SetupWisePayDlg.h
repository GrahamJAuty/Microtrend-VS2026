#pragma once
//********************************************************************
#include "resource.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\WisePayData.h"
//********************************************************************

class CSetupWisePayDlg : public CSSAutoShutdownDialog
{
public:
	CSetupWisePayDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_SETUP_WISEPAY };
	CEdit m_editAddPurse1;
	CEdit m_editAddPurse2;
	CButton m_buttonDelete;
	CButton m_checkIncludePurse2;
	CButton m_checkKeepXMLFiles;
	CButton m_checkKeepXMLLogin;
	CEdit m_editLastPayment;
	CEdit m_editLastPayment2;
	BOOL m_bKeepXmlFiles;
	BOOL m_bKeepXmlLogin;
	CString m_strSchoolID;
	CSSComboBox m_comboSoap;
	CString m_strSoapVersion;
	BOOL m_bImportAccount1;
	int	m_nAddPurse1ID;
	__int64 m_nLastAccount1;
	BOOL m_bImportAccount2;
	int	m_nAddPurse2ID;
	__int64 m_nLastAccount2;
	CSSComboBox m_comboAccount1Purse;
	CSSComboBox m_comboAccount2Purse;
	BOOL m_bIncludePurse2;
	CButton m_buttonAltKey1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnClickStaticPayment1();
	afx_msg void OnClickStaticPayment2();
	afx_msg void OnToggleAccount1();
	afx_msg void OnToggleAccount2();
	afx_msg void OnButtonSetup();
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
	CWisePayData m_data;
	int m_nAccount1Purse;
	int m_nAccount2Purse;
};

//********************************************************************
