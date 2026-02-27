#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\PasswordData.h"
/**********************************************************************/

class CPasswordListDlg : public CSSAutoShutdownDialog
{
public:
	CPasswordListDlg( CPasswordData* pPasswords, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPasswordListDlg)
	enum { IDD = IDD_PASSWORD_LIST };
	CButton	m_buttonShowLog;
	CEdit	m_editMasterPassword;
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
	afx_msg void OnButtonEditPassword();
	afx_msg void OnButtonDeletePassword();
	afx_msg void OnButtonClonePassword();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

	virtual void CSSAutoShutdownLogOff()
	{
		TRACE("Auto Shutdown LogOff Password List\n");
		EndDialog(IDCANCEL);
	}

private:
	void FillListBox();

private:
	CSSListCtrl m_listPasswords;
	CPasswordData* m_pPasswords;
	bool m_bLogExists;
};

/**********************************************************************/
