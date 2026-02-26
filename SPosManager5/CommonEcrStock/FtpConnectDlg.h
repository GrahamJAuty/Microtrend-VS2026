#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define CONNECT_STATUS_NOT_STARTED 0
#define CONNECT_STATUS_WAIT_CONNECT 1
#define CONNECT_STATUS_WAIT_CANCEL 2
#define CONNECT_STATUS_FINISHED 3
#define CONNECT_STATUS_FAILED 4
#define CONNECT_STATUS_USER_CANCEL 5
#define CONNECT_STATUS_CONNECTING 6
/**********************************************************************/

class CFtpConnectDlg : public CDialog
{
public:
	CFtpConnectDlg( int nRetries, bool bWaitOnFail, CWnd* pParent = NULL);
	~CFtpConnectDlg();

	//{{AFX_DATA(CFtpConnectDlg)
	enum { IDD = IDD_FTP_CONNECT };
	CEdit	m_editTime;
	CButton	m_buttonCancel;
	CEdit	m_editStatus;
	CEdit	m_editLocation;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFtpConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

public:
	int GetStatus(){ return m_nStatus; }
	int GetError(){ return m_nError; }
	int GetFtpError(){ return m_nFtpError; }
	
protected:
	//{{AFX_MSG(CFtpConnectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void ShowStatus();
	void CancelNow();

private:
	int m_nRetries;
	bool m_bWaitOnFail;

	int m_nError;
	int m_nFtpError;

	int m_nStatus;
	int m_nConnectCountdown;
	int m_nCancelCountdown;
		
private:
	UINT m_hTimer;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
