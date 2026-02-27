//**********************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SmartPay4Shared\GlobalState.h"
//**********************************************************************
#include "ProjectSQLRecordSetAccountFull.h"
#include "ProjectSQLRecordSetAuditPeriod.h"
#include "ProjectSQLRecordSetBioRegister.h"
#include "ProjectSQLRecordSetBromComBalanceChanges.h"
#include "ProjectSQLRecordSetBromComItem.h"
#include "ProjectSQLRecordSetBromComPendingItem.h"
#include "ProjectSQLRecordSetBromComPendingOrder.h"
#include "ProjectSQLRecordSetBromComTransaction.h"
#include "ProjectSQLRecordSetCCNo.h"
#include "ProjectSQLRecordSetChartwellExportPayment.h"
#include "ProjectSQLRecordSetClosingBalance.h"
#include "ProjectSQLRecordSetDbVersion.h"
#include "ProjectSQLRecordSetEposTerminal.h"
#include "ProjectSQLRecordSetExternalAccount.h"
#include "ProjectSQLRecordSetGroupFull.h"
#include "ProjectSQLRecordSetImportFilenameCache.h"
#include "ProjectSQLRecordSetPebbleConfig.h"
#include "ProjectSQLRecordSetPebbleLinkTime.h"
#include "ProjectSQLRecordSetPebbleExportPayment.h"
#include "ProjectSQLRecordSetPebbleExportTransaction.h"
#include "ProjectSQLRecordSetPebbleExportTransactionSent.h"
#include "ProjectSQLRecordSetPebblePaymentType.h"
#include "ProjectSQLRecordSetPebblePendingTransaction.h"
#include "ProjectSQLRecordSetPebbleRecentTransaction.h"
#include "ProjectSQLRecordSetPeriodRefresh.h"
#include "ProjectSQLRecordSetPINNumber.h"
#include "ProjectSQLRecordSetPluInfo.h"
#include "ProjectSQLRecordSetPurchaseControlRule.h"
#include "ProjectSQLRecordSetPurchaseControlStamp.h"
#include "ProjectSQLRecordSetUserText.h"
#include "ProjectSQLRecordSetWebPaymentOptions.h"
#include "SQLOut.h"
//**********************************************************************
#include "SQLScriptExport.h"
//**********************************************************************

CSQLScriptExport::CSQLScriptExport() : m_WorkingDlg("Preparing SQL Script for Backup", FALSE)
{
	m_WorkingDlg.Create();
}

//**********************************************************************

void CSQLScriptExport::DoExport()
{
	CSSFile fileOut;
	fileOut.Open(Filenames.GetSQLScriptFallbackFilename(), "wb");
	
	WriteFileBegin(fileOut);
	
	ExportDbVersion(fileOut);
	ExportAccounts(fileOut);
	ExportLeavers(fileOut);
	ExportGroups(fileOut);
	ExportExternalAccounts(fileOut);
	ExportPluInfo(fileOut);
	ExportAuditPeriod(fileOut);
	ExportBioRegister(fileOut);
	ExportPeriodRefresh(fileOut);
	ExportUserText(fileOut);
	ExportClosingBalances(fileOut);
	ExportPurchaseControlRules(fileOut);
	ExportPurchaseControlStamps(fileOut);
	ExportImportFilenameCache(fileOut);
	ExportBromComBalanceChanges(fileOut);
	ExportBromComItems(fileOut);
	ExportBromComPendingItems(fileOut);
	ExportBromComPendingOrders(fileOut);
	ExportBromComTransactions(fileOut);
	ExportEposTerminals(fileOut);
	ExportPebbleConfig(fileOut);
	ExportPebblePendingTransactions(fileOut, FALSE);
	ExportPebblePendingTransactions(fileOut, TRUE);
	ExportPebbleRecentTransactions(fileOut);
	ExportPebbleExportTransactions(fileOut);
	ExportPebbleExportTransactionsSent(fileOut);
	ExportPebbleExportPayments(fileOut);
	ExportPebbleLinkTimes(fileOut);
	ExportPebblePaymentType(fileOut);
	ExportChartwellExportPayments(fileOut);
	ExportWebPaymentOptions(fileOut);
	ExportPINNumbers(fileOut);
	ExportCCNOs(fileOut);

	WriteFileEnd(fileOut);
}

//**********************************************************************

void CSQLScriptExport::WriteFileBegin(CSSFile& fileOut)
{
	CStringArray arrayDummy;
	CSQLOut SQLOut(fileOut, "Dummy", arrayDummy);
	SQLOut.WriteFileBegin();
}

//**********************************************************************

void CSQLScriptExport::ExportDbVersion(CSSFile& fileOut)
{
	CProjectSQLRecordSetDbVersion RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportAccounts(CSSFile& fileOut)
{
	CProjectSQLRecordSetAccountFull RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg, ACCOUNTS_TABLE_CURRENT);
}

//**********************************************************************

void CSQLScriptExport::ExportLeavers(CSSFile& fileOut)
{
	g_GlobalState.SetLeaverYear(2000);
	CProjectSQLRecordSetAccountFull RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg, ACCOUNTS_TABLE_LEAVERS);
	g_GlobalState.ClearLeaverYear();
}

//**********************************************************************

void CSQLScriptExport::ExportGroups(CSSFile& fileOut)
{
	CProjectSQLRecordSetGroupFull RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportExternalAccounts(CSSFile& fileOut)
{
	CProjectSQLRecordSetExternalAccount RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPluInfo(CSSFile& fileOut)
{
	CProjectSQLRecordSetPluInfo RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportAuditPeriod(CSSFile& fileOut)
{
	CProjectSQLRecordSetAuditPeriod RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportBioRegister(CSSFile& fileOut)
{
	CProjectSQLRecordSetBioRegister RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPeriodRefresh(CSSFile& fileOut)
{
	CProjectSQLRecordSetPeriodRefresh RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);	
}

//**********************************************************************

void CSQLScriptExport::ExportUserText(CSSFile& fileOut)
{
	CProjectSQLRecordSetUserText RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportClosingBalances(CSSFile& fileOut)
{
	CProjectSQLRecordSetClosingBalance RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPurchaseControlRules(CSSFile& fileOut)
{
	CProjectSQLRecordSetPurchaseControlRule RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPurchaseControlStamps(CSSFile& fileOut)
{
	CProjectSQLRecordSetPurchaseControlStamp RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportImportFilenameCache(CSSFile& fileOut)
{
	CProjectSQLRecordSetImportFilenameCache RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportBromComBalanceChanges(CSSFile& fileOut)
{
	CProjectSQLRecordSetBromComBalanceChanges RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportBromComItems(CSSFile& fileOut)
{
	CProjectSQLRecordSetBromComItem RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportBromComPendingItems(CSSFile& fileOut)
{
	CProjectSQLRecordSetBromComPendingItem RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportBromComPendingOrders(CSSFile& fileOut)
{
	CProjectSQLRecordSetBromComPendingOrder RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportBromComTransactions(CSSFile& fileOut)
{
	CProjectSQLRecordSetBromComTransaction RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportEposTerminals(CSSFile& fileOut)
{
	CProjectSQLRecordSetEposTerminal RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebbleConfig(CSSFile& fileOut)
{
	CProjectSQLRecordSetPebbleConfig RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebblePendingTransactions(CSSFile& fileOut, bool bUnmatched)
{
	CProjectSQLRecordSetPebblePendingTransaction RecordSet(bUnmatched);
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebbleRecentTransactions(CSSFile& fileOut)
{
	CProjectSQLRecordSetPebbleRecentTransaction RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebbleExportTransactions(CSSFile& fileOut)
{
	CProjectSQLRecordSetPebbleExportTransaction RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebbleExportTransactionsSent(CSSFile& fileOut)
{
	CProjectSQLRecordSetPebbleExportTransactionSent RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebbleExportPayments(CSSFile& fileOut)
{
	CProjectSQLRecordSetPebbleExportPayment RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebbleLinkTimes(CSSFile& fileOut)
{
	CProjectSQLRecordSetPebbleLinkTime RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPebblePaymentType(CSSFile& fileOut)
{
	CProjectSQLRecordSetPebblePaymentType RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportChartwellExportPayments(CSSFile& fileOut)
{
	CProjectSQLRecordSetChartwellExportPayment RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportWebPaymentOptions(CSSFile& fileOut)
{
	CProjectSQLRecordSetWebPaymentOptions RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportPINNumbers(CSSFile& fileOut)
{
	CProjectSQLRecordSetPINNumber RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::ExportCCNOs(CSSFile& fileOut)
{
	CProjectSQLRecordSetCCNo RecordSet;
	RecordSet.ExportToScript(fileOut, m_WorkingDlg);
}

//**********************************************************************

void CSQLScriptExport::WriteFileEnd(CSSFile& fileOut)
{
	CStringArray arrayDummy;
	CSQLOut SQLOut(fileOut, "Dummy", arrayDummy);
	SQLOut.WriteFileEnd();
}

//**********************************************************************
