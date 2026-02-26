#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "PasswordData.h"
//$$******************************************************************

class CPasswordOptionsDlg : public CSSDialog
{
public:
	CPasswordOptionsDlg (int nPasswordIndex, CPasswordData* pPasswords, bool bHaveReader, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPasswordOptionsDlg();

	enum { IDD = IDD_PASSWORD_OPTIONS };
	CString m_strCardNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnToggleEditRecord();
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editPassword;
	CEdit m_editUserName;
	CStatic m_staticCard;
	CEdit m_editCard;
	/*****/
	CButton m_checkAddRecord;
	CButton m_checkEditRecord;
	CButton m_checkEditGeneral;
	CButton m_checkEditPurse;
	CButton m_checkEditPoints;
	CButton m_checkEditCashSpend;
	CButton m_checkEnableButtonPurse1Topup;
	CButton m_checkEnableButtonPurse1Spend;
	CButton m_checkEnableButtonPointsTopup;
	CButton m_checkEnableButtonPointsRedeem;
	CButton m_checkEnableButtonManualBonus;
	CButton m_checkTransferRecord;
	CButton m_checkHotlistRecord;
	CButton m_checkDeleteRecord;
	CButton m_checkImport;
	CButton m_checkExport;
	CButton m_checkBatch;
	CButton m_checkReports;
	CButton m_checkSetupSystem;
	CButton m_checkSetupServer;
	CButton m_checkInstallServer;
	CButton m_checkEditExternalAccountLink;
	
public:
	bool m_bHaveReader;

private:
	CPasswordData* m_pPasswords;
	int m_nPasswordIndex;
};

//$$******************************************************************

