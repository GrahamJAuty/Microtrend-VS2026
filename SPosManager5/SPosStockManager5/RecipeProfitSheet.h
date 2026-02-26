#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/

class CRecipeProfitSheet  
{
public:
	CRecipeProfitSheet();
	int CreateProfitSheet( int nStockIdx );

private:
	void BreakdownPluNo( CReportFile& ReportFile, __int64 nPluNo );
};

/**********************************************************************/
#endif
/**********************************************************************/
