#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLTable_PebbleLinkTime\SQLRecordSetPebbleLinkTime.h"
/**********************************************************************/
#include "ReportPebbleLinkTimesDlg.h"
/**********************************************************************/

class CReportPebbleLinkTimes
{
public:
	CReportPebbleLinkTimes ( CWnd* pParent );
	void ShowFromDialog();
	void ShowDirect();

private:
	int CreateFromDialog();
	int CreateDirect();
	int CreateReport(CWorkingDlg& dlgWorking);
	int CreateReportFromRecordSet(CSQLRecordSetPebbleLinkTime& recordSet, CWorkingDlg& dlgWorking);
	void AddColumnText();

private:
	bool ValidateDateOrTime(bool bDate, CString strIn, CString& strOut);
	CString GetAccountWhereFilter(CSQLRecordSetPebbleLinkTime& recordset);
	CString GetOrderBy(CSQLRecordSetPebbleLinkTime& recordset);

public:
	CReportPebbleLinkInfo m_ReportInfo;

private:
	CWnd* m_pParent;
	CReportPebbleLinkTimesDlg m_dlgReport;
	CRepmanHandler m_Repman;
	CReportFileWithEndText m_Report;
};

/**********************************************************************/
