/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSStaticDate.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPMSStaticDate, CStatic)
/**********************************************************************/

CPMSStaticDate::CPMSStaticDate( CPMSCalendarHelpers& Helpers ) :
		m_CalendarHelpers( Helpers )
{
	m_pClickPoint = NULL;
	m_bClickDate = FALSE;
}

/**********************************************************************/

CPMSStaticDate::~CPMSStaticDate()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPMSStaticDate, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/**********************************************************************/

void CPMSStaticDate::OnPaint()
{
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

	COLORREF crRed = COLORREF( 0x0000CC );
	COLORREF crBlue = COLORREF (0xFF0000 );

	int nStartCol = m_CalendarHelpers.GetCalendarStartCol();
	int nDayToday = DataManagerNonDb.PMSBooking.GetCurrentDayNumber();
	
	int nDayWidth = 1;
	int nColsToDraw = m_CalendarHelpers.GetDisplayCols();
	for ( int nOffset = 0; nOffset < nColsToDraw; nOffset += nDayWidth )
	{
		int nDayNumber = m_CalendarHelpers.GetActualDayNumberFromColumn( nStartCol + nOffset, nDayToday );
		
		if ( ( nStartCol + nOffset != nDayToday ) || ( nOffset >= nColsToDraw - 1 ) )
			nDayWidth = 1;
		else
			nDayWidth = 2;

		COleDateTime dateStart;
		GetOleDateSince2011( nDayNumber, dateStart );

		int nMDay = dateStart.GetDay();
		CString strMDay;
		strMDay.Format( "%d", nMDay );

		CStringArray arrayTexts;
		arrayTexts.Add( strMDay );

		COLORREF crText;
		switch( dateStart.GetDayOfWeek() )
		{
		case 1:		
		case 7:
			crText = crRed; 
			break;

		default:
			crText = crBlue;
			break;
		}

		CRect rectDate;
		GetMonthDayRect( nOffset, rectDate, nDayWidth );
		DrawTextInDeviceContext( pDCMem, arrayTexts, rectDate, TA_CENTER, 10, crText );

		if ( ( nMDay % 10 ) == 3 )
		{
			CString strMonth;
			strMonth = dateStart.Format( "%B %Y" );

			arrayTexts.RemoveAll();
			arrayTexts.Add( strMonth );

			CRect rectMonth;
			GetMonthNameRect( nOffset, rectMonth );
			DrawTextInDeviceContext( pDCMem, arrayTexts, rectMonth, TA_LEFT, 10, crBlue );
		}
	}

	CStringArray arrayTexts;
	arrayTexts.Add( "Today" );
	CRect rectToday( 10, 10, 60, 30 );
	DrawTextInDeviceContext( pDCMem, arrayTexts, rectToday, TA_CENTER, 10, crRed );

    // END DRAW   -------------------------------------------------


    dc.BitBlt( 0, 0, rFrame.Width(), rFrame.Height(), pDCMem, 0, 0, SRCCOPY );

    pDCMem->SelectObject( pOldBitmap );

    delete pDCMem;
}

/**********************************************************************/

void CPMSStaticDate::GetMonthNameRect( int nDayOffset, CRect& rect )
{
	rect.left = CALENDAR_BAR_LEFT + ( CALENDAR_DAY_WIDTH * nDayOffset );
	rect.top = 0;
	rect.right = rect.left + ( CALENDAR_DAY_WIDTH * 10 );
	rect.bottom = 24;
}

/**********************************************************************/

void CPMSStaticDate::GetMonthDayRect( int nDayOffset, CRect& rect, int nWidth )
{
	rect.left = CALENDAR_BAR_LEFT + ( CALENDAR_DAY_WIDTH * nDayOffset );
	rect.top = 25;
	rect.right = rect.left + ( CALENDAR_DAY_WIDTH * nWidth );
	rect.bottom = 49;
}

/**********************************************************************/

void CPMSStaticDate::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ( m_pClickPoint != NULL )
		*m_pClickPoint = point;

	m_bClickDate = ( point.x >= 17 ) && ( point.x <= 51 ) && ( point.y >= 16 ) && ( point.y <= 26 );

	GetParent() -> PostMessage( WM_APP + 1, 0, 0 );
}

/**********************************************************************/
#endif
/**********************************************************************/
