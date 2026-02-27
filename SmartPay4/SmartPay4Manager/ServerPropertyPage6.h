#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//$$******************************************************************

class CServerPropertyPage6 : public CSSAutoShutdownPropertyPage
{
public:
	CServerPropertyPage6();

	enum { IDD = IDD_PROPPAGE_SERVER6 };
	CSSComboBox m_comboPurse;
	CButton m_checkRedeemDisableAuto;
	CEdit m_editRedeemComment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();

private:
	CEdit* GetEditRedeemPointsTrip();
	CEdit* GetEditRedeemValue();

	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************
