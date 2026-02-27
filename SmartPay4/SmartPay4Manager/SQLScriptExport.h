#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\WorkingDlg.h"
//**********************************************************************

class CSQLScriptExport
{
public:
	CSQLScriptExport();

public:
	void DoExport();

private:
	void WriteFileBegin(CSSFile& fileOut);
	void ExportDbVersion(CSSFile& fileOut);
	void ExportAccounts(CSSFile& fileOut);
	void ExportLeavers(CSSFile& fileOut);
	void ExportGroups(CSSFile& fileOut);
	void ExportExternalAccounts(CSSFile& fileOut);
	void ExportPluInfo(CSSFile& fileOut);
	void ExportAuditPeriod(CSSFile& fileOut);
	void ExportBioRegister(CSSFile& fileOut);
	void ExportPeriodRefresh(CSSFile& fileOut);
	void ExportUserText(CSSFile& fileOut);
	void ExportClosingBalances(CSSFile& fileOut);
	void ExportPurchaseControlRules(CSSFile& fileOut);
	void ExportPurchaseControlStamps(CSSFile& fileOut);
	void ExportImportFilenameCache(CSSFile& fileOut);
	void ExportBromComBalanceChanges(CSSFile& fileOut);
	void ExportBromComItems(CSSFile& fileOut);
	void ExportBromComPendingItems(CSSFile& fileOut);
	void ExportBromComPendingOrders(CSSFile& fileOut);
	void ExportBromComTransactions(CSSFile& fileOut);
	void ExportPebbleConfig(CSSFile& fileOut);
	void ExportPebblePendingTransactions(CSSFile& fileOut,bool bUnmatched);
	void ExportPebbleRecentTransactions(CSSFile& fileOut);
	void ExportPebbleExportTransactions(CSSFile& fileOut);
	void ExportPebbleExportTransactionsSent(CSSFile& fileOut);
	void ExportPebbleExportPayments(CSSFile& fileOut);
	void ExportPebbleLinkTimes(CSSFile& fileOut);
	void ExportPebblePaymentType(CSSFile& fileOut);
	void ExportChartwellExportPayments(CSSFile& fileOut);
	void ExportEposTerminals(CSSFile& fileOut);
	void ExportWebPaymentOptions(CSSFile& fileOut);
	void ExportPINNumbers(CSSFile& fileOut);
	void ExportCCNOs(CSSFile& fileOut);
	void WriteFileEnd(CSSFile& fileOut);

private:
	CWorkingDlg m_WorkingDlg;
};

//**********************************************************************
