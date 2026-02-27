#pragma once
//$$******************************************************************
#include "Resource.h"
//$$******************************************************************

class CSelectImportTypeDlg : public CSSAutoShutdownDialog
{
public:
	CSelectImportTypeDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CSelectImportTypeDlg)
	enum { IDD = IDD_SELECT_IMPORT_TYPE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSelectImportTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSelectImportTypeDlg)
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

private:
	void FillListBox();

private:
	CSSListCtrl m_listImports;

public:
	int m_nSel;
};

//$$******************************************************************
