#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPriceTextEditDlg : public CSSDialog
{
public:
	CPriceTextEditDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CPriceTextEditDlg)
	enum { IDD = IDD_PRICETEXT_EDIT };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPriceTextEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	
private:
	int m_editTextID[10];
	CString m_strText[10];

protected:
	//{{AFX_MSG(CPriceTextEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
