/**********************************************************************/
#include "SalesHistoryRegionLocColMap.h"
/**********************************************************************/

CSalesHistoryRegionLocColMap::CSalesHistoryRegionLocColMap()
{
	ResetLastLocIdx();
}

/**********************************************************************/

void CSalesHistoryRegionLocColMap::ResetLastLocIdx()
{
	m_nLastLocIdx = -1;
	m_nLastLocColStart = -1;
	m_nLastLocColCount = -1;
}

/**********************************************************************/

void CSalesHistoryRegionLocColMap::RemoveAll()
{
	m_Map.RemoveAll();
	ResetLastLocIdx();
}

/**********************************************************************/

void CSalesHistoryRegionLocColMap::MapLocToColumn( int nLocIdx, int nColumnIdx )
{
	if ( nColumnIdx >= 0 )
	{
		ResetLastLocIdx();

		CSortedIntAndInt item;
		item.m_nInt1 = nLocIdx;
		item.m_nInt2 = nColumnIdx;
		m_Map.Consolidate( item );
	}
}

/**********************************************************************/

int CSalesHistoryRegionLocColMap::GetColumnCount( int nLocIdx )
{
	if ( nLocIdx != m_nLastLocIdx )
	{
		UpdateLastLoc( nLocIdx );
	}

	return m_nLastLocColCount;
}

/**********************************************************************/

int CSalesHistoryRegionLocColMap::GetColumnIdx( int nLocIdx, int nPos )
{
	if ( nLocIdx != m_nLastLocIdx )
	{
		UpdateLastLoc( nLocIdx );
	}

	int nResult = 0;
	if ( ( nPos >= 0 ) && ( nPos < m_nLastLocColCount ) )
	{
		CSortedIntAndInt item;
		m_Map.GetAt( m_nLastLocColStart + nPos, item );
		nResult = item.m_nInt2;
	}

	return nResult;
}

/**********************************************************************/

void CSalesHistoryRegionLocColMap::UpdateLastLoc( int nLocIdx )
{
	CSortedIntAndInt item;
	item.m_nInt1 = nLocIdx;
	item.m_nInt2 = -1;
	m_Map.Find( item, m_nLastLocColStart );

	int nLocEnd;
	item.m_nInt1 = nLocIdx + 1;
	m_Map.Find( item, nLocEnd );
		
	m_nLastLocColCount = ( nLocEnd - m_nLastLocColStart );
	m_nLastLocIdx = nLocIdx;
}

/**********************************************************************/
