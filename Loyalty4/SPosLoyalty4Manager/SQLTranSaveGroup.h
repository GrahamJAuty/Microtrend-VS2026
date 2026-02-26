#pragma once
/**********************************************************************/
#include "SQLRowSetGroup.h"
#include "SQLRowSetGroupTopupBonus.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranSaveGroup : public CSQLTranBase
{
public:
	void DoWork(CSQLRowSetGroup& RowSetGroup, CSQLRowSetGroupTopupBonus& RowSetGroupTopupBonus);
};

/**********************************************************************/
