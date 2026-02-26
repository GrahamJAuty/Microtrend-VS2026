/**********************************************************************/
#include "AutoBackupOptions.h"
#include "TimerNumbers.h"
/**********************************************************************/
#include "BackupPromptDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CBackupPromptDlg, CDialog)
/**********************************************************************/

CBackupPromptDlg::CBackupPromptDlg( bool bDefaultWait, CWnd* pParent )
	: CDialog(CBackupPromptDlg::IDD, pParent)
{
	m_bDefaultWait = bDefaultWait;
	m_strDefaultAction = ( bDefaultWait ) ? "Wait 10 minutes" : "Backup Now";
	m_hTimer = NULL;
	m_bInTimer = FALSE;
	m_bBlockTimer = FALSE;
}

/**********************************************************************/

CBackupPromptDlg::~CBackupPromptDlg()
{
}

/**********************************************************************/

void CBackupPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SYSTEM, m_staticSystem);
	DDX_Control(pDX, IDC_STATIC_TIME, m_staticTime);
	DDX_Control(pDX, IDC_STATIC_AUTOSELECT, m_staticAutoSelect);
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);
	DDX_Control(pDX, IDC_BUTTON_ACTION3, m_buttonAction3);
	DDX_Control(pDX, IDC_BUTTON_ACTION4, m_buttonAction4);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBackupPromptDlg, CDialog)
	//{{AFX_MSG_MAP(CBackupPromptDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ACTION1,OnButtonAction1)
	ON_BN_CLICKED(IDC_BUTTON_ACTION2,OnButtonAction2)
	ON_BN_CLICKED(IDC_BUTTON_ACTION3,OnButtonAction3)
	ON_BN_CLICKED(IDC_BUTTON_ACTION4,OnButtonAction4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CBackupPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_staticSystem.SetWindowText( "The Loyalty Manager system data has not been backed up since" );
	
	COleDateTime time;
	AutoBackupOptions.GetOleTimeLastBackup( time );
	
	CString str;
	str.Format( "%2.2d:%2.2d    on    %2.2d / %2.2d / %4.4d",
		time.GetHour(),
		time.GetMinute(),
		time.GetDay(),
		time.GetMonth(),
		time.GetYear() );

	m_staticTime.SetWindowText( str );

	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_AutoSelectTime = timeNow + COleDateTimeSpan( 0, 0, 0, 30 );
	ShowAutoSelectText( timeNow );

	m_hTimer = SetTimer( TIMER_BACKUPPROMPT, 500, NULL );

	if ( TRUE == m_bDefaultWait )
	{
		SetDefID( IDC_BUTTON_ACTION2 );
		m_buttonAction2.SetFocus();
	}
	else
	{
		SetDefID( IDC_BUTTON_ACTION1 );
		m_buttonAction1.SetFocus();
	}

	return FALSE;  
}

/**********************************************************************/

void CBackupPromptDlg::ShowAutoSelectText( COleDateTime& timeCompare )
{
	COleDateTimeSpan spanWait = m_AutoSelectTime - timeCompare;
	COleDateTimeSpan span30 = COleDateTimeSpan( 0, 0, 0, 30 );

	if ( spanWait > span30 )
	{
		spanWait = span30;
		m_AutoSelectTime = timeCompare + span30;
	}

	CString strSeconds;
	strSeconds.Format ( "%d", spanWait.GetSeconds() );
	CString strPlural = ( strSeconds != "1" ) ? "s" : "";
	
	CString strLine = "";
	strLine += "(";
	strLine += m_strDefaultAction;
	strLine += " will be selected automatically in ";
	strLine += strSeconds;
	strLine += " second";
	strLine += strPlural;
	strLine += ")";

	m_staticAutoSelect.SetWindowText( strLine ); 
}

/**********************************************************************/

void CBackupPromptDlg::OnButtonAction1()
{
	m_nSelectedAction = 0;
	EndDialog( IDOK );
}

/**********************************************************************/

void CBackupPromptDlg::OnButtonAction2()
{
	m_nSelectedAction = 1;
	EndDialog( IDOK );
}

/**********************************************************************/

void CBackupPromptDlg::OnButtonAction3()
{
	m_nSelectedAction = 2;
	EndDialog( IDOK );
}

/**********************************************************************/

void CBackupPromptDlg::OnButtonAction4()
{
	m_nSelectedAction = 3;
	EndDialog( IDOK );
}

/**********************************************************************/

void CBackupPromptDlg::OnTimer(UINT nIDEvent) 
{
	if ( TIMER_BACKUPPROMPT == nIDEvent )
	{
		if ( ( FALSE == m_bInTimer ) && ( FALSE == m_bBlockTimer ) )
		{
			m_bInTimer = TRUE;

			COleDateTime timeNow = COleDateTime::GetCurrentTime();
			
			if ( timeNow < m_AutoSelectTime )
				ShowAutoSelectText( timeNow );
			else
			{
				m_bBlockTimer = TRUE;
				m_nSelectedAction = ( m_bDefaultWait ) ? 1 : 0;
				EndDialog( IDOK );
			}

			m_bInTimer = FALSE;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CBackupPromptDlg::OnDestroy()
{	
	if ( m_hTimer != NULL )
		KillTimer( m_hTimer );

	CDialog::OnDestroy();
}

/**********************************************************************/
