#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderArray.h"
#include "ProcessedOrderEntity.h"
#include "ProcessedOrderFilterIndex.h"
#include "StockFilterArray.h"
/**********************************************************************/

class CProcessedOrderFilterArray 
{
public:
	CProcessedOrderFilterArray( CProcessedOrderArray& OrderArray ); 
	
	int GetSize(); 
	int GetItemsMatchingOrderStatus(){ return m_nItemsMatchingOrderStatus; }
	int GetItemIndexAt( int nIndex );
	int GetHeaderIndexAt( int nIndex );
	
	void BuildList();

	bool FindItemIdx( int nItemIdx, int& nPos );

	CProcessedOrderEntity GetHeaderEntity() { return m_HeaderEntity; }
	int GetSuppNo() { return m_nSuppNo; }
	int GetSearchType() { return m_nSearchType; }
	const char* GetDescription() { return m_strDescription; }
	bool GetMatchCase() { return m_bMatchCase; }

	bool GetIncludePendingItemsFlag(){ return m_bIncludePendingItems; }
	bool GetIncludeDeliveredItemsFlag(){ return m_bIncludeDeliveredItems; }

	bool IsFilteredByUser();
	bool IsFilteredByHeaderEntity() { return m_bFilteredByHeaderEntity; }
	bool IsFilteredByDescription() { return m_bFilteredByDescription; }
	bool IsFilteredBySupplier() { return m_bFilteredBySupplier; }
	
	void SetHeaderEntity( CProcessedOrderEntity& entity ) { m_HeaderEntity = entity; }
	void SetDescriptionFilter ( const char* szDescription, int nSearchType, bool bMatchCase );
	void SetSuppNo( int n ) { m_nSuppNo = n; }
	
	void SetIncludePendingItemsFlag( bool b ){ m_bIncludePendingItems = b; }
	void SetIncludeDeliveredItemsFlag( bool b ){ m_bIncludeDeliveredItems = b; }
	
	void CopySettingsFrom( CProcessedOrderFilterArray& Source );

	void ShowDebugInfo();

private:
	bool CheckItemForStatus( int nItemIdx );
	bool CheckItemForText( int nItemIdx );

private:
	CString m_strRecord;
	
private:
	CProcessedOrderArray& m_OrderArray;
	CArray<CProcessedOrderFilterIndex,CProcessedOrderFilterIndex> m_IndexArray;
	
private:
	CProcessedOrderEntity m_HeaderEntity;
	int m_nSuppNo;
	CString m_strDescription;
	int m_nSearchType;
	bool m_bMatchCase;

private:
	bool m_bFilteredByHeaderEntity;
	bool m_bFilteredByDescription;
	bool m_bFilteredBySupplier;
	bool m_bIncludePendingItems;
	bool m_bIncludeDeliveredItems;
	int m_nItemsMatchingOrderStatus;
};

/**********************************************************************/
#endif
/**********************************************************************/
