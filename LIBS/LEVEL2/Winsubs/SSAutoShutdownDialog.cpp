/**********************************************************************/

CSSAutoShutdownDialog::CSSAutoShutdownDialog(CSSAutoShutdownHelper& AutoShutdownHelper, UINT nIDTemplate, CWnd* pParentWnd)
	: CSSDialog(nIDTemplate, pParentWnd), m_AutoShutdownHelper( AutoShutdownHelper)
{
	m_bValidate = TRUE;
	m_hAutoShutdownTimer = NULL;
	m_pPreviousShutdownWindow = NULL;
}

/**********************************************************************/

CSSAutoShutdownDialog::CSSAutoShutdownDialog(CSSAutoShutdownHelper& AutoShutdownHelper, LPCTSTR lpszTemplateName, CWnd* pParentWnd)
	: CSSDialog(lpszTemplateName, pParentWnd), m_AutoShutdownHelper(AutoShutdownHelper)
{
	m_bValidate = TRUE;
	m_hAutoShutdownTimer = NULL;
	m_pPreviousShutdownWindow = NULL;
}

/**********************************************************************/

CSSAutoShutdownDialog::CSSAutoShutdownDialog(CSSAutoShutdownHelper& AutoShutdownHelper )
	: CSSDialog(), m_AutoShutdownHelper(AutoShutdownHelper)
{
	m_bValidate = TRUE;
	m_hAutoShutdownTimer = NULL;
	m_pPreviousShutdownWindow = NULL;
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSSAutoShutdownDialog, CSSDialog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************/

bool CSSAutoShutdownDialog::UpdateData(bool bSave, bool bValidate)
{
	CWnd* pWnd = m_AutoShutdownHelper.GetActiveWindow();
	m_AutoShutdownHelper.SetActiveWindow(NULL);
	bool bResult = CSSDialog::UpdateData(bSave, bValidate);
	m_AutoShutdownHelper.SetActiveWindow(pWnd);
	return bResult;
}

/**********************************************************************/

BOOL CSSAutoShutdownDialog::OnInitDialog()
{
	CSSAutoShutdownPreInitDialog();

	CDialog::OnInitDialog();
	
	if (m_AutoShutdownHelper.IsEnabled() == TRUE)
	{
		if (m_AutoShutdownHelper.IsFinalShutdownMode() == FALSE)
		{
			m_AutoShutdownHelper.ResetShutdownTime();
		}

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
					TRACE( "Uncreated window left active when opening new window\n");
				}
#endif
			}

			m_AutoShutdownHelper.SetActiveWindow(this);
		}
	}

	return CSSAutoShutdownPostInitDialog();
}

/**********************************************************************/

void CSSAutoShutdownDialog::OnTimer(UINT nIDEvent)
{
	if ( 999 == nIDEvent ) 
	{
		if (FALSE == m_AutoShutdownHelper.IsTimerBlocked() )
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

void CSSAutoShutdownDialog::OnDestroy()
{
	if (m_hAutoShutdownTimer != NULL)
	{
		KillTimer(m_hAutoShutdownTimer);
	}

	m_AutoShutdownHelper.ResetShutdownTime();
	CSSAutoShutdownDestroy();
	CSSDialog::OnDestroy();
}

/**********************************************************************/

void CSSAutoShutdownDialog::PostNcDestroy()
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
