#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSCalendarHelpers.h"
/**********************************************************************/

class CPMSStaticDate : public CStatic
{
	DECLARE_DYNAMIC(CPMSStaticDate)

public:
	CPMSStaticDate( CPMSCalendarHelpers& Helpers );
	virtual ~CPMSStaticDate();

public:
	void SetClickPoint( CPoint* p ){ m_pClickPoint = p; }
	bool GetClickDateFlag(){ return m_bClickDate; }

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	void GetMonthNameRect( int nDayOffset, CRect& rect );
	void GetMonthDayRect( int nDayOffset, CRect& rect, int nWidth );

private:
	CPMSCalendarHelpers& m_CalendarHelpers;
	CPoint* m_pClickPoint;
	bool m_bClickDate;
};

/**********************************************************************/
#endif
/**********************************************************************/

