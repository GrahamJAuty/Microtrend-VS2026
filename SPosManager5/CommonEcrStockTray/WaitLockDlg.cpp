/**********************************************************************/
#include "CommsHandshaker.h"
#include "FileRemover.h"
#include "LockManager.h"
#include "PosTrayTokenFiles.h"
#include "SysInfo.h"
#include "TimerNumbers.h"
/**********************************************************************/
#include "WaitLockDlg.h"
/**********************************************************************/

CWaitLockDlg::CWaitLockDlg( const char* szAction, CWnd* pParent)
		: CSSDialog(CWaitLockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitLockDlg)
	//}}AFX_DATA_INIT
	m_strMustBePaused = " must be paused before this action can continue.";
	m_strMustBeStopped = " must be Stopped before this action can continue.";
	m_strAction = szAction;
	m_bInTimer = FALSE;
}

/**********************************************************************/

CWaitLockDlg::~CWaitLockDlg()
{
}

/**********************************************************************/

void CWaitLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitLockDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_STATIC_PAUSE, m_staticPause );
	DDX_Control( pDX, IDC_STATIC_ACTION, m_staticAction );
	DDX_Control( pDX, IDC_STATIC_WAIT, m_staticWait);
	DDX_Control( pDX, IDOK, m_buttonOK );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CWaitLockDlg, CDialog)
	//{{AFX_MSG_MAP(CWaitLockDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CWaitLockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_staticAction.SetWindowText( m_strAction );
	m_buttonOK.SetWindowText( "Cancel" );

	SetTexts();

	m_hTimer = SetTimer( TIMER_WAITLOCK, 1000, NULL );

	if (m_hTimer == NULL)
	{
		EndDialog(IDCANCEL);
	}
	
	return TRUE;
}

/**********************************************************************/

void CWaitLockDlg::OnTimer(UINT nIDEvent) 
{
	if ( FALSE == m_bInTimer )
	{
		m_bInTimer = TRUE;

		if (AcquireLock() == TRUE)
		{
			EndDialog(IDOK);
		}
		
		m_bInTimer = FALSE;
	}
	
	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CWaitLockDlg::OnOK() 
{
	CancelNow();
}

/**********************************************************************/

void CWaitLockDlg::OnCancel() 
{
	CancelNow();
}

/**********************************************************************/

void CWaitLockDlg::CancelNow()
{	
	EndDialog( IDCANCEL );
}

/**********************************************************************/

void CWaitLockDlg::OnDestroy()
{	
	if (m_hTimer != NULL)
	{
		KillTimer(m_hTimer);
	}

	CDialog::OnDestroy();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitCommsDlg::CWaitCommsDlg( const char* szAction, CWnd* pParent ) : CWaitLockDlg( szAction, pParent )
{
}

/**********************************************************************/

CWaitCommsDlg::~CWaitCommsDlg()
{
}

/**********************************************************************/

void CWaitCommsDlg::SetTexts()
{
	CString str = "Real Time Comms";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitCommsDlg::AcquireLock( bool bFirstAttempt )
{
	return CommsHandshaker.GetConfirmStoppedFlag();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

CWaitLoyaltyPauseDlg::CWaitLoyaltyPauseDlg( const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent )
{
	m_bIsLocked = FALSE;
	m_bWasRunning = FALSE;
}

/**********************************************************************/

CWaitLoyaltyPauseDlg::~CWaitLoyaltyPauseDlg()
{
	if ( TRUE == m_bIsLocked )
	{
		m_Helpers.DeletePauseRequest();
		m_Helpers.DeleteBlockRequest();
	}
}

/**********************************************************************/

void CWaitLoyaltyPauseDlg::SetTexts()
{
	CString str = "Loyalty Manager Server";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitLoyaltyPauseDlg::LockLoyalty()
{
	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_LOY_V2 )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock( TRUE ) == FALSE )
	{
		m_bWasRunning = TRUE;
		m_Helpers.CreatePauseRequest();
		DoModal();
	}

	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitLoyaltyPauseDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		if ( m_Helpers.CheckRootFolder() == FALSE )
			m_bIsLocked = TRUE;
		else
		{
			if ( m_Helpers.CheckRunFile() == FALSE )
				m_bIsLocked = TRUE;
			else if ( FALSE == bFirstAttempt )
				m_bIsLocked = ( m_Helpers.CheckPauseConfirm() == TRUE );
				
			if ( TRUE == m_bIsLocked )
				m_Helpers.CreateBlockRequest();
		}
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitSmartPayServerPauseDlg::CWaitSmartPayServerPauseDlg( const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent )
{
	m_bIsLocked = FALSE;
	m_bWasRunning = FALSE;
}

/**********************************************************************/

CWaitSmartPayServerPauseDlg::~CWaitSmartPayServerPauseDlg()
{
	if ( TRUE == m_bIsLocked )
	{
		m_Helpers.DeletePauseRequest();
		m_Helpers.DeleteBlockRequest();
	}
}

/**********************************************************************/

void CWaitSmartPayServerPauseDlg::SetTexts()
{
	CString str = "SmartPay Server";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitSmartPayServerPauseDlg::LockSmartPay()
{
	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V1 )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock( TRUE ) == FALSE )
	{
		m_bWasRunning = TRUE;
		m_Helpers.CreatePauseRequest();
		DoModal();
	}

	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitSmartPayServerPauseDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		if ( m_Helpers.CheckRootFolder() == FALSE )
			m_bIsLocked = TRUE;
		else
		{
			if ( m_Helpers.CheckRunFile() == FALSE )
				m_bIsLocked = TRUE;
			else if ( FALSE == bFirstAttempt )
				m_bIsLocked = ( m_Helpers.CheckPauseConfirm() == TRUE );
	
			if ( TRUE == m_bIsLocked )
				m_Helpers.CreateBlockRequest();
		}
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitSmartPayServicePauseDlg::CWaitSmartPayServicePauseDlg( CLocalSmartPaySemaphore& Semaphore, const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent ), m_Semaphore( Semaphore )
{
	m_bIsLocked = FALSE;
	SmartPaySemaphore.ResetZombieCheck();
}

/**********************************************************************/

CWaitSmartPayServicePauseDlg::~CWaitSmartPayServicePauseDlg()
{
}

/**********************************************************************/

void CWaitSmartPayServicePauseDlg::SetTexts()
{
	CString str = "SmartPay Services";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitSmartPayServicePauseDlg::LockSmartPay()
{
	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V2 )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock( TRUE ) == FALSE )
	{
		DoModal();
	}

	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitSmartPayServicePauseDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		m_Semaphore.Lock();
		m_bIsLocked = m_Semaphore.GotLock();
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitSmartPayStopDlg::CWaitSmartPayStopDlg( const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent )
{
	m_bIsLocked = FALSE;
}

/**********************************************************************/

CWaitSmartPayStopDlg::~CWaitSmartPayStopDlg()
{
}

/**********************************************************************/

void CWaitSmartPayStopDlg::SetTexts()
{
	CString str = "SmartPay Server";
	SetWindowText( str );
	str += m_strMustBeStopped;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitSmartPayStopDlg::LockSmartPay()
{
	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V1 )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock() == FALSE )
		DoModal();
		
	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitSmartPayStopDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		if ( m_Helpers.CheckRootFolder() == FALSE )
			m_bIsLocked = TRUE;
		else 
			m_bIsLocked = ( m_Helpers.CheckRunFile() == FALSE );
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitSmartPayBgndPauseDlg::CWaitSmartPayBgndPauseDlg( const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent )
{
	m_bIsLocked = FALSE;
	m_bWasRunning = FALSE;
}

/**********************************************************************/

CWaitSmartPayBgndPauseDlg::~CWaitSmartPayBgndPauseDlg()
{
	if ( TRUE == m_bIsLocked )
		m_Helpers.DeletePauseRequest();
}

/**********************************************************************/

void CWaitSmartPayBgndPauseDlg::SetTexts()
{
	CString str = "SmartPay Background Processor";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitSmartPayBgndPauseDlg::LockSmartBack()
{
	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V1 )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock( TRUE ) == FALSE )
	{
		m_bWasRunning = TRUE;
		m_Helpers.CreatePauseRequest();
		DoModal();
	}

	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitSmartPayBgndPauseDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		if ( m_Helpers.CheckRootFolder() == FALSE )
			m_bIsLocked = TRUE;
		else if ( m_Helpers.CheckRunFile() == FALSE )
			m_bIsLocked = TRUE;
		else if ( FALSE == bFirstAttempt )
		{
			if	( m_Helpers.CheckPauseRequest() == FALSE )
				m_bIsLocked = TRUE;
		}
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitSmartBackStopDlg::CWaitSmartBackStopDlg( const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent )
{
	m_bIsLocked = FALSE;
}

/**********************************************************************/

CWaitSmartBackStopDlg::~CWaitSmartBackStopDlg()
{
}

/**********************************************************************/

void CWaitSmartBackStopDlg::SetTexts()
{
	CString str = "SmartPay Background Processor";
	SetWindowText( str );
	str += m_strMustBeStopped;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitSmartBackStopDlg::LockSmartBack()
{
	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_SMP_V1 )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock() == FALSE )
		DoModal();
		
	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitSmartBackStopDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		if ( m_Helpers.CheckRootFolder() == FALSE )
			m_bIsLocked = TRUE;
		else 
			m_bIsLocked = ( m_Helpers.CheckRunFile() == FALSE );
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitLoyaltyServicePauseDlg::CWaitLoyaltyServicePauseDlg( CLocalLoyaltySemaphore& Semaphore, const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent ), m_Semaphore( Semaphore )
{
	m_bIsLocked = FALSE;
}

/**********************************************************************/

CWaitLoyaltyServicePauseDlg::~CWaitLoyaltyServicePauseDlg()
{
}

/**********************************************************************/

void CWaitLoyaltyServicePauseDlg::SetTexts()
{
	CString str = "Loyalty Services";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitLoyaltyServicePauseDlg::LockLoyalty()
{
	if ( SysInfo.GetLoyaltyOrSmartPayType() != LOYALTY_TYPE_LOY_V3 )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock( TRUE ) == FALSE )
	{
		DoModal();
	}

	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitLoyaltyServicePauseDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		m_Semaphore.Lock();
		m_bIsLocked = m_Semaphore.GotLock();
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitPosTrayDlg::CWaitPosTrayDlg( const char* szAction, CWnd* pParent) : CWaitLockDlg( szAction, pParent )
{
	m_bIsLocked = FALSE;
}

/**********************************************************************/

CWaitPosTrayDlg::~CWaitPosTrayDlg()
{
	if ( SysInfo.IsPosTraySystem() == TRUE )
		if ( TRUE == m_bIsLocked )
			m_fileLock.Close();
}

/**********************************************************************/

void CWaitPosTrayDlg::SetTexts()
{
	CString str = "PosTray";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitPosTrayDlg::LockPosTray()
{
	if ( SysInfo.IsPosTraySystem() == FALSE )
		m_bIsLocked = TRUE;		
	else if ( AcquireLock() == FALSE )
	{
		CPosTrayTokenFiles::CreateStopFile();			
		DoModal();
		CPosTrayTokenFiles::DeleteStopFile();
	}

	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitPosTrayDlg::AcquireLock( bool bFirstAttempt )
{
	if ( FALSE == m_bIsLocked )
	{
		CFilenameUpdater FnUp ( SysFiles::PosTrayLockFile );
		m_bIsLocked = m_fileLock.Open( FnUp.GetInstanceFilenameToUse(), "wb" );
	}
	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitLoyaltyReportDlg::CWaitLoyaltyReportDlg(CString strResponseFile, CWnd* pParent) : CWaitLockDlg("", pParent)
{
	m_bIsLocked = FALSE;
	m_strResponseFile = strResponseFile;
}

/**********************************************************************/

CWaitLoyaltyReportDlg::~CWaitLoyaltyReportDlg()
{
}

/**********************************************************************/

void CWaitLoyaltyReportDlg::SetTexts()
{
	CString strSystemType = "";
	
	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V2:
	case LOYALTY_TYPE_SMP_V4:
		strSystemType = "SmartPay";
		break;

	case LOYALTY_TYPE_LOY_V3:
	case LOYALTY_TYPE_LOY_V4:
		strSystemType = "Loyalty";
		break;
	}

	SetWindowText(strSystemType + " Reconciliation Report");
		
	CString strPause = "";
	strPause += "Waiting for ";
	strPause += strSystemType;
	strPause += " Server response.";

	m_staticPause.SetWindowText(strPause);

	MoveControl(&m_staticPause, 23, 19);
	MoveControl(&m_staticWait, 23, 35);
	m_staticAction.ShowWindow(SW_HIDE);	
}

/**********************************************************************/

bool CWaitLoyaltyReportDlg::LockLoyalty()
{
	DoModal();
	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitLoyaltyReportDlg::AcquireLock(bool bFirstAttempt)
{
	if (FALSE == m_bIsLocked)
	{
		if (FileExists(m_strResponseFile) == TRUE)
		{
			CFileStatus FileStatus;
			CFile::GetStatus(m_strResponseFile, FileStatus);
			m_bIsLocked = (FileStatus.m_size != 0);
		}
	}

	return m_bIsLocked;
}

/**********************************************************************/
#endif
/**********************************************************************/

