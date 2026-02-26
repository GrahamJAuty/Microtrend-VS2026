#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CProcessedOrderPreviewCSVRecord : public CCSVRecord
{
public:
	CProcessedOrderPreviewCSVRecord(); //fill with default values
	virtual ~CProcessedOrderPreviewCSVRecord();
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	int GetStockpointCount() { return m_dSUQtyArray.GetSize(); }
	void SetSURequired( int nSpIdx, double dSU );
	void AddSURequired( int nSpIdx, double dSU );
	double GetSURequired( int nSpIdx );
	int CalculateDUItemRequired( int nSpIdx );
	int CalculateDURequired( int nSpIdx );

	int GetStockIdx() { return m_nStockIdx; }
	int GetStockSuppIdx() { return m_nStockSuppIdx; }
	int GetSupplierIdx() { return m_nSupplierIdx; }
	int GetDUItems() { return m_nDUItems; }
	double GetDUItemSize() { return m_dDUItemSize; }
	int GetDUItemsRequired() { return m_nDUItemsRequired; }
	int GetDURequired() { return m_nDURequired; }
	double GetSUTotal() { return m_dSUTotal; }
	double GetTotalCost() { return m_dTotalCost; }
	double GetSUCost() { return m_dSUCost; }
	int GetDUItemSurplus() { return m_nDUItemSurplus; }

	const char* GetDUItemSurplusString();
	
	void SetStockIdx( int nIdx ) { m_nStockIdx = nIdx; }
	void SetStockSuppIdx ( int nIdx ) { m_nStockSuppIdx = nIdx; }
	void SetSupplierIdx ( int nIdx ) { m_nSupplierIdx = nIdx; }
	void SetDUItems ( int nItems ) { m_nDUItems = nItems; }
	void SetDUItemSize( double dSize ) { m_dDUItemSize = dSize; }
	void SetDUItemsRequired( int nReq ) { m_nDUItemsRequired = nReq; }
	void SetDURequired ( int nReq ) { m_nDURequired = nReq; }
	void SetSUTotal ( double dTotal ) { m_dSUTotal = dTotal; }
	void SetTotalCost ( double dCost ) { m_dTotalCost = dCost; }
	void SetSUCost ( double dCost ) { m_dSUCost = dCost; }
	void SetDUItemSurplus ( int nSurplus ) { m_nDUItemSurplus = nSurplus; }

	void UpdateSupplierCosts( CLocationSelectorMiniLookup& LocSelMiniLookup );
	void AllocateSurplusStock ( int nSPIdx );

	CProcessedOrderPreviewCSVRecord& operator= ( CProcessedOrderPreviewCSVRecord& source );
	bool HasOrder( CLocationSelectorMiniLookup& lookup );

private:
	CArray<double,double> m_dSUQtyArray;
	void ClearOrders();
	
private:
	//Record fields
	int m_nStockIdx;
	int m_nStockSuppIdx;
	int m_nSupplierIdx;
	int m_nDUItems;
	double m_dDUItemSize;
	int m_nDUItemsRequired;
	int m_nDURequired;
	double m_dSUTotal;
	double m_dTotalCost;
	double m_dSUCost;
	int m_nDUItemSurplus;

private:
	CString m_strDUItemSurplus;
}; 

/**********************************************************************/

class CProcessedOrderPreviewCSVArray : public CCSVArray
{
public:
	bool FindStockIdx ( int nStockIdx, int& nIndex );
	static bool IsStockpointIncluded( int nSpIdx, CLocationSelectorMiniLookup& LocSelMiniLookup, bool bGenerate );
};

/**********************************************************************/
#endif				//__ORDERPREVIEWDATABASE_H__	
/**********************************************************************/
