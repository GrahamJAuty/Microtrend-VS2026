#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditGroupEposDlg : public CSSDialog
{
public:
	CDefEditGroupEposDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditGroupEposDlg)
	enum { IDD = IDD_DEF_EDIT_GROUP_EPOS };
	CEdit	m_editEposText;
	CEdit	m_editRepText;
	CStatic	m_staticSageNominalCode;
	CButton m_checkSptBook;
	CStatic m_staticSptBook;
	CButton m_buttonDefault;
	CButton m_buttonSpin;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CString	m_strEposText;
	CString	m_strRepText;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditGroupEposDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	CEdit* GetEditNominalCode();

protected:
	//{{AFX_MSG(CDefEditGroupEposDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	afx_msg void OnTickSportsBooker();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	int m_nIndex;
};

/**********************************************************************/
#endif
/**********************************************************************/

