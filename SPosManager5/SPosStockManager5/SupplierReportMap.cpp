/**********************************************************************/
#include "SupplierReportMap.h"
/**********************************************************************/

CSupplierReportMap::CSupplierReportMap()
{
}

/**********************************************************************/

void CSupplierReportMap::Clear()
{
	m_arraySuppIdxToSortIdx.RemoveAll();
	m_arraySortIdxToSuppIdx.RemoveAll();
}

/**********************************************************************/

void CSupplierReportMap::CreateMap( CWordArray& arraySuppIdx )
{
	Clear();

	for ( int n = 0; n <= DataManager.Supplier.GetSize(); n++ )
	{
		m_arraySuppIdxToSortIdx.Add( -1 );
		m_arraySortIdxToSuppIdx.Add( -1 );
	}

	CReportConsolidationArray<CSortedStringAndIntItem> tempArray;
	
	for ( int n = 0; n < arraySuppIdx.GetSize(); n++ )
	{
		int nSuppIdx = arraySuppIdx.GetAt(n);
		if ( nSuppIdx < 0 || nSuppIdx > DataManager.Supplier.GetSize() )
			continue;

		CString strSuppName = "";
		if ( nSuppIdx > 0 )
		{
			strSuppName = DataManager.Supplier.GetName( nSuppIdx - 1 );
			::TrimSpaces( strSuppName, FALSE );

			if ( strSuppName == "" )
				strSuppName.Format ( "Supplier %d", DataManager.Supplier.GetSuppNo( nSuppIdx - 1 ) );
		}

		CSortedStringAndIntItem infoTemp;
		infoTemp.m_strItem = strSuppName;
		infoTemp.m_strItem.MakeUpper();
		infoTemp.m_nItem = nSuppIdx;
		tempArray.Consolidate( infoTemp );
	}

	for ( int n = 0; n < tempArray.GetSize(); n++ )
	{
		CSortedStringAndIntItem infoTemp;
		tempArray.GetAt( n, infoTemp );

		if ( ( infoTemp.m_nItem ) >= 0 && ( infoTemp.m_nItem <= DataManager.Supplier.GetSize() ) )
		{
			m_arraySortIdxToSuppIdx.SetAt( n, infoTemp.m_nItem );
			m_arraySuppIdxToSortIdx.SetAt( infoTemp.m_nItem, n );
		}
	}
}

/**********************************************************************/

int CSupplierReportMap::GetSortIdxBySuppNo( int nSuppNo )
{
	int nSuppIdx = -1;

	if ( nSuppNo != 0 )
		if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
			return -1;

	nSuppIdx++;

	if ( nSuppIdx >= 0 && nSuppIdx < m_arraySuppIdxToSortIdx.GetSize() )
		return m_arraySuppIdxToSortIdx.GetAt( nSuppIdx );
	else
		return -1;
}

/**********************************************************************/

int CSupplierReportMap::GetSuppIdxBySortIdx( int nSortIdx )
{
	if ( nSortIdx < 0 || nSortIdx >= m_arraySortIdxToSuppIdx.GetSize() )
		return -1;
	else
		return m_arraySortIdxToSuppIdx.GetAt( nSortIdx );
}

/**********************************************************************/
