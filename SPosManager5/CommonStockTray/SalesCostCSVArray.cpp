/**********************************************************************/
#include "..\CommonStockTray\SalesCostCSVArray.h"
/**********************************************************************/

CSalesCostCSVArray::CSalesCostCSVArray() : CSharedCSVArray( ',', '"', FALSE, TRUE, TRUE )
{
}

/**********************************************************************/

CSalesCostCSVArray::~CSalesCostCSVArray()
{
}

/**********************************************************************/

void CSalesCostCSVArray::RemoveAll()
{
	for ( int nIndex = GetSize() - 1; nIndex >= 0; nIndex-- )
		RemoveAt ( nIndex );
}

/**********************************************************************/

const char* CSalesCostCSVArray::GetStockCode ( int nIndex )
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

bool CSalesCostCSVArray::FindStockItemByCode ( const char* szCode, int& nIndex )
{
	CString strStockCodeToFind = szCode;
	
	int nStart = 0;
	int nEnd = GetSize() - 1;

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