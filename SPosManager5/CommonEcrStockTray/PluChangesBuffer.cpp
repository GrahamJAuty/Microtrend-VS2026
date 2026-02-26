/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "PluChangesBuffer.h"
/**********************************************************************/

void CPluChangesBuffer::SetPluNo( const char* szPluNo )
{
	m_strPluNo = szPluNo;

	while ( m_BarcodeBuffer.GetSize() > 0 )
		m_BarcodeBuffer.RemoveAt(0);

	m_NewBarcodes.RemoveAll();

#ifdef STOCKMAN_SYSTEM
	while ( m_StockBuffer.GetSize() > 0 )
		m_StockBuffer.RemoveAt(0);

	m_NewStockCodes.RemoveAll();
#endif
}
 
/**********************************************************************/

void CPluChangesBuffer::LogStockChange ( const char* szStockCode )
{
#ifdef STOCKMAN_SYSTEM
	CString strStockCode = szStockCode;

	//NOTHING TO DO IF ALREADY LOGGED AS A NEW STOCK CODE
	for ( int nIndex = 0; nIndex < m_NewStockCodes.GetSize(); nIndex++ )
		if ( strStockCode == m_NewStockCodes.GetAt ( nIndex ) )
			return;
	
	//FIND OUT IF THE STOCK CODE IS DEFINED IN THE DATABASE
	int nStockIdx;
	if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
	{	
		//LOG THE STOCK ITEM AS CHANGED IF IT IS NOT ALREADY LOGGED
		int nSavedStockIdx;
		if ( m_StockBuffer.FindStockItemByCode ( strStockCode, nSavedStockIdx ) == FALSE )
		{
			CStockCSVRecord OldStockRecord;
			DataManager.Stock.GetAt ( nStockIdx, OldStockRecord );
			m_StockBuffer.InsertAt ( nSavedStockIdx, OldStockRecord );
		}
	}
	else
		m_NewStockCodes.Add ( strStockCode );
#endif
}

/**********************************************************************/

void CPluChangesBuffer::LogBarcodeChange ( const char* szBarcode )
{
	CString strBarcode = szBarcode;

	//NOTHING TO DO IF ALREADY LOGGED AS A NEW BARCODE
	for ( int nIndex = 0; nIndex < m_NewBarcodes.GetSize(); nIndex++ )
		if ( strBarcode == m_NewBarcodes.GetAt ( nIndex ) )
			return;
	
	//FIND OUT IF THE BARCODE IS DEFINED IN THE DATABASE
	int nBarcodeIdx;
	if ( DataManager.Barcode.FindBarcodeByNumber ( _atoi64 ( strBarcode ), nBarcodeIdx ) == TRUE )
	{	
		//LOG THE BARCODE ITEM AS CHANGED IF IT IS NOT ALREADY LOGGED
		int nSavedBarcodeIdx;
		if ( m_BarcodeBuffer.FindBarcodeByNumber ( _atoi64 ( strBarcode ), nSavedBarcodeIdx ) == FALSE )
		{
			CBarcodeCSVRecord OldBarcodeRecord;
			DataManager.Barcode.GetAt ( nBarcodeIdx, OldBarcodeRecord );
			m_BarcodeBuffer.InsertAt ( nSavedBarcodeIdx, OldBarcodeRecord );
		}
	}
	else
		m_NewBarcodes.Add ( strBarcode );
}

/**********************************************************************/

void CPluChangesBuffer::UndoChanges()
{
	bool bInvalidatePluBarcodeList = FALSE;

	for ( int nIndex = 0; nIndex < m_NewBarcodes.GetSize(); nIndex++ )
	{
		CString strBarcode = m_NewBarcodes.GetAt ( nIndex );
		
		int nBarcodeIdx;
		if ( DataManager.Barcode.FindBarcodeByNumber ( _atoi64 ( strBarcode ), nBarcodeIdx ) == TRUE )
		{
			DataManager.Barcode.RemoveAt ( nBarcodeIdx );
			bInvalidatePluBarcodeList = TRUE;
		}
	}

	for ( int nIndex = 0; nIndex < m_BarcodeBuffer.GetSize(); nIndex++ )
	{
		CBarcodeCSVRecord BarcodeRecord;
		m_BarcodeBuffer.GetAt ( nIndex, BarcodeRecord );

		__int64 nBarcodeNo = BarcodeRecord.GetBarcodeNoInt();

		int nBarcodeIdx;
		if ( DataManager.Barcode.FindBarcodeByNumber ( nBarcodeNo, nBarcodeIdx ) == TRUE )
			DataManager.Barcode.SetAt ( nBarcodeIdx, BarcodeRecord );
		else
			DataManager.Barcode.InsertAt( nBarcodeIdx, BarcodeRecord );

		bInvalidatePluBarcodeList = TRUE;
	} 

	if ( bInvalidatePluBarcodeList )
		DataManager.Barcode.InvalidatePluBarcodeList();

#ifdef STOCKMAN_SYSTEM
	for ( int nIndex = 0; nIndex < m_NewStockCodes.GetSize(); nIndex++ )
	{
		CString strStockCode = m_NewStockCodes.GetAt ( nIndex );
		
		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
			DataManager.Stock.RemoveAt ( nStockIdx );
	}

	for ( int nIndex = 0; nIndex < m_StockBuffer.GetSize(); nIndex++ )
	{
		CStockCSVRecord StockRecord;
		m_StockBuffer.GetAt ( nIndex, StockRecord );

		CString strStockCode = StockRecord.GetStockCode();

		int nStockIdx;
		if ( DataManager.Stock.FindStockItemByCode ( strStockCode, nStockIdx ) == TRUE )
			DataManager.Stock.SetAt ( nStockIdx, StockRecord );
	} 
#endif
}

/**********************************************************************/
