#pragma once
/**********************************************************************/
#include "SQLRowSetGroupFull.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveGroup : public CSQLTranBase
{
public:
	void DoWork(CSQLRowSetGroupFull& RowGroup);
};

/**********************************************************************/
