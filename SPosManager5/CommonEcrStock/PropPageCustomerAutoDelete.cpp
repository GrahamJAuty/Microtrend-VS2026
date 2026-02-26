/**********************************************************************/
#include "Password.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageCustomerAutoDelete.h"
/**********************************************************************/

CPropPageCustomerAutoDelete::CPropPageCustomerAutoDelete( int& nIndex, bool bOneOff ) : CSSPropertyPage(CPropPageCustomerAutoDelete::IDD), m_nIndex( nIndex )
{
	//{{AFX_DATA_INIT(CPropPageCustomerGeneral)
	//}}AFX_DATA_INIT
	m_bOneOff = bOneOff;
	m_bAutoDelete = ( m_bOneOff ) ? EcrmanOptions.GetFeaturesCustomerOneOffAutoDeleteFlag() : EcrmanOptions.GetFeaturesCustomerAccountAutoDeleteFlag();
}

/**********************************************************************/

CPropPageCustomerAutoDelete::~CPropPageCustomerAutoDelete()
{
}

/**********************************************************************/

void CPropPageCustomerAutoDelete::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageCustomerAutoDelete)
	DDX_Control(pDX, IDC_EDIT_NAME1, m_editName1);
	DDX_Control(pDX, IDC_EDIT_NAME2, m_editName2);
	DDX_Control(pDX, IDC_EDIT_CREATEDATE_EPOS, m_editCreateDateEpos);
	DDX_Control(pDX, IDC_DATEPICKER_CREATEDATE_EPOS, m_DatePickerCreateEpos);
	DDX_Control(pDX, IDC_EDIT_CREATETIME_EPOS, m_editCreateTimeEpos);
	DDX_Control(pDX, IDC_STATIC_PROGRAM, m_staticProgram);
	DDX_Control(pDX, IDC_EDIT_CREATEDATE_RECORD, m_editCreateDateRecord);
	DDX_Control(pDX, IDC_DATEPICKER_CREATEDATE_RECORD, m_DatePickerCreateRecord);
	DDX_Control(pDX, IDC_EDIT_CREATETIME_RECORD, m_editCreateTimeRecord);
	DDX_Control(pDX, IDC_EDIT_DELETE, m_editDelete);
	DDX_Control(pDX, IDC_CHECK_DELETE, m_checkDelete);
	DDX_Control(pDX, IDC_DATEPICKER_DELETE, m_DatePickerDelete);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageCustomerAutoDelete, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageCustomerAutoDelete)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DELETE, OnToggleSpecifyDate)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageCustomerAutoDelete::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_editName1.LimitText( Customer::CustomerName1.Max );
	m_editName2.LimitText( Customer::CustomerName2.Max );

	COleDateTime timeStart = COleDateTime( 1998, 1, 1, 0, 0, 0 );
	COleDateTime timeEnd = COleDateTime( 2050, 1, 1, 0, 0, 0 );
	
	m_DatePickerCreateEpos.SetRange ( &timeStart, &timeEnd );
	m_DatePickerCreateEpos.SetFormat( "ddd dd MMM yyy" );
	m_DatePickerCreateEpos.EnableWindow( FALSE );

	m_DatePickerCreateRecord.SetRange ( &timeStart, &timeEnd );
	m_DatePickerCreateRecord.SetFormat( "ddd dd MMM yyy" );
	m_DatePickerCreateRecord.EnableWindow( FALSE );

	m_DatePickerDelete.SetRange ( &timeStart, &timeEnd );
	m_DatePickerDelete.SetFormat( "ddd dd MMM yyy" );

	m_editCreateDateEpos.SetWindowText( "Unknown" );
	m_editCreateTimeEpos.SetWindowText( "--:--" );
	
	m_editCreateDateRecord.SetWindowText( "Unknown" );
	m_editCreateTimeRecord.SetWindowText( "--:--" );
	
	m_editDelete.SetWindowText( "Never" );

	if ( PasswordArray.GetEnable( PasswordTicks::EditCustomersDelDay ) == FALSE )
	{
		m_checkDelete.EnableWindow( FALSE );
		m_DatePickerDelete.EnableWindow( FALSE );
	}

	if ( SysInfo.IsStockSystem() == TRUE )
	{
		m_staticProgram.SetWindowText( "Imported by Stockman" );
	}
	else
	{
		m_staticProgram.SetWindowText( "Imported by Ecrman" );
	}

	if ( FALSE == m_bAutoDelete )
	{
		ShowAndEnableWindow( &m_editDelete, FALSE );
		ShowAndEnableWindow( &m_checkDelete, FALSE );
		ShowAndEnableWindow( &m_DatePickerDelete, FALSE );
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageCustomerAutoDelete::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageCustomerAutoDelete::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageCustomerAutoDelete::GetRecordData()
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

	{
		int nCreationDay = CustomerRecord.GetPosCreationDay();

		if ( nCreationDay <= Customer::PosCreationDay.Min )
		{
			ShowAndEnableWindow( &m_editCreateDateEpos, TRUE );
			ShowAndEnableWindow( &m_DatePickerCreateEpos, FALSE );
		}
		else
		{
			ShowAndEnableWindow( &m_editCreateDateEpos, FALSE );
			ShowAndEnableWindow( &m_DatePickerCreateEpos, TRUE, FALSE );

			COleDateTime dateCreated;
			GetOleDateSince2011( CustomerRecord.GetPosCreationDay(), dateCreated );
			m_DatePickerCreateEpos.SetTime( dateCreated );
		}

		CString strTime = "";

		if ( nCreationDay > Customer::PosCreationDay.Min )
		{
			int nCreationTime = CustomerRecord.GetPosCreationTime();

			if ( ( nCreationTime >= 0 ) && ( nCreationTime <= 1439 ) )
			{
				strTime.Format( "%2.2d:%2.2d", nCreationTime / 60, nCreationTime % 60 );
			}
		}

		m_editCreateTimeEpos.SetWindowText( strTime );
		ShowAndEnableWindow( &m_editCreateTimeEpos, strTime != "" );
	}
	
	{
		int nCreationDay = CustomerRecord.GetRecordCreationDay();

		if ( nCreationDay <= Customer::RecordCreationDay.Min )
		{
			ShowAndEnableWindow( &m_editCreateDateRecord, TRUE );
			ShowAndEnableWindow( &m_DatePickerCreateRecord, FALSE );
		}
		else
		{
			ShowAndEnableWindow( &m_editCreateDateRecord, FALSE );
			ShowAndEnableWindow( &m_DatePickerCreateRecord, TRUE, FALSE );

			COleDateTime dateCreated;
			GetOleDateSince2011( CustomerRecord.GetRecordCreationDay(), dateCreated );
			m_DatePickerCreateRecord.SetTime( dateCreated );
		}

		CString strTime = "";

		if ( nCreationDay > Customer::RecordCreationDay.Min )
		{
			int nCreationTime = CustomerRecord.GetRecordCreationTime();

			if ( ( nCreationTime >= 0 ) && ( nCreationTime <= 1439 ) )
			{
				strTime.Format( "%2.2d:%2.2d", nCreationTime / 60, nCreationTime % 60 );
			}
		}

		m_editCreateTimeRecord.SetWindowText( strTime );
		ShowAndEnableWindow( &m_editCreateTimeRecord, strTime != "" );
	}

	if ( TRUE == m_bAutoDelete )
	{
		if ( CustomerRecord.GetAutoDeleteFlag() == TRUE )
		{
			m_checkDelete.SetCheck( TRUE );
			ShowAndEnableWindow( &m_editDelete, FALSE );
			ShowAndEnableWindow( &m_DatePickerDelete, TRUE );

			COleDateTime dateStart;
			GetOleDateSince2011( CustomerRecord.GetAutoDeleteDay(), dateStart );
			m_DatePickerDelete.SetTime( dateStart );
		}
		else
		{
			m_checkDelete.SetCheck( FALSE );
			ShowAndEnableWindow( &m_editDelete, TRUE );
			ShowAndEnableWindow( &m_DatePickerDelete, FALSE );
		}
	}
}

/**********************************************************************/

void CPropPageCustomerAutoDelete::OnToggleSpecifyDate()
{
	bool bEnable = ( m_checkDelete.GetCheck() != 0 );
	ShowAndEnableWindow( &m_editDelete, FALSE == bEnable );
	ShowAndEnableWindow( &m_DatePickerDelete, bEnable );
	
	if ( TRUE == bEnable )
	{
		int nDays = ( m_bOneOff ) ? EcrmanOptions.GetFeaturesCustomerOneOffDelDay() : EcrmanOptions.GetFeaturesCustomerAccountDelDay();

		if ( nDays >= 0 )
			nDays = 21;

		COleDateTime dateDelete = COleDateTime::GetCurrentTime();
		dateDelete += COleDateTimeSpan( nDays, 0, 0, 0 );
		m_DatePickerDelete.SetTime( dateDelete );
	}
}

/**********************************************************************/

void CPropPageCustomerAutoDelete::SetRecordControls()
{
}

/**********************************************************************/

bool CPropPageCustomerAutoDelete::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageCustomerAutoDelete::SaveRecord()
{
	if ( PasswordArray.GetEnable( PasswordTicks::EditCustomersDelDay ) == TRUE )
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
		
		if ( TRUE == m_bAutoDelete )
		{
			CustomerRecord.SetAutoDeleteFlag( m_checkDelete.GetCheck() != 0 );

			COleDateTime dateStart;
			m_DatePickerDelete.GetTime( dateStart );
			CustomerRecord.SetAutoDeleteDay( GetDayNumberSince2011( dateStart ) );
		}

		if ( FALSE == m_bOneOff )
			DataManagerNonDb.CustomerAccount.SetAt ( nCustomerIdx, CustomerRecord );
		else
			DataManagerNonDb.CustomerOneOff.SetAt ( nCustomerIdx, CustomerRecord );
	}
}

/**********************************************************************/

void CPropPageCustomerAutoDelete::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
