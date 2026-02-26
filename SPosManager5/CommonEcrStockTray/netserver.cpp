/**********************************************************************/
#include <share.h>
/**********************************************************************/
#include "MessageLogger.h"
#include "..\SPosTray5\PosTrayLock.h"
#include "SPOSCommsConnect.h"
/**********************************************************************/
#include "NetServer.h"
/**********************************************************************/
#define TIMER_ID	3
/**********************************************************************/
bool CNetCommsServer::m_bTimerRunning = FALSE;
/**********************************************************************/

BEGIN_MESSAGE_MAP ( CNetCommsServer, CWnd )
	ON_WM_TIMER()
END_MESSAGE_MAP()

/**********************************************************************/

CNetCommsServer::CNetCommsServer()
{
	m_hTimer = 0;
	m_bEnableComms = FALSE;
	m_bCreatedWindow = FALSE;
}

/**********************************************************************/

//CALL ONCE DURING PROGRAM STARTUP
bool CNetCommsServer::InitialiseComms( const char* szNetworkPath, CWnd* pParent )
{
	if ( FALSE == m_bCreatedWindow )
	{
		m_hTimer = 0;
		m_bEnableComms = FALSE;

		m_strNetworkPath = szNetworkPath;

		m_bCreatedWindow = CWnd::Create ( NULL, "CNetCommsServer", WS_CHILD, CRect(0,0,0,0), pParent, NULL ) != 0;

		if ( TRUE == m_bCreatedWindow )
		{
			remove ( m_strNetworkPath + szAckFilename );
			remove ( m_strNetworkPath + szEnqFilename );

			m_hTimer = SetTimer ( TIMER_ID, 1000, NULL );	// 1 second timer

			if ( m_hTimer != 0 )
			{
				m_strError = "Ok";
				return TRUE;
			}
			else
				m_strError = "Cannot create server timer";
		}
		else
			m_strError = "Cannot create server window";
	}

	return FALSE;
}

/**********************************************************************/

//CALL ONCE WHEN CLOSING MAINFRAME WINDOW
void CNetCommsServer::TerminateComms()
{
	if ( TRUE == m_bCreatedWindow )
	{
		if ( m_hTimer != 0 )
		{
			KillTimer ( m_hTimer );
			m_hTimer = 0;
		}

		DestroyWindow();

		m_bEnableComms = FALSE;
		m_bCreatedWindow = FALSE;
	}
}

/**********************************************************************/

void CNetCommsServer::EnableComms()
{
	m_bEnableComms = TRUE;
}

/**********************************************************************/

void CNetCommsServer::DisableComms()
{
	m_bEnableComms = FALSE;
}

/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

void CNetCommsServer::OnTimer(UINT nIDEvent) 
{
	// every second
	if ( m_hTimer == nIDEvent )
		Timer();
	else
		CWnd::OnTimer(nIDEvent);
}

/**********************************************************************/
#else
/**********************************************************************/

void CNetCommsServer::OnTimer(UINT nIDEvent) 
{
	if ( m_hTimer == nIDEvent )
	{
		if ( FALSE == m_bTimerRunning )
		{
			m_bTimerRunning = TRUE;

			__int64 nLockTag = GetTickCount64();
			if ( PosTrayLock.LockFile( 0, nLockTag ) == TRUE )
			{
				Timer();
				PosTrayLock.UnlockFile( 0, nLockTag );
			}
			else
			{
				TRACE("Pos Tray Server Lock Fail\n");
			}

			m_bTimerRunning = FALSE;
		}
		else
			CWnd::OnTimer(nIDEvent);
	}
}

/**********************************************************************/
#endif
/**********************************************************************/

void CNetCommsServer::LogCommsMessage( bool bLineBefore, bool bLineAfter, const char* szMessage, const char* szError )
{
	CMessageLoggerInfo info;
	info.m_bLeadingLineSystem = bLineBefore;
	info.m_bFollowingLineSystem = bLineAfter;
	info.m_nNetworkIdx = 0;
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;
	info.m_bAddNetworkName = FALSE;
	info.m_strMessage = szMessage;
	info.m_strStatus = szError;
	MessageLogger.LogSchedulerMessage( info, FALSE, TRUE, FALSE );
}

/**********************************************************************/

