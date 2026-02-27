/**********************************************************************/
#include "AutoBackupOptions.h"
#include "AutoBackupProcessor.h"
/**********************************************************************/
#include "BackupInfoDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CBackupInfoDlg, CDialog)
/**********************************************************************/

CBackupInfoDlg::CBackupInfoDlg(CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper,CBackupInfoDlg::IDD, pParent)
{
}

/**********************************************************************/

CBackupInfoDlg::~CBackupInfoDlg()
{
}

/**********************************************************************/

void CBackupInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LAST, m_editLast);
	DDX_Control(pDX, IDC_EDIT_NEXT, m_editNext);
	DDX_Control(pDX, IDC_EDIT_ATTEMPT, m_editAttempt);
	DDX_Control(pDX, IDC_EDIT_REASON, m_editReason);
	DDX_Control(pDX, IDC_EDIT_PROMPT, m_editPrompt);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CBackupInfoDlg, CSSAutoShutdownDialog)
END_MESSAGE_MAP()
/**********************************************************************/

BOOL CBackupInfoDlg::CSSAutoShutdownPostInitDialog()
{
	int nReason = 0;
	COleDateTime timeNext, timeAttempt, timePrompt;

	if (AutoBackupOptions.HasScheduleChanged() == TRUE)
	{
		AutoBackupProcessor.TestPendingChanges(timeNext, timeAttempt, timePrompt, nReason);
	}
	else
	{
		timeNext = AutoBackupProcessor.GetNextBackupTime();
		timeAttempt = AutoBackupProcessor.GetNextAttemptTime();
		timePrompt = AutoBackupProcessor.GetForcePromptTime();
		nReason = AutoBackupProcessor.GetNextAttemptReason();
	}

	COleDateTime timeLast;
	if (AutoBackupOptions.GetOleTimeLastBackup(timeLast) == TRUE)
	{
		m_editLast.SetWindowText(FormatTimeDate(timeLast));
	}
	else
	{
		m_editLast.SetWindowText("Unknown");
	}

	m_editNext.SetWindowText( FormatTimeDate( timeNext ) ); 
	m_editAttempt.SetWindowText( FormatTimeDate( timeAttempt ) );
	m_editReason.SetWindowText( AutoBackupProcessor.GetBackupReasonText( nReason ) );
	m_editPrompt.SetWindowText( ( timeAttempt == timePrompt ) || ( AutoBackupOptions.GetAlwaysPromptFlag() ) ? "Yes" : "No" );

	return TRUE;  
}

/**********************************************************************/

const char* CBackupInfoDlg::FormatTimeDate( COleDateTime& timeDate )
{
	m_strTimeDate.Format( "%2.2d:%2.2d on %2.2d/%2.2d/%4.4d",
		timeDate.GetHour(),
		timeDate.GetMinute(),
		timeDate.GetDay(),
		timeDate.GetMonth(),
		timeDate.GetYear() );

	return m_strTimeDate;
}

/**********************************************************************/
