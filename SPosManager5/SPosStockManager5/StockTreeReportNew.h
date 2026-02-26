#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CCSVReportLine.h"
#include "SalesHistoryCSVArrayStock.h"
#include "StockLevelTreeNew.h"
#include "SupplierReportMap.h"
/**********************************************************************/
#define REPORT_PERIOD 0
#define REPORT_YTD 1
#define REPORT_YTD_PLUS_PERIOD 2
/**********************************************************************/

struct CStockTreeColumnNew
{
public:
	CStockTreeColumnNew()
	{
		m_strText = "";
		m_nFormat = 0;
		m_nWidth = 0;
	};

public:
	CString m_strText;
	int m_nFormat;
	int m_nWidth;
};

/**********************************************************************/

struct CPeriodChartItemNew
{
public:
	CPeriodChartItemNew()
	{
		m_nStockIdx = 0;
		m_dQty = 0.0;
		m_dValue = 0.0;
	};

public:
	int m_nStockIdx;
	double m_dQty;
	double m_dValue;
};

/**********************************************************************/

class CStockTreeReportNew  
{
public:
	CStockTreeReportNew( CStockLevelTreeNew& StockLevelTree );
	virtual ~CStockTreeReportNew();

public:
	void SetConLevelText( const char* sz ) { m_strConLevelText = sz; }
	void SetDrawLinesFlag( bool b ) { m_bDrawLines = b; }
	void SetChartInfo( CSalesHistoryCSVRecordStock& chartinfo );
	
	virtual void PrepareArrays();
	bool CreateReport();

public:
	const char* GetReportParamsKey();
	const char* GetReportParamsFilename();
	const char* GetReportTitle();

public:
	bool GetRetailPluInfo(bool bWantPluStockQty = FALSE);

protected:
	bool CreateReportInternal();
	void ProcessStockLevelsArray( int nArray, int nDepth );
	void WriteHeader( int nDepth );
	void WriteReportLine( const char* szLine );
	void WriteBlankLine( bool bOnlyIfNeeded );

protected:
	const char* GetHeaderLine( int nDepth );
	void AddTotalColumnText( CCSV& csv, int nDepth );
	
protected:
	void OutputStockCodeFields( CCSV& csv, bool bStockUnit );
	void OutputSupplierFields( CCSV& csv, bool bName, bool bRef, int nStockSuppIdx = 0 );

protected:
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels );
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord& TransferDetail );
	virtual void CreateReportLine( CStockLevelsCSVRecord& StockLevels, CSalesCostCSVRecord& SalesCost );
	virtual void CreateTotalsLine( int nDepth );

protected:
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels ){ return FALSE; } 
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels, CTransferDetailCSVRecord& TransferDetail ){ return FALSE; }
	virtual bool CreateReportLine( CStockReportLine& csv, CStockLevelsCSVRecord& StockLevels, CSalesCostCSVRecord& SalesCost ){ return FALSE; }
	virtual void CreateTotalsLine( CStockReportLine& csv, int nDepth ){}
	
protected:
	void AddColumn( const char* szText, int nFormat, int nWidth, bool bUnderline = FALSE );
	
	void AddStockCodeColumns( bool bStockUnit );
	void AddSupplierColumns( bool bName, bool bRef );

	void AddToTotalisers( int nTotaliser, double dValue );
	double GetTotaliser( int nDepth, int nTotaliser );

protected:
	const char* GetRetailTaxBandString();
	double CalculateAverage( double dValue, double dQuantity );
	double CalculateGPPercent( double dRetailExTax, double dCostAndWaste );
	double EstimateCostFromSUQty( double dSUQty );
	double EstimateWastageFromSales( double dSales );
	double EstimateWastageFromSalesAndWastage( double dValue );
	void GetSortedUnitCosts( CArray<double,double>& arrayCosts, double dQty, int nSize );	
	virtual void ShowStockpoints();

protected:
	CStockLevelTreeNew& m_StockLevelTree;
	CSupplierReportMap m_SupplierReportMap;

protected:
	CWordArray m_arrayTotalPos;
	CWordArray m_arrayReportFields;
	CByteArray m_arrayHeaderFlags;
	CByteArray m_arrayTotalFlags;
	
	CArray<CStockTreeColumnNew,CStockTreeColumnNew> m_arrayColumnNames;

	int m_nTotalisersPerLine;
	CArray<double,double> m_arrayTotalisers;

	CSalesHistoryCSVRecordStock m_chartinfo;

protected:
	int m_nLabelStockIdx;
	CStockCSVRecord m_LabelStockRecord;
	int m_nLabelSpIdx;
	int m_nLabelSiteNo;
	int m_nLabelDbIdx;
	int m_nLabelCategory;
	int m_nLabelSuppIdx;
	CString m_strLabelStockpoint;
	CString m_strLabelSite;
	CString m_strLabelDatabase;
	CString m_strLabelCategory;
	CString m_strLabelCustomField[6];
	CString m_strLabelSupplier;
	int m_nLinePluIdx;

protected:
	int m_nRetailPluIdx;
	CString m_strRetailPluNo;
	double m_dRetailPrice1;
	int m_nRetailTaxBand;
	CString m_strRetailTaxBand;
	double m_dRetailTaxRate;
	double m_dRetailStockQty;

protected:
	CReportFile m_ReportFile;
	__int64 m_nProgress;
	__int64 m_nProgressTarget;
	bool m_bLastLineWasBlank;
	bool m_bShowSubUnits;
	bool m_bDrawLines;
	int m_nItemIDColumns;
	bool m_bGotCategoryField;
	bool m_bForceBlankColumn;

protected:
	int m_nReportParamsType;
	CString m_strTotalsUnderline;
	CString m_strConLevelText;
	CString m_strDisplayStockCode;
	CString m_strReportName;
	CString m_strHeader;
	CString m_strTotalColumnText;
	CString m_strReportParamsFilename;
	CString m_strReportParamsKey;
	CString m_strReportTitle;

protected:
	bool m_bBlankBeforeHeader;
	bool m_bSubUnits;
	bool m_bWantTransferDetail;
	bool m_bWantSalesCost;
};

/**********************************************************************/
#endif
/**********************************************************************/
