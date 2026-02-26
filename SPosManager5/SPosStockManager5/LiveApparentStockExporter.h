#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#pragma once
/**********************************************************************/

class CLiveApparentStockExporter
{
public:
	CLiveApparentStockExporter(void);

public:
	void CheckAllDatabases();
	void CheckDatabase( int nDbIdx );
};

/**********************************************************************/
#endif
/**********************************************************************/
