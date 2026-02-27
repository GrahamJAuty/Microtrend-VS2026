#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//$$******************************************************************

class CSetupWebPaymentOptionsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupWebPaymentOptionsDlg(int m_nWebPaymentType, CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_SETUP_WEBPAYMENTOPTIONS };
	CStatic m_staticAccount1;
	CStatic m_staticAccount2;
	CStatic m_staticAccount3;
	CStatic m_staticAccount4;
	CStatic m_staticAccount5;
	CButton m_checkAccountMon;
	CButton m_checkAccountTue;
	CButton m_checkAccountWed;
	CButton m_checkAccountThu;
	CButton m_checkAccountFri;
	CButton m_checkAccountSat;
	CButton m_checkAccountSun;
	CDateTimeCtrl m_dtcAccount1;
	CDateTimeCtrl m_dtcAccount2;
	CEdit m_editDate4;
	CEdit m_editTime4;
	/*****/
	CStatic m_staticImport1;
	CStatic m_staticImport2;
	CStatic m_staticImport3;
	CStatic m_staticImport4;
	CStatic m_staticImport5;
	CStatic m_staticImport6;
	CStatic m_staticImport7;
	CStatic m_staticImport8;
	CStatic m_staticImport9;
	CButton m_checkImportMon;
	CButton m_checkImportTue;
	CButton m_checkImportWed;
	CButton m_checkImportThu;
	CButton m_checkImportFri;
	CButton m_checkImportSat;
	CButton m_checkImportSun;
	CEdit m_editDate1;
	CEdit m_editTime1;
	CButton m_checkExclude1Reqd;
	CButton m_checkExclude2Reqd;
	CButton m_checkExclude3Reqd;
	CButton m_checkExclude4Reqd;
	CDateTimeCtrl m_dtcTime5;
	CDateTimeCtrl m_dtcTime6;
	CDateTimeCtrl m_dtcTime7;
	CDateTimeCtrl m_dtcTime8;
	CDateTimeCtrl m_dtcTime9;
	CDateTimeCtrl m_dtcTime10;
	CDateTimeCtrl m_dtcTime11;
	CDateTimeCtrl m_dtcTime12;
	/*****/
	CStatic m_staticExports1;
	CStatic m_staticExports2;
	CStatic m_staticExports3;
	CStatic m_staticExports4;
	CStatic m_staticExports5;
	CStatic m_staticExports6;
	CDateTimeCtrl m_dtcTime3;
	CDateTimeCtrl m_dtcTime4;
	CDateTimeCtrl m_dtcTime13;
	CEdit m_editDate2;
	CEdit m_editDate3;
	CEdit m_editTime2;
	CEdit m_editTime3;
	CButton m_checkEODAfterPayments;
	/*****/
	CButton m_checkPromptBeforeAccess;
	CButton m_checkImportAccounts;
	CButton m_buttonOK;
	CButton m_buttonCancel;

	CDateTimeCtrl	m_dtcTime1;
	CDateTimeCtrl	m_dtcTime2;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnClickStaticLastBalance();
	afx_msg void OnClickStaticLastEndOfDay();
	afx_msg void OnToggleExclude();
	afx_msg void OnToggleExclude2();
	afx_msg void OnToggleExclude3();
	afx_msg void OnToggleExclude4();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();
	void SetTimeControl(CDateTimeCtrl& ctrl, CString strTime);

private:
	int m_nWebPaymentType;
	CSQLRepositoryWebPaymentOptions m_options;
};

//$$******************************************************************
