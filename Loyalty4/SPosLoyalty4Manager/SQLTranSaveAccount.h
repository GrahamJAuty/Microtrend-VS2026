#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRowExternalAccount.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveAccount : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternalOld, CSQLRowExternalAccount& RowExternalNew);
};

/**********************************************************************/
