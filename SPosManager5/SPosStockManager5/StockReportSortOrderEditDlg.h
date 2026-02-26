#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\SalesHistoryCSVRecordEditDlg.h"
#include "StockReportSortOrderCSVArray.h"
/**********************************************************************/

class CStockReportSortOrderEditDlg : public CSalesHistoryCSVRecordEditDlg
{
public:
	CStockReportSortOrderEditDlg( CStockReportSortOrderCSVRecord& Record, bool bAdhoc, CWnd* pParent = NULL);   

private:
	virtual const char* GetName();
	virtual const char* GetTitle();
	virtual void PrepareCombos( CWordArray& validFields, CWordArray& sortorderFields );
	virtual void TidyFields();
	virtual void ProcessChanges();
	virtual void FillCombos();
	virtual void SelectField();

private:
	void CheckSupplierField();

private:
	CStockReportSortOrderCSVRecord& m_StockReportSortOrder;
	CString m_strTitle;
};

/**********************************************************************/
#endif 
/**********************************************************************/

