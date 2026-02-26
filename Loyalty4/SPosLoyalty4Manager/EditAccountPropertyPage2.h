#pragma once
//$$******************************************************************
#include "..\SPosLoyalty4Shared\RedeemPoints.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroup.h"
//$$******************************************************************
#include "AuditRecord.h"
#include "EditAccountBuffer.h"
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CEditAccountPropertyPage2 : public CSSPropertyPage
{
public:
	CEditAccountPropertyPage2(CSQLRowAccountFull& RowAccount);
	virtual ~CEditAccountPropertyPage2();

	void SetPointer(CEditAccountBuffer* pEditAccountBuffer);

	void UpdateAudit();

public:
	enum { IDD = IDD_PROPPAGE_EDITACCOUNT2 };
	CButton m_buttonPurse1Topup;
	CButton m_buttonPurse1Spend;
	CEdit m_editPurse1UsedTime;
	CEdit m_editPurse2UsedTime;
	CEdit m_editCashUsedTime;
	CEdit m_editPurse2RefreshTime;
	CStatic m_staticRefresh;
	CButton m_buttonPointsTopup;
	CButton m_buttonPointsRedeem;
	CButton m_buttonManualBonus;
	CButton m_buttonRefreshNow;
	CStatic m_staticBalance;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPurse1Topup();
	afx_msg void OnButtonPurse1Spend();
	afx_msg void OnButtonPointsTopup();
	afx_msg void OnButtonPointsRedeem();
	afx_msg void OnButtonManualBonus();
	afx_msg void OnButtonRefreshNow();
	afx_msg void OnKillFocusPurse();
	afx_msg void OnKillFocusPoints();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	virtual BOOL OnQueryCancel();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	bool ValidateMaxSpend(CEdit* pEdit, double dMin, double dMax, CString strType);

private:
	CEdit* GetEditPurse1Balance();
	CEdit* GetEditPurse1STD();
	CEdit* GetEditPurse1UsedDate();
	CEdit* GetEditPurse1UsedValue();
	CEdit* GetEditPurse2Balance();
	CEdit* GetEditPurse2STD();
	CEdit* GetEditPurse2UsedDate();
	CEdit* GetEditPurse2UsedValue();
	CEdit* GetEditPurse2RefreshDate();
	CEdit* GetEditPurse2RefreshValue();
	CEdit* GetEditPointsBalance();
	CEdit* GetEditPointsValue();
	CEdit* GetEditPointsToDate();
	CEdit* GetEditGeneralBalance();
	CEdit* GetEditGeneralOverdraft();
	CEdit* GetEditGeneralMaxSpend();
	CEdit* GetEditGeneralCashUsedDate();
	CEdit* GetEditGeneralCashUsedValue();
	CEdit* GetEditGeneralCashSpend();

private:
	void SetEditBoxDouble(CEdit* pEdit, double d);
	double GetEditBoxDouble(CEdit* pEdit);
	double AddToEditBoxDouble(CEdit* pEdit, double dValue);
	int AddToEditBoxInt(CEdit* pEdit, int nValue);
	void SetTabStops();

private:
	void ApplyGroupSettings();
	void CalculateTopupBonus(bool bPurse2, double dCurrentBalance, double dCashAdded);
	void CalculateSpendBonus(double dCurrentBalance, double dSpend);
	int SetPointsValue();
	void SetAccountBalance();
	void RefreshPurse2();
	void TopupPurse2();

private:
	bool IsHotlisted();

private:
	CEditAccountBuffer* m_pEditAccountBuffer;
	CRedeemPoints m_pointsRedeemer;
	CAuditRecord m_atc1;
	CAuditRecord m_atc2;

private:
	CStringArray m_arrayAuditBuffer;

private:
	CSQLRowAccountFull& m_RowAccount;
	CSQLRowGroup m_RowGroup;
	bool m_bGroupExists;
	int m_nLastGroupNo;
	bool m_bCanTopupPurse2;
};

//$$******************************************************************
