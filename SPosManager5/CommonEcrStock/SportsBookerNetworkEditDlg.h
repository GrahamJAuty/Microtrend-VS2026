#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
//include "MySSDialog.h"
#include "SportsBookerWebAPISalesDateTime.h"
/**********************************************************************/

class CSportsBookerNetworkEditDlg : public CSSDialog
{
public:
	CSportsBookerNetworkEditDlg( int nNetworkIdx, CSportsBookerWebAPISalesDateTime& dates, CWnd* pParent = NULL);  

	//{{AFX_DATA(CSportsBookerNetworkEditDlg)
	enum { IDD = IDD_SPTBOOK_NETWORK_EDIT };

	CString				m_strName;

	CEdit				m_editSite;
	CEdit				m_editUserName;
	CEdit				m_editPassword;
	CButton				m_buttonPassword;

	CStatic				m_staticDate;
	CEdit				m_editDate;
	CButton				m_buttonDate;
	
	CString				m_strSite;
	CString				m_strUserName;

	int					m_nTimeOut;
	
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CSportsBookerNetworkEditDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSportsBookerNetworkEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPassword();
	afx_msg void OnButtonDateStart();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ShowDate();

public:
	int m_nNetworkIdx;
	int m_nNetworkNo;

public:
	CString m_strNewPassword;

private:
	CSportsBookerWebAPISalesDateTime& m_dates;
	bool m_bGotOldPassword;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
