/**********************************************************************/

CSSAutoShutdownHelper::CSSAutoShutdownHelper()
{
	m_bShutdownRequestMarker = FALSE;
	Terminate();
}

/**********************************************************************/

void CSSAutoShutdownHelper::Init(ULONGLONG nMinutes)
{
	if (FALSE == m_bDoneInit)
	{
		m_nIdleMinutesAllowed = nMinutes;

		if (m_nIdleMinutesAllowed > 0)
		{
			m_bEnabled = TRUE;
			ResetShutdownTime();
		}
	}
}

/**********************************************************************/

void CSSAutoShutdownHelper::Terminate()
{
	m_nIdleMinutesAllowed = 0;
	m_nShutdownTime = 0;
	m_bFinalShutdownMode = FALSE;
	m_bEnabled = FALSE;
	m_bDoneInit = FALSE;
	m_bTriggered = FALSE;
	m_bTimerBlock1 = FALSE;
	m_bTimerBlock2 = FALSE;
	m_pActiveWindow = NULL;
}

/**********************************************************************/

void CSSAutoShutdownHelper::ResetShutdownTime()
{
	//Add 2 extra seconds so we will briefly see full number of minutes in countdown
	m_nShutdownTime = GetTickCount64() + (60000 * m_nIdleMinutesAllowed) + 2000;
}

/**********************************************************************/

bool CSSAutoShutdownHelper::IsTriggered()
{
	if ( (TRUE == m_bEnabled) && ( FALSE == m_bTriggered ) )
	{
		if (GetTickCount64() >= m_nShutdownTime)
		{
			m_bTriggered = TRUE;
			m_bShutdownRequestMarker = TRUE;
		}
	}

	return m_bTriggered;
}

/**********************************************************************/

void CSSAutoShutdownHelper::SetFinalShutdownSeconds(ULONGLONG nSeconds)
{
	m_bTriggered = FALSE;
	m_bFinalShutdownMode = TRUE;
	m_nShutdownTime = GetTickCount64() + (nSeconds * 1000);
}

/**********************************************************************/

__int64 CSSAutoShutdownHelper::GetSecondsToShutdown()
{
	__int64 nResult = 0;

	if (TRUE == m_bEnabled)
	{
		ULONGLONG timeNow = GetTickCount64();

		if (m_nShutdownTime > timeNow)
		{
			nResult = ( m_nShutdownTime - timeNow ) / (ULONGLONG)1000;
		}
	}

	return nResult;
}

/**********************************************************************/

__int64 CSSAutoShutdownHelper::GetIdleMinutesAllowed()
{
	__int64 nResult = 0;

	if (TRUE == m_bEnabled)
	{
		if ( m_nIdleMinutesAllowed > 0)
		{
			nResult = m_nIdleMinutesAllowed;
		}
	}

	return nResult;
}

/**********************************************************************/

void CSSAutoShutdownHelper::SetActiveWindow(CWnd* pWnd)
{
	m_pActiveWindow = pWnd;

#ifdef _DEBUG

	CString strPrefix = "Setting active shutdown window to ";

	if (pWnd == NULL)
	{
		TRACE( strPrefix + "NULL\n");
	}
	else if (IsWindow(pWnd->m_hWnd) == FALSE)
	{
		TRACE( strPrefix + " uncreated window\n");
	}
	else
	{
		CString strTitle;
		pWnd->GetWindowText(strTitle);	
		TRACE( strPrefix + strTitle + "\n" );
	}

#endif
}

/**********************************************************************/
