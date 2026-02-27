#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************

class CGroupPropertyPage6 : public CSSAutoShutdownPropertyPage
{
public:
	CGroupPropertyPage6(CSQLRowGroupFull& RowGroup);
	virtual ~CGroupPropertyPage6();
	
	enum { IDD = IDD_PROPPAGE_GROUP6 };
	CButton m_checkUseGroupRedeem;
	CButton m_checkDisableRedeem;
	CSSComboBox m_comboAccount1Purse;
	CButton m_buttonDefault1;
	int m_nRedeemTrip;
	double m_dRedeemValue;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleGroupRedeem();
	afx_msg void OnToggleDisableRedeem();
	afx_msg void OnButtonDefault1();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditRedeemPointsTrip();
	CEdit* GetEditRedeemValue();

private:
	void SetDisplay();

private:
	CSQLRowGroupFull& m_RowGroup;
};

//$$******************************************************************
