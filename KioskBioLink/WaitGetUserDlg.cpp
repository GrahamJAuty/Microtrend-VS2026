//*******************************************************************
#include "SystemData.h"
#include "ImageHandler.h"
//*******************************************************************
#include "WaitGetUserDlg.h"
//*******************************************************************
IMPLEMENT_DYNAMIC(CWaitGetUserDlg, CDialog)
//*******************************************************************

CWaitGetUserDlg::CWaitGetUserDlg(CString strFilename, bool bServerValidate, CStringArray& arrayUserLines, CUserIDRecord& UserIDRecord, CWnd* pParent)
	: CSSColourDialog(CWaitGetUserDlg::IDD, pParent),
	m_GetUserThreadManager(strFilename,bServerValidate,arrayUserLines,UserIDRecord)
{
}

//*******************************************************************

CWaitGetUserDlg::~CWaitGetUserDlg()
{
}

//*******************************************************************

void CWaitGetUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CSSColourDialog::DoDataExchange(pDX);
}

//*******************************************************************
BEGIN_MESSAGE_MAP(CWaitGetUserDlg, CSSColourDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//*******************************************************************

BOOL CWaitGetUserDlg::OnInitDialog()
{
	CString strFilename = "";
	CImageHandler image ( System.m_strSysFilesImagePath, "WaitGetUser" );
	
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

	m_hTimer = SetTimer(ID_TIMER_WAITGETUSER, 100, NULL);
	
	return TRUE;
}

//*******************************************************************

void CWaitGetUserDlg::OnTimer(UINT_PTR nIDEvent)
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

void CWaitGetUserDlg::HandleTimer()
{
	m_GetUserThreadManager.CheckThread();

	if ( m_GetUserThreadManager.IsThreadDone() == TRUE )
	{
		m_bBoolResult = m_GetUserThreadManager.GetBoolResult();
		m_nMessageNo = m_GetUserThreadManager.GetMessageNo();
		KillTimer(m_hTimer);
		EndDialog(IDOK);
	}
}

//*******************************************************************

//INTENTIONALLY EMPTY TO PREVENT DLG CLOSING ON ENTER KEY
void CWaitGetUserDlg::OnOK()
{
}

//*******************************************************************

//INTENTIONALLY EMPTY TO PREVENT DLG CLOSING ON ESC KEY
void CWaitGetUserDlg::OnCancel()
{
}

//*******************************************************************
