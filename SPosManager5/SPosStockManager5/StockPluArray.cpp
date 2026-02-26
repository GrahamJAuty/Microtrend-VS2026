/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "StockPluArray.h"
/**********************************************************************/

CStockPluRecord::CStockPluRecord()
{
	m_strStockCode = "";
}

/**********************************************************************/

void CStockPluRecord::ConvertToCSV ( CCSV& csv )
{
	int nSize = m_nPluNoArray.GetSize();
	int nRecipeSize = m_nRecipePluNoArray.GetSize();

	csv.Add ( m_strStockCode );
	csv.Add ( nSize );

	for ( int nIdx = 0; nIdx < nSize; nIdx++ )
		csv.Add( m_nPluNoArray.GetAt( nIdx ) );

	csv.Add ( nRecipeSize );

	for ( int nIdx = 0; nIdx < nRecipeSize; nIdx++ )
		csv.Add( m_nRecipePluNoArray.GetAt( nIdx ) );
}

/**********************************************************************/

void CStockPluRecord::ConvertFromCSV ( CCSV& csv )
{
	m_nPluNoArray.RemoveAll();
	m_nRecipePluNoArray.RemoveAll();

	SetStockCode( csv.GetString ( 0 ) );
	int nCount = csv.GetInt ( 1 );
	
	int nOffset = 2;
	for ( int nIdx = 0; nIdx < nCount; nIdx++ )
		m_nPluNoArray.Add( csv.GetInt64( nOffset++ ) );

	int nRecipeCount = csv.GetInt ( nOffset++ );
	for ( int nIdx = 0; nIdx < nRecipeCount; nIdx++ )
		m_nRecipePluNoArray.Add( csv.GetInt64( nOffset++ ) );
}
	
/**********************************************************************/

__int64 CStockPluRecord::GetPluNo ( int nListPos )
{
	if ( nListPos >= 0 && nListPos < m_nPluNoArray.GetSize() )
		return m_nPluNoArray.GetAt ( nListPos );
	else
		return 0;
}

/**********************************************************************/

__int64 CStockPluRecord::GetRecipePluNo ( int nListPos )
{
	if ( nListPos >= 0 && nListPos < m_nRecipePluNoArray.GetSize() )
		return m_nRecipePluNoArray.GetAt ( nListPos );
	else
		return 0;
}

/**********************************************************************/

bool CStockPluRecord::AddPluNo ( __int64 nPluNo )
{
	for ( int nIdx = 0; nIdx < m_nPluNoArray.GetSize(); nIdx++ )
	{
		__int64 nArrayPluNo = m_nPluNoArray.GetAt ( nIdx );

		if ( nPluNo == nArrayPluNo )
			return FALSE;

		if ( nPluNo < nArrayPluNo )
		{
			m_nPluNoArray.InsertAt ( nIdx, nPluNo );
			return TRUE;
		}
	}
	m_nPluNoArray.Add ( nPluNo );
	return TRUE;
}

/**********************************************************************/

bool CStockPluRecord::AddRecipePluNo ( __int64 nPluNo )
{
	for ( int nIdx = 0; nIdx < m_nRecipePluNoArray.GetSize(); nIdx++ )
	{
		__int64 nArrayPluNo = m_nRecipePluNoArray.GetAt ( nIdx );

		if ( nPluNo == nArrayPluNo )
			return FALSE;

		if ( nPluNo < nArrayPluNo )
		{
			m_nRecipePluNoArray.InsertAt ( nIdx, nPluNo );
			return TRUE;
		}
	}
	m_nRecipePluNoArray.Add ( nPluNo );
	return TRUE;
}

/**********************************************************************/

bool CStockPluRecord::RemovePluNo ( __int64 nPluNo )
{
	for ( int nIdx = 0; nIdx < m_nPluNoArray.GetSize(); nIdx++ )
	{
		__int64 nArrayPluNo = m_nPluNoArray.GetAt ( nIdx );

		if ( nPluNo == nArrayPluNo )
		{
			m_nPluNoArray.RemoveAt ( nIdx );
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CStockPluRecord::RemoveRecipePluNo ( __int64 nPluNo )
{
	for ( int nIdx = 0; nIdx < m_nRecipePluNoArray.GetSize(); nIdx++ )
	{
		__int64 nArrayPluNo = m_nRecipePluNoArray.GetAt ( nIdx );

		if ( nPluNo == nArrayPluNo )
		{
			m_nRecipePluNoArray.RemoveAt ( nIdx );
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

CStockPluArray::CStockPluArray() : CCSVArray()
{
	m_bIsValid = FALSE;
	m_bHideProgress = FALSE;
}

/**********************************************************************/

bool CStockPluArray::FindStockCode ( const char* szStockCode, int& nIndex ) 
{
	Rebuild();

	int nStart = 0;
	int nEnd = GetSize() - 1;

	CString strStockCodeToFind = szStockCode;
	CStockPluRecord Record;

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

const char* CStockPluArray::GetStockCode ( int nIndex )
{
	Rebuild();
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

int CStockPluArray::GetPluNoCount ( int nIndex )
{
	Rebuild();
	CStockPluRecord Record;
	GetAt ( nIndex, Record );
	return Record.GetPluNoCount();
}

/**********************************************************************/

int CStockPluArray::GetRecipePluNoCount ( int nIndex )
{
	Rebuild();
	CStockPluRecord Record;
	GetAt ( nIndex, Record );
	return Record.GetRecipePluNoCount();
}

/**********************************************************************/

int CStockPluArray::GetTotalLinkCount ( int nIndex )
{
	Rebuild();
	CStockPluRecord Record;
	GetAt ( nIndex, Record );
	return Record.GetRecipePluNoCount() + Record.GetPluNoCount();
}

/**********************************************************************/

__int64 CStockPluArray::GetPluNo ( int nIndex, int nListPos )
{
	Rebuild();
	CStockPluRecord Record;
	GetAt ( nIndex, Record );
	return Record.GetPluNo ( nListPos );
}

/**********************************************************************/

__int64 CStockPluArray::GetRecipePluNo ( int nIndex, int nListPos )
{
	Rebuild();
	CStockPluRecord Record;
	GetAt ( nIndex, Record );
	return Record.GetRecipePluNo ( nListPos );
}

/**********************************************************************/

bool CStockPluArray::AddLink ( const char* szStockCode, __int64 nPluNo )
{
	Rebuild();

	int nIndex;
	if ( FindStockCode ( szStockCode, nIndex ) == TRUE )
	{
		CStockPluRecord Record;
		GetAt ( nIndex, Record );
		bool bResult = Record.AddPluNo ( nPluNo );

		if ( bResult == TRUE )
			SetAt ( nIndex, Record );

		return bResult;
	}
	else
	{
		CStockPluRecord Record;
		Record.SetStockCode ( szStockCode );
		Record.AddPluNo ( nPluNo );
		InsertAt ( nIndex, Record );
		return TRUE;
	}
}

/**********************************************************************/

bool CStockPluArray::AddRecipeLink ( const char* szStockCode, __int64 nPluNo )
{
	Rebuild();

	int nIndex;
	if ( FindStockCode ( szStockCode, nIndex ) == TRUE )
	{
		CStockPluRecord Record;
		GetAt ( nIndex, Record );
		bool bResult = Record.AddRecipePluNo ( nPluNo );

		if ( bResult == TRUE )
			SetAt ( nIndex, Record );

		return bResult;
	}
	else
	{
		CStockPluRecord Record;
		Record.SetStockCode ( szStockCode );
		Record.AddRecipePluNo ( nPluNo );
		InsertAt ( nIndex, Record );
		return TRUE;
	}
}

/**********************************************************************/

bool CStockPluArray::RemoveLink ( const char* szStockCode, __int64 nPluNo )
{
	Rebuild();

	int nIndex;
	if ( FindStockCode ( szStockCode, nIndex ) == FALSE )
		return FALSE;
	else
	{
		CStockPluRecord Record;
		GetAt ( nIndex, Record );
		bool bResult = Record.RemovePluNo ( nPluNo );

		if ( bResult == TRUE )
			SetAt ( nIndex, Record );

		return bResult;
	}
}

/**********************************************************************/

bool CStockPluArray::RemoveRecipeLink ( const char* szStockCode, __int64 nPluNo )
{
	Rebuild();

	int nIndex;
	if ( FindStockCode ( szStockCode, nIndex ) == FALSE )
		return FALSE;
	else
	{
		CStockPluRecord Record;
		GetAt ( nIndex, Record );
		bool bResult = Record.RemoveRecipePluNo ( nPluNo );

		if ( bResult == TRUE )
			SetAt ( nIndex, Record );

		return bResult;
	}
}

/**********************************************************************/

void CStockPluArray::RemoveAll()
{
	RemoveAt ( 0, GetSize() );
}

/**********************************************************************/

void CStockPluArray::Rebuild()
{
	if ( m_bIsValid == FALSE )
	{
		m_bIsValid = TRUE;
		if ( m_bHideProgress == FALSE ) StatusProgress.Lock( TRUE, "Creating PLU table" );
		RebuildInternal();
		if ( m_bHideProgress == FALSE ) StatusProgress.Unlock();
		m_bHideProgress = FALSE;
	}
}

/**********************************************************************/

void CStockPluArray::RebuildInternal()
{
	RemoveAll();
	int nSize = DataManager.Plu.GetSize();

	for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
	{
		if ( m_bHideProgress == FALSE ) StatusProgress.SetPos ( nPluIdx, nSize ); 

		CPluCSVRecord PluRecord;
		DataManager.Plu.GetAt ( nPluIdx, PluRecord );
		__int64 nPluNo = PluRecord.GetPluNoInt();
		
		CRecipeCSVRecord Recipe;
		DataManager.Plu.GetRecipe ( nPluIdx, nPluNo, Recipe );
		
		if ( Recipe.IsRecipe() == FALSE )
		{
			CString strStockCode;
			DataManager.Plu.GetPluStockCode ( PluRecord.GetPluNoInt(), strStockCode, FALSE );
			
			if ( strStockCode != "" )
				AddLink ( strStockCode, nPluNo );
		}
		else
		{
			for ( int nItem = 0; nItem < Recipe.GetSize(); nItem++ )
				AddRecipeLink ( Recipe.GetStockCode ( nItem ), nPluNo );
		}
	}
}

/**********************************************************************/

void CStockPluArray::MergePluNoArray( const char* szStockCode, CReportConsolidationArray<CSortedInt64Item>& arrayPluNo )
{
	int nStockPluIdx;
	CStockPluRecord StockPluRecord;
	if ( FindStockCode( szStockCode, nStockPluIdx ) == TRUE )
	{
		GetAt( nStockPluIdx, StockPluRecord );

		for ( int n = 0; n < StockPluRecord.GetPluNoCount(); n++ )
		{
			CSortedInt64Item item;
			item.m_nItem = StockPluRecord.GetPluNo(n);
			arrayPluNo.Consolidate( item );
		}

		for ( int n = 0; n < StockPluRecord.GetRecipePluNoCount(); n++ )
		{
			CSortedInt64Item item;
			item.m_nItem = StockPluRecord.GetRecipePluNo(n);
			arrayPluNo.Consolidate( item );
		}
	}
}
	
/**********************************************************************/
