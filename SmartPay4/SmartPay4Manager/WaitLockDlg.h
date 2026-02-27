#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayLockManager.h"
/**********************************************************************/

class CWaitLockDlg : public CSSAutoShutdownDialog
{
public:
	CWaitLockDlg( const char* szAction, CWnd* pParent = NULL);
	virtual ~CWaitLockDlg();

	//{{AFX_DATA(CWaitLockDlg)
	enum { IDD = IDD_WAIT_LOCK};
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CWaitLockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL
	CButton m_buttonOK;
	CStatic m_staticPause;
	CStatic m_staticAction;

protected:
	//{{AFX_MSG(CWaitLockDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
protected:
	virtual void SetTexts() = 0;
	virtual bool AcquireLock( bool bFirstAttempt = FALSE ) = 0;

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();
	virtual void CSSAutoShutdownTimer(UINT nIDEvent);
	virtual void CSSAutoShutdownDestroy();
	
private:
	void CancelNow();

private:
	CString m_strAction;
	UINT m_hTimer;
	bool m_bInTimer;

protected:
	CString m_strMustBePaused;
	CString m_strMustBeStopped;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

class CWaitSmartPayServicePauseDlg : public CWaitLockDlg
{
public:
	CWaitSmartPayServicePauseDlg( const char* szAction, CSmartPayLockManager& SmartPayLockManager, CWnd* pParent = NULL);
	~CWaitSmartPayServicePauseDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );

public:
	bool LockSmartPay();
	bool IsLocked(){ return m_bIsLocked; }
	
private:
	bool m_bIsLocked;
	
private:
	CSmartPayLockManager& m_SmartPayLockManager;
};

/**********************************************************************/

class CWaitUpdatePluDlg : public CWaitLockDlg
{
public:
	CWaitUpdatePluDlg(const char* szAction, CWnd* pParent = NULL);
	~CWaitUpdatePluDlg();

public:
	void SetTexts();
	bool AcquireLock(bool bFirstAttempt = FALSE);

public:
	bool LockLoyalty();
	bool IsLocked() { return m_bIsLocked; }

private:
	bool m_bIsLocked;
	CString m_strToken;
};

/**********************************************************************/