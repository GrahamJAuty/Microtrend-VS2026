#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\EmailAlertData.h"
//$$******************************************************************

class CEmailAlertPropertyPage4 : public CSSAutoShutdownPropertyPage
{
public:
	CEmailAlertPropertyPage4( CEmailAlertData& data );
	virtual ~CEmailAlertPropertyPage4();

	enum { IDD = IDD_PROPPAGE_EMAILALERT4 };

public:
	CSSComboBox m_comboEmailContactNo;
	CEdit m_editEmailSignatureText;
	/*****/
	CButton m_checkEnableEmail;
	CEdit m_editEmailText;
	CSSComboBox m_comboEmail;
	CSSComboBox m_comboStartOfWeek;
	CButton m_checkExcludeZeroPrice;
	CDateTimeCtrl m_DateTimePicker;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleEnable();
	afx_msg void OnStaticStartDate();
	DECLARE_MESSAGE_MAP()

private:
	CEmailAlertData& m_data;

private:
	bool m_bEnableStartDate;
};

//$$******************************************************************
