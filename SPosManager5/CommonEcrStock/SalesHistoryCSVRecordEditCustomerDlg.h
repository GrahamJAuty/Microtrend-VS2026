#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SalesHistoryCSVArrayCustomer.h"
#include "SalesHistoryCSVRecordEditDlg.h"
/**********************************************************************/

class CSalesHistoryCSVRecordEditCustomerDlg : public CSalesHistoryCSVRecordEditDlg
{
public:
	CSalesHistoryCSVRecordEditCustomerDlg( CSalesHistoryCSVRecordCustomer& Record, bool bAdhoc, CWnd* pParent = NULL);   

private:
	virtual const char* GetName();
	virtual const char* GetTitle();
	virtual void PrepareCombos( CWordArray& validFields, CWordArray& reportFields );
	virtual void TidyFields();
	virtual void ProcessChanges();

	virtual bool SpecialValidate();

private:
	CSalesHistoryCSVRecordCustomer& m_SalesHistoryCustomer;
	CString m_strTitle;
};

/**********************************************************************/
