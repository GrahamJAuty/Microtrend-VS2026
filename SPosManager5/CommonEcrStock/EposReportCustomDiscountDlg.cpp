/**********************************************************************/
 
/**********************************************************************/
#include "EposReportCustomSettings.h"
#include "Password.h"
/**********************************************************************/
#include "EposReportCustomDiscountDlg.h"
/**********************************************************************/

CEposReportCustomDiscountDlg::CEposReportCustomDiscountDlg( CEposReportSelect& EposReportSelect, CEposReportSelectInfo& infoReport, bool bAdhoc, CWnd* pParent)
: CDialog(CEposReportCustomDiscountDlg::IDD, pParent), m_EposReportSelect( EposReportSelect ), m_infoReport( infoReport )
{
	m_bAdhoc = bAdhoc;
}

/**********************************************************************/

CEposReportCustomDiscountDlg::~CEposReportCustomDiscountDlg()
{
}

/**********************************************************************/

void CEposReportCustomDiscountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_SERVER, m_checkServer);
	DDX_Control(pDX, IDC_CHECK_ITEM, m_checkItem);
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

BEGIN_MESSAGE_MAP(CEposReportCustomDiscountDlg, CDialog)
	ON_BN_CLICKED( IDC_BUTTON_SAVE,OnButtonSave)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomDiscountDlg::OnInitDialog() 
{
	CEposReportCustomSettingsDiscount Settings;
	CString strParams = m_infoReport.GetCustomReportParams();
	Settings.SetSettingsCSVLine( strParams );

	CDialog::OnInitDialog();
	
	m_editName.LimitText( EPOS_CUSTOM_MAXLEN_NAME );
	m_editName.SetWindowText( m_infoReport.GetCustomReportName() );

	m_checkServer.SetCheck( Settings.GetByServerFlag() );
	m_checkItem.SetCheck( Settings.GetSubtotalItemsFlag() );
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
		SetWindowText( "Adhoc Discount Detail Report Settings" );
	}
	
	if ( ( FALSE == m_bAdhoc ) || ( FALSE == bEnableSave ) )
	{
		m_buttonSave.ShowWindow( SW_HIDE );
		m_buttonSave.EnableWindow( FALSE );
	}

	return TRUE;
}

/**********************************************************************/

bool CEposReportCustomDiscountDlg::GetUpdatedSettings( CEposReportSelectInfo& infoReport )
{
	UpdateData( TRUE );

	CEposReportCustomSettingsDiscount Settings;
	Settings.SetByServerFlag( m_checkServer.GetCheck() != 0 );
	Settings.SetSubtotalItemsFlag( m_checkItem.GetCheck() != 0 );

	int nFilter = 0;
	int nMask = 1;
	for ( int n = 0; n <= 8; n++ )
	{
		if ( m_checkOffer[n].GetCheck() != 0 )
			nFilter += nMask;

		nMask *= 2;
	}
	Settings.SetOfferFilter( nFilter );

	Settings.SetAllowAdhocFlag( m_checkAdhoc.GetCheck() != 0 );
	
	infoReport.SetCustomReportFamily( EPOS_CUSTOM_FAMILY_DISCOUNT );

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

void CEposReportCustomDiscountDlg::OnButtonSave()
{
	CEposReportSelectInfo infoReport = m_infoReport;
	
	if ( GetUpdatedSettings( infoReport ) == TRUE )
	{
		m_EposReportSelect.SetReportInfo( infoReport );
		m_EposReportSelect.SaveReportSelection();
	}
}

/**********************************************************************/

void CEposReportCustomDiscountDlg::OnOK() 
{
	if ( GetUpdatedSettings( m_infoReport ) == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/
