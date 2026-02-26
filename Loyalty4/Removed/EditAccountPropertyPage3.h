#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\FamilyDatabase.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************

class CEditAccountPropertyPage3 : public CSSPropertyPage
{
public:
	CEditAccountPropertyPage3(CSQLRowAccountFull& RowAccount);
	virtual ~CEditAccountPropertyPage3();
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_EDITACCOUNT3 };
	CSSListCtrlVirtual m_list;
	CButton m_buttonPrint;
	CButton m_buttonAdd;
	CButton m_buttonEdit;
	CButton m_buttonDelete;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonPrint();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	void DisplayScreen();

private:
	CSQLRowAccountFull& m_RowAccount;
	CFamilyDatabase m_dbFamily;
};

//$$******************************************************************
