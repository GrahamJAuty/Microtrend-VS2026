#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRowExternalAccount.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveAccount : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternalOld, CSQLRowExternalAccount& RowExternalNew, CString strNewPINNumber);
};

/**********************************************************************/
