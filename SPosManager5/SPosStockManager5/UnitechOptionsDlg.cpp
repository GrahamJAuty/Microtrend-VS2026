/**********************************************************************/
#include "UnitechOptions.h"
/**********************************************************************/
#include "UnitechOptionsDlg.h"
/**********************************************************************/

CUnitechOptionsDlg::CUnitechOptionsDlg( CWnd* pParent )
	: CDialog("IDD_UNITECH_OPTIONS", pParent )
{
	//{{AFX_DATA_INIT(CUnitechOptionsDlg)
	//}}AFX_DATA_INIT
	m_nStatusType = UNITECH_STATUS_TYPE_NONE;

	CString strDataPath = "";
	::GetDataProgramPath( strDataPath );
	m_bAllowNetUse = ( ( strDataPath.GetLength() >= 8 ) && ( strDataPath.Left(2) == "\\\\" ) );
}

/**********************************************************************/

void CUnitechOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnitechOptionsDlg)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC2_CHECK_UNITECH, m_checkUnitechEnable );
	DDX_Control( pDX, IDC2_EDIT_PATH_PROGRAM, m_editPathProgram );
	DDX_Control( pDX, IDC2_BUTTON_BROWSE_PROGRAM, m_buttonBrowseProgram );
	DDX_Control( pDX, IDC2_EDIT_FILE_PROGRAM, m_editFileProgram );
	DDX_Control( pDX, IDC2_RADIO_STATUS_NONE, m_radioStatusNone );
	DDX_Control( pDX, IDC2_RADIO_STATUS_REGISTRY, m_radioStatusRegistry );
	DDX_Control( pDX, IDC2_RADIO_STATUS_FILE, m_radioStatusFile );
	DDX_Control( pDX, IDC2_EDIT_PATH_STATUS, m_editPathStatus );
	DDX_Control( pDX, IDC2_BUTTON_BROWSE_STATUS, m_buttonBrowseStatus );
	DDX_Control( pDX, IDC2_EDIT_FILE_STATUS, m_editFileStatus );
	DDX_Control( pDX, IDC2_COMBO_PORT, m_comboPort );
	DDX_Control( pDX, IDC2_COMBO_FASTSPEED, m_comboFastSpeed );
	DDX_Control( pDX, IDC2_STATIC_MAP, m_staticMap );
	DDX_Control( pDX, IDC2_COMBO_MAP, m_comboMap );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CUnitechOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CUnitechOptionsDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC2_CHECK_UNITECH, OnToggleEnable)
	ON_BN_CLICKED( IDC2_BUTTON_BROWSE_PROGRAM, OnButtonBrowseProgram)
	ON_BN_CLICKED( IDC2_BUTTON_BROWSE_STATUS, OnButtonBrowseStatus)
	ON_BN_CLICKED( IDC2_RADIO_STATUS_NONE, OnStatusNone)
	ON_BN_CLICKED( IDC2_RADIO_STATUS_REGISTRY, OnStatusRegistry)
	ON_BN_CLICKED( IDC2_RADIO_STATUS_FILE, OnStatusFile)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CUnitechOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_editPathProgram.LimitText( UNITECH_MAX_LENGTH_FOLDER );
	m_editPathStatus.LimitText( UNITECH_MAX_LENGTH_FOLDER );
	m_editFileProgram.LimitText( UNITECH_MAX_LENGTH_FILENAME );
	m_editFileStatus.LimitText( UNITECH_MAX_LENGTH_FILENAME );

	m_checkUnitechEnable.SetCheck( UnitechOptions.GetEnableFlag() );
	m_editPathProgram.SetWindowText( UnitechOptions.GetProgramFolder() );
	m_editFileProgram.SetWindowText( UnitechOptions.GetProgramFile() );
	m_editPathStatus.SetWindowText( UnitechOptions.GetStatusFolder() );
	m_editFileStatus.SetWindowText( UnitechOptions.GetStatusFile() );

	for ( int n = 1; n <= UNITECH_MAX_COMM_PORT; n++ )
	{
		CString strPort;
		strPort.Format ( "COM%d", n );
		m_comboPort.AddString( strPort );
	}

	int nPort = UnitechOptions.GetComPortNo();
	if ( ( nPort >= 1 ) && ( nPort <= UNITECH_MAX_COMM_PORT ) )
		m_comboPort.SetCurSel( nPort - 1 );
	else
		m_comboPort.SetCurSel( 0 );
	
	m_comboFastSpeed.AddString( "No" );
	m_comboFastSpeed.AddString( "Yes" );
	m_comboFastSpeed.SetCurSel( UnitechOptions.GetFastSpeedFlag() ? 1 : 0 );
	
	SetStatusType( UnitechOptions.GetStatusType() );

	CString strDataPath = "";
	::GetDataProgramPath( strDataPath );

	if ( FALSE == m_bAllowNetUse )
	{
		m_staticMap.ShowWindow( SW_HIDE );
		m_comboMap.ShowWindow( SW_HIDE );
	}
	else
	{
		m_comboMap.AddString( "None" );

		CString str = "G";
		for ( int n = 0; n < 20; n++ )
		{
			str.SetAt( 0, 'G' + n );
			m_comboMap.AddString( str );

			int nFlag = UnitechOptions.GetNetUseFlag();
			if ( ( nFlag >= 0 ) && ( nFlag <= 20 ) )
				m_comboMap.SetCurSel( nFlag );
			else
				m_comboMap.SetCurSel( 0 );
		}
	}

	return TRUE;  
}

/**********************************************************************/

void CUnitechOptionsDlg::OnToggleEnable()
{
	bool bEnable = ( m_checkUnitechEnable.GetCheck() != 0 );
	bool bStatus = ( UNITECH_STATUS_TYPE_FILE == m_nStatusType );
	m_editPathProgram.EnableWindow( bEnable );
	m_buttonBrowseProgram.EnableWindow( bEnable );
	m_editFileProgram.EnableWindow( bEnable );
	m_radioStatusNone.EnableWindow( bEnable );
	m_radioStatusRegistry.EnableWindow( bEnable );
	m_radioStatusFile.EnableWindow( bEnable );
	m_editPathStatus.EnableWindow( bEnable & bStatus );
	m_buttonBrowseStatus.EnableWindow( bEnable & bStatus );
	m_editFileStatus.EnableWindow( bEnable & bStatus );
	m_comboPort.EnableWindow( bEnable );
	m_comboFastSpeed.EnableWindow( bEnable );
	m_comboMap.EnableWindow( bEnable );
}

/**********************************************************************/
void CUnitechOptionsDlg::OnStatusNone(){ SetStatusType( UNITECH_STATUS_TYPE_NONE ); }
void CUnitechOptionsDlg::OnStatusRegistry(){ SetStatusType( UNITECH_STATUS_TYPE_REGISTRY ); }
void CUnitechOptionsDlg::OnStatusFile(){ SetStatusType( UNITECH_STATUS_TYPE_FILE ); }
/**********************************************************************/

void CUnitechOptionsDlg::SetStatusType( int n )
{
	switch(n)
	{
	case UNITECH_STATUS_TYPE_NONE:
	case UNITECH_STATUS_TYPE_REGISTRY:
	case UNITECH_STATUS_TYPE_FILE:
		m_nStatusType = n;
		break;

	default:
		m_nStatusType = UNITECH_STATUS_TYPE_NONE;
		break;
	}

	m_radioStatusNone.SetCheck( UNITECH_STATUS_TYPE_NONE == m_nStatusType );
	m_radioStatusRegistry.SetCheck( UNITECH_STATUS_TYPE_REGISTRY == m_nStatusType );
	m_radioStatusFile.SetCheck( UNITECH_STATUS_TYPE_FILE == m_nStatusType );

	OnToggleEnable();
}

/**********************************************************************/

void CUnitechOptionsDlg::OnButtonBrowseProgram() 
{
	CString strProgramFolder = "";
	if ( BrowseFolder ( strProgramFolder, "Select path to Unitech installation folder", NULL, this ) == TRUE )
		m_editPathProgram.SetWindowText ( strProgramFolder );
}

/**********************************************************************/

void CUnitechOptionsDlg::OnButtonBrowseStatus() 
{
	CString strStatusFolder = "";
	if ( BrowseFolder ( strStatusFolder, "Select path to Unitech status folder", NULL, this ) == TRUE )
		m_editPathStatus.SetWindowText ( strStatusFolder );
}

/**********************************************************************/

void CUnitechOptionsDlg::OnOK() 
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		CString strProgramFolder;
		m_editPathProgram.GetWindowText( strProgramFolder );

		CString strProgramFile;
		m_editFileProgram.GetWindowText( strProgramFile );

		CString strStatusFolder;
		m_editPathStatus.GetWindowText( strStatusFolder );

		CString strStatusFile;
		m_editFileStatus.GetWindowText( strStatusFile );

		UnitechOptions.SetEnableFlag( m_checkUnitechEnable.GetCheck() != 0 );
		UnitechOptions.SetProgramFolder( strProgramFolder );
		UnitechOptions.SetProgramFile( strProgramFile );
		UnitechOptions.SetStatusType( m_nStatusType );
		UnitechOptions.SetStatusFolder( strStatusFolder );
		UnitechOptions.SetStatusFile( strStatusFile );
		UnitechOptions.SetComPortNo( m_comboPort.GetCurSel() + 1 );
		UnitechOptions.SetFastSpeedFlag( m_comboFastSpeed.GetCurSel() == 1 );

		if ( TRUE == m_bAllowNetUse )
			UnitechOptions.SetNetUseFlag( m_comboMap.GetCurSel() );

		EndDialog( IDOK );
	}
}

/**********************************************************************/
