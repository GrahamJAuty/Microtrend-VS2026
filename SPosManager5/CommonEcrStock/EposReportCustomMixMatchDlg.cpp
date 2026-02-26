/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomMixMatchDlg.h"
/**********************************************************************/

CEposReportCustomMixMatchDlg::CEposReportCustomMixMatchDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CDialog(CEposReportCustomMixMatchDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomMixMatchDlg::~CEposReportCustomMixMatchDlg()
{
}

/**********************************************************************/

void CEposReportCustomMixMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_nReportType);
	DDX_Control(pDX, IDC_CHECK_COVERS, m_checkCovers);
	DDX_Control(pDX, IDC_CHECK_OFFER1, m_checkOffer[0]);
	DDX_Control(pDX, IDC_CHECK_OFFER2, m_checkOffer[1]);
	DDX_Control(pDX, IDC_CHECK_OFFER3, m_checkOffer[2]);
	DDX_Control(pDX, IDC_CHECK_OFFER4, m_checkOffer[3]);
	DDX_Control(pDX, IDC_CHECK_OFFER5, m_checkOffer[4]);
	DDX_Control(pDX, IDC_CHECK_OFFER6, m_checkOffer[5]);
	DDX_Control(pDX, IDC_CHECK_OFFER7, m_checkOffer[6]);
	DDX_Control(pDX, IDC_CHECK_OFFER8, m_checkOffer[7]);
	DDX_Control(pDX, IDC_CHECK_OFFER9, m_checkOffer[8]);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomMixMatchDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomMixMatchDlg::OnInitDialog() 
{
	CEposReportCustomSettingsMixMatch Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine( strParams );

	m_nReportType = Settings.GetReportType();
	if ( ( m_nReportType < 0 ) || ( m_nReportType > 2 ) )
		m_nReportType = 0;
	
	CDialog::OnInitDialog();
	
	m_editName.LimitText( EPOS_CUSTOM_MAXLEN_NAME );
	m_editName.SetWindowText( m_infoReport.GetCustomReportName() );

	m_checkCovers.SetCheck( Settings.GetCoversFlag() );
	m_checkAdhoc.SetCheck( Settings.GetAllowAdhocFlag() );

	int nMask = 1;
	int nFilter = Settings.GetOfferFilter();
	for ( int n = 0; n <= 8; n++ )
	{
		m_checkOffer[n].SetCheck( ( nFilter & nMask ) != 0 );
		nMask *= 2;
	}
	
	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenuTexts );

	if ( TRUE == m_bAdhoc )
	{
		m_editName.EnableWindow( FALSE );
		m_checkAdhoc.ShowWindow( SW_HIDE );
		m_checkAdhoc.EnableWindow( FALSE );
		SetWindowText( "Adhoc Mix Match Usage Report Settings" );
	}
	
	if ( ( FALSE == m_bAdhoc ) || ( FALSE == bEnableSave ) )
	{
		m_buttonSave.ShowWindow( SW_HIDE );
		m_buttonSave.EnableWindow( FALSE );
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportCustomMixMatchDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	UpdateData( TRUE );

	CEposReportCustomSettingsMixMatch Settings;
	Settings.SetReportType( m_nReportType );

	int nFilter = 0;
	int nMask = 1;
	for ( int n = 0; n <= 8; n++ )
	{
		if ( m_checkOffer[n].GetCheck() != 0 )
			nFilter += nMask;

		nMask *= 2;
	}
	Settings.SetOfferFilter( nFilter );
	Settings.SetCoversFlag( m_checkCovers.GetCheck() != 0 );
	Settings.SetAllowAdhocFlag( m_checkAdhoc.GetCheck() != 0 );
	
	infoReport.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_MIXMATCH );

	{
		CString strText;
		m_editName.GetWindowText( strText );
		infoReport.SetCustomReportName( strText );
	}

	CString strLine;
	Settings.GetSettingsCSVLine( strLine );
	infoReport.SetCustomReportParams( strLine );
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCustomMixMatchDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomMixMatchDlg::OnOK() 
{
	if ( GetUpdatedSettings( m_infoReport ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/
