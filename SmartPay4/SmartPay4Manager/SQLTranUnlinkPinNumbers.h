#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranUnlinkPINNumbers : public CSQLTranBase
{
public:
	void DoWork(int nGroupNo, __int64 nFrom, __int64 nTo);

public:
	int m_nRemoved = 0;
};

/**********************************************************************/
