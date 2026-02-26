#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define ADD_PAGES_END 0
#define ADD_PAGES_START 1
#define ADD_PAGES_MIDDLE 2
/**********************************************************************/

class CSPOSKeyboardPageAddDlg : public CSSDialog
{
public:
	CSPOSKeyboardPageAddDlg( int nPageNo, int nMaxNewPages, CWnd* pParent = NULL); 

	//{{AFX_DATA(CSPOSKeyboardPageAddDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_PAGE_ADD };
	CStatic	m_staticMax;
	CButton	m_checkCopyVisible;
	int		m_nMethod;
	int		m_nPagesToAdd;
	BOOL	m_bCopyVisible;
	//}}AFX_DATA
	CButton	m_radioStart;
	CButton	m_radioEnd;
	CButton	m_radioAfter;

	//{{AFX_VIRTUAL(CSPOSKeyboardPageAddDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CSPOSKeyboardPageAddDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPagesToAdd();

private:
	int m_nPageNo;
	int m_nMaxNewPages;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

