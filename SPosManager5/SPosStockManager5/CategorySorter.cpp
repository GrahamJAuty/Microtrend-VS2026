/**********************************************************************/
#include "CategorySorter.h"
/**********************************************************************/

CCategorySorter::CCategorySorter()
{
	for ( int n = 1; n <= MAX_CATEGORY; n++ )
		m_CategoryTable.Add( NULL );

	CCategorySetCSVRecord CategorySet;
	CategorySet.IncludeAllCategories( FALSE );
	Build( CategorySet );
}

/**********************************************************************/

CCategorySorter::CCategorySorter( CCategorySetCSVRecord& CategorySet ) 
{
	for ( int n = 1; n <= MAX_CATEGORY; n++ )
		m_CategoryTable.Add( NULL );

	Build ( CategorySet );
}

/**********************************************************************/

CCategorySorter::~CCategorySorter()
{
	Reset();
}

/**********************************************************************/

void CCategorySorter::Reset()
{
	for ( int n = 0; n < m_CategoryTable.GetSize(); n++ )
	{
		void* pTable = m_CategoryTable.GetAt(n);
		
		if ( pTable != NULL )
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			pArray -> RemoveAll();
			delete pArray;
			m_CategoryTable.SetAt( n, NULL );
		}
	}
}

/**********************************************************************/

void CCategorySorter::Add( int nCategory, int nStockIdx )
{
	if ( ( nCategory >= 1 ) && ( nCategory <= MAX_CATEGORY ) )
	{
		void* pTable = m_CategoryTable.GetAt( nCategory - 1 );

		if ( NULL == pTable )
		{
			CArray<int,int>* pArray = new CArray<int,int>;
			m_CategoryTable.SetAt( nCategory - 1, pArray );
			pArray -> Add( nStockIdx );
		}
		else
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			pArray -> Add ( nStockIdx );
		}
	}
}

/**********************************************************************/

void CCategorySorter::Build( CCategorySetCSVRecord& CategorySet )
{
	Reset();

	bool bGotWork = FALSE;

	for ( int n = 0; n < MAX_CATEGORY; n++ )
	{
		if ( CategorySet.GetCategoryStatus(n) == TRUE )
			bGotWork = TRUE;
	}

	if ( bGotWork == TRUE )
	{
		for ( int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++ )
		{
			int nCategory = DataManager.Stock.GetCategory ( nStockIdx );
			if ( CategorySet.GetCategoryStatus ( nCategory ) == TRUE )
				Add( nCategory, nStockIdx );
		}
	}
}

/**********************************************************************/

void CCategorySorter::Build( CReportFiltersStock& ReportFilters )
{
	Reset();

	bool bGotWork = FALSE;

	for ( int n = 0; n < MAX_CATEGORY; n++ )
	{
		if ( ReportFilters.CheckCategory(n) == TRUE )
			bGotWork = TRUE;
	}

	if ( bGotWork == TRUE )
	{
		for ( int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++ )
		{
			int nCategory = DataManager.Stock.GetCategory ( nStockIdx );
			if ( ReportFilters.CheckCategory( nCategory ) == TRUE )
				Add( nCategory, nStockIdx );
		}
	}
}

/**********************************************************************/

int CCategorySorter::GetItemCount ( int nCategory )
{
	int nResult = 0;

	if ( ( nCategory >= 1 ) && ( nCategory <= MAX_CATEGORY ) )
	{
		void* pTable = m_CategoryTable.GetAt( nCategory - 1 );

		if ( NULL != pTable )
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			nResult = pArray -> GetSize();
		}
	}

	return nResult;
}

/**********************************************************************/

int CCategorySorter::GetItemIndex ( int nCategory, int nIndex )
{
	int nResult = 0;

	if ( ( nCategory >= 1 ) && ( nCategory <= MAX_CATEGORY ) )
	{
		void* pTable = m_CategoryTable.GetAt( nCategory - 1 );

		if ( NULL != pTable )
		{
			CArray<int,int>* pArray = (CArray<int,int>*) pTable;
			
			if ( ( nIndex >= 0 ) && ( nIndex < pArray -> GetSize() ) )
				nResult = pArray -> GetAt( nIndex );
		}
	}

	return nResult;
}

/**********************************************************************/


