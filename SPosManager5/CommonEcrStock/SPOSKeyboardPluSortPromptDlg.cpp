/**********************************************************************/
#include "SPOSKeyboardPluSortPromptDlg.h"
/**********************************************************************/

CSPOSKeyboardPluSortPromptDlg::CSPOSKeyboardPluSortPromptDlg( bool bOnePageFromList, bool bMultiPageList, CWnd* pParent)
	: CSSDialog(CSPOSKeyboardPluSortPromptDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_bOnePageFromList = bOnePageFromList;
	m_bMultiPageList = bMultiPageList;
}

/**********************************************************************/

CSPOSKeyboardPluSortPromptDlg::~CSPOSKeyboardPluSortPromptDlg()
{
}

/**********************************************************************/

void CSPOSKeyboardPluSortPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPluSortPromptDlg)
	DDX_Control( pDX, IDC_STATIC_LINE1, m_staticLine1 );
	DDX_Control( pDX, IDC_STATIC_LINE2, m_staticLine2 );
	DDX_Control( pDX, IDC_STATIC_LINE3, m_staticLine3 );
	DDX_Control( pDX, IDC_STATIC_LINE4, m_staticLine4 );
	DDX_Control( pDX, IDC_STATIC_LINE5, m_staticLine5 );
	DDX_Control( pDX, IDC_STATIC_LINE6, m_staticLine6 );
	DDX_Control( pDX, IDC_BUTTON_PREVIOUS, m_buttonPrevious );
	DDX_Control( pDX, IDC_BUTTON_NEXT, m_buttonNext );
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPluSortPromptDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardPluSortPromptDlg)
	ON_BN_CLICKED( IDC_BUTTON_PREVIOUS, OnButtonPrevious )
	ON_BN_CLICKED( IDC_BUTTON_NEXT, OnButtonNext )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPluSortPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strLine1 = "Plu keys have been sorted by description for the current ";
	
	if ( TRUE == m_bOnePageFromList )
		m_staticLine1.SetWindowText( strLine1 + "page." );
	else
		m_staticLine1.SetWindowText( strLine1 + "list." );

	if ( FALSE == m_bMultiPageList )
	{
		m_staticLine2.ShowWindow( SW_HIDE );
		m_buttonPrevious.ShowWindow( SW_HIDE );
		m_buttonNext.ShowWindow( SW_HIDE );
		MoveControl( &m_staticLine3, 24, 95 );
		MoveControl( &m_staticLine4, 106, 95 );
		MoveControl( &m_staticLine5, 24, 111 );
		MoveControl( &m_staticLine6, 106, 111 );
		MoveControl( &m_buttonOK, 161, 140 );
		MoveControl( &m_buttonCancel, 219, 140 );
		ResizeDialog( 276, 175 );
	}
		
	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardPluSortPromptDlg::OnButtonPrevious()
{
	if ( TRUE == m_bMultiPageList )
		m_pParent -> PostMessage( WM_APP + 2, 0, 0 );
}

/**********************************************************************/

void CSPOSKeyboardPluSortPromptDlg::OnButtonNext()
{
	if ( TRUE == m_bMultiPageList )
		m_pParent -> PostMessage( WM_APP + 2, 1, 0 );
}

/**********************************************************************/
