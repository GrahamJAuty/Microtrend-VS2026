/**********************************************************************/
#include "PropPageCustomerGeneral.h"
/**********************************************************************/

CPropPageCustomerGeneral::CPropPageCustomerGeneral( bool& bAdminEnabled, int& nIndex, bool bOneOff ) : CSSPropertyPage(CPropPageCustomerGeneral::IDD), m_bAdminEnabled( bAdminEnabled ), m_nIndex( nIndex )
{
	//{{AFX_DATA_INIT(CPropPageCustomerGeneral)
	//}}AFX_DATA_INIT
	m_bOneOff = bOneOff;
	m_bRightClicked = FALSE;
}

/**********************************************************************/

CPropPageCustomerGeneral::~CPropPageCustomerGeneral()
{
}

/**********************************************************************/

void CPropPageCustomerGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageCustomerGeneral)
	DDX_Control(pDX, IDC_EDIT_NAME1, m_editName1);
	DDX_Control(pDX, IDC_EDIT_NAME2, m_editName2);
	DDX_Control(pDX, IDC_EDIT_ADDRESS1, m_editAddress1);
	DDX_Control(pDX, IDC_EDIT_ADDRESS2, m_editAddress2);
	DDX_Control(pDX, IDC_EDIT_ADDRESS3, m_editAddress3);
	DDX_Control(pDX, IDC_EDIT_POSTCODE, m_editPostcode);
	DDX_Control(pDX, IDC_STATIC_BALANCE, m_staticBalance);
	DDX_Control(pDX, IDC_CHECK_DATE, m_checkDate);
	DDX_Control(pDX, IDC_DATEPICKER_DATE, m_DatePickerDate);
	DDX_Control(pDX, IDC_DATEPICKER_TIME, m_DatePickerTime);
	DDX_Control(pDX, IDC_BUTTON_ADMIN, m_buttonAdmin);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_BALANCE, m_dBalance);
	DDV_MinMaxDouble(pDX, m_dBalance, Customer::StartBalance.Min, Customer::StartBalance.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageCustomerGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageCustomerGeneral)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DATE, OnToggleSpecifyDate)
	ON_BN_CLICKED(IDC_BUTTON_ADMIN, OnButtonAdmin)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageCustomerGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_editName1.LimitText( Customer::CustomerName1.Max );
	m_editName2.LimitText( Customer::CustomerName2.Max );
	m_editAddress1.LimitText( Customer::Address.Max );
	m_editAddress2.LimitText( Customer::Address.Max );
	m_editAddress3.LimitText( Customer::Address.Max );
	m_editPostcode.LimitText( Customer::Postcode.Max );

	SubclassEdit ( IDC_EDIT_BALANCE, SS_NUM_SDP, 9, "%.2f" );
	
	COleDateTime timeStart = COleDateTime( 2011, 1, 1, 0, 0, 0 );
	COleDateTime timeEnd = COleDateTime( 2050, 1, 1, 0, 0, 0 );
	m_DatePickerDate.SetRange ( &timeStart, &timeEnd );
	m_DatePickerDate.SetFormat( "ddd dd MMM yyy" );
	m_DatePickerTime.SetFormat ( "HH:mm" );

	m_buttonAdmin.ShowWindow( SW_HIDE );

	if ( TRUE == m_bOneOff )
	{
		ShowAndEnableWindow( &m_staticBalance, FALSE );
		ShowAndEnableWindow( GetEditBalance(), FALSE );
		ShowAndEnableWindow( &m_checkDate, FALSE );
		ShowAndEnableWindow( &m_DatePickerDate, FALSE );
		ShowAndEnableWindow( &m_DatePickerTime, FALSE );
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPageCustomerGeneral::GetEditBalance(){ return GetEdit( IDC_EDIT_BALANCE ); }
/**********************************************************************/

BOOL CPropPageCustomerGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageCustomerGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageCustomerGeneral::GetRecordData()
{	
	CCustomerCSVRecord CustomerRecord;

	if ( FALSE == m_bOneOff )
	{
		int nCustomerIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerAccount.GetAt ( nCustomerIdx, CustomerRecord );
	}
	else
	{
		int nCustomerIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerOneOff.GetAt ( nCustomerIdx, CustomerRecord );
	}

	m_editName1.SetWindowText( CustomerRecord.GetCustomerName1() );
	m_editName2.SetWindowText( CustomerRecord.GetCustomerName2() );
	m_editAddress1.SetWindowText(  CustomerRecord.GetAddress1() );
	m_editAddress2.SetWindowText(  CustomerRecord.GetAddress2() );
	m_editAddress3.SetWindowText(  CustomerRecord.GetAddress3() );
	m_editPostcode.SetWindowText(  CustomerRecord.GetPostcode() );

	m_dBalance = CustomerRecord.GetStartBalance();
	UpdateData( FALSE );
	
	m_editName1.SetFocus();
	m_editName1.SetSel ( 0, -1 );

	if ( CustomerRecord.GetSpecifyStartDayFlag() == TRUE )
	{
		m_checkDate.SetCheck( TRUE );

		COleDateTime dateStart;
		GetOleDateSince2011( CustomerRecord.GetStartDay(), dateStart );
		m_DatePickerDate.SetTime( dateStart );

		COleDateTime timeStart = COleDateTime( 2013, 1, 1, CustomerRecord.GetStartHour(), CustomerRecord.GetStartMinute(), 0 );
		m_DatePickerTime.SetTime( timeStart );
	}
	else
	{
		m_checkDate.SetCheck( FALSE );

		CCustomerCSVRecord StartDayRecord;
		DataManagerNonDb.CustomerAccount.GetStartDateRecord( StartDayRecord );

		COleDateTime dateStart;
		GetOleDateSince2011( StartDayRecord.GetStartDay(), dateStart );
		m_DatePickerDate.SetTime( dateStart );

		COleDateTime timeStart = COleDateTime( 2013, 1, 1, StartDayRecord.GetStartHour(), StartDayRecord.GetStartMinute(), 0 );
		m_DatePickerTime.SetTime( timeStart );
	}

	OnToggleSpecifyDate();
}

/**********************************************************************/

void CPropPageCustomerGeneral::OnToggleSpecifyDate()
{
	bool bEnable = ( m_checkDate.GetCheck() != 0 );
	m_DatePickerDate.EnableWindow( bEnable );
	m_DatePickerTime.EnableWindow( bEnable );
}

/**********************************************************************/

void CPropPageCustomerGeneral::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bRightClicked = TRUE;
	CSSPropertyPage::OnRButtonDown(nFlags, point);
}

/**********************************************************************/

void CPropPageCustomerGeneral::OnButtonAdmin()
{
	if ( ( TRUE == m_bRightClicked ) && ( FALSE == m_bAdminEnabled ) )
	{
		Prompter.Info( "Admin Options Enabled" );
		m_bAdminEnabled = TRUE;
	}
}

/**********************************************************************/

void CPropPageCustomerGeneral::SetRecordControls()
{
}

/**********************************************************************/

bool CPropPageCustomerGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageCustomerGeneral::SaveRecord()
{
	CCustomerCSVRecord CustomerRecord;

	int nCustomerIdx;
	if ( FALSE == m_bOneOff )
	{
		nCustomerIdx = DataManagerNonDb.CustomerNameTable.Account.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerAccount.GetAt ( nCustomerIdx, CustomerRecord );
	}
	else
	{
		nCustomerIdx = DataManagerNonDb.CustomerNameTable.OneOff.GetSortOrderItem( m_nIndex );
		DataManagerNonDb.CustomerOneOff.GetAt ( nCustomerIdx, CustomerRecord );
	}

	CustomerRecord.SetCustomerName1( GetEditBoxText( m_editName1  ) );
	CustomerRecord.SetCustomerName2( GetEditBoxText( m_editName2  ) );
	CustomerRecord.SetAddress1( GetEditBoxText( m_editAddress1  ) );
	CustomerRecord.SetAddress2( GetEditBoxText( m_editAddress2  ) );
	CustomerRecord.SetAddress3( GetEditBoxText( m_editAddress3  ) );
	CustomerRecord.SetPostcode( GetEditBoxText( m_editPostcode  ) );
	
	if ( FALSE == m_bOneOff )
	{
		CustomerRecord.SetStartBalance( m_dBalance );	
		CustomerRecord.SetSpecifyStartDayFlag( m_checkDate.GetCheck() != 0 );

		COleDateTime dateStart;
		m_DatePickerDate.GetTime( dateStart );
		CustomerRecord.SetStartDay( GetDayNumberSince2011( dateStart ) );

		COleDateTime timeStart;
		m_DatePickerTime.GetTime( timeStart );
		CustomerRecord.SetStartHour( timeStart.GetHour() );
		CustomerRecord.SetStartMinute( timeStart.GetMinute() );

		DataManagerNonDb.CustomerAccount.SetAt ( nCustomerIdx, CustomerRecord );
	}
	else
	{
		DataManagerNonDb.CustomerOneOff.SetAt ( nCustomerIdx, CustomerRecord );
	}
}

/**********************************************************************/

void CPropPageCustomerGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/

