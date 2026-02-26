/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomGeneralDlg.h"
/**********************************************************************/

CEposReportCustomGeneralDlg::CEposReportCustomGeneralDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CDialog(CEposReportCustomGeneralDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomGeneralDlg::~CEposReportCustomGeneralDlg()
{
}

/**********************************************************************/

void CEposReportCustomGeneralDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY, m_checkWeekday);
	DDX_Control(pDX, IDC_CHECK_QTY, m_checkQuantity);
	DDX_Control(pDX, IDC_CHECK_TAXBAND, m_checkTaxBand);
	DDX_Control(pDX, IDC_CHECK_TAXAMOUNT, m_checkTaxAmount);
	DDX_Control(pDX, IDC_CHECK_PROMO_DETAIL, m_checkPromoDetail);
	DDX_Control(pDX, IDC_CHECK_PROMO_SUMMARY, m_checkPromoSummary);
	DDX_Control(pDX, IDC_CHECK_PROMO_POSITIVE, m_checkPromoPositive);
	DDX_Control(pDX, IDC_CHECK_RECONCILE, m_checkReconcile);
	DDX_Control(pDX, IDC_CHECK_SEPARATE_CHANGE, m_checkSeparateChange);
	DDX_Control(pDX, IDC_CHECK_CCDVARIANCE, m_checkCCDVariance);
	DDX_Control(pDX, IDC_CHECK_VOID, m_checkVoid);
	DDX_Control(pDX, IDC_COMBO_VOIDTYPE, m_comboVoidType);
	DDX_Control(pDX, IDC_CHECK_NOSALE, m_checkNoSale);
	DDX_Control(pDX, IDC_CHECK_ADHOC, m_checkAdhoc);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDC_CHECK_CONSOL_DEPT, m_checkConsolDept );
	DDX_Control(pDX, IDC_CHECK_CONSOL_GROUP_EPOS, m_checkConsolGroupEpos );
	DDX_Control(pDX, IDC_CHECK_CONSOL_GROUP_REPORT, m_checkConsolGroupReport );
	DDX_Control(pDX, IDC_CHECK_CONSOL_GROUP_CONSOL, m_checkConsolGroupConsol );
	DDX_Control(pDX, IDC_CHECK_SALES_PROMO, m_checkSalesPromo );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomGeneralDlg, CDialog)
	ON_BN_CLICKED( IDC_CHECK_RECONCILE, OnCheckReconcile)
	ON_BN_CLICKED( IDC_CHECK_QTY, OnCheckVoid)
	ON_BN_CLICKED( IDC_CHECK_QTY, OnCheckVoid)
	ON_BN_CLICKED( IDC_CHECK_CONSOL_GROUP_EPOS, OnCheckEposGroup)
	ON_BN_CLICKED( IDC_CHECK_CONSOL_GROUP_REPORT, OnCheckReportGroup)
	ON_BN_CLICKED( IDC_CHECK_CONSOL_GROUP_CONSOL, OnCheckReportGroup)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomGeneralDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_checkTaxBand.SetWindowText( EcrmanOptions.GetTaxString( "Show sales by %T band" ) );
	m_checkTaxAmount.SetWindowText( EcrmanOptions.GetTaxString( "Show %T amount by band" ) );

	m_editName.LimitText( EPOS_CUSTOM_MAXLEN_NAME );
	m_editName.SetWindowText( m_infoReport.GetCustomReportName() );

	CEposReportCustomSettingsGeneral Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine( strParams );

	m_checkWeekday.SetCheck( Settings.GetWeekdayFlag() );
	m_checkQuantity.SetCheck( Settings.GetQuantityFlag() );
	m_checkTaxBand.SetCheck( Settings.GetTaxBandFlag() );
	m_checkTaxAmount.SetCheck( Settings.GetTaxAmountFlag() );
	m_checkPromoDetail.SetCheck( Settings.GetPromoDetailFlag() );
	m_checkPromoSummary.SetCheck( Settings.GetPromoSummaryFlag() );
	m_checkPromoPositive.SetCheck( Settings.GetPromoPositiveFlag() );
	m_checkReconcile.SetCheck( Settings.GetReconciliationFlag() );
	m_checkSeparateChange.SetCheck( Settings.GetSeparateChangeFlag() );
	m_checkCCDVariance.SetCheck( Settings.GetCCDVarianceFlag() );
	m_checkVoid.SetCheck( Settings.GetVoidFlag() );
	m_checkNoSale.SetCheck( Settings.GetNoSaleFlag() );
	m_checkAdhoc.SetCheck( Settings.GetAllowAdhocFlag() );
	m_checkConsolDept.SetCheck			( Settings.GetConsolDeptFlag() );
	m_checkConsolGroupEpos.SetCheck		( Settings.GetConsolGroupEposFlag() );
	m_checkConsolGroupReport.SetCheck	( Settings.GetConsolGroupReportFlag() );
	m_checkConsolGroupConsol.SetCheck	( Settings.GetConsolGroupConsolFlag() );
	m_checkSalesPromo.SetCheck ( Settings.GetSalesPromoFlag() );

	OnCheckReportGroup();
	OnCheckEposGroup();

	{
		int nSel = Settings.GetVoidQtyType();
		if ( ( nSel < 0 ) || ( nSel > 2 ) )
			nSel = 0;
		
		m_comboVoidType.AddString( "Void Qty => PLU Count (Modifier)" );
		m_comboVoidType.AddString( "Void Qty => PLU Count (Base Item)" );
		m_comboVoidType.AddString( "Void Qty => Void Count" );
		m_comboVoidType.SetCurSel( nSel );
	}

	OnCheckVoid();
	
	bool bEnableSave = TRUE;
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenu );
	bEnableSave &= PasswordArray.GetEnable( PasswordTicks::SetupMenuTexts );

	if ( TRUE == m_bAdhoc )
	{
		m_editName.EnableWindow( FALSE );
		m_checkAdhoc.ShowWindow( SW_HIDE );
		m_checkAdhoc.EnableWindow( FALSE );
		SetWindowText( "Adhoc General Report Settings" );
	}
	
	if ( ( FALSE == m_bAdhoc ) || ( FALSE == bEnableSave ) )
	{
		m_buttonSave.ShowWindow( SW_HIDE );
		m_buttonSave.EnableWindow( FALSE );
	}

	if ( DealerFlags.GetHoltsDiscountFlag() == TRUE )
	{
		m_checkSalesPromo.SetCheck( FALSE );
		m_checkSalesPromo.EnableWindow( FALSE );
	}

	OnCheckReconcile();

	return TRUE;
}

/**********************************************************************/

void CEposReportCustomGeneralDlg::OnCheckReportGroup()
{
	if ( ( m_checkConsolGroupReport.GetCheck() != 0 ) || ( m_checkConsolGroupConsol.GetCheck() != 0 ) )
	{
		m_checkConsolGroupEpos.SetCheck( FALSE );
		m_checkConsolGroupEpos.EnableWindow( FALSE );
	}
	else
		m_checkConsolGroupEpos.EnableWindow( TRUE );
}

/**********************************************************************/

void CEposReportCustomGeneralDlg::OnCheckEposGroup()
{
	if ( m_checkConsolGroupEpos.GetCheck() != 0 )
	{
		m_checkConsolGroupReport.SetCheck( FALSE );
		m_checkConsolGroupConsol.SetCheck( FALSE );
		m_checkConsolGroupReport.EnableWindow( FALSE );
		m_checkConsolGroupConsol.EnableWindow( FALSE );
	}
	else
	{
		m_checkConsolGroupReport.EnableWindow( TRUE );
		m_checkConsolGroupConsol.EnableWindow( TRUE );
	}
}

/**********************************************************************/

void CEposReportCustomGeneralDlg::OnCheckVoid()
{
	m_comboVoidType.EnableWindow ( ( m_checkVoid.GetCheck() != 0 ) && ( m_checkQuantity.GetCheck() != 0 ) );
}

/**********************************************************************/

void CEposReportCustomGeneralDlg::OnCheckReconcile()
{
	m_checkSeparateChange.EnableWindow( m_checkReconcile.GetCheck() != 0 );
}

/**********************************************************************/

bool CEposReportCustomGeneralDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	bool bResult = FALSE;

	bool bFlag1 = ( m_checkConsolDept.GetCheck() != 0 );
	bool bFlag2 = ( m_checkConsolGroupReport.GetCheck() != 0 );
	bool bFlag3 = ( m_checkConsolGroupConsol.GetCheck() != 0 );
	bool bFlag4 = ( m_checkConsolGroupEpos.GetCheck() != 0 );

	if ( ( bFlag1 | bFlag2 | bFlag3 | bFlag4 ) == FALSE )
		Prompter.Error( "You must select at least one sales consolidation type\nfor this custom report." );
	else
	{
		CEposReportCustomSettingsGeneral Settings;
		
		Settings.SetWeekdayFlag( m_checkWeekday.GetCheck() != 0 );
		Settings.SetQuantityFlag( m_checkQuantity.GetCheck() != 0 );
		Settings.SetTaxBandFlag( m_checkTaxBand.GetCheck() != 0 );
		Settings.SetTaxAmountFlag( m_checkTaxAmount.GetCheck() != 0 );
		Settings.SetPromoDetailFlag( m_checkPromoDetail.GetCheck() != 0 );
		Settings.SetPromoSummaryFlag( m_checkPromoSummary.GetCheck() != 0 );
		Settings.SetPromoPositiveFlag( m_checkPromoPositive.GetCheck() != 0 );
		Settings.SetReconciliationFlag( m_checkReconcile.GetCheck() != 0 );
		Settings.SetSeparateChangeFlag( m_checkSeparateChange.GetCheck() != 0 );
		Settings.SetCCDVarianceFlag( m_checkCCDVariance.GetCheck() != 0 );
		Settings.SetVoidFlag( m_checkVoid.GetCheck() != 0 );
		Settings.SetVoidQtyType( m_comboVoidType.GetCurSel() );
		Settings.SetNoSaleFlag( m_checkNoSale.GetCheck() != 0 );
		Settings.SetAllowAdhocFlag( m_checkAdhoc.GetCheck() != 0 );
		Settings.SetConsolDeptFlag( bFlag1 );
		Settings.SetConsolGroupReportFlag( bFlag2 );
		Settings.SetConsolGroupConsolFlag( bFlag3 );
		Settings.SetConsolGroupEposFlag( bFlag4 );
		Settings.SetSalesPromoFlag( m_checkSalesPromo.GetCheck() != 0 );
		Settings.ValidateConsolSettings();

		infoReport.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_GENERAL );

		CString strText;
		m_editName.GetWindowText( strText );
		infoReport.SetCustomReportName( strText );

		CString strLine;
		Settings.GetSettingsCSVLine( strLine );
		infoReport.SetCustomReportParams( strLine );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CEposReportCustomGeneralDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomGeneralDlg::OnOK() 
{
	if ( GetUpdatedSettings( m_infoReport ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/


