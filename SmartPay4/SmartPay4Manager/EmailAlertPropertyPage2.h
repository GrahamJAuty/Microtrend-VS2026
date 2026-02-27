#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\EmailAlertData.h"
//$$******************************************************************

class CEmailAlertPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CEmailAlertPropertyPage2(CEmailAlertData& data);
	virtual ~CEmailAlertPropertyPage2();

	enum { IDD = IDD_PROPPAGE_EMAILALERT2 };

public:
	CSSComboBox m_comboEmailContactNo;
	CEdit m_editEmailSignatureText;
	/*****/
	CButton m_checkEnableEmail;
	CEdit m_editEmailText;
	CSSComboBox m_comboEmail;

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
