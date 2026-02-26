#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "PluMonitorStockman.h"
#include "StockPluArray.h"
/**********************************************************************/

class CStockChangesBuffer  
{
public:
	CStockChangesBuffer();
	
	void SetStockCode( const char* szStockCode, bool bStockPlu );
	void LogPluChange ( __int64 nPluNumber );
	void LogStockPluChange ( const char* szStockCode );
	void UndoChanges();
	bool CheckModified();
	
private:
	int m_nStockIdx;
	CStockCSVRecord m_StockRecord;
	CString m_strStockCode;
	CPluMonitorStockman m_PluBuffer;
	CArray<__int64,__int64> m_NewPluNumbers;
	CStockPluArray m_StockPluBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
