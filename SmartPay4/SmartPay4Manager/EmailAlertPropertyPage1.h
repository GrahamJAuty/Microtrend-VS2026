#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\EmailAlertData.h"
//$$******************************************************************

class CEmailAlertPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CEmailAlertPropertyPage1( CEmailAlertData& data );
	virtual ~CEmailAlertPropertyPage1();

	enum { IDD = IDD_PROPPAGE_EMAILALERT1 };

public:
	CSSComboBox m_comboEmailContactNo;
	CEdit m_editEmailSignatureText;
	/*****/
	CButton m_checkEnableEmail;
	CEdit m_editEmailText;
	CSSComboBox m_comboEmail;
	CButton m_checkPurse1;
	CButton m_checkPurse2;
	CButton m_checkPurse3;
	CButton m_checkShowPurse1;
	CButton m_checkShowPurse2;
	CButton m_checkShowPurse3;
	CButton m_checkShowPoints;
	CButton m_checkPoundPurse1;
	CButton m_checkPoundPurse2;
	CButton m_checkPoundPurse3;
	CEdit m_editBalance1;
	CEdit m_editBalance2;
	CEdit m_editBalance3;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleEnable();
	DECLARE_MESSAGE_MAP()

private:
	CEmailAlertData& m_data;
};

//$$******************************************************************
