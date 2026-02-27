#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "WeeklySpendDatabase.h"
#include "ReportWeeklySpendDlg.h"
/**********************************************************************/

class CReportWeeklySpend
{
public:
	CReportWeeklySpend ( const char* szLabel, CWnd* pParent = NULL );
	void Show();

private:
	int Create();
	int ReportSpend ( CSmartPayAuditFile* pAudit, CWorkingDlg* pDlgWorking );

	void ResetTotals();
	void AddColumnText();

	void SaveLine ( CWeeklySpendDatabase* pDbSpend );
	void SaveDaySpend ( CCSV* pCsvLine, CWeeklySpendDatabase* pDbSpend, int nDayNo );
	void SaveValues ( CCSV* pCsvLine, double dValue, int nCount );
	void SaveTotals();

	CString CreateWeeklySpendFilename ( const char* szText );
	void TidyUpFiles();

private:
	CReportWeeklySpendDlg m_dlgReport;
	CRepmanHandler m_Repman;

	CString m_strAuditFilename;
	CReportFileWithEndText m_Report;

	CUIntArray m_arrayDays;
	double m_dTotalSpend[8];
	int m_nTotalCount[8];
};
