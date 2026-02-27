#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentMailData.h"
//*******************************************************************

class CSetupParentMailDlg : public CSSAutoShutdownDialog
{
public:
	CSetupParentMailDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_SETUP_PARENTMAIL };
	CSSComboBox m_comboSoap;
	CSSComboBox m_comboAccountPurse;
	CSSComboBox m_comboFormName;
	CSSComboBox m_comboAdditionalRefField;
	CButton m_buttonAltKey1;
	CButton m_buttonDelete;
	CButton m_checkKeepXMLFiles;
	CButton m_checkKeepXMLLogin;
	CEdit m_editLastPaymentID;
	CEdit m_editSnapshotID;
	CEdit m_editStaffGroupList;
	BOOL m_bKeepXmlFiles;
	BOOL m_bKeepXmlLogin;
	CString m_strSchoolID;
	CString m_strUsername;
	CString m_strPassword;
	CString m_strSoapVersion;
	__int64 m_nLastPaymentID;
	__int64 m_nInternalSnapshotID;
	int m_nFormNameRefField;
	int m_nMaxTransactionLines;
	int m_nAdditionalRefField;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnClickStaticPayment();
	afx_msg void OnButtonDelete();
	afx_msg void OnClickStaticSnapshot();
	afx_msg void OnButtonAltKey1();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	CString m_strStaffGroupList;

private:
	CParentMailData m_data;
	int m_nAccountPurse;
};

//*******************************************************************
