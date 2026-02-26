#pragma once
/**********************************************************************/
#include "DepartmentCSVArray.h"
/**********************************************************************/
#define FIRST_DEPARTMENTSET_FILTER 10000
#define DEPARTMENTSET_ALL 9999
#define DEPARTMENTSET_REPORT 9998
#define DEPARTMENTSET_BRITANNIA 9997
/**********************************************************************/

class CDepartmentSetCSVArray : public CSharedCSVArray
{
public:
	CDepartmentSetCSVArray();
	int Open( const char* szFilename, int nMode );
	
	const char* GetName ( int nIndex );
	bool GetBritanniaHotelsFlag ( int nIndex );

private:
	CString m_strName;
};

/**********************************************************************/

class CDepartmentSetCSVRecord : public CCSVRecord
{
public:
	CDepartmentSetCSVRecord(); 
	virtual ~CDepartmentSetCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	void SetName ( const char* szName ) { m_strName = szName; }
	const char* GetName() { return m_strName; }
	const char* GetFilterName( bool bTrailingComma, bool bHideReportEnabled );
	
	void SetBritanniaHotelsFlag( bool bFlag ) { m_bBritanniaHotels = bFlag; }
	bool GetBritanniaHotelsFlag() { return m_bBritanniaHotels; }

	bool IncludeDepartment ( int nDeptNo, bool bFlag );
	void IncludeAllDepartments( bool bFlag );
	
	bool GetDepartmentStatus ( int nDeptNo );
	int GetDefaultDepartment();
	
	int CreateDepartmentSet ( int nDepartmentFilter );
	int CreateDepartmentSetSystem ( int nDepartmentFilter );
	void CreateReportEnabledDepartmentSet( CDepartmentCSVArray& arrayDept );

	bool GetAllModeFlag(){ return m_bAllMode; }
	bool CreatedEmptyFilter(){ return m_bCreatedEmptyFilter; }

private:
	void ClearRecord();
	void V1To2ConvertFromCSV( CCSV& csv, int nVer );

private:
	int CreateDepartmentSetInternal( CDepartmentCSVArray& DeptArray, CDepartmentSetCSVArray& DeptSetArray, int nDepartmentFilter );

private:
	CWordArray m_arrayIncluded;
	CString m_strName;  
	CString m_strFilterName;
	CString m_strFilterNameOut;
	bool m_bBritanniaHotels;

private:
	bool m_bReportEnabledSet;
	bool m_bAllMode;

private:
	bool m_bCreatedEmptyFilter;
	bool m_bConvertedEmptyFilter;
};

/**********************************************************************/
