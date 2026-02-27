#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\EmailAlertData.h"
//$$******************************************************************

class CEmailAlertPropertyPage3 : public CSSAutoShutdownPropertyPage
{
public:
	CEmailAlertPropertyPage3( CEmailAlertData& data );
	virtual ~CEmailAlertPropertyPage3();

	enum { IDD = IDD_PROPPAGE_EMAILALERT3 };

public:
	CSSComboBox m_comboEmailContactNo;
	CEdit m_editEmailSignatureText;
	/*****/
	CButton m_checkEnableEmail;
	CEdit m_editEmailText;
	CSSComboBox m_comboEmail;
	CSSComboBox m_comboStartOfWeek;
	CButton m_checkExcludeNoChange;

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
