#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSDialog.h"
//include "MySSListCtrl.h"
/**********************************************************************/

class CSelectReportSessionTypeDlg : public CSSDialog
{
public:
	CSelectReportSessionTypeDlg( CWnd* pParent = NULL);   
	int GetAction(){ return m_nAction; }

	//{{AFX_DATA(CSelectReportSessionTypeDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_NEW };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSelectReportSessionTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CButton m_buttonOK;
	CButton m_buttonCancel;

protected:
	//{{AFX_MSG(CSelectReportSessionTypeDlg)
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
	CSSListCtrl m_listActions;
	int m_nAction;
};

/**********************************************************************/
#endif
/**********************************************************************/