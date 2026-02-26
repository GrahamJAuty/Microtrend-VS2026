#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CEditNetworkResourceDlg : public CSSDialog
{
public:
	CEditNetworkResourceDlg( int nResourceNo, CWnd* pParent = NULL);   

	//{{AFX_DATA(CEditNetworkResourceDlg)
	enum { IDD = IDD_EDIT_NETWORK_RESOURCE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEditNetworkResourceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CEdit m_editResource;
	CEdit m_editUser;
	CEdit m_editPassword;
	CStatic m_staticPassword;

protected:
	//{{AFX_MSG(CEditNetworkResourceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nResourceNo;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif 
/**********************************************************************/
