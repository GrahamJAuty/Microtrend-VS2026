#pragma once
/**********************************************************************/
#include "GlobalFunctions.h"
#include "ReportDepartmentFilter.h"
#include "ReportPluFilter.h"
/**********************************************************************/

class CReportFilters
{
public:
	CReportFilters();
	void DisablePluFilter(){ m_bDisablePluFilter = TRUE; }

public:
	void ResetDbIdx(){ m_nDbIdx = -1; }
	int GetDbIdx(){ return m_nDbIdx; } 
	
public:
	bool SetDatabase( int nDbIdx );
	bool CheckPlu(CSQLPluNoInfo& infoPlu){ return PluFilter.CheckPlu( infoPlu ); }
	bool CheckDepartment( int nDeptNo ){ return DeptFilter.CheckDepartment( nDeptNo ); }
	
public:
	CReportPluFilter PluFilter;
	CReportDepartmentFilter DeptFilter;

private:
	bool m_bDisablePluFilter;

private:
	int m_nDbIdx;
};

/**********************************************************************/

