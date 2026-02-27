#pragma once
//******************************************************************
#include "..\SmartPay4Manager\SQLRowSetAccountList.h"
//******************************************************************
#include "SmartPay4Manager.h"
//******************************************************************

class CSurnameListDlg : public CSSAutoShutdownDialog
{
public:
	CSurnameListDlg ( CWnd* pParent = NULL);   // standard constructor
	virtual ~CSurnameListDlg();

// Dialog Data
	enum { IDD = IDD_SURNAMELIST };
	CButton m_checkSortByGroup;
	CButton m_buttonOK;
	CButton m_buttonCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDestroy();
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopupPurchasehistory();
	afx_msg void OnPopupStatement();
	afx_msg void OnPopupAudit();
	afx_msg void OnPopupAuditSummary();
	afx_msg void OnPopupEdit();
	afx_msg void OnPopupGroupShift();
	afx_msg void OnToggleSortByGroup();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

private:
	void CSSAutoShutdownPreInitDialog(); 
	BOOL CSSAutoShutdownPostInitDialog();
	void CSSAutoShutdownLogOff();

public:
	__int64 m_nRequestedUserID;

private:
	void SaveSettings();
	void EditRecord();
	__int64 GetSelectedUserID();

private:
	CSSListCtrlVirtual m_list;

	int m_nUserColumnIndex;
	CString m_strFindLabel;

	bool m_bInitDialog;

private:
	CSQLRowSetAccountList m_RowSetList;
};

//******************************************************************
