#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRowPurchaseControlStamp.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranStampManualEdit : public CSQLTranBase
{
public:
	CSQLTranStampManualEdit(int nAppNo);
	
public:
	void DoWork(CSQLRowAccountFull& RowAccount, CSQLRowPurchaseControlStamp& PurchaseControlStampOld, CSQLRowPurchaseControlStamp& PurchaseControlStampNew, CString strRuleName );

public:
	virtual void AfterTransaction();

private:
	int m_nAppNo;
};

/**********************************************************************/
