#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyAuditFile.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//*******************************************************************
#include "AuditRecord.h"
#include "CashlessTotal.h"
//*******************************************************************

class CReportDailyDifference
{
public:
	CReportDailyDifference ( CWnd* pParent );
	~CReportDailyDifference();

public:
	void Show();

private:
	int Create(CString strReportFile);	
	void ResetAllTotals();
	void ResetLineTotals();
	void AddColumnText();
	void ProcessLine();
	void CreateReportLine(CString strDate);
	void CreateTotalsLine();

private:
	void Consolidate(int nAction, int nPurseNo, double dAmount);
	void ConsolidatePlus(int nAction, int nPurseNo, double dAmount);
	void ConsolidateMinus(int nAction, int nPurseNo, double dAmount);
	
private:
	CLoyaltyAuditFile m_auditFile;	
	CWnd* m_pParent;
	CString m_strReportLabel;
	CString m_strCaption;
	CReportFile m_Report;
	CAuditRecord m_atc;	
	CCashlessTotal m_arrayLineTotals[CASHLESS_ACTION_CNT];
	bool m_bShowDetail;

private:
	double m_dPurse1TotalAddTopup;
	double m_dPurse1TotalAddOther;
	double m_dPurse1TotalSubSpend;
	double m_dPurse1TotalSubOther;
	double m_dPurse2TotalAddTopup;
	double m_dPurse2TotalAddOther;
	double m_dPurse2TotalSubSpend;
	double m_dPurse2TotalSubOther;
	int m_nPurse1TotalAddTopupCount;
	int m_nPurse1TotalAddOtherCount;
	int m_nPurse1TotalSubSpendCount;
	int m_nPurse1TotalSubOtherCount;
	int m_nPurse2TotalAddTopupCount;
	int m_nPurse2TotalAddOtherCount;
	int m_nPurse2TotalSubSpendCount;
	int m_nPurse2TotalSubOtherCount;
};

//********************************************************************

