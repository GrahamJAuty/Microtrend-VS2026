#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordAddDlg.h"
/**********************************************************************/

class CSalesHistoryCSVRecordAddPluDlg : public CSalesHistoryCSVRecordAddDlg
{
public:
	CSalesHistoryCSVRecordAddPluDlg( CWnd* pParent = NULL);   

private:
	virtual void PrepareComboAndTitle();
};

/**********************************************************************/
