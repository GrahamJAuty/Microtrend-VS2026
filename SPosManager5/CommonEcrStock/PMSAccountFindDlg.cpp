/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "DateRangeHelpers.h"
#include "Password.h"
#include "PastAndFutureDateRange.h"
#include "PMSAccountFindReportDlg.h"
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
#include "PMSZeroNightStayArray.h"
/**********************************************************************/
#include "PMSAccountFindDlg.h"
/**********************************************************************/

CPMSAccountFindDlg::CPMSAccountFindDlg( int nType, CWnd* pParent ) : 
	CSSDialog(CPMSAccountFindDlg::IDD, pParent),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo )
{
	//{{AFX_DATA_INIT(CPMSAccountFindDlg)
	//}}AFX_DATA_INIT
	m_nType = nType;
	m_strName = "";
	m_strRoomNo = "";
	m_strReference = "";
	m_nSearchStartDay = 0;
	m_nSearchEndDay = 0;
	m_nSearchRoomNo = 0;
	m_nSearchReference = 0;
	m_nSearchInvoiceNum = 0;
	m_nLastEditedBookingId = 0;
	m_pCalendarDlg = NULL;
}

/**********************************************************************/

void CPMSAccountFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPMSAccountFindDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_BUTTON_ACTION1, m_buttonAction1);
	DDX_Control(pDX, IDC_BUTTON_ACTION2, m_buttonAction2);
	DDX_Control(pDX, IDC_BUTTON_ACTION3, m_buttonAction3);
	DDX_Control(pDX, IDC_CHECK_TRAINING, m_checkTraining);
	DDX_Control(pDX, IDC_STATIC_INVOICE, m_staticInvoiceNum);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, PMSCustomer::GuestName.Max);
	DDX_Text(pDX, IDC_EDIT_ROOM, m_strRoomNo);
	DDX_Text(pDX, IDC_EDIT_REFERENCE, m_strReference);
	DDX_Text(pDX, IDC_EDIT_INVOICE, m_strInvoiceNum);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSAccountFindDlg, CDialog)
	//{{AFX_MSG_MAP(CPMSAccountFindDlg)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE,OnButtonUpdate)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_CHECK_TRAINING, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_ACTION1, OnButtonAction1)
	ON_BN_CLICKED(IDC_BUTTON_ACTION2 ,OnButtonAction2)
	ON_BN_CLICKED(IDC_BUTTON_ACTION3, OnButtonAction3)
	ON_EN_KILLFOCUS(IDC_EDIT_REFERENCE, OnKillFocusReference)
	ON_EN_KILLFOCUS(IDC_EDIT_INVOICE, OnKillFocusInvoiceNum)
END_MESSAGE_MAP()

/**********************************************************************/

int CPMSAccountFindDlg::GetArraySize()
{
	return m_arrayAccountFindInfo.GetSize();
}

/**********************************************************************/

BOOL CPMSAccountFindDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Booking Ref", LVCFMT_LEFT, 75 );
	m_listData.InsertColumn ( 1, "Check In", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn ( 2, "Nights", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 3, "Check Out", LVCFMT_LEFT, 85 );
	m_listData.InsertColumn ( 4, "No", LVCFMT_LEFT, 50 );
	m_listData.InsertColumn ( 5, "Room", LVCFMT_LEFT, 130 );
	m_listData.InsertColumn ( 6, "Booking Name", LVCFMT_LEFT, 130 );
	
	m_listData.SetItemCountEx ( GetArraySize() );
	m_listData.SetCurSel(0);

	SubclassEdit( IDC_EDIT_ROOM, SS_NUM, 4, "%s" );
	SubclassEdit( IDC_EDIT_REFERENCE, SS_NUM, 8, "%s" );
	SubclassEdit( IDC_EDIT_INVOICE, SS_NUM, 8, "%s" );

	m_DateSelectorRange.SetCalendarMinYear( 2010 );
	m_DateSelectorRange.SetCalendarMaxYear( 2050 );
	m_DateSelectorRange.InitialiseDateControls();

	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:
		m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_FUTURE );
		m_DateSelectorRange.AddFutureReportRanges();
		m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );
		GetEditInvoiceNum() -> ShowWindow( SW_HIDE );
		GetEditInvoiceNum() -> EnableWindow( FALSE );
		m_staticInvoiceNum.ShowWindow( SW_HIDE );
		break;

	case IDM_PMSMENUBAR_CANCELLED_BOOKING:
		m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_FUTURE );
		m_DateSelectorRange.AddFutureReportRanges();
		m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );
		GetEditInvoiceNum() -> ShowWindow( SW_HIDE );
		GetEditInvoiceNum() -> EnableWindow( FALSE );
		m_staticInvoiceNum.ShowWindow( SW_HIDE );
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
		m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_PAST );
		m_DateSelectorRange.AddPastReportRanges();
		m_DateSelectorRange.SetComboSelection( 0, szRANGE_ALL );
		break;
	
	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:	
	default:
		m_DateSelectorRange.SetMode( DATE_SELECTOR_MODE_PAST );
		m_DateSelectorRange.AddPastReportRanges();
		m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );
		break;
	}
	
	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:	
		SetWindowText( "Find Booking" );	
		break;

	case IDM_PMSMENUBAR_CANCELLED_BOOKING:	
		SetWindowText( "Find Cancelled Booking" );	
		break;
	
	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
		SetWindowText( "Process Unpaid Accounts" );
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
		SetWindowText( "Process Unpaid Accounts (CR)" );
		break;

	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:	
	default:							
		SetWindowText( "Process Guest Accounts" );	
		break;
	}

	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:
		m_buttonAction1.SetWindowText( "Edit\nBooking" );
		m_buttonAction2.SetWindowText( "Delete\nBooking" );
		m_buttonAction3.EnableWindow( FALSE );
		m_buttonAction3.ShowWindow( SW_HIDE );
		break;

	case IDM_PMSMENUBAR_CANCELLED_BOOKING:
		m_buttonAction1.SetWindowText( "Edit\nBooking" );
		m_buttonAction2.EnableWindow( FALSE );
		m_buttonAction2.ShowWindow( SW_HIDE );
		m_buttonAction3.EnableWindow( FALSE );
		m_buttonAction3.ShowWindow( SW_HIDE );
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:
	default:
		if ( PasswordArray.GetEnable( PasswordTicks::PMSManualSales ) == FALSE )
		{
			m_buttonAction1.EnableWindow( FALSE );
			m_buttonAction1.ShowWindow( SW_HIDE );
		}

		if ( PasswordArray.GetEnable( PasswordTicks::PMSPayments ) == FALSE )
		{
			m_buttonAction2.EnableWindow( FALSE );
			m_buttonAction2.ShowWindow( SW_HIDE );
		}

		if ( PasswordArray.GetEnable( PasswordTicks::PMSInvoices ) == FALSE )
		{
			m_buttonAction3.EnableWindow( FALSE );
			m_buttonAction3.ShowWindow( SW_HIDE );
		}

		m_buttonAction1.SetWindowText( "Edit\nSales" );
		m_buttonAction2.SetWindowText( "Edit\nPayment" );
		m_buttonAction3.SetWindowText( "Print\nInvoice" );
		break;
	}

	m_checkTraining.SetCheck( FALSE );

	if ( PMSOptions.GetMiscEnableTrainingMode() == FALSE )
	{
		m_checkTraining.EnableWindow( FALSE );
		m_checkTraining.ShowWindow( SW_HIDE );
	}
	
	OnButtonUpdate();

	return TRUE;  
}

/**********************************************************************/
CEdit* CPMSAccountFindDlg::GetEditReference(){ return GetEdit( IDC_EDIT_REFERENCE ); }
CEdit* CPMSAccountFindDlg::GetEditInvoiceNum(){ return GetEdit( IDC_EDIT_INVOICE ); }
/**********************************************************************/

bool CPMSAccountFindDlg::FindBookingPart( int nCheckInDay, int nBookingId, int nBookingPart, int& nPos )
{
	int nStartIdx = 0;
	int nEndIdx = m_arrayAccountFindInfo.GetSize() - 1;

	while( nStartIdx <= nEndIdx )
	{
		nPos = ( nStartIdx + nEndIdx ) / 2;

		int nArrayCheckInDay = m_arrayAccountFindInfo[ nPos ].m_nCheckInDay;
		int nArrayId = m_arrayAccountFindInfo[ nPos ].m_nBookingId;
		int nArrayPart = m_arrayAccountFindInfo[ nPos ].m_nBookingPart;

		if ( nCheckInDay < nArrayCheckInDay )
			nEndIdx = nPos - 1;
		else if ( nCheckInDay > nArrayCheckInDay )
			nStartIdx = nPos + 1;
		else if ( nBookingId < nArrayId )
			nEndIdx = nPos - 1;
		else if ( nBookingId > nArrayId )
			nStartIdx = nPos + 1;
		else if ( nBookingPart < nArrayPart )
			nEndIdx = nPos - 1;
		else if ( nBookingPart > nArrayPart )
			nStartIdx = nPos + 1;
		else
			return TRUE;
	}

	nPos = nStartIdx;
	return FALSE;
}

/**********************************************************************/

void CPMSAccountFindDlg::SelectLine ( int nIndex )
{
	int nSize = m_listData.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listData.SetCurSel ( nIndex );
	else 
		m_listData.SetCurSel ( nSize - 1 );
	
	m_listData.Invalidate();
}

/**********************************************************************/

void CPMSAccountFindDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
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

bool CPMSAccountFindDlg::PrepareLine( int nIndex )
{
	if ( nIndex < 0 || nIndex >= GetArraySize() )
		return FALSE;

	CPMSAccountFindInfo infoAccountFind = m_arrayAccountFindInfo.GetAt( nIndex );

	COleDateTime dateIn;
	GetOleDateSince2011( infoAccountFind.m_nBookingPartStartDay, dateIn );

	CString strDateIn;
	strDateIn.Format( "%2.2d/%2.2d/%4.4d",
		dateIn.GetDay(),
		dateIn.GetMonth(),
		dateIn.GetYear() );

	COleDateTime dateOut;
	int nDateOut = infoAccountFind.m_nBookingPartStartDay + infoAccountFind.m_nBookingPartNights;
	GetOleDateSince2011( nDateOut, dateOut );

	CString strDateOut;
	strDateOut.Format( "%2.2d/%2.2d/%4.4d",
		dateOut.GetDay(),
		dateOut.GetMonth(),
		dateOut.GetYear() );

	CString strBookingRef;
	strBookingRef.Format( "%8.8d", infoAccountFind.m_nBookingId );
	
	CCSV csv( '\t' );
	csv.Add( strBookingRef );
	csv.Add( strDateIn );
	csv.Add( infoAccountFind.m_nBookingPartNights );
	csv.Add( strDateOut );
	csv.Add( infoAccountFind.m_nRoomNo );
	csv.Add( infoAccountFind.m_strRoomName );
	csv.Add( infoAccountFind.m_strGuestName );

	m_strDisplayLine = csv.GetLine();
	
	return TRUE;
}

/**********************************************************************/

int CPMSAccountFindDlg::GetSafeSelection()
{
	return GetSafeSelection ( 0, GetArraySize() - 1 );
}

/**********************************************************************/

int CPMSAccountFindDlg::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listData.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CPMSAccountFindDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	int nSel = GetSafeSelection();

	if ( nSel != -1 )
	{
		switch( m_nType )
		{
		case IDM_PMSMENUBAR_FIND_BOOKING:
		case IDM_PMSMENUBAR_CANCELLED_BOOKING:
			ProcessBooking();	
			break;
	
		case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
		case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
		case IDM_PMSMENUBAR_GUEST_ACCOUNTS:	
		default:
			if ( PasswordArray.GetEnable( PasswordTicks::PMSInvoices ) == TRUE )
				ProcessInvoice();
			break;
		}
	}

	*pResult = 0;
}

/**********************************************************************/

void CPMSAccountFindDlg::OnKillFocusReference()
{
	UpdateData( TRUE, FALSE );
	int nRef = atoi( m_strReference );
	
	if ( nRef == 0 )
		GetEditReference() -> SetWindowText( "" );
	else
	{
		CString strRef;
		strRef.Format( "%8.8d", nRef );
		GetEditReference() -> SetWindowText( strRef );
	}	
}

/**********************************************************************/

void CPMSAccountFindDlg::OnKillFocusInvoiceNum()
{
	UpdateData( TRUE, FALSE );
	int nNum = atoi( m_strInvoiceNum );
	
	if ( nNum == 0 )
		GetEditInvoiceNum() -> SetWindowText( "" );
	else
	{
		CString strNum;
		strNum.Format( "%8.8d", nNum );
		GetEditInvoiceNum() -> SetWindowText( strNum );
	}	
}

/**********************************************************************/

void CPMSAccountFindDlg::OnSelectDate() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_DateSelectorRange.UpdateDateControls();
		UpdateData ( FALSE );
		UpdateListAfterDateChange();
	}
}

/**********************************************************************/

void CPMSAccountFindDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	UpdateListAfterDateChange();
	*pResult = 0;
}

/**********************************************************************/

void CPMSAccountFindDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	UpdateListAfterDateChange();
	*pResult = 0;
}

/**********************************************************************/

void CPMSAccountFindDlg::UpdateListAfterDateChange()
{
	int nSelectCheckInDay = 0;
	int nSelectBookingId = 0;
	int nSelectBookingPart = 0;

	int nSel = m_listData.GetCurSel();	
	if ( ( nSel >= 0 ) && ( nSel < m_arrayAccountFindInfo.GetSize() ) )
	{
		nSelectCheckInDay = m_arrayAccountFindInfo[nSel].m_nCheckInDay;
		nSelectBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
		nSelectBookingPart = m_arrayAccountFindInfo[nSel].m_nBookingPart;
	}

	int nStartDayOld = m_nSearchStartDay;
	int nEndDayOld = m_nSearchEndDay;
	UpdateSearchDates();

	if ( ( nStartDayOld != m_nSearchStartDay ) || ( nEndDayOld != m_nSearchEndDay ) )
		UpdateList( nSelectCheckInDay, nSelectBookingId, nSelectBookingPart );
}

/**********************************************************************/

void CPMSAccountFindDlg::OnButtonUpdate()
{
	if ( UpdateData( TRUE ) == FALSE )
		return;

	UpdateSearchDates();
	m_nSearchRoomNo = atoi ( m_strRoomNo );
	m_nSearchReference = atoi ( m_strReference );
	m_nSearchInvoiceNum = atoi( m_strInvoiceNum );

	UpdateList(0,0,0);
}

/**********************************************************************/

void CPMSAccountFindDlg::UpdateSearchDates()
{
	COleDateTime dateFrom, dateTo;
	m_DatePickerFrom.GetTime( dateFrom );
	m_DatePickerTo.GetTime( dateTo );

	if ( dateFrom.m_status != COleDateTime::valid )
		return;

	if ( dateTo.m_status != COleDateTime::valid )
		return;

	if ( dateFrom > dateTo )
	{
		COleDateTime dateTemp = dateFrom;
		dateFrom = dateTo;
		dateTo = dateTemp;
	}

	m_nSearchStartDay = GetDayNumberSince2011( dateFrom );
	m_nSearchEndDay = GetDayNumberSince2011( dateTo );
}

/**********************************************************************/

void CPMSAccountFindDlg::UpdateList( int nSelectStartDay, int nSelectBookingId, int nSelectBookingPart )
{
	if ( IDM_PMSMENUBAR_CANCELLED_BOOKING != m_nType )
		UpdateListNormal( nSelectStartDay, nSelectBookingId, nSelectBookingPart );
	else
		UpdateListCancelled( nSelectStartDay, nSelectBookingId, nSelectBookingPart );	
}

/**********************************************************************/

void CPMSAccountFindDlg::UpdateListNormal( int nSelectStartDay, int nSelectBookingId, int nSelectBookingPart )
{
	m_arrayAccountFindInfo.RemoveAll();
	
	int nStartRoomIdx = 0;
	int nEndRoomIdx = DataManagerNonDb.PMSRoom.GetSize() - 1;

	if ( m_nSearchRoomNo != 0 )
	{
		if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( m_nSearchRoomNo, nStartRoomIdx ) == TRUE )
			nEndRoomIdx = nStartRoomIdx;
		else
		{
			nStartRoomIdx = 0;
			nEndRoomIdx = -1;
		}
	}

	for ( int nRoomIdx = nStartRoomIdx; nRoomIdx <= nEndRoomIdx; nRoomIdx++ )
	{
		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
		int nRoomNo = Room.GetRoomNo();
		CString strRoomName = Room.GetRoomName();

		int nStartIdx, nEndIdx;
		PMSCalendarTable.GetBookingIndexRange( nRoomNo, m_nSearchStartDay - 1, m_nSearchEndDay, nStartIdx, nEndIdx );

		for ( int nCalendarIdx = nStartIdx; nCalendarIdx <= nEndIdx; nCalendarIdx++ )
		{
			CPMSCalendarBooking CalendarBooking;
			PMSCalendarTable.GetAt( nCalendarIdx, CalendarBooking );

			if ( nRoomNo != CalendarBooking.m_nRoomNo )
				continue;

			if ( m_nSearchStartDay > CalendarBooking.m_nStartDay + CalendarBooking.m_nNights )
				continue;

			if ( m_nSearchEndDay < CalendarBooking.m_nStartDay )
				continue;

			CPMSBookingCSVRecord BookingRecord;
			CPMSCustomerCSVRecord CustomerRecord;

			int nBookingId = CalendarBooking.m_nBookingId;
			int nBookingPart = CalendarBooking.m_nBookingPart;
			if ( CheckBooking( nBookingId, BookingRecord, CustomerRecord ) == TRUE )
			{
				bool bValidPart = TRUE;

				switch( m_nType )
				{
				case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
				case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
					bValidPart = ( BookingRecord.GetPartCount() == CalendarBooking.m_nBookingPart + 1 );
					break;

				default:
					bValidPart = ( BookingRecord.GetPartCount() > CalendarBooking.m_nBookingPart );
					break;
				}

				if ( FALSE == bValidPart )
					continue;

				CPMSBookingPartInfo infoFirstPart, infoPart;
				BookingRecord.GetPartInfo( 0, infoFirstPart );
				BookingRecord.GetPartInfo( CalendarBooking.m_nBookingPart, infoPart );

				if ( infoPart.GetRoomNo() != nRoomNo )
					continue;

				int nCheckInDay = infoFirstPart.GetStartDay();

				int nPos;
				if ( FindBookingPart( nCheckInDay, nBookingId, nBookingPart, nPos ) == FALSE )
				{
					CPMSAccountFindInfo info;
					info.m_nInvoiceNum = BookingRecord.GetInvoiceNum();
					info.m_nCheckInDay = nCheckInDay;
					info.m_nBookingId = nBookingId;
					info.m_nBookingPart = nBookingPart;
					info.m_nRoomNo = nRoomNo;
					info.m_strRoomName = strRoomName;
					info.m_nCustomerId = BookingRecord.GetCustomerId();
					info.m_nGuests = BookingRecord.GetGuests();
					info.m_strGuestName = CustomerRecord.GetGuestName();
					info.m_strBillingName = CustomerRecord.GetBillingName();
					info.m_nBookingPartStartDay = infoPart.GetStartDay();
					info.m_nBookingPartNights = infoPart.GetNights();
					info.m_bCheckedOut = ( BookingRecord.GetBookingStatus() == PMS_BOOKING_STATUS_COMPLETE );
					m_arrayAccountFindInfo.InsertAt( nPos, info );
				}
			}
		}

		PMSZeroNightStayArray.GetZeroNightStayArrayRange( nRoomNo, m_nSearchStartDay, m_nSearchEndDay, nStartIdx, nEndIdx );

		for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
		{
			CString strDayNumber;
			PMSZeroNightStayArray.GetZeroNightStayArrayDay( nIdx, strDayNumber );
			int nDayNumber = atoi( strDayNumber );

			CString strIDs;
			PMSZeroNightStayArray.GetZeroNightStayArrayIDs( nIdx, strIDs );

			CCSV csv( strIDs );

			for ( int n = 0; n < csv.GetSize(); n += 2 )
			{
				int nBookingId = csv.GetInt(n);
				int nBookingPart = csv.GetInt(n + 1 );

				CPMSBookingCSVRecord BookingRecord;
				CPMSCustomerCSVRecord CustomerRecord;

				if ( CheckBooking( nBookingId, BookingRecord, CustomerRecord ) == TRUE )
				{
					if ( nBookingPart == BookingRecord.GetPartCount() - 1 )
					{
						int nPos;
						if ( FindBookingPart( nDayNumber, nBookingId, nBookingPart, nPos ) == FALSE )
						{
							CPMSAccountFindInfo info;
							info.m_nInvoiceNum = BookingRecord.GetInvoiceNum();
							info.m_nCheckInDay = nDayNumber;
							info.m_nBookingId = nBookingId;
							info.m_nBookingPart = nBookingPart;
							info.m_nRoomNo = nRoomNo;
							info.m_strRoomName = strRoomName;
							info.m_nCustomerId = BookingRecord.GetCustomerId();
							info.m_nGuests = BookingRecord.GetGuests();
							info.m_strGuestName = CustomerRecord.GetGuestName();
							info.m_strBillingName = CustomerRecord.GetBillingName();
							info.m_nBookingPartStartDay = nDayNumber;
							info.m_nBookingPartNights = 0;
							info.m_bCheckedOut = ( BookingRecord.GetBookingStatus() == PMS_BOOKING_STATUS_COMPLETE );
							m_arrayAccountFindInfo.InsertAt( nPos, info );
						}
					}
				}
			}
		}
	}
	
	m_listData.SetItemCountEx( m_arrayAccountFindInfo.GetSize() );
	m_listData.Invalidate();

	int nPos;
	if ( FindBookingPart( nSelectStartDay, nSelectBookingId, nSelectBookingPart, nPos ) == TRUE )
		m_listData.SetCurSel( nPos );
	else if ( m_arrayAccountFindInfo.GetSize() > 0 )
		m_listData.SetCurSel(0);
}

/**********************************************************************/

void CPMSAccountFindDlg::UpdateListCancelled( int nSelectStartDay, int nSelectBookingId, int nSelectBookingPart )
{
	m_arrayAccountFindInfo.RemoveAll();

	for ( int nBookingIdx = 0; nBookingIdx < DataManagerNonDb.PMSBooking.GetSize(); nBookingIdx++ )
	{
		CPMSBookingCSVRecord BookingRecord;
		DataManagerNonDb.PMSBooking.QuickGetAt( nBookingIdx, BookingRecord, FALSE );

		if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_CANCELLED )
			continue;

		CPMSBookingPartInfo infoPart;
		BookingRecord.GetPartInfo( 0, infoPart );

		int nRoomNo = infoPart.GetRoomNo();

		if ( ( m_nSearchRoomNo != 0 ) && ( m_nSearchRoomNo != nRoomNo ) )
			continue;

		if ( m_nSearchStartDay > infoPart.GetStartDay() + infoPart.GetNights() )
			continue;

		if ( m_nSearchEndDay < infoPart.GetStartDay() )
			continue;

		int nRoomIdx;
		CString strRoomName = "";
		if ( DataManagerNonDb.PMSRoom.FindRoomByNumber( nRoomNo, nRoomIdx ) == TRUE )
		{
			CPMSRoomCSVRecord Room;
			DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );
			strRoomName = Room.GetRoomName();
		}

		CPMSCustomerCSVRecord CustomerRecord;

		int nBookingId = BookingRecord.GetBookingId();
		if ( CheckBooking( nBookingId, BookingRecord, CustomerRecord ) == TRUE )
		{
			int nCheckInDay = infoPart.GetStartDay();

			int nPos;
			if ( FindBookingPart( nCheckInDay, nBookingId, 0, nPos ) == FALSE )
			{
				CPMSAccountFindInfo info;
				info.m_nInvoiceNum = BookingRecord.GetInvoiceNum();
				info.m_nCheckInDay = nCheckInDay;
				info.m_nBookingId = nBookingId;
				info.m_nBookingPart = 0;
				info.m_nRoomNo = nRoomNo;
				info.m_strRoomName = strRoomName;
				info.m_nCustomerId = BookingRecord.GetCustomerId();
				info.m_nGuests = BookingRecord.GetGuests();
				info.m_strGuestName = CustomerRecord.GetGuestName();
				info.m_strBillingName = CustomerRecord.GetBillingName();
				info.m_nBookingPartStartDay = infoPart.GetStartDay();
				info.m_nBookingPartNights = infoPart.GetNights();
				info.m_bCheckedOut = ( BookingRecord.GetBookingStatus() == PMS_BOOKING_STATUS_COMPLETE );
				m_arrayAccountFindInfo.InsertAt( nPos, info );
			}
		}
	}

	m_listData.SetItemCountEx( m_arrayAccountFindInfo.GetSize() );
	m_listData.Invalidate();

	int nPos;
	if ( FindBookingPart( nSelectStartDay, nSelectBookingId, nSelectBookingPart, nPos ) == TRUE )
		m_listData.SetCurSel( nPos );
	else if ( m_arrayAccountFindInfo.GetSize() > 0 )
		m_listData.SetCurSel(0);
}

/**********************************************************************/

void CPMSAccountFindDlg::RemoveListEntry( int nSel )
{
	if ( ( nSel < 0 ) || ( nSel >= m_arrayAccountFindInfo.GetSize() ) )
		return;

	CPMSAccountFindInfo infoFind = m_arrayAccountFindInfo.GetAt(nSel);

	int nStartSel;
	FindBookingPart( infoFind.m_nCheckInDay, infoFind.m_nBookingId, 0, nStartSel );

	int nEndSel;
	FindBookingPart( infoFind.m_nCheckInDay, infoFind.m_nBookingId + 1, 0, nEndSel );
		nEndSel--;

	for ( nSel = nEndSel; nSel >= nStartSel; nSel-- )
		m_arrayAccountFindInfo.RemoveAt( nSel );		
}

/**********************************************************************/

bool CPMSAccountFindDlg::CheckBooking( int nBookingId, CPMSBookingCSVRecord& BookingRecord, CPMSCustomerCSVRecord& CustomerRecord )
{
	if ( m_nSearchReference != 0 )
		if ( m_nSearchReference != nBookingId )
			return FALSE;

	int nBookingIdx;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return FALSE;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

	if ( m_nSearchInvoiceNum != 0 )
		if ( m_nSearchInvoiceNum != BookingRecord.GetInvoiceNum() )
			return FALSE;

	if ( m_checkTraining.GetCheck() == FALSE )
		if ( BookingRecord.GetTrainingFlag() == TRUE )
			return FALSE;

	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:

		if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_PENDING )
			return FALSE;

		break;

	case IDM_PMSMENUBAR_CANCELLED_BOOKING:

		if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_CANCELLED )
			return FALSE;

		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:

		if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
			return FALSE;

		switch ( BookingRecord.GetCheckoutBalanceType()  )
		{
		case PMS_CHECKOUT_BALANCE_ZERO:
		case PMS_CHECKOUT_BALANCE_CREDIT:
			return FALSE;
		}

		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:

		if ( BookingRecord.GetBookingStatus() != PMS_BOOKING_STATUS_COMPLETE )
			return FALSE;

		switch ( BookingRecord.GetCheckoutBalanceType()  )
		{
		case PMS_CHECKOUT_BALANCE_ZERO:
		case PMS_CHECKOUT_BALANCE_DEBIT:
			return FALSE;
		}

		break;

	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:
	default:

		if ( BookingRecord.GetBookingStatus() == PMS_BOOKING_STATUS_PENDING )
			return FALSE;

		break;
	}

	int nCustomerIdx;
	if ( DataManagerNonDb.PMSCustomer.FindCustomerById( BookingRecord.GetCustomerId(), nCustomerIdx ) == TRUE )
		DataManagerNonDb.PMSCustomer.GetAt( nCustomerIdx, CustomerRecord );
			
	if ( m_strName != "" )
	{
		CString strTemp1 = CustomerRecord.GetGuestName();
		strTemp1.MakeUpper();

		CString strTemp2 = m_strName;
		strTemp2.MakeUpper();

		if ( strTemp1.Find( strTemp2 ) == -1 )
			return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CPMSAccountFindDlg::OnButtonPrint()
{
	CArray<int,int> arrayDummy;
	CPMSAccountFindReportDlg dlgReport( m_arrayAccountFindInfo, arrayDummy, FALSE, m_nType, this );
	dlgReport.DoModal();
}

/**********************************************************************/

void CPMSAccountFindDlg::OnButtonAction1()
{
	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:
	case IDM_PMSMENUBAR_CANCELLED_BOOKING:
		ProcessBooking();
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:
	default:
		ProcessSales();
		break;
	}
}

/**********************************************************************/

void CPMSAccountFindDlg::OnButtonAction2()
{
	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:
		DeleteBooking();
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:
	default:
		ProcessPayments();
		break;
	}
}

/**********************************************************************/

void CPMSAccountFindDlg::OnButtonAction3()
{
	switch( m_nType )
	{
	case IDM_PMSMENUBAR_FIND_BOOKING:
		break;

	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
	case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
	case IDM_PMSMENUBAR_GUEST_ACCOUNTS:
	default:
		ProcessInvoice();
		break;
	}
}

/**********************************************************************/

void CPMSAccountFindDlg::DeleteBooking()
{
	int nSel = GetSafeSelection();
	if ( nSel == -1 )
		return;

	int nBookingIdx;
	int nBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == TRUE )
	{
		if ( Prompter.YesNo( "Are you sure you wish to delete this booking" ) == IDYES )
		{
			CPMSBookingCSVRecord BookingRecord;
			DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

			PMSCalendarTable.SetEditMarkers( BookingRecord );
			BookingRecord.SetBookingStatus( PMS_BOOKING_STATUS_CANCELLED );
			CPMSBookingCSVArray::InsertAndSaveBooking( BookingRecord );
			PMSCalendarTable.DeleteMarkedBookings();

			if ( m_pCalendarDlg != NULL )
				m_pCalendarDlg -> UpdateDisplay();

			UpdateList( 0, 0, 0 );
		}
	}	
}

/**********************************************************************/

void CPMSAccountFindDlg::ProcessInvoice()
{
	int nSel = GetSafeSelection();
	if ( nSel == -1 )
		return;

	int nBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
	if ( CPMSCalendarDlg::ProcessInvoiceComplete( nBookingId, this ) == TRUE )
	{
		m_nLastEditedBookingId = nBookingId;
		if ( FinaliseAccountChanges( nSel, nBookingId ) == FALSE )
		{
			int nSelectCheckInDay = m_arrayAccountFindInfo[nSel].m_nCheckInDay;
			int nSelectBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
			int	nSelectBookingPart = m_arrayAccountFindInfo[nSel].m_nBookingPart;
			UpdateList( nSelectCheckInDay, nSelectBookingId, nSelectBookingPart );
		}
	}
}

/**********************************************************************/

void CPMSAccountFindDlg::ProcessSales()
{
	int nSel = GetSafeSelection();
	if ( nSel == -1 )
		return;

	int nBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
	if ( CPMSCalendarDlg::ProcessSalesComplete( nBookingId, this ) == TRUE )
	{
		m_nLastEditedBookingId = nBookingId;
		FinaliseAccountChanges( nSel, nBookingId );
	}
}

/**********************************************************************/

void CPMSAccountFindDlg::ProcessPayments()
{
	int nSel = GetSafeSelection();
	if ( nSel == -1 )
		return;

	int nBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
	if ( CPMSCalendarDlg::ProcessPaymentsComplete( nBookingId, this ) == TRUE )
	{
		m_nLastEditedBookingId = nBookingId;
		FinaliseAccountChanges( nSel, nBookingId );
	}
}

/**********************************************************************/

void CPMSAccountFindDlg::ProcessBooking()
{
	int nSel = GetSafeSelection();
	if ( nSel == -1 )
		return;

	int nBookingIdx;
	int nBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
	CPMSBookingCSVRecord BookingRecord;
	if ( DataManagerNonDb.PMSBooking.FindBookingById( nBookingId, nBookingIdx ) == FALSE )
		return;

	DataManagerNonDb.PMSBooking.GetAt( nBookingIdx, BookingRecord );

	switch( BookingRecord.GetBookingStatus() )
	{
	case PMS_BOOKING_STATUS_PENDING:
		m_nLastEditedBookingId = nBookingId;
		CPMSCalendarDlg::EditBookingPending( nBookingId, this );
		break;

	case PMS_BOOKING_STATUS_CANCELLED:
		m_nLastEditedBookingId = nBookingId;
		CPMSCalendarDlg::EditBookingCancelled( nBookingId, this );
		break;
	}

	if ( m_pCalendarDlg != NULL )
		m_pCalendarDlg -> UpdateDisplay();

	int nSelectCheckInDay = m_arrayAccountFindInfo[nSel].m_nCheckInDay;
	int nSelectBookingId = m_arrayAccountFindInfo[nSel].m_nBookingId;
	int	nSelectBookingPart = m_arrayAccountFindInfo[nSel].m_nBookingPart;
	UpdateList( nSelectCheckInDay, nSelectBookingId, nSelectBookingPart );

	m_listData.Invalidate();
}

/**********************************************************************/

bool CPMSAccountFindDlg::FinaliseAccountChanges( int nSel, int nBookingId )
{
	bool bRemoveLine = FALSE;
		
	if ( TRUE == m_arrayAccountFindInfo[nSel].m_bCheckedOut )
	{
		bool bClearedDebit, bClearedCredit;
		CPMSCalendarDlg::UpdateCompleteAccounts( nBookingId, bClearedDebit, bClearedCredit );

		switch( m_nType )
		{
		case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_DEBIT:
			bRemoveLine = bClearedDebit;
			break;

		case IDM_PMSMENUBAR_CHECK_OUT_UNPAID_CREDIT:
			bRemoveLine = bClearedCredit;
			break;
		}
		
		if ( TRUE == bRemoveLine )
		{
			RemoveListEntry( nSel );

			int nSize = m_arrayAccountFindInfo.GetSize();
			m_listData.SetItemCountEx( nSize );

			if ( nSel >= nSize )
				nSel = nSize - 1;

			m_listData.SetCurSel( nSel );
		}		
	}

	m_listData.Invalidate();
	return bRemoveLine;
}

/**********************************************************************/
#endif
/**********************************************************************/
