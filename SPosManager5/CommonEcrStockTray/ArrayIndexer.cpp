/**********************************************************************/
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "ArrayIndexer.h"
/**********************************************************************/

CArrayIndexer::CArrayIndexer()
{
	m_bNativeOrder = TRUE;
}

/**********************************************************************/

int CArrayIndexer::GetSortedIdxForNativeIdx(int nIdx)
{
	if (TRUE == m_bNativeOrder)
	{
		return nIdx;
	}
	else if ((nIdx >= 0) && (nIdx < m_arrayNativeToSorted.GetSize()))
	{
		return m_arrayNativeToSorted[nIdx];
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

int CArrayIndexer::GetNativeIdxForSortedIdx(int nIdx)
{
	if (TRUE == m_bNativeOrder)
	{
		return nIdx;
	}
	else if ((nIdx >= 0) && (nIdx < m_arraySortedToNative.GetSize()))
	{
		return m_arraySortedToNative[nIdx];
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

CStockArrayIndexer::CStockArrayIndexer() : CArrayIndexer()
{
	m_nCurrentDbIdx = -1;
}

/**********************************************************************/

bool CStockArrayIndexer::RebuildIndex(int nDbIdx)
{
	if (nDbIdx == m_nCurrentDbIdx)
	{
		return FALSE;
	}

	m_arrayNativeToSorted.RemoveAll();
	m_arraySortedToNative.RemoveAll();

	if ((nDbIdx < 0) || (nDbIdx >= dbDatabase.GetSize()))
	{
		m_nCurrentDbIdx = -1;
		m_bNativeOrder = TRUE;
		return TRUE;
	}

	CReportConsolidationArray<CSortedStringAndIntItem> arrayTemp;

	for (int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++)
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt(nStockIdx, StockRecord);

		CSortedStringAndIntItem item;
		item.m_strItem = StockRecord.GetDescription();
		item.m_strItem.MakeUpper();
		item.m_nItem = nStockIdx;
		arrayTemp.Consolidate(item);
	}

	if (arrayTemp.GetSize() != DataManager.Stock.GetSize())
	{
		m_nCurrentDbIdx = -1;
		m_bNativeOrder = TRUE;
		return TRUE;
	}

	for (int n = 0; n < arrayTemp.GetSize(); n++)
	{
		CSortedStringAndIntItem item;
		arrayTemp.GetAt(n, item);
		m_arraySortedToNative.Add(item.m_nItem);
		m_arrayNativeToSorted.Add(0);
	}

	for (int n = 0; n < m_arraySortedToNative.GetSize(); n++)
	{
		m_arrayNativeToSorted[m_arraySortedToNative[n]] = n;
	}

	m_nCurrentDbIdx = nDbIdx;

	return TRUE;
}

/**********************************************************************/

bool CStockArrayIndexer::RebuildIndex( CStockFilterArray* pFilter )
{
	m_arrayNativeToSorted.RemoveAll();
	m_arraySortedToNative.RemoveAll();

	for (int n = 0; n < DataManager.Stock.GetSize(); n++)
	{
		m_arrayNativeToSorted.Add(-1);
	}

	for ( int n = 0; n < pFilter -> GetSize(); n++ )
	{
		int nStockIdx = pFilter -> GetStockItemIndexAt(n);
		if ( ( nStockIdx >= 0 ) && ( nStockIdx < m_arrayNativeToSorted.GetSize() ) )
		{
			m_arraySortedToNative.Add( nStockIdx );
			m_arrayNativeToSorted.SetAt( nStockIdx, m_arraySortedToNative.GetSize() - 1 );
		}
	}

	return TRUE;
}

/**********************************************************************/
#endif
/**********************************************************************/
