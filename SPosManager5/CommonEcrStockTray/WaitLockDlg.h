#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\SmartPaySemaphore.h"
/**********************************************************************/
#include "WaitLockHelpers.h"
/**********************************************************************/
 
class CWaitLockDlg : public CSSDialog
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
	CStatic m_staticWait;

protected:
	//{{AFX_MSG(CWaitLockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
	
protected:
	virtual void SetTexts() = 0;
	virtual bool AcquireLock( bool bFirstAttempt = FALSE ) = 0;
	
private:
	void CancelNow();
	
private:
	UINT m_hTimer;
	bool m_bInTimer;
	CString m_strAction;

protected:
	CString m_strMustBePaused;
	CString m_strMustBeStopped;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/

class CWaitCommsDlg : public CWaitLockDlg
{
public:
	CWaitCommsDlg( const char* szAction, CWnd* pParent = NULL);
	~CWaitCommsDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );
};

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CWaitLoyaltyPauseDlg : public CWaitLockDlg
{
public:
	CWaitLoyaltyPauseDlg( const char* szAction, CWnd* pParent = NULL);
	~CWaitLoyaltyPauseDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );
	
public:
	bool LockLoyalty();
	bool IsLocked(){ return m_bIsLocked; }
	bool WasRunning(){ return m_bWasRunning; }

private:
	bool LockFile();
	
private:
	bool m_bIsLocked;
	bool m_bWasRunning;

private:
	CWaitLockHelpersLoyalty m_Helpers;
};

/**********************************************************************/

class CWaitSmartPayBgndPauseDlg : public CWaitLockDlg
{
public:
	CWaitSmartPayBgndPauseDlg( const char* szAction, CWnd* pParent = NULL);
	~CWaitSmartPayBgndPauseDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );
	
public:
	bool LockSmartBack();
	bool IsLocked(){ return m_bIsLocked; }
	bool WasRunning(){ return m_bWasRunning; }

private:
	bool LockFile();
	
private:
	bool m_bIsLocked;
	bool m_bWasRunning;

private:
	CWaitLockHelpersSmartBack m_Helpers;
};

/**********************************************************************/

class CWaitSmartBackStopDlg : public CWaitLockDlg
{
public:
	CWaitSmartBackStopDlg( const char* szAction, CWnd* pParent = NULL);
	~CWaitSmartBackStopDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );

public:
	bool LockSmartBack();
	bool IsLocked(){ return m_bIsLocked; }

private:
	bool LockFile();

private:
	bool m_bIsLocked;
	
private:
	CWaitLockHelpersSmartBack m_Helpers;
};

/**********************************************************************/

class CWaitSmartPayServerPauseDlg : public CWaitLockDlg
{
public:
	CWaitSmartPayServerPauseDlg( const char* szAction, CWnd* pParent = NULL);
	~CWaitSmartPayServerPauseDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );

public:
	bool LockSmartPay();
	bool IsLocked(){ return m_bIsLocked; }
	bool WasRunning(){ return m_bWasRunning; }

private:
	bool m_bIsLocked;
	bool m_bWasRunning;

private:
	CWaitLockHelpersSmartPay m_Helpers;
};

/**********************************************************************/

class CWaitSmartPayServicePauseDlg : public CWaitLockDlg
{
public:
	CWaitSmartPayServicePauseDlg( CLocalSmartPaySemaphore& Semaphore, const char* szAction, CWnd* pParent = NULL);
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
	CLocalSmartPaySemaphore& m_Semaphore;
};

/**********************************************************************/

class CWaitLoyaltyServicePauseDlg : public CWaitLockDlg
{
public:
	CWaitLoyaltyServicePauseDlg( CLocalLoyaltySemaphore& Semaphore, const char* szAction, CWnd* pParent = NULL);
	~CWaitLoyaltyServicePauseDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );

public:
	bool LockLoyalty();
	bool IsLocked(){ return m_bIsLocked; }
	
private:
	bool m_bIsLocked;
	
private:
	CLocalLoyaltySemaphore& m_Semaphore;
};

/**********************************************************************/

class CWaitSmartPayStopDlg : public CWaitLockDlg
{
public:
	CWaitSmartPayStopDlg( const char* szAction, CWnd* pParent = NULL);
	~CWaitSmartPayStopDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );

public:
	bool LockSmartPay();
	bool IsLocked(){ return m_bIsLocked; }

private:
	bool m_bIsLocked;

private:
	CWaitLockHelpersSmartPay m_Helpers;
};

/**********************************************************************/

class CWaitPosTrayDlg : public CWaitLockDlg
{
public:
	CWaitPosTrayDlg( const char* szAction, CWnd* pParent = NULL);
	~CWaitPosTrayDlg();

public:
	void SetTexts();
	bool AcquireLock( bool bFirstAttempt = FALSE );
	
public:
	bool LockPosTray();
	bool IsLocked(){ return m_bIsLocked; }

private:
	bool m_bIsLocked;
	CSSFile m_fileLock;
};

/**********************************************************************/

class CWaitLoyaltyReportDlg : public CWaitLockDlg
{
public:
	CWaitLoyaltyReportDlg( CString strLoyaltyResponseFile, CWnd* pParent = NULL);
	~CWaitLoyaltyReportDlg();

public:
	void SetTexts();
	bool AcquireLock(bool bFirstAttempt = FALSE);

public:
	bool LockLoyalty();
	bool IsLocked() { return m_bIsLocked; }

private:
	bool LockFile();

private:
	bool m_bIsLocked;
	CString m_strResponseFile;
};

/**********************************************************************/
#endif
/**********************************************************************/
