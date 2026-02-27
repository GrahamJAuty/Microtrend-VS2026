//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "SQLRowSetAccountFind.h"
//$$******************************************************************

class CFindDlg : public CSSAutoShutdownDialog
{
public:
	CFindDlg( const char* szLeaversYear, CWnd* pParent );   // standard constructor

public:
	void SetListFilename(const char* szListFilename) { m_strListFilename = szListFilename; }

public:
	enum { IDD = IDD_FIND };
	CEdit m_editFindString;
	CSSTabbedComboBox m_tabcomboGroup;
	/*****/
	CButton m_checkUserName;
	CButton m_checkMemberID;
	CButton m_checkForename;
	CButton m_checkSurname;
	CButton m_checkInfo1;
	CButton m_checkInfo2;
	CButton m_checkInfo3;
	CButton m_checkInfo4;
	CButton m_checkInfo5;
	CButton m_checkInfo6;
	CButton m_checkInfo7;
	CButton m_checkInfo8;
	CButton m_buttonSearchAll;
	/*****/
	CButton m_checkValid;
	CButton m_checkExpired;
	CButton m_checkInactive;
	CButton m_checkAlert;
	CButton m_buttonIncludeAll;
	/*****/
	CSSListCtrlVirtual m_list;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	/*****/
	afx_msg void OnButtonFind();
	/*****/
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonClear();
	/*****/
	afx_msg void OnButtonIncludeAll();
	afx_msg void OnButtonSearchAll();
	/*****/
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPopupEdit();
	afx_msg void OnPopupPurchasehistory();
	afx_msg void OnPopupStatement();
	afx_msg void OnPopupAudit();
	afx_msg void OnPopupAuditSummary();
	afx_msg void OnPopupGroupShift();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void ReloadSelectedRow();

public:
	__int64 m_nRequestedUserID;

private:
	void SetTitle();
	void DisplayListFile();

private:
	__int64 GetSelectedUserID();
	bool HaveSearchField();
	bool HaveIncludeField();
	void HandleClose();

private:
	CString m_strFindLabel;

private:
	CString m_strLeaversYear;
	CString m_strListFilename;
	CString m_strTitle;

private:
	CString m_strFileTitle;
	CString m_strMatchString;
	int m_nSingleGroupNo;
	bool m_bSearchAll;
	bool m_bIncludeAll;

private:
	CSQLRowSetAccountFind m_RowSetFind;
};

//$$******************************************************************
