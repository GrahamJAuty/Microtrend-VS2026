#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
//*******************************************************************
#include "ReportStartingBalanceDlg.h"
//*******************************************************************
#define NORMAL				0
#define DELETED				1
#define NORMAL_ESTIMATED	2
#define DELETED_ESTIMATED	3
#define GRAND_TOTAL			4		// must be last in list, used as index into values
#define MAX_SIZE			5		// max array size for above
//*******************************************************************

struct CReportStartingBalanceTotals
{
public:
	CReportStartingBalanceTotals()
	{
		m_nPoints = 0;
		m_dPurse1 = 0.0;
		m_dPurse2 = 0.0;
		m_dPurse3 = 0.0;
		m_nCount = 0;
	};

	int m_nPoints;
	double m_dPurse1;
	double m_dPurse2;
	double m_dPurse3;
	int m_nCount;
};

//*******************************************************************

class CReportStartingBalance
{
public:
	CReportStartingBalance(const char* szAuditFilename, CWnd* pParent);
	bool Show();

private:
	void ResetGroupTotals();
	void ResetGrandTotals();
	int Create(const char* szReportFile);
	void AddColumnText();
	int CompileReport();
	void SaveTotal(int nLevel, int nType);

	bool SaveStartingBalanceLine(const char* szCardNo, bool bCalcBalance, int nGroupNo );
	bool SaveDatabaseLine(__int64 nUserID, int nGroupNo );

private:
	CWnd* m_pParent;
	CReportStartingBalanceDlg m_dlgFilter;
	CDbReportHandler m_DbReporter;
	CSmartPayAuditFile m_auditFile;
	bool m_bAuditFileChecked;

	CString m_strReportLabel;
	CSQLAuditRecord m_atc;
	CReportFileWithEndText m_Report;

private:
	CReportStartingBalanceTotals m_arrayTotals[2][MAX_SIZE];
};

//*******************************************************************
