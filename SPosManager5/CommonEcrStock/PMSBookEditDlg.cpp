/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "EmailOptions.h"
#include "ListDataPMSDepositDlg.h"
#include "Password.h"
#include "PMSBookEditActiveDlg.h"
#include "PMSBookSearchDlg.h"
#include "PMSCalendarTable.h"
#include "PMSDepositEditDlg.h"
#include "PMSEmailSignatureBuffer.h"
#include "PMSOptions.h"
#include "PriceHelpers.h"
#include "PropertySheetPMSDinner.h"
#include "SMTPEmailHelpers.h"
#include "SMTPResultsDlg.h"
/**********************************************************************/
#include "PMSBookEditDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSBookEditDlg, CDialog)
/**********************************************************************/

CPMSBookEditDlg::CPMSBookEditDlg( int nBookingType, int nRoomIdx, int nAccountIdx, CPMSBookingCSVRecord& BookingRecord, CPMSCustomerCSVRecord& CustomerRecord, CWnd* pParent) : CSSDialog(CPMSBookEditDlg::IDD, pParent), 
	m_BookingRecord( BookingRecord ),
	m_CustomerRecord( CustomerRecord )
{
	m_nBookingType = nBookingType;
	m_strDisplayLine = "";
	m_bEditedBooking = FALSE;
	m_bEmailedBooking = FALSE;
	m_bMultipleAccounts = FALSE;
	m_bDeleteBooking = FALSE;
	m_bRestoreBooking = FALSE;
	m_nActiveRoomIdx = nRoomIdx;
	m_nActiveAccountIdx = nAccountIdx;
	m_bAccountsTick = FALSE;
	m_dExtraBedRate = 0.0;
	m_dRoomRate = 0.0;
	m_nRoomRateType = 0;
	m_nTariffType = 0;

	PMSRoomStatusTable.GetAccountInfo( m_nActiveRoomIdx, m_nActiveAccountIdx, m_infoAccount );

	if ( m_nActiveRoomIdx == MAX_PMS_ROOMS )
		m_infoAccount.SetCustomerId( BookingRecord.GetCustomerId() + m_nActiveAccountIdx );

	m_nCovers = m_infoAccount.GetGuestCount();

	m_bCanEditRoomRate = PasswordArray.GetEnable( PasswordTicks::PMSChangeRoomRate );
}

/**********************************************************************/

CPMSBookEditDlg::~CPMSBookEditDlg()
{
}

/**********************************************************************/

void CPMSBookEditDlg::GetTitleText( CString& strText )
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:	strText = "Book Room";				break;
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:		strText = "Check In";				break;
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:	strText = "Add Account";			break;
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:	strText = "Edit Account";			break;
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:	strText = "Edit Cancelled Booking";	break;
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:	
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	default:									strText = "Edit Booking";			break;
	}
}

/**********************************************************************/

bool CPMSBookEditDlg::CanSendEmail()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
		return TRUE;

	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPMSBookEditDlg::CanEnableMultipleAccounts()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPMSBookEditDlg::CanEditCovers()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
		return TRUE;

	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPMSBookEditDlg::CanEditBooking()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPMSBookEditDlg::CanDeleteBookingOrCancelCheckIn()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
		return TRUE;

	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	default:
		return FALSE;
	}
}

/**********************************************************************/

bool CPMSBookEditDlg::CanEditDinner()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
		return FALSE;

	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	default:
		return TRUE;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::GetDeleteButtonText( CString& strText )
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
		strText = "Restore\nBooking";
		break;

	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
		strText = "Cancel\nBooking";
		break;

	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
		strText = "Cancel\nCheck In";
		break;

	default:
		strText = "";
		break;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::GetDeletePromptText( CString& strText )
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
		strText = "restore this booking";
		break;

	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
		strText = "cancel this booking";
		break;

	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
		strText = "cancel this check in";
		break;

	default:
		strText = "";
		break;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::GetOKButtonText( CString& strText )
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
		strText = "Confirm\nBooking";
		break;

	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
		strText = "Confirm\nCheck In";
		break;

	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
		strText = "Create\nAccount";
		break;

	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
	default:
		strText = "Confirm\nChanges";
		break;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::GetCancelButtonText( CString& strText )
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
		strText = "Cancel\nBooking";
		break;

	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
		strText = "Cancel\nCheck In";
		break;

	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
		strText = "Cancel\nAccount";
		break;

	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
	default:
		strText = "Cancel\nChanges";
		break;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_EDIT_REFERENCE, m_editReference);
	DDX_Control( pDX, IDC_EDIT_NOTE, m_editNote);
	DDX_Control( pDX, IDC_EDIT_CHECKIN, m_editCheckIn );
	DDX_Control( pDX, IDC_EDIT_NIGHTS, m_editNights );
	DDX_Control( pDX, IDC_EDIT_CHECKOUT, m_editCheckOut );
	DDX_Control( pDX, IDC_EDIT_GUESTS, m_editGuests );
	DDX_Control( pDX, IDC_EDIT_BALANCE, m_editBalance);
	DDX_Control( pDX, IDC_EDIT_DEPOSIT, m_editDeposit);
	DDX_Control( pDX, IDC_CHECK_ACCOUNTS, m_checkAccounts );
	DDX_Control( pDX, IDC_BUTTON_SEARCH, m_buttonSearch );
	DDX_Control( pDX, IDC_BUTTON_DELETE, m_buttonDelete );
	DDX_Control( pDX, IDC_CHECK_TRAINING, m_checkTraining );
	DDX_Control( pDX, IDC_BUTTON_DEPOSIT, m_buttonDeposit );
	DDX_Control( pDX, IDC_BUTTON_DINNER, m_buttonDinner );
	DDX_Control( pDX, IDC_COMBO_TARIFF, m_comboTariff );
	DDX_Control( pDX, IDC_COMBO_RATE, m_comboRate );
	DDX_Control( pDX, IDOK, m_buttonOK );
	DDX_Control( pDX, IDCANCEL, m_buttonCancel );
	DDX_Text( pDX, IDC_EDIT_NAME_GUEST, m_strGuestName );
	DDV_MaxChars( pDX, m_strGuestName, PMSCustomer::GuestName.Max );
	DDX_Text( pDX, IDC_EDIT_NAME_BILLING, m_strBillingName );
	DDV_MaxChars( pDX, m_strBillingName, PMSCustomer::BillingName.Max );
	DDX_Text( pDX, IDC_EDIT_ADDRESS1, m_strAddress1 );
	DDV_MaxChars( pDX, m_strAddress1, PMSCustomer::Address.Max );
	DDX_Text( pDX, IDC_EDIT_ADDRESS2, m_strAddress2 );
	DDV_MaxChars( pDX, m_strAddress2, PMSCustomer::Address.Max );
	DDX_Text( pDX, IDC_EDIT_ADDRESS3, m_strAddress3 );
	DDV_MaxChars( pDX, m_strAddress3, PMSCustomer::Address.Max );
	DDX_Text( pDX, IDC_EDIT_POSTCODE, m_strPostcode );
	DDV_MaxChars( pDX, m_strPostcode, PMSCustomer::Postcode.Max );
	DDX_Text( pDX, IDC_EDIT_PHONE, m_strPhone );
	DDV_MaxChars( pDX, m_strPhone, PMSCustomer::Phone.Max );
	DDX_Text( pDX, IDC_EDIT_FAX, m_strFax );
	DDV_MaxChars( pDX, m_strFax, PMSCustomer::Fax.Max );
	DDX_Text( pDX, IDC_EDIT_EMAIL, m_strEmail );
	DDV_MaxChars( pDX, m_strEmail, PMSCustomer::Email.Max );
	DDX_Control( pDX, IDC_STATIC_EMAIL, m_staticEmail );
	DDX_Control( pDX, IDC_CHECK_EMAIL, m_checkEmail );
	DDX_Control( pDX, IDC_STATIC_COVERS, m_staticCovers );
	DDX_Text( pDX, IDC_EDIT_NOTE, m_strNote );
	DDV_MaxChars( pDX, m_strNote, PMSBooking::BookingNote.Max );
	DDX_Text( pDX, IDC_EDIT_RATE, m_dRoomRate );

	if ( DealerFlags.GetPMSExtendedRoomRateFlag() == TRUE )
	{
		DDV_MinMaxDouble( pDX, m_dRoomRate, PMSRoom::ExtendedRoomRate.Min, PMSRoom::ExtendedRoomRate.Max );
	}
	else
	{
		DDV_MinMaxDouble( pDX, m_dRoomRate, PMSRoom::RoomRate.Min, PMSRoom::RoomRate.Max );
	}

	DDX_Text( pDX, IDC_EDIT_BED, m_dExtraBedRate );
	DDV_MinMaxDouble( pDX, m_dExtraBedRate, PMSRoom::RoomRate.Min, PMSRoom::RoomRate.Max );
	
	if ( CanEditCovers() == TRUE )
	{
		DDX_Text( pDX, IDC_EDIT_COVERS, m_nCovers );
		DDV_MinMaxInt( pDX, m_nCovers, 1, m_BookingRecord.GetGuests() );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSBookEditDlg, CDialog)

	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DEPOSIT, OnButtonDeposit)
	ON_BN_CLICKED(IDC_BUTTON_DINNER, OnButtonDinner)
	ON_CBN_SELCHANGE(IDC_COMBO_TARIFF, OnSelectTariff)
	ON_CBN_SELCHANGE(IDC_COMBO_RATE, OnSelectRoomRate)
	ON_EN_KILLFOCUS(IDC_EDIT_RATE, OnChangeRoomRate)
	ON_EN_KILLFOCUS(IDC_EDIT_BED, OnChangeExtraBedRate)

END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSBookEditDlg::OnInitDialog()
{
	int nCustomerIdx;
	int nCustomerId = m_infoAccount.GetCustomerId();
	if ( DataManagerNonDb.PMSCustomer.FindCustomerById( nCustomerId, nCustomerIdx ) == TRUE )
	{
		DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, m_CustomerRecord );
		m_strGuestName = m_CustomerRecord.GetGuestName();
		m_strBillingName = m_CustomerRecord.GetBillingName();
		m_strAddress1 = m_CustomerRecord.GetAddress1();
		m_strAddress2 = m_CustomerRecord.GetAddress2();
		m_strAddress3 = m_CustomerRecord.GetAddress3();
		m_strPostcode = m_CustomerRecord.GetPostcode();
		m_strPhone = m_CustomerRecord.GetPhone();
		m_strFax = m_CustomerRecord.GetFax();
		m_strEmail = m_CustomerRecord.GetEmail();
	}
	else
	{
		m_strGuestName = "";
		m_strBillingName = "";
		m_strAddress1 = "";
		m_strAddress2 = "";
		m_strAddress3 = "";
		m_strPostcode = "";
		m_strPhone = "";
		m_strFax = "";
		m_strEmail = "";
	}

	m_strNote = m_BookingRecord.GetBookingNote();
	m_dRoomRate = m_BookingRecord.GetRoomRate();
	m_dExtraBedRate = m_BookingRecord.GetExtraBedRate();

	CDialog::OnInitDialog();
	
	m_listRooms.SubclassDlgItem ( IDC_LIST, this );
	m_listRooms.InsertColumn ( 0, "No", LVCFMT_LEFT, 40 );
	m_listRooms.InsertColumn ( 1, "Room Name", LVCFMT_LEFT, 190 );
	m_listRooms.InsertColumn ( 2, "Booked Until", LVCFMT_LEFT, 120 );
	m_listRooms.InsertColumn ( 3, "Type", LVCFMT_LEFT, 100 );
	m_listRooms.InsertColumn ( 4, "Sleeps", LVCFMT_LEFT, 50 );
	m_listRooms.InsertColumn ( 5, "Options", LVCFMT_LEFT, 100 );
	m_listRooms.SetItemCountEx ( 0 );
	
	if ( ( CanSendEmail() == FALSE ) || ( m_BookingRecord.CheckEmailDates() == FALSE ) )
	{
		m_checkEmail.ShowWindow( SW_HIDE );
		m_checkEmail.EnableWindow( FALSE );
	}
	else 
	{
		m_staticEmail.ShowWindow( SW_HIDE );
		m_checkEmail.SetCheck( TRUE );
	}
	
	if ( ( CanEnableMultipleAccounts() == FALSE ) || ( PMSOptions.GetMiscEnableMultipleAccounts() == FALSE ) )
	{
		m_checkAccounts.ShowWindow( SW_HIDE );
		m_checkAccounts.EnableWindow( FALSE );
	}

	if ( CanEditCovers() == FALSE )
	{
		m_staticCovers.ShowWindow( SW_HIDE );
		GetEditCovers() -> ShowWindow( SW_HIDE );
	}

	if ( CanEditBooking() == FALSE )
	{
		m_buttonSearch.ShowWindow( SW_HIDE );
		m_buttonSearch.EnableWindow( FALSE );
	}

	if ( ( CanEditDinner() == FALSE ) || ( m_BookingRecord.GetRoomRateType() != PMS_BOOKING_RATE_DBB ) )
	{
		m_buttonDinner.ShowWindow( SW_HIDE );
		m_buttonDinner.EnableWindow( FALSE );
	}
	
	CString strTransFile;
	strTransFile.Format( "%s\\%d.1",
		PMSOptions.GetPMSFolderSales(),
		m_BookingRecord.GetBookingId() );

	bool bGotTransactions = FALSE;
	if ( ::FileExists( strTransFile ) == TRUE )
	{
		CFileStatus FileStatus;
		CFile::GetStatus ( strTransFile, FileStatus );
		bGotTransactions = ( FileStatus.m_size != 0 );
	}
	
	if ( ( CanDeleteBookingOrCancelCheckIn() == FALSE  ) || ( TRUE == bGotTransactions ) )
	{
		m_buttonDelete.ShowWindow( SW_HIDE );
		m_buttonDelete.EnableWindow( FALSE );
	}

	CString strDeleteText;
	GetDeleteButtonText( strDeleteText );
	m_buttonDelete.SetWindowText( strDeleteText );

	CString strOKText;
	GetOKButtonText( strOKText );
	m_buttonOK.SetWindowText( strOKText );

	CString strCancelText;
	GetCancelButtonText( strCancelText );
	m_buttonCancel.SetWindowText( strCancelText );

	SubclassEdit( IDC_EDIT_COVERS, SS_NUM, 1, "%d" );
	
	if ( DealerFlags.GetPMSExtendedRoomRateFlag() == TRUE )
	{
		SubclassEdit( IDC_EDIT_RATE, SS_NUM_DP, 8, "%.2f" );
	}
	else
	{
		SubclassEdit( IDC_EDIT_RATE, SS_NUM_DP, 6, "%.2f" );
	}

	SubclassEdit( IDC_EDIT_BED, SS_NUM_DP, 6, "%.2f" );

	CString strReference;
	strReference.Format( "%8.8d", m_BookingRecord.GetBookingId() );
	m_editReference.SetWindowText( strReference );

	if ( PMSOptions.GetMiscEnableTrainingMode() == FALSE )
	{
		m_checkTraining.SetCheck( FALSE );
		m_checkTraining.EnableWindow( FALSE );
		m_checkTraining.ShowWindow( SW_HIDE );
	}
	else
		m_checkTraining.SetCheck( m_BookingRecord.GetTrainingFlag() != 0 );

	DisplayBooking();

	{
		bool bEditRate = ( PMS_BOOKING_RATE_FREE != m_nRoomRateType ) && ( TRUE == m_bCanEditRoomRate );

		bool bExtraBed = FALSE;
		switch( m_BookingRecord.GetTariffType() )
		{
		case PMS_TARIFFTYPE_EXTRABED:
		case PMS_TARIFFTYPE_EXTRACOT:
			bExtraBed = TRUE;
			break;
		}

		GetEditRate() -> SetReadOnly( FALSE == bEditRate );
		GetEditBed() -> SetReadOnly( ( FALSE == bEditRate ) || ( FALSE == bExtraBed ) );
	}

	if ( m_nActiveAccountIdx != 0 )
	{
		m_checkTraining.EnableWindow( FALSE );
		m_editNote.EnableWindow( FALSE );

		GetEditRate() -> SetReadOnly( TRUE );
		m_comboRate.EnableWindow( FALSE );

		m_buttonDeposit.EnableWindow( FALSE );
		m_buttonDeposit.ShowWindow( SW_HIDE );

		m_buttonSearch.EnableWindow( FALSE );
		m_buttonSearch.ShowWindow( SW_HIDE );

		m_buttonDelete.EnableWindow( FALSE );
		m_buttonDelete.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSBookEditDlg::GetEditCovers(){ return GetEdit( IDC_EDIT_COVERS ); }
CEdit* CPMSBookEditDlg::GetEditRate(){ return GetEdit( IDC_EDIT_RATE ); }
CEdit* CPMSBookEditDlg::GetEditBed(){ return GetEdit( IDC_EDIT_BED ); } 
/**********************************************************************/

void CPMSBookEditDlg::DisplayBooking()
{
	CString strTitle = "";
	GetTitleText( strTitle );

#ifdef _DEBUG
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:		strTitle += " PENDING SINGLE NEW";		break;
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:			strTitle +=	" ACTIVE SINGLE NEW";		break;
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:		strTitle += " ACTIVE MULTIPLE NEW";		break;
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:		strTitle += " ACTIVE MULTIPLE EDIT";	break;
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:		strTitle += " PENDING SINGLE EDIT";		break;
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:		strTitle += " ACTIVE SINGLE EDIT";		break;
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:		strTitle += " CLOSED SINGLE EDIT";		break;
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:		strTitle += " CLOSED MULTIPLE EDIT";	break;
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:		strTitle += " CANCELLED SINGLE EDIT";	break;
	default:										strTitle += " UNKNOWN";					break;
	}
#endif

	m_TariffMap.BuildMap( m_BookingRecord );

	CPMSBookingPartInfo infoPart;
	m_BookingRecord.GetFinalPartInfo( infoPart );

	int nRoomNo = infoPart.GetRoomNo();
	double dExtraBedRate = 0.0;
	double dExtraCotRate = 0.0;

	int nRoomIdx;
	if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( nRoomNo, nRoomIdx ) == TRUE )
	{
		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
		dExtraBedRate = Room.GetRoomExtraBedRate();
		dExtraCotRate = Room.GetRoomExtraCotRate();
		
		CString strRoomText;
		::GetPMSRoomText( nRoomNo, Room.GetRoomName(), strRoomText );
	
		CString strExtra;
		strExtra.Format( " (%s, %s)",
			(const char*) strRoomText,
			Room.GetRoomTypeName( FALSE ) ); 
		
		strTitle += strExtra;

		m_TariffMap.BuildMap( m_BookingRecord, Room );
	}
	
	SetWindowText( strTitle );

	//FILL TARIFF COMBO
	CArray<int,int> arrayTariffs;
	m_TariffMap.GetTariffTypeList( arrayTariffs );

	CArray<int,int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(110);
	m_comboTariff.SetTabStops( arrayStops );
	m_comboTariff.ResetContent();

	for ( int n = 0 ; n < arrayTariffs.GetSize(); n++ )
	{
		int nType = arrayTariffs.GetAt(n);

		switch( nType )
		{
		case PMS_TARIFFTYPE_STANDARD:
			m_comboTariff.AddItem( "Standard" );
			break;

		case PMS_TARIFFTYPE_SINGLE:
			m_comboTariff.AddItem( "Single Occupancy" );
			break;

		case PMS_TARIFFTYPE_EXTRABED:
			{
				CString strText;
				strText.Format( "Extra Adult Bed,(+%.2f)", dExtraBedRate );
				m_comboTariff.AddItem( strText );
			}
			break;

		case PMS_TARIFFTYPE_EXTRACOT:
			{
				CString strText;
				strText.Format( "Extra Child Bed,(+%.2f)", dExtraCotRate );
				m_comboTariff.AddItem( strText );
			}
			break;

		default:
			{
				CString str;
				str.Format( "Tariff type %d", nType );
				m_comboTariff.AddItem( str );
			}
			break;
		}

		m_comboTariff.SetItemData( n, nType );
	}

	//SELECT CURRENT TARIFF
	m_comboTariff.SetCurSel(0);
	for ( int n = 1; n < m_comboTariff.GetCount(); n++ )
	{
		if ( m_comboTariff.GetItemData(n) == m_BookingRecord.GetTariffType() )
		{
			m_comboTariff.SetCurSel(n);
			break;
		}
	}

	int nSel = m_comboTariff.GetCurSel();
	m_BookingRecord.SetTariffType( m_comboTariff.GetItemData(nSel) );
	m_comboTariff.EnableWindow( m_comboTariff.GetCount() > 1 );

	//FILL ROOM RATE COMBO
	CArray<int,int> arrayRates;
	m_TariffMap.GetRoomRateTypeList( m_BookingRecord.GetTariffType(), arrayRates );

	m_comboRate.ResetContent();
	for ( int n = 0; n < arrayRates.GetSize(); n++ )
	{
		int nRate = arrayRates.GetAt(n);

		CString strRate = "";
		switch( nRate )
		{
			case PMS_BOOKING_RATE_ROOM:		strRate = "Room Only";	break;
			case PMS_BOOKING_RATE_BB:		strRate = "B & B";		break;
			case PMS_BOOKING_RATE_DBB:		strRate = "DB & B";		break;
			case PMS_BOOKING_RATE_FREE:		strRate = "Free Room";	break;
		}

		if ( strRate != "" )
		{
			int nPos = m_comboRate.AddString( strRate );
			m_comboRate.SetItemData( nPos, nRate );
		}
	}

	//SELECT CURRENT RATE
	m_comboRate.SetCurSel(0);
	for ( int n = 1; n < m_comboRate.GetCount(); n++ )
	{
		if ( m_comboRate.GetItemData(n) == m_BookingRecord.GetRoomRateType() )
		{
			m_comboRate.SetCurSel(n);
			break;
		}
	}

	nSel = m_comboRate.GetCurSel();
	m_BookingRecord.SetRoomRateType( m_comboRate.GetItemData(nSel) );
	m_comboRate.EnableWindow( m_comboRate.GetCount() > 1 );

	m_nTariffType = m_BookingRecord.GetTariffType();
	m_nRoomRateType = m_BookingRecord.GetRoomRateType();

	m_arrayRooms.RemoveAll();

	int nStartDay, nNights;

	CPMSBookingPartInfo infoFirstPart, infoFinalPart;
	m_BookingRecord.GetPartInfo( 0, infoFirstPart );
	m_BookingRecord.GetFinalPartInfo( infoFinalPart );

	nStartDay = infoFirstPart.GetStartDay();
	nNights = infoFinalPart.GetStartDay() + infoFinalPart.GetNights() - nStartDay;
	
	int nEndDay = nStartDay + nNights;

	int nGuests = m_BookingRecord.GetGuests();
	int nRoomSleeps = m_BookingRecord.GetSearchRoomSleeps();
	int nRoomSubType = m_BookingRecord.GetSearchRoomSubType();

	COleDateTime dateStart, dateEnd;
	GetOleDateSince2011( nStartDay, dateStart );
	GetOleDateSince2011( nEndDay, dateEnd );

	CString strStartDay = dateStart.Format( "%a  %d  %b  %Y" );
	m_editCheckIn.SetWindowText( strStartDay );

	CString strNights;
	strNights.Format( "%d", nNights );
	m_editNights.SetWindowText( strNights );

	CString strEndDay = dateEnd.Format( "%a  %d  %b  %Y" );
	m_editCheckOut.SetWindowText( strEndDay );

	CString strGuests;
	strGuests.Format( "%d", nGuests );
	m_editGuests.SetWindowText( strGuests );

	DisplayBalance();

	m_BookingRecord.BuildRoomListForEditing( m_arrayRooms );
	m_listRooms.SetItemCountEx( m_arrayRooms.GetSize() );
}

/**********************************************************************/

void CPMSBookEditDlg::DisplayBalance()
{	
	CString strBalance = "";
	CString strDeposit = "";

	int nCustomerId = m_infoAccount.GetCustomerId();
	PMSRoomStatusTable.UpdateAccountBalance( m_nActiveRoomIdx, m_nActiveAccountIdx, m_BookingRecord );
	PMSRoomStatusTable.GetAccountInfo( m_nActiveRoomIdx, m_nActiveAccountIdx, m_infoAccount );
	m_infoAccount.SetCustomerId( nCustomerId );
	
	double dBalance = m_infoAccount.CalculateBalance();
	switch ( CPriceHelpers::CompareDoubles( dBalance, 0.0, 2 ) )
	{
	case 1:
		strBalance.Format ( "%.2f", dBalance );
		break;

	case -1:
		strBalance.Format ( "%.2f CR", -dBalance );
		break;

	case 0:
	default:
		strBalance = "0.00";
		break;
	}
	m_editBalance.SetWindowText( strBalance );

	double dDeposit = m_BookingRecord.CalculateDepositValue();
	switch ( CPriceHelpers::CompareDoubles( dDeposit, 0.0, 2 ) )
	{
	case 1:
	case -1:
		strDeposit.Format ( "%.2f", dDeposit );
		break;

	case 0:
	default:
		strDeposit = "0.00";
		break;
	}
	m_editDeposit.SetWindowText( strDeposit );
}

/**********************************************************************/

void CPMSBookEditDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CPMSBookEditDlg::PrepareLine( int nIndex )
{
	m_strDisplayLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayRooms.GetSize() ) )
	{
		CString strOption = "";
		if ( m_arrayRooms[nIndex].m_bExtraBed && m_arrayRooms[nIndex].m_bExtraCot )
			strOption = "Extra Bed or Cot";
		else if ( m_arrayRooms[nIndex].m_bExtraBed )
			strOption = "Extra Bed";
		else if ( m_arrayRooms[nIndex].m_bExtraCot )
			strOption = "Extra Cot";

		CString strSleeps = "";
		{
			int nSleeps = m_arrayRooms[ nIndex ].m_nSleeps;
			if ( strOption == "" )
				strSleeps.Format( "%d", nSleeps );
			else
				strSleeps.Format( "%d / %d", nSleeps, nSleeps + 1 );
		}

		CCSV csv ('\t');
		csv.Add( m_arrayRooms[ nIndex ].m_nRoomNo );
		csv.Add( m_arrayRooms[ nIndex ].m_strRoomName );
		csv.Add( m_arrayRooms[ nIndex ].m_strDate );
		csv.Add( m_arrayRooms[ nIndex ].m_strType );
		csv.Add( strSleeps );
		csv.Add( strOption );

		m_strDisplayLine = csv.GetLine();
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnSelectTariff()
{
	int nSel = m_comboTariff.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= m_comboTariff.GetCount() ) )
		return;

	int nNewTariff = m_comboTariff.GetItemData( nSel );

	switch ( nNewTariff )
	{
	case PMS_TARIFFTYPE_STANDARD:
	case PMS_TARIFFTYPE_SINGLE:
	case PMS_TARIFFTYPE_EXTRABED:
	case PMS_TARIFFTYPE_EXTRACOT:
		break;

	default:
		return;
	}

	if ( nNewTariff == m_nTariffType )
		return;

	{
		CPMSBookingPartInfo infoPart;
		m_BookingRecord.GetFinalPartInfo( infoPart );

		int nRoomIdx;
		int nRoomNo = infoPart.GetRoomNo();
		if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( nRoomNo, nRoomIdx ) == TRUE )
		{
			CPMSRoomCSVRecord Room;
			DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
		
			switch( nNewTariff )
			{
			case PMS_TARIFFTYPE_STANDARD:
			case PMS_TARIFFTYPE_SINGLE:
				switch( m_nTariffType )
				{
				case PMS_TARIFFTYPE_EXTRABED:
				case PMS_TARIFFTYPE_EXTRACOT:
					if ( m_BookingRecord.GetGuests() > Room.GetSleeps() )
					{
						int nGuests = Room.GetSleeps();
						m_BookingRecord.SetGuests( nGuests );
						CString strGuests;
						strGuests.Format( "%d", nGuests );
						m_editGuests.SetWindowText( strGuests );
					}
					break;
				}
				break;

			case PMS_TARIFFTYPE_EXTRABED:
			case PMS_TARIFFTYPE_EXTRACOT:
				switch( m_nTariffType )
				{
				case PMS_TARIFFTYPE_STANDARD:
				case PMS_TARIFFTYPE_SINGLE:
					if ( m_BookingRecord.GetGuests() == Room.GetSleeps() )
					{
						int nGuests = Room.GetSleeps() + 1;
						m_BookingRecord.SetGuests( nGuests );
						CString strGuests;
						strGuests.Format( "%d", nGuests );
						m_editGuests.SetWindowText( strGuests );
					}
					break;
				}
				break;
			}
		}
	}

	m_nTariffType = nNewTariff;
	m_BookingRecord.SetTariffType( m_nTariffType );

	SetDefaultTariffRates();
}

/**********************************************************************/

void CPMSBookEditDlg::SetDefaultTariffRates()
{
	//FILL ROOM RATE COMBO
	CArray<int,int> arrayRates;
	m_TariffMap.GetRoomRateTypeList( m_BookingRecord.GetTariffType(), arrayRates );

	m_comboRate.ResetContent();
	for ( int n = 0; n < arrayRates.GetSize(); n++ )
	{
		int nRate = arrayRates.GetAt(n);

		CString strRate = "";
		switch( nRate )
		{
			case PMS_BOOKING_RATE_ROOM:		strRate = "Room Only";	break;
			case PMS_BOOKING_RATE_BB:		strRate = "B & B";		break;
			case PMS_BOOKING_RATE_DBB:		strRate = "DB & B";		break;
			case PMS_BOOKING_RATE_FREE:		strRate = "Free Room";	break;
		}

		if ( strRate != "" )
		{
			int nPos = m_comboRate.AddString( strRate );
			m_comboRate.SetItemData( nPos, nRate );
		}
	}

	//SELECT CURRENT RATE
	m_comboRate.SetCurSel(0);
	for ( int n = 1; n < m_comboRate.GetCount(); n++ )
	{
		if ( m_comboRate.GetItemData(n) == m_BookingRecord.GetRoomRateType() )
		{
			m_comboRate.SetCurSel(n);
			break;
		}
	}

	int nSel = m_comboRate.GetCurSel();
	m_nRoomRateType = m_comboRate.GetItemData(nSel);
	m_comboRate.EnableWindow( m_comboRate.GetCount() > 1 );
	
	UpdateRoomRate();
}

/**********************************************************************/

void CPMSBookEditDlg::OnSelectRoomRate()
{
	int nSel = m_comboRate.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= m_comboRate.GetCount() ) )
		return;

	int nNewRate = m_comboRate.GetItemData( nSel );

	switch ( nNewRate )
	{
	case PMS_BOOKING_RATE_ROOM:
	case PMS_BOOKING_RATE_BB:
	case PMS_BOOKING_RATE_DBB:
	case PMS_BOOKING_RATE_FREE:
		break;

	default:
		return;
	}

	if ( nNewRate != m_nRoomRateType )
	{
		m_nRoomRateType = nNewRate;
		UpdateRoomRate();
	}
}

/**********************************************************************/

void CPMSBookEditDlg::UpdateRoomRate()
{
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nActiveRoomIdx, infoRoom );

	double dNewRoomRate = 0.0;
	double dNewExtraBedRate = 0.0;

	switch( m_nTariffType )
	{
	case PMS_TARIFFTYPE_STANDARD:
	case PMS_TARIFFTYPE_EXTRABED:
	case PMS_TARIFFTYPE_EXTRACOT:
		switch( m_nRoomRateType )
		{
		case PMS_BOOKING_RATE_ROOM:	dNewRoomRate = infoRoom.GetRoomOnlyRate();	break;
		case PMS_BOOKING_RATE_BB:	dNewRoomRate = infoRoom.GetRoomBBRate();	break;
		case PMS_BOOKING_RATE_DBB:	dNewRoomRate = infoRoom.GetRoomDBBRate();	break;
		case PMS_BOOKING_RATE_FREE:	dNewRoomRate = 0.0;							break;
		}
		break;

	case PMS_TARIFFTYPE_SINGLE:
		switch( m_nRoomRateType )
		{
		case PMS_BOOKING_RATE_ROOM:	dNewRoomRate = infoRoom.GetRoomOnlySingleRate();	break;
		case PMS_BOOKING_RATE_BB:	dNewRoomRate = infoRoom.GetRoomBBSingleRate();		break;
		case PMS_BOOKING_RATE_DBB:	dNewRoomRate = infoRoom.GetRoomDBBSingleRate();		break;
		}
		break;
	}

	bool bExtraBedTariff = FALSE;
	switch( m_nTariffType )
	{
	case PMS_TARIFFTYPE_EXTRABED:
		bExtraBedTariff = TRUE;
		dNewExtraBedRate = infoRoom.GetRoomExtraBedRate();
		break;

	case PMS_TARIFFTYPE_EXTRACOT:
		bExtraBedTariff = TRUE;
		dNewExtraBedRate = infoRoom.GetRoomExtraCotRate();
		break;
	}

	{
		bool bEditRate = ( PMS_BOOKING_RATE_FREE != m_nRoomRateType ) && ( TRUE == m_bCanEditRoomRate );

		CString strRoom;
		strRoom.Format( "%.2f", dNewRoomRate );
		GetEditRate() -> SetWindowText( strRoom );
		GetEditRate() -> SetReadOnly( FALSE == bEditRate  );

		CString strBed;
		strBed.Format( "%.2f", dNewExtraBedRate );
		GetEditBed() -> SetWindowText( strBed );
		GetEditBed() -> SetReadOnly( ( FALSE == bEditRate ) || ( FALSE == bExtraBedTariff ) );
	}

	m_bEditedBooking = TRUE;
	m_BookingRecord.SetRoomRateType( m_nRoomRateType );
	m_BookingRecord.SetRoomRate( dNewRoomRate );
	m_BookingRecord.SetExtraBedRate( dNewExtraBedRate );
	DisplayBalance();

	if ( ( CanEditDinner() == TRUE ) && ( PMS_BOOKING_RATE_DBB == m_nRoomRateType ) )
	{
		m_buttonDinner.EnableWindow( TRUE );
		m_buttonDinner.ShowWindow( SW_SHOW );
	}
	else
	{
		m_buttonDinner.EnableWindow( FALSE );
		m_buttonDinner.ShowWindow( SW_HIDE );
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnChangeRoomRate()
{
	UpdateData( TRUE, FALSE );

	if ( CPriceHelpers::CompareDoubles( m_dRoomRate, m_BookingRecord.GetRoomRate(), 3 ) != 0 )
	{
		m_BookingRecord.SetRoomRate( m_dRoomRate );
		m_bEditedBooking = TRUE;
		DisplayBalance();
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnChangeExtraBedRate()
{
	UpdateData( TRUE, FALSE );

	if ( CPriceHelpers::CompareDoubles( m_dExtraBedRate, m_BookingRecord.GetExtraBedRate(), 3 ) != 0 )
	{
		m_BookingRecord.SetExtraBedRate( m_dExtraBedRate );
		m_bEditedBooking = TRUE;
		DisplayBalance();
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnButtonSearch()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT:
		{
			int nRateTypeOld = m_BookingRecord.GetRoomRateType();

			CPMSBookingPartInfo infoPartOld;
			m_BookingRecord.GetPartInfo( 0, infoPartOld );
			int nRoomNoOld = infoPartOld.GetRoomNo();

			bool bRoomRateChange = FALSE;
			bool bTariffChange = FALSE;
			
			CPMSBookSearchDlg dlg( m_BookingRecord, this );
	
			if ( dlg.DoModal() == IDOK )
			{
				int nRateTypeNew = m_BookingRecord.GetRoomRateType();

				CPMSBookingPartInfo infoPartNew;
				m_BookingRecord.GetPartInfo( 0, infoPartNew );
				int nRoomNoNew = infoPartNew.GetRoomNo();

				int nRoomIdx;
				bool bFoundRoom = FALSE;
				if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( nRoomNoNew, nRoomIdx ) == TRUE )
				{
					bFoundRoom = TRUE;
					CPMSRoomCSVRecord Room;
					DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
					
					if ( m_BookingRecord.GetGuests() == Room.GetSleeps() + 1 )
					{
						switch( m_BookingRecord.GetTariffType() )
						{
						case PMS_TARIFFTYPE_STANDARD:
						case PMS_TARIFFTYPE_SINGLE:
							if ( Room.GetRoomAllowExtraBedFlag() == TRUE )
							{
								m_BookingRecord.SetTariffType( PMS_TARIFFTYPE_EXTRABED );
								bTariffChange = TRUE;
							}
							else if ( Room.GetRoomAllowExtraCotFlag() == TRUE )
							{
								m_BookingRecord.SetTariffType( PMS_TARIFFTYPE_EXTRACOT );
								bTariffChange = TRUE;
							}
							break;
						}
					}
				}

				if ( ( nRateTypeOld != nRateTypeNew ) || ( nRoomNoOld != nRoomNoNew ) )
				{
					if ( TRUE == bFoundRoom )
					{
						m_nActiveRoomIdx = nRoomIdx;
						
						for ( int n = 0; n < m_comboRate.GetCount(); n++ )
						{
							if ( m_comboRate.GetItemData(n) == nRateTypeNew )
							{
								m_comboRate.SetCurSel(n);
								break;
							}
						}

						bRoomRateChange = TRUE;
					}
				}
					
				DisplayBooking();
				
				if ( TRUE == bTariffChange )
					SetDefaultTariffRates();
				else if ( TRUE == bRoomRateChange )
					UpdateRoomRate();

				m_bEditedBooking = TRUE;
			}
		}
		break;

	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_SINGLE_EDIT:
	case PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT:
	default:
		{
			m_BookingRecord.ValidateForEditing( PMSOptions.GetPMSDayNumber(), TRUE );
			if ( m_BookingRecord.GetValidationError() != 0 )
			{
				CString str;
				str.Format( "Unable to change check out date\n\nError code %d", m_BookingRecord.GetValidationError() );
				Prompter.Error( str );
			}
			else
			{
				int nRoomIdx;
				
				CPMSBookingPartInfo infoPart;
				m_BookingRecord.GetFinalPartInfo( infoPart );

				if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( infoPart.GetRoomNo(), nRoomIdx ) == TRUE )
				{
					CPMSBookEditActiveDlg dlg( nRoomIdx, m_BookingRecord, this );
					
					if ( dlg.DoModal() == IDOK )
					{
						DisplayBooking();
						m_bEditedBooking = TRUE;
					}
				}
			}
		}
		break;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnButtonDelete()
{
	if ( PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT == m_nBookingType )
		HandleRestore();
	else
		HandleDelete();
}

/**********************************************************************/

void CPMSBookEditDlg::HandleRestore()
{
	CPMSBookingPartInfo infoPart;
	m_BookingRecord.GetPartInfo( 0, infoPart );

	CPMSBookingQueryInfo Query{};
	Query.m_nStartDay = infoPart.GetStartDay();
	Query.m_nMinNights = infoPart.GetNights();
	Query.m_nMaxNights = infoPart.GetNights();
	Query.m_nRoomNo = infoPart.GetRoomNo();

	CPMSBookingResponseInfo Response;

	PMSCalendarTable.CheckRoomAvailability( Query, Response );

	if ( FALSE == Response.m_bSuccess )
	{
		CString strMsg = "";
		strMsg += "This booking cannot be restored because the requested room\n";
		strMsg += "is reserved for another booking on the selected dates.";
		Prompter.Error( strMsg );
	}
	else
	{
		CString strPrompt = "Are you sure you wish to ";

		CString strExtra;
		GetDeletePromptText( strExtra );
		strPrompt += strExtra;

		if ( Prompter.YesNo( strPrompt, FALSE ) == IDYES )
		{
			m_bRestoreBooking = TRUE;
			EndDialog( IDOK );
		}
	}
}

/**********************************************************************/

void CPMSBookEditDlg::HandleDelete()
{
	CString strPrompt = "Are you sure you wish to ";

	CString strExtra;
	GetDeletePromptText( strExtra );
	strPrompt += strExtra;

	if ( Prompter.YesNo( strPrompt, FALSE ) == IDYES )
	{
		m_bDeleteBooking = TRUE;
		SendEmail( PMS_EMAIL_CANCEL_BOOKING );
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnButtonDeposit()
{
	PMSOptions.BuildValidatedPaymentTypeArray();
	if ( PMSOptions.GetValidatedPaymentTypeArraySize() == 0 )
	{
		PMSOptions.NoPaymentTypeError();
		return;
	}

	CListDataPMSDepositDlg dlgDeposit( m_BookingRecord, this );
	
	if ( dlgDeposit.DoModal() == IDOK )
	{
		DisplayBalance();
		m_bEditedBooking = TRUE;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnButtonDinner()
{
	switch( m_nBookingType )
	{
	case PMS_EDITBOOKING_PENDING_SINGLE_EDIT:
	case PMS_EDITBOOKING_PENDING_SINGLE_NEW:
	case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
		{
			CString strDepts = m_BookingRecord.GetDinnerDepartments();
			double dAllow = m_BookingRecord.GetDinnerAllowance();

			if ( ( strDepts == "" ) && ( CPriceHelpers::CompareDoubles( dAllow, 0.0, 3 ) == 0 ) ) 
			{
				m_BookingRecord.SetDinnerDepartments( PMSOptions.GetDinnerDepartments() );
				m_BookingRecord.SetDinnerAllowance( PMSOptions.GetDinnerAllowance() );
				m_bEditedBooking = TRUE;
			}
		}
		break;
	}

	CPMSOptions LocalPMSOptions;
	LocalPMSOptions.SetDinnerAllowance( m_BookingRecord.GetDinnerAllowance() );
	LocalPMSOptions.SetDinnerDepartments( m_BookingRecord.GetDinnerDepartments() );

	CPropertySheetPMSDinner propSheet( &LocalPMSOptions, this );

	if ( propSheet.DoModal() == IDOK )
	{
		m_BookingRecord.SetDinnerAllowance( LocalPMSOptions.GetDinnerAllowance() );
		m_BookingRecord.SetDinnerDepartments( LocalPMSOptions.GetDinnerDepartments() );
		DisplayBalance();
		m_bEditedBooking = TRUE;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnOK()
{
	if ( UpdateData( TRUE ) == TRUE )
	{
		int nCustomerId = m_infoAccount.GetCustomerId();

		m_CustomerRecord.SetCustomerId( nCustomerId );
		m_CustomerRecord.SetGuestName( m_strGuestName );
		m_CustomerRecord.SetBillingName( m_strBillingName );
		m_CustomerRecord.SetAddress1( m_strAddress1 );
		m_CustomerRecord.SetAddress2( m_strAddress2 );
		m_CustomerRecord.SetAddress3( m_strAddress3 );
		m_CustomerRecord.SetPostcode( m_strPostcode );
		m_CustomerRecord.SetPhone( m_strPhone );
		m_CustomerRecord.SetFax( m_strFax );
		m_CustomerRecord.SetEmail( m_strEmail );

		switch( m_nBookingType )
		{
		case PMS_EDITBOOKING_ACTIVE_SINGLE_NEW:
		case PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT:
			m_bMultipleAccounts = ( m_checkAccounts.GetCheck() != 0 );
			break;

		default:
			m_bMultipleAccounts = FALSE;
			break;
		}

		if ( PMS_EDITBOOKING_ACTIVE_SINGLE_NEW == m_nBookingType )
		{
			CString strDepts = m_BookingRecord.GetDinnerDepartments();
			double dAllow = m_BookingRecord.GetDinnerAllowance();

			if ( ( strDepts == "" ) && ( CPriceHelpers::CompareDoubles( dAllow, 0.0, 3 ) == 0 ) ) 
			{
				m_BookingRecord.SetDinnerDepartments( PMSOptions.GetDinnerDepartments() );
				m_BookingRecord.SetDinnerAllowance( PMSOptions.GetDinnerAllowance() );
				m_bEditedBooking = TRUE;
			}

			m_bEditedBooking = TRUE;
		}

		if ( m_strNote != m_BookingRecord.GetBookingNote() )
		{
			m_BookingRecord.SetBookingNote( m_strNote );
			m_bEditedBooking = TRUE;
		}

		if ( CPriceHelpers::CompareDoubles( m_dRoomRate, m_BookingRecord.GetRoomRate(), 2 ) != 0 )
		{
			m_BookingRecord.SetRoomRate( m_dRoomRate );
			m_bEditedBooking = TRUE;
		}

		if ( CPriceHelpers::CompareDoubles( m_dExtraBedRate, m_BookingRecord.GetExtraBedRate(), 2 ) != 0 )
		{
			m_BookingRecord.SetExtraBedRate( m_dExtraBedRate );
			m_bEditedBooking = TRUE;
		}

		if ( PMSOptions.GetMiscEnableTrainingMode() == TRUE )
		{
			bool bTraining = ( m_checkTraining.GetCheck() != 0 );
			if ( bTraining != m_BookingRecord.GetTrainingFlag() )
			{
				if ( TRUE == bTraining )
				{
					CString strMsg = "";
					strMsg += "Are you sure you wish to switch this booking to training mode.\n\n";
					strMsg += "Training mode bookings will be deleted after check out.\n";

					if ( Prompter.YesNo( strMsg ) != IDYES )
						return;
				}

				m_BookingRecord.SetTrainingFlag( bTraining );
				m_bEditedBooking = TRUE;
			}
		}

		if ( m_nRoomRateType != m_BookingRecord.GetRoomRateType() )
		{
			m_BookingRecord.SetRoomRateType( m_nRoomRateType );
			m_bEditedBooking = TRUE;
		}

		if ( m_BookingRecord.CheckEmailDates() == FALSE )
		{
			if ( m_BookingRecord.GetEmailStartDay() == 0 )
				SendEmail( PMS_EMAIL_CREATE_BOOKING );
			else
				SendEmail( PMS_EMAIL_AMEND_BOOKING );
		}
		else if ( m_checkEmail.GetCheck() == FALSE )
			SendEmail( PMS_EMAIL_CREATE_BOOKING );

		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPMSBookEditDlg::SendEmail( int nType )
{
	if ( CanSendEmail() == FALSE )
		return;

	if ( PMSOptions.GetMiscEnableEmailConfirmation() == FALSE )
		return;

	if ( m_strEmail.Find( "@" ) == -1 )
		return;

	CString strPrompt = "";

	switch( nType )
	{
	case PMS_EMAIL_CREATE_BOOKING:
		strPrompt = "Would you like to email a confirmation of this booking";
		break;

	case PMS_EMAIL_AMEND_BOOKING:
		strPrompt = "Would you like to email a confirmation of this change\nto the existing booking";
		break;
	
	case PMS_EMAIL_CANCEL_BOOKING:
		strPrompt = "Would you like to email a confirmation that this booking\nhas been cancelled";
		break;
	}

	if ( strPrompt == "" )
		return;

	if ( Prompter.YesNo( strPrompt, TRUE ) != IDYES )
	{
		if ( PMS_EMAIL_AMEND_BOOKING == nType )
			m_BookingRecord.SetEmailDates();

		return;
	}

	CReportFile ReportFile;
	if ( ReportFile.Open ( Super.ReportFilename() ) == FALSE )
	{
		Prompter.Error( "Unable to create file for Email" );
		return;
	}

	CString strTelLine = PMSOptions.GetHotelPhone();
	::TrimSpaces( strTelLine, FALSE );
	if ( strTelLine != "" )
	{
		CString strTemp = strTelLine;
		strTelLine = "T: ";
		strTelLine += strTemp;
	}

	CString strFaxLine = PMSOptions.GetHotelFax();
	::TrimSpaces( strFaxLine, FALSE );
	if ( strFaxLine != "" )
	{
		CString strTemp = strFaxLine;
		strFaxLine = "F: ";
		strFaxLine += strTemp;
	}

	CString strReference;
	strReference.Format( "Your booking reference is %8.8d.",
		m_BookingRecord.GetBookingId() );
	
	CPMSBookingPartInfo infoFirstPart, infoFinalPart;
	m_BookingRecord.GetPartInfo( 0, infoFirstPart );
	m_BookingRecord.GetFinalPartInfo( infoFinalPart );

	int nStartDay = infoFirstPart.GetStartDay();
	int nNights = infoFinalPart.GetStartDay() + infoFinalPart.GetNights() - nStartDay;
	int nEndDay = nStartDay + nNights;

	COleDateTime dateStart, dateEnd;
	GetOleDateSince2011( nStartDay, dateStart );
	GetOleDateSince2011( nEndDay, dateEnd );

	CString strDates;
	strDates.Format( "For %d night%s between %2.2d/%2.2d/%4.4d and %2.2d/%2.2d/%4.4d",
		nNights,
		( 1 == nNights ) ? "" : "s",
		dateStart.GetDay(),
		dateStart.GetMonth(),
		dateStart.GetYear(),
		dateEnd.GetDay(),
		dateEnd.GetMonth(),
		dateEnd.GetYear() );

	CString strSubject = "";

	switch( nType )
	{
	case PMS_EMAIL_CREATE_BOOKING:
		strSubject = "Booking Confirmation";
		break;

	case PMS_EMAIL_AMEND_BOOKING:
		strSubject = "Confirmation of Amended Booking";
		break;

	case PMS_EMAIL_CANCEL_BOOKING:
		strSubject = "Booking Cancellation";
		break;
	}

	ReportFile.SetStyle1( strSubject );
	ReportFile.AddColumn ( "", TA_LEFT, 6000 );

	switch( nType )
	{
	case PMS_EMAIL_CREATE_BOOKING:
		ReportFile.WriteLine( "This is your booking confirmation for:" );
		break;

	case PMS_EMAIL_AMEND_BOOKING:
		ReportFile.WriteLine( "This is your amended booking confirmation for:" );
		break;

	case PMS_EMAIL_CANCEL_BOOKING:
		ReportFile.WriteLine( "This is your confirmation that the following booking has been CANCELLED:" );
		break;
	}
	
	ReportFile.WriteLine( "" );
	ReportFile.WriteLine( PMSOptions.GetHotelName() );
	ReportFile.WriteLine( PMSOptions.GetHotelAddress1() );
	ReportFile.WriteLine( PMSOptions.GetHotelAddress2() );
	ReportFile.WriteLine( PMSOptions.GetHotelAddress3() );
	ReportFile.WriteLine( PMSOptions.GetHotelPostcode() );
	if ( strTelLine != "" )ReportFile.WriteLine( strTelLine );
	if ( strFaxLine != "" )ReportFile.WriteLine( strFaxLine );
	ReportFile.WriteLine( "" );
	ReportFile.WriteLine( strReference );
	ReportFile.WriteLine( "" );
	
	if ( PMS_EMAIL_CANCEL_BOOKING != nType )
	{
		if ( PMSOptions.GetMiscEmailShowNote() == TRUE )
		{
			CString strNote = m_BookingRecord.GetBookingNote();
			::TrimSpaces( strNote, FALSE );
			::HTMLEuroConvert( strNote );

			if ( strNote != "" )
			{
				ReportFile.WriteLine( strNote );
				ReportFile.WriteLine( "" );
			}
		}
	}

	ReportFile.WriteLine( strDates );
	
	if ( PMS_EMAIL_CANCEL_BOOKING != nType )
	{
		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( "Please quote your booking reference when you check in." );

		CMultilineTextBuffer EmailSignature;

		int nRoomGroup = 1;
		{
			int nRoomIdx;
			int nRoomNo = infoFinalPart.GetRoomNo();
			if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( nRoomNo, nRoomIdx ) == TRUE )
			{
				CPMSRoomCSVRecord Room;
				DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
				nRoomGroup = Room.GetRoomGroup();
			}
		}

		EmailSignature.SetPMSGroupSource( nRoomGroup );
		EmailSignature.Read();
		EmailSignature.AppendToFile( ReportFile );
	}

	ReportFile.Close();

	bool bResult = FALSE;
	CString strError = "";

	if ( EmailOptions.GetEmailMethodToUse() == 0 )
	{
		CReportFileDoc ReportFileDoc( Super.ReportFilename() );
		ReportFileDoc.SaveAsHTML( Super.HTMLFilename() );

		char cBuffer[_MAX_PATH]{};
		cBuffer[0] = 0;
		strcat_s ( cBuffer, Super.HTMLFilename() );

		CSSMapi32 EmailHandler;
		bResult = EmailHandler.SendMessage( m_strEmail, strSubject, cBuffer, TRUE );
		
		if (FALSE == bResult)
		{
			strError = EmailHandler.GetError();
		}
	}
	else
	{
		CSMTPEmailHelpers SMTPEmailHelpers;
		SMTPEmailHelpers.OpenSession( 2 );

		CCSV csvFriendlyText;
		csvFriendlyText.Add( "Booking Confirmation" );
		csvFriendlyText.Add( m_CustomerRecord.GetGuestName() );
		CString strFriendlyText = csvFriendlyText.GetLine();

		CString strEmailFilename;
		strEmailFilename.Format( "%d_%d",
			m_BookingRecord.GetBookingId(),
			m_CustomerRecord.GetCustomerId() );

		CReportFileDoc ReportFileDoc ( Super.ReportFilename() );
		SMTPEmailHelpers.CreatePendingEmail( m_strEmail, strSubject, csvFriendlyText.GetLine(), ReportFileDoc, strEmailFilename );

		SMTPEmailHelpers.CreateIndexFile();
		CSMTPResultsDlg dlg( 2, SMTPEmailHelpers, this );
		dlg.DoModal();
		bResult = SMTPEmailHelpers.CheckAllSent();
		SMTPEmailHelpers.DeleteSession();
	}
 
	if ( FALSE == bResult )
	{
		if (strError != "")
		{
			Prompter.Error(strError);
		}
	}
	else 
	{
		m_BookingRecord.SetEmailDates();
		m_bEmailedBooking = TRUE;
	}
}

/**********************************************************************/

void CPMSBookEditDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
