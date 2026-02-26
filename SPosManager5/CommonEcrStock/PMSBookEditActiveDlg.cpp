/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSCalendarTable.h"
/**********************************************************************/
#include "PMSBookEditActiveDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSBookEditActiveDlg, CDialog)
/**********************************************************************/

CPMSBookEditActiveDlg::CPMSBookEditActiveDlg( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord, CWnd* pParent )
	: CSSDialog(CPMSBookEditActiveDlg::IDD, pParent), m_BookingRecord( BookingRecord )
{
	DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, m_Room );

	m_nSearchCheckOutDay = BookingRecord.GetValidationCheckOutDay();
	m_nSearchGuests = BookingRecord.GetValidationGuests();

	m_nCheckInDay = BookingRecord.GetValidationCheckInDay();
	m_nMinCheckOutDay = BookingRecord.GetValidationMinCheckOutDay();
	m_nMaxCheckOutDay = BookingRecord.GetValidationMaxCheckOutDay();
}

/**********************************************************************/

CPMSBookEditActiveDlg::~CPMSBookEditActiveDlg()
{
}

/**********************************************************************/

void CPMSBookEditActiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_STATIC_MAXGUESTS, m_staticMaxGuests );
	DDX_Control( pDX, IDC_STATIC_MAXNIGHTS, m_staticMaxNights );

	DDX_Control(pDX, IDC_COMBO_CHECKIN, m_comboCheckIn);
	
	DDX_Control(pDX, IDC_COMBO_CHECKOUT, m_comboCheckOut);
	DDX_Control(pDX, IDC_DATEPICKER_CHECKOUT, m_datePickerCheckOut);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSBookEditActiveDlg, CDialog)

	ON_EN_KILLFOCUS(IDC_EDIT_NIGHTS,OnKillFocusNights)

	ON_CBN_SELCHANGE(IDC_COMBO_CHECKOUT, OnChangeDateCheckOut)
	ON_CBN_DROPDOWN(IDC_COMBO_CHECKOUT, OnOpenCheckOut)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_CHECKOUT, OnDatePickerCheckOut)

END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPMSBookEditActiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strRoomName;
	::GetPMSRoomText( m_Room.GetRoomNo(), m_Room.GetRoomName(), strRoomName );

	CString strTitle;
	strTitle.Format( "Edit Booking (%s, %s)", 
		(const char*) strRoomName,
		m_Room.GetRoomTypeName(FALSE) );

	SetWindowText( strTitle );

	m_datePickerCheckOut.SetFormat( "'calendar'" );

	SubclassEdit( IDC_EDIT_NIGHTS, SS_NUM, 3, "%d" );
	SubclassEdit( IDC_EDIT_GUESTS, SS_NUM, 1, "%d" );

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

	COleDateTime dateCheckOutMin, dateCheckOutMax;
	GetOleDateSince2011( m_nMinCheckOutDay, dateCheckOutMin );
	GetOleDateSince2011( m_nMaxCheckOutDay, dateCheckOutMax );
	
	m_datePickerCheckOut.SetRange( &dateCheckOutMin, &dateCheckOutMax );	
	UpdateDatePickerDates();

	int nMaxGuests = m_Room.GetSleeps();
	
	if ( nMaxGuests <= 1 )
		nMaxGuests = 1;

	bool bExtraBed = FALSE;
	bool bExtraCot = FALSE;
	switch ( m_BookingRecord.GetTariffType() )
	{
	case PMS_TARIFFTYPE_EXTRABED:
		bExtraBed = TRUE;
		break;

	case PMS_TARIFFTYPE_EXTRACOT:
		bExtraCot = TRUE;
		break;
	}

	if ( ( 1 == nMaxGuests ) && ( FALSE == bExtraBed ) )
	{
		GetEditGuests() -> SetWindowText( "1" );
		GetEditGuests() -> EnableWindow( FALSE );
		m_staticMaxGuests.SetWindowText( "" );
	}
	else
	{
		CString strMaxGuests;
		
		if ( TRUE == bExtraBed )
			strMaxGuests.Format( "( max %d + 1 extra bed )", nMaxGuests );
		else if ( TRUE == bExtraCot )
			strMaxGuests.Format( "( max %d + 1 extra cot )", nMaxGuests );
		else
			strMaxGuests.Format( "( max %d guests )", nMaxGuests );
		
		m_staticMaxGuests.SetWindowText( strMaxGuests );

		CString strGuests;
		strGuests.Format( "%d", m_nSearchGuests );
		GetEditGuests() -> SetWindowText( strGuests );
	}

	UpdateMaxNightsDisplay();

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSBookEditActiveDlg::GetEditNights(){ return GetEdit( IDC_EDIT_NIGHTS ); }
CEdit* CPMSBookEditActiveDlg::GetEditGuests(){ return GetEdit( IDC_EDIT_GUESTS ); }
/**********************************************************************/

void CPMSBookEditActiveDlg::UpdateMaxNightsDisplay()
{
	int nMaxNights = m_nMaxCheckOutDay - m_nCheckInDay;
		
	switch( nMaxNights )
	{
	case 0:		
		m_staticMaxNights.SetWindowText( "" );		
		break;

	case 1:		
		m_staticMaxNights.SetWindowText( "( Vacant 1 night )" );
		break;

	default:
		{
			if ( nMaxNights > PMSBooking::Nights.Max )
				nMaxNights = PMSBooking::Nights.Max;

			CString str;
			str.Format( "( Vacant %d nights )",
				nMaxNights );

			m_staticMaxNights.SetWindowText( str );
		}
		break;
	}
}

/**********************************************************************/

void CPMSBookEditActiveDlg::FillCheckInCombo()
{
	FillDateCombo( m_comboCheckIn, m_nCheckInDay, m_nCheckInDay, FALSE );
}

/**********************************************************************/

void CPMSBookEditActiveDlg::FillCheckOutCombo()
{
	FillDateCombo( m_comboCheckOut, m_nSearchCheckOutBase, m_nSearchCheckOutDay, TRUE );
}

/**********************************************************************/

void CPMSBookEditActiveDlg::FillDateCombo( CSSTabbedComboBox& combo, int& nBaseDay, int nSelDay, bool bCheckout )
{
	int nMinDay = ( bCheckout ) ? m_nMinCheckOutDay : m_nCheckInDay;
	int nMaxDay = ( bCheckout ) ? m_nMaxCheckOutDay : m_nCheckInDay;

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

void CPMSBookEditActiveDlg::OnOpenCheckOut()
{
	FillCheckOutCombo();
}

/**********************************************************************/

void CPMSBookEditActiveDlg::UpdateNights()
{
	int nNights = m_nSearchCheckOutDay - m_nCheckInDay;
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

void CPMSBookEditActiveDlg::UpdateCheckOutDay()
{
	CString strNights;
	GetEditNights() -> GetWindowText( strNights );
	int nNights = atoi( strNights );

	if ( ( nNights < 1 ) || ( nNights > PMSBooking::Nights.Max ) )
		nNights = 1;

	m_nSearchCheckOutDay = m_nCheckInDay + nNights;

	if ( m_nSearchCheckOutDay > m_nMaxCheckOutDay )
		m_nSearchCheckOutDay = m_nMaxCheckOutDay;
}

/**********************************************************************/

void CPMSBookEditActiveDlg::UpdateDatePickerDates()
{
	int nDay = m_nSearchCheckOutDay;
	
	if ( ( nDay < 1 ) || ( nDay > MAX_PMS_DAY ) )
		nDay = 1;

	COleDateTime date;
	GetOleDateSince2011( nDay, date );
	m_datePickerCheckOut.SetTime( date );
}

/**********************************************************************/

void CPMSBookEditActiveDlg::OnKillFocusNights()
{
	ValidateNights( FALSE );
}

/**********************************************************************/

bool CPMSBookEditActiveDlg::ValidateNights( bool bPrompt )
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
		int nMaxNights = m_nMaxCheckOutDay - m_nCheckInDay;
		int nMinNights = m_nMinCheckOutDay - m_nCheckInDay;

		if ( ( nNights >= nMinNights ) && ( nNights <= nMaxNights ) )
		{
			m_nSearchCheckOutDay = m_nCheckInDay + nNights;
			UpdateNights();
			FillCheckOutCombo();
			UpdateDatePickerDates();
			return TRUE;
		}
		else if ( nNights < nMinNights )
		{
			if ( TRUE == bPrompt )
			{
				COleDateTime dateCheckIn;
				GetOleDateSince2011( m_nCheckInDay + nMinNights - 1, dateCheckIn );

				CString strDate;
				strDate.Format( "%2.2d/%2.2d/%4.4d",
					dateCheckIn.GetDay(),
					dateCheckIn.GetMonth(),
					dateCheckIn.GetYear() );

				CString strMsg = "";
				strMsg += "The check out date for this booking must be after\n";
				strMsg += "the room entry date of ";
				strMsg += strDate;

				Prompter.Error( strMsg );
			}

			return FALSE;
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
				GetOleDateSince2011( m_nCheckInDay, dateCheckIn );

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

void CPMSBookEditActiveDlg::OnChangeDateCheckOut() 
{
	m_nSearchCheckOutDay = m_nSearchCheckOutBase + m_comboCheckOut.GetCurSel();
	
	UpdateNights();
	UpdateDatePickerDates();
}

/**********************************************************************/

void CPMSBookEditActiveDlg::OnDatePickerCheckOut(NMHDR* pNMHDR, LRESULT* pResult) 
{
	COleDateTime date;
	m_datePickerCheckOut.GetTime( date );

	int nDate = GetDayNumberSince2011( date );

	if ( ( nDate >= m_nMinCheckOutDay ) && ( nDate <= m_nMaxCheckOutDay ) )
	{
		m_nSearchCheckOutDay = nDate;

		UpdateNights();
		FillCheckOutCombo();
	}

	UpdateDatePickerDates();
	*pResult = 0;
}

/**********************************************************************/

void CPMSBookEditActiveDlg::OnOK()
{	
	if ( ValidateEntry() == TRUE )
		EndDialog( IDOK );
}

/**********************************************************************/

bool CPMSBookEditActiveDlg::ValidateEntry()
{
	CString strGuests;
	GetEditGuests() -> GetWindowText( strGuests );
	m_nSearchGuests = atoi( strGuests );

	int nMaxGuests = m_Room.GetSleeps();	
	switch( m_BookingRecord.GetTariffType() )
	{
	case PMS_TARIFFTYPE_EXTRABED:
	case PMS_TARIFFTYPE_EXTRACOT:
		nMaxGuests++;
		break;
	}

	if ( ( m_nSearchGuests < 1 ) || ( m_nSearchGuests > nMaxGuests ) )
	{
		CString strMsg;
		strMsg.Format( "Please specify the number of guests\nfor this booking (maximum %d)", 
			nMaxGuests );
		Prompter.Error( strMsg );
		
		GetEditGuests() -> SetFocus();
		GetEditGuests() -> SetSel( 0, -1 );
		return FALSE;
	}

	if ( m_nSearchCheckOutDay <= m_nCheckInDay )
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

	int nNights = m_nSearchCheckOutDay - m_nCheckInDay;

	m_BookingRecord.SetSearchNights( nNights );
	m_BookingRecord.SetGuests( m_nSearchGuests );

	if ( m_BookingRecord.GetPartCount() > 0 )
	{
		CPMSBookingPartInfo infoPart;
		m_BookingRecord.GetFinalPartInfo( infoPart );
		infoPart.SetNights ( m_nSearchCheckOutDay - infoPart.GetStartDay() );
		m_BookingRecord.SetFinalPartInfo( infoPart );
	}

	return TRUE;
}

/**********************************************************************/

void CPMSBookEditActiveDlg::OnCancel()
{
	EndDialog( IDCANCEL );
}

/**********************************************************************/
#endif
/**********************************************************************/
