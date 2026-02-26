#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "PasswordData.h"
/**********************************************************************/

class CPasswordListDlg : public CSSDialog
{
public:
	CPasswordListDlg( CPasswordData* pPasswords, bool bHaveReader, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPasswordListDlg)
	enum { IDD = IDD_PASSWORD_LIST };
	CButton	m_buttonShowLog;
	CEdit	m_editMasterPassword;
	CStatic m_staticCard;
	CEdit m_editCard;
	BOOL	m_bLogPasswords;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPasswordListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPasswordListDlg)
	afx_msg void OnButtonShowLog();
	/*****/
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	/*****/
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEditPassword();
	afx_msg void OnButtonDeletePassword();
	afx_msg void OnButtonClonePassword();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillListBox();

private:
	CSSListCtrl m_listPasswords;
	CPasswordData* m_pPasswords;
	bool m_bHaveReader;
};

/**********************************************************************/

