/**********************************************************************/
 
/**********************************************************************/
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "NetworkTerminalMap.h"
/**********************************************************************/

CNetworkTerminalMap::CNetworkTerminalMap()
{
}

/**********************************************************************/

void CNetworkTerminalMap::BuildMap()
{
	m_arrayMap.RemoveAll();

	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );

		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( nLocIdx ); nTermIdx++ )
		{
			int nTNo = dbLocation.GetTNo( nLocIdx, nTermIdx );
			AddTerminal( nNwkNo, nTNo, nLocIdx );
		}
	}

	FiddleWriteMap( "Build" );
}

/**********************************************************************/

void CNetworkTerminalMap::AddTerminal( int nNwkNo, int nTNo, int nLocIdx )
{
	CNetworkTerminalInfo info;
	info.m_nNwkNo = nNwkNo;
	info.m_nTNo = nTNo;

	int nMapIdx;
	if ( FindEntry( info, nMapIdx ) == TRUE )
		m_arrayMap[ nMapIdx ].m_nUseCount++;
	else
	{
		info.m_nLocIdx = nLocIdx;
		info.m_nUseCount = 1;
		m_arrayMap.InsertAt( nMapIdx, info );
	}
}

/**********************************************************************/

void CNetworkTerminalMap::RemoveTerminal( int nNwkNo, int nTNo )
{
	CNetworkTerminalInfo info;
	info.m_nNwkNo = nNwkNo;
	info.m_nTNo = nTNo;

	int nMapIdx;
	if ( FindEntry( info, nMapIdx ) == TRUE )
	{
		if ( m_arrayMap[ nMapIdx ].m_nUseCount >= 2 )
			m_arrayMap[ nMapIdx ].m_nUseCount--;
		else
			m_arrayMap.RemoveAt ( nMapIdx );
	}
}

/**********************************************************************/

void CNetworkTerminalMap::RemoveNetwork( int nNwkNo )
{
	CNetworkTerminalInfo info;
	info.m_nNwkNo = nNwkNo;
	info.m_nTNo = 0;

	int nStart;
	FindEntry( info, nStart );

	int nEnd;
	info.m_nNwkNo++;
	FindEntry( info, nEnd );
	nEnd--;

	while ( ( nEnd >= nStart ) && ( nEnd >= 0 ) )
		m_arrayMap.RemoveAt( nEnd-- );
}

/**********************************************************************/

bool CNetworkTerminalMap::FindEntry( CNetworkTerminalInfo& info, int& nIdx )
{
	int nStart = 0;
	int nEnd = m_arrayMap.GetSize() - 1;

	while( nStart <= nEnd )
	{
		nIdx = ( nStart + nEnd ) / 2;

		if ( info.m_nNwkNo > m_arrayMap[nIdx].m_nNwkNo )
			nStart = nIdx + 1;
		else if ( info.m_nNwkNo < m_arrayMap[nIdx].m_nNwkNo )
			nEnd = nIdx - 1;
		else if ( info.m_nTNo > m_arrayMap[nIdx].m_nTNo )
			nStart = nIdx + 1;
		else if ( info.m_nTNo < m_arrayMap[nIdx].m_nTNo )
			nEnd = nIdx - 1;

		else
			return TRUE;
	}

	nIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CNetworkTerminalMap::FindFreeTerminalNumber( int nNwkNo )
{
	CNetworkTerminalInfo info;
	info.m_nNwkNo = nNwkNo;
	info.m_nTNo = 0;

	int nIdx;
	FindEntry( info, nIdx );

	int nNextNo = 1;
	int nResult = -1;

	while( nIdx < m_arrayMap.GetSize() )
	{
		info = m_arrayMap.GetAt( nIdx );

		if ( info.m_nNwkNo != nNwkNo )
		{
			nResult = nNextNo;
			break;
		}

		if ( info.m_nTNo > nNextNo )
		{
			nResult = nNextNo;
			break;
		}

		if ( info.m_nTNo < nNextNo )
		{
			nResult = 1;
			break;
		}
		
		nNextNo++;
		if ( nNextNo > 999 )
		{
			nResult = 1;
			break;
		}

		nIdx++;
	}

	if ( -1 == nResult )
		nResult = nNextNo;

	return nResult;
}

/**********************************************************************/

int CNetworkTerminalMap::GetLocIdx( int nNwkNo, int nTNo )
{
	CNetworkTerminalInfo info;
	info.m_nNwkNo = nNwkNo;
	info.m_nTNo = nTNo;

	int nMapIdx;
	if ( FindEntry( info, nMapIdx ) == TRUE )
		return m_arrayMap[ nMapIdx ].m_nLocIdx;
	else
		return -1;
}

/**********************************************************************/

void CNetworkTerminalMap::IncrementLocIdx( int nLocIdxFrom )
{
	for ( int n = 0; n < m_arrayMap.GetSize(); n++ )
		if ( m_arrayMap[n].m_nLocIdx >= nLocIdxFrom )
			m_arrayMap[n].m_nLocIdx++;

	FiddleWriteMap( "Increment" );
}

/**********************************************************************/

void CNetworkTerminalMap::DecrementLocIdx( int nLocIdxFrom )
{
	for ( int n = 0; n < m_arrayMap.GetSize(); n++ )
		if ( m_arrayMap[n].m_nLocIdx >= nLocIdxFrom )
			m_arrayMap[n].m_nLocIdx--;

	FiddleWriteMap( "Decrement" );
}

/**********************************************************************/

void CNetworkTerminalMap::FiddleWriteMap( const char* szWhy )
{
	/*FIDDLE
	CSSFile fileMap;
	fileMap.Open( "map.txt", "ab" );
	fileMap.WriteLine( szWhy );

	for ( int n = 0; n < m_arrayMap.GetSize(); n++ )
	{
		CString str;
		str.Format( "%3.3d  %3.3d  %3.3d  %3.3d",
			m_arrayMap[n].m_nNwkNo,
			m_arrayMap[n].m_nTNo,
			m_arrayMap[n].m_nUseCount,
			m_arrayMap[n].m_nLocIdx );

		fileMap.WriteLine( str );
	}

	fileMap.WriteLine( "" );
	*/
}

/**********************************************************************/
