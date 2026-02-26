#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSCalendarHelpers.h"
/**********************************************************************/
#define PMS_SPECIAL_HIGHLIGHT_NONE 0
#define PMS_SPECIAL_HIGHLIGHT_DEBIT 1
#define PMS_SPECIAL_HIGHLIGHT_CREDIT 2
#define PMS_SPECIAL_HIGHLIGHT_BOTH 3
/**********************************************************************/

class CPMSStaticCalendar : public CStatic
{
	DECLARE_DYNAMIC(CPMSStaticCalendar)

public:
	CPMSStaticCalendar( CPMSCalendarHelpers& Helpers );
	virtual ~CPMSStaticCalendar();

public:
	void SetClickPoint( CPoint* p ){ m_pClickPoint = p; }
	int GetCalendarClickRow(){ return m_nCalendarClickRow; }
	int GetCalendarClickCol(){ return m_nCalendarClickCol; }
	int GetCalendarMoveRow(){ return m_nCalendarMoveRow; }
	int GetCalendarMoveCol(){ return m_nCalendarMoveCol; }

public:
	int GetSpecialHighlightMode(){ return m_nSpecialHighlightMode; }
	void SetSpecialHighlightMode( int n );

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	void GetWeekdayRect( int nDayOffset, CRect& rect, int nWidth );
	void GetBookingRect( int nRoomOffset, int nDayOffsetStart, int nDayOffsetEnd, bool bFullWidthStart, bool bFullWidthEnd, CRect& rect );
	void GetDividerRect( int nRoomOffset, int nDayOffset, CRect& rect );
	void GetTodayMarkerRect( int nRooms, int nDayOffset, CRect& rect );
	void GetRoomNameRect( int nRoomOffset, CRect& rect );
	void ProcessMouseButton( UINT nFlags, CPoint point, bool bRightButton );

private:
	CPMSCalendarHelpers& m_CalendarHelpers;
	CPoint* m_pClickPoint;
	int m_nSpecialHighlightMode;
	int m_nCalendarClickRow;
	int m_nCalendarClickCol;
	int m_nCalendarMoveRow;
	int m_nCalendarMoveCol;
	int m_nActiveRows;
};

/**********************************************************************/
#endif
/**********************************************************************/

