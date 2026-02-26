#pragma once
/**********************************************************************/
#include "EposReportAccountBooking.h"
/**********************************************************************/

class CEposReportAccountBookingPurchase : public CEposReportAccountBooking
{
public:
	CEposReportAccountBookingPurchase( CEposSelectArray& SelectArray );
	CEposReportAccountBookingPurchase( CEposSelectArray& SelectArray, const char* szCustomParams );

public:
	void GetConsolidationFileList( CReportConsolidationArray<CTermFileListInfo>& arrayFileInfo, int& nTotalFileSize );
};

/**********************************************************************/

