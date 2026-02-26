#pragma once
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/

class CStockMonitorPosTray
{
public:
	CStockMonitorPosTray(){};
	~CStockMonitorPosTray(){};

	//ACCESS THE STOCK DATABASE
	int GetSize() { return m_StockDatabase.GetSize(); }
	
	void GetAt( int nStockIdx, CStockCSVRecord& StockRecord ) { m_StockDatabase.GetAt( nStockIdx, StockRecord ); }
	const char* GetAt( int nStockIdx ) { return m_StockDatabase.GetAt ( nStockIdx ); }
	void SetAt( int nStockIdx, CStockCSVRecord& StockRecord ) { m_StockDatabase.SetAt( nStockIdx, StockRecord ); }
		
	bool FindStockItemByCode ( const char* szCode, int& nIndex ) { return m_StockDatabase.FindStockItemByCode ( szCode, nIndex ); }
	
private:
	CStockCSVArray m_StockDatabase;
	
private:
	friend class CDataManagerPosTray;
};

/**********************************************************************/
#endif
/**********************************************************************/
