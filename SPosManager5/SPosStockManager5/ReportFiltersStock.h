#pragma once
/**********************************************************************/
#include "ReportCategoryFilter.h"
/**********************************************************************/

class CReportFiltersStock
{
public:
	CReportFiltersStock();

public:
	void ResetDbIdx(){ m_nDbIdx = -1; }
	int GetDbIdx(){ return m_nDbIdx; } 
	
public:
	bool SetDatabase( int nDbIdx );
	bool CheckCategory( int nCategoryNo ){ return CategoryFilter.CheckCategory( nCategoryNo ); }
	
public:
	CReportCategoryFilter CategoryFilter;

private:
	int m_nDbIdx;
};

/**********************************************************************/

