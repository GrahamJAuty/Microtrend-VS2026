#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "SQLRowSetAccountFind.h"
//$$******************************************************************

class CFindDlg : public CSSDialog
{
public:
	CFindDlg( CWnd* pParent = NULL);  
	virtual ~CFindDlg();

	void SetTitle(CString strTitle) { m_strTitle = strTitle; }

	enum { IDD = IDD_FIND };
	CEdit m_editFindString;
	CSSTabbedComboBox m_comboGroup;
	/*****/
	CButton m_checkCardName;
	CButton m_checkForename;
	CButton m_checkSurname;
	CButton m_checkAddress;
	CButton m_checkPostcode;
	CButton m_checkInformation;
	CButton m_checkTelephone;
	CButton m_buttonSearchAll;
	/*****/
	CButton m_checkValid;
	CButton m_checkExpired;
	CButton m_checkHotlisted;
	CButton m_checkAlertSet;
	CButton m_checkInactive;
	CButton m_buttonIncludeAll;
	/*****/
	CSSListCtrlVirtual m_list;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	/*****/
	afx_msg void OnButtonFind();
	/*****/
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnButtonClear();
	/*****/
	afx_msg void OnButtonIncludeAll();
	afx_msg void OnButtonSearchAll();
	/*****/
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPopupEdit();
	afx_msg void OnPopupPurchaseHistory();
	afx_msg void OnPopupPhotoID();
	DECLARE_MESSAGE_MAP()

public:
	__int64 m_nRequestedCardNo;

private:
	bool HaveSearchField();
	bool HaveIncludeField();
	void HandleClose();
	void SetTitle();
	__int64 GetSelectedUserID();
	
private:
	CString m_strFindLabel;
	CString m_strMatchString;
	int m_nSingleGroupNo;
	bool m_bSearchAll;
	bool m_bIncludeAll;
	CString m_strTitle;

private:
	CSQLRowSetAccountFind m_RowSetFind;
};

//$$******************************************************************
