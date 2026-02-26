#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CCustomerSyncDlg : public CSSDialog
{
public:
	CCustomerSyncDlg( bool bOneOff, CArray<int,int>& arrayIDs, CWnd* pParent = NULL);   

	//{{AFX_DATA(CCustomerSyncDlg)
	enum { IDD = IDD_CUSTOMER_SYNC };
	int m_nAction;
	CEdit m_editIDs;
	CButton m_radio2;
	CButton m_radio3;
	CButton	m_checkImport;
	CButton	m_checkRemove;
	CString m_strIDs;
	BOOL m_bImport;
	BOOL m_bRemove;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomerSyncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCustomerSyncDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bOneOff;
	CArray<int,int>& m_arrayIDs;
};

/**********************************************************************/
