//$$******************************************************************
#include "SPosLoyalty4ServerIcon.h"
//$$******************************************************************
#include "..\SPosLoyalty4Shared\FileRemover.h"
//$$******************************************************************
#include "LaunchWaitDlg.h"
//$$******************************************************************

CLaunchWaitDlg::CLaunchWaitDlg( CWnd* pParent)
	: CSSDialog(CLaunchWaitDlg::IDD, pParent)
{
	m_hTimer = NULL;
	m_bInTimer = FALSE;
	m_nCountdown = 10;

	m_strServiceStopFilename = Filenames.GetServerServiceStopFilename();

	CSSFile fileStop;
	fileStop.Open(m_strServiceStopFilename, "wb");
	fileStop.WriteLine("Stop");
	fileStop.Close();
}

//$$******************************************************************

CLaunchWaitDlg::~CLaunchWaitDlg()
{
	CFileRemover FileRemover;
	FileRemover.RemoveFile(m_strServiceStopFilename);
}

//$$******************************************************************

void CLaunchWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COUNTDOWN, m_staticCountdown);
}

//$$******************************************************************
BEGIN_MESSAGE_MAP(CLaunchWaitDlg, CSSDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//$$******************************************************************

BOOL CLaunchWaitDlg::OnInitDialog()
{
	CSSDialog::OnInitDialog();
	m_hTimer = SetTimer(2, 1000, 0);
	return TRUE;
}

//$$******************************************************************

void CLaunchWaitDlg::OnTimer(UINT nIDEvent)
{
	if (FALSE == m_bInTimer)
	{
		if (2 == nIDEvent)
		{
			m_bInTimer = TRUE;
			
			m_nCountdown--;
			if (m_nCountdown < 0)
			{
				m_nCountdown = 0;
			}

			CString str = "";
			str.Format("This message will close automatically in %d seconds", m_nCountdown);
			m_staticCountdown.SetWindowText(str);

			m_bInTimer = FALSE;
		}
	}

	if ((TRUE == theApp.m_bFailIconChange) && (GetTickCount64() > theApp.m_AppStartTime + 30000))
	{
		EndDialog(IDCANCEL);
	}

	if (m_nCountdown <= 0)
	{
		EndDialog(IDCANCEL);
	}
}

//$$******************************************************************

void CLaunchWaitDlg::OnDestroy()
{
	if (m_hTimer != NULL)
	{
		KillTimer(m_hTimer);
	}

	CSSDialog::OnDestroy();
}

//$$******************************************************************
