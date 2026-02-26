/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "Password.h"
#include "PMSOptions.h"
#include "PMSRoomStatusTable.h"
/**********************************************************************/
#include "PMSBookRoomDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSBookRoomDlg, CDialog)
/**********************************************************************/

CPMSBookRoomDlg::CPMSBookRoomDlg( int nRoomIdx, CPMSBookingResponseInfo& Response, CPMSBookingCSVRecord& BookingRecord, CWnd* pParent )
	: CSSDialog(CPMSBookRoomDlg::IDD, pParent), m_Response ( Response ), m_BookingRecord( BookingRecord ) 
{
	m_nRoomIdx = nRoomIdx;
	DataManagerNonDb.PMSRoom.GetAt( m_nRoomIdx, m_Room );

	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nRoomIdx, infoRoom );
	m_bAllowCheckIn = ( ( infoRoom.GetBookingId() == 0 ) && ( PMSOptions.GetPMSDayNumber() == Response.m_nStartDay ) );

	m_nSearchCheckInDay = m_Response.m_nStartDay;
	m_nSearchCheckOutDay = m_nSearchCheckInDay + 1;
	m_nSearchGuests = 1;
		
	m_nSearchCheckInBase = m_nSearchCheckInDay;
	m_nSearchCheckOutBase = m_nSearchCheckOutDay;

	m_bCanEditRoomRate = PasswordArray.GetEnable( PasswordTicks::PMSChangeRoomRate );

	m_nAction = 0;

	m_TariffMap.BuildMap( m_Room );
}

/**********************************************************************/

CPMSBookRoomDlg::~CPMSBookRoomDlg()
{
}

/**********************************************************************/

void CPMSBookRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_STATIC_MAXNIGHTS, m_staticMaxNights );
	DDX_Control( pDX, IDC_STATIC_MAXGUESTS, m_staticMaxGuests );

	DDX_Control(pDX, IDC_COMBO_CHECKIN, m_comboCheckIn);
	DDX_Control(pDX, IDC_DATEPICKER_CHECKIN, m_datePickerCheckIn);

	DDX_Control(pDX, IDC_COMBO_CHECKOUT, m_comboCheckOut);
	DDX_Control(pDX, IDC_DATEPICKER_CHECKOUT, m_datePickerCheckOut);

	DDX_Control(pDX, IDC_COMBO_TARIFF, m_comboTariff);
	DDX_Control(pDX, IDC_COMBO_RATE, m_comboRate);
	
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSBookRoomDlg, CDialog)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKIN, OnChangeDateCheckIn)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKIN, OnOpenCheckIn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_CHECKIN, OnDatePickerCheckIn)
	
	ON_EN_KILLFOCUS(IDC_EDIT_NIGHTS,OnKillFocusNights)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKOUT, OnChangeDateCheckOut)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKOUT, OnOpenCheckOut)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_CHECKOUT, OnDatePickerCheckOut)

	ON_CBN_SELCHANGE(IDC_COMBO_TARIFF, OnSelectTariff)
	ON_CBN_SELCHANGE(IDC_COMBO_RATE, OnSelectRate)

	ON_BN_CLICKED( IDC_BUTTON_ACTION1, OnButtonAction1 )
	ON_BN_CLICKED( IDC_BUTTON_ACTION2, OnButtonAction2 )

END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSBookRoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strRoomName;
	::GetPMSRoomText( m_Room.GetRoomNo(), m_Room.GetRoomName(), strRoomName );

	CString strTitle;
	strTitle.Format( "Book Room (%s, %s)", 
		(const char*) strRoomName,
		m_Room.GetRoomTypeName(FALSE) );

	SetWindowText( strTitle );

	m_datePickerCheckIn.SetFormat( "'calendar'" );
	m_datePickerCheckOut.SetFormat( "'calendar'" );

	SubclassEdit( IDC_EDIT_NIGHTS, SS_NUM, 3, "%d" );
	SubclassEdit( IDC_EDIT_GUESTS, SS_NUM, 1, "%s" );
	
	if ( DealerFlags.GetPMSExtendedRoomRateFlag() == TRUE )
	{
		SubclassEdit( IDC_EDIT_RATE, SS_NUM_DP, 8, "%.2f" );
	}
	else
	{
		SubclassEdit( IDC_EDIT_RATE, SS_NUM_DP, 6, "%.2f" );
	}

	SubclassEdit( IDC_EDIT_BED, SS_NUM_DP, 6, "%.2f" );

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(35);
	arrayStops.Add(55);
	arrayStops.Add(80);
	m_comboCheckIn.SetTabStops( arrayStops );
	m_comboCheckOut.SetTabStops( arrayStops );

	FillCheckInCombo();
	UpdateNights();
	UpdateCheckOutDay();
	FillCheckOutCombo();

	COleDateTime dateCheckInMin, dateCheckInMax, dateCheckOutMin, dateCheckOutMax;
	GetOleDateSince2011( m_Response.m_nMinStartDay, dateCheckInMin );
	GetOleDateSince2011( m_Response.m_nMaxStartDay, dateCheckInMax );
	GetOleDateSince2011( m_Response.m_nMinStartDay + 1, dateCheckOutMin );
	GetOleDateSince2011( m_Response.m_nMaxStartDay + 1, dateCheckOutMax );
	
	m_datePickerCheckIn.SetRange( &dateCheckInMin, &dateCheckInMax );
	m_datePickerCheckOut.SetRange( &dateCheckOutMin, &dateCheckOutMax );	
	UpdateDatePickerDates();

	if ( m_bAllowCheckIn == FALSE )
	{
		m_buttonAction1.ShowWindow( SW_HIDE );
		m_buttonAction1.EnableWindow( FALSE );
	}

	UpdateMaxNightsDisplay();

	m_nRoomRateType = PMSOptions.GetMiscDefaultRoomRateType();

	switch( m_nRoomRateType )
	{
	case PMS_BOOKING_RATE_ROOM:
	case PMS_BOOKING_RATE_BB:
	case PMS_BOOKING_RATE_DBB:
	case PMS_BOOKING_RATE_FREE:
		break;

	default:
		m_nRoomRateType = PMS_BOOKING_RATE_ROOM;
		break;
	}

	//FILL TARIFF COMBO
	CArray<int,int> arrayTariffs;
	m_TariffMap.GetTariffTypeList( arrayTariffs );

	arrayStops.RemoveAll();
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
				strText.Format( "Extra Adult Bed,(+%.2f)", m_Room.GetRoomExtraBedRate() );
				m_comboTariff.AddItem( strText );
			}
			break;

		case PMS_TARIFFTYPE_EXTRACOT:
			{
				CString strText;
				strText.Format( "Extra Child Bed,(+%.2f)", m_Room.GetRoomExtraCotRate() );
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
	m_nTariffType = m_comboTariff.GetItemData(nSel);
	m_comboTariff.EnableWindow( m_comboTariff.GetCount() > 1 );

	//MAX GUESTS
	{
		int nMaxGuests = m_Room.GetSleeps();

		if ( nMaxGuests <= 1 )
			nMaxGuests = 1;

		bool bExtraBed = m_Room.GetRoomAllowExtraBedFlag();
		bool bExtraCot = m_Room.GetRoomAllowExtraCotFlag();
		
		if ( ( 1 == nMaxGuests ) && ( FALSE == ( bExtraBed | bExtraCot ) ) )
		{
			m_staticMaxGuests.SetWindowText( "" );
			GetEditGuests() -> SetWindowText( "1" );
			GetEditGuests() -> EnableWindow( FALSE );
		}
		else
		{
			CString strMaxGuests;
			
			if ( FALSE == ( bExtraBed | bExtraCot ) )
				strMaxGuests.Format( "( max %d guests )", nMaxGuests );
			else if ( FALSE == bExtraCot )
				strMaxGuests.Format( "( max %d + 1 extra bed )", nMaxGuests );
			else if ( FALSE == bExtraBed )
				strMaxGuests.Format( "( max %d + 1 extra cot )", nMaxGuests );
			else
				strMaxGuests.Format( "( max %d + 1 extra bed or cot )", nMaxGuests );

			m_staticMaxGuests.SetWindowText( strMaxGuests );
			GetEditGuests() -> SetWindowText( "" );
			GetEditGuests() -> EnableWindow( TRUE );
		}
	}

	//FILL ROOM RATE COMBO
	CArray<int,int> arrayRates;
	m_TariffMap.GetRoomRateTypeList( m_nTariffType, arrayRates );

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
		if ( m_comboRate.GetItemData(n) == m_nRoomRateType )
		{
			m_comboRate.SetCurSel(n);
			break;
		}
	}

	nSel = m_comboRate.GetCurSel();
	m_nRoomRateType = m_comboRate.GetItemData(nSel);
	m_comboRate.EnableWindow( m_comboRate.GetCount() > 1 );
	m_comboRate.SetCurSel( nSel );

	{
		bool bEditRate = ( PMS_BOOKING_RATE_FREE != m_nRoomRateType ) && ( TRUE == m_bCanEditRoomRate );
		
		bool bExtraBed = FALSE;
		switch( m_nTariffType )
		{
		case PMS_TARIFFTYPE_EXTRABED:
		case PMS_TARIFFTYPE_EXTRACOT:
			bExtraBed = TRUE;
			break;
		}

		GetEditRate() -> SetReadOnly( FALSE == bEditRate );
		GetEditBed() -> SetReadOnly( ( FALSE == bEditRate ) || ( FALSE == bExtraBed ) );
	}

	m_BookingRecord.SetTariffType( m_nTariffType );
	m_BookingRecord.SetRoomRateType( m_nRoomRateType );

	double dRoomRate = 0.0;
	switch( m_nRoomRateType )
	{
	case PMS_BOOKING_RATE_ROOM:	dRoomRate = m_Room.GetRoomOnlyRate();	break;
	case PMS_BOOKING_RATE_BB:	dRoomRate = m_Room.GetRoomBBRate();		break;
	case PMS_BOOKING_RATE_DBB:	dRoomRate = m_Room.GetRoomDBBRate();	break;
	case PMS_BOOKING_RATE_FREE:	dRoomRate = 0.0;						break;
	}
	m_BookingRecord.SetRoomRate( dRoomRate );

	CString str;
	str.Format( "%.2f", dRoomRate );
	GetEditRate() -> SetWindowText( str );

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSBookRoomDlg::GetEditNights(){ return GetEdit( IDC_EDIT_NIGHTS ); }
CEdit* CPMSBookRoomDlg::GetEditGuests(){ return GetEdit( IDC_EDIT_GUESTS ); }
CEdit* CPMSBookRoomDlg::GetEditRate(){ return GetEdit( IDC_EDIT_RATE ); }
CEdit* CPMSBookRoomDlg::GetEditBed(){ return GetEdit( IDC_EDIT_BED ); }
/**********************************************************************/

void CPMSBookRoomDlg::OnSelectTariff()
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

	m_nTariffType = nNewTariff;
	m_BookingRecord.SetTariffType( m_nTariffType );

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
	m_nRoomRateType = m_BookingRecord.GetRoomRateType();

	UpdateRoomRate();
}

/**********************************************************************/

void CPMSBookRoomDlg::OnSelectRate()
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

void CPMSBookRoomDlg::UpdateRoomRate()
{
	CPMSRoomStatusInfo infoRoom;
	PMSRoomStatusTable.GetRoomInfo( m_nRoomIdx, infoRoom );

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

	m_BookingRecord.SetRoomRateType( m_nRoomRateType );
	m_BookingRecord.SetRoomRate( dNewRoomRate );
	m_BookingRecord.SetExtraBedRate( dNewExtraBedRate );
}

/**********************************************************************/

void CPMSBookRoomDlg::UpdateMaxNightsDisplay()
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	int nMaxNights = ( m_Response.m_nMaxStartDay + 1 ) - m_nSearchCheckInDay;

	switch( nMaxNights )
	{
	case 0:		
		m_staticMaxNights.SetWindowText( "" );		
		break;

	case 1:		
		m_staticMaxNights.SetWindowText( "( vacant 1 night )" );
		break;

	default:
		{
			if ( nMaxNights > PMSBooking::Nights.Max )
				nMaxNights = PMSBooking::Nights.Max;

			CString str;
			str.Format( "( vacant %d nights )",
				nMaxNights );

			m_staticMaxNights.SetWindowText( str );
		}
		break;
	}
}

/**********************************************************************/

void CPMSBookRoomDlg::FillCheckInCombo()
{
	FillDateCombo( m_comboCheckIn, m_nSearchCheckInBase, m_nSearchCheckInDay, FALSE );
}

/**********************************************************************/

void CPMSBookRoomDlg::FillCheckOutCombo()
{
	FillDateCombo( m_comboCheckOut, m_nSearchCheckOutBase, m_nSearchCheckOutDay, TRUE );
}

/**********************************************************************/

void CPMSBookRoomDlg::FillDateCombo( CSSTabbedComboBox& combo, int& nBaseDay, int nSelDay, bool bCheckout )
{
	int nMinDay = m_Response.m_nMinStartDay;
	int nMaxDay = m_Response.m_nMaxStartDay;

	if ( TRUE == bCheckout )
	{
		nMinDay++;
		nMaxDay++;
	}

	nBaseDay = nSelDay - 7;
	
	if ( nBaseDay < nMinDay )
		nBaseDay = nMinDay;

	int nEndDay = nSelDay + 7;
	if ( nEndDay > nMaxDay )
		nEndDay = nMaxDay;

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

void CPMSBookRoomDlg::OnOpenCheckIn()
{
	FillCheckInCombo();
}

/**********************************************************************/

void CPMSBookRoomDlg::OnOpenCheckOut()
{
	FillCheckOutCombo();
}

/**********************************************************************/

void CPMSBookRoomDlg::UpdateNights()
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

void CPMSBookRoomDlg::UpdateCheckOutDay()
{
	CString strNights;
	GetEditNights() -> GetWindowText( strNights );
	int nNights = atoi( strNights );

	if ( ( nNights < 1 ) || ( nNights > PMSBooking::Nights.Max ) )
		nNights = 1;

	m_nSearchCheckOutDay = m_nSearchCheckInDay + nNights;

	if ( m_nSearchCheckOutDay > m_Response.m_nMaxStartDay + 1 )
		m_nSearchCheckOutDay = m_Response.m_nMaxStartDay + 1;
}

/**********************************************************************/

void CPMSBookRoomDlg::UpdateDatePickerDates()
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

void CPMSBookRoomDlg::OnChangeDateCheckIn() 
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	
	UpdateCheckOutDay();	
	UpdateNights();
	FillCheckOutCombo();
	UpdateDatePickerDates();
	UpdateMaxNightsDisplay();
}

/**********************************************************************/

void CPMSBookRoomDlg::OnDatePickerCheckIn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePickerCheckIn.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= m_Response.m_nMinStartDay ) && ( nDate <= m_Response.m_nMaxStartDay ) )
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

void CPMSBookRoomDlg::OnKillFocusNights()
{
	ValidateNights( FALSE );
}

/**********************************************************************/

bool CPMSBookRoomDlg::ValidateNights( bool bPrompt )
{
	CString str;
	GetEditNights() -> GetWindowText( str );
	int nNights = atoi( str );

	if ( ( nNights < 1 ) || ( nNights > PMSBooking::Nights.Max ) )
	{
		if ( TRUE == bPrompt )
		{
			CString strMsg;
			strMsg.Format ( "Please specify the required length of stay\nbetween 1 and %d nights.", PMSBooking::Nights.Max );
			Prompter.Error( strMsg );
		}

		return FALSE;
	}

	{
		m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
		int nMaxNights = ( m_Response.m_nMaxStartDay + 1 ) - m_nSearchCheckInDay;

		if ( nNights <= nMaxNights )
		{
			m_nSearchCheckOutDay = m_nSearchCheckInDay + nNights;
			UpdateNights();
			FillCheckOutCombo();
			UpdateDatePickerDates();
			return TRUE;
		}
		else
		{
			if ( TRUE == bPrompt )
			{
				CString strMaxNights;
				if ( 1 == nMaxNights )
					strMaxNights = "1 night";
				else
					strMaxNights.Format( "%d nights", nMaxNights );

				COleDateTime dateCheckIn;
				GetOleDateSince2011( m_nSearchCheckInDay, dateCheckIn );

				CString strDate;
				strDate.Format( "%2.2d/%2.2d/%4.4d",
					dateCheckIn.GetDay(),
					dateCheckIn.GetMonth(),
					dateCheckIn.GetYear() );

				CString strError = "";
				strError += "This room is only available for a maximum of ";
				strError += strMaxNights + "\n";
				strError += "from the check in date of ";
				strError += strDate;
				Prompter.Error( strError );
			}

			return FALSE;
		}
	}

	return FALSE;
}

/**********************************************************************/

void CPMSBookRoomDlg::OnChangeDateCheckOut() 
{
	m_nSearchCheckInDay = m_nSearchCheckInBase + m_comboCheckIn.GetCurSel();
	m_nSearchCheckOutDay = m_nSearchCheckOutBase + m_comboCheckOut.GetCurSel();
	
	UpdateNights();
	UpdateDatePickerDates();
}

/**********************************************************************/

void CPMSBookRoomDlg::OnDatePickerCheckOut(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePickerCheckOut.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= m_Response.m_nMinStartDay + 1 ) && ( nDate <= m_Response.m_nMaxStartDay + 1 ) )
	{
		m_nSearchCheckOutDay = nDate;

		UpdateNights();
		FillCheckOutCombo();
	}

	UpdateDatePickerDates();
	*pResult = 0;
}

/**********************************************************************/

void CPMSBookRoomDlg::OnButtonAction1()
{
	if ( ValidateEntry ( TRUE ) == TRUE )
	{
		m_nAction = 1;
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPMSBookRoomDlg::OnButtonAction2()
{
	if ( ValidateEntry ( FALSE ) == TRUE )
	{
		m_nAction = 2;
		EndDialog( IDOK );
	}
}

/**********************************************************************/

void CPMSBookRoomDlg::OnOK()
{
	OnButtonAction2();
}

/**********************************************************************/

void CPMSBookRoomDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/

bool CPMSBookRoomDlg::ValidateEntry( bool bCheckIn )
{	
	CString strGuests;
	GetEditGuests() -> GetWindowText( strGuests );
	m_nSearchGuests = atoi( strGuests );

	int nMaxGuests = m_Room.GetSleeps();
	
	bool bCanHaveExtraBed = FALSE;
	bool bGotExtraBed = FALSE;
	if ( m_Room.GetRoomAllowExtraBedFlag() || m_Room.GetRoomAllowExtraCotFlag() )
	{
		bCanHaveExtraBed = TRUE;

		switch( m_nTariffType )
		{
		case PMS_TARIFFTYPE_EXTRABED:
		case PMS_TARIFFTYPE_EXTRACOT:
			bGotExtraBed = TRUE;
			nMaxGuests++;
			break;
		}
	}

	if ( ( m_nSearchGuests == nMaxGuests + 1 ) && ( TRUE == bCanHaveExtraBed ) && ( FALSE == bGotExtraBed ) )
	{
		CString strMsg;
		strMsg.Format( "You must specify an extra bed tariff to\naccommodate %d guests in this room.", 
			m_nSearchGuests );
		Prompter.Error( strMsg );

		GetEditGuests() -> SetFocus();
		GetEditGuests() -> SetSel( 0, -1 );
		return FALSE;
	}

	if ( ( m_nSearchGuests < 1 ) || ( m_nSearchGuests > nMaxGuests ) )
	{
		CString strExtra = ( bCanHaveExtraBed ) ? " + 1" : "";

		CString strMsg;
		strMsg.Format( "Please specify the number of guests\nfor this booking (maximum %d%s)", 
			( bGotExtraBed ) ? nMaxGuests - 1 : nMaxGuests,
			(const char*) strExtra );
		Prompter.Error( strMsg );
		
		GetEditGuests() -> SetFocus();
		GetEditGuests() -> SetSel( 0, -1 );
		return FALSE;
	}

	if ( m_nSearchCheckOutDay <= m_nSearchCheckInDay )
	{
		Prompter.Error( "The check out date for this booking\nmust be after the check in date" );
		return FALSE;
	}

	if ( ValidateNights( TRUE ) == FALSE )
	{
		GetEditNights() -> SetFocus();
		GetEditNights() -> SetSel( 0, -1 );
		return FALSE;
	}

	if ( TRUE == bCheckIn )
	{
		if ( m_nSearchCheckInDay != PMSOptions.GetPMSDayNumber() )
		{
			Prompter.Error( "The quick check in option can only be used\nfor bookings that start today." );
			return FALSE;
		}
	}

	double dRoomRate = 0.0;
	{
		double dMinRate = PMSRoom::RoomRate.Min;
		double dMaxRate = PMSRoom::RoomRate.Max;
		if ( DealerFlags.GetPMSExtendedRoomRateFlag() == TRUE )
		{
			dMinRate = PMSRoom::ExtendedRoomRate.Min;
			dMaxRate = PMSRoom::ExtendedRoomRate.Max;
		}

		CString strRate;
		GetEditRate() -> GetWindowText( strRate );
		dRoomRate = atof( strRate );

		if ( ( dRoomRate < dMinRate ) || ( dRoomRate > dMaxRate ) )
		{
			CString strError;
			strError.Format( "Please set a room rate before %.2f and %.2f", dMinRate, dMaxRate );
			AfxMessageBox( strError );
			GetEditRate() -> SetSel(0,-1);
			GetEditRate() -> SetFocus();
			return FALSE;
		}
	}

	if ( m_Room.GetSleeps() <= 1 )
		m_nSearchGuests = 1;

	m_BookingRecord.ClearRecord( TRUE );
	m_BookingRecord.SetGuests( m_nSearchGuests );
	m_BookingRecord.SetSearchStartDay( m_nSearchCheckInDay );
	m_BookingRecord.SetSearchNights( m_nSearchCheckOutDay - m_nSearchCheckInDay );
	m_BookingRecord.SetSearchRoomSleeps(0);
	m_BookingRecord.SetSearchRoomSubType(0);
	m_BookingRecord.SetTariffType( m_nTariffType );
	m_BookingRecord.SetRoomRateType( m_nRoomRateType );

	{
		CString strRate;
		GetEditRate() -> GetWindowText( strRate );
		m_BookingRecord.SetRoomRate( atof( strRate ) );
	}

	{
		CString strRate;
		GetEditBed() -> GetWindowText( strRate );
		m_BookingRecord.SetExtraBedRate( atof( strRate ) );
	}

	CPMSBookingPartInfo infoPart;
	infoPart.SetRoomNo ( m_Room.GetRoomNo() );
	infoPart.SetStartDay ( m_nSearchCheckInDay );
	infoPart.SetNights ( m_nSearchCheckOutDay - m_nSearchCheckInDay );
	m_BookingRecord.AddPartInfo( infoPart );

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
