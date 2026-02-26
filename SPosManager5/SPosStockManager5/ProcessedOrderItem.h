#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/
#define ORDERITEM_VERSION 1
/**********************************************************************/

struct COrderItemSpInfo
{
	int m_nSpNo;
	int m_nDUItems;
};

/**********************************************************************/

class CProcessedOrderItem : public CCSVRecord
{
public:
	CProcessedOrderItem(); //fill with default values
	//CProcessedOrderItem( CProcessedOrderPreviewCSVRecord& PreviewRecord, CStockCSVRecord& StockRecord, int nOrderNum, int nStockSuppIdx, int nConLevel, int nTableNo );

	virtual ~CProcessedOrderItem();
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 

	void ClearStockpointOrders();
	void ContractStockpointOrders();
	void ExpandStockpointOrders( int nStockIdx, CStockCSVRecord& StockRecord );

	void AddOrder( int nSpNo, int nDUItems );
	
	int GetStockpointCount() { return m_arraySPOrders.GetSize(); }
	int GetStockpointOrder ( int nSpNo );

	int GetSpNo ( int nIndex );
	int GetDUItems ( int nIndex );
	void SetDUItems( int nIndex, int nItems );

	const char* GetDate() { return m_strDate; }
	const char* GetStockCode() { return m_strStockCode; }
	const char* GetDescription() { return m_strDescription; }
	const char* GetSuppRef() { return m_strSuppRef; }
	const char* GetCase() { return m_strCase; }
	int GetDUItems() { return m_nDUItems; }
	double GetDUItemSize() { return m_dDUItemSize; }
	int GetDUQty() { return m_nDUQty; }
	double GetDUCost() { return m_dDUCost; }
	bool GetDeliveredFlag() { return m_bDelivered; }
	bool GetDeliveredOldFlag() { return m_bDeliveredOld; }
	int GetStockIdx() { return m_nStockIdx; }

	double CalculateSUQty();
	double CalculateSUCost( double dQty );
	
	void SetDate();
	void SetDate( const char* szDate ) { m_strDate = szDate; }
	void SetStockCode( const char* szCode ); 
	void SetDescription( const char* szDesc ); 
	void SetSuppRef( const char* szRef ) { m_strSuppRef = szRef; }
	void SetCase ( const char* szCase ) { m_strCase = szCase; }
	void SetDUItems ( int nItems ) { m_nDUItems = nItems; }
	void SetDUItemSize( double dSize ) { m_dDUItemSize = dSize; }
	void SetDUQty( int nQty ) { m_nDUQty = nQty; }
	void SetDUCost ( double dCost ) { m_dDUCost = dCost; }
	void SetDeliveredFlag() { m_bDelivered = TRUE; }
	void ClearDeliveredFlag() { m_bDelivered = FALSE; }
	void SetDeliveredOldFlag() { m_bDeliveredOld = TRUE; }
	void ClearDeliveredOldFlag() { m_bDeliveredOld = FALSE; }
	void SetStockIdx( int nIdx ) { m_nStockIdx = nIdx; }

	CProcessedOrderItem& operator= ( CProcessedOrderItem& source );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	//Record fields
	CString m_strDate;
	CString m_strStockCode;
	CString m_strDescription;
	CString m_strSuppRef;
	CString m_strCase;
	int m_nDUItems;
	double m_dDUItemSize;
	int m_nDUQty;
	double m_dDUCost;
	bool m_bDelivered;
	bool m_bDeliveredOld;
	int m_nStockIdx;
	
private:
	CArray<COrderItemSpInfo,COrderItemSpInfo> m_arraySPOrders;
};

/**********************************************************************/
#endif
/**********************************************************************/
