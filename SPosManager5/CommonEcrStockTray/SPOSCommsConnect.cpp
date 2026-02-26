/**********************************************************************/
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "SPOSCommsConnect.h"
/**********************************************************************/

CSPOSCommsConnect::CSPOSCommsConnect()
{
	m_bUserExit = FALSE;
	m_pStatusWnd = NULL;
	m_nNetworkIdx = 0;
}

CSPOSCommsConnect::~CSPOSCommsConnect()
{
	Disconnect();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSPOSCommsConnect::UserWaitLoop ( DWORD dwMSecs )
{
	DWORD dwStartTime = ::GetTickCount();
	MSG msg;

	while ( m_bUserExit == FALSE )
	{
		if ( dwMSecs > 0 )
		{
			if (( ::GetTickCount() - dwStartTime ) >= dwMSecs )
				return;
		}

		if ( ::PeekMessage ( &msg, NULL, 0, 0, PM_REMOVE ) != 0 )
		{
			::TranslateMessage ( &msg );
			::DispatchMessage ( &msg );
		}
	}
}

/**********************************************************************/

bool CSPOSCommsConnect::AccessWait ( const char* szPath, DWORD dwMSecs )
{
	UserWaitLoop ( 500 );

	DWORD dwStartTime = ::GetTickCount();

	while ( GetFileAttributes ( szPath ) == -1 )
	{
		if (( ::GetTickCount() - dwStartTime ) >= dwMSecs )
		{
			m_strError = "Error: Timeout";
			return FALSE;
		}

		UserWaitLoop ( 1000 );

		if ( m_bUserExit == TRUE )
		{
			m_strError = "Terminated by user";
			return FALSE;
		}
	}

	m_strError = "Ok";
	return TRUE;
}

/**********************************************************************/

void CSPOSCommsConnect::DisplayStatus ( const char* szText )
{
	if ( m_pStatusWnd != NULL )
		m_pStatusWnd->SetWindowText ( szText );
}

/**********************************************************************/

bool CSPOSCommsConnect::Connect ( int nNetworkIdx, const char* szLocationName, const char* szNetworkPath, CWnd* pStatusWnd )
{
	m_nNetworkIdx = nNetworkIdx;
	
	m_pStatusWnd = pStatusWnd;
	m_bUserExit = FALSE;		// clear terminated by user flag
	bool bConnected = FALSE;

	Disconnect();				// disconnect if already connected

	DisplayStatus ( "Connecting" );

	CString strMessage;

	{
		if (( bConnected = AccessWait ( szNetworkPath, 1000 )) == TRUE )
			DisplayStatus ( "Connected" );

		strMessage.Format ( "Connect: %s (%s)", szLocationName, szNetworkPath );
		LogMessage ( strMessage, m_strError );
	}

	return bConnected;
}

/**********************************************************************/

void CSPOSCommsConnect::Disconnect( bool bForceLog )
{
	if ( bForceLog == TRUE )
		LogMessage ( "Disconnect", "Ok" );
}

/**********************************************************************/

void CSPOSCommsConnect::Terminate()
{
	m_bUserExit = TRUE;			// terminated by user
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CSPOSCommsConnect::LogMessage ( const char* szText, const char* szError, bool bHide )
{
	CString strText = "Comms ";
	strText += szText;

	CMessageLoggerInfo info;
	info.m_strMessage = szText;
	info.m_strStatus = szError;
	info.m_nHideLevel = ( bHide == TRUE ) ? 1 : 0;
	info.m_nNetworkIdx = m_nNetworkIdx;
	info.m_bAddDate = TRUE;
	info.m_bAddTimeOfDay = TRUE;

	MessageLogger.LogSchedulerMessage( info, TRUE, TRUE, TRUE );
}

/**********************************************************************/
