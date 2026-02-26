#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSCalendarDlg.h"
/**********************************************************************/

class CPMSAccountListDlg : public CDialog
{
public:
	CPMSAccountListDlg( int nRoomIdx, CWnd* pParent = NULL);  
	void SetCalendarDlg( CPMSCalendarDlg* pDlg ){ m_pCalendarDlg = pDlg; }

	//{{AFX_DATA(CPMSAccountListDlg)
	enum { IDD = IDD_PMS_ACCOUNT_LIST };
	//}}AFX_DATA
	CButton m_button1;
	CButton m_button2;
	CButton m_buttonOK;

	//{{AFX_VIRTUAL(CPMSAccountListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnButton1();
	afx_msg void OnButton2();

protected:
	void RebuildList( int nAccountIdx );

protected:
	virtual void DoubleClickList(){};
	virtual void HandleButton1(){};
	virtual void HandleButton2(){};
	virtual void SetButtonControls(){};
	
protected:
	//{{AFX_MSG(CPMSAccountListDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

protected:
	void SelectLine( int nIndex );

private:
	void PrepareLine( int nIndex );

protected:
	int m_nRoomIdx;

protected:
	CString m_strTitle;
	CSSListCtrlVirtual m_listAccounts;
	CString m_strDisplayLine;
	bool m_bTraining;
	bool m_bInvoicePrint;

private:
	CPMSCalendarDlg* m_pCalendarDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
