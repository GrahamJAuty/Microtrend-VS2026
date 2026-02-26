#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#define CALENDAR_BAR_TOP 20
#define CALENDAR_BAR_LEFT 200
#define CALENDAR_DAY_WIDTH 30
#define CALENDAR_ROW_HEIGHT 40
#define CALENDAR_ROW_MARGIN 4
/**********************************************************************/

class CPMSCalendarHelpers
{
public:
	CPMSCalendarHelpers();

public:
	static bool CheckReports();
	void SetMetrics( CRect& rect );

public:
	int GetVScrollBarX(){ return m_nVScrollBarX; }
	int GetVScrollBarY(){ return m_nVScrollBarY; }
	int GetVScrollBarCX(){ return m_nVScrollBarCX; }
	int GetVScrollBarCY(){ return m_nVScrollBarCY; }

	int GetHScrollBarX(){ return m_nHScrollBarX; }
	int GetHScrollBarY(){ return m_nHScrollBarY; }
	int GetHScrollBarCX(){ return m_nHScrollBarCX; }
	int GetHScrollBarCY(){ return m_nHScrollBarCY; }

	int GetDateX(){ return m_nDateX; }
	int GetDateY(){ return m_nDateY; }
	int GetDateCX(){ return m_nDateCX; }
	int GetDateCY(){ return m_nDateCY; }

	int GetCalendarX(){ return m_nCalendarX; }
	int GetCalendarY(){ return m_nCalendarY; }
	int GetCalendarCX(){ return m_nCalendarCX; }
	int GetCalendarCY(){ return m_nCalendarCY; }

	int GetDisplayRows(){ return m_nDisplayRows; }
	int GetDisplayCols(){ return m_nDisplayCols; }

	int GetVScrollBarMin(){ return m_nVScrollBarMin; }
	int GetVScrollBarMax(){ return m_nVScrollBarMax; }

	int GetCalendarStartCol(){ return m_nCalendarStartCol; }
	void SetCalendarStartCol( int n ){ m_nCalendarStartCol = n; }
	int GetCalendarStartRoom(){ return m_nCalendarStartRoom; }
	void SetCalendarStartRoom( int n ){ m_nCalendarStartRoom = n; }
	int GetSelectedBookingId(){ return m_nSelectedBookingId; }
	void SetSelectedBookingId( int n ){ m_nSelectedBookingId = n; }
	int GetSelectedClosureId(){ return m_nSelectedClosureId; }
	void SetSelectedClosureId( int n ){ m_nSelectedClosureId = n; }

	int GetActualDayNumberFromColumn( int nCol, int nDayToday ){ return ( nCol <= nDayToday ) ? nCol : nCol - 1; }
	int GetColumnFromDayNumber( int nDay, int nDayToday ){ return ( nDay < nDayToday ) ? nDay : nDay + 1; }
	int GetCalendarClickDay( int nOffset );

private:
	int m_nVScrollBarX;
	int m_nVScrollBarY;
	int m_nVScrollBarCX;
	int m_nVScrollBarCY;

	int m_nHScrollBarX;
	int m_nHScrollBarY;
	int m_nHScrollBarCX;
	int m_nHScrollBarCY;

	int m_nDateX;
	int m_nDateY;
	int m_nDateCX;
	int m_nDateCY;

	int m_nCalendarX;
	int m_nCalendarY;
	int m_nCalendarCX;
	int m_nCalendarCY;

	int m_nDisplayRows;
	int m_nDisplayCols;

	int m_nVScrollBarMin;
	int m_nVScrollBarMax;

	int m_nCalendarStartCol;
	int m_nCalendarStartRoom;
	int m_nSelectedBookingId;
	int m_nSelectedClosureId;
};

/**********************************************************************/
#endif
/**********************************************************************/
