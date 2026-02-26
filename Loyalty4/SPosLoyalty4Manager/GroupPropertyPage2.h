#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
//$$******************************************************************

class CGroupPropertyPage2 : public CSSPropertyPage
{
public:
	CGroupPropertyPage2(CSQLRowGroup& RowGroup);
	virtual ~CGroupPropertyPage2();
	
	enum { IDD = IDD_PROPPAGE_GROUP2 };
	CButton m_checkLoyaltyGroup;
	CButton m_checkDisableLoyalty;
	CButton m_radioLoyaltyPoints;
	CButton m_radioLoyaltyValue;
	CButton m_checkGroupRedeem;
	CButton m_checkDisableRedeem;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	/****/
	afx_msg void OnToggleLoyaltyGroup();
	afx_msg void OnToggleLoyaltyDisable();
	afx_msg void OnRadioLoyaltyPoints();
	afx_msg void OnRadioLoyaltyValue();
	afx_msg void OnButtonDefaultSpend();
	afx_msg void OnToggleGroupRedeem();
	afx_msg void OnToggleDisableRedeem();
	afx_msg void OnButtonDefaultRedeem();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditLoyaltyTripCard();
	CEdit* GetEditLoyaltyPoints();
	CEdit* GetEditLoyaltyValue();
	CEdit* GetEditUpperSpendLimit();
	CEdit* GetEditMultiplierTrigger();
	CEdit* GetEditRedeemPointsTrip();
	CEdit* GetEditRedeemPointsValue();

private:
	void SetLoyaltyDisplay();
	void SetRedeemDisplay();

private:
	int m_nLoyaltyType;

private:
	CSQLRowGroup& m_RowGroup;
};

//$$******************************************************************
