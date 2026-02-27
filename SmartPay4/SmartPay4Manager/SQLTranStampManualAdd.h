#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRowPurchaseControlStamp.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranStampManualAdd : public CSQLTranBase
{
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CSQLRowPurchaseControlStamp& PurchaseControlStampNew, CString strRuleName );

public:
	virtual void AfterTransaction();
};

/**********************************************************************/
