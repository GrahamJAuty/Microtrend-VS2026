/**********************************************************************/
 
/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomDepositDlg.h"
/**********************************************************************/

CEposReportCustomDepositDlg::CEposReportCustomDepositDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CDialog(CEposReportCustomDepositDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomDepositDlg::~CEposReportCustomDepositDlg()
{
}

/**********************************************************************/

void CEposReportCustomDepositDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_nReportType);
	DDX_Control(pDX, IDC_CHECK_REDEEM, m_checkRedeem);
	DDX_Control(pDX, IDC_CHECK_REFUND, m_checkRefund);
	DDX_Control(pDX, IDC_CHECK_PAYMENT, m_checkPayment);
	DDX_Control(pDX, IDC_CHECK_ENDDATE, m_checkEndDate);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomDepositDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomDepositDlg::OnInitDialog() 
{
	CEposReportCustomSettingsDeposit Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine( strParams );

	m_nReportType = Settings.GetReportType();
	if ( ( m_nReportType < 0 ) || ( m_nReportType > 3 ) )
		m_nReportType = 0;
	
	CDialog::OnInitDialog();
	
	m_editName.LimitText( EPOS_CUSTOM_MAXLEN_NAME );
	m_editName.SetWindowText( m_infoReport.GetCustomReportName() );

	m_checkRefund.SetCheck( Settings.GetShowRefundFlag() );
	m_checkRedeem.SetCheck( Settings.GetShowRedeemFlag() );
	m_checkPayment.SetCheck( Settings.GetShowPaymentFlag() );
	m_checkEndDate.SetCheck( Settings.GetEndDateCutOffFlag() );
	m_checkAdhoc.SetCheck( Settings.GetAllowAdhocFlag() );
	
	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenuTexts );

	if ( TRUE == m_bAdhoc )
	{
		m_editName.EnableWindow( FALSE );
		m_checkAdhoc.ShowWindow( SW_HIDE );
		m_checkAdhoc.EnableWindow( FALSE );
		SetWindowText( "Adhoc Deposit Report Settings" );
	}
	
	if ( ( FALSE == m_bAdhoc ) || ( FALSE == bEnableSave ) )
	{
		m_buttonSave.ShowWindow( SW_HIDE );
		m_buttonSave.EnableWindow( FALSE );
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportCustomDepositDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	UpdateData( TRUE );

	CEposReportCustomSettingsDeposit Settings;
	Settings.SetReportType( m_nReportType );
	Settings.SetShowRefundFlag( m_checkRefund.GetCheck() == 1 );
	Settings.SetShowRedeemFlag( m_checkRedeem.GetCheck() == 1 );
	Settings.SetShowPaymentFlag( m_checkPayment.GetCheck() == 1 );
	Settings.SetEndDateCutOffFlag( m_checkEndDate.GetCheck() == 1 );
	Settings.SetAllowAdhocFlag( m_checkAdhoc.GetCheck() != 0 );
	
	infoReport.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_DEPOSIT );

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

void CEposReportCustomDepositDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomDepositDlg::OnOK() 
{
	if ( GetUpdatedSettings( m_infoReport ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/
