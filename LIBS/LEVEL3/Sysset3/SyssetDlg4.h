/**********************************************************************/
#pragma once
/**********************************************************************/

#include "SimpleListCtrl.h"
#include "sysset3.h"

/**********************************************************************/

class CSyssetDlg4 : public CDialog
{
	DECLARE_DYNAMIC(CSyssetDlg4)

public:
	CSyssetDlg4(CWnd* pParent = NULL);
	virtual ~CSyssetDlg4();

	enum { IDD = IDD_SYSSET_DLG4 };

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	void DisplayUsers();
	void SetColumnHeader(int nColumn, CString strHeader);

public:
	CSysset* m_pSysset = nullptr;	

private:
	CSimpleListCtrl m_listUsers;
	UINT_PTR m_nTimer = 0;
	bool m_bShowProgUser = FALSE;
	bool m_bShowClientPC = FALSE;
};

/**********************************************************************/
