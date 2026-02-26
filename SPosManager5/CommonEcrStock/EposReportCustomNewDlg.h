#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
/**********************************************************************/

class CEposReportCustomNewDlg : public CSSDialog
{
public:
	CEposReportCustomNewDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CEposReportCustomNewDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_NEW };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEposReportCustomNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEposReportCustomNewDlg)
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
	CSSListCtrl m_listReports;

public:
	int m_nSel;
};

/**********************************************************************/
#endif
/**********************************************************************/
