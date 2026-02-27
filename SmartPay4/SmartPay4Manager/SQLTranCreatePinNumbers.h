#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranCreatePINNumbers : public CSQLTranBase
{
public:
	void DoWork(bool bDeleteExisting, bool bExcludeWeak);

private:
	bool WeakPIN(int nPINNumber);
};

/**********************************************************************/
