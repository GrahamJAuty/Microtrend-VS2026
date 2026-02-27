#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\TLogRecord.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//**********************************************************************
#include "PurchasePropertySheet.h"
#include "ReportPurchaseHelpers.h"
//**********************************************************************

class CPurchaseReport
{
public:
	CPurchaseReport ( const char* szLabel, __int64 nSingleUserID, CWnd* pParent );
	void Reset();
	void Show();

private:
	void ResetReportTotals();

	void AddColumnText();
	int DisplayAll();
	int DisplayOne();

	int PrintPurchaseReport (CSQLRowAccountFull& RowAccount, CWorkingDlg* pProgress, bool& bCancelButton );
	int PrintPurchaseRecord (CTLogRecord* pTLogRecord );
	void PrintPurchaseTotals();
	void PrintReportTotals();


	int PrintConsolidateReport (CSQLRowAccountFull& RowAccount, CWorkingDlg* pProgress, bool& bCancelButton );
	void PrintConsolidateTotals();

	int PrintStatementRecord (CTLogRecord* pTLogRecord );

	bool ValidateSale (CTLogRecord* pTLogRecord );
	bool ValidatePlu ( const char* szPluNo );

protected:
	void NewPage();
	void AddCardDetails (CSQLRowAccountFull& RowAccount);
	int AddStatementPurchases(CSQLRowAccountFull& RowAccount, bool bRowOK);

protected:
	CWnd* m_pParent;
	CRepmanHandler m_Repman;

	CReportFileWithEndText m_Report;
	
	CString m_strLastDate;
	bool m_bNewPageReqd;
	
private:
	bool m_bShowPoints;
	double m_dCardSpendTotal;
	double m_dValueTotal;
	int m_nPointsTotal;
	double m_dReportTotalValue;
	double m_dReportTotalQty;
	int m_nReportTotalPoints;
	__int64 m_nSingleUserID;
	CReportConsolidationArray<CSortedInt64Item> m_arrayPluNumbers;

protected:
	CReportPurchaseHelpers m_ReportPurchaseHelpers;
};

//**********************************************************************
