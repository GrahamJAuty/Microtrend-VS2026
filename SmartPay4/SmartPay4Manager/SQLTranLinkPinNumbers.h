#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranLinkPINNumbers : public CSQLTranBase
{
public:
	void DoWork(bool bKeep, int nGroup, __int64 nFrom, __int64 nTo);

private:
	bool WeakPIN(int nPINNumber);

public:
	int m_nAdded = 0;
	int m_nRetained = 0;
};

/**********************************************************************/
