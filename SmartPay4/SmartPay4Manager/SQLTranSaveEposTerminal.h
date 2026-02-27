#pragma once
/**********************************************************************/
#include "SQLRowSetEposTerminal.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveEposTerminal : public CSQLTranBase
{
public:
	void DoWork(CSQLRowSetEposTerminal& RowSetEP, CArray<int,int>& arrayDeletedEP);
};

/**********************************************************************/
