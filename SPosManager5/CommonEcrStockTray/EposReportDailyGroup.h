#pragma once
/**********************************************************************/
#include "EposReportCoversTimedGroup.h"
/**********************************************************************/

class CEposReportDailyGroup : public CEposReportCoversTimedGroup
{
public:
	CEposReportDailyGroup( CEposSelectArray& SelectArray, bool bDeptMode );

private:
	virtual bool GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel );
	virtual const char* GetDateOrTimeReport( CString& strLabel );
};

/**********************************************************************/
