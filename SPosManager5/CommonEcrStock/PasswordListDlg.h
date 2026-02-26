#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "Password.h"
/**********************************************************************/

class CPasswordListDlg : public CSSDialog
{
public:
	CPasswordListDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CPasswordListDlg)
	enum { IDD = IDD_PASSWORD_LIST };
	CButton	m_buttonDeleteLog;
	CButton	m_buttonLog;
	CEdit	m_editMasterPassword;
	CString	m_strMasterPassword;
	BOOL	m_bLogPasswords;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPasswordListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPasswordListDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDblclkListPwtext();
	afx_msg void OnButtonSettings();
	afx_msg void OnButtonLog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDelete();
	virtual void OnCancel();
	afx_msg void OnCheckLog();
	afx_msg void OnButtonDeleteLog();
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonClone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillListBox();

private:
	CSSListCtrl m_listPasswords;
	bool m_bLogExists;
};

/**********************************************************************/
#endif
/**********************************************************************/
