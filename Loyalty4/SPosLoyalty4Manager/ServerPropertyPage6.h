#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage6 : public CSSPropertyPage
{
public:
	CServerPropertyPage6();
	
	enum { IDD = IDD_PROPPAGE_SERVER6 };
	int m_nRedeemPointsTrip;
	double m_dRedeemPointsValue;
	CButton m_radioCash;
	CButton m_radioPromo;
	CEdit m_editComment;
	CButton m_checkDisableAutoRedeem;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditRedeemPointsValue();
};

//$$******************************************************************

