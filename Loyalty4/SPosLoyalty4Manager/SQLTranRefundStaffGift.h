#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRowStaffGiftException.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranRefundStaffGift : public CSQLTranBase
{
public:
	void DoWork(CSQLRowStaffGiftException& RowSG);
};

/**********************************************************************/
