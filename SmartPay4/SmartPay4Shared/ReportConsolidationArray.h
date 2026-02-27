#pragma once
/**********************************************************************/

template <class T>

class CReportConsolidationArray
{
public:
	CReportConsolidationArray( bool bAllowDuplicates = FALSE, int nHint = 0 );

public:
	void RemoveAll(){ m_array.RemoveAll(); }
	void RemoveAt( int nIdx ){ m_array.RemoveAt( nIdx ); }
	void InsertAt( int nIdx, T& data ){ m_array.InsertAt( nIdx, data ); }
	void DirectAdd( T& data ){ m_array.Add( data ); }
	
public:
	int Consolidate( T& data );
	int ConsolidateWithHint( T& data, int nHint );
	bool Find( T& data, int& nPos );
	bool FindWithHint( T& data, int nHint, int& nPos );

public:
	int GetSize(){ return m_array.GetSize(); }
	void GetAt( int nIdx, T& data );
	void SetAt( int nIdx, T& data );

public:
	void QuickSort( int nHint );

protected:
	void QuickSortInternal( int nLeft, int nRight, int nHint );
	int QuickSortPartition( int nLeft, int nRight, int nPivot, int nHint );

private:
	CArray<T,T> m_array;
	bool m_bAllowDuplicates;
	int m_nHint;
};

/**********************************************************************/

template <class T> CReportConsolidationArray<T>::CReportConsolidationArray( bool bAllowDuplicates, int nHint )
{
	m_bAllowDuplicates = bAllowDuplicates;
	m_nHint = nHint;
}

/**********************************************************************/

template <class T> int CReportConsolidationArray<T>::Consolidate(T& data )
{
	int nPos;
	if ( ( Find( data, nPos ) == FALSE ) || ( TRUE == m_bAllowDuplicates ) )
		m_array.InsertAt( nPos, data );
	else
		m_array[ nPos ].Add( data );

	return nPos;
}

/**********************************************************************/

template <class T> int CReportConsolidationArray<T>::ConsolidateWithHint(T& data, int nHint )
{
	int nSavedHint = m_nHint;
	m_nHint = nHint;
	int nResult = Consolidate( data );
	m_nHint = nSavedHint;
	return nResult;
}

/**********************************************************************/

template <class T> bool CReportConsolidationArray<T>::Find(T& data, int& nPos )
{
	int nStart = 0;
	int nEnd = m_array.GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nPos = ( nStart + nEnd ) / 2;

		T arrayData = m_array.GetAt( nPos );

		switch( arrayData.Compare( data, m_nHint ) )
		{
		case -1:	
			nStart = nPos + 1;		
			break;

		case 1:		
			nEnd = nPos - 1;		
			break;

		case 0:
		default:
			return TRUE;
		}
	}

	nPos = nStart;
	return FALSE;
}

/**********************************************************************/

template <class T> bool CReportConsolidationArray<T>::FindWithHint(T& data, int nHint, int& nPos )
{
	int nSavedHint = m_nHint;
	m_nHint = nHint;
	bool bResult = Find( data, nPos );
	m_nHint = nSavedHint;
	return bResult;
}

/**********************************************************************/

template <class T> void CReportConsolidationArray<T>::GetAt( int nIdx, T& data )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
		data = m_array.GetAt( nIdx );
}

/**********************************************************************/

template <class T> void CReportConsolidationArray<T>::SetAt( int nIdx, T& data )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
		m_array.SetAt( nIdx, data );
}

/**********************************************************************/

template <class T> void CReportConsolidationArray<T>::QuickSort( int nHint )
{
	QuickSortInternal( 0, GetSize() - 1, nHint );
}

/**********************************************************************/

template <class T> void CReportConsolidationArray<T>::QuickSortInternal( int nLeft, int nRight, int nHint )
{
	if ( nLeft < nRight )
	{
		int nPivot = ( nLeft + nRight ) / 2;
		int nNewPivot = QuickSortPartition( nLeft, nRight, nPivot, nHint );
		QuickSortInternal( nLeft, nNewPivot - 1, nHint );
		QuickSortInternal( nNewPivot + 1, nRight, nHint );
	}
}

/**********************************************************************/

template <class T> int CReportConsolidationArray<T>::QuickSortPartition( int nLeft, int nRight, int nPivot, int nHint )
{
	T itemPivot, itemTemp;
	GetAt( nPivot, itemPivot );
	GetAt( nRight, itemTemp );
	SetAt( nPivot, itemTemp );
	SetAt( nRight, itemPivot );

	int nStoreIdx = nLeft;

	for ( int i = nLeft; i < nRight; i++ )
	{
		T itemArray;
		GetAt( i, itemArray );

		if ( itemArray.Compare( itemPivot, nHint ) != 1 )
		{
			GetAt( nStoreIdx, itemTemp );
			SetAt( nStoreIdx, itemArray );
			SetAt( i, itemTemp );
			nStoreIdx++;
		}
	}

	GetAt( nStoreIdx, itemTemp );
	SetAt( nStoreIdx, itemPivot );
	SetAt( nRight, itemTemp );

	return nStoreIdx;
}

/**********************************************************************/
