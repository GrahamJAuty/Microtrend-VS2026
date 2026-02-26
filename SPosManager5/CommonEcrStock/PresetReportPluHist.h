#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_PLUHIST_VERSION 8
/**********************************************************************/

class CPresetReportInfoPluHist
{
public:
	CPresetReportInfoPluHist();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1To8ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

public:
	const char* GetDateType(){ return m_strDateType; }
	const char* GetDateFrom(){ return m_strDateFrom; }
	const char* GetDateTo(){ return m_strDateTo; }
	int GetRoundToType() { return m_nRoundToType; }
	const char* GetSessionName(){ return m_strSessionName; }
	const char* GetTimeSliceName(){ return m_strTimeSliceName; }
	bool GetSecondDateBlockFlag(){ return m_bSecondDateBlock; }
	int GetSecondDateBlockAutoType(){ return m_nSecondDateBlockAutoType; }
	const char* GetDateType2(){ return m_strDateType2; }
	const char* GetDateFrom2(){ return m_strDateFrom2; }
	const char* GetDateTo2(){ return m_strDateTo2; }
	int GetRoundToType2() { return m_nRoundToType2; }
	
	int GetTableFilterType() { return m_nTableFilterType; }
	int GetTableFilterLevel() { return m_nTableFilterLevel; }
	int GetTableFilterEntityNo() { return m_nTableFilterEntityNo; }
	bool GetEposSalesFlag() { return m_bEposSales; }
	bool GetManualSalesFlag() { return m_bManualSales; }
	bool GetImportSalesFlag() { return m_bImportSales; }
	
	bool GetPluRangeFlag() { return m_bPluRange; }
	const char* GetPluFrom() { return m_strPluFrom; }
	const char* GetPluTo() { return m_strPluTo; }
	int GetDepartmentFilter(){ return m_nDepartmentFilter; }
	int GetDepartmentFilterType() { return m_nDepartmentFilterType; }
	int GetPluFilterNo(){ return m_nPluFilterNo; }
	bool GetSaleTimeFlag(){ return m_bSaleTime; }
	int GetTaxFilter() { return m_nTaxFilter; }
	int GetPriceLevelFilter() { return m_nPriceLevelFilter; }
	int GetModifierFilter() { return m_nModifierFilter; }
	int GetTaxCustomFilter() { return m_nTaxCustomFilter; }
	int GetPriceLevelCustomFilter() { return m_nPriceLevelCustomFilter; }
	int GetModifierCustomFilter() { return m_nModifierCustomFilter; }

	const char* GetReportName(){ return m_strReportName; }
	bool GetByMonthFlag(){ return m_bByMonth; }
	bool GetDeptSortFlag(){ return m_bDeptSort; }
	bool GetPercentSalesFlag(){ return m_bPercentSales; }
	bool GetEstProfitFlag() { return m_bEstProfit; }
	bool GetCustomerFlag(){ return m_bCustomer; }
	bool GetVIPSalesFlag(){ return m_bVIPSales; }
	bool GetGuestFlag(){ return m_bGuest; }

	int GetTermDateType() { return m_nTermDateType;  }

public:
	void SetDateType( const char* sz ){ m_strDateType = sz; }
	void SetDateFrom( const char* sz ){ m_strDateFrom = sz; }
	void SetDateTo( const char* sz ){ m_strDateTo = sz; }
	void SetRoundToType( int n ){ m_nRoundToType = n; }
	void SetSecondDateBlockFlag( bool b ){ m_bSecondDateBlock = b; }
	void SetSecondDateBlockAutoType( int n ){ m_nSecondDateBlockAutoType = n; }
	void SetDateType2( const char* sz ){ m_strDateType2 = sz; }
	void SetDateFrom2( const char* sz ){ m_strDateFrom2 = sz; }
	void SetDateTo2( const char* sz ){ m_strDateTo2 = sz; }
	void SetRoundToType2( int n ){ m_nRoundToType2 = n; }
	
	void SetSessionName( const char* sz ){ m_strSessionName = sz; }
	void SetTimeSliceName( const char* sz ){ m_strTimeSliceName = sz; }

	void SetTableFilterType( int n ) { m_nTableFilterType = n; }
	void SetTableFilterLevel( int n ) { m_nTableFilterLevel = n; }
	void SetTableFilterEntityNo( int n ) { m_nTableFilterEntityNo = n; }
	void SetEposSalesFlag( bool b ){ m_bEposSales = b; }
	void SetManualSalesFlag( bool b ){ m_bManualSales = b; }
	void SetImportSalesFlag( bool b ){ m_bImportSales = b; }
	
	void SetPluRangeFlag( bool b ){ m_bPluRange = b; }
	void SetPluFrom( const char* sz ){ m_strPluFrom = sz; }
	void SetPluTo( const char* sz ){ m_strPluTo = sz; }
	void SetDepartmentFilter( int n ){ m_nDepartmentFilter = n; }
	void SetDepartmentFilterType( int n ){ m_nDepartmentFilterType = n; }
	void SetPluFilterNo( int n ){ m_nPluFilterNo = n; }
	void SetSaleTimeFlag( bool b ){ m_bSaleTime = b; }
	void SetTaxFilter( int n ){ m_nTaxFilter = n; }
	void SetPriceLevelFilter( int n ){ m_nPriceLevelFilter = n; }
	void SetModifierFilter( int n ){ m_nModifierFilter = n; }
	void SetTaxCustomFilter( int n ){ m_nTaxCustomFilter = n; }
	void SetPriceLevelCustomFilter( int n ){ m_nPriceLevelCustomFilter = n; }
	void SetModifierCustomFilter( int n ){ m_nModifierCustomFilter = n; }

	void SetReportName( const char* sz ){ m_strReportName = sz; }
	void SetByMonthFlag( bool b ){ m_bByMonth = b; }
	void SetDeptSortFlag( bool b ){ m_bDeptSort = b; }
	void SetPercentSalesFlag( bool b ){ m_bPercentSales = b; }
	void SetEstProfitFlag( bool b ){ m_bEstProfit = b; }
	void SetCustomerFlag( bool b ){ m_bCustomer = b; }
	void SetVIPSalesFlag( bool b ){ m_bVIPSales = b; }
	void SetGuestFlag( bool b ){ m_bGuest = b; }

	void SetTermDateType(int n) { m_nTermDateType = n;  }
		
private:
	CString m_strDateType;
	CString m_strDateFrom;
	CString m_strDateTo;
	int m_nRoundToType;
	bool m_bSecondDateBlock;
	int m_nSecondDateBlockAutoType;
	CString m_strDateType2;
	CString m_strDateFrom2;
	CString m_strDateTo2;
	int m_nRoundToType2;
	CString m_strSessionName;
	CString m_strTimeSliceName;

	int m_nTableFilterType;
	int m_nTableFilterLevel;
	int m_nTableFilterEntityNo;
	bool m_bEposSales;
	bool m_bManualSales;
	bool m_bImportSales;

	bool m_bPluRange;
	CString m_strPluFrom;
	CString m_strPluTo;
	int m_nDepartmentFilter;
	int m_nDepartmentFilterType;
	int m_nPluFilterNo;
	bool m_bSaleTime;
	int m_nTaxFilter;
	int m_nPriceLevelFilter;
	int m_nModifierFilter;
	int m_nTaxCustomFilter;
	int m_nPriceLevelCustomFilter;
	int m_nModifierCustomFilter;

	CString m_strReportName;
	bool m_bByMonth;
	bool m_bDeptSort;
	bool m_bPercentSales;
	bool m_bEstProfit;
	bool m_bCustomer;
	bool m_bVIPSales;
	bool m_bGuest;

	int m_nTermDateType;
};

/**********************************************************************/

class CPresetReportArrayPluHist : public CPresetReportArray
{
public:
	CPresetReportArrayPluHist(void);
	~CPresetReportArrayPluHist(void);

public:
	const char* GetFilename();

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();
};

/**********************************************************************/
#endif
/**********************************************************************/
