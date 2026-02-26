/**********************************************************************/
 
/**********************************************************************/
#include "PosTrayStartDlg.h"
/**********************************************************************/
static const int RETRY_TIMER = 1;
/**********************************************************************/

CPosTrayStartDlg::CPosTrayStartDlg( CWnd* pParent)
		: CDialog(CPosTrayStartDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPosTrayStartDlg)
	//}}AFX_DATA_INIT
	m_bRetry = TRUE;
	m_nCountdown = 10;
}

/**********************************************************************/

CPosTrayStartDlg::~CPosTrayStartDlg()
{
}

/**********************************************************************/

void CPosTrayStartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPosTrayStartDlg)
	DDX_Control(pDX, IDC_EDIT_TIME, m_editTime);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPosTrayStartDlg, CDialog)
	//{{AFX_MSG_MAP(CPosTrayStartDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPosTrayStartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ShowCountdown();

	m_hTimer = SetTimer( RETRY_TIMER, 1000, NULL );

	if ( m_hTimer == NULL )
	{
		m_bRetry = FALSE;
		EndDialog( IDCANCEL );
	}
	
	return TRUE;
}

/**********************************************************************/

void CPosTrayStartDlg::OnTimer(UINT nIDEvent) 
{
	m_nCountdown--;

	if ( m_nCountdown < 0 )
		m_nCountdown = 0;

	ShowCountdown();

	if ( m_nCountdown <= 0 )
		CancelNow();

	CDialog::OnTimer(nIDEvent);
}

/**********************************************************************/

void CPosTrayStartDlg::ShowCountdown()
{
	CString strTime = "";
	strTime.Format( "%d", m_nCountdown );
	m_editTime.SetWindowText( strTime );
	m_editTime.RedrawWindow();
}

/**********************************************************************/

void CPosTrayStartDlg::OnOK() 
{
	m_bRetry = TRUE;
	CancelNow();
}

/**********************************************************************/

void CPosTrayStartDlg::OnCancel() 
{
	m_bRetry = FALSE;
	CancelNow();
}

/**********************************************************************/

void CPosTrayStartDlg::CancelNow()
{	
	if ( m_hTimer != NULL )
		KillTimer( m_hTimer );

	EndDialog( IDCANCEL );
}

/**********************************************************************/

