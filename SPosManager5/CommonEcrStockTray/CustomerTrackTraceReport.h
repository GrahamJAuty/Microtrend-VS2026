/**********************************************************************/
#pragma once
/**********************************************************************/
#include "EposReportFile.h"
/**********************************************************************/
#define MINUTES_PER_DAY 1440
/**********************************************************************/

class CCustomerTrackTraceReport
{
public:
	CCustomerTrackTraceReport( int nLocIdx, int nCustIdx, int nBeforeThis, int nAfterThis, int nBeforeOther, int nAfterOther, int nDefaultVisit );

public:
	bool CreateReport();

private:
	void CreateReportInternal();

private:
	int m_nLocIdx;
	int m_nCustIdx;

private:
	CEposReportFile m_ReportFile;
	CString m_strNoCheckInTime;
	CString m_strBadCheckOutTime;
	int m_nBeforeThis;
	int m_nAfterThis;
	int m_nBeforeOther;
	int m_nAfterOther;
	int m_nDefaultVisit;

private:
	CString m_strDefaultVisitMarker;
};

/**********************************************************************/

