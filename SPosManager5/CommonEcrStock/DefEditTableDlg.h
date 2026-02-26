#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditTableDlg : public CSSDialog
{
public:
	CDefEditTableDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);  

	//{{AFX_DATA(CDefEditTableDlg)
	enum { IDD = IDD_DEF_EDIT_TABLE };
	
	CEdit m_editTableName;
	CButton m_checkFilter;
	CSSTabbedComboBox m_comboGroup[3];
	CButton m_checkAccount;
	CButton m_checkLimit;
	CButton m_checkAutoStatus1;
	CButton m_checkAutoStatus2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

	CEdit* GetEditLimit();

public:
	int GetIndex() { return m_nIndex; }
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void GetData();
	void SaveData();
	void SpinPrevious();
	void SpinNext();

protected:
	//{{AFX_MSG(CDefEditTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CListDataDlg* m_pListDataDlg;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;
	int m_nIndex;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
