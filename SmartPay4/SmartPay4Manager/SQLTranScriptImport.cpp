/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_DbVersion\SQLRowDbVersion.h"
/**********************************************************************/
#include "ProjectSQLRepositoryAccount.h"
#include "ProjectSQLRepositoryAuditPeriod.h"
#include "ProjectSQLRepositoryBioRegister.h"
#include "ProjectSQLRepositoryBromComBalanceChanges.h"
#include "ProjectSQLRepositoryBromComItem.h"
#include "ProjectSQLRepositoryBromComPendingItem.h"
#include "ProjectSQLRepositoryBromComPendingOrder.h"
#include "ProjectSQLRepositoryBromComTransaction.h"
#include "ProjectSQLRepositoryCCNo.h"
#include "ProjectSQLRepositoryChartwellExportPayment.h"
#include "ProjectSQLRepositoryClosingBalance.h"
#include "ProjectSQLRepositoryEposTerminal.h"
#include "ProjectSQLRepositoryExternalAccount.h"
#include "ProjectSQLRepositoryGroup.h"
#include "ProjectSQLRepositoryImportFilenameCache.h"
#include "ProjectSQLRepositoryPebbleConfig.h"
#include "ProjectSQLRepositoryPebbleExportPayment.h"
#include "ProjectSQLRepositoryPebbleExportTransaction.h"
#include "ProjectSQLRepositoryPebbleExportTransactionSent.h"
#include "ProjectSQLRepositoryPebbleLinkTime.h"
#include "ProjectSQLRepositoryPebblePaymentType.h"
#include "ProjectSQLRepositoryPebblePendingTransaction.h"
#include "ProjectSQLRepositoryPebbleRecentTransaction.h"
#include "ProjectSQLRepositoryPeriodRefresh.h"
#include "ProjectSQLRepositoryPINNumber.h"	
#include "ProjectSQLRepositoryPluInfo.h"
#include "ProjectSQLRepositoryPurchaseControlRule.h"
#include "ProjectSQLRepositoryPurchaseControlStamp.h"
#include "ProjectSQLRepositoryUserText.h"
#include "ProjectSQLRepositoryWebPaymentOptions.h"
#include "SQLIn.h"
/**********************************************************************/
#include "SQLTranScriptImport.h"
/**********************************************************************/

CSQLTranScriptImport::CSQLTranScriptImport() : CSQLTranBase(), m_WorkingDlg("Restoring Database from SQL Script", FALSE)
{
	m_WorkingDlg.Create();
	m_bBadDbVersion = FALSE;
	m_nIncomingDbVersion = 0;
}

/**********************************************************************/

void CSQLTranScriptImport::DoWork(bool bUseFallbackScript)
{
	m_nIncomingDbVersion = 0;

	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;
		m_bBadDbVersion = TRUE;

		CSSFile fileIn;
		if (FALSE == bUseFallbackScript)
		{
			if (fileIn.Open(Filenames.GetSQLScriptLegacyFilename(), "rb") == FALSE)
			{
				return;
			}
		}
		else
		{
			if (fileIn.Open(Filenames.GetSQLScriptFallbackFilename(), "rb") == FALSE)
			{
				return;
			}
		}

		if (CheckDbVersion(fileIn) == FALSE)
		{
			return;
		}

		m_bBadDbVersion = FALSE;

		
		if (ImportAccounts(fileIn,ACCOUNTS_TABLE_CURRENT) == FALSE)
		{
			return;
		}

		if (ImportAccounts(fileIn,ACCOUNTS_TABLE_LEAVERS) == FALSE)
		{
			return;
		}

		if (ImportGroups(fileIn) == FALSE)
		{
			return;
		}

		if (ImportExternalAccounts(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPluInfo(fileIn) == FALSE)
		{
			return;
		}

		if (ImportAuditPeriods(fileIn) == FALSE)
		{
			return;
		}

		if (ImportBioRegister(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPeriodRefresh(fileIn) == FALSE)
		{
			return;
		}

		if (ImportUserTexts(fileIn) == FALSE)
		{
			return;
		}

		if (ImportClosingBalances(fileIn) == FALSE)
		{
			return;
		}

		if (ImportBromComBalanceChanges(fileIn) == FALSE)
		{
			return;
		}

		if (ImportBromComItems(fileIn) == FALSE)
		{
			return;
		}

		if (ImportBromComPendingItems(fileIn) == FALSE)
		{
			return;
		}

		if (ImportBromComPendingOrders(fileIn) == FALSE)
		{
			return;
		}

		if (ImportBromComTransactions(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPurchaseControlRules(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPurchaseControlStamps(fileIn) == FALSE)
		{
			return;
		}

		if (ImportImportFilenameCache(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPebbleConfig(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPebblePendingTransactions(fileIn,FALSE) == FALSE)
		{
			return;
		}

		if (ImportPebblePendingTransactions(fileIn, TRUE) == FALSE)
		{
			return;
		}

		if (ImportPebbleRecentTransactions(fileIn) == FALSE)
		{
			return;
		}

		m_arrayPebbleExportTransaction.RemoveAll();
		if (ImportPebbleExportTransactions(fileIn) == FALSE)
		{
			return;
		}

		m_arrayPebbleExportTransactionSent.RemoveAll();
		if (ImportPebbleExportTransactionsSent(fileIn) == FALSE)
		{
			return;
		}

		if (ProcessPebbleExportTransactions() == FALSE)
		{
			return;
		}

		if (ImportPebbleExportPayments(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPebbleLinkTimes(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPebblePaymentTypes(fileIn) == FALSE)
		{
			return;
		}

		if (ImportChartwellExportPayments(fileIn) == FALSE)
		{
			return;
		}

		if (ImportPINNumbers(fileIn) == FALSE)
		{
			return;
		}

		if (ImportEposTerminals(fileIn) == FALSE)
		{
			return;
		}
		
		if (ImportWebPaymentOptions(fileIn) == FALSE)
		{
			return;
		}

		if (ImportCCNOs(fileIn) == FALSE)
		{
			return;
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

bool CSQLTranScriptImport::CheckDbVersion(CSSFile& fileIn)
{
	fileIn.Seek(0, CFile::begin);

	CStringArray arrayHeader;
	arrayHeader.Add(DbVersion::LineID.Label);
	arrayHeader.Add(DbVersion::Version.Label);
	
	CSQLIn SQLIn(fileIn, SQLTableNames::DbVersion, arrayHeader);

	bool bResult = FALSE;
	
	CString strBuffer = "";
	while (SQLIn.ReadLine(strBuffer))
	{
		CCSV csvIn(strBuffer);

		m_nIncomingDbVersion = csvIn.GetInt(1);

		if ((m_nIncomingDbVersion >= 1) && (m_nIncomingDbVersion <= SMARTPAY_DBVERSION))
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportAccounts(CSSFile& fileIn, int nTableType)
{
	CProjectSQLRepositoryAccount repoAccount(nTableType);
	return repoAccount.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportGroups(CSSFile& fileIn)
{
	CProjectSQLRepositoryGroup repoGroup;
	return repoGroup.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportExternalAccounts(CSSFile& fileIn)
{
	CProjectSQLRepositoryExternalAccount repoExt;
	return repoExt.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPluInfo(CSSFile& fileIn)
{
	CProjectSQLRepositoryPluInfo repoPluInfo;
	return repoPluInfo.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportAuditPeriods(CSSFile& fileIn)
{
	CProjectSQLRepositoryAuditPeriod repoAuditPeriod;
	return repoAuditPeriod.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportBioRegister(CSSFile& fileIn)
{
	CProjectSQLRepositoryBioRegister repoBioRegister;
	return repoBioRegister.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPeriodRefresh(CSSFile& fileIn)
{
	CProjectSQLRepositoryPeriodRefresh repoPeriodRefresh;
	return repoPeriodRefresh.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportUserTexts(CSSFile& fileIn)
{
	CProjectSQLRepositoryUserText repoUserText;
	return repoUserText.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportClosingBalances(CSSFile& fileIn)
{
	CProjectSQLRepositoryClosingBalance repoClosingBalance;
	return repoClosingBalance.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportBromComBalanceChanges(CSSFile& fileIn)
{
	CProjectSQLRepositoryBromComBalanceChanges repoBCBalChg;
	return repoBCBalChg.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportBromComItems(CSSFile& fileIn)
{
	CProjectSQLRepositoryBromComItem repoBCItem;
	return repoBCItem.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportBromComPendingItems(CSSFile& fileIn)
{
	CProjectSQLRepositoryBromComPendingItem repoBCPendItem;
	return repoBCPendItem.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportBromComPendingOrders(CSSFile& fileIn)
{
	CProjectSQLRepositoryBromComPendingOrder repoBCPendOrder;
	return repoBCPendOrder.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportBromComTransactions(CSSFile& fileIn)
{
	CProjectSQLRepositoryBromComTransaction repoBCTran;
	return repoBCTran.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPurchaseControlRules(CSSFile& fileIn)
{
	CProjectSQLRepositoryPurchaseControlRule repoPCRule;
	return repoPCRule.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPurchaseControlStamps(CSSFile& fileIn)
{
	CProjectSQLRepositoryPurchaseControlStamp repoPCStamp;
	return repoPCStamp.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportImportFilenameCache(CSSFile& fileIn)
{
	CProjectSQLRepositoryImportFilenameCache repoCache;
	return repoCache.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebbleConfig(CSSFile& fileIn)
{
	CProjectSQLRepositoryPebbleConfig repoConfig;
	return repoConfig.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebblePendingTransactions(CSSFile& fileIn, bool bUnmatched)
{
	CProjectSQLRepositoryPebblePendingTransaction repoPendTran(bUnmatched);
	return repoPendTran.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebbleRecentTransactions(CSSFile& fileIn)
{
	CProjectSQLRepositoryPebbleRecentTransaction repoTran;
	return repoTran.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebbleExportTransactions(CSSFile& fileIn)
{
	CProjectSQLRepositoryPebbleExportTransaction RepoTran;
	return RepoTran.ImportFromScript(fileIn, m_pDatabase, m_arrayPebbleExportTransaction, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebbleExportTransactionsSent(CSSFile& fileIn)
{
	CProjectSQLRepositoryPebbleExportTransactionSent RepoTran;
	return RepoTran.ImportFromScript(fileIn, m_pDatabase, m_arrayPebbleExportTransactionSent, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ProcessPebbleExportTransactions()
{
	CProjectSQLRepositoryPebbleExportTransaction RepoTran;
	return RepoTran.ProcessImportedTransactions(m_pDatabase, m_arrayPebbleExportTransaction, m_arrayPebbleExportTransactionSent);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebbleExportPayments(CSSFile& fileIn)
{
	CProjectSQLRepositoryPebbleExportPayment RepoPay;
	return RepoPay.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebbleLinkTimes(CSSFile& fileIn)
{
	CProjectSQLRepositoryPebbleLinkTime RepoLink;
	return RepoLink.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPebblePaymentTypes(CSSFile& fileIn)
{
	CProjectSQLRepositoryPebblePaymentType RepoType;
	return RepoType.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportChartwellExportPayments(CSSFile& fileIn)
{
	CProjectSQLRepositoryChartwellExportPayment RepoPay;
	return RepoPay.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportEposTerminals(CSSFile& fileIn)
{
	CProjectSQLRepositoryEposTerminal RepoTerminal;
	return RepoTerminal.ImportFromScript(fileIn, m_pDatabase, m_nIncomingDbVersion, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportWebPaymentOptions(CSSFile& fileIn)
{
	CProjectSQLRepositoryWebPaymentOptions RepoOptions;
	return RepoOptions.ImportFromScript(fileIn, m_pDatabase, m_nIncomingDbVersion, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportPINNumbers(CSSFile& fileIn)
{
	CProjectSQLRepositoryPINNumber RepoPIN;
	return RepoPIN.ImportFromScript(fileIn, m_pDatabase, m_WorkingDlg);
}

/**********************************************************************/

bool CSQLTranScriptImport::ImportCCNOs(CSSFile& fileIn)
{
	CProjectSQLRepositoryCCNo repoCC;
	return repoCC.ImportFromScript(fileIn, m_pDatabase, m_nIncomingDbVersion, m_WorkingDlg);
}

/**********************************************************************/
