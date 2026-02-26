/**********************************************************************/
 
/**********************************************************************/
#include "PriceHelpers.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "StockpointOrderCreator.h"
/**********************************************************************/

CStockpointOrderCreator::CStockpointOrderCreator( CProcessedOrderPreviewCSVArray& arrayPreview )
{
	StatusProgress.Lock ( TRUE, "Sorting orders" );

	int nSize = arrayPreview.GetSize();
	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos( n, nSize );

		CProcessedOrderPreviewCSVRecord order;
		arrayPreview.GetAt( n, order );

		CSpOrderCreatorInfo info;
		info.m_nSuppIdx = order.GetSupplierIdx();
		info.m_nStockIdx = order.GetStockIdx();
		info.m_nStockSuppIdx = order.GetStockSuppIdx();
		info.m_nDUItems = order.GetDUItems();
		info.m_dDUItemSize = order.GetDUItemSize();

		for ( int nSpIdx = 0; nSpIdx < order.GetStockpointCount(); nSpIdx++ )
		{
			double dSpOrder = order.GetSURequired(nSpIdx);

			if ( CPriceHelpers::CompareDoubles( dSpOrder, 0.0, 3 ) == 1 )
			{
				info.m_nSpIdx = nSpIdx;
				info.m_dSpOrder = dSpOrder;

				int nIdx;
				FindOrder( nSpIdx, info.m_nSuppIdx, info.m_nStockIdx, nIdx );
				m_arrayOrder.InsertAt( nIdx, info );
			}
		}
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CStockpointOrderCreator::FindOrder( int nSpIdx, int nSuppIdx, int nStockIdx, int &nIdx )
{
	int nStart = 0;
	int nEnd = m_arrayOrder.GetSize() - 1;

	while( nStart <= nEnd )
	{
		int nArrayIdx = ( nStart + nEnd ) / 2;

		if ( m_arrayOrder[nArrayIdx].m_nSpIdx > nSpIdx )
		{
			nEnd = nArrayIdx - 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nSpIdx < nSpIdx )
		{
			nStart = nArrayIdx + 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nSuppIdx > nSuppIdx )
		{
			nEnd = nArrayIdx - 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nSuppIdx < nSuppIdx )
		{
			nStart = nArrayIdx + 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nStockIdx > nStockIdx )
		{
			nEnd = nArrayIdx - 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nStockIdx < nStockIdx )
		{
			nStart = nArrayIdx + 1;
			continue;
		}
		else
		{
			nIdx = nArrayIdx;
			return TRUE;
		}
	}

	nIdx = nStart;
	return FALSE;
}

/**********************************************************************/
