#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRowStaffGiftException.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranRetryStaffGift : public CSQLTranBase
{
public:
	void DoWork(CSQLRowStaffGiftException& RowSG, CSQLRowAccountFull& RowAccount);
};

/**********************************************************************/
