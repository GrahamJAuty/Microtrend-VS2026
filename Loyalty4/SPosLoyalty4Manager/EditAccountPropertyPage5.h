#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPOSLoyalty4Shared\SQLStampOfferCache.h"
//$$******************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPOSLoyalty4Shared\SQLTable_OfferStamp\SQLRowOfferStamp.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StampOfferInfo\SQLRowStampOfferInfo.h"
//$$******************************************************************

class CEditAccountPropertyPage5 : public CSSPropertyPage
{
public:
	CEditAccountPropertyPage5(CSQLRowAccountFull& Row);
	virtual ~CEditAccountPropertyPage5();
	
// Dialog Data
	enum { IDD = IDD_PROPPAGE_EDITACCOUNT5 };
	CSSListCtrlVirtual m_list;
	CSSTabbedComboBox m_comboOffer;
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
	afx_msg void OnSelectOffer();
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
	void SelectOffer(int nOfferID);
	void SelectRow(CSQLRowOfferStamp& Row);

private:
	CSQLRowAccountFull& m_SQLRowAccount;
	CArray<CSQLRowOfferStamp, CSQLRowOfferStamp> m_arrayStamps;
	CSQLStampOfferCache m_StampOfferCache;
};

//$$******************************************************************
