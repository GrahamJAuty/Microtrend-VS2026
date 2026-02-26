#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEODDateTimeFilter
{
public:
	CEODDateTimeFilter( int nLocIdx );
	void Clear();
	
public:
	void CheckFile();
	bool GetFilterTimes( CString& strDateFrom, CString& strDateTo );
	bool CheckTime( CString& strDateTime );
	bool CheckTime( COleDateTime& dateTime );
	bool GetBusinessDay( CString& strDateTime, CSortedDateTimeItem& itemDay );
	bool GetBusinessDay( COleDateTime& oleDateTime, CSortedDateTimeItem& itemDay );

public:
	bool GetLastResult(){ return m_bLastResult; }
	CSortedDateTimeItem GetCheckDateFrom(){ return m_CheckDateFrom; }
	CSortedDateTimeItem GetCheckDateTo(){ return m_CheckDateTo; }
	CSortedDateTimeItem GetCheckTimeFrom(){ return m_CheckTimeFrom; }
	CSortedDateTimeItem GetCheckTimeTo(){ return m_CheckTimeTo; }

private:
	bool FormatEODSearchTime( CString& strDate, CString& strDateTime, bool bEnd );
	bool GetBusinessDay( CSortedDateTimeItem& itemSearch, CSortedDateTimeItem& itemDay );

private:
	CReportConsolidationArray<CSortedDateTimeItem> m_arrayEODTimes;
	
private:
	int m_nLocIdx;
	__int64 m_nFileSize;
	int m_nLineCount;
	bool m_bLocked;
	
private:
	bool m_bLastResult;
	CSortedDateTimeItem m_CheckDateFrom;
	CSortedDateTimeItem m_CheckDateTo;
	CSortedDateTimeItem m_CheckTimeFrom;
	CSortedDateTimeItem m_CheckTimeTo;
	
private:
	bool m_bGotBusinessDay;
	COleDateTime m_oleBusinessDay;
	CSortedDateTimeItem m_BusinessDayStart;
	CSortedDateTimeItem m_BusinessDayEnd;
	CSortedDateTimeItem m_BusinessDayCurrent;
	
private:
	CString m_strEODSearchTime;
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

class CEODDateTimeFilterArray
{
public:
	CEODDateTimeFilterArray();
	~CEODDateTimeFilterArray();
	void PrepareForUse();
	void ClearAfterUse();

public:
	void CheckFile( int nLocIdx );
	bool GetFilterTimes( int nLocIdx, CString& strDateFrom, CString& strDateTo );
	bool CheckTime( int nLocIdx, CString& strDateTime );
	bool CheckTime( int nLocIdx, COleDateTime& dateTime );
	bool GetBusinessDay( int nLocIdx, CString& strDateTime, CSortedDateTimeItem& itemDay );
	bool GetBusinessDay( int nLocIdx, COleDateTime& oleDateTime, CSortedDateTimeItem& itemDay );

public:
	bool GetLastResult( int nLocIdx );
	CSortedDateTimeItem GetCheckDateFrom( int nLocIdx );
	CSortedDateTimeItem GetCheckDateTo( int nLocIdx );
	CSortedDateTimeItem GetCheckTimeFrom( int nLocIdx );
	CSortedDateTimeItem GetCheckTimeTo( int nLocIdx );

private:
	bool CheckFilter( int nLocIdx );

private:
	CArray<CEODDateTimeFilter*,CEODDateTimeFilter*> m_arrayLocs;
};

/**********************************************************************/
