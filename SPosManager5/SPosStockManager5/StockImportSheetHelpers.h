/**********************************************************************/
#pragma once
/**********************************************************************/
#define STOCK_IMPORTSHEET_STOCKCODE 0
#define STOCK_IMPORTSHEET_DESCRIPTION 1
#define STOCK_IMPORTSHEET_CATEGORY 2
#define STOCK_IMPORTSHEET_SHEETCODE 3
#define STOCK_IMPORTSHEET_SUNAME 4
#define STOCK_IMPORTSHEET_SUQTY 5
#define STOCK_IMPORTSHEET_DUNAME 6
#define STOCK_IMPORTSHEET_DUQTY 7
#define STOCK_IMPORTSHEET_SUPPLIER 8
/**********************************************************************/
#define MAX_STOCK_IMPORTSHEET_FIELD 8
/**********************************************************************/
#include "StockFilterArray.h"
#include "SupplierReportMap.h"
/**********************************************************************/

struct CStockImportSheetItem
{
public:
	CStockImportSheetItem();
	void Reset();

public:
	int Compare ( CStockImportSheetItem& source, int nHint = 0 );
	void Add	( CStockImportSheetItem& source );
	
public:
	int m_nStockIdx;
	int m_nSuppSortIdx;
	int m_nCatNo;
	CString m_strDescription;
};

/**********************************************************************/

class CStockImportSheetHelpers
{
public:
	CStockImportSheetHelpers( int nSpIdx, int nStocktakeNo );
	~CStockImportSheetHelpers(void);

public:
	void CreateSheet( CStockFilterArray* pFilterArray, CString& strFields, int nCopies );
	bool CreateReportHeader( const char* szHeader, CReportFile& ReportFile );

public:
	bool ConsolidateImportSheets();
	bool ProcessHeader( const char* szHeader );
	void ProcessLine( const char* szLine, CString& strStockCode, bool& bGotSUQty, double& dSUQty, bool& bGotDUQty, double& dDUQty );

public:
	const char* GetImportSheetFilename();
	const char* GetImportSheetConsolidatedFilename() { return m_strImportSheetConsolidatedFilename; }
	const char* GetFieldName( int nField, bool MakeUpper = FALSE );

private:
	int m_nSpIdx;
	int m_nStocktakeNo;
	CString m_strFieldName;
	CString m_strImportSheetFilename;
	CString m_strImportSheetFolder;
	CString m_strImportSheetConsolidatedFilename;

public:
	int m_nColumnStockCode;
	int m_nColumnSUQty;
	int m_nColumnDUQty;

private:
	CSupplierReportMap m_SupplierReportMap;
};

/**********************************************************************/
