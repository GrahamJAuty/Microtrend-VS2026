#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CReasonTextTypeDlg : public CSSDialog
{
public:
	CReasonTextTypeDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CReasonTextTypeDlg)
	enum { IDD = IDD_EPOS_REPORT_CUSTOM_NEW };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReasonTextTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	CButton m_buttonOK;
	CButton m_buttonCancel;
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReasonTextTypeDlg)
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
	CSSListCtrl m_listTextTypes;

public:
	int m_nSel;
};

/**********************************************************************/
#endif
/**********************************************************************/
