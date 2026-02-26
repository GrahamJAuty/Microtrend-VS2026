/**********************************************************************/
#include "SPOSKeyboardScreenAddDlg.h"
/**********************************************************************/

CSPOSKeyboardScreenAddDlg::CSPOSKeyboardScreenAddDlg( int nVisibleScreenNo, CWnd* pParent)
	: CSSDialog(CSPOSKeyboardScreenAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardScreenAddDlg)
	m_bCopy = FALSE;
	//}}AFX_DATA_INIT
	m_nVisibleScreenNo = nVisibleScreenNo;
	
	SPOSKeyboardManager.GetListManager() -> GetNextFreeScreenNo( m_nVisibleScreenNo, m_nScreenNo );
	if ( m_nScreenNo == 0 )
		SPOSKeyboardManager.GetListManager() -> GetNextFreeScreenNo( 0, m_nScreenNo );
}

/**********************************************************************/

void CSPOSKeyboardScreenAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardScreenAddDlg)
	DDX_Control(pDX, IDC_CHECK_COPY, m_checkCopy);
	DDX_Check(pDX, IDC_CHECK_COPY, m_bCopy);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_LISTNO, m_nScreenNo);
	DDV_MinMaxInt(pDX, m_nScreenNo, 1, MAX_SCREEN_NO); //CHECKED 28-07-03
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardScreenAddDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardScreenAddDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardScreenAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_LISTNO, SS_NUM, 9, "%d" );

	int nScreenIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindScreen ( m_nVisibleScreenNo, nScreenIdx ) == FALSE )
		m_checkCopy.ShowWindow ( SW_HIDE );
	
	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardScreenAddDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nScreenIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindScreen ( m_nScreenNo, nScreenIdx ) )
		{
			CString strMsg;
			strMsg.Format ( "Screen number %d already exists\nWould you like to search for the next free screen number?", m_nScreenNo );
			if ( Prompter.YesNo ( strMsg, TRUE ) != IDYES )
				return;

			int nNewScreenNo;
			SPOSKeyboardManager.GetListManager() -> GetNextFreeScreenNo( m_nScreenNo, nNewScreenNo );
			if ( nNewScreenNo == 0 )
			{
				strMsg.Format ( "There are no more free screen numbers above %d", nNewScreenNo );
				Prompter.Error ( strMsg );
				return;
			}

			m_nScreenNo = nNewScreenNo;
			UpdateData ( FALSE );
			return;
		}
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
