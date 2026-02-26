#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluRangeDlg : public CSSDialog
{
public:
	CPluRangeDlg( const char* szTitle, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluRangeDlg)
	enum { IDD = IDD_PLU_RANGE };
	CString m_strPluFrom;
	CString m_strPluTo;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPluRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusPluNo();
	afx_msg void OnStaticFrom();
	afx_msg void OnStaticTo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditPluFrom();
	CEdit* GetEditPluTo();

private:
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
