#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPMSCheckoutDateArray
{
public:
	CPMSCheckoutDateArray(void);
	
public:
	void InvalidateCheckoutDateArray();
	void BuildCheckoutDateArray();
	
public:
	int GetActiveArraySize(){ return m_arrayActiveID.GetSize(); }
	int GetActiveID( int nIdx );

	int GetCompleteDateArraySize(){ return m_arrayCompleteDate.GetSize(); }
	void GetCompleteDateArrayRange( CString& strStartDate, CString& strEndDate, int& nStartIdx, int& nEndIdx );
	void GetCompleteDateArrayDate( int nIdx, CString& str );
	void GetCompleteDateArrayIDs( int nIdx, CString& str );	
	
private:
	void AddCompleteDateEntry( int nDayNumber, int nId );

private:
	CReportConsolidationArray<CSortedStringByString> m_arrayCompleteDate;
	CArray<int, int> m_arrayActiveID;
	bool m_bGotCheckoutDateArray;
};

/**********************************************************************/
extern CPMSCheckoutDateArray PMSCheckoutDateArray;
/**********************************************************************/
#endif
/**********************************************************************/
