/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "Consolidation.h"
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "PMSBookSearchDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSBookSearchDlg, CDialog)
/**********************************************************************/

CPMSBookSearchDlg::CPMSBookSearchDlg( CPMSBookingCSVRecord& BookingRecord, CWnd* pParent) : CSSDialog(CPMSBookSearchDlg::IDD, pParent), m_BookingRecord( BookingRecord )
{
	m_nCurrentBookedRoomNo = 0;
	m_bNewBooking = ( m_BookingRecord.GetPartCount() == 0 );

	if ( TRUE == m_bNewBooking )
	{
		m_nSearchCheckInDayMin = PMSOptions.GetPMSDayNumber();
		m_nSearchCheckInDay = PMSOptions.GetPMSDayNumber();
		m_nSearchCheckOutDay = m_nSearchCheckInDay + 1;
		m_nSearchRoomSleeps = 0;
		m_nSearchRoomSubType = 0;
		m_nSearchGuests = 1;
		m_nListBookedRooms = 0;
	}
	else
	{
		m_nSearchCheckInDay = m_BookingRecord.GetSearchStartDay();
		m_nSearchCheckOutDay = m_BookingRecord.GetSearchStartDay() + m_BookingRecord.GetSearchNights();
		m_nSearchRoomSleeps = m_BookingRecord.GetSearchRoomSleeps();
		m_nSearchRoomSubType = m_BookingRecord.GetSearchRoomSubType();
		m_nSearchGuests = m_BookingRecord.GetGuests();

		m_BookingRecord.BuildRoomListForEditing( m_arrayRooms );
 
		m_nListBookedRooms = m_BookingRecord.GetPartCount();
		
		if ( m_nListBookedRooms > 0 )
		{
			CPMSBookSearchListInfo info;
			m_arrayRooms.Add( info );

			info = m_arrayRooms.GetAt( 0 );
			m_nCurrentBookedRoomNo = info.m_nRoomNo;
		}

		m_nSearchCheckInDayMin = CPMSBookingCSVArray::GetCurrentDayNumber();
		if ( m_nSearchCheckInDayMin > m_nSearchCheckInDay )
			m_nSearchCheckInDayMin = m_nSearchCheckInDay;
	}

	m_nSearchCheckInBase = m_nSearchCheckInDay;
	m_nSearchCheckOutBase = m_nSearchCheckOutDay;
		
	m_nListStartDay = 0;
	m_nListNights = 1;
	m_nListRoomSleeps = 1;
	m_nListRoomSubType = 0;
	m_nListGuests = 1;
	m_nListUnbookedDay = 0;
	
	m_strDisplayLine = "";
}

/**********************************************************************/

CPMSBookSearchDlg::~CPMSBookSearchDlg()
{
}

/**********************************************************************/

void CPMSBookSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_CHECKIN, m_comboCheckIn);
	DDX_Control(pDX, IDC_DATEPICKER_CHECKIN, m_datePickerCheckIn);

	DDX_Control(pDX, IDC_COMBO_CHECKOUT, m_comboCheckOut);
	DDX_Control(pDX, IDC_DATEPICKER_CHECKOUT, m_datePickerCheckOut);

	DDX_Control(pDX, IDC_COMBO_ROOMTYPE, m_comboRoomType);
	DDX_Control(pDX, IDC_COMBO_RATE, m_comboRoomRate);

	DDX_Control(pDX, IDOK, m_buttonBook);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSBookSearchDlg, CDialog)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKIN, OnChangeDateCheckIn)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKIN, OnOpenCheckIn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_CHECKIN, OnDatePickerCheckIn)
	
	ON_EN_KILLFOCUS(IDC_EDIT_NIGHTS,OnKillFocusNights)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKOUT, OnChangeDateCheckOut)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKOUT, OnOpenCheckOut)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_CHECKOUT, OnDatePickerCheckOut)

	ON_EN_KILLFOCUS(IDC_EDIT_GUESTS,OnKillFocusGuests)

	ON_BN_CLICKED(IDC_BUTTON_SEARCH,OnButtonSearch)

	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)

END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSBookSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( TRUE == m_bNewBooking )
		SetWindowText( "Book Room" );
	else
		SetWindowText( "Edit Booking" );

	m_buttonBook.EnableWindow( FALSE );
	
	m_datePickerCheckIn.SetFormat( "'calendar'" );
	m_datePickerCheckOut.SetFormat( "'calendar'" );

	SubclassEdit( IDC_EDIT_NIGHTS, SS_NUM, 3, "%d" );
	SubclassEdit( IDC_EDIT_GUESTS, SS_NUM, 1, "%s" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(35);
	arrayStops.Add(55);
	arrayStops.Add(80);
	m_comboCheckIn.SetTabStops( arrayStops );
	m_comboCheckOut.SetTabStops( arrayStops );

	if ( ( m_nSearchCheckInDay < m_nSearchCheckInDayMin ) || ( m_nSearchCheckInDay > MAX_PMS_DAY - 1 ) )
		m_nSearchCheckInDay = 0;

	if ( ( m_nSearchCheckOutDay < m_nSearchCheckInDayMin + 1 ) || ( m_nSearchCheckOutDay > MAX_PMS_DAY ) )
		m_nSearchCheckOutDay = 1;

	FillCheckInCombo();
	UpdateNights();
	UpdateCheckOutDay();
	FillCheckOutCombo();

	COleDateTime dateCheckInMin, dateCheckInMax, dateCheckOutMin, dateCheckOutMax;
	GetOleDateSince2011( m_nSearchCheckInDayMin, dateCheckInMin );
	GetOleDateSince2011( MAX_PMS_DAY - 1, dateCheckInMax );
	GetOleDateSince2011( m_nSearchCheckInDayMin + 1, dateCheckOutMin );
	GetOleDateSince2011( MAX_PMS_DAY, dateCheckOutMax );
	
	m_datePickerCheckIn.SetRange( &dateCheckInMin, &dateCheckInMax );
	m_datePickerCheckOut.SetRange( &dateCheckOutMin, &dateCheckOutMax );	
	UpdateDatePickerDates();

	m_listRooms.SubclassDlgItem ( IDC_LIST, this );
	m_listRooms.InsertColumn ( 0, "No", LVCFMT_LEFT, 40 );
	m_listRooms.InsertColumn ( 1, "Room", LVCFMT_LEFT, 180 );
	m_listRooms.InsertColumn ( 2, "Type", LVCFMT_LEFT, 100 );
	m_listRooms.InsertColumn ( 3, "Sleeps", LVCFMT_LEFT, 50 );
	m_listRooms.InsertColumn ( 4, "Options", LVCFMT_LEFT, 100 );
	m_listRooms.SetItemCountEx ( m_arrayRooms.GetSize() );

	FillRoomTypeCombo( m_nSearchRoomSleeps, m_nSearchRoomSubType, m_nSearchGuests );

	if ( FALSE == m_bNewBooking )
	{
		CString strGuests;
		strGuests.Format( "%d", m_nSearchGuests );
		GetEditGuests() -> SetWindowText( strGuests );
	}
	else
		GetEditGuests() -> SetWindowText( "" );

	int nRoomRateType = m_BookingRecord.GetRoomRateType();

	switch( nRoomRateType )
	{
	case PMS_BOOKING_RATE_ROOM:
	case PMS_BOOKING_RATE_BB:
	case PMS_BOOKING_RATE_DBB:
	case PMS_BOOKING_RATE_FREE:
		break;

	default:
		nRoomRateType = PMS_BOOKING_RATE_ROOM;
		break;
	}

	m_comboRoomRate.AddString( "Room Only" );
	m_comboRoomRate.AddString( "B & B" );
	m_comboRoomRate.AddString( "DB & B" );
	
	m_comboRoomRate.SetItemData( 0, PMS_BOOKING_RATE_ROOM );
	m_comboRoomRate.SetItemData( 1, PMS_BOOKING_RATE_BB );
	m_comboRoomRate.SetItemData( 2, PMS_BOOKING_RATE_DBB );
	
	if ( ( PMS_BOOKING_RATE_FREE == nRoomRateType ) || ( PMSOptions.GetMiscEnableFreeRoom() == TRUE ) )
	{
		m_comboRoomRate.InsertString( 0, "Free Room" );
		m_comboRoomRate.SetItemData( 0, PMS_BOOKING_RATE_FREE );
	}

	int nSel = 0;
	for ( int n = 0; n < m_comboRoomRate.GetCount(); n++ )
	{
		if ( m_comboRoomRate.GetItemData( n ) == nRoomRateType )
		{
			nSel = n;
			break;
		}
	}

	m_comboRoomRate.SetCurSel( nSel );

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSBookSearchDlg::GetEditNights(){ return GetEdit( IDC_EDIT_NIGHTS ); }
CEdit* CPMSBookSearchDlg::GetEditGuests(){ return GetEdit( IDC_EDIT_GUESTS ); }
/**********************************************************************/

void CPMSBookSearchDlg::FillCheckInCombo()
{
	FillDateCombo( m_comboCheckIn, m_nSearchCheckInBase, m_nSearchCheckInDay );
}

/**********************************************************************/

void CPMSBookSearchDlg::FillCheckOutCombo()
{
	FillDateCombo( m_comboCheckOut, m_nSearchCheckOutBase, m_nSearchCheckOutDay );
}

/**********************************************************************/

void CPMSBookSearchDlg::FillDateCombo( CSSTabbedComboBox& combo, int& nBaseDay, int nSelDay )
{
	nBaseDay = nSelDay - 7;
	
	if ( nBaseDay < m_nSearchCheckInDayMin )
		nBaseDay = m_nSearchCheckInDayMin;

	if ( nBaseDay > nSelDay )
		nBaseDay = nSelDay;

	int nEndDay = nSelDay + 7;
	if ( nEndDay > MAX_PMS_DAY )
		nEndDay = MAX_PMS_DAY;

	if ( nBaseDay > nEndDay )
		nBaseDay = nEndDay;

	combo.ResetContent();

	for ( int nDay = nBaseDay; nDay <= nEndDay; nDay++ )
	{
		COleDateTime date;
		GetOleDateSince2011( nDay, date );

		CString strDayName = date.Format( "%a" );
		CString strDayNum = date.Format( "%d" );
		CString strMonth = date.Format( "%b" );
		CString strYear = date.Format( "%Y" );
		
		CCSV csv;
		csv.Add( strDayName );
		csv.Add( strDayNum );
		csv.Add( strMonth );
		csv.Add( strYear );
		combo.AddItem( csv.GetLine() );
	}

	combo.SetCurSel( nSelDay - nBaseDay );
}

/**********************************************************************/

void CPMSBookSearchDlg::OnOpenCheckIn()
{
	FillCheckInCombo();
}

/**********************************************************************/

void CPMSBookSearchDlg::OnOpenCheckOut()
{
	FillCheckOutCombo();
}

/**********************************************************************/

void CPMSBookSearchDlg::UpdateNights()
{
	int nNights = m_nSearchCheckOutDay - m_nSearchCheckInDay;
	if ( ( nNights >= 1 ) && ( nNights <= PMSBooking::Nights.Max ) )
	{
		CString strNights;
		strNights.Format( "%d", nNights );
		GetEditNights() -> SetWindowText( strNights );
	}
	else
		GetEditNights() -> SetWindowText( "" );
}

/**********************************************************************/

void CPMSBookSearchDlg::UpdateCheckOutDay()
{
	CString strNights;
	GetEditNights() -> GetWindowText( strNights );
	int nNights = atoi( strNights );

	if ( ( nNights < 1 ) || ( nNights > PMSBooking::Nights.Max ) )
		nNights = 1;

	m_nSearchCheckOutDay = m_nSearchCheckInDay + nNights;

	if ( m_nSearchCheckOutDay > MAX_PMS_DAY )
		m_nSearchCheckOutDay = MAX_PMS_DAY;
}

/**********************************************************************/

void CPMSBookSearchDlg::UpdateDatePickerDates()
{
	{
		int nDay = m_nSearchCheckInDay;
		
		if ( ( nDay < 0 ) || ( nDay > MAX_PMS_DAY - 1 ) )
			nDay = 0;

		COleDateTime date;
		GetOleDateSince2011( nDay, date );
		m_datePickerCheckIn.SetTime( date );
	}

	{
		int nDay = m_nSearchCheckOutDay;
		
		if ( ( nDay < 1 ) || ( nDay > MAX_PMS_DAY ) )
			nDay = 1;

		COleDateTime date;
		GetOleDateSince2011( nDay, date );
		m_datePickerCheckOut.SetTime( date );
	}
}

/**********************************************************************/

void CPMSBookSearchDlg::OnChangeDateCheckIn() 
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	
	UpdateCheckOutDay();	
	UpdateNights();
	FillCheckOutCombo();
	UpdateDatePickerDates();
}

/**********************************************************************/

void CPMSBookSearchDlg::OnDatePickerCheckIn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePickerCheckIn.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= 0 ) && ( nDate <= MAX_PMS_DAY - 1 ) )
	{
		m_nSearchCheckInDay = nDate;

		FillCheckInCombo();
		UpdateCheckOutDay();
		UpdateNights();
		FillCheckOutCombo();
	}
	
	UpdateDatePickerDates();
	*pResult = 0;
}

/**********************************************************************/

void CPMSBookSearchDlg::OnKillFocusNights()
{
	CString str;
	GetEditNights() -> GetWindowText( str );
	int nNights = atoi( str );

	if ( ( nNights >= 1 ) && ( nNights <= PMSBooking::Nights.Max ) )
	{
		m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();

		int nDay = m_nSearchCheckInDay + nNights;

		if ( nDay <= MAX_PMS_DAY )
			m_nSearchCheckOutDay = nDay;
	}

	UpdateNights();
	FillCheckOutCombo();
	UpdateDatePickerDates();
}

/**********************************************************************/

void CPMSBookSearchDlg::OnChangeDateCheckOut() 
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	m_nSearchCheckOutDay = m_nSearchCheckOutBase + m_comboCheckOut.GetCurSel();
	
	UpdateNights();
	UpdateDatePickerDates();
}

/**********************************************************************/

void CPMSBookSearchDlg::OnDatePickerCheckOut(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePickerCheckOut.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= 1 ) && ( nDate <= MAX_PMS_DAY ) )
	{
		m_nSearchCheckOutDay = nDate;

		UpdateNights();
		FillCheckOutCombo();
	}

	UpdateDatePickerDates();
	*pResult = 0;
}

/**********************************************************************/

void CPMSBookSearchDlg::OnKillFocusGuests()
{
	CString str;
	GetEditGuests() -> GetWindowText( str );
	int nGuests = atoi( str );

	if ( ( nGuests >= PMSRoom::Sleeps.Min ) && ( nGuests <= PMSRoom::Sleeps.Max + 1 ) )
	{
		int nRoomSleeps = 0;
		int nRoomSubType = 0;

		int nSel = m_comboRoomType.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_comboRoomType.GetCount() ) )
		{
			int nRoomType = m_comboRoomType.GetItemData( nSel );
			nRoomSleeps = nRoomType / 100;
			nRoomSubType = nRoomType % 100;
		}

		FillRoomTypeCombo( nRoomSleeps, nRoomSubType, nGuests );
	}
}

/**********************************************************************/

void CPMSBookSearchDlg::OnButtonSearch()
{
	if ( m_nSearchCheckInDay >= m_nSearchCheckOutDay )
	{
		Prompter.Error( "The check out day for this search\nmust be after the check in day." );
		m_comboCheckOut.SetFocus();
		return;
	}

	CString strGuests;
	GetEditGuests() -> GetWindowText( strGuests );

	m_nSearchGuests = atoi( strGuests );

	if ( ( m_nSearchGuests < 1 ) || ( m_nSearchGuests > PMSBooking::Guests.Max ) )
	{
		CString strMsg;
		strMsg.Format( "Please specify the number of guests\nfor this search (maximum %d)", 
			PMSBooking::Guests.Max );
		Prompter.Error( strMsg );
		
		GetEditGuests() -> SetFocus();
		GetEditGuests() -> SetSel( 0, -1 );
		return;
	}

	int nRoomSleeps = 0;
	int nRoomSubType = 0;

	int nSel = m_comboRoomType.GetCurSel();
	if ( nSel != 0 )
	{
		int nRoomType = m_comboRoomType.GetItemData( nSel );
		nRoomSleeps = nRoomType / 100;
		nRoomSubType = nRoomType % 100;

		if ( m_nSearchGuests > nRoomSleeps + 1 ) 
			return;
	}
	
	COleDateTime dateFrom, dateTo;
	GetOleDateSince2011( m_nSearchCheckInDay, dateFrom );
	GetOleDateSince2011( m_nSearchCheckOutDay, dateTo );
		
	m_arrayRooms.RemoveAll();
	m_nListStartDay = m_nSearchCheckInDay;
	m_nListNights = m_nSearchCheckOutDay - m_nSearchCheckInDay;
	m_nListRoomSleeps = nRoomSleeps;
	m_nListRoomSubType = nRoomSubType;
	m_nListGuests = m_nSearchGuests;
	m_nListBookedRooms = 0;
	m_nListUnbookedDay = m_nListStartDay;

	UpdateRoomList();
	
	int nSize = m_arrayRooms.GetSize();

	m_listRooms.SetItemCountEx( nSize );
	m_listRooms.Invalidate();

	if ( nSize > 0 )
		m_listRooms.SetCurSel(0);

	m_buttonBook.EnableWindow( nSize > 0 );
}

/**********************************************************************/

void CPMSBookSearchDlg::UpdateRoomList()
{
	CReportConsolidationArray<CSortedIntByString> IndexArray;
	CArray<CPMSBookingQueryInfo,CPMSBookingQueryInfo> QueryArray;
	CArray<CPMSBookingResponseInfo,CPMSBookingResponseInfo> ResponseArray;

	int nNightsWanted = m_nListNights - ( m_nListUnbookedDay - m_nListStartDay );

	for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
	{
		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

		int nMaxGuests = Room.GetSleeps();
		if ( Room.GetRoomAllowExtraBedFlag() || Room.GetRoomAllowExtraCotFlag() )
			nMaxGuests++;

		if ( nMaxGuests < m_nListGuests )
			continue;

		if ( m_nListRoomSleeps != 0 )
		{
			if ( Room.GetSleeps() != m_nListRoomSleeps )
				continue;

			if ( Room.GetRoomType() != m_nListRoomSubType )
				continue;
		}

		CPMSBookingQueryInfo Query;
		Query.m_nStartDay = m_nListUnbookedDay;
		Query.m_nMinNights = nNightsWanted;
		Query.m_nMaxNights = nNightsWanted;
		Query.m_nRoomNo = Room.GetRoomNo();

		CPMSBookingResponseInfo Response;

		PMSCalendarTable.CheckRoomAvailability( Query, Response );

		if ( TRUE == Response.m_bSuccess )
		{
			CString strLabel;
			strLabel.Format( "%4.4d%3.3d",
			Query.m_nMaxNights - Response.m_nMaxNights,
				nRoomIdx );

			CSortedIntByString item;
			item.m_strItem = strLabel;
			item.m_nItem = QueryArray.GetSize();
			IndexArray.Consolidate( item );

			QueryArray.Add( Query );
			ResponseArray.Add( Response );
		}
	}

	//FIDDLE - PREVENT SUGGESTION OF ROOMS THAT ARE NOT AVAILABLE FOR FULL STAY
	//bool bGotRoom = FALSE;
	bool bGotRoom = TRUE;

	for ( int n = 0; n < IndexArray.GetSize(); n++ )
	{
		CSortedIntByString item;
		IndexArray.GetAt( n, item );
		int nArrayIdx = item.m_nItem;

		CPMSBookingQueryInfo Query = QueryArray[ nArrayIdx ];
		CPMSBookingResponseInfo Response = ResponseArray[ nArrayIdx ];

		if ( ( TRUE == bGotRoom ) && ( Query.m_nMaxNights != Response.m_nMaxNights ) )
			continue;

		CString strLabel = item.m_strItem;
		int nRoomIdx = atoi( strLabel.Right(3) );

		COleDateTime dateAvail;
		GetOleDateSince2011( Response.m_nStartDay + Response.m_nMaxNights, dateAvail );

		CString strDate;
		strDate.Format( "%2.2d/%2.2d/%4.4d",
			dateAvail.GetDay(),
			dateAvail.GetMonth(),
			dateAvail.GetYear() );

		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

		CPMSBookSearchListInfo info;
		info.m_nRoomNo = Room.GetRoomNo();
		info.m_strRoomName = Room.GetRoomName();
		info.m_strDate = strDate;
		info.m_strType = Room.GetRoomTypeName( FALSE );
		info.m_nSleeps = Room.GetSleeps();
		info.m_nStartDay = Response.m_nStartDay;
		info.m_nNights = Response.m_nMaxNights;
		info.m_bFullStay = ( Query.m_nMaxNights == Response.m_nMaxNights );
		info.m_bExtraBed = Room.GetRoomAllowExtraBedFlag();
		info.m_bExtraCot = Room.GetRoomAllowExtraCotFlag();

		if ( info.m_nRoomNo == m_nCurrentBookedRoomNo )
			m_arrayRooms.InsertAt( 0, info );
		else
			m_arrayRooms.Add( info );

		if ( TRUE == info.m_bFullStay )
			bGotRoom = TRUE;
	}
}

/**********************************************************************/

void CPMSBookSearchDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CPMSBookSearchDlg::PrepareLine( int nIndex )
{
	m_strDisplayLine = "";

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayRooms.GetSize() ) )
	{
		CCSV csv ('\t');

		if ( ( m_nListBookedRooms != 0 ) && ( nIndex == m_nListBookedRooms ) )
		{
			csv.Add( "--" );
			csv.Add( "-----" );
			csv.Add( "-----" );
			csv.Add( "--" );
			csv.Add( "-----" );
		}
		else
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

			csv.Add( m_arrayRooms[ nIndex ].m_nRoomNo );
			csv.Add( m_arrayRooms[ nIndex ].m_strRoomName );
			csv.Add( m_arrayRooms[ nIndex ].m_strType );
			csv.Add( strSleeps );
			csv.Add( strOption );
		}

		m_strDisplayLine = csv.GetLine();
	}
}

/**********************************************************************/

void CPMSBookSearchDlg::FillRoomTypeCombo( int nRoomSleeps, int nRoomType, int nGuestCount )
{
	m_comboRoomType.ResetContent();

	m_comboRoomType.AddString( "Any" );
	m_comboRoomType.SetItemData( 0, 0 );

	int nMinSleeps = nGuestCount;
	if ( nMinSleeps > 1 )
	{
		for ( int nRoomIdx = 0; nRoomIdx < DataManagerNonDb.PMSRoom.GetSize(); nRoomIdx++ )
		{
			CPMSRoomCSVRecord Room;
			DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

			if ( Room.GetSleeps() == nMinSleeps - 1 )
			{
				if ( Room.GetRoomAllowExtraBedFlag() || Room.GetRoomAllowExtraCotFlag() )
				{
					nMinSleeps--;
					break;
				}
			}
		}
	}

	for ( int nSleeps = nMinSleeps; nSleeps <= PMSRoom::Sleeps.Max; nSleeps++ )
	{
		CCSV csv( DataManagerNonDb.PMSRoomType.GetRoomTypes( nSleeps ) );

		int nPos = m_comboRoomType.AddString( csv.GetString(0) );
		m_comboRoomType.SetItemData( nPos, nSleeps * 100 );

		for ( int nType = 1; nType < 6; nType++ )
		{
			if ( csv.GetBool( ( nType * 2 ) + 1 ) == TRUE )
			{
				int nPos = m_comboRoomType.AddString( csv.GetString( nType * 2 ) );
				m_comboRoomType.SetItemData( nPos, ( nSleeps * 100 ) + nType );
			}
		}
	}

	int nSel = -1;
	int nTarget = ( nRoomSleeps * 100 ) + nRoomType;

	for ( int n = 0; n < m_comboRoomType.GetCount(); n++ )
	{
		int nItemData = m_comboRoomType.GetItemData(n);

		if ( ( nSel == -1 ) && ( nItemData / 100 == nRoomSleeps ) )
			nSel = n;
		
		if ( nItemData == nTarget )
		{
			nSel = n;
			break;
		}
	}

	if ( nSel == -1 )
		nSel = 0;

	m_comboRoomType.SetCurSel( nSel );
}

/**********************************************************************/

void CPMSBookSearchDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CPMSBookSearchDlg::OnOK()
{
	int nSel = m_listRooms.GetCurSel();
	if ( ( nSel < 0 ) || ( nSel >= m_arrayRooms.GetSize() ) )
		return;

	if ( ( m_nListBookedRooms != 0 ) && ( nSel < m_nListBookedRooms + 1 ) )
		return;

	CPMSBookSearchListInfo infoList = m_arrayRooms[ nSel ];

	if ( TRUE == infoList.m_bFullStay )
	{
		m_arrayRooms.InsertAt( m_nListBookedRooms++, infoList );

		int nBookingId = m_BookingRecord.GetBookingId();
		int nCustomerId = m_BookingRecord.GetCustomerId();
		int nEmailStartDay = m_BookingRecord.GetEmailStartDay();
		int nEmailNights = m_BookingRecord.GetEmailNights();

		m_BookingRecord.ClearRecord( FALSE );

		m_BookingRecord.SetBookingId( nBookingId );
		m_BookingRecord.SetCustomerId( nCustomerId );
		m_BookingRecord.SetEmailStartDay( nEmailStartDay );
		m_BookingRecord.SetEmailNights( nEmailNights );
		m_BookingRecord.SetGuests( m_nListGuests );
		m_BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_PENDING );
		m_BookingRecord.SetSearchStartDay( m_nListStartDay );
		m_BookingRecord.SetSearchNights( m_nListNights );
		m_BookingRecord.SetSearchRoomSleeps( m_nListRoomSleeps );
		m_BookingRecord.SetSearchRoomSubType( m_nListRoomSubType );

		int nSel = m_comboRoomRate.GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < m_comboRoomRate.GetCount() ) )
			m_BookingRecord.SetRoomRateType( m_comboRoomRate.GetItemData( nSel ) );

		for ( int n = 0; n < m_nListBookedRooms; n++ )
		{
			CPMSBookingPartInfo infoPart;
			infoPart.SetRoomNo ( m_arrayRooms[n].m_nRoomNo );
			infoPart.SetStartDay ( m_arrayRooms[n].m_nStartDay );
			infoPart.SetNights ( m_arrayRooms[n].m_nNights );
			m_BookingRecord.AddPartInfo( infoPart );
		}

		EndDialog( IDOK );
	}
	else
	{
		int nLinesToKeep = m_nListBookedRooms;
		
		while ( m_arrayRooms.GetSize() > nLinesToKeep )
			m_arrayRooms.RemoveAt( m_arrayRooms.GetSize() - 1 );

		m_arrayRooms.InsertAt( m_nListBookedRooms++, infoList );
		m_arrayRooms.InsertAt( m_nListBookedRooms, infoList );

		m_nListUnbookedDay = infoList.m_nStartDay + infoList.m_nNights;

		UpdateRoomList();

		int nSize = m_arrayRooms.GetSize();

		m_listRooms.SetItemCountEx( nSize );
		m_listRooms.Invalidate();

		if ( nSize > 0 )
			m_listRooms.SetCurSel( m_nListBookedRooms + 1 );

		m_buttonBook.EnableWindow( m_arrayRooms.GetSize() >= m_nListBookedRooms + 2 );
	}
}

/**********************************************************************/

void CPMSBookSearchDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
