//*******************************************************************
#include "SystemData.h"
#include "ImageHandler.h"
//*******************************************************************
#include "WaitPostTransactionDlg.h"
//*******************************************************************
IMPLEMENT_DYNAMIC(CWaitPostTransactionDlg, CDialog)
//*******************************************************************

CWaitPostTransactionDlg::CWaitPostTransactionDlg(int nAction, CWnd* pParent)
	: CSSColourDialog(CWaitPostTransactionDlg::IDD, pParent),
	m_PostTransactionThreadManager(nAction)
{
}

//*******************************************************************

CWaitPostTransactionDlg::~CWaitPostTransactionDlg()
{
}

//*******************************************************************

void CWaitPostTransactionDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CWaitPostTransactionDlg, CSSColourDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//*******************************************************************

BOOL CWaitPostTransactionDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image ( System.m_strSysFilesImagePath, "WaitPostTransaction" );

	if (image.GetFirst() == TRUE)
	{
		strFilename = image.GetFilepath();
	}
	else
	{
		strFilename = "";
	}

	bool bRounded = ( strFilename == "" );

	CSSColourDialog::OnInitDialog( MYCOLOUR_DIALOG, bRounded );
	CSSColourDialog::SetImage ( strFilename, TRANSPARENT_COLOUR );

	m_staticPrompt.Init(IDC_STATIC_PROMPT, this, BLACK, -1, "" );
	m_staticPrompt.SetWindowText("Please Wait");

	//CALL TIMER HANDLER DIRECTLY SO THAT THREAD IS STARTED IMMEDIATELY
	m_bInTimer = TRUE;
	HandleTimer();
	m_bInTimer = FALSE;

	m_hTimer = SetTimer(ID_TIMER_WAITPOSTTRAN, 100, NULL);
	
	return TRUE;
}

//*******************************************************************

void CWaitPostTransactionDlg::OnTimer(UINT_PTR nIDEvent)
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

void CWaitPostTransactionDlg::HandleTimer()
{
	m_PostTransactionThreadManager.CheckThread();

	if ( m_PostTransactionThreadManager.IsThreadDone() == TRUE )
	{
		m_bBoolResult = m_PostTransactionThreadManager.GetBoolResult();
		m_nIntResult = m_PostTransactionThreadManager.GetIntResult();
		KillTimer(m_hTimer);
		EndDialog(IDOK);
	}
}

//*******************************************************************

//INTENTIONALLY EMPTY TO PREVENT DLG CLOSING ON ENTER KEY
void CWaitPostTransactionDlg::OnOK()
{
}

//*******************************************************************

//INTENTIONALLY EMPTY TO PREVENT DLG CLOSING ON ESC KEY
void CWaitPostTransactionDlg::OnCancel()
{
}

//*******************************************************************
