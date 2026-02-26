#pragma once
/**********************************************************************/
#include "SQLRowSetStaffCard.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveStaffCard : public CSQLTranBase
{
public:
	void DoWork(CSQLRowSetStaffCard& RowGroup);
};

/**********************************************************************/
