/**********************************************************************/
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "Password.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockFilterArray.h"
/**********************************************************************/

CStockIndexItem::CStockIndexItem()
{
	Reset();
}

/**********************************************************************/

void CStockIndexItem::Reset()
{
	m_strDescription = "";
	m_nCategory = 0;
	m_strStockCode = "";
	m_strNewStockCode = "";
	m_nIndex = 0;
	m_bDeletionFlag = FALSE;
	m_nIndexVersion = 0;
}

/**********************************************************************/

void CStockIndexItem::Add ( CStockIndexItem& source )
{
}

/**********************************************************************/

int CStockIndexItem::Compare( CStockIndexItem& source, int nHint )
{
	switch( nHint )
	{
	case STOCKFILTER_SORT_FORWARD_CATEGORY:
		{
			if ( m_nCategory != source.m_nCategory )
				return ( m_nCategory > source.m_nCategory ? 1 : -1 );
		}
		break;

	case STOCKFILTER_SORT_REVERSE_CATEGORY:
		{
			if ( m_nCategory != source.m_nCategory )
				return ( m_nCategory < source.m_nCategory ? 1 : -1 );
		}
		break;

	case STOCKFILTER_SORT_FORWARD_DESCRIPTION:
		{
			if ( m_strDescription != source.m_strDescription )
				return ( m_strDescription > source.m_strDescription ? 1 : -1 );
		}
		break;

	case STOCKFILTER_SORT_REVERSE_DESCRIPTION:
		{
			if ( m_strDescription != source.m_strDescription )
				return ( m_strDescription < source.m_strDescription ? 1 : -1 );
		}
		break;

	case STOCKFILTER_SORT_FORWARD_NEWSTOCKCODE:
		{
			if ( m_strNewStockCode != source.m_strNewStockCode )
				return ( m_strNewStockCode > source.m_strNewStockCode ? 1 : -1 );
		}
		break;

	case STOCKFILTER_SORT_REVERSE_NEWSTOCKCODE:
		{
			if ( m_strNewStockCode != source.m_strNewStockCode )
				return ( m_strNewStockCode < source.m_strNewStockCode ? 1 : -1 );
		}
		break;
	}

	switch( nHint )
	{
	case STOCKFILTER_SORT_REVERSE_STOCKCODE:
		{
			if ( m_strStockCode != source.m_strStockCode )
				return ( m_strStockCode < source.m_strStockCode ? 1 : -1 );
		}
		break;

	default:
		{
			if ( m_strStockCode != source.m_strStockCode )
				return ( m_strStockCode > source.m_strStockCode ? 1 : -1 );
		}
		break;
	}

	return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CStockFilterMinMaxFlags::CStockFilterMinMaxFlags( bool bAllowCheck )
{
	m_bCheckMin = ( StockOptions.GetStockBelowMinimumFlag() && bAllowCheck );
	m_bCheckMax = ( StockOptions.GetStockAboveMaximumFlag() && bAllowCheck );
	m_bBelowMin = FALSE;
	m_bAboveMax = FALSE;
}

/**********************************************************************/

void CStockFilterMinMaxFlags::ResetResults( bool bTrueIfNoCheck )
{
	if ( FALSE == bTrueIfNoCheck )
	{
		m_bBelowMin = FALSE;
		m_bAboveMax = FALSE;
	}
	else
	{
		m_bBelowMin = ( FALSE == m_bCheckMin );
		m_bAboveMax = ( FALSE == m_bCheckMax );
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CStockFilterArray::CStockFilterArray( int nFilterType ) : m_MinMaxFlags( TRUE ) 
{ 
	m_nFilterType = nFilterType;
	Reset();
}

/**********************************************************************/

void CStockFilterArray::Reset() 
{ 
	m_nSortType = STOCKFILTER_SORT_FORWARD_STOCKCODE;
	m_bFiltered = FALSE;
	m_nIndexVersion = 0;
	m_strDescription = "";
	m_nSearchType = STOCK_FILTER_DESCRIPTION;
	m_bMatchCase = FALSE;
	
	m_bNoPluLink = TRUE;
	m_bSinglePluLink = TRUE;
	m_bMultiPluLink = TRUE;
	m_nStocktakeExclude = 0;
	m_nInactiveExclude = 0;
	m_bActiveItemOnly = FALSE;
	m_arrayActiveStockpoints.RemoveAll();
	
	m_nSpIdx = -1;
	m_nSpIdx2 = -1;
	m_nSpNo = 0;
	m_nSpNo2 = 0;
	m_nDbNo = 0;

	m_bCheckApparent = TRUE;
	m_nOrderPreviewConLevel = NODE_DATABASE;
	m_nOrderPreviewTableNo = 0;
	
	SetCategoryFilter(0);
	SetSupplierFilter(0);
	SetStocktakeFilter(-1);
	SetMinMaxFilter( STOCKFILTER_MINMAX_ALL );

	m_nArrayIdx = 0;
} 

/**********************************************************************/

void CStockFilterArray::SetActiveStockpointsArray(CArray<int, int>& arrayStockpoints)
{
	m_arrayActiveStockpoints.RemoveAll();
	for (int n = 0; n < arrayStockpoints.GetSize(); n++)
	{
		m_arrayActiveStockpoints.Add(arrayStockpoints.GetAt(n));
	}
}

/**********************************************************************/

void CStockFilterArray::SetDescriptionFilter ( const char* szDescription, int nSearchType, bool bMatchCase )
{
	m_strDescription = szDescription;
	m_nSearchType = nSearchType;
	m_bMatchCase = bMatchCase;
}

/**********************************************************************/

void CStockFilterArray::SetCategoryFilter ( int nCategoryFilter )
{
	m_nCategoryFilter = m_CategorySet.CreateCategorySet ( nCategoryFilter );
}

/**********************************************************************/

void CStockFilterArray::SetSupplierFilter(int nSupplierFilter)
{
	m_SupplierNoArray.RemoveAll();
	m_nSupplierFilter = nSupplierFilter;
	int nSupplierSet = m_nSupplierFilter - FIRST_SUPPLIERSET_FILTER;

	//ALL SUPPLIERS OR UNSPECIFIED SUPPLIER
	if (m_nSupplierFilter == -1 || m_nSupplierFilter == 0)
	{
		return;
	}

	//SINGLE SUPPLIER
	if (m_nSupplierFilter >= 1 && m_nSupplierFilter <= MAX_SYSTEM_SUPPLIERS)
	{
		m_SupplierNoArray.InsertAt(0, 0, 1001);
		m_SupplierNoArray.SetAt(m_nSupplierFilter, 1);
	}
	//SUPPLIER SET
	else if (nSupplierSet >= 0 && nSupplierSet < DataManager.SupplierSet.GetSize())
	{
		m_SupplierNoArray.InsertAt(0, 0, 1001);

		CSupplierSetCSVRecord SupplierSet;
		DataManager.SupplierSet.GetAt(nSupplierSet, SupplierSet);

		for (int nIndex = 0; nIndex < SupplierSet.GetSupplierCount(); nIndex++)
		{
			int nSupplierNo = SupplierSet.GetSuppNo(nIndex);

			if (nSupplierNo >= 1 && nSupplierNo <= MAX_SYSTEM_SUPPLIERS)
			{
				m_SupplierNoArray.SetAt(nSupplierNo, 1);
			}
		}
	}
	//UNKNOWN SO FORCE ALL SUPPLIERS
	else
	{
		m_nSupplierFilter = 0;
	}
}

/**********************************************************************/

void CStockFilterArray::SetMinMaxFilter( int n )
{
	switch(n)
	{
	case STOCKFILTER_MINMAX_ALL:
	case STOCKFILTER_MINMAX_BELOW_MIN:
	case STOCKFILTER_MINMAX_ABOVE_MAX:
		m_nMinMaxFilter = n;
		break;
	}
}

/**********************************************************************/

int CStockFilterArray::GetDefaultCategory()
{
	return m_CategorySet.GetDefaultCategory();
}

/**********************************************************************/

void CStockFilterArray::SetSpIdx ( int nSpIdx, int nSpIdx2 )
{
	if ( nSpIdx >= 0 && nSpIdx < dbStockpoint.GetSize() )
	{
		m_nSpIdx = nSpIdx;
		m_nSpNo = dbStockpoint.GetSpNo ( nSpIdx );
		m_nDbNo = dbStockpoint.GetDbNo ( nSpIdx );
	}

	if ( nSpIdx2 >= 0 && nSpIdx2 < dbStockpoint.GetSize() )
	{
		m_nSpIdx2 = nSpIdx2;
		m_nSpNo2 = dbStockpoint.GetSpNo ( nSpIdx2 );
	}
}

/**********************************************************************/

void CStockFilterArray::SetSpIdx( CStockFilterArray& source )
{
	m_nSpIdx = source.m_nSpIdx;
	m_nSpIdx2 = source.m_nSpIdx2;
	m_nSpNo = source.m_nSpNo;
	m_nSpNo2 = source.m_nSpNo2;
	m_nDbNo = source.m_nDbNo;
}

/**********************************************************************/

bool CStockFilterArray::CheckDescription( CStockCSVRecord& StockRecord )
{
	if (m_strDescription == "")
	{
		return TRUE;
	}

	CString strSearchText = m_strDescription;
	if (m_bMatchCase == FALSE)
	{
		strSearchText.MakeUpper();
	}

	if ((m_nSearchType & STOCK_FILTER_DESCRIPTION) != 0)
	{
		CString strDesc = StockRecord.GetDescription();

		if (m_bMatchCase == FALSE)
		{
			strDesc.MakeUpper();
		}

		if (strDesc.Find(strSearchText) != -1)
		{
			return TRUE;
		}
	}

	if ((m_nSearchType & STOCK_FILTER_STOCKCODE) != 0)
	{
		CString strCode = StockRecord.GetStockCode();

		if (m_bMatchCase == FALSE)
		{
			strCode.MakeUpper();
		}

		if (strCode.Find(strSearchText) != -1)
		{
			return TRUE;
		}
	}
	
	if ( ( m_nSearchType & STOCK_FILTER_SUPPREF ) != 0 )
	{
		for (int nSuppIdx = 0; nSuppIdx < StockRecord.GetSupplierCount(); nSuppIdx++)
		{
			CString strSuppRef = StockRecord.GetSuppRefExternal(nSuppIdx, DealerFlags.GetSuppRefFlag());

			if (m_bMatchCase == FALSE)
			{
				strSuppRef.MakeUpper();
			}

			if (strSuppRef.Find(strSearchText) != -1)
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

/**********************************************************************/

bool CStockFilterArray::CheckSupplier( CStockCSVRecord& StockRecord )
{
	if (m_nSupplierFilter == -1)
	{
		return (StockRecord.GetSupplierCount() == 0);
	}

	for (int nSuppIdx = 0; nSuppIdx < StockOptions.GetStockMaxSuppliersPerItem(); nSuppIdx++)
	{
		int nSuppNo = StockRecord.GetSuppNo(nSuppIdx);

		if ((nSuppNo >= 0) && (nSuppNo < m_SupplierNoArray.GetSize()))
		{
			if (m_SupplierNoArray.GetAt(nSuppNo) == 1)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

/**********************************************************************/

void CStockFilterArray::CreateUnfilteredList()
{
	for ( int nStockIdx = DataManager.Stock.GetSize() - 1; nStockIdx >= 0; nStockIdx-- )
	{
		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt( nStockIdx, StockRecord );

		CStockIndexItem item;
		item.m_strStockCode = StockRecord.GetStockCode();
		item.m_strNewStockCode = GetNewStockCode( nStockIdx );
		item.m_nCategory = StockRecord.GetCategory();
		item.m_strDescription = StockRecord.GetDescription();
		item.m_nIndex = nStockIdx;
		item.m_nIndexVersion = 0;
		m_index.DirectAdd( item );
	}
}

/**********************************************************************/

void CStockFilterArray::BuildRecordList()
{
	m_index.RemoveAll();
	m_nIndexVersion = 0;
	m_nStocktakeExclude = 0;
	m_nInactiveExclude = 0;

	m_bFiltered = CheckFiltered();

	if (HasAllRecords() == TRUE)
	{
		switch (m_nSortType)
		{
		case STOCKFILTER_SORT_FORWARD_STOCKCODE:
			break;

		case STOCKFILTER_SORT_REVERSE_STOCKCODE:
			CreateUnfilteredList();
			break;

		default:
			CreateUnfilteredList();
			m_index.QuickSort(m_nSortType);
			break;
		}
	}
	else
	{
		for (int nStockIdx = 0; nStockIdx < DataManager.Stock.GetSize(); nStockIdx++)
		{
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt(nStockIdx, StockRecord);

			//MUST CHECK THIS FIRST SO THAT WE ALWAYS KNOW HOW MANY
			//RECORDS WOULD BE EXCLUDED BY STOCKTAKE
			if ((m_nStocktakeFilter != -1) || (TRUE == m_bActiveItemOnly))
			{
				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, m_nSpNo, StockLevels);

				if (m_nStocktakeFilter != -1)
				{
					if (StockLevels.GetStocktakeNo() != m_nStocktakeFilter)
					{
						m_nStocktakeExclude++;
						continue;
					}
				}

				if (TRUE == m_bActiveItemOnly)
				{
					if (m_arrayActiveStockpoints.GetSize() > 0)
					{
						bool bFoundActive = FALSE;
						for (int n = 0; n < m_arrayActiveStockpoints.GetSize(); n++)
						{
							int nSpIdx = m_arrayActiveStockpoints.GetAt(n);
							DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, dbStockpoint.GetSpNo(nSpIdx), StockLevels);
							if (StockLevels.GetActiveFlag() == TRUE)
							{
								bFoundActive = TRUE;
								break;
							}
						}

						if (FALSE == bFoundActive)
						{
							m_nInactiveExclude++;
							continue;
						}
					}
					else
					{
						bool bInactive = (StockLevels.GetActiveFlag() == FALSE);

						if ((m_nSpNo2 != 0) && (FALSE == bInactive))
						{
							DataManager.Stock.GetStockpointTotals(nStockIdx, StockRecord, m_nSpNo2, StockLevels);
							bInactive = (StockLevels.GetActiveFlag() == FALSE);
						}

						if (TRUE == bInactive)
						{
							m_nInactiveExclude++;
							continue;
						}
					}
				}
			}

			if (m_bNoPluLink == FALSE || m_bSinglePluLink == FALSE || m_bMultiPluLink == FALSE)
			{
				int nLinkCount = 0;

				int nStockPluIdx;
				if (DataManager.StockPlu.FindStockCode(StockRecord.GetStockCode(), nStockPluIdx) == TRUE)
				{
					CStockPluRecord StockPluRecord;
					DataManager.StockPlu.GetAt(nStockPluIdx, StockPluRecord);
					nLinkCount = StockPluRecord.GetPluNoCount() + StockPluRecord.GetRecipePluNoCount();
				}

				if ((m_bNoPluLink == FALSE) && (nLinkCount == 0))
				{
					continue;
				}

				if ((m_bSinglePluLink == FALSE) && (nLinkCount == 1))
				{
					continue;
				}

				if ((m_bMultiPluLink == FALSE) && (nLinkCount > 1))
				{
					continue;
				}
			}

			if (m_CategorySet.GetCategoryStatus(StockRecord.GetCategory()) == FALSE)
			{
				continue;
			}

			if (CheckDescription(StockRecord) == FALSE)
			{
				continue;
			}

			if (STOCKFILTER_MINMAX_BELOW_MIN == m_nMinMaxFilter)
			{
				CStockFilterMinMaxFlags MinMax(TRUE);
				MinMax.SetCheckMin(TRUE);
				MinMax.SetCheckMax(FALSE);

				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetDatabaseTotals(nStockIdx, StockRecord, StockLevels, MinMax);

				if (MinMax.BelowMin() == FALSE)
				{
					continue;
				}
			}

			if (STOCKFILTER_MINMAX_ABOVE_MAX == m_nMinMaxFilter)
			{
				CStockFilterMinMaxFlags MinMax(TRUE);
				MinMax.SetCheckMin(FALSE);
				MinMax.SetCheckMax(TRUE);

				CStockLevelsCSVRecord StockLevels;
				DataManager.Stock.GetDatabaseTotals(nStockIdx, StockRecord, StockLevels, MinMax);

				if (MinMax.AboveMax() == FALSE)
				{
					continue;
				}
			}

			if ((0 == m_nSupplierFilter) || (CheckSupplier(StockRecord) == TRUE))
			{
				CStockIndexItem item;
				item.m_strStockCode = StockRecord.GetStockCode();
				item.m_strNewStockCode = GetNewStockCode(nStockIdx);
				item.m_nCategory = StockRecord.GetCategory();
				item.m_strDescription = StockRecord.GetDescription();
				item.m_nIndex = nStockIdx;
				item.m_nIndexVersion = 0;
				m_index.DirectAdd(item);
			}
		}

		if (STOCKFILTER_SORT_FORWARD_STOCKCODE != m_nSortType)
		{
			m_index.QuickSort(m_nSortType);
		}
	}
}

/**********************************************************************/

void CStockFilterArray::RefreshIndexItems()
{
	if ( IsNativeOrder() == FALSE )
	{
		bool bNewStockCode = FALSE;
		switch( m_nSortType )
		{
		case STOCKFILTER_SORT_FORWARD_NEWSTOCKCODE:
		case STOCKFILTER_SORT_REVERSE_NEWSTOCKCODE:
			bNewStockCode = TRUE;
			break;
		}

		int nStockDbSize = DataManager.Stock.GetSize();

		for ( int n = 0; n < m_index.GetSize(); n++ )
		{
			int nStockIdx = GetStockItemIndexAt(n);

			if ( ( nStockIdx >= 0 ) && ( nStockIdx < nStockDbSize ) )
			{
				CStockCSVRecord StockRecord;
				DataManager.Stock.GetAt( nStockIdx, StockRecord );

				CStockIndexItem item;
				m_index.GetAt( n, item );
				item.m_strStockCode = StockRecord.GetStockCode();
				item.m_nCategory = StockRecord.GetCategory();
				item.m_strDescription = StockRecord.GetDescription();

				if (TRUE == bNewStockCode)
				{
					item.m_strNewStockCode = GetNewStockCode(nStockIdx);
				}
				else
				{
					item.m_strNewStockCode = "";
				}

				m_index.SetAt( n, item );
			}
		}
	}
}

/**********************************************************************/

void CStockFilterArray::ChangeSortOrder( int nType, bool bRefreshIndexItems )
{
	switch( nType )
	{
	case STOCKFILTER_SORT_FORWARD_STOCKCODE:
	case STOCKFILTER_SORT_FORWARD_DESCRIPTION:
	case STOCKFILTER_SORT_FORWARD_CATEGORY:
	case STOCKFILTER_SORT_FORWARD_NEWSTOCKCODE:
		break;

	default:
		return;
	}

	if ( nType == m_nSortType )
	{
		switch( nType )
		{
		case STOCKFILTER_SORT_FORWARD_STOCKCODE:	nType = STOCKFILTER_SORT_REVERSE_STOCKCODE;		break;
		case STOCKFILTER_SORT_FORWARD_DESCRIPTION:	nType = STOCKFILTER_SORT_REVERSE_DESCRIPTION;	break;
		case STOCKFILTER_SORT_FORWARD_CATEGORY:		nType = STOCKFILTER_SORT_REVERSE_CATEGORY;		break;
		case STOCKFILTER_SORT_FORWARD_NEWSTOCKCODE:	nType = STOCKFILTER_SORT_REVERSE_NEWSTOCKCODE;	break;
		}
	}

	if ( IsNativeOrder() == TRUE )
	{
		SetSortType( nType );
		BuildRecordList();
	}
	else
	{
		SetSortType( nType );

		if ( IsNativeOrder() == TRUE )
		{
			m_index.RemoveAll();
			m_nIndexVersion = 0;
			m_nStocktakeExclude = 0;
			m_nInactiveExclude = 0;
		}
		else
		{
			if (TRUE == bRefreshIndexItems)
			{
				RefreshIndexItems();
			}

			QuickSort();
		}
	}
}

/**********************************************************************/

void CStockFilterArray::QuickSort()
{
	if ( IsNativeOrder() == FALSE )
		m_index.QuickSort( m_nSortType );
}

/**********************************************************************/

void CStockFilterArray::QuickBuild( bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales )
{
	StatusProgress.Lock( TRUE, "Building deletion list" );
	QuickBuildInternal( bKeepOnOrder, bKeepApparent, bKeepSales );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockFilterArray::QuickBuild( CByteArray& FlagArray, bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales )
{
	StatusProgress.Lock( TRUE, "Building deletion list" );
	QuickBuildInternal( FlagArray, bKeepOnOrder, bKeepApparent, bKeepSales );
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockFilterArray::QuickBuildInternal( bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales )
{
	Reset();	
	m_bFiltered = TRUE;

	m_index.RemoveAll();
	m_nIndexVersion = 0;
	
	int nSize = DataManager.Stock.GetSize();
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nSize );

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );
		CString strStockCode = StockRecord.GetStockCode();

		int nStockPluIdx;
		if ( DataManager.StockPlu.FindStockCode ( strStockCode, nStockPluIdx ) == TRUE )
			if ( DataManager.StockPlu.GetTotalLinkCount( nStockPluIdx ) != 0 )
				continue;

		if ( TRUE == bKeepApparent )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );

			if ( StockLevels.GetApparentQty() != 0.0 )
				continue;
		}

		if ( TRUE == bKeepOnOrder )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );

			if ( StockLevels.GetQtyOnOrder() != 0 )
				continue;
		}

		if ( TRUE == bKeepSales )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );

			if ( StockLevels.GetTotalSalesQty() != 0.0 )
				continue;
		}

		CStockIndexItem item;
		item.m_strStockCode = StockRecord.GetStockCode();
		item.m_strNewStockCode = "";
		item.m_nCategory = StockRecord.GetCategory();
		item.m_strDescription = StockRecord.GetDescription();
		item.m_nIndex = nStockIdx;
		item.m_nIndexVersion = 0;
		m_index.DirectAdd( item );
	}
}

/**********************************************************************/

void CStockFilterArray::QuickBuildInternal( CByteArray& FlagArray, bool bKeepOnOrder, bool bKeepApparent, bool bKeepSales )
{
	Reset();	
	m_bFiltered = TRUE;

	m_index.RemoveAll();
	m_nIndexVersion = 0;

	int nSize = DataManager.Stock.GetSize();
	for ( int nStockIdx = 0; nStockIdx < nSize; nStockIdx++ )
	{
		StatusProgress.SetPos ( nStockIdx, nSize );

		CStockCSVRecord StockRecord;
		DataManager.Stock.GetAt ( nStockIdx, StockRecord );

		if ( FlagArray.GetAt ( nStockIdx ) == TRUE )
			continue;

		if ( TRUE == bKeepApparent )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );

			if ( StockLevels.GetApparentQty() != 0.0 )
				continue;
		}

		if ( TRUE == bKeepOnOrder )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );

			if ( StockLevels.GetQtyOnOrder() != 0 )
				continue;
		}

		if ( TRUE == bKeepSales )
		{
			CStockLevelsCSVRecord StockLevels;
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );

			if ( StockLevels.GetTotalSalesQty() != 0.0 )
				continue;
		}

		CStockIndexItem item;
		item.m_strStockCode = StockRecord.GetStockCode();
		item.m_strNewStockCode = "";
		item.m_nCategory = StockRecord.GetCategory();
		item.m_strDescription = StockRecord.GetDescription();
		item.m_nIndex = nStockIdx;
		item.m_nIndexVersion = 0;
		m_index.DirectAdd( item );
	}
}

/**********************************************************************/

bool CStockFilterArray::CheckFiltered()
{
	if ( m_nCategoryFilter != 0 )
		return TRUE;

	if ( m_nSupplierFilter != 0 )
		return TRUE;

	if ( m_strDescription != "" )
		return TRUE;

	if ( FALSE == m_bNoPluLink )
		return TRUE;

	if ( FALSE == m_bSinglePluLink )
		return TRUE;

	if ( FALSE == m_bMultiPluLink )
		return TRUE;

	switch( m_nMinMaxFilter )
	{
	case STOCKFILTER_MINMAX_BELOW_MIN:
	case STOCKFILTER_MINMAX_ABOVE_MAX:
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CStockFilterArray::IsNativeOrder()
{
	return ( ( FALSE == m_bFiltered ) && ( STOCKFILTER_SORT_FORWARD_STOCKCODE == m_nSortType ) && ( -1 == m_nStocktakeFilter ) && ( FALSE == m_bActiveItemOnly ) );
}

/**********************************************************************/

bool CStockFilterArray::HasAllRecords()
{
	return ( ( FALSE == m_bFiltered ) && ( -1 == m_nStocktakeFilter ) && ( FALSE == m_bActiveItemOnly ) );
}

/**********************************************************************/

int CStockFilterArray::GetSize()
{
	if (IsNativeOrder() == FALSE)
	{
		return m_index.GetSize();
	}
	else
	{
		return DataManager.Stock.GetSize();
	}
}

/**********************************************************************/

void CStockFilterArray::SetArrayIdx(int nArrayIdx)
{
	if (nArrayIdx >= 0 && nArrayIdx < GetSize())
	{
		m_nArrayIdx = nArrayIdx;
	}
	else
	{
		m_nArrayIdx = -1;
	}
}

/**********************************************************************/

void CStockFilterArray::Next()
{
	if (m_nArrayIdx < GetSize() - 1)
	{
		m_nArrayIdx++;
	}
}

/**********************************************************************/

void CStockFilterArray::Previous()
{
	if (m_nArrayIdx > 0)
	{
		m_nArrayIdx--;
	}
}

/**********************************************************************/

bool CStockFilterArray::HasNext()
{
	return (m_nArrayIdx < GetSize() - 1);
}

/**********************************************************************/

bool CStockFilterArray::HasPrevious()
{
	return (m_nArrayIdx > 0);
}

/**********************************************************************/

int CStockFilterArray::GetStockItemIndexAt( int nArrayIdx )
{
	if ((nArrayIdx < 0) || (nArrayIdx >= GetSize()))
	{
		return -1;
	}

	if (IsNativeOrder() == TRUE)
	{
		return nArrayIdx;
	}

	CStockIndexItem item;
	m_index.GetAt( nArrayIdx, item );

	if (item.m_nIndexVersion == m_nIndexVersion)
	{
		return item.m_nIndex;
	}

	int nStockIdx = 0;
	if (DataManager.Stock.FindStockItemByCode(item.m_strStockCode, nStockIdx) == FALSE)
	{
		return -1;
	}

	item.m_nIndex = nStockIdx;
	item.m_nIndexVersion = m_nIndexVersion;
	m_index.SetAt( nArrayIdx, item );

	return nStockIdx;
}

/**********************************************************************/

bool CStockFilterArray::FindStockItemByCode ( const char* szCode, int& nArrayIdx )
{
	bool bResult = FALSE;
	CString strCodeToFind = szCode;

	if (IsNativeOrder() == TRUE)
	{
		bResult = DataManager.Stock.FindStockItemByCode(strCodeToFind, nArrayIdx);
	}
	else if ( ( STOCKFILTER_SORT_FORWARD_STOCKCODE == m_nSortType ) ||( STOCKFILTER_SORT_REVERSE_STOCKCODE == m_nSortType ) )
	{
		CStockIndexItem item;
		item.m_nCategory = 0;
		item.m_strDescription = "";
		item.m_strStockCode = strCodeToFind;
		bResult = m_index.FindWithHint( item, m_nSortType, nArrayIdx );
	}
	else
	{
		nArrayIdx = 0;
		CString strBestMatch = "";

		for ( int n = 0; n < GetSize(); n++ )
		{
			CStockIndexItem item;
			m_index.GetAt( n, item );

			if ( item.m_strStockCode == strCodeToFind )
			{
				nArrayIdx = n;
				bResult = TRUE;
				break;
			}

			if ( ( item.m_strStockCode > strBestMatch ) && ( item.m_strStockCode < strCodeToFind ) )
			{
				strBestMatch = item.m_strStockCode;
				nArrayIdx = n;
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CStockFilterArray::GetStockItemRange( CString& strFirstStockCode, CString& strLastStockCode )
{
	strFirstStockCode = "";
	strLastStockCode = "";

	if ( GetSize() > 0 )
	{
		if ( IsNativeOrder() == TRUE )
		{
			strFirstStockCode = DataManager.Stock.GetStockCode(0);
			strLastStockCode = DataManager.Stock.GetStockCode( GetSize() - 1 );
		}
		else if ( STOCKFILTER_SORT_FORWARD_STOCKCODE == m_nSortType )
		{
			CStockIndexItem itemFirst;
			m_index.GetAt( 0, itemFirst );
			strFirstStockCode = itemFirst.m_strStockCode;
			
			CStockIndexItem itemLast;
			m_index.GetAt( GetSize() - 1, itemLast );
			strLastStockCode = itemLast.m_strStockCode;
		}
		else if ( STOCKFILTER_SORT_REVERSE_STOCKCODE == m_nSortType )
		{
			CStockIndexItem itemFirst;
			m_index.GetAt( 0, itemFirst );
			strLastStockCode = itemFirst.m_strStockCode;
			
			CStockIndexItem itemLast;
			m_index.GetAt( GetSize() - 1, itemLast );
			strFirstStockCode = itemLast.m_strStockCode;
		}
		else
		{
			for ( int n = 0; n < GetSize(); n++ )
			{
				CStockIndexItem item;
				m_index.GetAt( n, item );
				
				if ( ( 0 == n ) || ( item.m_strStockCode < strFirstStockCode ) )
					strFirstStockCode = item.m_strStockCode;

				if ( ( 0 == n ) || ( item.m_strStockCode > strLastStockCode ) )
					strLastStockCode = item.m_strStockCode;
			}
		}
	}
}

/**********************************************************************/

void CStockFilterArray::GetDefaultRange ( int nArrayIdx, CString& strStockFrom, CString& strStockTo )
{
	switch ( m_nSortType )
	{
	case STOCKFILTER_SORT_FORWARD_STOCKCODE:
	case STOCKFILTER_SORT_REVERSE_STOCKCODE:
		if ( ( nArrayIdx >= 0 ) && ( nArrayIdx < GetSize() ) )
		{
			int nStockIdx = GetStockItemIndexAt ( nArrayIdx );
			
			CString strTempFrom, strTempTo;
			GetStockItemRange( strTempFrom, strTempTo );

			if ( STOCKFILTER_SORT_FORWARD_STOCKCODE == m_nSortType )
			{
				strStockFrom = DataManager.Stock.GetStockCode ( nStockIdx );
				strStockTo = strTempTo;
			}
			else
			{
				strStockFrom = strTempFrom;
				strStockTo = DataManager.Stock.GetStockCode ( nStockIdx );
			}
		}
		else if ( GetSize() > 0 )
		{
			GetStockItemRange( strStockFrom, strStockTo );
		}
		else
		{
			strStockFrom = "";
			strStockTo = "";
		}
		break;

	default:
		GetStockItemRange( strStockFrom, strStockTo );
		break;
	}
}

/**********************************************************************/

void CStockFilterArray::GetArrayIdxRange( const char* szFrom, const char* szTo, int& nStartIdx, int& nRange )
{
	nStartIdx = 0;
	nRange = GetSize();

	if ( GetSortType() == STOCKFILTER_SORT_FORWARD_STOCKCODE )
	{
		FindStockItemByCode ( szFrom, nStartIdx );

		int nEndIdx = 0;
		if (FindStockItemByCode(szTo, nEndIdx) == FALSE)
		{
			nEndIdx--;
		}

		nRange = ( nEndIdx - nStartIdx ) + 1;
	}
	if ( GetSortType() == STOCKFILTER_SORT_REVERSE_STOCKCODE )
	{
		FindStockItemByCode ( szTo, nStartIdx );

		int nEndIdx = 0;
		if (FindStockItemByCode(szFrom, nEndIdx) == FALSE)
		{
			nEndIdx--;
		}

		nRange = ( nEndIdx - nStartIdx ) + 1;
	}
}

/**********************************************************************/

int CStockFilterArray::InsertRecord ( CStockCSVRecord& NewRecord )
{
	int nStockIdx;
	int nListIdx;

	if ( DataManager.Stock.InsertRecord ( NewRecord, nStockIdx ) == TRUE )
	{
		nListIdx = nStockIdx;

		if ( IsNativeOrder() == FALSE )
		{
			CStockIndexItem item;
			item.m_strStockCode = NewRecord.GetStockCode();
			item.m_nCategory = NewRecord.GetCategory();
			item.m_strDescription = NewRecord.GetDescription();
			item.m_nIndex = nStockIdx;
			item.m_nIndexVersion = 0;

			switch( GetSortType() )
			{
			case STOCKFILTER_SORT_FORWARD_STOCKCODE:
			case STOCKFILTER_SORT_REVERSE_STOCKCODE:
				m_index.ConsolidateWithHint( item, m_nSortType );
				m_index.FindWithHint( item, m_nSortType, nListIdx );
				break;

			default:
				m_index.DirectAdd( item );
				nListIdx = m_index.GetSize() - 1;
				break;
			}

			m_nIndexVersion++;
		}
	}
	else
	{
		nListIdx = nStockIdx;
	}

	return nListIdx;
}

/**********************************************************************/

//deletes stock items by range in the text array but does not delete items
//which have outstanding orders. returns the number of items that had to 
//be skipped to achieve this.

int CStockFilterArray::DeleteStockItemRange ( const char* szFrom, const char* szTo )
{
	CString strFrom = szFrom;
	CString strTo = szTo;

	int nSkipped = 0;

	int nStartIdx, nRange;
	GetArrayIdxRange( strFrom, strTo, nStartIdx, nRange );

	if ( IsNativeOrder() == TRUE )
	{
		for (int nOffset = nRange - 1; nOffset >= 0; nOffset--)
		{
			if (CheckStockOnOrder(nStartIdx + nOffset) == TRUE)
			{
				nSkipped++;
			}
			else
			{
				DataManager.Stock.RemoveAt(nStartIdx + nOffset);
			}
		}
	}
	else if ( STOCKFILTER_SORT_FORWARD_STOCKCODE == m_nSortType )
	{
		for ( int nOffset = nRange - 1; nOffset >= 0; nOffset-- )
		{
			int nArrayIdx = nStartIdx + nOffset;
			int nStockIdx = GetStockItemIndexAt( nArrayIdx );
			
			if (CheckStockOnOrder(nStockIdx) == TRUE)
			{
				nSkipped++;
			}
			else
			{
				DataManager.Stock.RemoveAt( nStockIdx );
				m_index.RemoveAt( nArrayIdx );
			}
		}

		m_nIndexVersion++;
	}
	else
	{
		for ( int nOffset = nRange - 1; nOffset >= 0; nOffset-- )
		{
			int nArrayIdx = nStartIdx + nOffset;
			
			CStockIndexItem item;
			m_index.GetAt( nArrayIdx, item );

			if ( ( item.m_strStockCode >= strFrom ) && ( item.m_strStockCode <= strTo ) )
			{
				int nStockIdx;
				if ( DataManager.Stock.FindStockItemByCode( item.m_strStockCode, nStockIdx ) == TRUE )
				{
					if (CheckStockOnOrder(nStockIdx) == TRUE)
					{
						nSkipped++;
					}
					else
					{
						DataManager.Stock.RemoveAt( nStockIdx );
						m_index.RemoveAt( nArrayIdx );
					}
				}
			}
		}

		m_nIndexVersion++;
	}

	return nSkipped;
}

/**********************************************************************/

//this is a helper function for the stock database dlg to use when 
//removing a single item

void CStockFilterArray::RemoveIndex ( int nIndex )
{
	if ( ( IsNativeOrder() == FALSE ) && ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		m_index.RemoveAt( nIndex );
		m_nIndexVersion++;
	}
}

/**********************************************************************/

bool CStockFilterArray::CheckStockOnOrder( int nStockIdx )
{
	CStockCSVRecord StockRecord;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );

	CStockLevelsCSVRecord StockLevels;
	DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevels );
	
	return ( StockLevels.GetQtyOnOrder() != 0 );	
}

/**********************************************************************/

bool CStockFilterArray::GetDeletionFlagAt( int nArrayIdx )
{
	if ( ( nArrayIdx < 0 ) || ( nArrayIdx >= GetSize() ) )
		return FALSE;

	if ( IsNativeOrder() == TRUE )
		return FALSE;

	CStockIndexItem item;
	m_index.GetAt( nArrayIdx, item );
	
	return item.m_bDeletionFlag;
}

/**********************************************************************/

void CStockFilterArray::SetDeletionFlagAt( int nArrayIdx, bool bFlag )
{
	if ( ( nArrayIdx < 0 ) || ( nArrayIdx >= GetSize() ) )
		return;

	if ( IsNativeOrder() == TRUE )
		return;

	CStockIndexItem item;
	m_index.GetAt( nArrayIdx, item );
	item.m_bDeletionFlag = bFlag;
	m_index.SetAt( nArrayIdx, item );
}

/**********************************************************************/

void CStockFilterArray::CreateNewStockCodeArray()
{
	m_arrayNewStockCodes.RemoveAll();
	for ( int nIndex = 0; nIndex < DataManager.Stock.GetSize(); nIndex++ )
		m_arrayNewStockCodes.Add( DataManager.Stock.GetStockCode( nIndex ) );
}

/**********************************************************************/

const char* CStockFilterArray::GetNewStockCode( int nStockIdx )
{
	m_strNewStockCode = "";
	if ( ( nStockIdx >= 0 ) && ( nStockIdx < m_arrayNewStockCodes.GetSize() ) )
		m_strNewStockCode = m_arrayNewStockCodes.GetAt( nStockIdx );

	return m_strNewStockCode;
}

/**********************************************************************/

void CStockFilterArray::SetNewStockCode( int nStockIdx, const char* szStockCode )
{
	if ( ( nStockIdx >= 0 ) && ( nStockIdx < m_arrayNewStockCodes.GetSize() ) )
		m_arrayNewStockCodes.SetAt( nStockIdx, szStockCode ); 
}

/**********************************************************************/

const char* CStockFilterArray::GetDisplayLine ( int nArrayIdx )
{
	m_strRecord ="";
	int nStockIdx = GetStockItemIndexAt( nArrayIdx );
	
	if ( nStockIdx != -1 )
		GetLine( nStockIdx, nArrayIdx );
	
	return m_strRecord;
}

/**********************************************************************/
				
void CStockFilterArray::GetLine ( int nStockIdx, int nArrayIdx )
{
	m_bNegativeStock = FALSE;
	m_MinMaxFlags.ResetResults( FALSE );

	if ( nStockIdx > DataManager.Stock.GetSize() )
	{	
		m_strRecord = "";
		return;
	}

	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord StockLevelsSp1;
	CStockLevelsCSVRecord StockLevelsSp2;
	CStockLevelsCSVRecord StockLevelsSite;
	CStockLevelsCSVRecord StockLevelsDb;
	CStockLevelsCSVRecord StockLevelsOrderBrowse;
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	CString strStockCode = StockRecord.GetStockCode();

	switch ( m_nFilterType )
	{
	case FILTERTYPE_ORDERBROWSE_PREVIEW:
		switch ( m_nOrderPreviewConLevel )
		{
		case NODE_STOCKPOINT:
			DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nOrderPreviewTableNo, StockLevelsOrderBrowse, m_MinMaxFlags );
			break;

		case NODE_SITE:
			DataManager.Stock.GetSiteTotals ( nStockIdx, StockRecord, m_nOrderPreviewTableNo, StockLevelsOrderBrowse, m_MinMaxFlags );
			break;

		case NODE_DATABASE:
		default:
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevelsOrderBrowse, m_MinMaxFlags );
			break;
		}
		break;

	case FILTERTYPE_ORDERBROWSE_NEWITEM:
	case FILTERTYPE_ORDERBROWSE_NEWORDER:
		DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevelsDb, m_MinMaxFlags );
		break;

	case FILTERTYPE_STOCK_DELETION:
		DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevelsDb );
		break;

	case FILTERTYPE_BROWSE_STOCK:
	case FILTERTYPE_DATABASE:
		if ( TRUE == m_bCheckApparent )
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, StockLevelsDb, m_MinMaxFlags );
		break;

	case FILTERTYPE_REORDER:
	case FILTERTYPE_STOCK_ITEMSET:
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevelsSp1 );
		break;

	case FILTERTYPE_OPENING:
	case FILTERTYPE_STOCKTAKE:
	case FILTERTYPE_BROWSE_OPENING:
	case FILTERTYPE_BROWSE_STOCKTAKE:
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevelsSp1, m_MinMaxFlags );
		break;

	case FILTERTYPE_TRANSFERS:
	case FILTERTYPE_BROWSE_TRANSFER:
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo, StockLevelsSp1 );
		DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, m_nSpNo2, StockLevelsSp2 );
		break;

	case FILTERTYPE_STOCKCODES:
	default:
		break;
	}

	CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
	double dSessionQty = 0.0;
	
	switch ( m_nFilterType )
	{
	case FILTERTYPE_ORDERBROWSE_PREVIEW:	
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
		{
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsOrderBrowse.GetApparentQty() ) );
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsOrderBrowse.GetMinimumLevelQty() ) );
		}
		else
		{
			csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsOrderBrowse.GetApparentQty() ) );
		}	
		m_bNegativeStock = ( StockLevelsOrderBrowse.GetApparentQty() < 0.0 );
		break;

	case FILTERTYPE_ORDERBROWSE_NEWITEM:
	case FILTERTYPE_ORDERBROWSE_NEWORDER:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetApparentQty() + StockLevelsDb.GetQtyOnOrder() ) );	
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetMinimumLevelQty() ) );
		else
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetApparentQty() ) );
		
		m_bNegativeStock = ( StockLevelsDb.GetApparentQty() < 0.0 );
		break;

	case FILTERTYPE_STOCK_DELETION:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetQtyOnOrder() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetApparentQty() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetTotalSalesQty() ) );
		
		if ( GetDeletionFlagAt( nArrayIdx ) == TRUE )
			csv.Add ( "Yes" );

		break;

	case FILTERTYPE_BROWSE_STOCK:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		
		if ( TRUE == m_bCheckApparent )
		{
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetApparentQty() ) );
			m_bNegativeStock = ( StockLevelsDb.GetApparentQty() < 0.0 );
		}

		break;

	case FILTERTYPE_DATABASE:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) );

		if ( TRUE == m_bCheckApparent )
		{
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsDb.GetApparentQty() ) );				
			m_bNegativeStock = ( StockLevelsDb.GetApparentQty() < 0.0 );
		}
		break;

	case FILTERTYPE_OPENING:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetOpeningQty() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetApparentQty() ) );	
		m_bNegativeStock = ( StockLevelsSp1.GetApparentQty() < 0.0 );
		break;

	case FILTERTYPE_STOCKTAKE:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetStocktakeApparentQty() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetStocktakeClosingQty() ) );
		m_bNegativeStock = ( StockLevelsSp1.GetStocktakeClosingQty() < 0.0 );
		break;

	case FILTERTYPE_STOCK_ITEMSET:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) );
		csv.Add ( StockLevelsSp1.GetActiveFlag() ? "Yes" : "" );
		m_bNegativeStock = ( StockLevelsSp1.GetStocktakeClosingQty() < 0.0 );
		break;

	case FILTERTYPE_REORDER:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetMinimumLevelQty() ) );
		
		if ( StockOptions.GetOrderGenerationMethod() != ORDER_TYPE_SALES )
			csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetReorderLevelQty() ) );

		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetMaximumLevelQty() ) );
		break;

	case FILTERTYPE_BROWSE_OPENING:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetOpeningQty() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetApparentQty() ) );
		m_bNegativeStock = ( StockLevelsSp1.GetApparentQty() < 0.0 );
		break;

	case FILTERTYPE_BROWSE_STOCKTAKE:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetStocktakeClosingQty() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetStocktakeApparentQty() ) );
		m_bNegativeStock = ( StockLevelsSp1.GetApparentQty() < 0.0 );
		break;

	case FILTERTYPE_TRANSFERS:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetApparentQty() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp2.GetApparentQty() ) );
		break;

	case FILTERTYPE_BROWSE_TRANSFER:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp1.GetApparentQty() ) );
		csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, StockLevelsSp2.GetApparentQty() ) );
		break;

	case FILTERTYPE_STOCKCODES:
		csv.Add ( strStockCode );
		csv.Add ( StockRecord.GetDescription() );		
		csv.Add ( DataManager.Category.GetText ( StockRecord.GetCategory() ) );
		csv.Add ( GetNewStockCode ( nStockIdx ) );
		break;

	default:
		break;
	}
	m_strRecord = csv.GetLine();
}

/**********************************************************************/

const char* CStockFilterArray::GetColumnManagerDisplayLine ( CColumnManager& ColumnManager, int nArrayIdx )
{
	m_strRecord ="";
	int nStockIdx = GetStockItemIndexAt( nArrayIdx );
	
	if ( nStockIdx != -1 )
		GetColumnManagerLine( ColumnManager, nStockIdx );
	
	return m_strRecord;
}

/**********************************************************************/

void CStockFilterArray::GetColumnManagerLine ( CColumnManager& ColumnManager, int nStockIdx )
{
	m_bNegativeStock = FALSE;
	m_MinMaxFlags.ResetResults( FALSE );
	
	if ( nStockIdx > DataManager.Stock.GetSize() )
	{	
		m_strRecord = "";
		return;
	}

	CStockCSVRecord StockRecord;
	CStockLevelsCSVRecord DbStockLevels;	
	DataManager.Stock.GetAt ( nStockIdx, StockRecord );
	
	if ( TRUE == m_bCheckApparent )
	{
		if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_LOC )
			DataManager.Stock.GetDatabaseTotals ( nStockIdx, StockRecord, DbStockLevels, m_MinMaxFlags );
		else
		{
			int nSpNo = dbStockpoint.GetSpNo( PasswordArray.GetLocationAccessSpIdx() );
			DataManager.Stock.GetStockpointTotals ( nStockIdx, StockRecord, nSpNo, DbStockLevels, m_MinMaxFlags );
		}
	}
	
	CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
	double dSessionQty = 0.0;
	int nDPValue = SysInfo.GetDPValue();

	for ( int n = 0; n < ColumnManager.GetListColumnCount(); n++ )
	{
		if ( ColumnManager.GetListColumnStatus(n) == COLUMN_STATUS_HIDDEN )
			break;
	
		int nType = ColumnManager.GetListColumnType(n);

		switch( nType )
		{
		case COLUMN_STOCKCODE:	csv.Add( StockRecord.GetStockCode() );			break;
		case COLUMN_DESC:		csv.Add( StockRecord.GetDescription() );		break;
		case COLUMN_CATEGORY:	csv.Add( DataManager.Category.GetText ( StockRecord.GetCategory() ) ); break;
		case COLUMN_STOCKUNIT:	csv.Add ( DataManager.Stock.GetStockSubUnitName( StockRecord ) ); break;
		case COLUMN_DUCOST:		csv.Add( StockRecord.GetCost(0), nDPValue );	break;
		case COLUMN_SUCOST:		csv.Add( StockRecord.GetUnitCost(), nDPValue );	break;
		case COLUMN_APPARENT:
			{
				if ( TRUE == m_bCheckApparent )
				{
					csv.Add ( DataManager.Stock.GetStockLevelString ( StockRecord, DbStockLevels.GetApparentQty() ) );				
					m_bNegativeStock = ( DbStockLevels.GetApparentQty() < 0.0 );
				}
				else
					csv.Add( "" );
			}		
			break;

		case COLUMN_SUPPLIER:
			{
				CString strSupplier = "";
				if ( StockRecord.GetSupplierCount() == 0 )
					strSupplier = "Unspecified";
				else
				{
					int nSuppIdx;
					int nSuppNo = StockRecord.GetSuppNo(0);
					if ( DataManager.Supplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
						strSupplier = "Unknown";
					else
						strSupplier = DataManager.Supplier.GetName( nSuppIdx );
				}

				csv.Add( strSupplier );
			}
			break;
		}
	}

	m_strRecord = csv.GetLine();
}

/**********************************************************************/
