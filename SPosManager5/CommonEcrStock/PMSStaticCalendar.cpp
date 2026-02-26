/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSBrushObject.h"
#include "PMSCalendarTable.h"
#include "PMSOptions.h"
/**********************************************************************/
#include "PMSStaticCalendar.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSStaticCalendar, CStatic)
/**********************************************************************/

CPMSStaticCalendar::CPMSStaticCalendar( CPMSCalendarHelpers& Helpers ) :
		m_CalendarHelpers( Helpers )
{
	m_pClickPoint = NULL;
	m_nCalendarClickRow = -1;
	m_nCalendarClickCol = -1;
	m_nCalendarMoveRow = -1;
	m_nCalendarMoveCol = -1;
	m_nActiveRows = 0;
	m_nSpecialHighlightMode = PMS_SPECIAL_HIGHLIGHT_NONE;
}

/**********************************************************************/

CPMSStaticCalendar::~CPMSStaticCalendar()
{
}

/**********************************************************************/

void CPMSStaticCalendar::SetSpecialHighlightMode( int n )
{
	switch(n)
	{
	case PMS_SPECIAL_HIGHLIGHT_NONE:
	case PMS_SPECIAL_HIGHLIGHT_DEBIT:
	case PMS_SPECIAL_HIGHLIGHT_CREDIT:
	case PMS_SPECIAL_HIGHLIGHT_BOTH:
		m_nSpecialHighlightMode = n;
		break;
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSStaticCalendar, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/**********************************************************************/

void CPMSStaticCalendar::OnPaint()
{
	m_nActiveRows = 0;

    // Device context for painting

    CPaintDC dc(this);
    
    // Double-buffering

    CDC*        pDCMem        = new CDC;
    CBitmap*    pOldBitmap    = NULL;
    CBitmap     bmpCanvas;
    CRect       rFrame;

    GetClientRect( rFrame );

    pDCMem->CreateCompatibleDC( &dc );

    bmpCanvas.CreateCompatibleBitmap( &dc, rFrame.Width(), rFrame.Height() );

    pOldBitmap = pDCMem->SelectObject( &bmpCanvas );

    // START DRAW -------------------------------------------------

    pDCMem->FillSolidRect( rFrame, RGB(255,255,255) );    // Background

    //CDataManagerInfo info;
	//DataManagerNonDb.OpenPMSRoom( DB_READONLY, info );

	int nStartRoom = m_CalendarHelpers.GetCalendarStartRoom();
	int nStartCol = m_CalendarHelpers.GetCalendarStartCol();
	int nSelectedBookingId = m_CalendarHelpers.GetSelectedBookingId();
	int nSelectedClosureId = m_CalendarHelpers.GetSelectedClosureId();

	int nDayToday = DataManagerNonDb.PMSBooking.GetCurrentDayNumber();
	int nStartDay = m_CalendarHelpers.GetActualDayNumberFromColumn ( nStartCol, nDayToday );
	
	COleDateTime dateStart;
	GetOleDateSince2011( nStartDay, dateStart );
	int nWeekday = dateStart.GetDayOfWeek();

	COLORREF crWhite =	COLORREF( 0xFFFFFF );
	COLORREF crRed =	COLORREF( 0x0000CC );
	COLORREF crBlue =	COLORREF (0xFF0000 );
	COLORREF crBlack =	COLORREF( 0x000000 );

	//WEEKDAY INITIALS
	int nDayWidth = 1;
	int nColsToDraw = m_CalendarHelpers.GetDisplayCols();

	for ( int nOffset = 0; nOffset < nColsToDraw; nOffset += nDayWidth )
	{
		CStringArray arrayTexts;
		COLORREF crText = crBlue;

		if ( ( nStartCol + nOffset != nDayToday ) || ( nOffset >= nColsToDraw - 1 ) )
		{
			nDayWidth = 1;
		
			switch( nWeekday )
			{
			case 1:		arrayTexts.Add( "S" );	crText = crRed; break;
			case 2:		arrayTexts.Add( "M" );	break;
			case 3:		arrayTexts.Add( "T" );	break;
			case 4:		arrayTexts.Add( "W" );	break;
			case 5:		arrayTexts.Add( "T" );	break;
			case 6:		arrayTexts.Add( "F" );	break;
			case 7:		arrayTexts.Add( "S" );	crText = crRed; break;
			default:	arrayTexts.Add( "?" );	break;
			}
		}
		else
		{
			nDayWidth = 2;

			switch( nWeekday )
			{
			case 1:		arrayTexts.Add( "SUN" );	crText = crRed; break;
			case 2:		arrayTexts.Add( "MON" );	break;
			case 3:		arrayTexts.Add( "TUE" );	break;
			case 4:		arrayTexts.Add( "WED" );	break;
			case 5:		arrayTexts.Add( "THU" );	break;
			case 6:		arrayTexts.Add( "FRI" );	break;
			case 7:		arrayTexts.Add( "SAT" );	crText = crRed; break;
			default:	arrayTexts.Add( "???" );	break;
			}
		}

		CRect rectText;
		GetWeekdayRect( nOffset, rectText, nDayWidth );
		DrawTextInDeviceContext( pDCMem, arrayTexts, rectText, TA_CENTER, 10, crText );
	
		nWeekday++;
		if ( nWeekday > 7 )
			nWeekday = 1;	
	}

	int nRoomOffset = 0;
	for ( ; nRoomOffset < m_CalendarHelpers.GetDisplayRows(); nRoomOffset++ )
	{
		int nRoomIdx = nStartRoom + nRoomOffset;
		if ( nRoomIdx >= DataManagerNonDb.PMSRoom.GetSize() )
			break;

		CPMSRoomCSVRecord Room;
		DataManagerNonDb.PMSRoom.GetAt( nRoomIdx, Room );

		//ROOM NAME
		CRect rectText;
		GetRoomNameRect( nRoomOffset, rectText );
		
		CStringArray arrayTexts;
		arrayTexts.Add( Room.GetRoomName() );
		DrawTextInDeviceContext( pDCMem, arrayTexts, rectText, TA_RIGHT, 10, crBlue );

		//ROOM TYPE
		arrayTexts.RemoveAll();
		arrayTexts.Add( Room.GetRoomTypeName( TRUE ) );
		DrawTextInDeviceContext( pDCMem, arrayTexts, rectText, TA_LEFT, 10, crBlue );
	
		//DRAW ROW (ROOM) BACKGROUND AS GREY RECTANGLE
		{
			CRect rectRow;
			GetBookingRect( nRoomOffset, 0, m_CalendarHelpers.GetDisplayCols(), TRUE, TRUE, rectRow );
			CPMSBrushObject Brush( PMSOptions.GetColourVacantBack() );
			pDCMem -> FillRect ( rectRow, Brush.GetBrush() );
		}

		int nStartDay = m_CalendarHelpers.GetActualDayNumberFromColumn( nStartCol, nDayToday );
		int nEndDay = m_CalendarHelpers.GetActualDayNumberFromColumn( nStartCol + m_CalendarHelpers.GetDisplayCols() - 1, nDayToday );

		if ( nStartDay == nDayToday )
			nStartDay--;

		int nStartIdx, nEndIdx;
		int nRoomNo = Room.GetRoomNo();
		PMSCalendarTable.GetBookingIndexRange( nRoomNo, nStartDay, nEndDay, nStartIdx, nEndIdx );

		for ( int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++ )
		{
			if ( ( nIdx < 0 ) || ( nIdx >= PMSCalendarTable.GetSize() ) )
				continue;

			CPMSCalendarBooking CalendarBooking;
			PMSCalendarTable.GetAt( nIdx, CalendarBooking );

			if ( CalendarBooking.m_nRoomNo != nRoomNo )
				continue;

			bool bFullWidthStart = FALSE;
			bool bFullWidthEnd = FALSE;

			int nBookingStartCol = m_CalendarHelpers.GetColumnFromDayNumber( CalendarBooking.m_nStartDay, nDayToday );
			int nBookingEndCol = m_CalendarHelpers.GetColumnFromDayNumber( CalendarBooking.m_nStartDay + CalendarBooking.m_nNights, nDayToday );

			int nBookingOffsetStart = nBookingStartCol - nStartCol;
			int nBookingOffsetEnd = nBookingEndCol - nStartCol;
			
			if ( nBookingOffsetStart >= m_CalendarHelpers.GetDisplayCols() )
				continue;

			if ( nBookingOffsetStart < 0 )
			{
				bFullWidthStart = TRUE;
				nBookingOffsetStart = 0;
			}

			if ( nBookingOffsetEnd > m_CalendarHelpers.GetDisplayCols() )
			{
				bFullWidthEnd = TRUE;
				nBookingOffsetEnd = m_CalendarHelpers.GetDisplayCols();
			}

			if ( FALSE == CalendarBooking.m_bIsFinalPart )
				bFullWidthEnd = TRUE;

			if ( 0 != CalendarBooking.m_nBookingPart )
				bFullWidthStart = TRUE;

			int nBookingWidth = nBookingOffsetEnd - nBookingOffsetStart;
			if ( nBookingWidth <= 0 )
				continue;

			//FIDDLE
			bFullWidthEnd = TRUE;

			//DRAW BOOKING IN APPROPRIATE COLOUR
			{
				COLORREF cr = PMSOptions.GetColourSelectedBack();

				switch( CalendarBooking.m_nStatus )
				{
				case PMS_BOOKING_STATUS_CLOSED:
					cr = PMSOptions.GetColourClosedBack();
					break;

				case PMS_BOOKING_STATUS_ACTIVE:		
					cr = PMSOptions.GetColourActiveBack();
					break;

				case PMS_BOOKING_STATUS_COMPLETE:		
					cr = PMSOptions.GetColourCompleteBack();	
					break;

				case PMS_BOOKING_STATUS_PENDING:
				default:							
					cr = PMSOptions.GetColourPendingBack();		
					break;
				}

				//HIGHLIGHT COMPLETED BOOKINGS WITH CREDIT OR DEBIT BALANCE
				if ( PMS_SPECIAL_HIGHLIGHT_NONE != m_nSpecialHighlightMode )
				{
					cr = PMSOptions.GetColourUnhighlightedBack();

					if ( PMS_BOOKING_STATUS_COMPLETE == CalendarBooking.m_nStatus )
					{
						int nBookingIdx;
						if ( DataManagerNonDb.PMSBooking.FindBookingById( CalendarBooking.m_nBookingId, nBookingIdx ) == TRUE )
						{
							CPMSBookingCSVRecord BookingRecord;
							DataManagerNonDb.PMSBooking.QuickGetAt( nBookingIdx, BookingRecord, FALSE );

							if ( ( BookingRecord.GetCheckoutBalanceType() & m_nSpecialHighlightMode ) != 0 )
								cr = PMSOptions.GetColourHighlightedBack();
						}
					}
				}

				switch( CalendarBooking.m_nStatus )
				{
				case PMS_BOOKING_STATUS_CLOSED:
					if ( nSelectedClosureId == CalendarBooking.m_nBookingId )
						cr = PMSOptions.GetColourSelectedBack();
					break;

				case PMS_BOOKING_STATUS_ACTIVE:
				case PMS_BOOKING_STATUS_COMPLETE:
				case PMS_BOOKING_STATUS_PENDING:
					if ( nSelectedBookingId == CalendarBooking.m_nBookingId )
						cr = PMSOptions.GetColourSelectedBack();
					break;
				}

				CRect rectBooking;
				GetBookingRect( nRoomOffset, nBookingOffsetStart, nBookingOffsetEnd, bFullWidthStart, bFullWidthEnd, rectBooking );
			
				CPMSBrushObject Brush( cr );
				pDCMem -> FillRect ( rectBooking, Brush.GetBrush() );
			}
		}

		//DRAW VERTICAL LINES BETWEEN DAYS
		{
			CPMSBrushObject Brush( crWhite );
			
			for ( int nColOffset = 0; nColOffset < m_CalendarHelpers.GetDisplayCols(); nColOffset++ )
			{
				CRect rectDivider;
				GetDividerRect( nRoomOffset, nColOffset, rectDivider );
				pDCMem -> FillRect ( rectDivider, Brush.GetBrush() );
			}
		}
	}

	m_nActiveRows = nRoomOffset;

	//MARK CURRENT DAY ON CALENDAR
	{
		CPMSBrushObject Brush( crBlack );

		int nTodayOffsetLeft = ( nDayToday - 1 ) - nStartCol;
		int nTodayOffsetRight = ( nDayToday + 1 ) - nStartCol;

		if ( ( nTodayOffsetLeft >= -1 ) && ( nTodayOffsetLeft < m_CalendarHelpers.GetDisplayCols() - 1 ) )
		{
			CRect rectToday;
			GetTodayMarkerRect( nRoomOffset, nTodayOffsetLeft, rectToday );
			pDCMem -> FillRect ( rectToday, Brush.GetBrush() );
		}

		if ( ( nTodayOffsetRight >= 0 ) && ( nTodayOffsetRight < m_CalendarHelpers.GetDisplayCols() ) )
		{
			CRect rectToday;
			GetTodayMarkerRect( nRoomOffset, nTodayOffsetRight, rectToday );
			pDCMem -> FillRect ( rectToday, Brush.GetBrush() );
		}
	}
	
    // END DRAW   -------------------------------------------------

    dc.BitBlt( 0, 0, rFrame.Width(), rFrame.Height(), pDCMem, 0, 0, SRCCOPY );

    pDCMem->SelectObject( pOldBitmap );

    delete pDCMem;
}

/**********************************************************************/

void CPMSStaticCalendar::GetWeekdayRect( int nDayOffset, CRect& rect, int nWidth )
{
	rect.left = CALENDAR_BAR_LEFT + ( CALENDAR_DAY_WIDTH * nDayOffset );
	rect.top = 0;
	rect.right = rect.left + ( CALENDAR_DAY_WIDTH * nWidth );
	rect.bottom = CALENDAR_BAR_TOP;
}

/**********************************************************************/

void CPMSStaticCalendar::GetBookingRect( int nRoomOffset, int nDayOffsetStart, int nDayOffsetEnd, bool bFullWidthStart, bool bFullWidthEnd, CRect& rect )
{
	rect.left = CALENDAR_BAR_LEFT + ( CALENDAR_DAY_WIDTH * nDayOffsetStart );
	rect.top = CALENDAR_BAR_TOP + ( nRoomOffset * CALENDAR_ROW_HEIGHT ) + CALENDAR_ROW_MARGIN;
	rect.right = rect.left + CALENDAR_DAY_WIDTH * ( nDayOffsetEnd - nDayOffsetStart );
	rect.bottom = rect.top + CALENDAR_ROW_HEIGHT - ( CALENDAR_ROW_MARGIN * 2 );

	if ( FALSE == bFullWidthStart )
		rect.left += 12;

	if ( FALSE == bFullWidthEnd )
		rect.right -= 12;
}

/**********************************************************************/

void CPMSStaticCalendar::GetDividerRect( int nRoomOffset, int nDayOffset, CRect& rect )
{
	rect.left = CALENDAR_BAR_LEFT + ( CALENDAR_DAY_WIDTH * nDayOffset ) + CALENDAR_DAY_WIDTH - 1;
	rect.top = CALENDAR_BAR_TOP + ( nRoomOffset * CALENDAR_ROW_HEIGHT ) + CALENDAR_ROW_MARGIN;
	rect.right = rect.left + 2;
	rect.bottom = rect.top + CALENDAR_ROW_HEIGHT - ( CALENDAR_ROW_MARGIN * 2 );
}

/**********************************************************************/

void CPMSStaticCalendar::GetTodayMarkerRect( int nRooms, int nDayOffset, CRect& rect )
{
	rect.left = CALENDAR_BAR_LEFT + ( CALENDAR_DAY_WIDTH * nDayOffset ) + CALENDAR_DAY_WIDTH - 1;
	rect.top = CALENDAR_BAR_TOP;
	rect.right = rect.left + 2;
	rect.bottom = rect.top + ( CALENDAR_ROW_HEIGHT * nRooms );
}

/**********************************************************************/

void CPMSStaticCalendar::GetRoomNameRect( int nRoomOffset, CRect& rect )
{
	rect.left = 20;
	rect.top = CALENDAR_BAR_TOP + ( nRoomOffset * CALENDAR_ROW_HEIGHT );
	rect.right = CALENDAR_BAR_LEFT - 10;
	rect.bottom = rect.top + CALENDAR_ROW_HEIGHT;
}

/**********************************************************************/

void CPMSStaticCalendar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ProcessMouseButton( nFlags, point, FALSE );
}

/**********************************************************************/

void CPMSStaticCalendar::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ProcessMouseButton( nFlags, point, TRUE );
}

/**********************************************************************/

void CPMSStaticCalendar::ProcessMouseButton(UINT nFlags, CPoint point, bool bRightButton ) 
{
	if ( m_pClickPoint != NULL )
		*m_pClickPoint = point;

	m_nCalendarClickRow = -1;
	m_nCalendarClickCol = -1;

	if ( point.y >= CALENDAR_BAR_TOP )
	{
		int nRow = ( point.y - CALENDAR_BAR_TOP ) / CALENDAR_ROW_HEIGHT;

		if  ( ( nRow >= 0 ) && ( nRow < m_nActiveRows ) )
		{
			int nRowStart = CALENDAR_BAR_TOP +( nRow * CALENDAR_ROW_HEIGHT );
			int nRowOffset = point.y - nRowStart;

			if ( ( nRowOffset >= CALENDAR_ROW_MARGIN ) && ( nRowOffset < CALENDAR_ROW_HEIGHT - CALENDAR_ROW_MARGIN ) )
				m_nCalendarClickRow = nRow;
		}
	}

	if ( point.x >= CALENDAR_BAR_LEFT )
	{
		int nCol = ( point.x - CALENDAR_BAR_LEFT ) / CALENDAR_DAY_WIDTH;

		if ( ( nCol >= 0 ) && ( nCol < m_CalendarHelpers.GetDisplayCols() ) )
			m_nCalendarClickCol = nCol;
	}

	if ( ( m_nCalendarClickCol > -1 ) && ( m_nCalendarClickRow > -1 ) )
	{
		if ( FALSE == bRightButton )
			GetParent() -> PostMessage( WM_APP + 2, 0, 0 );
		else
			GetParent() -> PostMessage( WM_APP + 3, 0, 0 );
	}
	else
		GetParent() -> PostMessage( WM_APP + 4, 0, 0 );
}

/**********************************************************************/

void CPMSStaticCalendar::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_nCalendarMoveRow = -1;
	m_nCalendarMoveCol = -1;

	if ( point.y >= CALENDAR_BAR_TOP )
	{
		int nRow = ( point.y - CALENDAR_BAR_TOP ) / CALENDAR_ROW_HEIGHT;

		if  ( ( nRow >= 0 ) && ( nRow < m_nActiveRows ) )
		{
			int nRowStart = CALENDAR_BAR_TOP +( nRow * CALENDAR_ROW_HEIGHT );
			int nRowOffset = point.y - nRowStart;

			if ( ( nRowOffset >= CALENDAR_ROW_MARGIN ) && ( nRowOffset < CALENDAR_ROW_HEIGHT - CALENDAR_ROW_MARGIN ) )
				m_nCalendarMoveRow = nRow;
		}
	}

	if ( point.x >= CALENDAR_BAR_LEFT )
	{
		int nCol = ( point.x - CALENDAR_BAR_LEFT ) / CALENDAR_DAY_WIDTH;

		if ( ( nCol >= 0 ) && ( nCol < m_CalendarHelpers.GetDisplayCols() ) )
			m_nCalendarMoveCol = nCol;
	}

	LPARAM lparam = ( ( point.y ) << 16 ) + point.x;
	GetParent() -> PostMessage( WM_APP + 5, 0, lparam );
}

/**********************************************************************/
#endif
/**********************************************************************/