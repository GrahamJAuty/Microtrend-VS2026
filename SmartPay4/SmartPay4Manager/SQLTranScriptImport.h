#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRowPebbleExportTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransactionSent\SQLRowPebbleExportTransactionSent.h"
/**********************************************************************/
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranScriptImport : public CSQLTranBase
{
public:
	CSQLTranScriptImport();

public:
	void DoWork(bool bUseFallbackScript);

public:
	bool GetBadDbVersionFlag() { return m_bBadDbVersion; }

private:
	bool CheckDbVersion(CSSFile& fileIn);
	bool ImportAccounts(CSSFile& fileIn, int nTableType);
	bool ImportGroups(CSSFile& fileIn);
	bool ImportExternalAccounts(CSSFile& fileIn);
	bool ImportPluInfo(CSSFile& fileIn);
	bool ImportAuditPeriods(CSSFile& fileIn);
	bool ImportBioRegister(CSSFile& fileIn);
	bool ImportPeriodRefresh(CSSFile& fileIn);
	bool ImportClosingBalances(CSSFile& fileIn);
	bool ImportUserTexts(CSSFile& fileIn);
	bool ImportPurchaseControlRules(CSSFile& fileIn);
	bool ImportPurchaseControlStamps(CSSFile& fileIn);
	bool ImportImportFilenameCache(CSSFile& fileIn);
	bool ImportBromComBalanceChanges(CSSFile& fileIn);
	bool ImportBromComItems(CSSFile& fileIn);
	bool ImportBromComPendingItems(CSSFile& fileIn);
	bool ImportBromComPendingOrders(CSSFile& fileIn);
	bool ImportBromComTransactions(CSSFile& fileIn);
	bool ImportEposTerminals(CSSFile& fileIn);
	bool ImportPebbleConfig(CSSFile& fileIn);
	bool ImportPebblePendingTransactions(CSSFile& fileIn,bool bUnmatched);
	bool ImportPebbleRecentTransactions(CSSFile& fileIn);
	bool ImportPebbleExportTransactions(CSSFile& fileIn);
	bool ImportPebbleExportTransactionsSent(CSSFile& fileIn);
	bool ImportPebbleExportPayments(CSSFile& fileIn);
	bool ImportPebbleLinkTimes(CSSFile& fileIn);
	bool ImportPebblePaymentTypes(CSSFile& fileIn);
	bool ImportChartwellExportPayments(CSSFile& fileIn);
	bool ImportWebPaymentOptions(CSSFile& fileIn);
	bool ImportPINNumbers(CSSFile& fileIn);
	bool ImportCCNOs(CSSFile& fileIn);

private:
	bool ProcessPebbleExportTransactions();

private:
	int m_nIncomingDbVersion = 0;
	bool m_bBadDbVersion = FALSE;
	CArray<CSQLRowPebbleExportTransaction, CSQLRowPebbleExportTransaction&> m_arrayPebbleExportTransaction;
	CArray<CSQLRowPebbleExportTransactionSent, CSQLRowPebbleExportTransactionSent&> m_arrayPebbleExportTransactionSent;

private:
	CWorkingDlg m_WorkingDlg;
};

/**********************************************************************/
