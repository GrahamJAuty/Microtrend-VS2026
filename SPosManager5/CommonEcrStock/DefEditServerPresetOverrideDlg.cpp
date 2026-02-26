/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "DefEditServerPresetOverrideDlg.h"
/**********************************************************************/

CDefEditServerPresetOverrideDlg::CDefEditServerPresetOverrideDlg( int nPresetIdx, int nConLevel, int nServerEntityIdx, bool bDeferName, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditServerPresetOverrideDlg::IDD, pParent), m_ServerSelector( m_editServer, m_comboServer ), 
	m_ServerSelectorDefault( m_editServerDefault, m_comboServerDefault ),
	m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	//{{AFX_DATA_INIT(CDefEditServerPresetOverrideDlg)
	//}}AFX_DATA_INIT

	m_nPresetIdx = nPresetIdx;
	m_nServerEntityIdx = nServerEntityIdx;
	m_bDeferName = bDeferName;
	
	switch( nConLevel )
	{
	case NODE_DATABASE:
	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
		m_nConLevel = nConLevel;
		break;

	default:
		m_nConLevel = NODE_DATABASE;
		break;
	}

	m_pListDataDlg = NULL;
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditServerPresetOverrideDlg)
	DDX_Control(pDX, IDC_CHECK_OVERRIDE, m_checkOverride);
	DDX_Control(pDX, IDC_STATIC_ORIGIN, m_staticOrigin);
	DDX_Control(pDX, IDC_EDIT_SERVER_DEFAULT, m_editServerDefault);
	DDX_Control( pDX, IDC_STATIC_NAME, m_staticName );
	DDX_Control( pDX, IDC_STATIC_SETTINGS, m_staticSettings );
	DDX_Control( pDX, IDC_EDIT_SERVER, m_editServer );
	DDX_Control( pDX, IDC_COMBO_SERVER, m_comboServer );
	DDX_Control( pDX, IDC_EDIT_SERVER_DEFAULT, m_editServerDefault );
	DDX_Control( pDX, IDC_COMBO_SERVER_DEFAULT, m_comboServerDefault );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditServerPresetOverrideDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditServerPresetOverrideDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_CHECK_OVERRIDE, OnToggleOverride)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_EN_KILLFOCUS(IDC_EDIT_SERVER, OnKillFocusServerNo)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVER, OnSelectServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditServerPresetOverrideDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( TRUE == m_bDeferName )
	{
		m_staticName.SetWindowText( "(Server names are location specific)" );
		m_comboServer.ShowWindow( SW_HIDE );
		m_comboServerDefault.ShowWindow( SW_HIDE );
	}
	else
	{
		m_staticName.SetWindowText( "" );
		m_staticName.ShowWindow( SW_HIDE );
	}

	m_editServerDefault.EnableWindow( FALSE );
	m_comboServerDefault.EnableWindow( FALSE );

	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		m_staticSettings.SetWindowText( "Database Settings" );
		break;

	case NODE_LOCATION:
		m_staticSettings.SetWindowText( "Location Settings" );
		break;

	case NODE_LOCATION_TERMINAL:
		m_staticSettings.SetWindowText( "Terminal Settings" );
		break;
	}

	GetData();
	return FALSE;  
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::SpinPrevious()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nPresetIdx <= 0 )
			return;

		SaveData();
		m_nPresetIdx--;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine( m_nPresetIdx );
	}
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::SpinNext()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( m_nPresetIdx < DataManagerNonDb.ServerPresetSystem.GetPresetCount() - 1 )
		{
			SaveData();
			m_nPresetIdx++;
			GetData();

			if ( m_pListDataDlg != NULL )
				m_pListDataDlg -> SelectLine( m_nPresetIdx );
		}
	}
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::GetData()
{
	CServerPresetMiniInfo infoMini;
	
	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		DataManagerNonDb.ServerPresetDatabase.GetPreset( m_nPresetIdx, infoMini );
		break;

	case NODE_LOCATION:
		DataManagerNonDb.ServerPresetLocation.GetPreset( m_nPresetIdx, infoMini );
		break;

	case NODE_LOCATION_TERMINAL:
		DataManagerNonDb.ServerPresetTerminal.GetPreset( m_nPresetIdx, infoMini );
		break;
	}

	{
		CString strTitle = "";
		strTitle.Format ( "Server Preset %d", infoMini.m_nPresetNo );
		SetWindowText ( strTitle );
	}

	CWordArray arrayDefaultLevels;
	arrayDefaultLevels.Add( NODE_SYSTEM );
	
	switch ( m_nConLevel )
	{
	case NODE_LOCATION:
		arrayDefaultLevels.InsertAt( 0, NODE_DATABASE );
		break;

	case NODE_LOCATION_TERMINAL:
		arrayDefaultLevels.InsertAt( 0, NODE_DATABASE );
		arrayDefaultLevels.InsertAt( 0, NODE_LOCATION );
		break;
	}

	CString strExtra = "";
	CString strDefaultText = "Default Settings ";
	for ( int n = 0; n < arrayDefaultLevels.GetSize(); n++ )
	{
		int nDefaultLevel = arrayDefaultLevels.GetAt(n); 
		
		switch( nDefaultLevel )
		{
		case NODE_SYSTEM:
			DataManagerNonDb.ServerPresetSystem.GetPreset( m_nPresetIdx, m_infoDefault );
			strExtra = "(From System)";
			break;

		case NODE_DATABASE:
			DataManagerNonDb.ServerPresetDatabase.GetPreset( m_nPresetIdx, m_infoDefault );
			strExtra = "(From Database)";
			break;

		case NODE_LOCATION:
			DataManagerNonDb.ServerPresetLocation.GetPreset( m_nPresetIdx, m_infoDefault );
			strExtra = "(From Location)";
			break;	
		}

		if ( m_infoDefault.m_nServerNo != -1 )
			break;
	}
	
	m_staticOrigin.SetWindowText( strDefaultText + strExtra );

	bool bGotServer = FALSE;
	if ( infoMini.m_nServerNo >= 0 )
	{
		m_checkOverride.SetCheck( TRUE );
		m_editServer.EnableWindow( TRUE );
		m_comboServer.EnableWindow( FALSE == m_bDeferName );
		bGotServer = TRUE;
	}
	else
	{
		m_checkOverride.SetCheck( FALSE );
		m_editServer.EnableWindow( FALSE );
		m_comboServer.EnableWindow( FALSE );
		infoMini.m_nServerNo = m_infoDefault.m_nServerNo;
	}

	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_DB:
		m_ServerSelector.FillServerComboDatabase( m_nServerEntityIdx, infoMini.m_nServerNo );
		m_ServerSelectorDefault.FillServerComboDatabase( m_nServerEntityIdx, m_infoDefault.m_nServerNo );
		break;

	case SERVER_LOC:
		m_ServerSelector.FillServerComboLocation( m_nServerEntityIdx, infoMini.m_nServerNo );
		m_ServerSelectorDefault.FillServerComboLocation( m_nServerEntityIdx, m_infoDefault.m_nServerNo );
		break;

	default:
		m_ServerSelector.FillServerComboSystem( infoMini.m_nServerNo );
		m_ServerSelectorDefault.FillServerComboSystem( m_infoDefault.m_nServerNo );
		break;
	}
		
	if ( TRUE == bGotServer )
	{
		m_editServer.SetSel ( 0, -1 );
		m_editServer.SetFocus();
	}
	else
	{
		m_checkOverride.SetFocus();
	}
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::CopyDefaultSettings()
{
	m_editServer.EnableWindow( FALSE );
	m_comboServer.EnableWindow( FALSE );
	m_ServerSelector.FillServerComboSystem( m_infoDefault.m_nServerNo );
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::SaveData()
{
	CServerPresetMiniInfo infoMini;

	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		DataManagerNonDb.ServerPresetDatabase.GetPreset( m_nPresetIdx, infoMini );
		break;

	case NODE_LOCATION:
		DataManagerNonDb.ServerPresetLocation.GetPreset( m_nPresetIdx, infoMini );
		break;

	case NODE_LOCATION_TERMINAL:
		DataManagerNonDb.ServerPresetTerminal.GetPreset( m_nPresetIdx, infoMini );
		break;
	}

	if ( m_checkOverride.GetCheck() != 0 )
	{
		m_ServerSelector.SelectServerFromEditBox( FALSE == m_bDeferName );
		infoMini.m_nServerNo = m_ServerSelector.GetServerNo();
	}
	else
		infoMini.m_nServerNo = -1;
	
	switch( m_nConLevel )
	{
	case NODE_DATABASE:
		DataManagerNonDb.ServerPresetDatabase.UpdatePreset( m_nPresetIdx, infoMini );
		break;

	case NODE_LOCATION:
		DataManagerNonDb.ServerPresetLocation.UpdatePreset( m_nPresetIdx, infoMini );
		break;

	case NODE_LOCATION_TERMINAL:
		DataManagerNonDb.ServerPresetTerminal.UpdatePreset( m_nPresetIdx, infoMini );
		break;
	}

	m_WarnCancelAfterChange.SetFlag();
 }

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::OnToggleOverride() 
{
	if ( m_checkOverride.GetCheck() != 0 )
	{
		m_editServer.EnableWindow( TRUE );
		m_comboServer.EnableWindow( FALSE == m_bDeferName );
	}
	else
	{
		CopyDefaultSettings();
	}
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::OnKillFocusServerNo()
{
	m_ServerSelector.SelectServerFromEditBox( FALSE == m_bDeferName );
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::OnSelectServer()
{
	if ( FALSE == m_bDeferName )
		m_ServerSelector.SelectServerFromCombo();
}

/**********************************************************************/

void CDefEditServerPresetOverrideDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveData();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
