#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PersistentSelectionEdit.h"
/**********************************************************************/

class CDefEditStartupMsgDlg : public CSSDialog
{
public:
	CDefEditStartupMsgDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditStartupMsgDlg)
	enum { IDD = IDD_DEF_EDIT_STARTUPMSG };
	CString	m_strStartupMsg;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditStartupMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefEditStartupMsgDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	CPersistentSelectionEdit m_editStartupMsg;
};

/**********************************************************************/
#endif
/**********************************************************************/
