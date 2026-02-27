#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************
#define nCARDLENGTHS_COUNTER		2
//$$******************************************************************

class CServerPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CServerPropertyPage1();
	virtual ~CServerPropertyPage1();
	
	enum { IDD = IDD_PROPPAGE_SERVER1 };
	CButton m_checkLogAction2;
	/*****/
	CEdit m_editEposRequestPath;
	CButton m_checkAllowOverdraft;
	CButton m_checkAddUnknowns;
	CEdit m_editOfflineSpendID;
	CSSTabbedComboBox m_tabcomboGroup;
	CSSTabbedComboBox m_comboForename;
	/*****/
	CEdit m_editEposLogPath;
	CButton m_checkNewBalance;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/*****/
	afx_msg void OnButtonFolders();
	/*****/
	afx_msg void OnToggleAddUnknowns();
	afx_msg void OnButtonBrowseLoyReq();
	afx_msg void OnButtonTestLoyReq();
	/*****/
	afx_msg void OnButtonBrowseLoyLog();
	afx_msg void OnButtonTestLoyLog();
	/*****/
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditOfflineSpendLimit();
	int ExtractGroupNo();
	
private:
	bool m_bHadServer;
};

//$$******************************************************************
