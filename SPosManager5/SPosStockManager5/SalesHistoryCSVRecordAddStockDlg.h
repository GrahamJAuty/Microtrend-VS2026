#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\SalesHistoryCSVRecordAddDlg.h"
/**********************************************************************/

class CSalesHistoryCSVRecordAddStockDlg : public CSalesHistoryCSVRecordAddDlg
{
public:
	CSalesHistoryCSVRecordAddStockDlg( CWnd* pParent = NULL);   

private:
	virtual void PrepareComboAndTitle();
};

/**********************************************************************/
#endif 
/**********************************************************************/
