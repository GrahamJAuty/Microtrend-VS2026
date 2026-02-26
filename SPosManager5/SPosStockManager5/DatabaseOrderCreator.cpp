/**********************************************************************/
 
/**********************************************************************/
#include "PriceHelpers.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "DatabaseOrderCreator.h"
/**********************************************************************/

CDatabaseOrderCreator::CDatabaseOrderCreator( CProcessedOrderPreviewCSVArray& arrayPreview )
{
	StatusProgress.Lock ( TRUE, "Sorting orders" );

	int nSize = arrayPreview.GetSize();
	for ( int n = 0; n < nSize; n++ )
	{
		StatusProgress.SetPos( n, nSize );

		CProcessedOrderPreviewCSVRecord order;
		arrayPreview.GetAt( n, order );

		CDbOrderCreatorInfo info;
		info.m_nSuppIdx = order.GetSupplierIdx();
		info.m_nOrderIdx = n;
		
		int nIdx;
		FindOrder( info.m_nSuppIdx, info.m_nOrderIdx, nIdx );
		m_arrayOrder.InsertAt( nIdx, info );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CDatabaseOrderCreator::FindOrder( int nSuppIdx, int nOrderIdx, int &nIdx )
{
	int nStart = 0;
	int nEnd = m_arrayOrder.GetSize() - 1;

	while( nStart <= nEnd )
	{
		int nArrayIdx = ( nStart + nEnd ) / 2;

		if ( m_arrayOrder[nArrayIdx].m_nSuppIdx > nSuppIdx )
		{
			nEnd = nArrayIdx - 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nSuppIdx < nSuppIdx )
		{
			nStart = nArrayIdx + 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nOrderIdx > nOrderIdx )
		{
			nEnd = nArrayIdx - 1;
			continue;
		}
		else if ( m_arrayOrder[nArrayIdx].m_nOrderIdx < nOrderIdx )
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
