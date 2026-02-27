#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLPurchaseControlCache.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRowPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRowPurchaseControlRule.h"
//$$******************************************************************

class CEditRecordPropertyPagePurchaseControl : public CSSPropertyPage
{
public:
	CEditRecordPropertyPagePurchaseControl(CSQLRowAccountFull& Row);
	virtual ~CEditRecordPropertyPagePurchaseControl();
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_EDITRECORD_PURCHASECONTROL };
	CSSListCtrlVirtual m_list;
	CSSTabbedComboBox m_comboRule;
	CButton m_buttonAdd;
	CButton m_buttonEdit;
	CButton m_buttonDelete;
	CButton m_buttonModifyExpired;
	CButton m_buttonDeleteExpired;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectRule();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonDeleteExpired();
	afx_msg void OnButtonModifyExpired();
	virtual void OnOK();
	virtual BOOL OnQueryCancel();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	CString FormatDate(CString strDate, bool bIsExpiryDate);
	int GetSafeSelection();
	void SelectLine(int nSel);
	void SelectRule(int nRuleID);
	void SelectRow(CSQLRowPurchaseControlStamp& Row);

private:
	CSQLRowAccountFull& m_SQLRowAccount;
	CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp> m_arrayStamps;
	CSQLPurchaseControlCache m_PurchaseControlCache;
};

//$$******************************************************************
