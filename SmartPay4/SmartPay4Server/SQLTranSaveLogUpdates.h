#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRowEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRowPebbleExportPayment.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveLogUpdates : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CPendingExportPaymentData& PendingExportPaymentData, CPendingEposTerminalData& PendingEposTerminalData);
};

/**********************************************************************/
