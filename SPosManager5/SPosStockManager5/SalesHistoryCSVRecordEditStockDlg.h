#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SalesHistoryCSVArrayStock.h"
#include "..\CommonEcrStock\SalesHistoryCSVRecordEditDlg.h"
/**********************************************************************/

class CSalesHistoryCSVRecordEditStockDlg : public CSalesHistoryCSVRecordEditDlg
{
public:
	CSalesHistoryCSVRecordEditStockDlg( CSalesHistoryCSVRecordStock& Record, bool bAdhoc, CWnd* pParent = NULL);   

private:
	virtual const char* GetName();
	virtual const char* GetTitle();
	virtual void PrepareCombos( CWordArray& validFields, CWordArray& reportFields );
	virtual void TidyFields();
	virtual void ProcessChanges();

private:
	void UpdateReportModeFromCombo();

private:
	CSalesHistoryCSVRecordStock& m_SalesHistoryStock;
	CString m_strTitle;
};

/**********************************************************************/
#endif 
/**********************************************************************/

