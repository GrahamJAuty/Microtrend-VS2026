#pragma once
/**********************************************************************/
#include "EposReportCoversTimedGroup.h"
/**********************************************************************/

class CEposReportCoversHourlyGroup : public CEposReportCoversTimedGroup
{
public:
	CEposReportCoversHourlyGroup( CEposSelectArray& SelectArray, bool bDeptMode );
	CEposReportCoversHourlyGroup( CEposSelectArray& SelectArray, const char* szCustomSettings, bool bDeptMode );

private:
	void Init( bool bDeptMode );

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
