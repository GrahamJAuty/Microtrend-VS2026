/**********************************************************************/
 
/**********************************************************************/
#include "StockLevelsCSVArray.h"
/**********************************************************************/

CStockLevelsCSVArray::CStockLevelsCSVArray( int nSpNo ) : CSharedCSVArray( ',', '"', FALSE, TRUE, TRUE )
{
	m_nSpNo = nSpNo;
	m_bInSync = TRUE;
}

/**********************************************************************/

CStockLevelsCSVArray::~CStockLevelsCSVArray()
{
}

/**********************************************************************/

void CStockLevelsCSVArray::RemoveAll()
{
	for ( int nIndex = GetSize() - 1; nIndex >= 0; nIndex-- )
		RemoveAt ( nIndex );
}

/**********************************************************************/

const char* CStockLevelsCSVArray::GetStockCode ( int nIndex )
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

bool CStockLevelsCSVArray::FindStockItemByCode ( int nMasterIdx, const char* szCode, int& nIndex )
{
	CString strStockCodeToFind = szCode;
	
	if ( ( nMasterIdx >= 0 ) && ( nMasterIdx < GetSize() ) && ( strStockCodeToFind == GetStockCode ( nMasterIdx ) ) )
	{
		nIndex = nMasterIdx;
		return TRUE;
	}
	else if ( nMasterIdx != -1 )
		m_bInSync = FALSE;

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
