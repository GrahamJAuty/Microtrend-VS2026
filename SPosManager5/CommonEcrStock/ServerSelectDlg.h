#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
/**********************************************************************/
//include "MySSListCtrl.h"
#include "ServerSelectionList.h"
#include "EposSelectArray.h"
/**********************************************************************/

class CServerSelectDlg : public CDialog
{
public:
	CServerSelectDlg( CServerSelectionList& list, CEposSelectArray& select, CWnd* pParent = NULL);   

	//{{AFX_DATA(CServerSelectDlg)
	enum { IDD = IDD_SERVER_SELECT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CServerSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CServerSelectDlg)
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonSelectNone();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddListSystem();
	void AddListDatabase();
	void AddListLocation();

private:
	BOOL m_bServer;

private:
	CSSListCtrl m_listServers;
	CServerSelectionList& m_ServerSelectionList;
	CEposSelectArray& m_SelectArray;
};

/**********************************************************************/
