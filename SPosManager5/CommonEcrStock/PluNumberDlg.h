#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluNumberDlg : public CSSDialog
{
public:
	CPluNumberDlg( const char* szTitle, bool bBrowse = FALSE, CWnd* pParent = NULL);  

	//{{AFX_DATA(CPluNumberDlg)
	enum { IDD = IDD_PLU_NUMBER };
	CButton	m_buttonBrowse;
	CString	m_strPluNumber;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluNumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluNumberDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluFilterArray m_FilterArray;
	bool m_bBrowse;
	CString m_strTitle;
};

/**********************************************************************/
#endif				
/**********************************************************************/

