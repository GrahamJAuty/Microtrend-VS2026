/**********************************************************************/
#include "SPOSKeyboardSQLImportStatsDlg.h"
/**********************************************************************/

CSPOSKeyboardSQLImportStatsDlg::CSPOSKeyboardSQLImportStatsDlg( CSPOSKeyboardSQLImporter& SQLImporter, CWnd* pParent) 
	: CDialog(CSPOSKeyboardSQLImportStatsDlg::IDD, pParent), m_SQLImporter( SQLImporter )
{
	//{{AFX_DATA_INIT(CSPOSKeyboardSQLImportStatsDlg)
	//}}AFX_DATA_INIT
	m_bDelete = FALSE;
	m_bWantLayout = FALSE;
	m_bWantMacros = FALSE;
}

/**********************************************************************/

void CSPOSKeyboardSQLImportStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardSQLImportStatsDlg)
	DDX_Control(pDX, IDC_STATIC_MACROS, m_staticMacros);
	DDX_Control(pDX, IDC_STATIC_SCREENMODES, m_staticScreenModes);
	DDX_Control(pDX, IDC_STATIC_LISTITEMS, m_staticListItems);
	DDX_Control(pDX, IDC_STATIC_LISTHEADERS, m_staticListHeaders);
	DDX_Control(pDX, IDC_STATIC_COLOURS, m_staticColours);
	DDX_Check(pDX, IDC_CHECK_LAYOUT, m_bWantLayout);
	DDX_Check(pDX, IDC_CHECK_MACRO, m_bWantMacros);
	DDX_Check(pDX, IDC_CHECK_DELETE, m_bDelete);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardSQLImportStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CSPOSKeyboardSQLImportStatsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardSQLImportStatsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strText;
	strText.Format( "%d", m_SQLImporter.GetListHeaderCount() );
	m_staticListHeaders.SetWindowText( strText );
	
	strText.Format( "%d", m_SQLImporter.GetListItemCount() );
	m_staticListItems.SetWindowText( strText );
	
	strText.Format( "%d", m_SQLImporter.GetScreenModeCount() );
	m_staticScreenModes.SetWindowText( strText );
	
	strText.Format( "%d", m_SQLImporter.GetSysValuesCount() );
	m_staticColours.SetWindowText( strText );

	strText.Format( "%d", m_SQLImporter.GetMacroCount() );
	m_staticMacros.SetWindowText( strText );
	
	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardSQLImportStatsDlg::OnOK() 
{
	UpdateData( TRUE );

	if ( ( 0 == m_bWantLayout ) && ( 0 == m_bWantMacros ) )
	{
		Prompter.Error ( "Please select the data types to import\n(Layout, Macros or both)" );
		return;
	}

	EndDialog( IDOK );
}

/**********************************************************************/
