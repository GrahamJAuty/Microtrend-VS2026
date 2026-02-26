#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRowOfferStamp.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranStampManualAdd : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CSQLRowOfferStamp& OfferStampNew, CString strOfferName );

public:
	virtual void AfterTransaction();
};

/**********************************************************************/
