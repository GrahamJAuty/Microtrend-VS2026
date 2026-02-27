#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
//*******************************************************************

class CSetupPasswordOptionsDlg : public CSSAutoShutdownDialog
{
public:
	CSetupPasswordOptionsDlg(int nPasswordNo, CPasswordData* pPasswords, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetupPasswordOptionsDlg();

// Dialog Data
	enum { IDD = IDD_SETUP_PASSWORDOPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	afx_msg void OnToggleModifyRecord();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

	virtual void CSSAutoShutdownLogOff()
	{
		TRACE("Auto Shutdown LogOff Setup Password Options\n");
		EndDialog(IDCANCEL);
	}

public:
	CEdit m_editUserName;
	CEdit m_editPassword;
	CButton m_checkAddRecord;
	CButton m_checkModifyRecord;
	CButton m_checkEditGeneral;
	CButton m_checkEditPurse;
	CButton m_checkEditPoints;
	CButton m_checkEditExternal;
	CButton m_checkEnableTopup;
	CButton m_checkEnablePoints;
	CButton m_checkTransferRecord;
	CButton m_checkDeleteRecord;
	CButton m_checkBatch;
	CButton m_checkImport;
	CButton m_checkExport;
	CButton m_checkReports;
	CButton m_checkSetupSystem;
	CButton m_checkSetupServer;
	CButton m_checkInstallServer;
	CButton m_checkSetupBgnd;
	CButton m_checkInstallBgnd;

private:
	CPasswordData* m_pPasswords;
	int m_nPasswordNo;
};

//*******************************************************************
