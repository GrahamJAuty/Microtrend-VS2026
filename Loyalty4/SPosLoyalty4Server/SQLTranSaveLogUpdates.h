#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveLogUpdates : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount);
};

/**********************************************************************/
