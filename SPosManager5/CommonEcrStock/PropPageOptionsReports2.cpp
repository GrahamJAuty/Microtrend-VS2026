/**********************************************************************/
#include "PaymentWarningDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageOptionsReports2.h"
/**********************************************************************/

CPropPageOptionsReports2::CPropPageOptionsReports2() : CSSPropertyPage(CPropPageOptionsReports2::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsReports2)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageOptionsReports2::~CPropPageOptionsReports2()
{
}

/**********************************************************************/

void CPropPageOptionsReports2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsReports2)		
	DDX_Control(pDX, IDC_COMBO_CASH, m_comboReportCash);
	DDX_Control(pDX, IDC_CHECK_RA_PAYMENT1, m_checkRAPayment1);
	DDX_Control(pDX, IDC_CHECK_RA_PAYMENT2, m_checkRAPayment2);
	DDX_Control(pDX, IDC_CHECK_RA_BALANCE1, m_checkRABalance1);
	DDX_Control(pDX, IDC_CHECK_RA_BALANCE2, m_checkRABalance2);
	DDX_Control(pDX, IDC_CHECK_PAYMENT, m_checkPaymentChange);
	DDX_Control(pDX, IDC_CHECK_PAYMENT_QTY, m_checkPaymentQuantity);
	/*****/
	DDX_Control(pDX, IDC_STATIC_VIP, m_staticVIP);
	DDX_Control(pDX, IDC_COMBO_VIP_TYPE, m_comboVIPType);
	DDX_Control(pDX, IDC_COMBO_VIP_LEVEL, m_comboVIPLevel);
	DDX_Control(pDX, IDC_CHECK_VIP_RETAIL, m_checkVIPRetail);
	/*****/
	DDX_Control(pDX, IDC_STATIC_CUSTOMER1, m_staticCustomer1);
	DDX_Control(pDX, IDC_STATIC_CUSTOMER2, m_staticCustomer2);
	DDX_Control(pDX, IDC_STATIC_CUSTOMER3, m_staticCustomer3);
	DDX_Control(pDX, IDC_STATIC_CUSTOMER4, m_staticCustomer4);
	DDX_Control(pDX, IDC_CHECK_TRANSACTION, m_checkTransaction);
	DDX_Control(pDX, IDC_CHECK_STATEMENT, m_checkStatement);
	DDX_Control(pDX, IDC_CHECK_CONTACT, m_checkContact);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsReports2, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsReports2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_RA_BALANCE1, OnToggleRABalance1)
	ON_BN_CLICKED(IDC_CHECK_RA_PAYMENT1, OnToggleRAPayment1)
	ON_BN_CLICKED(IDC_CHECK_PAYMENT, OnTogglePaymentChange)
	ON_BN_CLICKED(IDC_CHECK_CONTACT, OnToggleCustomerContact)
	ON_CBN_SELCHANGE(IDC_COMBO_VIP_TYPE, OnSelectVIPType)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsReports2::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit( IDC_EDIT_BEFORE, SS_NUM, 2, "%d" );
	SubclassEdit( IDC_EDIT_AFTER, SS_NUM, 2, "%d" );
	SubclassEdit( IDC_EDIT_VISIT, SS_NUM, 3, "%d" );

	m_comboReportCash.AddString( "Payment method 1 only" );
	m_comboReportCash.AddString( "All cash methods, report as method 1" );
	m_comboReportCash.AddString( "All cash methods, change as method 1" );
	m_comboReportCash.AddString( "All cash methods with own change" );
	m_comboReportCash.SetItemData(0,0);
	m_comboReportCash.SetItemData(1,2);
	m_comboReportCash.SetItemData(2,3);
	m_comboReportCash.SetItemData(3,1);

	m_comboVIPType.AddString( "Disabled" );
	m_comboVIPType.AddString( "All Matching Sales" );
	m_comboVIPType.AddString( "Exclude chargeable legacy sales" );
	
	m_comboVIPLevel.AddString( "None" );
	m_comboVIPLevel.AddString( "Price level 1" );
	m_comboVIPLevel.AddString( "Price level 2" );
	m_comboVIPLevel.AddString( "Price level 3" );
	m_comboVIPLevel.AddString( "Price level 4" );
	m_comboVIPLevel.AddString( "Price level 5" );
	m_comboVIPLevel.AddString( "Price level 6" );
	m_comboVIPLevel.AddString( "Price level 7" );
	m_comboVIPLevel.AddString( "Price level 8" );
	m_comboVIPLevel.AddString( "Price level 9" );
	m_comboVIPLevel.AddString( "Price level 10" );

	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageOptionsReports2::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsReports2::OnSetActive() 
{
	GetRecordData();
	ShowOrHideCustomerArea();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsReports2::OnToggleRAPayment1()
{
	m_checkRAPayment2.EnableWindow( m_checkRAPayment1.GetCheck() != 0 );
}

/**********************************************************************/

void CPropPageOptionsReports2::OnToggleRABalance1()
{
	m_checkRABalance2.EnableWindow( m_checkRABalance1.GetCheck() != 0 );
}

/**********************************************************************/

void CPropPageOptionsReports2::OnTogglePaymentChange()
{
	if ( m_checkPaymentChange.GetCheck() == 0 )
	{
		CPaymentWarningDlg dlg( this );
		dlg.DoModal();
	}
}

/**********************************************************************/

void CPropPageOptionsReports2::OnSelectVIPType()
{
	bool bEnable = ( m_comboVIPType.GetCurSel() != 0 );
	m_comboVIPLevel.EnableWindow( bEnable );
	m_checkVIPRetail.EnableWindow( bEnable );
}

/**********************************************************************/

void CPropPageOptionsReports2::GetRecordData()
{
}

/**********************************************************************/

void CPropPageOptionsReports2::SaveRecord()
{
	bool bRAPayments = ( m_checkRAPayment1.GetCheck() != 0 );
	bool bRABalance = ( m_checkRABalance1.GetCheck() != 0 );

	EcrmanOptions.SetReportsShowCashChangeFlag( m_checkPaymentChange.GetCheck() != 0 );
	EcrmanOptions.SetReportsMultiCashMethod(m_comboReportCash.GetItemDataOfSelection() );
	EcrmanOptions.SetReportsShowPaymentQuantityFlag( m_checkPaymentQuantity.GetCheck() != 0 );
	EcrmanOptions.SetReportsShowRAPaymentsFlag( bRAPayments );
	EcrmanOptions.SetReportsSeparateRAPaymentsFlag( bRAPayments && ( m_checkRAPayment2.GetCheck() != 0 ) );
	EcrmanOptions.SetReportsShowRABalanceFlag( bRABalance );
	EcrmanOptions.SetReportsSeparateRABalanceFlag( bRABalance && ( m_checkRABalance2.GetCheck() != 0 ) );

	int nVIPType = m_comboVIPType.GetCurSel();
	if ( nVIPType < 0 || nVIPType > 2 ) nVIPType = 0;
	EcrmanOptions.SetReportsVIPType( nVIPType );
	
	int nVIPLevel = m_comboVIPLevel.GetCurSel();
	if ( nVIPLevel < 0 || nVIPLevel > 10 ) nVIPLevel = 0;
	EcrmanOptions.SetReportsVIPLevel( nVIPLevel );

	EcrmanOptions.SetReportsVIPRetailFlag( IsTicked( m_checkVIPRetail ) );

	EcrmanOptions.SetReportsCustomerTransactionFlag( IsTicked ( m_checkTransaction ) );
	EcrmanOptions.SetReportsCustomerStatementFlag( IsTicked ( m_checkStatement ) );
	EcrmanOptions.SetReportsCustomerContactFlag( IsTicked ( m_checkContact ) );
	
	EcrmanOptions.SetReportsContactBeforeTime( GetEditBoxInt( *GetEditBefore() ) );
	EcrmanOptions.SetReportsContactAfterTime( GetEditBoxInt( *GetEditAfter() ) );
	EcrmanOptions.SetReportsContactVisitTime( GetEditBoxInt( *GetEditVisit() ) );
}

/**********************************************************************/

void CPropPageOptionsReports2::SetRecordControls()
{
	m_checkPaymentChange.SetCheck( EcrmanOptions.GetReportsShowCashChangeFlag() );	
	m_comboReportCash.SelectUsingItemData( EcrmanOptions.GetReportsMultiCashMethod(), 0 );
	m_checkPaymentQuantity.SetCheck(  EcrmanOptions.GetReportsShowPaymentQuantityFlag() );
	m_checkRAPayment1.SetCheck( EcrmanOptions.GetReportsShowRAPaymentsFlag() );
	m_checkRAPayment2.SetCheck( ( EcrmanOptions.GetReportsSeparateRAPaymentsFlag() != 0 ) & ( m_checkRAPayment1.GetCheck() != 0 ) );
	m_checkRABalance1.SetCheck( EcrmanOptions.GetReportsShowRABalanceFlag() );
	m_checkRABalance2.SetCheck( ( EcrmanOptions.GetReportsSeparateRABalanceFlag() != 0 ) & ( m_checkRABalance1.GetCheck() != 0 ) );
	
	int nVIPType = EcrmanOptions.GetReportsVIPType();
	if ( nVIPType < 0 || nVIPType > 2 ) nVIPType = 0;
	m_comboVIPType.SetCurSel( nVIPType );

	int nVIPLevel = EcrmanOptions.GetReportsVIPLevel();
	if ( nVIPLevel < 0 || nVIPLevel > 10 ) nVIPLevel = 0;
	m_comboVIPLevel.SetCurSel( nVIPLevel );

	m_checkVIPRetail.SetCheck( EcrmanOptions.GetReportsVIPRetailFlag() );

	OnSelectVIPType();
	OnToggleRAPayment1();
	OnToggleRABalance1();

	m_checkTransaction.SetCheck( EcrmanOptions.GetReportsCustomerTransactionFlag() );
	m_checkStatement.SetCheck( EcrmanOptions.GetReportsCustomerStatementFlag() );
	m_checkContact.SetCheck( EcrmanOptions.GetReportsCustomerContactFlag() );

	SetEditBoxInt( *GetEditBefore(), EcrmanOptions.GetReportsContactBeforeTime() );
	SetEditBoxInt( *GetEditAfter(), EcrmanOptions.GetReportsContactAfterTime() );
	SetEditBoxInt( *GetEditVisit(), EcrmanOptions.GetReportsContactVisitTime() );
}

/**********************************************************************/

void CPropPageOptionsReports2::ShowOrHideCustomerArea()
{
	bool bShowCustomer = FALSE;

	if ( SysInfo.IsEcrLiteSystem() == FALSE )
	{
		bShowCustomer = EcrmanOptions.GetFeaturesCustomerBalanceFlag();
	}

	ShowAndEnableWindow( &m_staticCustomer1, bShowCustomer );
	ShowAndEnableWindow( &m_staticCustomer2, bShowCustomer );
	ShowAndEnableWindow( &m_staticCustomer3, bShowCustomer );
	ShowAndEnableWindow( &m_staticCustomer4, bShowCustomer );
	ShowAndEnableWindow( &m_checkTransaction, bShowCustomer );
	ShowAndEnableWindow( &m_checkStatement, bShowCustomer );
	ShowAndEnableWindow( &m_checkContact, bShowCustomer );
	ShowAndEnableWindow( GetEditBefore(), bShowCustomer );
	ShowAndEnableWindow( GetEditAfter(), bShowCustomer );
	ShowAndEnableWindow( GetEditVisit(), bShowCustomer );

	if ( TRUE == bShowCustomer )
	{
		ResizeControl( &m_staticVIP, 230, 102 );
	}
	else
	{
		ResizeControl( &m_staticVIP, 230, 265 );
	}

	OnToggleCustomerContact();
}

/**********************************************************************/

void CPropPageOptionsReports2::OnToggleCustomerContact()
{
	bool bEnable = IsTicked( m_checkContact );
	GetEditBefore() -> EnableWindow( bEnable );
	GetEditAfter() -> EnableWindow( bEnable );
	GetEditVisit() -> EnableWindow( bEnable );
}

/**********************************************************************/
