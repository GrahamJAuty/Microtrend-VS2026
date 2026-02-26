#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockCustomListFields.h"
#include "StockTreeReportNew.h"
/**********************************************************************/

class CStockTreeReportNewCustomList : public CStockTreeReportNew
{
public:
	CStockTreeReportNewCustomList( CStockLevelTreeNew& StockLevelTree );
	void PrepareReport();

public:
	void SetKey( const char* szKey ){ m_strKey = szKey; }
	void SetCustomName( const char* sz ){ m_strCustomName = sz; }

	static void GetCSVReportName( int n, CString& strName );

private:
	void AddReportColumns();
	bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels );
	
	void CreateTotalsLine( int nDepth ){}
	void CreateTotalsLine( CStockReportLine& csv, int nDepth ){}

	void AddFieldToHeader ( CustomStockField Field, const char* szCustomLabel = "" );

private:
	void AppendSupplierName( CStockReportLine& csv, int nStockSuppIdx );
	void AppendSupplierRef( CStockReportLine& csv, int nStockSuppIdx );
	void AppendWrapperCode( CStockReportLine& csv, int nStockSuppIdx );
	void AppendDelUnitName( CStockReportLine& csv, int nStockSuppIdx );
	void AppendDelUnitDetail( CStockReportLine& csv, int nStockSuppIdx );
	void AppendDelUnitYield( CStockReportLine& csv, int nStockSuppIdx );
	void AppendDelUnitCost( CStockReportLine& csv, int nStockSuppIdx );
	void AppendDelUnitRetail( CStockReportLine& csv, int nStockSuppIdx );
	void AppendStockUnitCost( CStockReportLine& csv, int nStockSuppIdx );
	void AppendCustomFieldValueName( CStockReportLine& csv, int nFieldNo );

private:
	CString m_strKey;
	CString m_strCustomName;
};

/**********************************************************************/
#endif
/**********************************************************************/
