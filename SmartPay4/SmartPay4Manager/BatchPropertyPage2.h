#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "afxwin.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "BatchAccountBalanceMemory.h"
//$$******************************************************************

class CBatchPropertyPage2 : public CSSAutoShutdownPropertyPage
{
public:
	CBatchPropertyPage2(CBatchAccountBalanceMemory& BalanceMemory);
	virtual ~CBatchPropertyPage2() {}
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);
	void AddWarnings ( CStringArray* pArray );

	enum { IDD = IDD_PROPPAGE_BATCH2 };
	CButton m_checkAddPurse1Liability;
	CButton m_checkAddPurse1Credit;
	CButton m_checkAddPurse3Liability;
	CButton m_checkAddPurse3Credit;
	CButton m_checkAddPoints;
	CButton m_checkAddPurse1SpendTD;
	CButton m_checkAddPurse2SpendTD;
	CButton m_checkAddPurse3SpendTD;
	CButton m_checkAddPointsTD;
	CButton m_checkClearPurse1Liability;
	CButton m_checkClearPurse1Credit;
	CButton m_checkClearPurse3Liability;
	CButton m_checkClearPurse3Credit;
	CButton m_checkClearPoints;
	CButton m_checkClearPurse1SpendTD;
	CButton m_checkClearPurse2SpendTD;
	CButton m_checkClearPurse3SpendTD;
	CButton m_checkClearCashSpendTD;
	CButton m_checkClearPointsTD;

private:
	CEdit* GetEditPurse1Liability();
	CEdit* GetEditPurse1Credit();
	CEdit* GetEditPurse3Liability();
	CEdit* GetEditPurse3Credit();
	CEdit* GetEditPoints();
	CEdit* GetEditPurse1SpendTD();
	CEdit* GetEditPurse2SpendTD();
	CEdit* GetEditPurse3SpendTD();
	CEdit* GetEditPointsTD();

private:
	void UpdateMemory();
	void SetEditDouble( CEdit* pEdit, double d );
	void SetEditInt( CEdit* pEdit, int n );
	double GetEditDouble( CEdit* pEdit );
	int GetEditInt( CEdit* pEdit );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleAddPurse1Liability();
	afx_msg void OnToggleAddPurse1Credit();
	afx_msg void OnToggleAddPurse3Liability();
	afx_msg void OnToggleAddPurse3Credit();
	afx_msg void OnToggleAddPoints();
	afx_msg void OnToggleAddPurse1SpendTD();
	afx_msg void OnToggleAddPurse2SpendTD();
	afx_msg void OnToggleAddPurse3SpendTD();
	afx_msg void OnToggleAddPointsTD();
	afx_msg void OnToggleClearPurse1Liability();
	afx_msg void OnToggleClearPurse1Credit();
	afx_msg void OnToggleClearPurse3Liability();
	afx_msg void OnToggleClearPurse3Credit();
	afx_msg void OnToggleClearPoints();
	afx_msg void OnToggleClearPurse1SpendTD();
	afx_msg void OnToggleClearPurse2SpendTD();
	afx_msg void OnToggleClearPurse3SpendTD();
	afx_msg void OnToggleClearPointsTD();
	afx_msg void OnKillFocusEditPoints();
	
	DECLARE_MESSAGE_MAP()

private:
	CBatchAccountBalanceMemory& m_Memory;
};

//$$******************************************************************
