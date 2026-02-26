#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeCSVArray.h"
/**********************************************************************/

class CPluChangesBuffer  
{
public:
	CPluChangesBuffer() { m_strPluNo = ""; }
	void SetPluNo( const char* szPluNo );
	void LogStockChange ( const char* szStockCode );
	void LogBarcodeChange ( const char* szBarcode );
	void UndoChanges();

private:
	CString m_strPluNo;
	CBarcodeCSVArray m_BarcodeBuffer;
	CStringArray m_NewBarcodes;

#ifdef STOCKMAN_SYSTEM
	CStockCSVArray m_StockBuffer;
	CStringArray m_NewStockCodes;
#endif
};

/**********************************************************************/

