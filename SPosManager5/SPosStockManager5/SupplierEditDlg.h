#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSupplierEditDlg : public CSSDialog
{
public:
	CSupplierEditDlg( const char* szTitle, int nSuppIdx, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSupplierEditDlg)
	enum { IDD = IDD_SUPPLIER_EDIT };
	CString	m_strAddress1;
	CString	m_strAddress2;
	CString	m_strAddress3;
	CString	m_strAddress4;
	CString	m_strAddress5;
	CString	m_strName;
	CString	m_strPhone;
	CString	m_strPostcode;
	CString	m_strEmail;
	CString	m_strFax;
	CString	m_strAccountNo;
	CString m_strFAO;
	CString	m_strKey;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSupplierEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	int GetSuppIdx() { return m_nSuppIdx; }

protected:
	//{{AFX_MSG(CSupplierEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nSuppIdx;
	CSupplierCSVRecord m_SupplierRecord;
	CString m_strOldKey;
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
