#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLRowSetPebblePayment.h"
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSavePebblePayment : public CSQLTranBase
{
public:
	void DoWork(CSQLRowSetPebblePayment& RowPayment);
};

/**********************************************************************/