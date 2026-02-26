#pragma once
/**********************************************************************/
#include "DepartmentSetCSVArray.h"
/**********************************************************************/

class CReportDepartmentFilter
{
public:
	CReportDepartmentFilter();

public:
	void SetConsolidationType( int nConType );
	void SetDeptFilterNo( int nFilter );
	void SetDatabase( int nDbIdx );
	
public:
	bool CheckDepartment( int nDeptNo );
	const char* GetFilterNameForReport();
	bool GetAllModeFlag(){ return m_DeptFilter.GetAllModeFlag(); }

private:
	int m_nDeptFilterNo;
	bool m_bSystemDeptFilter;
	bool m_bSystemDeptFilterByName;
	CString m_strSystemDeptFilterName;
	CDepartmentSetCSVRecord m_DeptFilter;

private:
	CString m_strFilterNameForReport;
};

/**********************************************************************/

