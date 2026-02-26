#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_STOCKHIST_VERSION 4
/**********************************************************************/

class CPresetReportInfoStockHist
{
public:
	CPresetReportInfoStockHist();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1To4ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

public:
	const char* GetDateType(){ return m_strDateType; }
	const char* GetDateFrom(){ return m_strDateFrom; }
	const char* GetDateTo(){ return m_strDateTo; }
	int GetRoundToType() { return m_nRoundToType; }
	const char* GetSessionName(){ return m_strSessionName; }

	bool GetEposSalesFlag() { return m_bEposSales; }
	bool GetManualSalesFlag() { return m_bManualSales; }
	bool GetImportSalesFlag() { return m_bImportSales; }
	
	bool GetStockRangeFlag() { return m_bStockRange; }
	const char* GetStockFrom() { return m_strStockFrom; }
	const char* GetStockTo() { return m_strStockTo; }
	int GetCategoryFilter(){ return m_nCategoryFilter; }
	int GetCategoryFilterType() { return m_nCategoryFilterType; }
	int GetSupplierFilter(){ return m_nSupplierFilter; }
	
	const char* GetReportName(){ return m_strReportName; }
	bool GetByMonthFlag(){ return m_bByMonth; }
	bool GetCatSortFlag(){ return m_bCatSort; }
	bool GetPercentSalesFlag(){ return m_bPercentSales; }
	bool GetEstProfitFlag() { return m_bEstProfit; }
	bool GetApparentFlag() { return m_bApparent; }

	int GetCustomFieldFilter( int nFieldNo );

public:
	void SetDateType( const char* sz ){ m_strDateType = sz; }
	void SetDateFrom( const char* sz ){ m_strDateFrom = sz; }
	void SetDateTo( const char* sz ){ m_strDateTo = sz; }
	void SetRoundToType( int n ){ m_nRoundToType = n; }
	void SetSessionName( const char* sz ){ m_strSessionName = sz; }

	void SetEposSalesFlag( bool b ){ m_bEposSales = b; }
	void SetManualSalesFlag( bool b ){ m_bManualSales = b; }
	void SetImportSalesFlag( bool b ){ m_bImportSales = b; }

	void SetStockRangeFlag( bool b ){ m_bStockRange = b; }
	void SetStockFrom( const char* sz ){ m_strStockFrom = sz; }
	void SetStockTo( const char* sz ){ m_strStockTo = sz; }
	void SetCategoryFilter( int n ){ m_nCategoryFilter = n; }
	void SetCategoryFilterType( int n ){ m_nCategoryFilterType = n; }
	void SetSupplierFilter( int n ){ m_nSupplierFilter = n; }
	
	void SetReportName( const char* sz ){ m_strReportName = sz; }
	void SetByMonthFlag( bool b ){ m_bByMonth = b; }
	void SetCatSortFlag( bool b ){ m_bCatSort = b; }
	void SetPercentSalesFlag( bool b ){ m_bPercentSales = b; }
	void SetEstProfitFlag( bool b ){ m_bEstProfit = b; }
	void SetApparentFlag( bool b ){ m_bApparent = b; }

	void SetCustomFieldFilter( int nFieldNo, int n );
	
private:
	void ClearRecord();
	void V1To4ConvertFromCSV( CCSV& csv, int nVer );

private:
	CString m_strDateType;
	CString m_strDateFrom;
	CString m_strDateTo;
	int m_nRoundToType;
	CString m_strSessionName;

	bool m_bEposSales;
	bool m_bManualSales;
	bool m_bImportSales;

	bool m_bStockRange;
	CString m_strStockFrom;
	CString m_strStockTo;
	int m_nCategoryFilter;
	int m_nCategoryFilterType;
	int m_nSupplierFilter;

	CString m_strReportName;
	bool m_bByMonth;
	bool m_bCatSort;
	bool m_bPercentSales;
	bool m_bEstProfit;
	bool m_bApparent;

	int m_nCustomFieldFilter[6];
};

/**********************************************************************/

class CPresetReportArrayStockHist : public CPresetReportArray
{
public:
	CPresetReportArrayStockHist(void);
	~CPresetReportArrayStockHist(void);

public:
	const char* GetFilename();

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
