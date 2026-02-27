#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
/**********************************************************************/
#include "BirthdaysReportDlg.h"
/**********************************************************************/

class CBirthdayReport
{
public:
	CBirthdayReport ( CWnd* pParent );
	bool Show();

private:
	int Create ( const char* szReportFile );
	void AddColumnText();
	bool SaveLine(CSQLRowAccountFull& RowAccount);
	void AddToSorter ( bool bExpiry, CSQLRowAccountFull& RowAccount);
	void ShowMonthTotal ( int nCount );
	bool SaveBanner ( int nMonth );

private:
	CWnd* m_pParent;
	CDbReportHandler m_DbReporter;
	CBirthdaysReportDlg m_dlgBirthday;

	CReportFileWithEndText m_Report;
	CReportConsolidationArray<CSortedInt64ByString> m_Sorter;
};

/**********************************************************************/
