#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
#include "SMTPEmailHelpers.h"
/**********************************************************************/

class CSMTPResultsDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CSMTPResultsDlg)

public:
	CSMTPResultsDlg( int nType, CSMTPEmailHelpers& SMTPHelpers, CWnd* pParent = NULL ); 
	virtual ~CSMTPResultsDlg();

	enum { IDD = IDD_SMTP_RESULTS };
	CButton m_buttonPause;
	CButton m_buttonCancel;
	CSSListCtrlVirtual m_listResults;
	CTabCtrl m_TabStatus;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonPause();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	void SetEmailStatus( int nInfoIdx, int nStatus );
	void SetEmailError( int nInfoIdx, int nContextError, int nSMTPError );
	void SelectPendingListItem( int nPendingIdx );
	bool GetInfoIdx( int& nPendingIdx, int& nInfoIdx );
	int SelectTab();
	
private:
	void StartTimer();
	void StopTimer();
	
private:
	int m_nSessionType;
	CSMTPEmailHelpers& m_SMTPHelpers;

private:
	int m_nTabPage;
	int m_nTabType;
	CArray<int,int> m_arrayTabPages;
	CArray<int,int>* m_pArrayTabCurrent;
	CArray<int,int> m_arrayTabPending;
	CArray<int,int> m_arrayTabRedirected;
	CArray<int,int> m_arrayTabSent;
	CArray<int,int> m_arrayTabInvalid;
	CArray<int,int> m_arrayTabPendingNotSent;

private:
	UINT m_hTimer;
	int m_nWaitingIdx;
	int m_nState;
	bool m_bPauseByUser;
	bool m_bPauseByDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
