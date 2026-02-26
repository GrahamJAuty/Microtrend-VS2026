/**********************************************************************/
#pragma once
/**********************************************************************/
#if defined(STOCKMAN_SYSTEM) || defined(POSTRAY_UTILITY)
/**********************************************************************/
#include "..\CommonStockTray\StockCSVArray.h"
#include "..\CommonStockTray\StockLevelsCSVArray.h"
/**********************************************************************/
#define SALESCOSTDB_VERSION 1
/**********************************************************************/

class CSalesCostCSVRecord : public CCSVRecord
{
public:
	CSalesCostCSVRecord();
	virtual ~CSalesCostCSVRecord();
	
public:
	void Reset();

	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	CSalesCostCSVRecord& operator= ( CSalesCostCSVRecord& source );
	CSalesCostCSVRecord& operator+ ( CSalesCostCSVRecord& source );

	const char* GetStockCode() { return m_strStockCode; }
	double GetEposSalesCost();
	double GetEposSalesCost( int nLevel );
	double GetRecipeSalesCost(){ return m_dRecipeSalesCost; }
	double GetManualSalesCost(){ return m_dManualSalesCost; }
	double GetImportSalesCost(){ return m_dImportSalesCost; }
	double GetEcrWasteCost(){ return m_dEcrWasteCost; }
	double GetAutoWasteCost(){ return m_dAutoWasteCost; }

	void AddEposSalesCost( int nLevel, double dCost );
	void AddRecipeSalesCost( double dCost ){ m_dRecipeSalesCost += dCost; }
	void AddManualSalesCost( double dCost ){ m_dManualSalesCost += dCost; }
	void AddImportSalesCost( double dCost ){ m_dImportSalesCost += dCost; }
	void AddEcrWasteCost( double dCost ){ m_dEcrWasteCost += dCost; }
	void AddAutoWasteCost( double dCost ){ m_dAutoWasteCost += dCost; }

	double GetTotalSalesCost();
	
	void SetStockCode( const char* szStockCode ) { m_strStockCode = szStockCode; }	
	void SetEposSalesCost( int nLevel, double dCost );
	void SetRecipeSalesCost( double dCost ){ m_dRecipeSalesCost = dCost; }
	void SetManualSalesCost( double dCost ){ m_dManualSalesCost = dCost; }
	void SetImportSalesCost( double dCost ){ m_dImportSalesCost = dCost; }
	void SetEcrWasteCost( double dCost ){ m_dEcrWasteCost += dCost; }
	void SetAutoWasteCost( double dCost ){ m_dAutoWasteCost += dCost; }

	void SetCosts( CStockCSVRecord& StockRecord, CStockLevelsCSVRecord& StockLevels, bool bAutoWaste );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	CString m_strStockCode;
	double m_dEposSalesCost[ STOCK_PRICES ];
	double m_dRecipeSalesCost;
	double m_dManualSalesCost;
	double m_dImportSalesCost;
	double m_dEcrWasteCost;
	double m_dAutoWasteCost;
};

/**********************************************************************/

class CSalesCostCSVArray : public CSharedCSVArray
{
public:
	CSalesCostCSVArray();
	~CSalesCostCSVArray();
	void RemoveAll();
	
	const char* GetStockCode( int nIndex );
	bool FindStockItemByCode ( const char* szCode, int& nIndex );
	
private:
	CString m_strStockCode;
};

/**********************************************************************/
#endif
/**********************************************************************/
