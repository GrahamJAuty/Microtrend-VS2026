#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\PresetReportBase.h"
/**********************************************************************/
#define PRESET_ECRSTOCK_VERSION 1
/**********************************************************************/

class CPresetReportInfoEcrStock
{
public:
	CPresetReportInfoEcrStock();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

public:
	int GetReportType(){ return m_nReportType; }
	const char* GetDateType(){ return m_strDateType; }
	const char* GetDateFrom(){ return m_strDateFrom; }
	const char* GetDateTo(){ return m_strDateTo; }
	
	bool GetPluRangeFlag() { return m_bPluRange; }
	const char* GetPluFrom() { return m_strPluFrom; }
	const char* GetPluTo() { return m_strPluTo; }
	int GetDepartmentFilter(){ return m_nDepartmentFilter; }
	int GetSupplierFilter(){ return m_nSupplierFilter; }
	bool GetZeroSkipFlag(){ return m_bZeroSkip; }

	bool GetDepartmentSortFlag(){ return m_bDepartmentSort; }
	bool GetSupplierSortFlag(){ return m_bSupplierSort; }

	void SetReportType( int n ){ m_nReportType = n; }	
	void SetDateType( const char* sz ){ m_strDateType = sz; }
	void SetDateFrom( const char* sz ){ m_strDateFrom = sz; }
	void SetDateTo( const char* sz ){ m_strDateTo = sz; }

	void SetPluRangeFlag( bool b ){ m_bPluRange = b; }
	void SetPluFrom( const char* sz ){ m_strPluFrom = sz; }
	void SetPluTo( const char* sz ){ m_strPluTo = sz; }
	void SetDepartmentFilter( int n ){ m_nDepartmentFilter = n; }
	void SetSupplierFilter( int n ){ m_nSupplierFilter = n; }
	void SetZeroSkipFlag( bool b ){ m_bZeroSkip = b; }

	void SetDepartmentSortFlag( bool b ){ m_bDepartmentSort = b; }
	void SetSupplierSortFlag( bool b ){ m_bSupplierSort = b; }

private:	
	int m_nReportType;
	CString m_strDateType;
	CString m_strDateFrom;
	CString m_strDateTo;
	
	bool m_bPluRange;
	CString m_strPluFrom;
	CString m_strPluTo;
	int m_nDepartmentFilter;
	int m_nSupplierFilter;
	bool m_bZeroSkip;

	bool m_bDepartmentSort;
	bool m_bSupplierSort;
};

/**********************************************************************/

class CPresetReportArrayEcrStock : public CPresetReportArray
{
public:
	CPresetReportArrayEcrStock(void);
	~CPresetReportArrayEcrStock(void);

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
