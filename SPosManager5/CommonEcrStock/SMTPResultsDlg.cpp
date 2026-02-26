/**********************************************************************/
#include "CustomerStatementErrorInfo.h"
#include "SMTPErrorDlg.h"
/**********************************************************************/
#include "SMTPResultsDlg.h"
/**********************************************************************/
static const int SMTP_TIMER = 1;
/**********************************************************************/
static const int SMTP_RESULTS_FIND_JOB = 0;
static const int SMTP_RESULTS_WAIT_REPLY = 1;
static const int SMTP_RESULTS_ALL_DONE = 2;
/**********************************************************************/
static const int SMTP_TAB_PENDING = 0;
static const int SMTP_TAB_REDIRECTED = 1;
static const int SMTP_TAB_SENT = 2;
static const int SMTP_TAB_INVALID = 3;
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSMTPResultsDlg, CDialog)
/**********************************************************************/

CSMTPResultsDlg::CSMTPResultsDlg(int nSessionType, CSMTPEmailHelpers& SMTPHelpers, CWnd* pParent)
	: CSSDialog(CSMTPResultsDlg::IDD, pParent), m_SMTPHelpers(SMTPHelpers)
{
	m_nWaitingIdx = 0;
	m_nTabPage = 0;
	m_hTimer = NULL;
	m_nState = SMTP_RESULTS_FIND_JOB;
	m_bPauseByUser = FALSE;
	m_bPauseByDlg = FALSE;

	m_pArrayTabCurrent = &m_arrayTabPending;
	m_nSessionType = nSessionType;
	m_nTabType = SMTP_TAB_PENDING;

	for (int n = 0; n < m_SMTPHelpers.GetSessionSize(); n++)
	{
		CSMTPPendingEmailInfo info;
		m_SMTPHelpers.GetSessionEmailInfo(n, info);

		switch (info.m_nStatus)
		{
		case SMTP_PENDING_WAIT_SEND:
		case SMTP_PENDING_WAIT_PREPARE:
		case SMTP_PENDING_WAIT_RETRY:
			m_arrayTabPending.Add(n);
			info.m_nStatus = SMTP_PENDING_WAIT_SEND;
			m_SMTPHelpers.SetSessionEmailInfo(n, info);
			break;

		case SMTP_PENDING_REDIRECTED:
			m_arrayTabRedirected.Add(n);
			break;

		case SMTP_PENDING_SENT:
			m_arrayTabSent.Add(n);
			break;

		case SMTP_PENDING_INVALID:
			m_arrayTabInvalid.Add(n);
			break;
		}
	}

	for (int n = 0; n < m_arrayTabPending.GetSize(); n++)
	{
		m_arrayTabPendingNotSent.Add(n);
	}
}

/**********************************************************************/

CSMTPResultsDlg::~CSMTPResultsDlg()
{
}

/**********************************************************************/

void CSMTPResultsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_STATUS, m_TabStatus);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_buttonPause);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSMTPResultsDlg, CDialog)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_STATUS, OnTcnSelchangeTabs)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSMTPResultsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTitle = "";
	CString strHeader1 = "";
	CString strHeader2 = "";
	CString strHeader3 = "";

	switch (m_nSessionType)
	{
	case SMTP_SESSIONTYPE_CUSTOMER:
		strTitle = "Email Customer Statements";
		strHeader1 = "Location";
		strHeader2 = "Customer";
		strHeader3 = "Result";
		break;

	case SMTP_SESSIONTYPE_PMS:
		strTitle = "Email Booking Confirmation";
		strHeader1 = "Subject";
		strHeader2 = "Guest Name";
		strHeader3 = "Result";
		break;

	case SMTP_SESSIONTYPE_ORDER:
		strTitle = "Email Purchase Orders";
		strHeader1 = "Subject";
		strHeader2 = "Supplier";
		strHeader3 = "Result";
		break;

	case SMTP_SESSIONTYPE_TEST:
		strTitle = "Test SMTP Email Send";
		strHeader1 = "Subject";
		strHeader2 = "";
		strHeader3 = "Result";
		break;
	}

	SetWindowText(strTitle);

	m_listResults.SubclassDlgItem(IDC_LIST, this);

	if (m_nSessionType != SMTP_SESSIONTYPE_TEST)
	{
		m_listResults.InsertColumn(0, strHeader1, LVCFMT_LEFT, 140);
		m_listResults.InsertColumn(1, strHeader2, LVCFMT_LEFT, 130);
		m_listResults.InsertColumn(2, strHeader3, LVCFMT_LEFT, 200);
	}
	else
	{
		m_listResults.InsertColumn(0, strHeader1, LVCFMT_LEFT, 270);
		m_listResults.InsertColumn(1, strHeader3, LVCFMT_LEFT, 200);

	}

	m_listResults.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	if (m_arrayTabPending.GetSize() > 0)
	{
		m_arrayTabPages.Add(SMTP_TAB_PENDING);
	}

	if (m_arrayTabRedirected.GetSize() > 0)
	{
		m_arrayTabPages.Add(SMTP_TAB_REDIRECTED);
	}

	if (m_arrayTabSent.GetSize() > 0)
	{
		m_arrayTabPages.Add(SMTP_TAB_SENT);
	}

	if (m_arrayTabInvalid.GetSize() > 0)
	{
		m_arrayTabPages.Add(SMTP_TAB_INVALID);
	}

	if (m_arrayTabPages.GetSize() == 0)
	{
		m_arrayTabPages.Add(SMTP_TAB_PENDING);
	}

	for (int n = 0; n < m_arrayTabPages.GetSize(); n++)
	{
		switch (m_arrayTabPages.GetAt(n))
		{
		case SMTP_TAB_PENDING:
			m_TabStatus.InsertItem(n, "Pending");
			break;


		case SMTP_TAB_REDIRECTED:
			m_TabStatus.InsertItem(n, "Redirected");
			break;

		case SMTP_TAB_SENT:
			m_TabStatus.InsertItem(n, "Sent");
			break;

		case SMTP_TAB_INVALID:
			m_TabStatus.InsertItem(n, "Exceptions");
			break;
		}
	}

	if (SMTP_SESSIONTYPE_TEST == m_nSessionType)
	{
		ResizeControl(&m_listResults, 337, 68);
		MoveControl(&m_buttonPause, 7, 102);
		MoveControl(&m_buttonCancel, 294, 102);
		ResizeDialog(351,123);
	}

	m_nWaitingIdx = 0;
	m_nTabPage = 0;
	SelectTab();

	m_listResults.SetItemCountEx(m_pArrayTabCurrent->GetSize());
	m_listResults.SetCurSel(0);

	if (m_arrayTabPendingNotSent.GetSize() == 0)
	{
		m_nState = SMTP_RESULTS_ALL_DONE;
	}
	else
	{
		StartTimer();
	}

	return TRUE;
}

/**********************************************************************/

void CSMTPResultsDlg::StartTimer()
{
	if (NULL == m_hTimer)
	{
		m_hTimer = SetTimer(SMTP_TIMER, 1000, NULL);
	}
}

/**********************************************************************/

void CSMTPResultsDlg::StopTimer()
{
	if (NULL != m_hTimer)
	{
		KillTimer(m_hTimer);
		m_hTimer = NULL;
	}
}

/**********************************************************************/

void CSMTPResultsDlg::OnButtonPause()
{
	if (FALSE == m_bPauseByUser)
	{
		m_buttonPause.SetWindowText("Resume");
		m_bPauseByUser = TRUE;
	}
	else
	{
		m_buttonPause.SetWindowText("Pause");
		m_bPauseByUser = FALSE;
	}
}

/**********************************************************************/

void CSMTPResultsDlg::OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	m_nTabPage = m_TabStatus.GetCurSel();
	if ((m_nTabPage < 0) || (m_nTabPage >= m_arrayTabPages.GetSize()))
	{
		m_nTabPage = 0;
	}

	int nNewSel = SelectTab();

	m_listResults.SetItemCountEx(m_pArrayTabCurrent->GetSize());
	m_listResults.SetCurSel(nNewSel);
	m_listResults.Invalidate();
}

/**********************************************************************/

int CSMTPResultsDlg::SelectTab()
{
	m_nTabType = m_arrayTabPages.GetAt( m_nTabPage ); 

	switch ( m_nTabType )
	{
	case SMTP_TAB_PENDING:		
		m_pArrayTabCurrent = &m_arrayTabPending;		
		break;

	case SMTP_TAB_REDIRECTED:
		m_pArrayTabCurrent = &m_arrayTabRedirected;	
		break;

	case SMTP_TAB_SENT:	
		m_pArrayTabCurrent = &m_arrayTabSent;
		break;

	case SMTP_TAB_INVALID:	
		m_pArrayTabCurrent = &m_arrayTabInvalid;	
		break;
	}

	int nNewSel = 0;
	if ( SMTP_TAB_PENDING == m_nTabType )
	{
		int nInfoIdx, nPendingIdx;
		if (GetInfoIdx(nPendingIdx, nInfoIdx) == TRUE)
		{
			nNewSel = nPendingIdx;
		}
	}

	return nNewSel;
}

/**********************************************************************/

void CSMTPResultsDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		CCSV csv('\t');

		int nIndex = pDispInfo->item.iItem;

		if ((nIndex >= 0) && (nIndex < m_pArrayTabCurrent->GetSize()))
		{
			int nInfoIdx = m_pArrayTabCurrent->GetAt(nIndex);
			if ((nInfoIdx >= 0) && (nInfoIdx < m_SMTPHelpers.GetSessionSize()))
			{
				CSMTPPendingEmailInfo info;
				m_SMTPHelpers.GetSessionEmailInfo(nInfoIdx, info);

				CCSV csvFriendlyName(info.m_strFriendlyName);

				CString strError = "";

				switch (info.m_nStatus)
				{
				case SMTP_PENDING_WAIT_SEND:
					strError = "Waiting to Send";
					break;

				case SMTP_PENDING_WAIT_PREPARE:
					strError = "Sending Email";
					break;

				case SMTP_PENDING_REDIRECTED:
					strError = "Redirected to Printer";
					break;

				case SMTP_PENDING_SENT:
					strError = "Sent";
					break;

				case SMTP_PENDING_WAIT_RETRY:
				{
					CCustomerStatementErrorInfo infoError;
					infoError.m_nStatementError = info.m_nContextError;
					infoError.m_nSMTPError = info.m_nSMTPError;
					infoError.GetErrorText(strError);
				}
				break;

				case SMTP_PENDING_INVALID:
				{
					CCustomerStatementErrorInfo infoError;
					infoError.m_nStatementError = info.m_nContextError;
					infoError.m_nSMTPError = SMTP_ERROR_NONE;
					infoError.GetErrorText(strError);
				}
				break;
				}

				csv.Add(csvFriendlyName.GetString(0));

				if (m_nSessionType != SMTP_SESSIONTYPE_TEST)
				{
					csv.Add(csvFriendlyName.GetString(1));
				}

				csv.Add(strError);
			}

			lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
		}
	}
	*pResult = 0;
}

/**********************************************************************/

void CSMTPResultsDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_bPauseByDlg = TRUE;

	if (SMTP_TAB_PENDING == m_nTabType)
	{
		int nPendingIdx = m_listResults.GetCurSel();
		if ((nPendingIdx >= 0) && (nPendingIdx < m_arrayTabPending.GetSize()))
		{
			int nInfoIdx = m_arrayTabPending.GetAt(nPendingIdx);

			CSMTPErrorDlg dlgError(m_nSessionType, m_SMTPHelpers, nInfoIdx, this);

			if (dlgError.GotErrorFile() == TRUE)
			{
				dlgError.DoModal();
			}
		}
	}

	m_bPauseByDlg = FALSE;
	*pResult = 0;
}

/**********************************************************************/

bool CSMTPResultsDlg::GetInfoIdx( int& nPendingIdx, int& nInfoIdx )
{
	bool bResult = FALSE;
	nPendingIdx = 0;
	nInfoIdx = 0;

	if ( ( m_nWaitingIdx >= 0 ) && ( m_nWaitingIdx < m_arrayTabPendingNotSent.GetSize() ) )
	{
		nPendingIdx = m_arrayTabPendingNotSent.GetAt( m_nWaitingIdx );

		if ( ( nPendingIdx >= 0 ) && ( nPendingIdx < m_arrayTabPending.GetSize() ) )
		{
			nInfoIdx = m_arrayTabPending.GetAt( nPendingIdx );
			bResult = ( ( nInfoIdx >= 0 ) && ( nInfoIdx < m_SMTPHelpers.GetSessionSize() ) );
		}
	}

	return bResult;
}

/**********************************************************************/

void CSMTPResultsDlg::SetEmailStatus( int nInfoIdx, int nStatus )
{
	if ( ( nInfoIdx >= 0 ) && ( nInfoIdx < m_SMTPHelpers.GetSessionSize() ) )
	{
		CSMTPPendingEmailInfo info;
		m_SMTPHelpers.GetSessionEmailInfo( nInfoIdx, info );
		info.m_nStatus = nStatus;
		m_SMTPHelpers.SetSessionEmailInfo( nInfoIdx, info );
	}
}

/**********************************************************************/

void CSMTPResultsDlg::SetEmailError( int nInfoIdx, int nContextError, int nSMTPError )
{
	if ( ( nInfoIdx >= 0 ) && ( nInfoIdx < m_SMTPHelpers.GetSessionSize() ) )
	{
		CSMTPPendingEmailInfo info;
		m_SMTPHelpers.GetSessionEmailInfo( nInfoIdx, info );
		info.m_nContextError = nContextError;
		info.m_nSMTPError = nSMTPError;
		m_SMTPHelpers.SetSessionEmailInfo( nInfoIdx, info );
	}
}

/**********************************************************************/

void CSMTPResultsDlg::SelectPendingListItem( int nPendingIdx )
{
	if ( SMTP_TAB_PENDING == m_nTabType )
	{
		m_listResults.SetCurSel( nPendingIdx );
		m_listResults.Invalidate();
	}
}

/**********************************************************************/

void CSMTPResultsDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == m_hTimer)
	{
		StopTimer();

		switch (m_nState)
		{
		case SMTP_RESULTS_FIND_JOB:
			if ((m_arrayTabPendingNotSent.GetSize() != 0) && (FALSE == m_bPauseByUser) && (FALSE == m_bPauseByDlg))
			{
				if ((m_nWaitingIdx < 0) || (m_nWaitingIdx >= m_arrayTabPendingNotSent.GetSize()))
				{
					m_nWaitingIdx = 0;
				}

				int nPendingIdx = 0;
				int nInfoIdx = 0;
				if (GetInfoIdx(nPendingIdx, nInfoIdx) == TRUE)
				{
					SetEmailStatus(nInfoIdx, SMTP_PENDING_WAIT_PREPARE);
					SelectPendingListItem(nPendingIdx);

					int nResult = m_SMTPHelpers.SendSessionEmail(this, nInfoIdx);

					if (SMTP_ERROR_NONE == nResult)
					{
						m_nState = SMTP_RESULTS_WAIT_REPLY;
					}
					else
					{
						SetEmailStatus(nInfoIdx, SMTP_PENDING_WAIT_RETRY);
						SetEmailError(nInfoIdx, CUSTOMER_STATEMENT_ERROR_SMTP, nResult);
						SelectPendingListItem(nPendingIdx);
						m_nWaitingIdx++;
					}
				}
			}
			break;

		case SMTP_RESULTS_WAIT_REPLY:
		{
			int nPendingIdx = 0;
			int nInfoIdx = 0;
			
			if (GetInfoIdx(nPendingIdx, nInfoIdx) == TRUE)
			{
				CString strFileName = m_SMTPHelpers.GetEmailFileName(nInfoIdx);

				if (::FileExists(strFileName + ".OK") == TRUE)
				{
					m_arrayTabPendingNotSent.RemoveAt(m_nWaitingIdx);
					SetEmailStatus(nInfoIdx, SMTP_PENDING_SENT);
					SetEmailError(nInfoIdx, CUSTOMER_STATEMENT_ERROR_NONE, SMTP_ERROR_NONE);
					SelectPendingListItem(nPendingIdx);
					m_nState = SMTP_RESULTS_FIND_JOB;
				}
				else if (::FileExists(strFileName + ".ERR") == TRUE)
				{
					SetEmailStatus(nInfoIdx, SMTP_PENDING_WAIT_RETRY);
					SetEmailError(nInfoIdx, CUSTOMER_STATEMENT_ERROR_SMTP, SMTP_ERROR_EXCEPTION);
					SelectPendingListItem(nPendingIdx);
					m_nState = SMTP_RESULTS_FIND_JOB;
					m_nWaitingIdx++;
				}
			}
		}
		break;
		}

		if (m_arrayTabPendingNotSent.GetSize() == 0)
		{
			m_nState = SMTP_RESULTS_ALL_DONE;
		}

		if (SMTP_RESULTS_ALL_DONE != m_nState)
		{
			StartTimer();
		}
		else
		{
			m_buttonPause.ShowWindow(SW_HIDE);
			m_buttonPause.EnableWindow(FALSE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CSMTPResultsDlg::OnOK()
{
	OnCancel();
}

/**********************************************************************/

void CSMTPResultsDlg::OnCancel()
{
	m_bPauseByDlg = TRUE;
	bool bReallyCancel = TRUE;

	int nWaiting = m_arrayTabPendingNotSent.GetSize();
	if (nWaiting > 0)
	{
		CString str;
		str.Format("There %s %d email%s from this batch still waiting to be sent.\n\nAre you sure you wish to cancel %s email%s.",
			(1 == nWaiting) ? "is" : "are",
			nWaiting,
			(1 == nWaiting) ? "" : "s",
			(1 == nWaiting) ? "this" : "these",
			(1 == nWaiting) ? "" : "s");

		if (Prompter.YesNo(str) != IDYES)
		{
			bReallyCancel = FALSE;
		}
	}

	if (TRUE == bReallyCancel)
	{
		StopTimer();
		EndDialog(IDCANCEL);
	}
	else
	{
		m_bPauseByDlg = FALSE;
	}
}

/**********************************************************************/
