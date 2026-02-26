#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardScreenAddDlg : public CSSDialog
{
public:
	CSPOSKeyboardScreenAddDlg( int nVisibleScreenNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSPOSKeyboardScreenAddDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_SCREEN_ADD };
	CButton	m_checkCopy;
	int		m_nScreenNo;
	BOOL	m_bCopy;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardScreenAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardScreenAddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nVisibleScreenNo;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
