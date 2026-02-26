#pragma once
/**********************************************************************/
#include "EposReportTimed.h"
/**********************************************************************/

class CEposReportHourly : public CEposReportTimed
{
public:
	CEposReportHourly( CEposSelectArray& SelectArray );
	CEposReportHourly( CEposSelectArray& SelectArray, const char* szCustomSettings );

private:
	void Init();

private:
	virtual bool GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel );
	virtual const char* GetDateOrTimeReport( CString& strLabel );

private:
	CTimeMap m_TimeMap;
	int m_nTimeSliceType;
	CString m_strSODLabel;

private:
	CEposReportCustomSettingsHourly m_ReportSettings;
};

/**********************************************************************/
