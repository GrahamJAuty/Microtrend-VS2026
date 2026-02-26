/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
//#include "DealerFlags.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "SuppRefFinder.h"
/**********************************************************************/

CSuppRefFinderItem::CSuppRefFinderItem()
{
	Reset();
}

/*************************************************************************/

void CSuppRefFinderItem::Reset()
{
	m_strKey = "";
	m_strStockCode = "";
	m_nStockSuppIdx = 0;
}

/*************************************************************************/

void CSuppRefFinderItem::Add ( CSuppRefFinderItem& source )
{
}

/*************************************************************************/

int CSuppRefFinderItem::Compare( CSuppRefFinderItem& source, int nHint )
{
	if ( m_strKey != source.m_strKey )
		return ( m_strKey > source.m_strKey ? 1 : -1 );
	else
		return 0;
}

/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

CSuppRefFinder::CSuppRefFinder()
{
	m_nSuppNo = 0;
	Invalidate();
}

/**********************************************************************/

void CSuppRefFinder::Rebuild( int nSuppNo )
{
	if ( ( m_bIsInSync == TRUE ) && ( nSuppNo == m_nSuppNo ) )
		return;

	m_nSuppNo = nSuppNo;

	m_arraySuppRef.RemoveAll();
	m_arrayWrapper.RemoveAll();

	StatusProgress.Lock( TRUE, "Building supplier list" );

	for ( int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++ )
	{
		StatusProgress.SetPos( nStockIdx, DataManager.Stock.GetSize() );

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		for ( int nStockSuppIdx = 0; nStockSuppIdx < StockRecord.GetSupplierCount(); nStockSuppIdx++ )
		{
			if ( ( StockRecord.GetSuppNo( nStockSuppIdx ) == m_nSuppNo ) || ( m_nSuppNo <= 0 ) )
			{
				{
					CString strKey = StockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );

					if ( strKey != "" )
					{
						strKey.MakeUpper();

						CSuppRefFinderItem item;
						item.m_strKey = strKey;
						item.m_strStockCode = StockRecord.GetStockCode();
						item.m_nStockSuppIdx = nStockSuppIdx;
						m_arraySuppRef.Consolidate( item );
					}
				}

				{

					CString strKey = StockRecord.GetWrapper( nStockSuppIdx );
					
					if ( strKey != "" )
					{
						strKey.MakeUpper();

						CSuppRefFinderItem item;
						item.m_strKey = strKey;
						item.m_strStockCode = StockRecord.GetStockCode();
						item.m_nStockSuppIdx = nStockSuppIdx;
						m_arrayWrapper.Consolidate( item );
					}
				}
			}
		}
	}

	StatusProgress.Unlock();
	m_bIsInSync = TRUE;
}

/**********************************************************************/

bool CSuppRefFinder::FindItemByWrapper( CString& strWrapper, int& nStockIdx, int& nStockSuppIdx )
{
	Rebuild( m_nSuppNo );

	int nIndex;
	strWrapper.MakeUpper();

	CSuppRefFinderItem item;
	item.m_strKey = strWrapper;
	if ( m_arrayWrapper.Find( item, nIndex ) == FALSE )
		return FALSE;

	m_arrayWrapper.GetAt( nIndex, item );

	if ( DataManager.Stock.FindStockItemByCode( item.m_strStockCode, nStockIdx ) == FALSE )
		return FALSE;

	nStockSuppIdx = item.m_nStockSuppIdx;
	return TRUE;
}

/**********************************************************************/

bool CSuppRefFinder::FindItemBySuppRef( CString& strSuppRef, int& nStockIdx, int& nStockSuppIdx )
{
	Rebuild( m_nSuppNo );

	int nIndex;
	strSuppRef.MakeUpper();

	CSuppRefFinderItem item;
	item.m_strKey = strSuppRef;
	if ( m_arraySuppRef.Find( item, nIndex ) == FALSE )
		return FALSE;

	m_arraySuppRef.GetAt( nIndex, item );

	if ( DataManager.Stock.FindStockItemByCode( item.m_strStockCode, nStockIdx ) == FALSE )
		return FALSE;

	nStockSuppIdx = item.m_nStockSuppIdx;
	return TRUE;
}

/**********************************************************************/

void CSuppRefFinder::HandlePreferredSupplierChange( int nStockIdx )
{
	if ( nStockIdx < 0 || nStockIdx >= DataManager.Stock.GetSize() )
		return;
	
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt( nStockIdx, StockRecord );

	//PREFERRED SUPPLIER WILL TAKE PRIORITY IF TWO OR MORE HAVE SAME CODE
	for ( int nStockSuppIdx = StockRecord.GetSupplierCount() - 1; nStockSuppIdx >= 0; nStockSuppIdx-- )
	{
		{
			CSuppRefFinderItem item;
			item.m_strKey = StockRecord.GetSuppRefExternal( nStockSuppIdx, DealerFlags.GetSuppRefFlag() );
			item.m_strKey.MakeUpper();

			if ( item.m_strKey != "" )
			{
				int nIndex;
				if ( m_arraySuppRef.Find( item, nIndex ) == TRUE )
				{
					m_arraySuppRef.GetAt( nIndex, item );

					CString strStockCode = item.m_strStockCode;

					if ( strStockCode == StockRecord.GetStockCode() )
					{
						item.m_nStockSuppIdx = nStockSuppIdx;
						m_arraySuppRef.SetAt( nIndex, item );
					}
				}
			}
		}

		{
			CSuppRefFinderItem item;
			item.m_strKey = StockRecord.GetWrapper( nStockSuppIdx );
			item.m_strKey.MakeUpper();
		
			if ( item.m_strKey != "" )
			{
				int nIndex;
				if ( m_arrayWrapper.Find( item, nIndex ) == TRUE )
				{
					m_arrayWrapper.GetAt( nIndex, item );

					CString strStockCode = item.m_strStockCode;

					if ( strStockCode == StockRecord.GetStockCode() )
					{
						item.m_nStockSuppIdx = nStockSuppIdx;
						m_arrayWrapper.SetAt( nIndex, item );
					}
				}
			}
		}
	}
}

/**********************************************************************/
