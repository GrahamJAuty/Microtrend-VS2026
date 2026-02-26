/**********************************************************************/
 
/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomTransactionDlg.h"
/**********************************************************************/

CEposReportCustomTransactionDlg::CEposReportCustomTransactionDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CSSDialog(CEposReportCustomTransactionDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomTransactionDlg::~CEposReportCustomTransactionDlg()
{
}

/**********************************************************************/

void CEposReportCustomTransactionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_FILTERTEXT, m_editFilterText);
	DDX_Control(pDX, IDC_CHECK_ITEMSERVER, m_checkItemServer);
	DDX_Control(pDX, IDC_CHECK_ITEMSALEDATE, m_checkItemSaleDate);
	DDX_Control(pDX, IDC_CHECK_MODTEXT, m_checkModText);
	DDX_Control(pDX, IDC_CHECK_DISCREPANCY, m_checkDiscrepancy);
	DDX_Control(pDX, IDC_CHECK_VERSION, m_checkVersion);
	DDX_Radio(pDX, IDC_RADIO_SORT, m_nSortType);
	DDX_Radio(pDX, IDC_RADIO_SALETIME, m_nTimeType);
	DDX_Radio(pDX, IDC_RADIO_TEXT1, m_nTextType);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomTransactionDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomTransactionDlg::OnInitDialog() 
{
	CEposReportCustomSettingsTransaction Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine( strParams );

	SubclassEdit( IDC_EDIT_SEQNO_FROM, SS_NUM, 6, "%6.6d" );
	SubclassEdit( IDC_EDIT_SEQNO_TO, SS_NUM, 6, "%6.6d" );
	SubclassEdit( IDC_EDIT_TRANSERVER_FROM, SS_NUM, 4, "%4.4d" );
	SubclassEdit( IDC_EDIT_TRANSERVER_TO, SS_NUM, 4, "%4.4d" );
	SubclassEdit( IDC_EDIT_ITEMSERVER_FROM, SS_NUM, 4, "%4.4d" );
	SubclassEdit( IDC_EDIT_ITEMSERVER_TO, SS_NUM, 4, "%4.4d" );

	m_nSortType = Settings.GetSortType();
	if ( ( m_nSortType < 0 ) || ( m_nSortType > 2 ) )
		m_nSortType = 0;

	m_nTimeType = Settings.GetTimeType();
	if ( ( m_nTimeType < 0 ) || ( m_nTimeType > 2 ) )
		m_nTimeType = 0;

	m_nTextType = Settings.GetFilterTextType();
	if ( ( m_nTextType < 0 ) || ( m_nTextType > 2 ) )
		m_nTextType = 0;

	CDialog::OnInitDialog();
	
	m_editName.LimitText( EPOS_CUSTOM_MAXLEN_NAME );
	m_editName.SetWindowText( m_infoReport.GetCustomReportName() );

	m_editFilterText.LimitText( EPOS_CUSTOM_MAXLEN_NAME );
	m_editFilterText.SetWindowText( Settings.GetFilterText() );

	m_checkItemSaleDate.SetCheck( Settings.GetItemSaleDateFlag() );

	m_checkItemServer.SetCheck( Settings.GetShowItemServerFlag() );
	m_checkModText.SetCheck( Settings.GetModifierTextFlag() );
	m_checkDiscrepancy.SetCheck( Settings.GetDiscrepancyFlag() );
	m_checkVersion.SetCheck( Settings.GetCashRSPVersionFlag() );
	m_checkAdhoc.SetCheck( Settings.GetAllowAdhocFlag() );

	UpdateEditBox( GetEditSeqNoFrom(), Settings.GetSeqNoFrom(), 6 );
	UpdateEditBox( GetEditSeqNoTo(), Settings.GetSeqNoTo(), 6 );
	UpdateEditBox( GetEditTranServerFrom(), Settings.GetTranServerFrom(), 4 );
	UpdateEditBox( GetEditTranServerTo(), Settings.GetTranServerTo(), 4 );
	UpdateEditBox( GetEditItemServerFrom(), Settings.GetItemServerFrom(), 4 );
	UpdateEditBox( GetEditItemServerTo(), Settings.GetItemServerTo(), 4 );
	
	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenuTexts );

	if ( TRUE == m_bAdhoc )
	{
		m_editName.EnableWindow( FALSE );
		m_checkAdhoc.ShowWindow( SW_HIDE );
		m_checkAdhoc.EnableWindow( FALSE );
		SetWindowText( "Adhoc Transaction Report Settings" );
	}
	
	if ( ( FALSE == m_bAdhoc ) || ( FALSE == bEnableSave ) )
	{
		m_buttonSave.ShowWindow( SW_HIDE );
		m_buttonSave.EnableWindow( FALSE );
	}

	return TRUE;
}


/**********************************************************************/

void CEposReportCustomTransactionDlg::UpdateEditBox( CEdit* pEdit, int nValue, int nWidth )
{
	CString str;
	str.Format( "%*.*d", nWidth, nWidth, nValue );
	pEdit -> SetWindowText( str );
}

/**********************************************************************/

int CEposReportCustomTransactionDlg::GetEditBoxInt( CEdit* pEdit )
{
	CString str;
	pEdit -> GetWindowText( str );
	return atoi( str );
}

/**********************************************************************/
CEdit* CEposReportCustomTransactionDlg::GetEditSeqNoFrom(){ return GetEdit( IDC_EDIT_SEQNO_FROM ); }
CEdit* CEposReportCustomTransactionDlg::GetEditSeqNoTo(){ return GetEdit( IDC_EDIT_SEQNO_TO ); }
CEdit* CEposReportCustomTransactionDlg::GetEditTranServerFrom(){ return GetEdit( IDC_EDIT_TRANSERVER_FROM ); }
CEdit* CEposReportCustomTransactionDlg::GetEditTranServerTo(){ return GetEdit( IDC_EDIT_TRANSERVER_TO ); }
CEdit* CEposReportCustomTransactionDlg::GetEditItemServerFrom(){ return GetEdit( IDC_EDIT_ITEMSERVER_FROM ); }
CEdit* CEposReportCustomTransactionDlg::GetEditItemServerTo(){ return GetEdit( IDC_EDIT_ITEMSERVER_TO ); }
/**********************************************************************/

bool CEposReportCustomTransactionDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	UpdateData( TRUE );

	CEposReportCustomSettingsTransaction Settings;
	Settings.SetShowItemServerFlag( m_checkItemServer.GetCheck() != 0 );
	Settings.SetModifierTextFlag( m_checkModText.GetCheck() != 0 );
	Settings.SetDiscrepancyFlag( m_checkDiscrepancy.GetCheck() != 0 );
	Settings.SetCashRSPVersionFlag( m_checkVersion.GetCheck() != 0 );
	Settings.SetSortType( m_nSortType );
	Settings.SetTimeType( m_nTimeType );
	Settings.SetFilterTextType( m_nTextType );
	Settings.SetItemSaleDateFlag( m_checkItemSaleDate.GetCheck() != 0 );
	Settings.SetAllowAdhocFlag( m_checkAdhoc.GetCheck() != 0 );
	
	{
		CString strText;
		m_editFilterText.GetWindowText( strText );
		Settings.SetFilterText( strText );
	}

	infoReport.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_TRANSACTION );

	{
		CString strText;
		m_editName.GetWindowText( strText );
		infoReport.SetCustomReportName( strText );
	}

	Settings.SetSeqNoFrom( GetEditBoxInt( GetEditSeqNoFrom() ) );
	Settings.SetSeqNoTo( GetEditBoxInt( GetEditSeqNoTo() ) );
	Settings.SetTranServerFrom( GetEditBoxInt( GetEditTranServerFrom() ) );
	Settings.SetTranServerTo( GetEditBoxInt( GetEditTranServerTo() ) );
	Settings.SetItemServerFrom( GetEditBoxInt( GetEditItemServerFrom() ) );
	Settings.SetItemServerTo( GetEditBoxInt( GetEditItemServerTo() ) );
	
	CString strLine;
	Settings.GetSettingsCSVLine( strLine );
	infoReport.SetCustomReportParams( strLine );
	
	return TRUE;
}

/**********************************************************************/

void CEposReportCustomTransactionDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomTransactionDlg::OnOK() 
{
	if ( GetUpdatedSettings( m_infoReport ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/
