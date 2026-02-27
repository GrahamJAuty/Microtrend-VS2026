#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\ParentPayData.h"
//$$******************************************************************

class CSetupParentPayDlg : public CSSAutoShutdownDialog
{
public:
	CSetupParentPayDlg(CWnd* pParent = NULL);  
	virtual ~CSetupParentPayDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_PARENTPAY };
	CEdit m_editMealSession1;
	CEdit m_editMealSession2;
	CEdit m_editSnapShotID1;
	CEdit m_editSnapShotID2;
	CEdit m_editSnapShotID3;
	CEdit m_editPosID;
	CEdit m_editNextID;
	CEdit m_editLastPaymentID1;
	CEdit m_editLastPaymentID2;
	CEdit m_editServiceID1;
	CEdit m_editServiceID2;
	CButton m_buttonDelete;
	CButton m_checkKeepXMLFiles;
	CButton m_checkKeepXMLLogin;
	CButton m_checkExportPurse2Sales;

	int m_nInterfaceType;
	BOOL m_bKeepXmlFiles;
	BOOL m_bKeepXmlLogin;
	CString m_strUsername;
	CString m_strPassword;
	CString m_strSoapVersion;
	CString m_strSchoolID;
	CSSComboBox m_comboSoap;

	BOOL m_bImportPayments1;
	__int64	m_nServiceID1;
	CSSComboBox m_comboAccount1Purse;
	int m_nMealSession1;
	__int64 m_nLastPaymentID1;

	BOOL m_bImportPayments2;
	__int64	m_nServiceID2;
	CSSComboBox m_comboAccount2Purse;
	int m_nMealSession2;
	__int64 m_nLastPaymentID2;

	CString m_strPosID;

	__int64 m_nLastDinerSnapshotID;
	__int64 m_nLastBalanceSnapshotID;
	__int64 m_nLastPosTxnID;
	__int64 m_nNextPosTxnID;

	CButton m_buttonAltKey1;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void OnOK();
	afx_msg void OnClickStaticPayment1();
	afx_msg void OnClickStaticPayment2();
	afx_msg void OnClickStaticSnapshot();
	afx_msg void OnClickStaticSnapshot2();
	afx_msg void OnClickStaticSnapshot3();
	afx_msg void OnClickStaticPosid();
	afx_msg void OnClickStaticNextid();
	afx_msg void OnButtonSetup();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonAltKey1();
	DECLARE_MESSAGE_MAP()

private:
	virtual void CSSAutoShutdownPreInitDialog();
	virtual BOOL CSSAutoShutdownPostInitDialog();


private:
	void SetDisplay();

private:
	CParentPayData m_data;	
	int m_nAccount1Purse;
	int m_nAccount2Purse;
};

//$$******************************************************************

