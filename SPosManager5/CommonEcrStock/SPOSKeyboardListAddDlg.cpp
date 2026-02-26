/**********************************************************************/
#include "SPOSKeyboardListAddDlg.h"
/**********************************************************************/

CSPOSKeyboardListAddDlg::CSPOSKeyboardListAddDlg( __int64 nVisibleListNo, int nListType, CWnd* pParent)
	: CSSDialog(CSPOSKeyboardListAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardListAddDlg)
	m_bCopy = FALSE;
	//}}AFX_DATA_INIT
	m_strMaxListNo.Format( "%d", MAX_LIST_NO );

	m_nVisibleListNo = nVisibleListNo;
	m_nListType = nListType;
	
	SPOSKeyboardManager.GetListManager() -> GetNextFreeListNo( m_nVisibleListNo, m_nListNo );
	
	if ( m_nListNo == 0 )
		SPOSKeyboardManager.GetListManager() -> GetNextFreeListNo( 0, m_nListNo );

	m_strListNo.Format( "%I64d", m_nListNo );
}

/**********************************************************************/

void CSPOSKeyboardListAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardListAddDlg)
	DDX_Control(pDX, IDC_CHECK_COPY, m_checkCopy);
	DDX_Check(pDX, IDC_CHECK_COPY, m_bCopy);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_LISTNO, m_strListNo);
	DDV_IntegerString(pDX, m_strListNo, "1", m_strMaxListNo ); //CHECKED 28-07-03
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardListAddDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardListAddDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardListAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_LISTNO, SS_NUM, 5, "%d" );

	switch( m_nListType )
	{
	case LISTTYPE_ITEM_STANDARD:
		SetWindowText( "Add Standard List" );
		break;

	case LISTTYPE_ITEM_LARGE:
		SetWindowText( "Add Large List" );
		break;

	case LISTTYPE_FUNCTION_STANDARD:
		SetWindowText( "Add Standard Function List" );
		break;

	case LISTTYPE_FUNCTION_LARGE:
		SetWindowText( "Add Large Function List" );
		break;

	case LISTTYPE_FUNCTION_BOTTOM:
		SetWindowText( "Add Bottom Function Bar" );
		break;

	default:
		SetWindowText( "Add List" );
		break;
	}

	int nListIdx;
	if ( SPOSKeyboardManager.GetListManager() -> FindList ( m_nVisibleListNo, nListIdx ) == FALSE )
		m_checkCopy.ShowWindow ( SW_HIDE );

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardListAddDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_nListNo = atoi( m_strListNo );

		int nListIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindList ( m_nListNo, nListIdx ) == TRUE )
		{
			CString strMsg;
			strMsg.Format ( "List number %d already exists\n\nWould you like to search for the next free list number?", m_nListNo );
			if ( Prompter.YesNo ( strMsg, TRUE ) != IDYES )
				return;

			__int64 nNewListNo;
			SPOSKeyboardManager.GetListManager() -> GetNextFreeListNo( m_nListNo, nNewListNo );
			if ( nNewListNo == 0 )
			{
				strMsg.Format ( "There are no more free list numbers above %d", m_nListNo );
				Prompter.Error ( strMsg );
				return;
			}

			m_nListNo = nNewListNo;
			m_strListNo.Format( "%I64d", m_nListNo );

			UpdateData ( FALSE );
			return;
		}
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
