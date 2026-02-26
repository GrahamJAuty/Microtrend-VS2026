#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLDb.h"
//*******************************************************************
#include "AuditRecord.h"
#include "CashlessTotal.h"
#include "CashlessAuditDlg.h"
#include "ReportInfoCashless.h"
//*******************************************************************

class CReportCashlessPosting
{
	friend class CCashlessAuditDlg;

public:
	CReportCashlessPosting ( const char* szLabel, CWnd* pParent );
	~CReportCashlessPosting();

public:
	void ProcessLine();
	void CompileReport ( CLoyaltyAuditFile* pAudit );
	void Show();

private:
	int ShowInternal();
	
private:
	void Reset();
	void AddColumnText();
	void ProcessPcLine ( int nAppNo );
	void ProcessEcrOrExtLine ( int nAppNo );
	void ProcessServerLine ( int nAppNo );
	void CalculateTotals();
	void ShowTotals();
	void ShowLines();
	void CreateLine ( CString& strLine, CString strSource, CString strAction, double dValue1, double dValue2= 0.0 );
	void CalculateDatabaseSpend();
	double GetDailyRefreshValue ( int nGroupNo );
	
private:
	void Consolidate(int nSource, int nAction, int nPurseNo, double dAmount);
	void ConsolidatePlus(int nSource, int nAction, int nPurseNo, double dAmount);
	void ConsolidateMinus(int nSource, int nAction, int nPurseNo, double dAmount);

private:
	CString GetSourceText(int s);
	CString GetPlusActionText(int a);
	CString GetMinusActionText(int a);

public:
	double m_dTotalAdd[3];							// [0]=TotalAdded,[1]=totalPurse1,[2]=TotalPurse2
	double m_dTotalSub[3];							// total taken off cards inperiod

private:
	CWnd* m_pParent;
	CString m_strReportLabel;
	CReportFile m_Report;
	CAuditRecord m_atc;
	CReportInfoCashless m_ReportInfo;
	CCashlessAuditDlg m_dlgCashlessFilter;
	
	bool m_bCancelButton;
	bool m_bShowPossibleSpend;

	CCashlessTotal m_arrayTotals[CASHLESS_SOURCE_CNT][CASHLESS_ACTION_CNT];

	double m_dDailySpend;
	double m_dPossibleSpend;
	double m_dDatabasePossibleSpend;
	CReportConsolidationArray<CConsolidatedDoubleByInt> m_arrayGroupDailyRefresh;
};

//********************************************************************

