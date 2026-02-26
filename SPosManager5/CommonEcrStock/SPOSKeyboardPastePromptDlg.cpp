/**********************************************************************/
#include "SPOSKeyboardPastePromptDlg.h"
/**********************************************************************/

CSPOSKeyboardPastePromptDlg::CSPOSKeyboardPastePromptDlg( bool bMultiPageList, CWnd* pParent)
	: CSSDialog(CSPOSKeyboardPastePromptDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_bMultiPageList = bMultiPageList;
}

/**********************************************************************/

CSPOSKeyboardPastePromptDlg::~CSPOSKeyboardPastePromptDlg()
{
}

/**********************************************************************/

void CSPOSKeyboardPastePromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPastePromptDlg)
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

BEGIN_MESSAGE_MAP(CSPOSKeyboardPastePromptDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardPastePromptDlg)
	ON_BN_CLICKED( IDC_BUTTON_PREVIOUS, OnButtonPrevious )
	ON_BN_CLICKED( IDC_BUTTON_NEXT, OnButtonNext )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPastePromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( FALSE == m_bMultiPageList )
	{
		m_staticLine2.ShowWindow( SW_HIDE );
		m_buttonPrevious.ShowWindow( SW_HIDE );
		m_buttonNext.ShowWindow( SW_HIDE );
		MoveControl( &m_staticLine3, 24, 52 );
		MoveControl( &m_staticLine4, 106, 52 );
		MoveControl( &m_staticLine5, 24, 68 );
		MoveControl( &m_staticLine6, 106, 68 );
		MoveControl( &m_buttonOK, 161, 98 );
		MoveControl( &m_buttonCancel, 219, 98 );
		ResizeDialog( 276, 133 );
	}
		
	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardPastePromptDlg::OnButtonPrevious()
{
	if ( TRUE == m_bMultiPageList )
		m_pParent -> PostMessage( WM_APP + 2, 0, 0 );
}

/**********************************************************************/

void CSPOSKeyboardPastePromptDlg::OnButtonNext()
{
	if ( TRUE == m_bMultiPageList )
		m_pParent -> PostMessage( WM_APP + 2, 1, 0 );
}

/**********************************************************************/
