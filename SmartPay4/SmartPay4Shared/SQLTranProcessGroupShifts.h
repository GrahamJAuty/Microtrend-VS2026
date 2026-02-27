#pragma once
/**********************************************************************/
#include "SQLTranBase.h"
/**********************************************************************/

class CSQLTranProcessGroupShifts : public CSQLTranBase
{
public:
	void DoWork(COleDateTime& oleDateToday);
};

/**********************************************************************/
