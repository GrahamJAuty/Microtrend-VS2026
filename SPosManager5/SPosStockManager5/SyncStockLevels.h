#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CSyncStockLevels
{
public:
	CSyncStockLevels(void);
	void CheckStockpoint();
};

extern CSyncStockLevels GlobalStockSync;
/**********************************************************************/
#endif
/**********************************************************************/
