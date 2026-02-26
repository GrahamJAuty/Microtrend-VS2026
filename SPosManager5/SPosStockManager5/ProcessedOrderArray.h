#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "Consolidation.h"
#include "ProcessedOrder.h"
#include "ProcessedOrderEntity.h"
#include "ProcessedOrderFilterIndex.h"
#include "ProcessedOrderHeader.h"
#include "ProcessedOrderItem.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CProcessedOrderArray
{
public:
	CProcessedOrderArray(void);

public:
	bool IsModified(){ return m_bModified; }
	void MarkModifiedOrder( CProcessedOrderEntity& entity );

public:
	int GetOrderCount(){ return m_arrayOrderHeaders.GetSize(); }
	int GetItemCount(){ return m_arrayOrderItems.GetSize(); }
	
public:
	bool FindHeader( CProcessedOrderEntity& entity, int& nPos );
	void GetItemIdxRange( CProcessedOrderEntity& entity, int& nStartIdx, int& nEndIdx );

private:
	void FindFirstItem( CProcessedOrderEntity& entity, int& nPos );
	void FindLastItem( CProcessedOrderEntity& entity, int& nPos );
	
public:
	void GetOrder( CProcessedOrderEntity& entity, CProcessedOrder& Order );
	void GetItemRecord( int nIndex, CProcessedOrderItem& item );
	void SetItemRecord( int nIndex, CProcessedOrderItem& item );
	void GetHeaderRecord( int nIndex, CProcessedOrderHeader& header );
	void SetHeaderRecord( int nIndex, CProcessedOrderHeader& header );

public:
	void GetHeaderEntity( int nIndex, CProcessedOrderEntity& entity );
	
public:
	int AddItem( CProcessedOrderEntity& entity, CProcessedOrderItem& item, int nInsertAfterIdx = -1 );
	void AddHeader( CProcessedOrderEntity& entity, int nSuppNo );

public:
	int RemoveItem( CProcessedOrderEntity& entity, int nItemIdx );
	int RemoveAllUndeliveredItems( CProcessedOrderEntity& entity );
	int RemoveAllUndeliveredItems();
	
private:
	void CancelItemOrder( int nItemIdx );
	
public:
	void LoadDatabaseOrders( int nSuppNo, int nDbIdx, const char* szDateFrom, const char* szDateTo, int nOrderType, bool bLoadOrders );
	void LoadStockpointOrders( int nSuppNo, int nSpIdx, const char* szDateFrom, const char* szDateTo, int nOrderType, bool bLoadOrders );
	bool CheckForOpenOrders( int nSuppNo, int nConLevel, int nTableIdx );
	void SaveOrders();

private:
	void LoadOrders( int nSuppNo, int nConLevel, int nTableIdx, const char* szDateFrom, const char* szDateTo, int nOrderType, bool bLoadOrders );
	
private:
	CReportConsolidationArray<CProcessedOrderEntity> m_arrayOrderHeaders;
	CReportConsolidationArray<CProcessedOrderEntity> m_arrayOrderItems;

private:
	CString m_strArrayLineLabel;
	
private:
	int m_nConLevel;
	int m_nDbNo;
	int m_nSpNo;
	int m_nSuppNo;
	bool m_bModified;
};

/**********************************************************************/
#endif
/**********************************************************************/
