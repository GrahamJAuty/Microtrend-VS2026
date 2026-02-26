/**********************************************************************/
#include "DateSelectorRange.h"
/**********************************************************************/
#include "PropPageOptionsReports1.h"
/**********************************************************************/

CPropPageOptionsReports1::CPropPageOptionsReports1() : CSSPropertyPage(CPropPageOptionsReports1::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsReports1)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageOptionsReports1::~CPropPageOptionsReports1()
{
}

/**********************************************************************/

void CPropPageOptionsReports1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsReports1)
	DDX_Control(pDX, IDC_DATEPICKER_FYEAR, m_DatePickerFYear);
	DDX_Control(pDX, IDC_STATIC_TAXQUARTER, m_staticTaxQuarter);
	DDX_Control(pDX, IDC_COMBO_TAXQUARTER, m_comboTaxQuarter);
	DDX_Control(pDX, IDC_COMBO_WEEK, m_comboWeek);
	DDX_Control(pDX, IDC_COMBO_WEEKNUMBER, m_comboWeekNumber);
	DDX_Control(pDX, IDC_TIMEPICKER_SOD, m_TimePickerSOD);
	DDX_Control(pDX, IDC_TIMEPICKER_TIMEATTEND, m_TimePickerTimeAttend);
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_COMBO_SINGLEDAY, m_comboSingleDay);
	DDX_Control(pDX, IDC_COMBO_PREVIOUSDAY, m_comboPreviousDay);
	DDX_Control(pDX, IDC_CHECK_EODSESSION, m_checkEODSession);
	/*****/
	DDX_Control(pDX, IDC_COMBO_CUSTOMER, m_comboCustomer);
	DDX_Control(pDX, IDC_COMBO_SALESTIME, m_comboSalesTime);
	DDX_Control(pDX, IDC_CHECK_DISCOUNT_DETAIL_SALES, m_checkDiscountDetailSales);
	DDX_Control(pDX, IDC_CHECK_PREMIUM_DETAIL_SALES, m_checkPremiumDetailSales);
	DDX_Control(pDX, IDC_CHECK_NETOFTAX_SALES, m_checkNetOfTaxSales);
	DDX_Control(pDX, IDC_CHECK_DISCOUNT_DETAIL_COVERS, m_checkDiscountDetailCovers);
	DDX_Control(pDX, IDC_CHECK_PREMIUM_DETAIL_COVERS, m_checkPremiumDetailCovers);
	DDX_Control(pDX, IDC_CHECK_NETOFTAX_COVERS, m_checkNetOfTaxCovers);
	DDX_Control(pDX, IDC_CHECK_AUTHSERVER, m_checkAuthServer);
	DDX_Control(pDX, IDC_CHECK_REPORTPOINT, m_checkReportpoint);
	DDX_Control(pDX, IDC_CHECK_REASON, m_checkReason);
	DDX_Control(pDX, IDC_CHECK_GENERAL_PAGE, m_checkGeneralPage);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsReports1, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsReports1)
	ON_BN_CLICKED(IDC_CHECK_DISCOUNT_DETAIL_SALES, OnToggleDiscountDetailSales)
	ON_BN_CLICKED(IDC_CHECK_DISCOUNT_DETAIL_COVERS, OnToggleDiscountDetailCovers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsReports1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetOptionTicks();
	
	m_comboWeek.AddString ( "Sunday" );
	m_comboWeek.AddString ( "Monday" );
	m_comboWeek.AddString ( "Tuesday" );
	m_comboWeek.AddString ( "Wednesday" );
	m_comboWeek.AddString ( "Thursday" );
	m_comboWeek.AddString ( "Friday" );
	m_comboWeek.AddString ( "Saturday" );

	m_comboTaxQuarter.AddString ( "Jan/Apr/Jul/Oct" );
	m_comboTaxQuarter.AddString ( "Feb/May/Aug/Nov" );
	m_comboTaxQuarter.AddString ( "Mar/Jun/Sep/Dec" );

	m_comboWeekNumber.AddString ( "Calendar Year" );
	m_comboWeekNumber.AddString ( "Financial Year" );

	m_comboSingleDay.AddString( "None" );
	m_comboSingleDay.AddString( "This Week" );
	m_comboSingleDay.AddString( "Last Week" );
	m_comboSingleDay.AddString( "This Week / Last Week" );

	m_comboPreviousDay.AddString( "None" );
	m_comboPreviousDay.AddString( "7/14/21/28 days" );
	m_comboPreviousDay.AddString( "13 weeks" );
	m_comboPreviousDay.AddString( "7/14/21/28 days, 13 weeks" );

	CTime time ( 1999,1,1,0,0,0,-1 );
	m_DatePickerFYear.SetTime ( &time );
	m_DatePickerFYear.SetFormat ( "dd MMMM" );
	
	m_TimePickerSOD.SetFormat ( "HH:mm" );
	m_TimePickerTimeAttend.SetFormat ( "HH:mm" );
	
	CDateSelectorRange::AddStandardReportRanges( m_comboDates );

	m_comboCustomer.AddString( "Sort by last name" );
	m_comboCustomer.AddString( "Sort by first name" );
	m_comboCustomer.AddString( "Sort by customer ID" );

	m_comboSalesTime.AddString( "Filter & Sort by transaction time" );
	m_comboSalesTime.AddString( "Filter & Sort by item sale time" );

	SetRecordControls();
	OnToggleDiscountDetailSales();
	OnToggleDiscountDetailCovers();
	return TRUE;  
}

/**********************************************************************/

void CPropPageOptionsReports1::UpdateTaxQuarterString()
{
	m_staticTaxQuarter.SetWindowText( EcrmanOptions.GetTaxString( "Start of %T quarter" ) );
}

/**********************************************************************/

void CPropPageOptionsReports1::OnToggleDiscountDetailSales()
{
	m_checkPremiumDetailSales.EnableWindow(IsTicked(m_checkDiscountDetailSales));
}

/**********************************************************************/

void CPropPageOptionsReports1::OnToggleDiscountDetailCovers()
{
	m_checkPremiumDetailCovers.EnableWindow(IsTicked(m_checkDiscountDetailCovers));
}

/**********************************************************************/

BOOL CPropPageOptionsReports1::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	bResult &= SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsReports1::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	m_checkNetOfTaxSales.SetWindowText( EcrmanOptions.GetTaxString( "Show sales retail value net of %T" ) );
	m_checkNetOfTaxCovers.SetWindowText( EcrmanOptions.GetTaxString( "Show covers retail value net of %T" ) );
	SetOptionTicks();
	UpdateTaxQuarterString();
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsReports1::SetOptionTicks()
{
	m_checkEODSession.SetCheck( EcrmanOptions.GetReportsEODSessionFlag() );
}

/**********************************************************************/

bool CPropPageOptionsReports1::SaveRecord()
{
	int nWeekday = m_comboWeek.GetCurSel();
	if ( nWeekday < 0 || nWeekday > 6 ) nWeekday = 0;
	EcrmanOptions.SetReportsWeekStartDay ( nWeekday );
	
	int nTaxQuarter = m_comboTaxQuarter.GetCurSel();
	if ( nTaxQuarter < 0 || nTaxQuarter > 2 ) nTaxQuarter = 0;
	EcrmanOptions.SetReportsTaxQuarterStart ( nTaxQuarter );
	
	int nWeekNumbering = m_comboWeekNumber.GetCurSel();
	if ( nWeekNumbering < 0 || nWeekNumbering > 1 ) nWeekNumbering = 0;
	EcrmanOptions.SetReportsWeekNumberingType( nWeekNumbering );
	
	CTime time;
	m_DatePickerFYear.GetTime( time );
	m_strFYear.Format ( "%2.2d%2.2d", time.GetDay(), time.GetMonth() );
	EcrmanOptions.SetReportsFinancialYearStart ( m_strFYear );

	m_TimePickerSOD.GetTime ( time );
	EcrmanOptions.SetReportsDayStartHour ( time.GetHour() );
	EcrmanOptions.SetReportsDayStartMinute ( time.GetMinute() );

	m_TimePickerTimeAttend.GetTime ( time );
	EcrmanOptions.SetReportsTimeAttendSignOutHour ( time.GetHour() );
	EcrmanOptions.SetReportsTimeAttendSignOutMinute ( time.GetMinute() );

	EcrmanOptions.SetReportsSingleDayRanges( m_comboSingleDay.GetCurSel() );
	EcrmanOptions.SetReportsPreviousDayRanges( m_comboPreviousDay.GetCurSel() );
	EcrmanOptions.SetReportsEODSessionFlag( m_checkEODSession.GetCheck() != 0 );
		
	CString strDate;
	m_comboDates.GetWindowText( strDate );
	EcrmanOptions.SetReportsDefaultDateRange( strDate );

	/*****/

	int nSortType = m_comboCustomer.GetCurSel();
	if ( nSortType < 0 || nSortType > 2 ) nSortType = 0;
	EcrmanOptions.SetReportsCustomerSortType( nSortType );

	int nSalesTime = m_comboSalesTime.GetCurSel();
	if ( nSalesTime < 0 || nSalesTime > 1 ) nSalesTime = 1;
	EcrmanOptions.SetReportsSalesTimeType( nSalesTime );

	EcrmanOptions.SetReportsDiscountDetailSalesFlag( m_checkDiscountDetailSales.GetCheck() != 0 );
	EcrmanOptions.SetReportsPremiumDetailSalesFlag(m_checkPremiumDetailSales.GetCheck() != 0);
	EcrmanOptions.SetReportsNetOfTaxSalesFlag( m_checkNetOfTaxSales.GetCheck() != 0 );
	EcrmanOptions.SetReportsDiscountDetailCoversFlag( m_checkDiscountDetailCovers.GetCheck() != 0 );
	EcrmanOptions.SetReportsPremiumDetailCoversFlag(m_checkPremiumDetailCovers.GetCheck() != 0);
	EcrmanOptions.SetReportsNetOfTaxCoversFlag( m_checkNetOfTaxCovers.GetCheck() != 0 );
	EcrmanOptions.SetReportsAuthServerFlag( m_checkAuthServer.GetCheck() != 0 );
	EcrmanOptions.SetReportsGroupReportpointFlag( m_checkReportpoint.GetCheck() != 0 );
	EcrmanOptions.SetReportsEODReasonsFlag( m_checkReason.GetCheck() != 0 );
	EcrmanOptions.SetReportsGeneralPageFlag( m_checkGeneralPage.GetCheck() != 0 );
	
	return TRUE;
}

/**********************************************************************/

void CPropPageOptionsReports1::SetRecordControls()
{
	int nWeekday = EcrmanOptions.GetReportsWeekStartDay();
	if ( nWeekday < 0 || nWeekday > 6 ) nWeekday = 0;
	m_comboWeek.SetCurSel ( nWeekday );
	
	int nTaxQuarter = EcrmanOptions.GetReportsTaxQuarterStart();
	if ( nTaxQuarter < 0 || nTaxQuarter > 2 ) nTaxQuarter = 0;
	m_comboTaxQuarter.SetCurSel ( nTaxQuarter );
	
	int nWeekNumbering = EcrmanOptions.GetReportsWeekNumberingType();
	if ( nWeekNumbering < 0 || nWeekNumbering > 1 ) nWeekNumbering = 0;
	m_comboWeekNumber.SetCurSel( nWeekNumbering );

	int nType = EcrmanOptions.GetReportsSingleDayRanges();
	if ( nType < 0 || nType > 3 ) nType = 3;
	m_comboSingleDay.SetCurSel( nType );

	nType = EcrmanOptions.GetReportsPreviousDayRanges();
	if ( nType < 0 || nType > 3 ) nType = 3;
	m_comboPreviousDay.SetCurSel( nType );
	
	{
		int nDay;
		int nMonth;
		EcrmanOptions.GetFYearStart ( nDay, nMonth );
		m_strFYear.Format ( "%2.2d%2.2d", nDay, nMonth );
		CTime time ( 1999, nMonth, nDay, 0, 0, 0, -1 );
		m_DatePickerFYear.SetTime ( &time );
	}

	{
		CTime time ( 1999, 1, 1, EcrmanOptions.GetReportsDayStartHour(), EcrmanOptions.GetReportsDayStartMinute(), 0, -1 );
		m_TimePickerSOD.SetTime ( &time );
	}

	{
		CTime time ( 1999, 1, 1, EcrmanOptions.GetReportsTimeAttendSignOutHour(), EcrmanOptions.GetReportsTimeAttendSignOutMinute(), 0, -1 );
		m_TimePickerTimeAttend.SetTime ( &time );
	}

	int nResult = m_comboDates.FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
		
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );
	else
		m_comboDates.SetCurSel( 0 );

	/*****/

	int nSortType = EcrmanOptions.GetReportsCustomerSortType();
	if ( nSortType < 0 || nSortType > 2 ) nSortType = 0;
	m_comboCustomer.SetCurSel( nSortType );

	int nSalesTime = EcrmanOptions.GetReportsSalesTimeType();
	if ( nSalesTime < 0 || nSalesTime > 2 ) nSalesTime = 0;
	m_comboSalesTime.SetCurSel( nSalesTime );

	m_checkDiscountDetailSales.SetCheck(  EcrmanOptions.GetReportsDiscountDetailSalesFlag() );
	m_checkPremiumDetailSales.SetCheck(EcrmanOptions.GetReportsPremiumDetailSalesFlag());
	m_checkNetOfTaxSales.SetCheck( EcrmanOptions.GetReportsNetOfTaxSalesFlag() );
	m_checkDiscountDetailCovers.SetCheck(  EcrmanOptions.GetReportsDiscountDetailCoversFlag() );
	m_checkPremiumDetailCovers.SetCheck(EcrmanOptions.GetReportsPremiumDetailCoversFlag());
	m_checkNetOfTaxCovers.SetCheck( EcrmanOptions.GetReportsNetOfTaxCoversFlag() );
	m_checkAuthServer.SetCheck( EcrmanOptions.GetReportsAuthServerFlag() );
	m_checkReportpoint.SetCheck( EcrmanOptions.GetReportsGroupReportpointFlag() );
	m_checkReason.SetCheck(  EcrmanOptions.GetReportsEODReasonsFlag() );
	m_checkGeneralPage.SetCheck( EcrmanOptions.GetReportsGeneralPageFlag() );
}

/**********************************************************************/
