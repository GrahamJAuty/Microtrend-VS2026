/**********************************************************************/
#include "DefEditMacroLocalDlg.h"
/**********************************************************************/

CDefEditMacroLocalDlg::CDefEditMacroLocalDlg( int nIndex, int nLocalType, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditMacroLocalDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditMacroLocalDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_bAllowSpin = bAllowSpin;
	m_pMacroHelpDlg = NULL;
	m_bNoHelpButton = FALSE;

	switch( nLocalType )
	{
	case NODE_DBKEYBOARDSET:
		m_nLocalType = NODE_DBKEYBOARDSET;
		m_pMacroArray = &DataManagerNonDb.CCMacroKeyboard;
		break;

	case NODE_DATABASE:
		m_nLocalType = NODE_DATABASE;
		m_pMacroArray = &DataManagerNonDb.CCMacroDatabase;
		break;

	case NODE_LOCATION:
	default:
		m_nLocalType = NODE_LOCATION;
		m_pMacroArray = &DataManagerNonDb.CCMacroLocation;
		break;
	}
}

/**********************************************************************/

CDefEditMacroLocalDlg::~CDefEditMacroLocalDlg()
{
	if (m_pMacroHelpDlg != NULL)
	{
		delete (m_pMacroHelpDlg);
	}
}

/**********************************************************************/

void CDefEditMacroLocalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditMacroLocalDlg)
	DDX_Control(pDX, IDC_EDIT_PARENT_TYPE, m_editParentType);
	DDX_Control(pDX, IDC_EDIT_PARENT_NAME, m_editParentName);
	DDX_Control(pDX, IDC_EDIT_PARENT_DATA, m_editParentData);
	DDX_Control(pDX, IDC_STATIC_LOCAL, m_staticLocal);
	DDX_Control(pDX, IDC_CHECK_LOCAL, m_checkLocal);
	DDX_Control(pDX, IDC_EDIT_LOCAL_NAME, m_editLocalName);
	DDX_Control(pDX, IDC_EDIT_LOCAL_DATA, m_editLocalData);
	DDX_Control(pDX, IDC_BUTTON_MACROHELP, m_buttonMacroHelp);
	DDX_Control(pDX, IDC_SPIN, m_buttonSpin);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditMacroLocalDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditMacroLocalDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_CHECK_LOCAL, OnToggleLocal)
	ON_BN_CLICKED(IDC_BUTTON_MACROHELP, OnButtonMacroHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditMacroLocalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( FALSE == m_bAllowSpin )
	{
		ShowAndEnableWindow( &m_buttonSpin, FALSE );
	}
	
	m_editLocalName.LimitText( CascadingMacro::MacroName.Max );
	m_editLocalData.LimitText( CascadingMacro::MacroData.Max );

	switch( m_nLocalType )
	{
	case NODE_DBKEYBOARDSET:
		m_staticLocal.SetWindowText( "Keyboard Macro" );
		m_checkLocal.SetWindowText( "Override macro for this keyboard" );
		break;

	case NODE_DATABASE:
		m_staticLocal.SetWindowText( "Database Macro" );
		m_checkLocal.SetWindowText( "Override macro for this database" );
		break;

	case NODE_LOCATION:
	default:
		m_staticLocal.SetWindowText( "Location Macro" );
		m_checkLocal.SetWindowText( "Override macro for this location" );
		break;
	}

	if (TRUE == m_bNoHelpButton)
	{
		ShowAndEnableWindow(&m_buttonMacroHelp, FALSE);
	}

	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditMacroLocalDlg::GetData()
{
	CCascadingMacroCSVRecord Macro;
	m_pMacroArray -> GetAt( m_nIndex, Macro );

	CString strParentType = "";
	switch( Macro.GetParentMacroTypeExternal() )
	{
	case NODE_SYSTEM:	strParentType = "System";		break;
	case NODE_DATABASE:	strParentType = "Database";		break;
	case NODE_LOCATION:	strParentType = "Location";		break;
	}

	CString strName = "";

	if ( strParentType != "" )
	{
		strName = Macro.GetParentMacroName();

		m_editParentType.SetWindowText( strParentType );
		m_editParentName.SetWindowText( strName );
		m_editParentData.SetWindowText( Macro.GetParentMacroData() );
		m_checkLocal.EnableWindow( TRUE );
		
		if ( Macro.GetLocalMacroFlag() == TRUE )
		{
			strName = Macro.GetLocalMacroName();

			m_checkLocal.SetCheck( TRUE );
			m_editLocalName.EnableWindow( TRUE );
			m_editLocalData.EnableWindow( TRUE );
			m_editLocalName.SetWindowText(strName );
			m_editLocalData.SetWindowText( Macro.GetLocalMacroData() );
		}
		else
		{
			m_checkLocal.SetCheck( FALSE );
			m_editLocalName.EnableWindow( FALSE );
			m_editLocalData.EnableWindow( FALSE );
			m_editLocalName.SetWindowText( "" );
			m_editLocalData.SetWindowText( "" );
		}
	}
	else
	{
		strName = Macro.GetLocalMacroName();

		m_editParentType.SetWindowText( "None" );
		m_editParentName.SetWindowText( "" );
		m_editParentData.SetWindowText( "" );
		m_checkLocal.EnableWindow( FALSE );
		m_checkLocal.SetCheck( TRUE );
		m_editLocalName.EnableWindow( TRUE );
		m_editLocalData.EnableWindow( TRUE );
		m_editLocalName.SetWindowText( strName );
		m_editLocalData.SetWindowText( Macro.GetLocalMacroData() );
	}

	CString strTitle;
	strTitle.Format( "M%3.3d %s", Macro.GetMacroNo(), (const char*) strName );
	SetWindowText( strTitle );
}

/**********************************************************************/

void CDefEditMacroLocalDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if ( TRUE == m_bAllowSpin )
	{
		NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
		
		if ( pNMUpDown -> iDelta > 0 )
			SpinPrevious();
		else
			SpinNext();
	}

	*pResult = 0;
}

/**********************************************************************/

void CDefEditMacroLocalDlg::OnDefaultButton() 
{
	if ( TRUE == m_bAllowSpin )
	{
		SpinNext();
	}
}

/**********************************************************************/

void CDefEditMacroLocalDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex > 0 )
		{
			SaveData();
			m_nIndex--;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine ( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditMacroLocalDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nIndex < m_pMacroArray -> GetSize() - 1 )
		{
			SaveData();
			m_nIndex++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nIndex );
		}
	}
}

/**********************************************************************/

void CDefEditMacroLocalDlg::OnToggleLocal()
{
	bool bEnable = IsTicked( m_checkLocal );
	m_editLocalName.EnableWindow( bEnable );
	m_editLocalData.EnableWindow( bEnable );
}

/**********************************************************************/

void CDefEditMacroLocalDlg::OnButtonMacroHelp()
{
	if (FALSE == m_bNoHelpButton)
	{
		if (m_pMacroHelpDlg == NULL)
		{
			m_pMacroHelpDlg = new CMacroHelpDlg(&m_pMacroHelpDlg, this);
			m_pMacroHelpDlg->Create(IDD_MACRO_HELP, this);
		}

		m_pMacroHelpDlg->SetFocus();
	}
}

/**********************************************************************/

void CDefEditMacroLocalDlg::SaveData()
{
	CCascadingMacroCSVRecord Macro;
	m_pMacroArray -> GetAt( m_nIndex, Macro );
	
	if ( IsTicked( m_checkLocal ) )
	{
		Macro.SetLocalMacroFlag( TRUE );
		Macro.SetLocalMacroName( GetEditBoxText( m_editLocalName ) );
		Macro.SetLocalMacroData( GetEditBoxText( m_editLocalData ) );
	}
	else
	{
		Macro.SetLocalMacroFlag( FALSE );
		Macro.SetLocalMacroName( "" );
		Macro.SetLocalMacroData( "" );
	}

	m_pMacroArray -> SetAt( m_nIndex, Macro );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditMacroLocalDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
