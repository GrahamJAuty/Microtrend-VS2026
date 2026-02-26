/**********************************************************************/
#include <direct.h>
/**********************************************************************/
#include "CommsErrors.h"
#include "FileRemover.h"
#include "FtpConnect.h"
#include "IDraughtOptions.h"
#include "NetworkCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "FtpConnectDlg.h"
/**********************************************************************/
static const int UPLOAD_TIMER = 1;
/**********************************************************************/

CFtpConnectDlg::CFtpConnectDlg( int nRetries, bool bWaitOnFail, CWnd* pParent)
		: CDialog(CFtpConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFtpConnectDlg)
	//}}AFX_DATA_INIT
	m_nRetries = nRetries;
	m_bWaitOnFail = bWaitOnFail;

	m_hTimer = NULL;
	m_nConnectCountdown = 1;
	m_nCancelCountdown = 5;
	
	m_nStatus = CONNECT_STATUS_NOT_STARTED;
}

/**********************************************************************/

CFtpConnectDlg::~CFtpConnectDlg()
{
}

/**********************************************************************/

void CFtpConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpConnectDlg)
	DDX_Control(pDX, IDC_EDIT_TIME, m_editTime);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
	DDX_Control(pDX, IDC_EDIT_LOCATION, m_editLocation);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CFtpConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CFtpConnectDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CFtpConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText( "FTP Export to i Draught" );
	m_editLocation.SetWindowText( IDraughtOptions.GetFTPServer() );
	
	m_nStatus = CONNECT_STATUS_NOT_STARTED;
	ShowStatus();

	int nResult = COMMS_ERROR_NONE;

	m_hTimer = SetTimer( UPLOAD_TIMER, 1000, NULL );

	if ( m_hTimer == NULL )
		nResult = COMMS_ERROR_UPLOAD_TIMER;
	
	if ( nResult != COMMS_ERROR_NONE )
	{
		m_nStatus = CONNECT_STATUS_FAILED;
		m_nError = nResult;
		
		ShowStatus();
		m_buttonCancel.SetWindowText( "Close" );

		if ( m_bWaitOnFail == FALSE )
			CancelNow();
	}
	
	return TRUE;
}

/**********************************************************************/

void CFtpConnectDlg::OnTimer(UINT nIDEvent) 
{
	switch( m_nStatus )
	{
	case CONNECT_STATUS_NOT_STARTED:
	case CONNECT_STATUS_WAIT_CONNECT:
		m_nConnectCountdown--;

		if ( m_nConnectCountdown < 0 )
			m_nConnectCountdown = 0;

		if ( m_nConnectCountdown == 0 )
		{
			m_nStatus = CONNECT_STATUS_CONNECTING;
			ShowStatus();

			if ( FtpConnect.ConnectIDraught() == TRUE )
			{
				m_nStatus = CONNECT_STATUS_FINISHED;
				CancelNow();
			}
			else
			{
				m_nRetries--;

				if ( m_nRetries >= 0 )
				{
					m_nStatus = CONNECT_STATUS_WAIT_CONNECT;
					m_nConnectCountdown = 10;
				}
				else
				{
					m_nStatus = CONNECT_STATUS_FAILED;
					m_nError = COMMS_ERROR_FTP_CONNECT;
					m_nFtpError = FtpConnect.GetLastFtpError();

					if ( m_bWaitOnFail == FALSE )
						CancelNow();
				}
			}
		}
		break;
		
	case CONNECT_STATUS_WAIT_CANCEL:
		m_nCancelCountdown--;
		
		if ( m_nCancelCountdown < 0 ) 
			m_nCancelCountdown = 0;

		if ( m_nCancelCountdown == 0 )
			m_nStatus = CONNECT_STATUS_USER_CANCEL;
		
		break;
	}

	ShowStatus();

	switch( m_nStatus )
	{
	case CONNECT_STATUS_FINISHED:
	case CONNECT_STATUS_USER_CANCEL:
		CancelNow();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CFtpConnectDlg::ShowStatus()
{
	CString strStatus = "";
	CString strTime = "";

	switch( m_nStatus )
	{
	case CONNECT_STATUS_NOT_STARTED:
		strStatus = "Waiting to connect";
		break;

	case CONNECT_STATUS_WAIT_CONNECT:
		strStatus = "Waiting to retry connection";
		strTime.Format( "%d", m_nConnectCountdown );
		break;

	case CONNECT_STATUS_WAIT_CANCEL:
		strStatus = "Waiting to cancel connection";
		strTime.Format( "%d", m_nCancelCountdown );
		break;

	case CONNECT_STATUS_CONNECTING:
		strStatus = "Connecting";
		break;

	case CONNECT_STATUS_USER_CANCEL:
		strStatus = "Cancelled";
		break;

	case CONNECT_STATUS_FINISHED:
		strStatus = "Connected";
		break;

	case CONNECT_STATUS_FAILED:
		strStatus = "Connection failed";
		break;

	default:
		strStatus = "Unknown";
		break;
	}

	m_editStatus.SetWindowText( strStatus );
	m_editTime.SetWindowText( strTime );
	m_editStatus.RedrawWindow();
	m_editTime.RedrawWindow();
}

/**********************************************************************/

void CFtpConnectDlg::OnOK() 
{
}

/**********************************************************************/

void CFtpConnectDlg::OnCancel() 
{
	switch( m_nStatus )
	{
	case CONNECT_STATUS_NOT_STARTED:
		break;

	case CONNECT_STATUS_FINISHED:
	case CONNECT_STATUS_FAILED:
	case CONNECT_STATUS_USER_CANCEL:
		CancelNow();
		break;

	case CONNECT_STATUS_WAIT_CONNECT:
		m_nStatus = CONNECT_STATUS_WAIT_CANCEL;
		m_nCancelCountdown = 5;
		m_buttonCancel.SetWindowText( "Resume" );
		ShowStatus();
		break;

	case CONNECT_STATUS_WAIT_CANCEL:
		m_nStatus = CONNECT_STATUS_WAIT_CONNECT;
		m_nConnectCountdown = 5;
		m_buttonCancel.SetWindowText( "Cancel" );
		ShowStatus();
		break;
	}
}

/**********************************************************************/

void CFtpConnectDlg::CancelNow()
{	
	if ( m_hTimer != NULL )
		KillTimer( m_hTimer );

	EndDialog( IDCANCEL );
}

/**********************************************************************/

