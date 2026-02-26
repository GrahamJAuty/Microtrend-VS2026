#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordAddDlg.h"
/**********************************************************************/

class CTimeSliceMapAddDlg : public CSalesHistoryCSVRecordAddDlg
{
public:
	CTimeSliceMapAddDlg( CWnd* pParent = NULL);   

private:
	virtual void PrepareComboAndTitle();
};

/**********************************************************************/
#endif
/**********************************************************************/
