#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//*******************************************************************
#include "AuditRecord.h"
#include "CashlessAuditDlg.h"
#include "CashlessTotal.h"
#include "RecSort.h"
#include "ReportInfoCashless.h"
#include "WorkingDlg.h"
//*******************************************************************

class CReportCashlessLiability  
{
	friend class CCashlessAuditDlg;

public:
	CReportCashlessLiability(  const char* szLabel, CWnd* pParent);
	void Show();

private:
	int ShowInternal();

private:
	void Reset();
	void AddColumnText();
	void ShowTotals();
	void ShowLines();
	void WriteLine ( const char* szText, double dPurse1, double dPurse2 = 0.0 );

	bool CalculateOpeningBalances ( CLoyaltyAuditFile& AuditFile, CReportConsolidationArray<CSortedIntByInt64>& arrayBalancePos, CWorkingDlg& WorkingDlg );
	void SaveStartingBalanceLine ( bool bCalcBalance ) ;
	
	bool CalculateUsageFigures ( CLoyaltyAuditFile* pAudit, CWorkingDlg& WorkingDlg);
	void ProcessCardUsage( CLoyaltyAuditFile* pAudit, CWorkingDlg& WorkingDlg, bool& bCancel );
	
	void AdjustCardBalance();
	
private:
	void ProcessLine();
	
private:
	CWnd* m_pParent;
	CString m_strReportLabel;
	CReportInfoCashless m_ReportInfo;
	CCashlessAuditDlg m_dlgCashlessFilter;

	CReportFile m_Report;
	CAuditRecord m_atc;

	bool m_bCancelButton;
	double m_dPurse1Opening;
	double m_dPurse2Opening;
	double m_dPurse1Total;
	double m_dPurse2Total;

	CReportConsolidationArray<CConsolidatedCashlessReconcilePurse> m_arrayCardBalances;
	CCashlessTotal m_TotalsPurseTran;
	CCashlessTotal m_TotalsPurseXfer;
	CCashlessTotal m_TotalsPurseAdjust;
};

//*******************************************************************
