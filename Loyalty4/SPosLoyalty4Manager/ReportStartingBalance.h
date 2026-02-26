#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
//*******************************************************************
#include "DbReportHandler.h"
#include "ReportInfoStartingBalance.h"
#include "ReportStartingBalanceDlg.h"
#include "WorkingDlg.h"
//*******************************************************************
#define NORMAL				0
#define DELETED				1
#define NORMAL_ESTIMATED	2
#define DELETED_ESTIMATED	3
#define GRAND_TOTAL			4		// must be last in list, used as index into values
#define MAX_SIZE			5		// max array size for above
//*******************************************************************

class CReportStartingBalance
{
public:
	CReportStartingBalance ( const char* szAuditFilename, CWnd* pParent  );
	bool Show();

private:
	void Reset();
	void AddColumnText();
	int CompileReport();
	void SaveTotal ( int nType );

	void SaveStartingBalanceLine (CArray<int, int>& arrayHeader, CString strCardNo, bool bCalcBalance );
	void SaveDatabaseLine ( CArray<int,int>& arrayHeader, __int64 nCardNo );

public:
	static bool FindStartingBalances(
		CReportConsolidationArray<CSortedInt64WithFlag>& arrayCurrentIDS,
		bool bIncludeUnknowns,
		CLoyaltyAuditFile& auditFile,
		CString strSearchStartDate,
		CString strSearchEndDate,
		CString strReportStartDate,
		CString strReportStartTime,
		CReportConsolidationArray<CSortedIntByInt64>& arrayBalancePos,
		CWorkingDlg& WorkingDlg);
		
private:
	CWnd* m_pParent;
	CReportInfoStartingBalance m_ReportInfo;
	CReportStartingBalanceDlg m_dlgFilter;
	CDbReportHandler m_DbReporter;

	CLoyaltyAuditFile m_auditFile;
	CAuditRecord m_atc;
	CReportFile m_Report;

	CString m_strReportLabel;

	long   m_lTotalPoints[MAX_SIZE];
	double m_dTotalPurse1[MAX_SIZE];
	double m_dTotalPurse2[MAX_SIZE];
	int    m_nTotalCount[MAX_SIZE];
};

//*******************************************************************
