#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_STOCKLEVEL_VERSION 4
/**********************************************************************/

class CPresetReportInfoStockLevel
{
public:
	CPresetReportInfoStockLevel();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1To4ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );	

public:
	int GetReportType(){ return m_nReportType; }
	int GetSortOrder(){ return m_nSortOrder; }
	bool GetShowItemsFlag(){ return m_bShowItems; }
	int GetTimeType(){ return m_nTimeType; }
	bool GetSuppNameFlag(){ return m_bSuppName; }
	bool GetSuppRefFlag(){ return m_bSuppRef; }
	bool GetStockUnitFlag(){ return m_bStockUnit; }
	bool GetDrawLinesFlag(){ return m_bDrawLines; }
	bool GetAlphaSortFlag(){ return m_bAlphaSort; }
	int GetSupplierFilter(){ return m_nSupplierFilter; }
	int GetCategoryFilter(){ return m_nCategoryFilter; }
	int GetCustomFieldFilter( int nFieldNo );
	bool GetStockRangeFlag(){ return m_bStockRange; }
	const char* GetStockFrom(){ return m_strStockFrom; }
	const char* GetStockTo(){ return m_strStockTo; } 
	bool GetExcludeInactiveFlag(){ return m_bExcludeInactive; }
	bool GetItemFilterFlag(){ return m_bItemFilter; }
	int GetStocktake(){ return m_nStocktake; }
	
public:
	void SetReportType( int n ){ m_nReportType = n; }
	void SetSortOrder( int n ){ m_nSortOrder = n; }
	void SetShowItemsFlag( bool b ){ m_bShowItems = b; }
	void SetTimeType( int n ){ m_nTimeType = n; }
	void SetSuppNameFlag( bool b ){ m_bSuppName = b; }
	void SetSuppRefFlag( bool b ){ m_bSuppRef = b; }
	void SetStockUnitFlag( bool b ){ m_bStockUnit = b; }
	void SetDrawLinesFlag( bool b ){ m_bDrawLines = b; }
	void SetAlphaSortFlag( bool b ){ m_bAlphaSort = b; }
	void SetSupplierFilter( int n ){ m_nSupplierFilter = n; }
	void SetCategoryFilter( int n ){ m_nCategoryFilter = n; }
	void SetCustomFieldFilter( int nFieldNo, int n );
	void SetStockRangeFlag( bool b ){ m_bStockRange = b; }
	void SetStockFrom( const char* sz ){ m_strStockFrom = sz; }
	void SetStockTo( const char* sz ){ m_strStockTo = sz; }
	void SetExcludeInactiveFlag( bool b ){ m_bExcludeInactive = b; }
	void SetItemFilterFlag( bool b ){ m_bItemFilter = b; }
	void SetStocktake( int n ){ m_nStocktake = n; }

private:
	int m_nReportType;
	int m_nSortOrder;
	bool m_bShowItems;
	int m_nTimeType;
	bool m_bSuppName;
	bool m_bSuppRef;
	bool m_bStockUnit;
	bool m_bDrawLines;
	bool m_bAlphaSort;
	int m_nSupplierFilter;
	int m_nCategoryFilter;
	int m_nCustomFieldFilter[6];
	bool m_bStockRange;
	CString m_strStockFrom;
	CString m_strStockTo;
	bool m_bExcludeInactive;
	bool m_bItemFilter;
	int m_nStocktake;
};

/**********************************************************************/

class CPresetReportArrayStockLevel : public CPresetReportArray
{
public:
	CPresetReportArrayStockLevel(void);
	~CPresetReportArrayStockLevel(void);

public:
	const char* GetFilename();
	void SetStocktakeFlag( bool b ){ m_bStocktake = b; }

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();

private:
	bool m_bStocktake;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
