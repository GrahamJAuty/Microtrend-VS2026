#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
/**********************************************************************/

class CSelectPresetActionDlg : public CSSDialog
{
public:
	CSelectPresetActionDlg( CArray<int,int>& arrayActions, int nOldEntityNo, int nNewEntityNo, CWnd* pParent = NULL);   
	int GetAction(){ return m_nAction; }

	//{{AFX_DATA(CSelectPresetActionDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_NEW };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSelectPresetActionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSelectPresetActionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillListBox();

private:
	CArray<int,int>& m_arrayActions;
	CSSListCtrl m_listActions;

private:
	int m_nAction;
	int m_nOldEntityNo;
	int m_nNewEntityNo;
};

/**********************************************************************/
#endif
/**********************************************************************/