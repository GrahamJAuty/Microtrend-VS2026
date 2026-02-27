#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage7 : public CSSAutoShutdownPropertyPage
{
public:
	CSystemPropertyPage7();
	bool RestartServerReqd() { return m_bRestartServerReqd; }
	
	enum { IDD = IDD_PROPPAGE_SYSTEM7 };
	CSSTabbedComboBox m_comboWebPayment;
	CButton m_buttonSetupWebPayment;
	CButton m_buttonSetupSchedule;
	/*****/
	CButton m_checkEnableChartwells;
	/*****/
	CButton m_buttonSetupCSVImport;
	CButton m_checkTrimZeroExtRef1;
	/*****/
	CButton m_checkEnableAutoImport;
	CButton m_buttonSetupAutoImport;
	/*****/
	CButton m_checkEnableSimsImport;
	CButton m_buttonSetupSimsImport;
	/*****/
	CButton m_checkEnableWonde;
	CButton m_buttonSetupWonde;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnSelectWebPaymentType();
	afx_msg void OnButtonSetupWebPayment();
	afx_msg void OnButtonSetupSchedule();
	/*****/
	afx_msg void OnButtonSetupCSVImport();
	/*****/
	afx_msg void OnToggleAutoImport();
	afx_msg void OnButtonSetupAutoImport();
	/*****/
	afx_msg void OnToggleSimsImport();
	afx_msg void OnButtonSetupSims();
	/*****/
	afx_msg void OnToggleWonde();
	afx_msg void OnButtonSetupWonde();
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();

private:
	bool m_bRestartServerReqd;
	int m_nCurrentWebPayment;
};

//$$******************************************************************
