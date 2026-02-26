#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "PersistentSelectionEdit.h"
/**********************************************************************/

class CDefEditEmailDlg : public CSSDialog
{
public:
	CDefEditEmailDlg( const char* szTitle, CStringArray& arrayEmail, CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditEmailDlg)
	enum { IDD = IDD_DEF_EDIT_EMAIL };
	CString	m_strName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditEmailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditEmailDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	CPersistentSelectionEdit m_editEmail;
	CString m_strEmail;
	CString m_strTitle;
	CStringArray& m_arrayEmail;
};

/**********************************************************************/
