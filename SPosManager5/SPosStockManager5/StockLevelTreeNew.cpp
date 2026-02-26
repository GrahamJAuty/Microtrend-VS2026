/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "NodeTypes.h"
#include "SalesHistoryFields.h"
#include "StatusProgress.h"
#include "StockOptionsIni.h"
#include "StockpointCSVArray.h"
#include "SupplierReportMap.h"
/**********************************************************************/
#include "StockLevelTreeNew.h"
/**********************************************************************/

CStockLevelBranchNew::CStockLevelBranchNew()
{
}

/**********************************************************************/

CStockLevelBranchNew::~CStockLevelBranchNew()
{
	DeleteTransferDetail();
	DeleteSalesCost();
}

/**********************************************************************/

void CStockLevelBranchNew::Clear()
{
	m_arrayKeys.RemoveAll();
	m_arrayStockLevels.RemoveAll();
	DeleteTransferDetail();
	DeleteSalesCost();
	m_arrayNextArrayIdx.RemoveAll();
}

/**********************************************************************/

void CStockLevelBranchNew::DeleteTransferDetail()
{
	for ( int n = 0; n < m_arrayTransferDetail.GetSize(); n++ )
	{
		CTransferDetailCSVRecord* pTotal = (CTransferDetailCSVRecord*) m_arrayTransferDetail.GetAt(n);

		if ( pTotal != NULL )
			delete pTotal;
	}
	m_arrayTransferDetail.RemoveAll();
}

/**********************************************************************/

void CStockLevelBranchNew::DeleteSalesCost()
{
	for ( int n = 0; n < m_arraySalesCost.GetSize(); n++ )
	{
		CSalesCostCSVRecord* pTotal = (CSalesCostCSVRecord*) m_arraySalesCost.GetAt(n);

		if ( pTotal != NULL )
			delete pTotal;
	}
	m_arraySalesCost.RemoveAll();
}

/**********************************************************************/

bool CStockLevelBranchNew::FindNodeInt( int nLabelToFind, int& nIndex )
{
	int nStart = 0;
	int nEnd = m_arrayKeys.GetSize() - 1;
	
	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		int nLabel = m_arrayKeys.GetAt(nIndex);
		
		if ( nLabel == nLabelToFind )
			return TRUE;

		if ( nLabel < nLabelToFind )
			nStart = nIndex + 1;
		else
			nEnd = nIndex - 1;
	}
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

int CStockLevelBranchNew::ConsolidateInt( int n, CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord* pTransferDetail, CSalesCostCSVRecord* pSalesCost, bool bEndNode, int nNextArrayIdx, __int64& nNodeCount )
{
	int nNodeIdx;
		
	if ( FindNodeInt ( n, nNodeIdx ) == FALSE )
	{
		nNodeCount++;

		m_arrayKeys.InsertAt( nNodeIdx, n );
		
		if ( bEndNode == FALSE )
		{
			m_arrayNextArrayIdx.InsertAt( nNodeIdx, nNextArrayIdx );
			return nNextArrayIdx;
		}
		else
		{
			m_arrayStockLevels.InsertAt( nNodeIdx, StockLevels );

			if ( pTransferDetail != NULL )
			{
				CTransferDetailCSVRecord* pTransferDetailArray = new CTransferDetailCSVRecord;
				if ( pTransferDetailArray != NULL )
					*pTransferDetailArray = *pTransferDetail;
				
				m_arrayTransferDetail.InsertAt( nNodeIdx, pTransferDetailArray );
			}

			if ( pSalesCost != NULL )
			{
				CSalesCostCSVRecord* pSalesCostArray = new CSalesCostCSVRecord;
				if ( pSalesCostArray != NULL )
					*pSalesCostArray = *pSalesCost;

				m_arraySalesCost.InsertAt( nNodeIdx, pSalesCostArray );
			}

			m_arrayNextArrayIdx.InsertAt( nNodeIdx, 0 );
			return 0;
		}
	}
	else
	{
		if ( bEndNode == FALSE )
			return m_arrayNextArrayIdx[nNodeIdx];
		else
		{
			m_arrayStockLevels[ nNodeIdx ] = m_arrayStockLevels[ nNodeIdx ] + StockLevels;

			if ( pTransferDetail != NULL )
			{
				CTransferDetailCSVRecord* pTransferDetailArray = (CTransferDetailCSVRecord*) m_arrayTransferDetail[ nNodeIdx ];
				if ( pTransferDetail != NULL )
					*pTransferDetailArray = *pTransferDetailArray + *pTransferDetail;
			}

			if ( pSalesCost != NULL )
			{
				CSalesCostCSVRecord* pSalesCostArray = (CSalesCostCSVRecord*) m_arraySalesCost[ nNodeIdx ];
				if ( pSalesCost != NULL )
					*pSalesCostArray = *pSalesCostArray + *pSalesCost;
			}

			return 0;
		}
	}
}

/**********************************************************************/

CStockLevelTreeNew::CStockLevelTreeNew() : CPtrArray()
{
	m_nNodeCount = 0;
	
	m_nDbIdx = 0;
	m_nConLevel = NODE_DATABASE;
	m_nTableNo = dbDatabase.GetDbNo(0);
	m_nStocktakeNo = 0;
	
	m_bSummary = FALSE;
	m_nCategoryFilter = 0;
	
	m_arraySuppIdx.RemoveAll();
	m_bAllSupp = FALSE;

	m_bWantSalesCost = FALSE;
	m_bGotSalesCost = FALSE;

	m_bWantTransferDetail = FALSE;
	m_bGotTransferDetail = FALSE;
	
	m_strStockCodeFrom = "";
	m_strStockCodeTo = "";
	m_bExcludeNonMovers = FALSE;
	m_bItemFilter = TRUE;

	m_nSortType = 0;

	m_pStockFilterArray = NULL;
	
	m_bInvalid = TRUE;
}

/**********************************************************************/

CStockLevelTreeNew::~CStockLevelTreeNew()
{
	int nSize = GetSize();
	
	StatusProgress.Lock( TRUE, "Clearing temporary data" );
	
	for ( int n = nSize - 1; n >= 0; n-- )
	{
		StatusProgress.SetPos( nSize - n, nSize );  
		delete ( GetBranch( n ) );
	}
	
	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockLevelTreeNew::Clear()
{
	int nSize = GetSize();
	
	StatusProgress.Lock( TRUE, "Clearing temporary data" );
	
	for ( int n = nSize - 1; n >= 0; n-- )
	{
		StatusProgress.SetPos( nSize - n, nSize );  
		delete ( GetBranch( n ) );
		RemoveAt(n);
	}

	StatusProgress.Unlock();
	
	m_bufferInt.RemoveAll();
	m_nNodeCount = 0;
	m_totalLevels.Reset();
}

/**********************************************************************/

void CStockLevelTreeNew::ConsolidateStockLevels( CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord* pTransferDetail, CSalesCostCSVRecord* pSalesCost )
{
	int nArrayToSearch = 0;
	int nFieldCount = m_bufferInt.GetSize();
	
	if ( ( GetSize() == 0 ) && ( nFieldCount != 0  ) )
	{
		CStockLevelBranchNew* pArray = new CStockLevelBranchNew;
		Add( pArray );
	}

	m_totalLevels = m_totalLevels + StockLevels;

	for ( int nPos = 0; nPos < nFieldCount; nPos++ )
	{
		bool bEndNode = ( nPos == nFieldCount - 1 );
		CStockLevelBranchNew* pStockLevelBranch = GetBranch( nArrayToSearch );
		
		int nField = m_bufferInt.GetAt( nPos );
		nArrayToSearch = pStockLevelBranch -> ConsolidateInt( nField, StockLevels, pTransferDetail, pSalesCost, bEndNode, GetSize(), m_nNodeCount );
		
		if ( nArrayToSearch == GetSize() )
		{
			CStockLevelBranchNew* pArray = new CStockLevelBranchNew;
			Add( pArray );
		}
	}
}

/**********************************************************************/

int CStockLevelTreeNew::GetArraySize( int nArrayIdx )
{
	if ( nArrayIdx >= 0 && nArrayIdx < GetSize() )
		return GetBranch( nArrayIdx ) -> m_arrayKeys.GetSize();
	else
		return 0;
}

/**********************************************************************/

int CStockLevelTreeNew::GetLabelInt( int nArrayIdx, int nNodeIdx )
{
	return GetBranch( nArrayIdx ) -> m_arrayKeys.GetAt(nNodeIdx);
}

/**********************************************************************/

CStockLevelsCSVRecord CStockLevelTreeNew::GetStockLevels( int nArrayIdx, int nNodeIdx )
{
	return GetBranch( nArrayIdx ) -> m_arrayStockLevels[nNodeIdx];
}

/**********************************************************************/

void CStockLevelTreeNew::GetTransferDetail( int nArrayIdx, int nNodeIdx, CTransferDetailCSVRecord& Detail )
{
	CTransferDetailCSVRecord* pDetail = (CTransferDetailCSVRecord*) ( GetBranch( nArrayIdx ) -> m_arrayTransferDetail[nNodeIdx] );

	if ( pDetail != NULL )
		Detail = *pDetail;
}

/**********************************************************************/

void CStockLevelTreeNew::GetSalesCost( int nArrayIdx, int nNodeIdx, CSalesCostCSVRecord& SalesCost )
{
	CSalesCostCSVRecord* pSalesCost = (CSalesCostCSVRecord*) ( GetBranch( nArrayIdx ) -> m_arraySalesCost[nNodeIdx] );

	if ( pSalesCost != NULL )
		SalesCost = *pSalesCost;
}

/**********************************************************************/

int CStockLevelTreeNew::GetNextArrayIdx( int nArrayIdx, int nNodeIdx )
{
	return GetBranch( nArrayIdx ) -> m_arrayNextArrayIdx[nNodeIdx];
}

/**********************************************************************/

void CStockLevelTreeNew::Consolidate()
{
	dbDatabase.BuildSortIdxTables();
	dbStockpoint.BuildSortIdxTables();

	StatusProgress.Lock( TRUE, "Consolidating stock levels" );
	
	if ( m_bAllSupp == FALSE )
		ConsolidateOneSupplierPerItem();
	else
		ConsolidateMultiSuppliersPerItem();

	StatusProgress.Unlock();
}

/**********************************************************************/

void CStockLevelTreeNew::ConsolidateOneSupplierPerItem()
{
	if ( m_bInvalid == FALSE )
		return;

	m_bGotSalesCost = m_bWantSalesCost;
	m_bGotTransferDetail = m_bWantTransferDetail;

	Clear();

	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet( m_nCategoryFilter );

	bool bSupplierFilter = TRUE;
	
	if ( m_arraySuppIdx.GetSize() == DataManager.Supplier.GetSize() + 1 )
	{
		bSupplierFilter = FALSE;

		for ( int n = 0; n < m_arrayReportFieldTypes.GetSize(); n++ )
		{
			if ( SH_FIELD_SUPP == m_arrayReportFieldTypes.GetAt(n) )
			{
				bSupplierFilter = TRUE;
				break;
			}
		}
	}

	CSupplierReportMap SupplierReportMap;
	
	if ( bSupplierFilter == TRUE )
		SupplierReportMap.CreateMap( m_arraySuppIdx );

	m_bufferInt.RemoveAll();
	for( int n = 0; n < m_arrayReportFieldTypes.GetSize(); n++ )
		m_bufferInt.Add(0);

	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartSpIdx, nEndSpIdx );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );
	
	CWordArray arraySpIdx;
	for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		if ( MatchStockpoint( nSpIdx ) == TRUE )
			arraySpIdx.Add( nSpIdx );

	//FILTER BY STOCK CODE
	int nStartIdx = 0;
	int nEndIdx = DataManager.Stock.GetSize() - 1;

	if ( m_strStockCodeFrom != "" )
		DataManager.Stock.FindStockItemByCode( m_strStockCodeFrom, nStartIdx );

	if ( m_strStockCodeTo != "" )
		if ( DataManager.Stock.FindStockItemByCode( m_strStockCodeTo, nEndIdx ) == FALSE )
			nEndIdx--;

	__int64 nItems = ( nEndIdx - nStartIdx ) + 1;

	if ( nItems < 0 ) 
		nItems = 0;

	__int64 nTarget = nItems * arraySpIdx.GetSize();
	__int64 nProgress = 0;

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		nProgress = ( nItems * n );
		StatusProgress.SetPos( nProgress, nTarget );

		int nSpIdx = arraySpIdx.GetAt(n);

		CString str;
		str.Format ( "(%s)", dbStockpoint.GetName( nSpIdx ) );
		StatusProgress.SetDialogText( str );
		
		bool bCanCloseStockLevels = FALSE;
		bool bCanCloseTransferDetail = FALSE;

		if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info ) == TRUE )
			bCanCloseStockLevels = TRUE;
		else if ( info.m_nErrorType != DATAMANAGER_ERROR_ALREADYOPEN )
			continue;

		if ( DataManager.OpenTransferDetail( nSpIdx, DATAMANAGER_OPEN_READONLY, info ) == TRUE )
			bCanCloseTransferDetail = TRUE;
		else if ( info.m_nErrorType != DATAMANAGER_ERROR_ALREADYOPEN )
		{
			if ( TRUE == bCanCloseStockLevels )
				DataManager.CloseStockLevels( nSpIdx, info );

			continue;
		}

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( m_nDbIdx );
		int nSortSpIdx = dbStockpoint.GetSortIdxFromSpIdx( nSpIdx );
		
		//THESE FIELDS STAY THE SAME THROUGHOUT THE STOCKPOINT
		for ( int n = 0; n < m_arrayReportFieldTypes.GetSize(); n++ )
		{
			switch( m_arrayReportFieldTypes.GetAt(n) )
			{
			case SH_FIELD_DBASE:m_bufferInt.SetAt ( n, nSortDbIdx );	break;
			case SH_FIELD_SITE:	m_bufferInt.SetAt ( n, dbStockpoint.GetSiteNo( nSpIdx ) ); break;
			case SH_FIELD_SP:	m_bufferInt.SetAt ( n, nSortSpIdx ); break; 
			}
		}

		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

		for ( int nStockIdx = nStartIdx; nStockIdx <= nEndIdx; nStockIdx++ )
		{
			StatusProgress.SetPos( nProgress++, nTarget );

			if ( m_StockArrayIndexer.GetSortedIdxForNativeIdx( nStockIdx ) == -1 )
				continue;

			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			
			//FILTER BY CATEGORY
			int nCategory = StockRecord.GetCategory();
			if ( CategorySet.GetCategoryStatus( nCategory ) == FALSE )
				continue;

			//FILTER BY CUSTOM FIELD
			int nCustomFieldSortIdx[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
			for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
				nCustomFieldSortIdx[nFieldNo] = CUSTOM_FIELD_SORTIDX_NA;

			bool bNoCustomFieldMatch = FALSE;

			for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
			{
				int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
				int nValueNo = StockRecord.GetCustomFieldValueNo( nFieldNo );
				int nSortIdx = DataManager.CustomFieldManager.GetSortIdxByValueNo( nSlot, nValueNo );
	
				if ( nSortIdx == -1 )
				{
					bNoCustomFieldMatch = TRUE;
					break;
				}

				nCustomFieldSortIdx[ nFieldNo ] = nSortIdx;
			}

			if ( TRUE == bNoCustomFieldMatch )
				continue;

			//FILTER BY SUPP NO
			int nSuppSortIdx = 0;
			
			if ( bSupplierFilter == TRUE )
			{
				nSuppSortIdx = SupplierReportMap.GetSortIdxBySuppNo( StockRecord.GetSuppNo(0) );
				if ( nSuppSortIdx == -1 )
					continue;
			}
			
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
			
			//FILTER INACTIVE ITEMS
			if ( ( TRUE == m_bExcludeNonMovers ) && ( StockLevels.HasStockMovements() == FALSE ) )
				continue;

			//FILTER NON STOCKPOINT ITEMS
			if ( ( m_bItemFilter == TRUE ) && ( StockLevels.GetActiveFlag() == FALSE ) )
				continue;

			//FILTER BY STOCKTAKE NO
			if ( ( m_nStocktakeNo != 0 ) && ( m_nStocktakeNo != StockLevels.GetStocktakeNo() ) )
				continue;

			for ( int n = 0; n < m_arrayReportFieldTypes.GetSize(); n++ )
			{	
				int nFieldType = m_arrayReportFieldTypes.GetAt(n); 
				switch ( nFieldType )
				{
				case SH_FIELD_CODE:		m_bufferInt.SetAt( n, m_StockArrayIndexer.GetSortedIdxForNativeIdx( nStockIdx ) );		break;
				case SH_FIELD_CAT_ITEM:	m_bufferInt.SetAt( n, nCategory );		break;
				case SH_FIELD_SUPP:		m_bufferInt.SetAt( n, nSuppSortIdx );	break;
				case SH_FIELD_CUSTOM1:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[1] );	break;
				case SH_FIELD_CUSTOM2:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[2] );	break;
				case SH_FIELD_CUSTOM3:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[3] );	break;
				case SH_FIELD_CUSTOM4:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[4] );	break;
				case SH_FIELD_CUSTOM5:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[5] );	break;
				}
			}

			if ( TRUE == m_bWantTransferDetail )
			{
				CTransferDetailCSVRecord TransferDetail;
				DataManager.Stock.GetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );
				ConsolidateStockLevels( StockLevels, &TransferDetail, NULL );
			}
			else if ( TRUE == m_bWantSalesCost )
			{
				CSalesCostCSVRecord SalesCost;
				SalesCost.SetCosts( StockRecord, StockLevels, StockOptions.GetStockSubUnitsFlag() );
				ConsolidateStockLevels( StockLevels, NULL, &SalesCost );		
			}
			else
				ConsolidateStockLevels( StockLevels, NULL, NULL );
		}

		if ( TRUE == bCanCloseStockLevels )
		{
			CDataManagerInfo info;
			DataManager.CloseStockLevels( nSpIdx, info );
		}

		if ( TRUE == bCanCloseTransferDetail )
		{
			CDataManagerInfo info;
			DataManager.CloseTransferDetail( nSpIdx, info );
		}
	}

	m_bInvalid = FALSE;
}

/**********************************************************************/

void CStockLevelTreeNew::ConsolidateMultiSuppliersPerItem()
{
	if ( m_bInvalid == FALSE )
		return;

	m_bGotSalesCost = m_bWantSalesCost;
	m_bGotTransferDetail = m_bWantTransferDetail;

	Clear();

	CCategorySetCSVRecord CategorySet;
	CategorySet.CreateCategorySet( m_nCategoryFilter );

	CSupplierReportMap SupplierReportMap;
	SupplierReportMap.CreateMap( m_arraySuppIdx );

	m_bufferInt.RemoveAll();
	for( int n = 0; n < m_arrayReportFieldTypes.GetSize(); n++ )
		m_bufferInt.Add(0);

	int nStartSpIdx, nEndSpIdx;
	dbStockpoint.GetSpIdxRange( m_nDbIdx, nStartSpIdx, nEndSpIdx );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( m_nDbIdx, info, FALSE );
	
	CWordArray arraySpIdx;
	for ( int nSpIdx = nStartSpIdx; nSpIdx <= nEndSpIdx; nSpIdx++ )
		if ( MatchStockpoint( nSpIdx ) == TRUE )
			arraySpIdx.Add( nSpIdx );

	//FILTER BY STOCK CODE
	int nStartIdx = 0;
	int nEndIdx = DataManager.Stock.GetSize() - 1;

	if ( m_strStockCodeFrom != "" )
		DataManager.Stock.FindStockItemByCode( m_strStockCodeFrom, nStartIdx );

	if ( m_strStockCodeTo != "" )
		if ( DataManager.Stock.FindStockItemByCode( m_strStockCodeTo, nEndIdx ) == FALSE )
			nEndIdx--;

	__int64 nItems = ( nEndIdx - nStartIdx ) + 1;
	
	if ( nItems < 0 ) 
		nItems = 0;

	__int64 nTarget = nItems * arraySpIdx.GetSize();
	__int64 nProgress = 0;

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		nProgress = ( nItems * n );
		StatusProgress.SetPos( nProgress, nTarget );

		int nSpIdx = arraySpIdx.GetAt(n);

		CString str;
		str.Format ( "(%s)", dbStockpoint.GetName( nSpIdx ) );
		StatusProgress.SetDialogText( str );

		bool bCanCloseStockLevels = FALSE;
		bool bCanCloseTransferDetail = FALSE;

		if ( DataManager.OpenStockLevels( nSpIdx, DATAMANAGER_OPEN_READONLY, info ) == TRUE )
			bCanCloseStockLevels = TRUE;
		else if ( info.m_nErrorType != DATAMANAGER_ERROR_ALREADYOPEN )
			continue;

		if ( DataManager.OpenTransferDetail( nSpIdx, DATAMANAGER_OPEN_READONLY, info ) == TRUE )
			bCanCloseTransferDetail = TRUE;
		else if ( info.m_nErrorType != DATAMANAGER_ERROR_ALREADYOPEN )
		{
			if ( TRUE == bCanCloseStockLevels )
				DataManager.CloseStockLevels( nSpIdx, info );

			continue;
		}

		int nSortDbIdx = dbDatabase.GetSortIdxFromDbIdx( m_nDbIdx );
		int nSortSpIdx = dbStockpoint.GetSortIdxFromSpIdx( nSpIdx );
		
		//THESE FIELDS STAY THE SAME THROUGHOUT THE STOCKPOINT
		for ( int n = 0; n < m_arrayReportFieldTypes.GetSize(); n++ )
		{
			switch( m_arrayReportFieldTypes.GetAt(n) )
			{
			case SH_FIELD_DBASE:m_bufferInt.SetAt ( n, nSortDbIdx );	break;
			case SH_FIELD_SITE:	m_bufferInt.SetAt ( n, dbStockpoint.GetSiteNo( nSpIdx ) ); break;
			case SH_FIELD_SP:	m_bufferInt.SetAt ( n, nSortSpIdx ); break; 
			}
		}

		CStockCSVRecord StockRecord;
		CStockLevelsCSVRecord StockLevels;
		CTransferDetailCSVRecord TransferDetail;
		int nSpNo = dbStockpoint.GetSpNo( nSpIdx );

		for ( int nStockIdx = nStartIdx; nStockIdx <= nEndIdx; nStockIdx++ )
		{
			StatusProgress.SetPos( nProgress++, nTarget );

			if ( m_StockArrayIndexer.GetSortedIdxForNativeIdx( nStockIdx ) == -1 )
				continue;

			DataManager.Stock.GetAt( nStockIdx, StockRecord );
			
			//FILTER BY CATEGORY
			int nCategory = StockRecord.GetCategory();
			if ( CategorySet.GetCategoryStatus( nCategory ) == FALSE )
				continue;
			
			DataManager.Stock.GetStockpointTotals( nStockIdx, StockRecord, nSpNo, StockLevels );
			DataManager.Stock.GetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );

			//FILTER BY CUSTOM FIELD
			int nCustomFieldSortIdx[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
			for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
				nCustomFieldSortIdx[nFieldNo] = CUSTOM_FIELD_SORTIDX_NA;

			bool bNoCustomFieldMatch = FALSE;

			for ( int nSlot = 0; nSlot < DataManager.CustomFieldManager.GetSlotCount(); nSlot++ )
			{
				int nFieldNo = DataManager.CustomFieldManager.GetFieldNoBySlot( nSlot );
				int nValueNo = StockRecord.GetCustomFieldValueNo( nFieldNo );
				int nSortIdx = DataManager.CustomFieldManager.GetSortIdxByValueNo( nSlot, nValueNo );
	
				if ( nSortIdx == -1 )
				{
					bNoCustomFieldMatch = TRUE;
					break;
				}

				nCustomFieldSortIdx[ nFieldNo ] = nSortIdx;
			}

			if ( TRUE == bNoCustomFieldMatch )
				continue;
			
			//FILTER INACTIVE ITEMS
			if ( ( TRUE == m_bExcludeNonMovers ) && StockLevels.HasStockMovements() == FALSE )
				continue;

			//FILTER NON STOCKPOINT ITEMS
			if ( ( m_bItemFilter == TRUE ) && ( StockLevels.GetActiveFlag() == FALSE ) )
				continue;

			//FILTER BY SUPP NO
			CWordArray arraySuppSortIdx;
			for ( int n = 0; n < StockRecord.GetSupplierCount(); n++ )
			{
				int nSuppSortIdx = SupplierReportMap.GetSortIdxBySuppNo( StockRecord.GetSuppNo(n) );
				if ( nSuppSortIdx == -1 )
					continue;

				bool bFound = FALSE;
				for ( int x = 0; x < arraySuppSortIdx.GetSize(); x++ )
				{
					if ( arraySuppSortIdx.GetAt(x) == nSuppSortIdx )
					{
						bFound = TRUE;
						break;
					}
				}

				if ( bFound == FALSE )
					arraySuppSortIdx.Add( nSuppSortIdx );
			}

			//HANDLE UNSPECIFIED SUPPLIER
			if ( StockRecord.GetSupplierCount() == 0 )
			{
				int nSuppSortIdx = SupplierReportMap.GetSortIdxBySuppNo(0);
				if ( nSuppSortIdx != -1 )
					arraySuppSortIdx.Add( nSuppSortIdx );
			}

			int nSuppFieldPos = -1;
			for ( int n = 0; n < m_arrayReportFieldTypes.GetSize(); n++ )
			{
				int nFieldType = m_arrayReportFieldTypes.GetAt(n);
				switch ( nFieldType )
				{
				case SH_FIELD_CODE:		m_bufferInt.SetAt( n, m_StockArrayIndexer.GetSortedIdxForNativeIdx( nStockIdx ) );		break;
				case SH_FIELD_CAT_ITEM:	m_bufferInt.SetAt( n, nCategory );		break;
				case SH_FIELD_SUPP:		nSuppFieldPos = n;						break;
				case SH_FIELD_CUSTOM1:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[1] );	break;
				case SH_FIELD_CUSTOM2:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[2] );	break;
				case SH_FIELD_CUSTOM3:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[3] );	break;
				case SH_FIELD_CUSTOM4:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[4] );	break;
				case SH_FIELD_CUSTOM5:	m_bufferInt.SetAt( n, nCustomFieldSortIdx[5] );	break;
				}
			}
			
			if ( nSuppFieldPos != -1 )
			{
				for ( int n = 0; n < arraySuppSortIdx.GetSize(); n++ )
				{
					m_bufferInt.SetAt( nSuppFieldPos, arraySuppSortIdx.GetAt(n) );

					if ( TRUE == m_bWantTransferDetail )
					{
						CTransferDetailCSVRecord TransferDetail;
						DataManager.Stock.GetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );
						ConsolidateStockLevels( StockLevels, &TransferDetail, NULL );
					}
					else if ( TRUE == m_bWantSalesCost )
					{
						CSalesCostCSVRecord SalesCost;
						SalesCost.SetCosts( StockRecord, StockLevels, StockOptions.GetStockSubUnitsFlag() );
						ConsolidateStockLevels( StockLevels, NULL, &SalesCost );		
					}
					else
						ConsolidateStockLevels( StockLevels, NULL, NULL );
				}
			}
			else if ( arraySuppSortIdx.GetSize() > 0 )
			{
				if ( TRUE == m_bWantTransferDetail )
				{
					CTransferDetailCSVRecord TransferDetail;
					DataManager.Stock.GetTransferTotals( nStockIdx, StockRecord, nSpNo, TransferDetail );
					ConsolidateStockLevels( StockLevels, &TransferDetail, NULL );
				}
				else if ( TRUE == m_bWantSalesCost )
				{
					CSalesCostCSVRecord SalesCost;
					SalesCost.SetCosts( StockRecord, StockLevels, StockOptions.GetStockSubUnitsFlag() );
					ConsolidateStockLevels( StockLevels, NULL, &SalesCost );		
				}
				else
					ConsolidateStockLevels( StockLevels, NULL, NULL );
			}
		}

		if ( TRUE == bCanCloseStockLevels )
		{
			CDataManagerInfo info;
			DataManager.CloseStockLevels( nSpIdx, info );
		}

		if ( TRUE == bCanCloseTransferDetail )
		{
			CDataManagerInfo info;
			DataManager.CloseTransferDetail( nSpIdx, info );
		}
	}

	m_bInvalid = FALSE;
}

/**********************************************************************/

void CStockLevelTreeNew::Configure( int nSortType, CStockFilterArray* pFilterArray, CLocationSelectorEntity& LocSelEntity, int nStocktakeNo, CWordArray& arraySortOrder, bool bShowItems, int nCatFilter, const char* szFrom, const char* szTo, bool bExcludeNonMovers, bool bItemFilter, CWordArray& arraySuppIdx, bool bAllSupp, bool bWantTransferDetail, bool bWantSalesCost )
{
	int nDbIdx = LocSelEntity.GetDbIdx();
	int nConLevel = LocSelEntity.GetConType();
	int nTableNo = LocSelEntity.GetEntityNo();

	bool bRebuildIndexer = FALSE;

	if ( ( nSortType < 0 ) || ( nSortType > 2 ) )
		nSortType = 0;

	if ( ( 2 == nSortType ) && ( NULL == pFilterArray ) )
		nSortType = 0;

	if ( m_nDbIdx != nDbIdx )
	{
		m_nDbIdx = nDbIdx;
		bRebuildIndexer = ( nSortType != 0 );
		m_bInvalid = TRUE;
	}

	if ( nSortType != m_nSortType )
	{
		m_nSortType = nSortType;
		bRebuildIndexer = ( nSortType != 0 );
		m_bInvalid = TRUE;
	}

	if ( TRUE == bRebuildIndexer )
	{
		if ( 1 == m_nSortType )
			m_StockArrayIndexer.RebuildIndex( m_nDbIdx );
		else
			m_StockArrayIndexer.RebuildIndex( pFilterArray );
	}

	m_StockArrayIndexer.SetNativeOrderFlag( 0 == m_nSortType );

	if ( m_nConLevel != nConLevel )
	{
		m_nConLevel = nConLevel;
		m_bInvalid = TRUE;
	}

	if ( m_nTableNo != nTableNo )
	{
		m_nTableNo = nTableNo;
		m_bInvalid = TRUE;
	}

	if ( bShowItems == m_bSummary )
	{
		m_bSummary = ( bShowItems == FALSE );
		m_bInvalid = TRUE;
	}

	if ( nCatFilter != m_nCategoryFilter )
	{
		m_nCategoryFilter = nCatFilter;
		m_bInvalid = TRUE;
	}

	if ( m_strStockCodeFrom != szFrom )
	{
		m_strStockCodeFrom = szFrom;
		m_bInvalid = TRUE;
	}

	if ( m_strStockCodeTo != szTo )
	{
		m_strStockCodeTo = szTo;
		m_bInvalid = TRUE;
	}

	if ( bExcludeNonMovers != m_bExcludeNonMovers )
	{
		m_bExcludeNonMovers = bExcludeNonMovers;
		m_bInvalid = TRUE;
	}

	if ( bItemFilter != m_bItemFilter )
	{
		m_bItemFilter = bItemFilter;
		m_bInvalid = TRUE;
	}
	
	if ( bAllSupp != m_bAllSupp )
	{
		m_bAllSupp = bAllSupp;
		m_bInvalid = TRUE;
	}

	if ( m_nStocktakeNo != nStocktakeNo )
	{
		m_nStocktakeNo = nStocktakeNo;
		m_bInvalid = TRUE;
	}

	bool bSameSuppIdxArray = TRUE;
	if ( arraySuppIdx.GetSize() != m_arraySuppIdx.GetSize() )
		bSameSuppIdxArray = FALSE;
	else
	{
		for ( int n = 0; n < arraySuppIdx.GetSize(); n++ )
		{
			if ( arraySuppIdx.GetAt(n) != m_arraySuppIdx.GetAt(n) )
			{
				bSameSuppIdxArray = FALSE;
				break;
			}
		}
	}

	if ( bSameSuppIdxArray == FALSE )
	{
		m_arraySuppIdx.RemoveAll();
		for ( int n = 0; n < arraySuppIdx.GetSize(); n++ )
			m_arraySuppIdx.Add( arraySuppIdx.GetAt(n) );

		m_bInvalid = TRUE;
	}

	m_bWantSalesCost = bWantSalesCost;
	if ( TRUE == m_bWantSalesCost )
		if ( FALSE == m_bGotSalesCost )
			m_bInvalid = TRUE;

	m_bWantTransferDetail = bWantTransferDetail;
	if ( TRUE == m_bWantTransferDetail )
		if ( FALSE == m_bGotTransferDetail )
			m_bInvalid = TRUE;

	if ( FALSE == m_bInvalid )
	{
		if ( ( arraySortOrder.GetSize() + 1 ) != m_arrayReportFields.GetSize() )
			m_bInvalid = TRUE;
		else
		{
			for ( int n = 0; n < arraySortOrder.GetSize(); n++ )
			{
				if ( arraySortOrder.GetAt(n) != m_arrayReportFields.GetAt(n) )
				{
					m_bInvalid = TRUE;
					break;
				}
			}
		}
	}

	if ( DataManager.CustomFieldManager.UpdateValueMapsFromComboSelectorsDatabase() == TRUE )
		m_bInvalid = TRUE;

	if ( TRUE == m_bInvalid )
	{
		m_arrayReportFields.RemoveAll();
	
		for ( int n = 0; n < arraySortOrder.GetSize(); n++ )
			m_arrayReportFields.Add( arraySortOrder.GetAt(n) );

		int nSize = m_arrayReportFields.GetSize();
			
		if ( ( m_bSummary == TRUE ) && ( nSize > 0 ) )
		{
			int nField = m_arrayReportFields.GetAt( nSize - 1 );
			int nFieldType = nField % 100;
			m_arrayReportFields.SetAt( nSize - 1, nFieldType + SH_FIELD_TOTAL );
		}
			
		m_arrayReportFields.Add( SH_FIELD_CODE );

		m_arrayReportFieldTypes.RemoveAll();
		for ( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
			m_arrayReportFieldTypes.Add( m_arrayReportFields.GetAt(n) % 100 );
	}
}

/**********************************************************************/

bool CStockLevelTreeNew::MatchStockpoint ( int nSpIdx )
{
	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		return FALSE;

	int nDbNo = dbDatabase.GetDbNo( m_nDbIdx );
	if ( dbStockpoint.GetDbNo( nSpIdx ) != nDbNo )
		return FALSE;

	bool bMatch = FALSE;
	switch ( m_nConLevel )
	{
	case NODE_STOCKPOINT:	bMatch = ( dbStockpoint.GetSpNo( nSpIdx ) == m_nTableNo );		break;
	case NODE_SITE:			bMatch = ( dbStockpoint.GetSiteNo( nSpIdx ) == m_nTableNo );	break;		
	case NODE_DATABASE:		bMatch = TRUE;		break;
	default:				bMatch = FALSE;	break;
	}
	return bMatch;
}

/**********************************************************************/

void CStockLevelTreeNew::GetReportFields( CWordArray& array )
{
	array.RemoveAll();
	for( int n = 0; n < m_arrayReportFields.GetSize(); n++ )
		array.Add( m_arrayReportFields.GetAt(n) );
}

/**********************************************************************/

void CStockLevelTreeNew::GetSuppIdxArray( CWordArray& array )
{
	array.RemoveAll();
	for( int n = 0; n < m_arraySuppIdx.GetSize(); n++ )
		array.Add( m_arraySuppIdx.GetAt(n) );
}

/**********************************************************************/
