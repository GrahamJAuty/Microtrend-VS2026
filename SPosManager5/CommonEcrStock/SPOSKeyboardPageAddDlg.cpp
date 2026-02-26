/**********************************************************************/
#include "SPOSKeyboardPageAddDlg.h"
/**********************************************************************/

CSPOSKeyboardPageAddDlg::CSPOSKeyboardPageAddDlg( int nPageNo, int nMaxNewPages, CWnd* pParent)
	: CSSDialog(CSPOSKeyboardPageAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPageAddDlg)
	m_nPagesToAdd = 1;
	m_bCopyVisible = FALSE;
	//}}AFX_DATA_INIT
	m_nMethod = ADD_PAGES_END;
	m_nPageNo = nPageNo;
	m_nMaxNewPages = nMaxNewPages;
}

/**********************************************************************/

void CSPOSKeyboardPageAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPageAddDlg)
	DDX_Control(pDX, IDC_STATIC_MAX, m_staticMax);
	DDX_Control(pDX, IDC_CHECK_COPY, m_checkCopyVisible);
	DDX_Radio(pDX, IDC_RADIO_END, m_nMethod);
	DDX_Check(pDX, IDC_CHECK_COPY, m_bCopyVisible);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO_START, m_radioStart);
	DDX_Control(pDX, IDC_RADIO_END, m_radioEnd);
	DDX_Control(pDX, IDC_RADIO_AFTER, m_radioAfter);
	DDX_Text(pDX, IDC_EDIT_PAGECOUNT, m_nPagesToAdd);
	DDV_MinMaxInt(pDX, m_nPagesToAdd, 1, m_nMaxNewPages );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPageAddDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardPageAddDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CSPOSKeyboardPageAddDlg::GetEditPagesToAdd() { return GetEdit( IDC_EDIT_PAGECOUNT ); }
/**********************************************************************/

BOOL CSPOSKeyboardPageAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	SubclassEdit( IDC_EDIT_PAGECOUNT, SS_NUM, 2, "%d" ); 

	CString strText;
	strText.Format ( "After page %d", m_nPageNo );
	m_radioAfter.SetWindowText( strText );

	strText.Format( "Copy page %d", m_nPageNo );
	m_checkCopyVisible.SetWindowText( strText );

	GetEditPagesToAdd() -> EnableWindow( m_nMaxNewPages > 1 );

	CString strMax;
	strMax.Format ( "new pages (maximum %d)", m_nMaxNewPages );
	m_staticMax.SetWindowText( strMax );

	return TRUE;  
}

/**********************************************************************/
