#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CSystemPropertyPage2 : public CSSPropertyPage
{
public:
	CSystemPropertyPage2();
	virtual ~CSystemPropertyPage2();
	
	enum { IDD = IDD_PROPPAGE_SYSTEM2 };
	CSSComboBox m_comboEditSelectPage;
	CSSComboBox m_comboEditRefresh;
	CButton m_checkShowButtonPurse1Topup;
	CButton m_checkShowButtonPurse1Spend;
	CButton m_checkShowButtonPurse2Topup;
	CButton m_checkShowButtonPointsTopup;
	CButton m_checkShowButtonPointsRedeem;
	CButton m_checkShowButtonManualBonus;
	CButton m_checkColour;
	CButton m_checkAccountPreload;
	CButton m_checkCancelPrompt;
	/*****/
	CButton m_checkEditPurse1;
	CButton m_checkEditPurse2;
	CButton m_checkEditPoints;
	CButton m_checkEditCashSpend;
	/*****/
	CButton m_checkEnableBatchUpdates;
	CButton m_checkBatchUpdatePoints;
	CButton m_checkBatchUpdatePurse1;
	CButton m_checkBatchUpdatePurse2;
	CButton m_checkBatchUpdateSpendTD;
	/*****/
	CButton m_checkContextMenuPurse1Topup;
	CButton m_checkContextMenuPurse2Topup;
	/*****/
	CButton m_checkFinderValid;
	CButton m_checkFinderExpired;
	CButton m_checkFinderHotlisted;
	CButton m_checkFinderAlertSet;
	CButton m_checkFinderInactive;
	CButton m_checkFinderUsername;
	CButton m_checkFinderForename;
	CButton m_checkFinderSurname;
	CButton m_checkFinderAddress;
	CButton m_checkFinderPostcode;
	CButton m_checkFinderPhone;
	CButton m_checkFinderInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleBatchUpdates();
	DECLARE_MESSAGE_MAP()

private:
	void SetDisplay();

public:
	virtual BOOL OnSetActive();
};

//$$******************************************************************
