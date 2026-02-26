#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSiteEditDlg : public CDialog
{
public:
	CSiteEditDlg( const char* szTitle, CWnd* pParent = NULL);   

	//{{AFX_DATA(CSiteEditDlg)
	enum { IDD = IDD_SITE_EDIT };
	CString	m_strAddress1;
	CString	m_strAddress2;
	CString	m_strAddress3;
	CString	m_strName;
	CString	m_strPhone;
	CString	m_strPostcode;
	CString	m_strEmail;
	CString	m_strFax;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSiteEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSiteEditDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_strTitle;
};

/**********************************************************************/
#endif
/**********************************************************************/
