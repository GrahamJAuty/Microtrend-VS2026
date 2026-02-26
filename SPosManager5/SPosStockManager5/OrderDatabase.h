#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderPreviewDatabase.h"
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/
#define ORDERDB_VERSION 1
/**********************************************************************/

struct CSPOrderInfo
{
	int m_nSpNo;
	int m_nDUItems;
};

/**********************************************************************/

class COrderCSVRecord : public CCSVRecord
{
public:
	COrderCSVRecord(); //fill with default values
	COrderCSVRecord( CProcessedOrderPreviewCSVRecord& PreviewRecord, CStockCSVRecord& StockRecord, int nOrderNum, int nStockSuppIdx, CLocationSelectorLookup& lookup );

	virtual ~COrderCSVRecord();
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 

	void ClearStockpointOrders();
	
	void AddOrder( int nSpNo, int nDUItems );
	
	int GetStockpointCount() { return m_arraySPOrders.GetSize(); }
	int GetStockpointOrder ( int nSpNo );

	int GetSpNo ( int nIndex );
	int GetDUItems ( int nIndex );
	void SetDUItems( int nIndex, int nItems );

	int GetOrderNum() { return m_nOrderNum; }
	int GetSuppNo() { return m_nSuppNo; }
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
	int GetStockIdx() { return m_nStockIdx; }

	double CalculateSUQty();
	double CalculateSUCost( double dQty );
	
	void SetOrderNum( int nNum ) { m_nOrderNum = nNum; }
	void SetSuppNo( int nNum ) { m_nSuppNo = nNum; }
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
	void SetStockIdx( int nIdx ) { m_nStockIdx = nIdx; }

	COrderCSVRecord& operator= ( COrderCSVRecord& source );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	//Record fields
	int m_nOrderNum;
	int m_nSuppNo;
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
	int m_nStockIdx;
	
private:
	CArray<CSPOrderInfo,CSPOrderInfo> m_arraySPOrders;
};

/**********************************************************************/

class COrderCSVArray : public CSharedCSVArray
{
public:
	COrderCSVArray();
	int Open( const char* szFilename, int nMode );

	void SortOrders();

	int FindEndOfOrder( int nSuppNo );
	int FindEndOfOrder( int SuppNo, int nOrderNum );
	
	int GetOrderLineCount( int nSuppNo );
	int GetOrderLineCount( int nSuppNo, int nOrderNum );
	
	void FindOrderLines( int nSuppNo, int& nStartIdx, int& nEndIdx );
	void FindOrderLines( int nSuppNo, int nOrderNum, int& nStartIdx, int& nEndIdx );

	bool NeedUpdateDatabaseOrders();
	bool NeedUpdateStockpointOrders();

	bool UpdateDatabaseOrders( int nDbIdx );
	bool UpdateStockpointOrders( int nSpIdx );

	void ShowUpgradeError();

private:
	void UpdateDatabaseOrdersInternal( int nDbIdx, CString& strFilename );
	void UpdateStockpointOrdersInternal( int nSpIdx, CString& strFilename );

public:
	void RemoveAll();
	int AddNewOrderLine ( COrderCSVRecord& NewRecord );
	bool GetDeliveredFlag ( int nOrderIdx );
};

/**********************************************************************/
#endif
/**********************************************************************/
