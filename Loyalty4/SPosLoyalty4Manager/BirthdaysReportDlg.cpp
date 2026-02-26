//$$******************************************************************
#include "..\SPosLoyalty4Shared\ExpiryDateRange.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "LoyaltyManager.h"
//$$******************************************************************
#include "BirthdaysReportDlg.h"
//$$******************************************************************

CBirthdaysReportDlg::CBirthdaysReportDlg( CWnd* pParent)
	: CDialog(CBirthdaysReportDlg::IDD, pParent)
	, m_DateSelectorRange( m_comboDateRange, m_DatePickerFrom, m_DatePickerTo )
{
	m_pReportInfo = NULL;
	m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_EXPIRY );

	m_nCheckMonthID[0] = IDC_CHECK_MONTH1;
	m_nCheckMonthID[1] = IDC_CHECK_MONTH2;
	m_nCheckMonthID[2] = IDC_CHECK_MONTH3;
	m_nCheckMonthID[3] = IDC_CHECK_MONTH4;
	m_nCheckMonthID[4] = IDC_CHECK_MONTH5;
	m_nCheckMonthID[5] = IDC_CHECK_MONTH6;
	m_nCheckMonthID[6] = IDC_CHECK_MONTH7;
	m_nCheckMonthID[7] = IDC_CHECK_MONTH8;
	m_nCheckMonthID[8] = IDC_CHECK_MONTH9;
	m_nCheckMonthID[9] = IDC_CHECK_MONTH10;
	m_nCheckMonthID[10] = IDC_CHECK_MONTH11;
	m_nCheckMonthID[11] = IDC_CHECK_MONTH12;

	m_nSelectAll = 0;
	CTime t = CTime::GetCurrentTime();
	m_nCurrentMonth = t.GetMonth();
	m_nNextMonth = m_nCurrentMonth;
}

//$$******************************************************************

CBirthdaysReportDlg::~CBirthdaysReportDlg()
{
}

//$$******************************************************************

void CBirthdaysReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//expiry
	DDX_Control(pDX, IDC_CHECK_EXPIRY, m_checkExpiry);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_CHECK_EXPIRY_MONTH, m_checkExpiryMonth);
	
	//dob
	DDX_Control(pDX, IDC_STATIC_DOB, m_staticDOB);
	DDX_Control(pDX, IDC_CHECK_DOB, m_checkDOB);
	
	for ( int n = 0; n < 12; n++ )
		DDX_Control(pDX, m_nCheckMonthID[n], m_checkMonth[n] );

	DDX_Control(pDX, IDC_BUTTON_ALL, m_buttonAll);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_buttonNext);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBirthdaysReportDlg, CDialog)

	//expiry
	ON_BN_CLICKED(IDC_CHECK_EXPIRY, OnToggleExpiry)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_BN_CLICKED(IDC_CHECK_EXPIRY_MONTH, OnToggleDOB)
	//dob
	ON_BN_CLICKED(IDC_CHECK_DOB, OnToggleDOB)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBirthdaysReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int n = 0; n < 12; n++)
	{
		m_checkMonth[n].SetCheck(m_pReportInfo->GetMonthFlag(n));
	}

	if ( MonthsSelected() < 12 )
	{
		m_buttonAll.SetWindowText ( "&All" );
		m_nSelectAll = 0;
	}
	else
	{
		m_nSelectAll = 1;
		m_buttonAll.SetWindowText ( "N&one" );
	}
	
	CUserTexts texts;
	texts.Read();

	//expiry
	m_checkExpiry.SetCheck( m_pReportInfo -> GetExpiryFlag() );

	CString strPreviousDateType = m_pReportInfo -> GetDateRangeType();
	CString strPreviousDateFrom = m_pReportInfo -> GetDateFromString();
	CString strPreviousDateTo = m_pReportInfo -> GetDateToString();

	m_DateSelectorRange.InitialiseDateControls();
	m_comboDateRange.AddString ( szRANGE_TODAY );
	m_comboDateRange.AddString ( szRANGE_TOMORROW );
	m_comboDateRange.AddString ( szRANGE_THIS_WEEK );
	m_comboDateRange.AddString ( szRANGE_THIS_MONTH );
	m_comboDateRange.AddString ( szRANGE_THIS_YEAR );
	m_comboDateRange.AddString ( szRANGE_NEXT_WEEK );
	m_comboDateRange.AddString ( szRANGE_NEXT_MONTH );
	m_comboDateRange.AddString ( szRANGE_NEXT_YEAR );
	m_comboDateRange.AddString ( szRANGE_NEXT_3_MONTHS );
	m_comboDateRange.AddString ( szRANGE_NEXT_6_MONTHS );
	m_comboDateRange.AddString ( szRANGE_NEXT_9_MONTHS );
	m_comboDateRange.AddString ( szRANGE_NEXT_12_MONTHS );
	m_comboDateRange.AddString ( szRANGE_ALL );
	m_comboDateRange.AddString ( szRANGE_CUSTOM_DATE );
	m_comboDateRange.AddString ( szRANGE_CUSTOM_RANGE );
	m_DateSelectorRange.SetComboSelection(strPreviousDateType);

	if ( ( strPreviousDateType == szRANGE_CUSTOM_DATE ) || ( strPreviousDateType == szRANGE_CUSTOM_RANGE ) )
	{
		CSSDate dateFrom( strPreviousDateFrom );
		CSSDate dateTo( strPreviousDateTo );
		
		if ( dateFrom.IsSet() )
		{
			m_DatePickerFrom.SetTime( COleDateTime( dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay(), 0, 0, 0 ) );
		}

		if ( dateTo.IsSet() )
		{
			m_DatePickerTo.SetTime( COleDateTime( dateTo.GetYear(), dateTo.GetMonth(), dateTo.GetDay(), 0, 0, 0 ) );
		}
	}

	m_checkExpiryMonth.SetCheck( m_pReportInfo -> GetExpiryMonthFilterFlag() );
	OnToggleExpiry();

	{
		CString strMatchBy = "Match accounts by ";
		CString strDOB = texts.GetText( LOYALTYFIELD_DOB,"");
		m_staticDOB.SetWindowText( strDOB );
		m_checkDOB.SetWindowText( strMatchBy + strDOB );
		m_checkDOB.SetCheck( m_pReportInfo -> GetDOBFlag() );
	}

	OnToggleDOB();
	
	return TRUE;  
}

//$$******************************************************************

void CBirthdaysReportDlg::OnToggleExpiry() 
{
	bool bEnable = ( m_checkExpiry.GetCheck() != 0 );

	CString strDateRangeType;
	m_comboDateRange.GetWindowText( strDateRangeType );
	bool bCustomDate = ( strDateRangeType == szRANGE_CUSTOM_DATE );

	m_comboDateRange.EnableWindow( bEnable );
	m_DatePickerFrom.EnableWindow( bEnable );
	m_DatePickerTo.EnableWindow( bEnable & !bCustomDate );

	OnToggleDOB();
}

//$$******************************************************************

void CBirthdaysReportDlg::OnSelectDateRange() 
{
	m_DateSelectorRange.UpdateDateControlsExpiry(TRUE);
}

//$$******************************************************************

void CBirthdaysReportDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CBirthdaysReportDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

//$$******************************************************************

void CBirthdaysReportDlg::OnToggleDOB() 
{
	bool bEnable = FALSE;
	bEnable |= ( m_checkDOB.GetCheck() != 0 );
	bEnable |= ( ( m_checkExpiry.GetCheck() != 0 ) && ( m_checkExpiryMonth.GetCheck() != 0 ) );

	m_buttonAll.EnableWindow( bEnable );
	m_buttonNext.EnableWindow( bEnable );

	for ( int n = 0; n < 12; n++ )
		m_checkMonth[n].EnableWindow( bEnable );
}

//$$******************************************************************

void CBirthdaysReportDlg::OnButtonNext()
{
	m_nNextMonth++;
	if ( ( m_nNextMonth < 1 ) || ( m_nNextMonth > 12 ) )
		m_nNextMonth = 1;

	m_checkMonth[ m_nNextMonth - 1 ].SetCheck( m_checkMonth[ m_nNextMonth - 1 ].GetCheck() == 0 ); 

}

//$$******************************************************************

void CBirthdaysReportDlg::OnButtonAll()
{
	m_nSelectAll = ( 0 == m_nSelectAll ) ? 1 : 0;

	for ( int n = 0; n < 12; n++ )
		m_checkMonth[n].SetCheck( 1 == m_nSelectAll );

	if ( 1 == m_nSelectAll )	
		m_buttonAll.SetWindowText ( "N&one" );
	else					
		m_buttonAll.SetWindowText ( "&All" );

	m_nNextMonth = m_nCurrentMonth;
}

//$$******************************************************************

int CBirthdaysReportDlg::MonthsSelected() 
{
	int nCount = 0;

	for ( int n = 0; n < 12; n++ )
	{
		if ( m_checkMonth[n].GetCheck() != 0 )
			nCount++;
	}

	return nCount;
}

//$$******************************************************************

void CBirthdaysReportDlg::OnOK()
{
	bool bNeedMonths = FALSE;
	bNeedMonths |= ( m_checkDOB.GetCheck() != 0 );
	bNeedMonths |= ( ( m_checkExpiry.GetCheck() != 0 ) && ( m_checkExpiryMonth.GetCheck() != 0 ) );

	if ( ( TRUE == bNeedMonths ) && ( MonthsSelected() == 0 ) )
	{
		MessageBeep( MB_ICONEXCLAMATION );
		Prompter.Error( "No month selected", "Cannot continue" );
		return;
	}

	if ( ( m_checkDOB.GetCheck() == 0 ) && ( m_checkExpiry.GetCheck() == 0 ) )
	{
		MessageBeep( MB_ICONEXCLAMATION );
		Prompter.Error( "No date criteria selected", "Cannot continue" );
		return;
	}

	//expiry
	m_pReportInfo -> SetExpiryFlag( m_checkExpiry.GetCheck() != 0 );

	CString strDateRangeType;
	m_comboDateRange.GetWindowText( strDateRangeType );
	m_pReportInfo ->SetDateRangeType( strDateRangeType );

	{
		CTime dateFrom;
		m_DatePickerFrom.GetTime( dateFrom );
		m_pReportInfo -> SetDateFrom( dateFrom );
	}

	{
		CTime dateTo;
		m_DatePickerTo.GetTime( dateTo );
		m_pReportInfo -> SetDateTo( dateTo );
	}

	m_pReportInfo -> SetExpiryMonthFilterFlag( m_checkExpiryMonth.GetCheck() != 0 );

	//dob
	m_pReportInfo -> SetDOBFlag( m_checkDOB.GetCheck() != 0 );

	for ( int n = 0; n < 12; n++ )
		m_pReportInfo -> SetMonthFlag( n, m_checkMonth[n].GetCheck() != 0 );
	
	EndDialog( IDOK );
}

//$$******************************************************************
