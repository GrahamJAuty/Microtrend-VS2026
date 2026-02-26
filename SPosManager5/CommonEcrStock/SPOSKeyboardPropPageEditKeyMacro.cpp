/**********************************************************************/
#include "SPOSKeyboardPropPageEditKeyMacro.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditKeyMacro::CSPOSKeyboardPropPageEditKeyMacro() : CSSPropertyPage(CSPOSKeyboardPropPageEditKeyMacro::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditKeyMacro)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CSPOSKeyboardPropPageEditKeyMacro::~CSPOSKeyboardPropPageEditKeyMacro()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyMacro::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditKeyMacro)
	DDX_Control(pDX, IDC_EDIT_PARENT_TYPE, m_editParentType);
	DDX_Control(pDX, IDC_EDIT_PARENT_NAME, m_editParentName);
	DDX_Control(pDX, IDC_EDIT_PARENT_DATA, m_editParentData);
	DDX_Control(pDX, IDC_STATIC_LOCAL, m_staticLocal);
	DDX_Control(pDX, IDC_CHECK_LOCAL, m_checkLocal);
	DDX_Control(pDX, IDC_EDIT_LOCAL_NAME, m_editLocalName);
	DDX_Control(pDX, IDC_EDIT_LOCAL_DATA, m_editLocalData);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditKeyMacro, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditKeyMacro)
	ON_BN_CLICKED(IDC_CHECK_LOCAL, OnToggleLocal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyMacro::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_editLocalName.LimitText( CascadingMacro::MacroName.Max );
	m_editLocalData.LimitText( CascadingMacro::MacroData.Max );

	CString strParentType = "";
	switch( m_pMacroRecord -> GetParentMacroTypeExternal() )
	{
	case NODE_SYSTEM:	strParentType = "System";		break;
	case NODE_DATABASE:	strParentType = "Database";		break;
	case NODE_LOCATION:	strParentType = "Location";		break;
	}

	CString strName = "";

	if ( strParentType != "" )
	{
		strName = m_pMacroRecord -> GetParentMacroName();

		m_editParentType.SetWindowText( strParentType );
		m_editParentName.SetWindowText( strName );
		m_editParentData.SetWindowText( m_pMacroRecord -> GetParentMacroData() );
		m_checkLocal.EnableWindow( TRUE );
		
		if ( m_pMacroRecord -> GetLocalMacroFlag() == TRUE )
		{
			strName = m_pMacroRecord -> GetLocalMacroName();

			m_checkLocal.SetCheck( TRUE );
			m_editLocalName.EnableWindow( TRUE );
			m_editLocalData.EnableWindow( TRUE );
			m_editLocalName.SetWindowText(strName );
			m_editLocalData.SetWindowText( m_pMacroRecord -> GetLocalMacroData() );
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
		strName = m_pMacroRecord -> GetLocalMacroName();

		m_editParentType.SetWindowText( "None" );
		m_editParentName.SetWindowText( "" );
		m_editParentData.SetWindowText( "" );
		m_checkLocal.EnableWindow( FALSE );
		m_checkLocal.SetCheck( TRUE );
		m_editLocalName.EnableWindow( TRUE );
		m_editLocalData.EnableWindow( TRUE );
		m_editLocalName.SetWindowText( strName );
		m_editLocalData.SetWindowText( m_pMacroRecord -> GetLocalMacroData() );
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyMacro::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyMacro::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyMacro::OnToggleLocal()
{
	bool bEnable = IsTicked( m_checkLocal );
	m_editLocalName.EnableWindow( bEnable );
	m_editLocalData.EnableWindow( bEnable );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyMacro::SaveRecord()
{	
	if ( IsTicked( m_checkLocal ) )
	{
		m_pMacroRecord -> SetLocalMacroFlag( TRUE );
		m_pMacroRecord -> SetLocalMacroName( GetEditBoxText( m_editLocalName ) );
		m_pMacroRecord -> SetLocalMacroData( GetEditBoxText( m_editLocalData ) );
	}
	else
	{
		m_pMacroRecord -> SetLocalMacroFlag( FALSE );
		m_pMacroRecord -> SetLocalMacroName( "" );
		m_pMacroRecord -> SetLocalMacroData( "" );
	}
}

/**********************************************************************/


