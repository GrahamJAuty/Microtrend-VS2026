#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\ProgressDlg.h"
#include "..\SmartPay4ManagerBgnd\RepmanHandler.h"
/**********************************************************************/
#include "ReportKioskAuditDlg.h"
/**********************************************************************/

class CReportKioskAudit
{
public:
	CReportKioskAudit ( const char* szLabel, CWnd* pParent );
	void Show();

private:
	int Create();
	void ResetTotals(int nIndex);
	void AddColumnText();
	bool SaveLine ( CSQLAuditRecord*  pAtc, bool bCardExists );
	void SaveTotals ( int nIndex, int nKioskNo = 0 );
	void SaveSummaryTotals ( int nKioskNo = 0 );

	int ReportByKiosk ( CSmartPayAuditFile* pAudit, CProgressDlg* pDlgProgress );

private:
	CWnd* m_pParent;
	CReportKioskAuditDlg m_dlgReport;
	CRepmanHandler m_Repman;

	CString m_strAuditFilename;
	CReportFileWithEndText m_Report;

	double m_dTotalAddedPurse1[2];
	double m_dTotalAddedPurse3[2];
};

/**********************************************************************/
