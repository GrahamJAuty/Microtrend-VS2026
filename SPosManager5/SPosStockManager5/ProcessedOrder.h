#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "ProcessedOrderPreviewDatabase.h"
#include "ProcessedOrderHeader.h"
#include "ProcessedOrderItem.h"
/**********************************************************************/
#define ORDERHEADER_VERSION 1
/**********************************************************************/
#define ORDER_TYPE_OPEN 0
#define ORDER_TYPE_MIXED 1
#define ORDER_TYPE_CLOSED 2
/**********************************************************************/

class CProcessedOrder 
{
public:
	CProcessedOrder();
	
public:
	void Initialise( int nConLevel, int nDbNo, int nSpNo, int nOrderNum, int nSuppNo, int nOrderType, CString& strDate, CString& strTime );
	
public:
	void GetHeader( CProcessedOrderHeader& header );
	void SetHeader( CProcessedOrderHeader& header );

	int GetItemCount(){ return m_arrayOrderItems.GetSize(); }
	bool GetItem( int nItemIdx, CProcessedOrderItem& item );
	bool GetItemString( int nItemIdx, CString& strItem );

public:
	bool AddItem( CProcessedOrderItem& item );
	
public:
	const char* GetOrderFilename();
	void ReadOrder();
	void WriteOrder();
	
private:
	void CreateDatabaseOrderFolders( int nDbIdx, int nSuppNo );
	void CreateStockpointOrderFolders( int nSpIdx, int nSuppNo );

private:
	CProcessedOrderHeader m_OrderHeader;
	CStringArray m_arrayOrderItems;

private:
	bool m_bInitialised;
	int m_nConLevel;
	int m_nDbNo;
	int m_nSpNo;
	
private:
	CString m_strFilename;
};

/**********************************************************************/
#endif
/**********************************************************************/
