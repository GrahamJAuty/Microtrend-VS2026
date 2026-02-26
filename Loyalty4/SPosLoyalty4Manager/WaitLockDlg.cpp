/**********************************************************************/
#include "..\SPOSLoyalty4Shared\FileRemover.h"
#include "..\SPOSLoyalty4Shared\LoyaltyLockManager.h"
/**********************************************************************/
#include "LockManager.h"
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
	m_hTimer = NULL;
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
	DDX_Control( pDX, IDOK, m_buttonOK );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CWaitLockDlg, CSSDialog)
	//{{AFX_MSG_MAP(CWaitLockDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CWaitLockDlg::OnInitDialog() 
{
	CSSDialog::OnInitDialog();
	m_staticAction.SetWindowText( m_strAction );
	m_buttonOK.SetWindowText( "Cancel" );
	SetTexts();
	m_hTimer = SetTimer(TIMER_WAITLOCK, 1000, 0);
	return TRUE;
}

/**********************************************************************/

void CWaitLockDlg::OnTimer(UINT nIDEvent)
{
	if ( TIMER_WAITLOCK == nIDEvent)
	{
		if (FALSE == m_bInTimer)
		{
			m_bInTimer = TRUE;

			if (AcquireLock() == TRUE)
			{
				EndDialog(IDOK);
			}

			m_bInTimer = FALSE;
		}
	}
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

CWaitLoyaltyServicePauseDlg::CWaitLoyaltyServicePauseDlg( const char* szAction, CLoyaltyLockManager& LoyaltyLockManager, CWnd* pParent) : CWaitLockDlg( szAction, pParent ), m_LoyaltyLockManager(LoyaltyLockManager)
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
	CString str = "The Loyalty Server";
	SetWindowText( str );
	str += m_strMustBePaused;
	m_staticPause.SetWindowText( str );
}

/**********************************************************************/

bool CWaitLoyaltyServicePauseDlg::LockLoyalty()
{
	if ( AcquireLock( TRUE ) == FALSE )
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
		CLoyaltyLockRequest LockRequest;
		LockRequest.m_bWantLockServerOuter = TRUE;
		LockRequest.m_bOneShot = TRUE;
		m_bIsLocked = ( m_LoyaltyLockManager.AcquireLocks(LockRequest) == LOCKMANAGER_OK );
	}

	return m_bIsLocked;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CWaitUpdatePluDlg::CWaitUpdatePluDlg(const char* szAction, CWnd* pParent) : CWaitLockDlg(szAction, pParent)
{
	m_bIsLocked = FALSE;
	m_strToken.Format("%I64d", GetTickCount64());
}

/**********************************************************************/

CWaitUpdatePluDlg::~CWaitUpdatePluDlg()
{
}

/**********************************************************************/

void CWaitUpdatePluDlg::SetTexts()
{
	SetWindowText("Update Stamp Offers");
	m_staticPause.SetWindowText("Waiting for Loyalty Server to respond.");
}

/**********************************************************************/

bool CWaitUpdatePluDlg::LockLoyalty()
{
	{
		CSSFile fileReq;
		if (fileReq.Open(Filenames.GetServerUpdatePluReqFilename(), "wb") == TRUE)
		{
			fileReq.WriteLine(m_strToken);
			fileReq.Close();
		}
	}

	if (AcquireLock(TRUE) == FALSE)
	{
		DoModal();
	}

	return m_bIsLocked;
}

/**********************************************************************/

bool CWaitUpdatePluDlg::AcquireLock(bool bFirstAttempt)
{
	if (FALSE == m_bIsLocked)
	{
		CSSFile fileRsp;
		if (fileRsp.Open(Filenames.GetServerUpdatePluRspFilename(), "rb") == TRUE)
		{
			CString str;
			fileRsp.ReadString(str);
			fileRsp.Close();
			m_bIsLocked = (str == m_strToken);
		}
	}

	return m_bIsLocked;
}

/**********************************************************************/
