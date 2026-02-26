#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "OPGroupArray.h"
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditOPGroupDlg : public CSSDialog
{
public:
	CDefEditOPGroupDlg( int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditOPGroupDlg)
	enum { IDD = IDD_DEF_EDIT_OPGROUP };
	CEdit m_editName;
	CButton	m_checkPrinter[PLU_ORDER_PRINTERS_NOW];
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditOPGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditOPGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefaultButton();
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
	WORD m_IDPrinter[PLU_ORDER_PRINTERS_NOW];

private:
	int m_nIndex;
};

/**********************************************************************/
#endif
/**********************************************************************/

