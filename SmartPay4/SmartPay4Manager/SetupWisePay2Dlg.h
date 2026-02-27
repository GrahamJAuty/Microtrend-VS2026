#pragma once
//********************************************************************
#include "resource.h"
//********************************************************************
#include "..\SmartPay4ManagerBgnd\WisePayData.h"
//********************************************************************

class CSetupWisePay2Dlg : public CSSAutoShutdownDialog
{
public:
	CSetupWisePay2Dlg( CWisePayData* pData, CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_SETUP_WISEPAY2 };
	CEdit m_editCardNo;
	CStatic m_staticAccountID;
	BOOL m_bSaveEmail;
	CSSComboBox m_comboEmail;
	int m_nEmailInfoIndex;
	CSSComboBox m_comboYear;
	int m_nYearInfoIndex;
	CSSComboBox m_comboReg;
	int m_nRegInfoIndex;
	CSSComboBox m_comboDob;
	int m_nDobInfoIndex;
	int m_nNewIntakeAccount;
	CSSComboBox m_comboNewIntake;
	BOOL m_bAddUnknowns;
	CString m_strBalanceText;
	CString m_strStartFromUserID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnToggleAddUnknowns();
	afx_msg void OnSelectNewIntake();
	afx_msg void OnToggleSaveEmail();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void SetDisplay();

private:
	CWisePayData* m_pData;
};

//********************************************************************
