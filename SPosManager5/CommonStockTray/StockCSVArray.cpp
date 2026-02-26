/**********************************************************************/
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/

CStockCSVArray::CStockCSVArray() : CSharedCSVArray( ',', '"', FALSE, TRUE, TRUE )
{
}

/**********************************************************************/

bool CStockCSVArray::FindStockItemByCode ( const char* szCode, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CString strStockCodeToFind = szCode;
	CStockCSVRecord Record;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		CString strStockCode = GetStockCode ( nIndex );
		
		if ( strStockCode < strStockCodeToFind )
			nStart = nIndex + 1;

		if ( strStockCode > strStockCodeToFind )
			nEnd = nIndex - 1;

		if ( strStockCode == strStockCodeToFind )
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

const char* CStockCSVArray::GetStockCode ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strStockCode = csv.GetString ( 0 );
		return m_strStockCode;
	}
	return "";
}

/**********************************************************************/

const char* CStockCSVArray::GetDescription ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		m_strDescription = csv.GetString ( 1 );
		return m_strDescription;
	}
	return "";
}

/**********************************************************************/

int CStockCSVArray::GetCategory ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 3 );
		return csv.GetInt ( 2 );
	}
	return -1;
}

/**********************************************************************/

int CStockCSVArray::ImportLine ( int nImportMethod, CSSImportFile& import, const char* szImportLine, int nSupplierCount, CSupplierCSVArray& SupplierArray, CPtrArray& CustomFieldArrays, int nUserMaxSuppliers )
{
	CCSV csvImport ( szImportLine );
	CString strStockCode = csvImport.GetString(0);
	
	strStockCode = strStockCode.Left ( Stock::StockCode.Max );
	::TrimSpaces ( strStockCode, FALSE );
	
	if ( strStockCode == "" )
		return IMPORT_LINE_IGNORED;

	int nStockIdx;
	bool bFound = FindStockItemByCode ( strStockCode, nStockIdx );

	switch ( nImportMethod )
	{
	case IMPORT_METHOD_ALL:		break;
	case IMPORT_METHOD_EXIST:	if ( bFound == FALSE )	return IMPORT_LINE_IGNORED;	break;
	case IMPORT_METHOD_NEW:		if ( bFound == TRUE )	return IMPORT_LINE_IGNORED;	break;
	}

	if ( bFound == TRUE )	
	{
		CStockCSVRecord NewRecord;
		GetAt ( nStockIdx, NewRecord );
		NewRecord.ImportLine ( import, csvImport, nSupplierCount, SupplierArray, CustomFieldArrays );
		NewRecord.TidySuppliers( nUserMaxSuppliers );
		SetAt ( nStockIdx, NewRecord );
		return IMPORT_LINE_MODIFIED;
	}	
	else			
	{	
		CStockCSVRecord NewRecord;
		NewRecord.ImportLine ( import, csvImport, nSupplierCount, SupplierArray, CustomFieldArrays );
		NewRecord.TidySuppliers( nUserMaxSuppliers );		
		InsertAt ( nStockIdx, NewRecord );
		return IMPORT_LINE_ADDED;
	}
}

/**********************************************************************/

int CStockCSVArray::PurgeSupplier ( int nSuppNo, int nUserMaxSuppliers )
{
	int nResult = PurgeSupplierInternal ( nSuppNo, nUserMaxSuppliers );
	return nResult;
}

/**********************************************************************/

int CStockCSVArray::PurgeSupplierInternal ( int nSuppNo, int nUserMaxSuppliers )
{
	int nResult = SUPPLIER_PURGE_NOCHANGE;	
	int nSize = GetSize();
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{
		CStockCSVRecord Record;
		GetAt ( nIndex, Record );
		
		int nRecordResult = Record.PurgeSupplier ( nSuppNo, nUserMaxSuppliers );

		if ( nRecordResult != SUPPLIER_PURGE_NOCHANGE )
		{
			if ( nRecordResult == SUPPLIER_PURGE_CHANGESTOCK && nResult == SUPPLIER_PURGE_NOCHANGE )
				nResult = SUPPLIER_PURGE_CHANGESTOCK;

			if ( nRecordResult == SUPPLIER_PURGE_CHANGEPLU )
				nResult = SUPPLIER_PURGE_CHANGEPLU;

			SetAt ( nIndex, Record );
		}
	}
	return nResult;
}

/**********************************************************************/
