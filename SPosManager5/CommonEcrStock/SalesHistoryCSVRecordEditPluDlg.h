#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SalesHistoryCSVArrayPlu.h"
#include "SalesHistoryCSVRecordEditDlg.h"
/**********************************************************************/

class CSalesHistoryCSVRecordEditPluDlg : public CSalesHistoryCSVRecordEditDlg
{
public:
	CSalesHistoryCSVRecordEditPluDlg( CSalesHistoryCSVRecordPlu& Record, bool bAdhoc, CWnd* pParent = NULL);   

private:
	virtual const char* GetName();
	virtual const char* GetTitle();
	virtual void PrepareCombos( CWordArray& validFields, CWordArray& reportFields );
	virtual void TidyFields();
	virtual void ProcessChanges();
	virtual bool SpecialValidate();

private:
	void UpdateReportModeFromCombo();

private:
	CSalesHistoryCSVRecordPlu& m_SalesHistoryPlu;
	CString m_strTitle;
};

/**********************************************************************/
