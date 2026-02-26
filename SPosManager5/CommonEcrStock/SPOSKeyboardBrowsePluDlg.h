#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SPOSKeyboardPluFilterArray.h"
/**********************************************************************/

class CSPOSKeyboardBrowsePluDlg : public CSSDialog
{
public:
	CSPOSKeyboardBrowsePluDlg( CSPOSKeyboardPluFilterArray& PluFilterArray, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSPOSKeyboardBrowsePluDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_BROWSE_PLU };
	CString	m_strFilter;
	BOOL	m_bShowModifiers;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardBrowsePluDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSPOSKeyboardBrowsePluDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusFilter();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToggleShowModifiers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	__int64 GetPluNo() { return m_nPluNo; }

private:
	CSSListCtrlVirtual m_listPlu;
	
private:
	CSPOSKeyboardPluFilterArray& m_PluFilterArray;
	__int64 m_nPluNo;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
