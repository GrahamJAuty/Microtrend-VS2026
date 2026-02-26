#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSupplierKeyDlg : public CDialog
{
public:
	CSupplierKeyDlg( bool bAdd, bool bCustomField, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSupplierKeyDlg)
	enum { IDD = IDD_SUPPLIER_KEY };
	CString	m_strKey;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSupplierKeyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSupplierKeyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_bAdd;
	bool m_bCustomField;
};

/**********************************************************************/
#endif
/**********************************************************************/
