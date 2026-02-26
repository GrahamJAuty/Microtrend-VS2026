#pragma once
/**********************************************************************/
#include "EposReportTimed.h"
/**********************************************************************/

class CEposReportDaily : public CEposReportTimed
{
public:
	CEposReportDaily( CEposSelectArray& SelectArray );

private:
	virtual bool GetDateOrTimeLabel( int nLocIdx, CString& strThisSaleDate, CString& strThisSaleTime, CString& strLabel );
	virtual const char* GetDateOrTimeReport( CString& strLabel );
};

/**********************************************************************/
