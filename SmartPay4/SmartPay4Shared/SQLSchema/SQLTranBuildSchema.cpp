/**********************************************************************/
#include "..\SQLHelpers.h"
/**********************************************************************/
#include "SQLBuilderCreateTable.h"
#include "SQLNamespaces.h"
/**********************************************************************/
#include "SQLTranBuildSchema.h"
/**********************************************************************/

void CSQLTranBuildSchema::DoWork()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CStringArray arrayCommands;

		CSQLBuilderCreateTable TableCreator{};
		arrayCommands.Add(TableCreator.GetCommandCreateVersionTable());
		arrayCommands.Add(TableCreator.GetCommandCreateAccountTable());
		arrayCommands.Add(TableCreator.GetCommandCreateGroupTable());
		arrayCommands.Add(TableCreator.GetCommandCreateExternalAccountTable());
		arrayCommands.Add(TableCreator.GetCommandCreateLeaversTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePeriodRefreshTable());
		arrayCommands.Add(TableCreator.GetCommandCreateBioRegisterTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePluInfoTable());
		arrayCommands.Add(TableCreator.GetCommandCreateAuditPeriodTable());
		arrayCommands.Add(TableCreator.GetCommandCreateUserTextTable());
		arrayCommands.Add(TableCreator.GetCommandCreateClosingBalanceTable());
		arrayCommands.Add(TableCreator.GetCommandCreateBromComPendingOrderTable());
		arrayCommands.Add(TableCreator.GetCommandCreateBromComPendingItemTable());
		arrayCommands.Add(TableCreator.GetCommandCreateBromComItemTable());
		arrayCommands.Add(TableCreator.GetCommandCreateBromComBalanceChangesTable());
		arrayCommands.Add(TableCreator.GetCommandCreateBromComTransactionTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePurchaseControlRuleTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePurchaseControlStampTable());
		arrayCommands.Add(TableCreator.GetCommandCreateImportFilenameCacheTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePebbleConfigTable(FALSE));
		arrayCommands.Add(TableCreator.GetCommandCreatePebblePendingTransactionTable(FALSE));
		arrayCommands.Add(TableCreator.GetCommandCreatePebblePendingTransactionTable(TRUE));
		arrayCommands.Add(TableCreator.GetCommandCreatePebbleRecentTransactionTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePebbleExportTransactionTable(FALSE));
		arrayCommands.Add(TableCreator.GetCommandCreatePebbleExportTransactionSentTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePebbleExportPaymentTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePebbleLinkTimeTable());
		arrayCommands.Add(TableCreator.GetCommandCreateChartwellExportPaymentTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePebblePaymentNameTable());
		//arrayCommands.Add(TableCreator.GetCommandCreateOperatorNameTable());
		arrayCommands.Add(TableCreator.GetCommandCreateEposTerminalTable());
		arrayCommands.Add(TableCreator.GetCommandCreateWebPaymentOptionTable());
		//arrayCommands.Add(TableCreator.GetCommandCreatePurchaseHistoryHeaderTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePINNumberTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePINNumberIndex());
		arrayCommands.Add(TableCreator.GetCommandCreatePINNumberConstraint());
		arrayCommands.Add(TableCreator.GetCommandCreateCCNoTable());
		TableCreator.AddPebblePaymentNames(arrayCommands);

		{
			CString strVersion = "";
			strVersion.Format("INSERT INTO DbVersion(LineId, Version, ExternalVersion1) VALUES(1, %d, 1)",
				SMARTPAY_DBVERSION);

			arrayCommands.Add(strVersion);
		}

		for (int n = 0; n < arrayCommands.GetSize(); n++)
		{
			if (CSQLHelpers::ExecuteSQL(m_pDatabase,arrayCommands.GetAt(n)) != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
