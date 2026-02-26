/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "FilterTypes.h"
#include "ProcessedOrderHeader.h"
#include "ProcessedOrderItem.h"
 
/**********************************************************************/
#include "ProcessedOrderFilterArray.h"
/**********************************************************************/

CProcessedOrderFilterArray::CProcessedOrderFilterArray( CProcessedOrderArray& OrderArray ) : m_OrderArray( OrderArray ) 
{ 
	m_HeaderEntity.Reset();

	m_strDescription = "";
	m_nSuppNo = -1;

	m_bFilteredByHeaderEntity = FALSE;
	m_bFilteredByDescription = FALSE;
	m_bFilteredBySupplier = FALSE;
	m_bIncludePendingItems = TRUE;
	m_bIncludeDeliveredItems = TRUE;
	m_nItemsMatchingOrderStatus = 0;

	m_nSearchType = STOCK_FILTER_DESCRIPTION;
	m_bMatchCase = FALSE;
} 

/**********************************************************************/

void CProcessedOrderFilterArray::SetDescriptionFilter ( const char* szDescription, int nSearchType, bool bMatchCase )
{
	m_strDescription = szDescription;
	m_nSearchType = nSearchType;
	m_bMatchCase = bMatchCase;
}

/**********************************************************************/

int CProcessedOrderFilterArray::GetSize()
{
	return m_IndexArray.GetSize();
}

/**********************************************************************/

int CProcessedOrderFilterArray::GetHeaderIndexAt( int nIndex )
{
	if ( nIndex >= GetSize() )
		return -1;

	return m_IndexArray[nIndex].m_nHeaderIdx;
}

/**********************************************************************/

int CProcessedOrderFilterArray::GetItemIndexAt( int nIndex )
{
	if ( nIndex >= GetSize() )
		return -1;

	return m_IndexArray[nIndex].m_nItemIdx;	
}

/**********************************************************************/

bool CProcessedOrderFilterArray::IsFilteredByUser()
{
	return ( IsFilteredByDescription() || IsFilteredByHeaderEntity() );
}

/**********************************************************************/

void CProcessedOrderFilterArray::BuildList()
{
	m_IndexArray.RemoveAll();
	m_nItemsMatchingOrderStatus = 0;
	
	m_bFilteredByHeaderEntity = ( m_HeaderEntity.m_nOrderNo != 0 );
	m_bFilteredByDescription = ( m_strDescription != "" );
	m_bFilteredBySupplier = ( m_nSuppNo != -1 );

	for ( int nOrderIdx = 0; nOrderIdx < m_OrderArray.GetOrderCount(); nOrderIdx++ )
	{
		CProcessedOrderHeader HeaderRecord;
		m_OrderArray.GetHeaderRecord( nOrderIdx, HeaderRecord );
		int nHeaderOrderNum = HeaderRecord.GetOrderNum();
		int nHeaderSuppNo = HeaderRecord.GetSuppNo();

		if ( m_nSuppNo != -1 )
			if ( m_nSuppNo != nHeaderSuppNo )
				continue;

		CProcessedOrderEntity HeaderEntity;
		m_OrderArray.GetHeaderEntity( nOrderIdx, HeaderEntity );

		bool bMatchOrderNum = TRUE;
		if ( m_HeaderEntity.m_nOrderNo != 0 )	
			if ( m_HeaderEntity.Compare( HeaderEntity ) != 0 )
				bMatchOrderNum = FALSE;
		
		int nStartIdx, nEndIdx;
		m_OrderArray.GetItemIdxRange( HeaderEntity, nStartIdx, nEndIdx );

		bool bCheckStatus = ( ( FALSE == m_bIncludePendingItems ) || ( FALSE == m_bIncludeDeliveredItems ) );
		bool bCheckText = ( m_strDescription != "" );

		if ( FALSE == bCheckStatus )
			m_nItemsMatchingOrderStatus += ( nEndIdx - nStartIdx ) + 1;
		
		if ( ( TRUE == bCheckStatus ) || ( TRUE == bMatchOrderNum ) )
		{
			for ( int nItemIdx = nStartIdx; nItemIdx <= nEndIdx; nItemIdx++ )
			{
				CProcessedOrderFilterIndex index;
				index.m_nHeaderIdx = nOrderIdx;
				index.m_nItemIdx = nItemIdx;

				bool bMatchStatus = TRUE;
				if ( TRUE == bCheckStatus )
					bMatchStatus = CheckItemForStatus( nItemIdx );

				if ( ( TRUE == bCheckStatus ) && ( TRUE == bMatchStatus ) )
					m_nItemsMatchingOrderStatus++;

				if ( ( TRUE == bMatchOrderNum ) && ( TRUE == bMatchStatus ) )
				{
					if ( FALSE == bCheckText )
						m_IndexArray.Add( index );
					else if ( CheckItemForText( nItemIdx ) == TRUE )
						m_IndexArray.Add( index );
				}
			}
		}
	}
}

/**********************************************************************/

bool CProcessedOrderFilterArray::CheckItemForStatus( int nItemIdx )
{
	CProcessedOrderItem ItemRecord;
	m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

	if ( FALSE == m_bIncludePendingItems )
		if ( ItemRecord.GetDeliveredOldFlag() == FALSE )
			return FALSE;

	if ( FALSE == m_bIncludeDeliveredItems )
		if ( ItemRecord.GetDeliveredOldFlag() == TRUE )
			return FALSE;

	return TRUE;
}

/**********************************************************************/

bool CProcessedOrderFilterArray::CheckItemForText( int nItemIdx )
{
	CProcessedOrderItem ItemRecord;
	m_OrderArray.GetItemRecord( nItemIdx, ItemRecord );

	if ( m_strDescription == "" )
		return TRUE;
		
	CString strSearchText = m_strDescription;
	if ( m_bMatchCase == FALSE )
		strSearchText.MakeUpper();
		
	if ( ( m_nSearchType & STOCK_FILTER_DESCRIPTION ) != 0 )
	{
		CString strDesc = ItemRecord.GetDescription();
			
		if ( m_bMatchCase == FALSE )
			strDesc.MakeUpper();
		
		if ( strDesc.Find ( strSearchText ) != - 1 )  
			return TRUE;
	}

	if ( ( m_nSearchType & STOCK_FILTER_STOCKCODE ) != 0 )
	{
		CString strCode = ItemRecord.GetStockCode();
		if ( m_bMatchCase == FALSE )
			strCode.MakeUpper();
		
		if ( strCode.Find ( strSearchText ) != - 1 )   
			return TRUE;
	}
	
	if ( ( m_nSearchType & STOCK_FILTER_SUPPREF ) != 0 )
	{
		CString strSuppRef = ItemRecord.GetSuppRef();
		if ( m_bMatchCase == FALSE )
			strSuppRef.MakeUpper();
		
		if ( strSuppRef.Find ( strSearchText ) != - 1 )  
			return TRUE;
	}
	
	return FALSE;
}

/**********************************************************************/

bool CProcessedOrderFilterArray::FindItemIdx( int nItemIdx, int& nPos )
{
	int nStartIdx = 0;
	int nEndIdx = m_IndexArray.GetSize() - 1;

	while( nStartIdx <= nEndIdx )
	{
		nPos = ( nStartIdx + nEndIdx ) / 2;

		int nArrayIdx = m_IndexArray[nPos].m_nItemIdx;

		if ( nItemIdx > nArrayIdx )
			nStartIdx = nPos + 1;
		else if ( nItemIdx < nArrayIdx )
			nEndIdx = nPos - 1;
		else
			return TRUE;
	}

	nPos = nStartIdx;
	return FALSE;
}

/**********************************************************************/

void CProcessedOrderFilterArray::CopySettingsFrom( CProcessedOrderFilterArray& Source )
{
	m_strDescription = Source.GetDescription();
	m_nSearchType = Source.GetSearchType();
	m_bMatchCase = Source.GetMatchCase();
	m_nSuppNo = Source.GetSuppNo();
	m_bIncludePendingItems = Source.GetIncludePendingItemsFlag();
	m_bIncludeDeliveredItems = Source.GetIncludeDeliveredItemsFlag();
}

/**********************************************************************/

void CProcessedOrderFilterArray::ShowDebugInfo()
{
#ifdef _DEBUG
	CString strMsg;
	
	for ( int n = 0; n < GetSize(); n++ )
	{
		CString strLine;
		strLine.Format( "%d %d\n", m_IndexArray[n].m_nHeaderIdx, m_IndexArray[n].m_nItemIdx );
		strMsg += strLine;
	}

	Prompter.Info( strMsg );

#endif
}

/**********************************************************************/
