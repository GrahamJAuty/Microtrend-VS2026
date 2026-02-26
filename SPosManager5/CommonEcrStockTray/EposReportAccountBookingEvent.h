#pragma once
/**********************************************************************/
#include "EposReportAccountBooking.h"
/**********************************************************************/

class CEposReportAccountBookingEvent : public CEposReportAccountBooking
{
public:
	CEposReportAccountBookingEvent( CEposSelectArray& SelectArray );
	CEposReportAccountBookingEvent( CEposSelectArray& SelectArray, const char* szCustomParams );

public:
	void GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );		
	void ValidatePurchaseTime( int nLocIdx, CString& strPurchaseDate, CString& strPurchaseTime, COleDateTime& timeTransaction, COleDateTime& dateTransaction, bool& bValidPurchaseTime, bool& bValidEventTime );
};

/**********************************************************************/

