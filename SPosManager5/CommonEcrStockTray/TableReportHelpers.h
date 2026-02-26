/**********************************************************************/
#pragma once
/**********************************************************************/
#define TABLEFILTER_ALL 0
#define TABLEFILTER_TABLE 1
#define TABLEFILTER_GROUP 2
#define TABLEFILTER_SET 3
/**********************************************************************/
#include "TableNameCSVArray.h"
/**********************************************************************/

struct CTableFilter
{
public:
	CTableFilter();

public:
	void Reset();

public:
	int m_nType;
	int m_nLevel;
	int m_nEntityNo;
	CString m_strName;
};

/**********************************************************************/

class CTableReportHelpers
{
public:
	CTableReportHelpers(void);
	~CTableReportHelpers(void);

public:
	void SetLocIdx( int n );

public:
	void BuildTableFilterList();
	int GetTableFilterCount(){ return m_arrayTableFilters.GetSize(); }
	void GetTableFilter( int n, CTableFilter& TableFilter );

public:
	void BuildGroupMap( CTableFilter& TableFilter );
	void AddTableToGroupMap( CTableNameCSVRecord& Table, int nLevel );
	bool CheckTableGroup( int nTableNo, int& nGroup );
	const char* GetTableName( int nTableNo );
	const char* GetGroupName( int nGroupNo );

private:
	void OpenTableData();
	void CloseTableData();
	
private:
	CArray<CTableFilter,CTableFilter> m_arrayTableFilters;
	CReportConsolidationArray<CSortedIntByInt> m_GroupMap;

private:
	int m_nLocIdx;
	bool m_bIncludeUnknown;
	bool m_bIncludeNonTable;

private:
	int m_nLastLookupTableNo;
	int m_nLastLookupGroupNo;
	bool m_bLastLookupResponse;
	
private:
	CReportConsolidationArray<CSortedStringByInt> m_arrayTableNames;
	CStringArray m_arrayGroupNames;
	CString m_strTableName;
	CString m_strGroupName;
};

/**********************************************************************/

