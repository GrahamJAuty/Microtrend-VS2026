/**********************************************************************/

CSSAutoShutdownPropertySheet::CSSAutoShutdownPropertySheet(CSSAutoShutdownHelper& AutoShutdownHelper, const char* szTitle, CWnd* pWndParent, UINT nFirstPage ) 
	: CPropertySheet(szTitle, pWndParent, nFirstPage), m_AutoShutdownHelper( AutoShutdownHelper)
{
	m_hAutoShutdownTimer = NULL;
	m_pPreviousShutdownWindow = NULL;
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSAutoShutdownPropertySheet, CPropertySheet)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSSAutoShutdownPropertySheet::OnInitDialog()
{
	CSSAutoShutdownPreInitDialog();

	CPropertySheet::OnInitDialog();

	if (m_AutoShutdownHelper.IsEnabled() == TRUE)
	{
		m_AutoShutdownHelper.ResetShutdownTime();

		m_hAutoShutdownTimer = SetTimer(999, 1000, 0);

		if (m_hAutoShutdownTimer == NULL)
		{
			m_AutoShutdownHelper.Trigger();
			EndDialog(IDCANCEL);
		}
		else
		{
			CWnd* pWnd = m_AutoShutdownHelper.GetActiveWindow();

			if (pWnd != NULL)
			{
				m_pPreviousShutdownWindow = pWnd;

#ifdef _DEBUG
				if (IsWindow(pWnd->m_hWnd))
				{
					CString strTitle;
					pWnd->GetWindowText(strTitle);
					TRACE(strTitle + " left active when opening new window\n");
				}
				else
				{
					TRACE("Uncreated window left active when opening new window\n");
				}
#endif
			}

			m_AutoShutdownHelper.SetActiveWindow(this);
		}
	}

	return CSSAutoShutdownPostInitDialog();
}

/**********************************************************************/

void CSSAutoShutdownPropertySheet::OnTimer(UINT nIDEvent)
{
	if (999 == nIDEvent)
	{
		if (FALSE == m_AutoShutdownHelper.IsTimerBlocked())
		{
			if (this == m_AutoShutdownHelper.GetActiveWindow())
			{
				if (m_AutoShutdownHelper.IsTriggered())
				{
					CSSAutoShutdownLogOff();
				}
			}
		}
	}
	else
	{
		CSSAutoShutdownTimer(nIDEvent);
	}
}

/**********************************************************************/

void CSSAutoShutdownPropertySheet::OnDestroy()
{
	if (m_hAutoShutdownTimer != NULL)
	{
		KillTimer(m_hAutoShutdownTimer);
	}

	m_AutoShutdownHelper.ResetShutdownTime();
	CSSAutoShutdownDestroy();
	CPropertySheet::OnDestroy();
}

/**********************************************************************/

void CSSAutoShutdownPropertySheet::PostNcDestroy()
{
	if (m_AutoShutdownHelper.IsTriggered() == TRUE)
	{
		Sleep(500);
	}

	if (m_pPreviousShutdownWindow != NULL)
	{
#ifdef _DEBUG
		TRACE("Reverting to previous shutdown window\n");
#endif
		m_AutoShutdownHelper.SetActiveWindow(m_pPreviousShutdownWindow);
	}
}

/**********************************************************************/
