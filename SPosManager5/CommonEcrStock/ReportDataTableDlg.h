#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CReportDataTableDlg : public CSSDialog
{
public:
	CReportDataTableDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CReportDataTableDlg)
	enum { IDD = IDD_REPORT_DATA_TABLE };
	//}}AFX_DATA
	CButton m_buttonOK;
	CButton m_buttonCancel;

	//{{AFX_VIRTUAL(CReportDataTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReportDataTableDlg)
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
	virtual void HandleInitDialog(){}
	virtual void HandleReport1(){};
	virtual void HandleReport2(){};
	virtual void HandleReport3(){};
	virtual void HandleReport4(){};

private:
	CSSListCtrl m_listReports;

protected:
	CString m_strTitle;
	CStringArray m_arrayReports;
	bool m_bKeepOpen;
};

/**********************************************************************/
#endif
/**********************************************************************/
