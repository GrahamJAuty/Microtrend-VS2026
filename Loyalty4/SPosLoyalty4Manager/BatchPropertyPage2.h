#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "BatchAccountBalanceMemory.h"
//$$******************************************************************

class CBatchPropertyPage2 : public CSSPropertyPage
{
public:
	CBatchPropertyPage2(CBatchAccountBalanceMemory& Memory);
	virtual ~CBatchPropertyPage2() {}
	bool HaveUpdates();
	bool UpdateRecord(CSQLRowAccountFull& RowAccount);

	enum { IDD = IDD_PROPPAGE_BATCH2 };
	CButton m_checkAddPurse1;
	CButton m_checkAddPurse2;
	CButton m_checkAddPoints;
	CButton m_checkAddPurse1SpendTD;
	CButton m_checkAddPurse2SpendTD;
	CButton m_checkAddCashSpendTD;
	CButton m_checkAddPointsTD;
	CButton m_checkClearPurse1;
	CButton m_checkClearPurse2;
	CButton m_checkClearPoints;
	CButton m_checkClearPurse1SpendTD;
	CButton m_checkClearPurse2SpendTD;
	CButton m_checkClearCashSpendTD;
	CButton m_checkClearPointsTD;

private:
	CEdit* GetEditPurse1();
	CEdit* GetEditPurse2();
	CEdit* GetEditPoints();
	CEdit* GetEditPurse1SpendTD();
	CEdit* GetEditPurse2SpendTD();
	CEdit* GetEditCashSpendTD();
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
	afx_msg void OnToggleAddPurse1();
	afx_msg void OnToggleAddPurse2();
	afx_msg void OnToggleAddPoints();
	afx_msg void OnToggleAddPurse1SpendTD();
	afx_msg void OnToggleAddPurse2SpendTD();
	afx_msg void OnToggleAddCashSpendTD();
	afx_msg void OnToggleAddPointsTD();
	afx_msg void OnToggleClearPurse1();
	afx_msg void OnToggleClearPurse2();
	afx_msg void OnToggleClearPoints();
	afx_msg void OnToggleClearPurse1SpendTD();
	afx_msg void OnToggleClearPurse2SpendTD();
	afx_msg void OnToggleClearCashSpendTD();
	afx_msg void OnToggleClearPointsTD();
	afx_msg void OnKillFocusEditPoints();
	
	DECLARE_MESSAGE_MAP()

private:
	CBatchAccountBalanceMemory& m_Memory;
};

//$$******************************************************************
