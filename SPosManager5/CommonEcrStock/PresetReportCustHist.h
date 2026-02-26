#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_CUSTHIST_VERSION 4
/**********************************************************************/

class CPresetReportInfoCustHist
{
public:
	CPresetReportInfoCustHist();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1To4ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

	const char* GetDateType(){ return m_strDateType; }
	const char* GetDateFrom(){ return m_strDateFrom; }
	const char* GetDateTo(){ return m_strDateTo; }
	int GetRoundToType() { return m_nRoundToType; }
	const char* GetSessionName(){ return m_strSessionName; }
	const char* GetTimeSliceName(){ return m_strTimeSliceName; }
	
	bool GetPluRangeFlag() { return m_bPluRange; }
	const char* GetPluFrom() { return m_strPluFrom; }
	const char* GetPluTo() { return m_strPluTo; }
	int GetDepartmentFilter(){ return m_nDepartmentFilter; }
	int GetDepartmentFilterType() { return m_nDepartmentFilterType; }
	int GetTaxFilter() { return m_nTaxFilter; }
	int GetPriceLevelFilter() { return m_nPriceLevelFilter; }
	int GetPriceLevelCustomFilter() { return m_nPriceLevelCustomFilter; }
	int GetTaxCustomFilter() { return m_nTaxCustomFilter; }

	bool GetSalesFlag(){ return m_bSales; }
	bool GetRAFlag(){ return m_bRA; }
	bool GetTaxFlag(){ return m_bTax; }
	bool GetChargesFlag(){ return m_bCharges; }
	bool GetPayMethFlag(){ return m_bPayMeth; }
	bool GetCustomerListFlag(){ return m_bCustomerList; }
	bool GetCustomerPageFlag(){ return m_bCustomerPage; }

	const char* GetReportName(){ return m_strReportName; }
	
	void SetDateType( const char* sz ){ m_strDateType = sz; }
	void SetDateFrom( const char* sz ){ m_strDateFrom = sz; }
	void SetDateTo( const char* sz ){ m_strDateTo = sz; }
	void SetRoundToType( int n ){ m_nRoundToType = n; }
	void SetSessionName( const char* sz ){ m_strSessionName = sz; }
	void SetTimeSliceName( const char* sz ){ m_strTimeSliceName = sz; }

	void SetPluRangeFlag( bool b ){ m_bPluRange = b; }
	void SetPluFrom( const char* sz ){ m_strPluFrom = sz; }
	void SetPluTo( const char* sz ){ m_strPluTo = sz; }
	void SetDepartmentFilter( int n ){ m_nDepartmentFilter = n; }
	void SetDepartmentFilterType( int n ){ m_nDepartmentFilterType = n; }
	void SetTaxFilter( int n ){ m_nTaxFilter = n; }
	void SetPriceLevelFilter( int n ){ m_nPriceLevelFilter = n; }
	void SetTaxCustomFilter( int n ){ m_nTaxCustomFilter = n; }
	void SetPriceLevelCustomFilter( int n ){ m_nPriceLevelCustomFilter = n; }
	
	void SetSalesFlag( bool b ){ m_bSales = b; }
	void SetRAFlag( bool b ){ m_bRA = b; }
	void SetTaxFlag( bool b ){ m_bTax = b; }
	void SetChargesFlag( bool b ){ m_bCharges = b; }
	void SetPayMethFlag( bool b ){ m_bPayMeth = b; }
	void SetCustomerListFlag( bool b ){ m_bCustomerList = b; }
	void SetCustomerPageFlag( bool b ){ m_bCustomerPage = b; }

	void SetReportName( const char* sz ){ m_strReportName = sz; }
		
private:	
	CString m_strDateType;
	CString m_strDateFrom;
	CString m_strDateTo;
	int m_nRoundToType;
	CString m_strSessionName;
	CString m_strTimeSliceName;
	
	bool m_bPluRange;
	CString m_strPluFrom;
	CString m_strPluTo;
	int m_nDepartmentFilter;
	int m_nDepartmentFilterType;
	int m_nTaxFilter;
	int m_nPriceLevelFilter;
	int m_nTaxCustomFilter;
	int m_nPriceLevelCustomFilter;

	bool m_bSales;
	bool m_bRA;
	bool m_bTax;
	bool m_bCharges;
	bool m_bPayMeth;
	bool m_bCustomerList;
	bool m_bCustomerPage;

	CString m_strReportName;
};

/**********************************************************************/

class CPresetReportArrayCustHist : public CPresetReportArray
{
public:
	CPresetReportArrayCustHist(void);
	~CPresetReportArrayCustHist(void);

public:
	const char* GetFilename();

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();
};

/**********************************************************************/
#endif
/**********************************************************************/
