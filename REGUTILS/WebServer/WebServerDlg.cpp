/**********************************************************************/

#include "WebServer.h"
#include "Defines.h"
#include "WebServerDlg.h"

extern const char* szVERSION_ABOUT;

/**********************************************************************/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**********************************************************************/

CWebServerDlg::CWebServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

/**********************************************************************/

void CWebServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SERVER, m_buttonServer);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CWebServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SERVER, &CWebServerDlg::OnBnClickedButtonServer)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CWebServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	MakeSubdirectory ( PRIVATE_PATH );			// "private"
	MakeSubdirectory ( LOG_PATH );			// "private/logs"
	MakeSubdirectory ( ROOT_PATH );			// "wwwroot"

	m_server.m_pWnd = this;
	m_bServerRunning = FALSE;
	m_buttonServer.SetWindowText ( "Start" );

	OnBnClickedButtonServer();		// start server
	ShowWindow ( SW_MINIMIZE );		// start minimized

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/**********************************************************************/
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWebServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

/**********************************************************************/
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.

HCURSOR CWebServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**********************************************************************/

void CWebServerDlg::OnBnClickedButtonServer()
{
	if ( m_bServerRunning == FALSE )
	{
		CSSIniFile file;
		file.Read ( INI_FILENAME );
		int nPort = file.GetInt ( "Port", 80 );		// default port = 80

		if ( m_server.StartListening ( nPort ) == TRUE )
		{
			CString str;
			str.Format ( "Running: (Port %d)", nPort );
			SetWindowText ( str );

			str.Format ( "Server Started (%s) (Port %d)", szVERSION_ABOUT, nPort );
			m_server.Log ( "", "", str, -1 );

			m_buttonServer.SetWindowText ( "Stop" );
			m_bServerRunning = TRUE;
		}
		else
			AfxMessageBox ( m_server.GetError() );
	}
	else
	{
		SetWindowText ( "Stopped" );

		CString str;
		str.Format ( "Server Stopped (%s)", szVERSION_ABOUT );
		m_server.Log ( "", "", str, -1 );

		m_buttonServer.SetWindowText ( "Start" );
		m_bServerRunning = FALSE;

		if ( m_server.StopListening() == FALSE )
			AfxMessageBox ( m_server.GetError() );
	}
}

/**********************************************************************/

void CWebServerDlg::OnCancel()
{
	if (	m_bServerRunning == TRUE )		// stop server
		OnBnClickedButtonServer();

	CDialog::OnCancel();
}

/**********************************************************************/
