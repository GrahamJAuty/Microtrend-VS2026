/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CategorySetCSVArray.h"
#include "DefTextCategory.h"
#include "ReportFiltersStock.h"
/**********************************************************************/

class CCategorySorter
{
public:
	CCategorySorter();
	CCategorySorter( CCategorySetCSVRecord& CategorySet );
	~CCategorySorter();
	
public:
	void Build( CCategorySetCSVRecord& CategorySet );
	void Build( CReportFiltersStock& ReportFilters );
	int GetItemCount ( int nCategory );
	int GetItemIndex ( int nCategory, int nIndex );

private:
	void Reset();
	void Add( int nCategory, int nStockIdx );

private:
	CPtrArray m_CategoryTable;
};

/**********************************************************************/
#endif		//__CATEGORYSORTER_H__
/**********************************************************************/

