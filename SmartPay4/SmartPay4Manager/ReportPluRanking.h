#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\DbReportHandler.h"
/**********************************************************************/
#include "AccumulatorReportInfo.h"
/**********************************************************************/
static const char* szDISCOUNT_PLUNO = "..";
static const char* szDISCOUNT_ECRTEXT = "Discount";
//*******************************************************************
class CReportPluRanking
{
public:
	CReportPluRanking(CWnd* pParent);
	void Show();

private:
	void AddColumnText(CDbReportHandler& ReportHandler);
	void Consolidate(CString strUserID, CWorkingDlg& dlgWorking, bool& bCheckCancel);
	void OutputRanking(int nStartIdx, int nEndIdx, CDbReportHandler& ReportHandler, bool bDiscount);
	void SaveReportLine(CSQLRowAccountFull& RowAccount, int nRank, double dQty, double dValue, double dPoints, bool bDiscount, CDbReportHandler& ReportHandler);
	void AddPluDetails(__int64 nPluNo, CString strEcrText);
	void AddToTotals(double dQty, double dValue, double dPoints, bool bDiscount);
	void ResetTotals(int nLevel);
	void ShowTotals(int nLevel);

private:
	CWnd* m_pParent;

	CReportFileWithEndText m_Report;
	CString m_strReportLabel;

	bool m_bHaveFractionalQty;

	double m_dTotalQty[3];
	double m_dTotalValue[3];
	double m_dTotalPoints[3];

private:
	CAccumulatorReportInfo m_ReportInfo;
	CReportConsolidationArray<CSortedInt64Item> m_arrayPluNumbers;
	CReportConsolidationArray<CPluRankingReportItem> m_arrayPluSales;
	CReportConsolidationArray<CSortedInt64AndStringItem> m_arrayPluDescription;
};

/**********************************************************************/
