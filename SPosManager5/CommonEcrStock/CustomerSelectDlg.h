#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
/**********************************************************************/
//include "MySSListCtrl.h"
#include "EposSelectArray.h"
/**********************************************************************/

class CCustomerSelectDlg : public CDialog
{
public:
	CCustomerSelectDlg( CString& strFilter, CEposSelectArray& select, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCustomerSelectDlg)
	enum { IDD = IDD_CUSTOMER_SELECT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomerSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CEdit m_editFilter;

protected:
	//{{AFX_MSG(CCustomerSelectDlg)
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonSelectNone();
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual void OnKillFocusFilter();
	DECLARE_MESSAGE_MAP()

private:
	void AddListLocation();

private:
	BOOL m_bIncludeUnknown;
	BOOL m_bIncludeOneOff;
	CString& m_strFilter;

private:
	CSSListCtrl m_listCustomers;
	CEposSelectArray& m_SelectArray;
};

/**********************************************************************/
