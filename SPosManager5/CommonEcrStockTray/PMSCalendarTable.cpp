/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSZeroNightStayArray.h"
/**********************************************************************/
#include "PMSCalendarTable.h"
/**********************************************************************/

CPMSCalendarTable::CPMSCalendarTable(void)
{
}

/**********************************************************************/

CPMSCalendarTable::~CPMSCalendarTable(void)
{
}

/**********************************************************************/

void CPMSCalendarTable::RemoveAll()
{
	m_arrayBookings.RemoveAll();
}

/**********************************************************************/

void CPMSCalendarTable::GetAt( int nIdx, CPMSCalendarBooking& Booking )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayBookings.GetSize() ) )
		Booking = m_arrayBookings.GetAt( nIdx );
}

/**********************************************************************/

void CPMSCalendarTable::SetAt( int nIdx, CPMSCalendarBooking& Booking )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayBookings.GetSize() ) )
		m_arrayBookings.SetAt( nIdx, Booking );
}

/**********************************************************************/

void CPMSCalendarTable::InsertAt( int nIdx, CPMSCalendarBooking& Booking )
{
	if ( ( nIdx >= 0 ) && ( nIdx <= m_arrayBookings.GetSize() ) )
		m_arrayBookings.InsertAt( nIdx, Booking );
}

/**********************************************************************/

void CPMSCalendarTable::RemoveAt( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx <= m_arrayBookings.GetSize() ) )
		m_arrayBookings.RemoveAt( nIdx );
}

/**********************************************************************/

bool CPMSCalendarTable::FindBooking( CPMSCalendarBooking& Booking, int& nPos )
{
	int nStart = 0;
	int nEnd = m_arrayBookings.GetSize() - 1;

	while( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		if ( Booking.m_nRoomNo > m_arrayBookings[ nPos ].m_nRoomNo )
			nStart = nPos + 1;
		else if ( Booking.m_nRoomNo < m_arrayBookings[ nPos ].m_nRoomNo )
			nEnd = nPos - 1;
		else if ( Booking.m_nStartDay > m_arrayBookings[ nPos ].m_nStartDay )
			nStart = nPos + 1;
		else if ( Booking.m_nStartDay < m_arrayBookings[ nPos ].m_nStartDay )
			nEnd = nPos - 1;
		else
			return TRUE;
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

void CPMSCalendarTable::GetBookingIndexRange( int nRoomNo, int nStartDay, int nEndDay, int& nStartIdx, int& nEndIdx )
{
	CPMSCalendarBooking Booking;
	Booking.m_nRoomNo = nRoomNo;
	Booking.m_nStartDay = nStartDay;

	if ( FindBooking( Booking, nStartIdx ) == FALSE )
		nStartIdx--;

	Booking.m_nStartDay = nEndDay;
	if ( FindBooking( Booking, nEndIdx ) == FALSE )
		nEndIdx--;
}

/**********************************************************************/

bool CPMSCalendarTable::Validate( int x, int y, int z )
{
	return ( ( x >= y ) && ( x <= z ) );
}

/**********************************************************************/

bool CPMSCalendarTable::ValidateRoomCheck( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response )
{
	//ASSUME ROOM NOT AVAILABLE
	Response.m_bSuccess = FALSE;
	Response.m_nStartDay = Query.m_nStartDay;
	Response.m_nMaxNights = 0;
	Response.m_nMinStartDay = Query.m_nStartDay;
	Response.m_nMaxStartDay = Query.m_nStartDay;
	Response.m_nBookingClashId = 0;
	Response.m_nBookingClashPart = 0;
	Response.m_nBookingClashArrayIdx = 0;

	//BASIC VALIDATION OF BOOKING REQUEST
	if ( Validate( Query.m_nStartDay, PMSBooking::StartDay.Min, PMSBooking::StartDay.Max ) == FALSE )
		return FALSE;

	if ( Validate( Query.m_nMinNights, 1, PMSBooking::Nights.Max ) == FALSE )
		return FALSE;

	if ( Validate( Query.m_nMaxNights, 1, PMSBooking::Nights.Max ) == FALSE )
		return FALSE;
	
	if ( Query.m_nMinNights > Query.m_nMaxNights )
		return FALSE;

	int nEndCheck = Query.m_nStartDay + Query.m_nMaxNights - 1;
	if ( Validate( nEndCheck, PMSBooking::StartDay.Min, PMSBooking::StartDay.Max ) == FALSE )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CPMSCalendarTable::CheckRoomAvailability( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response )
{
	CheckRoomAvailabilityInternal( Query, Response );

	if ( FALSE == Response.m_bSuccess )
	{
		if ( Response.m_nBookingClashId != 0 )
		{
			int nArrayIdx = Response.m_nBookingClashArrayIdx;
			CPMSCalendarBooking Booking = m_arrayBookings[ nArrayIdx ];

			if ( TRUE == Booking.m_bEditing )
			{
				m_arrayBookings.RemoveAt( nArrayIdx );
				CheckRoomAvailabilityInternal( Query, Response );
				m_arrayBookings.InsertAt( nArrayIdx, Booking );
			}
		}
	}

	/*
	CString strResult;
	strResult.Format( "Check Availability\n\nQuery\n\nRoom No : %d\nStart Day : %d\nMin Nights : %d\nMax Nights %d\n\nResponse\n\nStart Day : %d\nMin Start Day : %d\nMax Start Day : %d\nMax Nights : %d\nClash Id %d",
		Query.m_nRoomNo,
		Query.m_nStartDay,
		Query.m_nMinNights,
		Query.m_nMaxNights,
		Response.m_nStartDay,
		Response.m_nMinStartDay,
		Response.m_nMaxStartDay,
		Response.m_nMaxNights,
		Response.m_nBookingClashId );

	Prompter.Diagnostic( strResult );
	*/
}

/**********************************************************************/

void CPMSCalendarTable::CheckRoomAvailabilityInternal( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response )
{
	if ( ValidateRoomCheck( Query, Response ) == FALSE )
		return;

	//WHOLE DATE RANGE IS AVAILABLE IF NO PREVIOUS BOOKINGS
	if ( GetSize() == 0 )
	{	
		Response.m_bSuccess = TRUE;
		Response.m_nMaxNights = Query.m_nMaxNights;
		Response.m_nMinStartDay = PMSBooking::StartDay.Min;
		Response.m_nMaxStartDay = PMSBooking::StartDay.Max;
		return;
	}

	//ROOM IS NOT AVAILABLE IF A BOOKING STARTS ON THE CHECK IN DATE
	CPMSCalendarBooking CalendarBooking;
	CalendarBooking.m_nRoomNo = Query.m_nRoomNo;
	CalendarBooking.m_nStartDay = Query.m_nStartDay;

	int nPos;
	if ( FindBooking( CalendarBooking, nPos ) == TRUE )
	{
		Response.m_nBookingClashId = m_arrayBookings[nPos].m_nBookingId;
		Response.m_nBookingClashPart = m_arrayBookings[nPos].m_nBookingPart;
		Response.m_nBookingClashArrayIdx = nPos;
		return;
	}

	int nMinStartDay = PMSBooking::StartDay.Min;
	int nMaxStartDay = PMSBooking::StartDay.Max;

	//ROOM IS NOT AVAILABLE IF PREVIOUS BOOKING CROSSES CHECKIN DATE
	if ( ( nPos >= 1 ) && ( nPos <= GetSize() ) )
	{
		CPMSCalendarBooking Booking = m_arrayBookings[ nPos - 1 ];

		if ( Booking.m_nRoomNo == Query.m_nRoomNo )
		{
			nMinStartDay = Booking.m_nStartDay + Booking.m_nNights;

			if ( nMinStartDay > Query.m_nStartDay )
			{
				Response.m_nBookingClashId = Booking.m_nBookingId;
				Response.m_nBookingClashPart = Booking.m_nBookingPart;
				Response.m_nBookingClashArrayIdx = nPos - 1;
				return;
			}
		}
	}

	//ROOM IS NOT AVAILABLE IF NEXT BOOKING CROSSES CHECKOUT DATE
	if ( ( nPos >= 0 ) && ( nPos < GetSize() ) )
	{
		CPMSCalendarBooking Booking = m_arrayBookings[ nPos ];
			
		if ( Booking.m_nRoomNo == Query.m_nRoomNo )
		{
			if ( Booking.m_nStartDay < Query.m_nStartDay + Query.m_nMinNights )
			{
				Response.m_nBookingClashId = Booking.m_nBookingId;
				Response.m_nBookingClashPart = Booking.m_nBookingPart;
				Response.m_nBookingClashArrayIdx = nPos;
				return;
			}

			nMaxStartDay = Booking.m_nStartDay - 1;
		}
	}

	Response.m_bSuccess = TRUE;
	Response.m_nMinStartDay = nMinStartDay;
	Response.m_nMaxStartDay = nMaxStartDay;
	Response.m_nMaxNights = ( Response.m_nMaxStartDay - Response.m_nStartDay ) + 1;

	if ( Response.m_nMaxNights > Query.m_nMaxNights )
		Response.m_nMaxNights = Query.m_nMaxNights;
}

/**********************************************************************/

void CPMSCalendarTable::BookRoom( CPMSBookingQueryInfo& Query, CPMSBookingResponseInfo& Response )
{
	Query.m_nMaxNights = Query.m_nMinNights;

	if ( ValidateRoomCheck( Query, Response ) == FALSE )
		return;

	Response.m_nMaxNights = Query.m_nMaxNights;
	
	int nPos = 0;
	if ( GetSize() != 0 )
	{
		//ROOM IS NOT AVAILABLE IF A BOOKING STARTS ON THE CHECK IN DATE
		CPMSCalendarBooking CalendarBooking;
		CalendarBooking.m_nRoomNo = Query.m_nRoomNo;
		CalendarBooking.m_nStartDay = Query.m_nStartDay;
		
		//ROOM IS NOT AVAILABLE IF A BOOKING STARTS ON THIS DATE
		if ( FindBooking( CalendarBooking, nPos ) == TRUE )
		{
			Response.m_nBookingClashId = m_arrayBookings[ nPos ].m_nBookingId;
			Response.m_nBookingClashPart = m_arrayBookings[ nPos ].m_nBookingPart;
			return;
		}

		//ROOM IS NOT AVAILABLE IF PREVIOUS BOOKING CROSSES START DATE
		if ( ( nPos >= 1 ) && ( nPos <= GetSize() ) )
		{
			CPMSCalendarBooking Booking = m_arrayBookings[ nPos - 1 ];

			if ( Booking.m_nRoomNo == Query.m_nRoomNo )
			{
				if ( Booking.m_nStartDay + Booking.m_nNights > Query.m_nStartDay )
				{
					Response.m_nBookingClashId = Booking.m_nBookingId;
					Response.m_nBookingClashPart = Booking.m_nBookingPart;
					return;
				}
			}
		}

		//ROOM IS NOT AVAILABLE IF NEXT BOOKING CROSSES CHECK OUT DATE
		if ( ( nPos >= 0 ) && ( nPos < GetSize() ) )
		{
			CPMSCalendarBooking Booking = m_arrayBookings[ nPos ];

			if ( Booking.m_nRoomNo == Query.m_nRoomNo )
			{
				if ( Booking.m_nStartDay < Query.m_nStartDay + Query.m_nMinNights )
				{
					Response.m_nBookingClashId = Booking.m_nBookingId;
					Response.m_nBookingClashPart = Booking.m_nBookingPart;
					return;
				}
			}
		}
	}

	CPMSCalendarBooking CalendarBooking;
	CalendarBooking.m_nBookingId = Query.m_nBookingId;
	CalendarBooking.m_nBookingPart = Query.m_nBookingPart;
	CalendarBooking.m_nRoomNo = Query.m_nRoomNo;
	CalendarBooking.m_nStartDay = Query.m_nStartDay;
	CalendarBooking.m_nNights = Query.m_nMinNights;
	CalendarBooking.m_nStatus = Query.m_nStatus;
	CalendarBooking.m_bEditing = FALSE;
	CalendarBooking.m_bIsFinalPart = Query.m_bIsFinalPart;

	InsertAt( nPos, CalendarBooking );
	
	Response.m_bSuccess = TRUE;
}

/**********************************************************************/

void CPMSCalendarTable::SetEditMarkers( CPMSBookingCSVRecord& BookingRecord )
{
	m_arrayEditingIdx.RemoveAll();

	for ( int nIdx = 0; nIdx < BookingRecord.GetPartCount(); nIdx++ )
	{
		CPMSBookingPartInfo infoPart;
		BookingRecord.GetPartInfo( nIdx, infoPart );

		CPMSCalendarBooking CalendarBooking;
		CalendarBooking.m_nRoomNo = infoPart.GetRoomNo();
		CalendarBooking.m_nStartDay = infoPart.GetStartDay();

		int nCalendarIdx;
		if ( FindBooking( CalendarBooking, nCalendarIdx ) == TRUE )
		{
			m_arrayEditingIdx.Add( nCalendarIdx );
			m_arrayBookings[ nCalendarIdx ].m_bEditing = TRUE;
		}
	}
}

/**********************************************************************/

void CPMSCalendarTable::SetEditMarkers( CPMSClosedRoomCSVRecord& ClosureRecord, bool bKeepExisting )
{
	if ( FALSE == bKeepExisting )
		m_arrayEditingIdx.RemoveAll();

	CPMSCalendarBooking CalendarBooking;
	CalendarBooking.m_nRoomNo = ClosureRecord.GetRoomNo();
	CalendarBooking.m_nStartDay = ClosureRecord.GetStartDay();

	int nCalendarIdx;
	if ( FindBooking( CalendarBooking, nCalendarIdx ) == TRUE )
	{
		m_arrayEditingIdx.Add( nCalendarIdx );
		m_arrayBookings[ nCalendarIdx ].m_bEditing = TRUE;
	}
}

/**********************************************************************/

void CPMSCalendarTable::ClearEditMarkers()
{
	for ( int n = 0; n < m_arrayEditingIdx.GetSize(); n++ )
	{
		int nIdx = m_arrayEditingIdx.GetAt(n);
		if ( ( nIdx >= 0 ) && ( nIdx < m_arrayBookings.GetSize() ) )
			m_arrayBookings[ nIdx ].m_bEditing = FALSE;
	}

	m_arrayEditingIdx.RemoveAll();
}

/**********************************************************************/

void CPMSCalendarTable::DeleteMarkedBookings()
{
	CReportConsolidationArray<CSortedStringItem> arrayIdx;
	
	for ( int n = 0; n < m_arrayEditingIdx.GetSize(); n++ )
	{
		int nIdx = m_arrayEditingIdx.GetAt(n);
		
		CString str;
		str.Format( "%8.8d", nIdx );

		CSortedStringItem item;
		item.m_strItem = str;
		arrayIdx.Consolidate( item );
	}

	for ( int n = arrayIdx.GetSize() - 1; n >= 0; n-- )
	{
		CSortedStringItem item;
		arrayIdx.GetAt( n, item );
		int nIdx = atoi( item.m_strItem );

		if ( ( nIdx >= 0 ) && ( nIdx < m_arrayBookings.GetSize() ) )
		{
			if ( TRUE == m_arrayBookings[ nIdx ].m_bEditing )
				m_arrayBookings.RemoveAt( nIdx );
		}
	}

	m_arrayEditingIdx.RemoveAll();
}

/**********************************************************************/

void CPMSCalendarTable::ApplyBooking( CPMSBookingCSVRecord& BookingRecord )
{
	for ( int nPart = 0; nPart < BookingRecord.GetPartCount(); nPart++ )
	{
		CPMSBookingPartInfo infoPart;
		BookingRecord.GetPartInfo( nPart, infoPart );

		if ( infoPart.GetNights() > 0 )
		{
			CPMSBookingQueryInfo Query;
			CPMSBookingResponseInfo Response;

			bool bIsFinalPart = TRUE;
			int nCalendarStatus = BookingRecord.GetBookingStatus();

			if ( nPart != BookingRecord.GetPartCount() - 1 )
			{
				if ( BookingRecord.GetBookingStatus() == PMS_BOOKING_STATUS_ACTIVE )
					nCalendarStatus = PMS_BOOKING_STATUS_COMPLETE;

				bIsFinalPart = FALSE;
			}

			Query.m_nBookingId = BookingRecord.GetBookingId();
			Query.m_nBookingPart = nPart;
			Query.m_nRoomNo = infoPart.GetRoomNo();
			Query.m_nStartDay = infoPart.GetStartDay();
			Query.m_nMinNights = infoPart.GetNights();
			Query.m_nStatus = nCalendarStatus;
			Query.m_bIsFinalPart = bIsFinalPart;
			BookRoom( Query, Response );
		}
		else
			PMSZeroNightStayArray.AddZeroNightStayEntry( infoPart.GetRoomNo(), infoPart.GetStartDay(), BookingRecord.GetBookingId(), nPart );
	}
}

/**********************************************************************/

void CPMSCalendarTable::ApplyClosure( CPMSClosedRoomCSVRecord& ClosureRecord )
{
	CPMSBookingQueryInfo Query;
	CPMSBookingResponseInfo Response;

	Query.m_nBookingId = ClosureRecord.GetClosureId();
	Query.m_nBookingPart = 0;
	Query.m_nRoomNo = ClosureRecord.GetRoomNo();
	Query.m_nStartDay = ClosureRecord.GetStartDay();
	Query.m_nMinNights = ClosureRecord.GetNights();
	Query.m_nStatus = PMS_BOOKING_STATUS_CLOSED;
	Query.m_bIsFinalPart = TRUE;
	BookRoom( Query, Response );
}

/**********************************************************************/
#endif
/**********************************************************************/
