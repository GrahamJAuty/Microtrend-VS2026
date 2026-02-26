#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CategorySetCSVArray.h"
/**********************************************************************/

class CReportCategoryFilter
{
public:
	CReportCategoryFilter();

public:
	void SetConsolidationType( int nConType );
	void SetCategoryFilterNo( int nFilter );
	void SetDatabase( int nDbIdx );
	
public:
	bool CheckCategory( int nDeptNo );
	const char* GetFilterNameForReport();
	bool GetAllModeFlag(){ return m_CategoryFilter.GetAllModeFlag(); }

private:
	int m_nCategoryFilterNo;
	bool m_bSystemCategoryFilter;
	bool m_bSystemCategoryFilterByName;
	CString m_strSystemCategoryFilterName;
	CCategorySetCSVRecord m_CategoryFilter;

private:
	CString m_strFilterNameForReport;
};

/**********************************************************************/
#endif
/**********************************************************************/
