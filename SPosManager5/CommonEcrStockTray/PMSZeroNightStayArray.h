#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

class CPMSZeroNightStayArray
{
public:
	CPMSZeroNightStayArray(void);
	void RemoveAll();
	
public:
	void GetZeroNightStayArrayRange( int nRoomNo, int nStartDay, int nEndDay, int& nStartIdx, int& nEndIdx );
	void GetZeroNightStayArrayRoom( int nIdx, CString& str );
	void GetZeroNightStayArrayDay( int nIdx, CString& str );
	void GetZeroNightStayArrayIDs( int nIdx, CString& str );	
	void AddZeroNightStayEntry( int nRoomNo, int nDayNumber, int nId, int nPart );

private:
	CReportConsolidationArray<CSortedStringByString> m_arrayZeroNightStay;
};

/**********************************************************************/
extern CPMSZeroNightStayArray PMSZeroNightStayArray;
/**********************************************************************/
#endif
/**********************************************************************/
