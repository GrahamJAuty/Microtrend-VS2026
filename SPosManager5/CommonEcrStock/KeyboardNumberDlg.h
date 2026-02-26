#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CKeyboardNumberDlg : public CSSDialog
{
public:
	CKeyboardNumberDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CKeyboardNumberDlg)
	enum { IDD = IDD_KEYBOARD_NUMBER };
	int		m_nKbNo;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CKeyboardNumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKeyboardNumberDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
