//*******************************************************************
#include "SystemData.h"
#include "ImageHandler.h"
//*******************************************************************
#include "WaitStatusDlg.h"
//*******************************************************************
IMPLEMENT_DYNAMIC(CWaitStatusDlg, CDialog)
//*******************************************************************

CWaitStatusDlg::CWaitStatusDlg(CStatus& status, int nAction, bool bMakeVisible, CWnd* pParent)
	: CSSColourDialog(CWaitStatusDlg::IDD, pParent)
	, m_StatusCheckThreadManager(status, nAction)
{
	switch (nAction)
	{
	case nWAITSTATUS_ACTION_SETSTATUSINSERVICE_CHECKSERVERONLINE:
	case nWAITSTATUS_ACTION_CHECKSERVERONLINE:
	case nWAITSTATUS_ACTION_SETSTATUSON_CHECKSERVERONLINE:
		m_nIntResult = nSTATUS_OFFLINE;
		m_bBoolResult = FALSE;
		break;

	case nWAITSTATUS_ACTION_SETSTATUSON:
	case nWAITSTATUS_ACTION_SETSTATUSOFF:
	case nWAITSTATUS_ACTION_SETSTATUSOUTSERVICE:
	case nWAITSTATUS_ACTION_SETSTATUSINSERVICE:
	case nWAITSTATUS_ACTION_EMPTYCASH:
		m_nIntResult = 0;
		m_bBoolResult = FALSE;
		break;
	}

	m_bMakeVisible = bMakeVisible;
}

//*******************************************************************

CWaitStatusDlg::~CWaitStatusDlg()
{
}

//*******************************************************************

void CWaitStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CWaitStatusDlg, CSSColourDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()
//*******************************************************************

BOOL CWaitStatusDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image ( System.m_strSysFilesImagePath, "WaitStatus" );
	
	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}
	else
	{
		strFilename = "";
	}

	bool bRounded = ( strFilename == "" );

	CSSColourDialog::OnInitDialog(MYCOLOUR_DIALOG, bRounded);
	CSSColourDialog::SetImage ( strFilename, TRANSPARENT_COLOUR );

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "" );
	m_staticPrompt.SetWindowText("Please Wait");

	//CALL TIMER HANDLER DIRECTLY SO THAT THREAD IS STARTED IMMEDIATELY
	m_bInTimer = TRUE;
	HandleTimer();
	m_bInTimer = FALSE;

	if ( FALSE == m_bMakeVisible )
	{
		SetWindowPos(NULL, -2000, -2000, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	m_hTimer = SetTimer(ID_TIMER_WAITSTATUS, 100, NULL);
	
	return TRUE;
}

//*******************************************************************

void CWaitStatusDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_hTimer)
	{
		if (FALSE == m_bInTimer)
		{
			m_bInTimer = TRUE;
			HandleTimer();
			m_bInTimer = FALSE;
		}
	}
}

//*******************************************************************

void CWaitStatusDlg::HandleTimer()
{
	m_StatusCheckThreadManager.CheckThread();

	if ( m_StatusCheckThreadManager.IsThreadDone() == TRUE )
	{
		m_nIntResult = m_StatusCheckThreadManager.GetIntResult();
		m_bBoolResult = m_StatusCheckThreadManager.GetBoolResult();
		KillTimer(m_hTimer);
		EndDialog(IDOK);
	}
}

//*******************************************************************

//INTENTIONALLY EMPTY TO PREVENT DLG CLOSING ON ENTER KEY
void CWaitStatusDlg::OnOK()
{
}

//*******************************************************************

//INTENTIONALLY EMPTY TO PREVENT DLG CLOSING ON ESC KEY
void CWaitStatusDlg::OnCancel()
{
}

//*******************************************************************
