#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardListAddDlg : public CSSDialog
{
public:
	CSPOSKeyboardListAddDlg( __int64 nVisibleListNo, int nListType, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSPOSKeyboardListAddDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_LIST_ADD };
	CButton	m_checkCopy;
	CString m_strListNo;
	BOOL	m_bCopy;
	//}}AFX_DATA
	__int64 m_nListNo;

	//{{AFX_VIRTUAL(CSPOSKeyboardListAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardListAddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	__int64 m_nVisibleListNo;
	int m_nListType;
	CString m_strMaxListNo;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
