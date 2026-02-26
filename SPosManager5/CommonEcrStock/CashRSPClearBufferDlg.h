#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "5000Server.h"
#include "RealTimeCommsThread.h"
/**********************************************************************/
#define CLEARBUFFERDLG_ACTIVE 0
#define CLEARBUFFERDLG_WAITCLOSE 1
/**********************************************************************/

struct CCashRSPClearBufferInfo
{
public:
	int m_nNetworkIdx;
	bool m_bIsTerminal;
	int m_nLocIdx;
	int m_nTNo;
	CString m_strDataPath;
	CString m_strLocalPath;
	CString m_strLockFile;
};

/**********************************************************************/

class CCashRSPClearBufferDlg : public CSSDialog
{
public:
	CCashRSPClearBufferDlg( CArray<int,int>& arrayNetworkIdx, CWnd* pParent = NULL);
	
	//{{AFX_DATA(CCashRSPClearBufferDlg)
	enum { IDD = IDD_CASHRSP_CLEARBUFFER };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCashRSPClearBufferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	CStatic m_staticCashRSP;
	CEdit m_editCashRSP1;
	CEdit m_editCashRSP2;
	CStatic m_staticTransactions;
	CEdit m_editTransactions;
	CStatic m_staticSources;
	CEdit m_editSources;
	CStatic m_staticTime;
	CEdit m_editTime;
	CStatic m_staticInfo1;
	CStatic m_staticInfo2;
	CStatic m_staticInfo3;
	CStatic m_staticInfo4;
	CStatic m_staticInfo5;
	CButton m_buttonMoreInfo;
	CButton m_buttonOK;
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCashRSPClearBufferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnButtonMoreInfo();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void HandleTimer();

private:
	CArray<int,int>& m_arrayNetworkIdx;
	CArray<CCashRSPClearBufferInfo,CCashRSPClearBufferInfo> m_arraySites;
	
private:
	int m_nCurrentSiteIdx;
	int m_nCurrentSiteDisplay;
	int m_nTransactionCount;
	bool m_bMoreInfoMode;
	
private:
	UINT m_hTimer;
	int m_nDialogState;
	DWORD m_dwTickCountStartTimer;
	bool m_bTimerBusy;
};

/**********************************************************************/
#endif
/**********************************************************************/
