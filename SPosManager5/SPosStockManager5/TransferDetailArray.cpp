/**********************************************************************/
#include "TransferDetailCSVArray.h"
/**********************************************************************/

CTransferDetailInfo::CTransferDetailInfo()
{
	m_nSpNo = 0;
	m_dTransferInQty = 0.0;
	m_dTransferOutQty = 0.0;
	m_dStocktakeTransferInQty = 0.0;
	m_dStocktakeTransferOutQty = 0.0;
}

/**********************************************************************/

void CTransferDetailArray::GetTransferDetailInfo( CTransferDetailInfo& info )
{
	info.m_dTransferInQty = 0.0;
	info.m_dTransferOutQty = 0.0;
	info.m_dStocktakeTransferInQty = 0.0;
	info.m_dStocktakeTransferOutQty = 0.0;

	int nIdx;
	if ( FindTransferDetailInfo( info.m_nSpNo, nIdx ) == TRUE )
		info = m_arrayDetail.GetAt( nIdx );
}

/**********************************************************************/

void CTransferDetailArray::SetTransferDetailInfo( CTransferDetailInfo& info )
{
	int nIdx;
	if ( FindTransferDetailInfo( info.m_nSpNo, nIdx ) == TRUE )
		m_arrayDetail.SetAt( nIdx, info );
	else
		m_arrayDetail.InsertAt( nIdx, info );
}

/**********************************************************************/

void CTransferDetailArray::GetTransferDetailInfo( int nIdx, CTransferDetailInfo& info )
{
	info.m_dTransferInQty = 0.0;
	info.m_dTransferOutQty = 0.0;
	info.m_dStocktakeTransferInQty = 0.0;
	info.m_dStocktakeTransferOutQty = 0.0;

	if ( ( nIdx >= 0 ) && ( nIdx < GetSpCount() ) ) 
		info = m_arrayDetail.GetAt( nIdx );
}

/**********************************************************************/

void CTransferDetailArray::SetTransferDetailInfo( int nIdx, CTransferDetailInfo& info )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSpCount() ) ) 
		m_arrayDetail.SetAt( nIdx, info );
}

/**********************************************************************/

void CTransferDetailArray::AddTransferDetailInfo( CTransferDetailInfo& info )
{
	int nIdx;
	if ( FindTransferDetailInfo( info.m_nSpNo, nIdx ) == TRUE )
	{
		m_arrayDetail[nIdx].m_dTransferInQty += info.m_dTransferInQty;
		m_arrayDetail[nIdx].m_dTransferOutQty += info.m_dTransferOutQty;
		m_arrayDetail[nIdx].m_dStocktakeTransferInQty += info.m_dStocktakeTransferInQty;
		m_arrayDetail[nIdx].m_dStocktakeTransferOutQty += info.m_dStocktakeTransferOutQty;
	}
	else
		m_arrayDetail.InsertAt( nIdx, info );
}

/**********************************************************************/

bool CTransferDetailArray::FindTransferDetailInfo( int nSpNo, int& nIdx )
{
	int nStart = 0;
	int nEnd = m_arrayDetail.GetSize() - 1;

	while( nStart <= nEnd )
	{
		nIdx = ( nStart + nEnd ) / 2;
		int nArraySpNo = m_arrayDetail[nIdx].m_nSpNo;

		if ( nSpNo > nArraySpNo )
			nStart = nIdx + 1;
		else if ( nSpNo < nArraySpNo )
			nEnd = nIdx - 1;
		else
			return TRUE;
	}

	nIdx = nStart;

	return FALSE;
}

/**********************************************************************/

void CTransferDetailArray::Reset()
{
	m_arrayDetail.RemoveAll();
}

/**********************************************************************/

void CTransferDetailArray::AddTransferDetailToCSVLine( CCSV& csv )
{
	int nCount = m_arrayDetail.GetSize();

	csv.Add( nCount );
	for ( int nIdx = 0; nIdx < nCount; nIdx++ )
	{
		csv.Add( m_arrayDetail[nIdx].m_nSpNo );
		csv.Add( m_arrayDetail[nIdx].m_dTransferInQty, 5 );
		csv.Add( m_arrayDetail[nIdx].m_dTransferOutQty, 5 );
		csv.Add( m_arrayDetail[nIdx].m_dStocktakeTransferInQty, 5 );
		csv.Add( m_arrayDetail[nIdx].m_dStocktakeTransferOutQty, 5 );
	}
}

/**********************************************************************/

void CTransferDetailArray::RemoveTransferDetailInfo( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayDetail.GetSize() ) )
		m_arrayDetail.RemoveAt( nIdx );
}

/**********************************************************************/
