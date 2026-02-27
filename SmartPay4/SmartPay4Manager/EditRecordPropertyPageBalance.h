#pragma once
//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\RedeemPoints.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRowGroupFull.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
//$$******************************************************************
#include "EditAccountBuffer.h"
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

struct CAccountEditEnables
{
public:
	CAccountEditEnables()
	{
		m_bSystemEditMaxSpend = FALSE;
		m_bAccountEditMaxSpend = FALSE;
		m_bEditOverdraft = FALSE;
		m_bShowMaxSpend = FALSE;
		m_bShowOverdraft = FALSE;
		m_bEditPurses = FALSE;
		m_bEditPurse1 = FALSE;
		m_bEditPurse2 = FALSE;
		m_bEditPurse3 = FALSE;
		m_bEditSpendTD = FALSE;
		m_bShowP1CashButton = FALSE;
		m_bShowP2CashButton = FALSE;
		m_bShowP3CashButton = FALSE;
		m_bMaxSpendFromItemP1 = FALSE;
		m_bMaxSpendFromItemP3 = FALSE;
		m_bMaxSpendFromGroupP1 = FALSE;
		m_bMaxSpendFromGroupP3 = FALSE;
		m_bShowPurse2 = FALSE;
		m_bShowPurse2Refresh = FALSE;
		m_bEditPoints = FALSE;
		m_bShowPointsRedeemButton = FALSE;
		m_bShowPointsTopupButton = TRUE;
	};

public:
	bool m_bSystemEditMaxSpend;		//FLAG IF ANYTHING AT SYSTEM LEVEL IS BLOCKING EDIT OF MAX SPEND
	bool m_bAccountEditMaxSpend;	//FLAG IF CURRENT ACCOUNT SETTINGS ARE BLOCKING EDIT OF MAX SPEND
	bool m_bEditOverdraft;
	bool m_bShowMaxSpend;
	bool m_bShowOverdraft;
	bool m_bEditPurses;
	bool m_bEditPurse1;
	bool m_bEditPurse2;
	bool m_bEditPurse3;
	bool m_bEditSpendTD;
	bool m_bShowP1CashButton;
	bool m_bShowP2CashButton;
	bool m_bShowP3CashButton;
	bool m_bMaxSpendFromItemP1;
	bool m_bMaxSpendFromItemP3;
	bool m_bMaxSpendFromGroupP1;
	bool m_bMaxSpendFromGroupP3;
	bool m_bShowPurse2;
	bool m_bShowPurse2Refresh;
	bool m_bEditPoints;
	bool m_bShowPointsRedeemButton;
	bool m_bShowPointsTopupButton;
};

//$$******************************************************************

class CEditRecordPropertyPageBalance : public CSSAutoShutdownPropertyPage
{
public:
    CEditRecordPropertyPageBalance(CSQLRowAccountFull& Row);
    virtual ~CEditRecordPropertyPageBalance();

    void SetPointer ( CEditAccountBuffer* pEditAccountBuffer, int nMode, CLeaversHandler* pLeaversData = NULL);

public:
    enum { IDD = IDD_PROPPAGE_EDITRECORD_BALANCE };
    CButton m_buttonPurse1Topup;
    CButton m_buttonPebble;
    CButton m_buttonPurse2Topup;
    CButton m_buttonPurse3Topup;
    CEdit m_editPurse1UsedTime;
    CEdit m_editPurse2UsedTime;
    CEdit m_editPurse3UsedTime;
    CEdit m_editPurse2RefreshTime; 
    CStatic m_staticRefresh;
    CButton m_buttonPointsTopup;
    CButton m_buttonPointsRedeem;
    CStatic m_staticPurse1MaxSpend;
    CStatic m_staticPurse1Overdraft;
    CStatic m_staticPurse1LabelLiability;
    CStatic m_staticPurse1LabelCredit;
    CStatic m_staticPurse1LabelTotal;
    CStatic m_staticPurse3MaxSpend;
    CStatic m_staticPurse3Overdraft;
    CStatic m_staticPurse3Label1;
    CStatic m_staticPurse3Label2;
    CStatic m_staticPurse3Label3;
    CStatic m_staticPurse3Label4;
    CStatic m_staticPurse3LabelLiability;
    CStatic m_staticPurse3LabelCredit;
    CStatic m_staticPurse3LabelTotal;
    CStatic m_staticPurse3Label8;
    CStatic m_staticPurse3Label9;
    CStatic m_staticPurse3Label10;
    CStatic m_staticPointsLabel1;
    CStatic m_staticPointsLabel2;
    CStatic m_staticPointsLabel3;
    CStatic m_staticPointsLabel4;
    CStatic m_staticGeneralLabel1;
    CStatic m_staticGeneralLabel2;
    CStatic m_staticGeneralLabel3;
    CStatic m_staticAuditOff;

public:
    void UpdateAudit(bool bCancel);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnButtonPurse1Topup();
    afx_msg void OnButtonPurse2Topup();
    afx_msg void OnButtonPurse3Topup();
    afx_msg void OnButtonPointsTopup();
    afx_msg void OnButtonPointsRedeem();
    afx_msg void OnButtonPebble();
    afx_msg void OnKillFocusPurse();
    afx_msg void OnKillFocusPoints();
    virtual BOOL OnSetActive();
    virtual BOOL OnKillActive();
    afx_msg void OnButtonAltKey1();
    afx_msg void OnDoubleClickStaticBalance();
    virtual void OnOK();
    virtual BOOL OnQueryCancel();
    virtual void OnCancel();
    DECLARE_MESSAGE_MAP()

private:
    CEdit* GetEditPurse1Liability();
    CEdit* GetEditPurse1Credit();
    CEdit* GetEditPurse1Total();
    CEdit* GetEditPurse1Overdraft();
    CEdit* GetEditPurse1MaxSpend();
    CEdit* GetEditPurse1STD();
    CEdit* GetEditPurse1UsedDate();
    CEdit* GetEditPurse1UsedValue();
    
    CEdit* GetEditPurse2Balance();
    CEdit* GetEditPurse2STD();
    CEdit* GetEditPurse2UsedDate();
    CEdit* GetEditPurse2UsedValue();
    CEdit* GetEditPurse2RefreshDate();
    CEdit* GetEditPurse2RefreshValue();

    CEdit* GetEditPurse3Liability();
    CEdit* GetEditPurse3Credit();
    CEdit* GetEditPurse3Total();
    CEdit* GetEditPurse3Overdraft();
    CEdit* GetEditPurse3MaxSpend();
    CEdit* GetEditPurse3STD();
    CEdit* GetEditPurse3UsedDate();
    CEdit* GetEditPurse3UsedValue();

    CEdit* GetEditPointsBalance();
    CEdit* GetEditPointsValue();
    CEdit* GetEditPointsToDate();
    CEdit* GetEditGeneralBalance();
    CEdit* GetEditGeneralCashSpend();

private:
    void SetEditBoxDouble(CEdit* pEdit, double d);
    double GetEditBoxDouble(CEdit* pEdit);
    double AddToEditBoxDouble(CEdit* pEdit, double dValue);
    int AddToEditBoxInt(CEdit* pEdit, int nValue);
    
private:
    void SetEditEnablesSystem();
    void SetEditEnablesGroup();
    void RemoveUnusedControls();

private:
    void SetOneOffFields();
    void SetMaxSpendFields();
    void SetGroupFields();
    int SetPointsValue();
    void SetAccountBalance();
    void SetTabStops();
    double GetRefreshValue();
    void HandlePurse1TopupButton(bool bPebbleAdjust);

private:
    bool ValidateMaxSpend(CEdit* pEdit, double dMin, double dMax, CString strType);
    
private:
    bool IsHotlisted();

private:
    CSQLRowAccountFull& m_SQLRowAccount;
    CSQLRowGroupFull m_SQLRowGroup;
    CLeaversHandler* m_pLeaversData;
    CEditAccountBuffer* m_pEditAccountBuffer;
    CRedeemPoints m_pointsRedeemer;        
    CSQLAuditRecord m_atc1;
    CSQLAuditRecord m_atc2;
    
private:
    CAccountEditEnables m_editEnables;
    CStringArray m_arrayAuditBuffer;
    int m_nMode;
    bool m_bAuditTurnedOff;
    CString m_strSessionAuditFilename;
    double m_dPendingPebbleAdjust;
    bool m_bDonePebbleAdjust;
    bool m_bDonePurse1Topup;
}; 

//$$******************************************************************
