#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "GroupSelector.h"
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditGroupReportDlg : public CSSDialog
{
public:
	CDefEditGroupReportDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditGroupReportDlg)
	enum { IDD = IDD_DEF_EDIT_GROUP_REPORT };
	
	CEdit	m_editRepText;

	CEdit	m_editGroupConsol;
	CSSComboBox	m_comboGroupConsol;
	
	CString	m_strRepText;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditGroupReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditGroupReportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
	virtual void OnOK();
	afx_msg void OnSelectGroupConsol();
	afx_msg void OnKillFocusGroupNoConsol();
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

private:
	CGroupSelector m_GroupSelectorConsol;
};

/**********************************************************************/
#endif
/**********************************************************************/

