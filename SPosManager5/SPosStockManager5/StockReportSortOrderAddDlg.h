#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockReportSortOrderCSVArray.h"
#include "..\CommonEcrStock\SalesHistoryCSVRecordAddDlg.h"
/**********************************************************************/

class CStockReportSortOrderAddDlg : public CSalesHistoryCSVRecordAddDlg
{
public:
	CStockReportSortOrderAddDlg( CWnd* pParent = NULL);   

private:
	virtual void PrepareComboAndTitle();
};

/**********************************************************************/
#endif 
/**********************************************************************/
