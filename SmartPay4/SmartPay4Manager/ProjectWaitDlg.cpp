//*******************************************************************
#include "..\SmartPay4Shared\WaitDlg.h"
//*******************************************************************

CWaitDlg::CWaitDlg ( bool bBackgroundMode, CWnd* pParent ) : CDialog()
{
	m_bBackgroundMode = bBackgroundMode;

	if ( m_bBackgroundMode == FALSE )
		DisplayWindow ( pParent);
}

//*******************************************************************

CWaitDlg::CWaitDlg(CWnd* pParent) : CDialog()
{
	m_bBackgroundMode = FALSE;
	DisplayWindow ( pParent);
}

//*******************************************************************

void CWaitDlg::DisplayWindow ( CWnd* pParent )
{
	if ( pParent == NULL )
		pParent = AfxGetMainWnd();

	CDialog::Create (CWaitDlg::IDD, pParent);
	CDialog::CenterWindow();
}

//*******************************************************************

CWaitDlg::~CWaitDlg()
{
	if ( m_bBackgroundMode == FALSE )
		DestroyWindow();
}

//*******************************************************************

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
END_MESSAGE_MAP()

//*******************************************************************
