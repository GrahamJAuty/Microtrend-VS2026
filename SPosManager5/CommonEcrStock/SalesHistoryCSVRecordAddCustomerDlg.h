#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordAddDlg.h"
/**********************************************************************/

class CSalesHistoryCSVRecordAddCustomerDlg : public CSalesHistoryCSVRecordAddDlg
{
public:
	CSalesHistoryCSVRecordAddCustomerDlg( CWnd* pParent = NULL);   

private:
	virtual void PrepareComboAndTitle();
};

/**********************************************************************/
