/**********************************************************************/
#include "..\SQLHelpers.h"
#include "..\SQLRecordSetConnectionPool.h"
/**********************************************************************/
#include "..\SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "..\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
#include "..\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
/**********************************************************************/
#include "..\..\SmartPay4ManagerBgnd\LegacyWebPaymentOptions.h"
/**********************************************************************/
#include "SQLBuilderCreateTable.h"
#include "SQLNamespaces.h"
/**********************************************************************/
#include "SQLTranMigrateSchema.h"
/**********************************************************************/

void CSQLTranMigrateSchema::DoWork(int nTargetVersion)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_arrayCommands.RemoveAll();
		bool bCanContinue = TRUE;

		switch (nTargetVersion)
		{
		case 2:
			MigrateV2();
			break;

		case 3:
			MigrateV3();
			break;

		case 4:
			MigrateV4();
			break;

		case 5:
			MigrateV5();
			break;

		case 6:
			MigrateV6();
			break;

		case 7:
			MigrateV7();
			break;

		case 8:
			MigrateV8();
			break;

		case 9:
			MigrateV9();
			break;

		case 10:
			MigrateV10();
			break;

		case 11:
			MigrateV11();
			break;

		case 12:
			MigrateV12();
			break;

		case 13:
			MigrateV13();
			break;

		case 14:
			MigrateV14();
			break;

		case 15:
			MigrateV15();
			break;

		case 16:
			MigrateV16();
			break;

		case 17:
			MigrateV17();
			break;

		case 18:
			MigrateV18();
			break;

		case 19:
			MigrateV19();
			break;

		default:
			bCanContinue = FALSE;
		}

		if (FALSE == bCanContinue)
		{
			return;
		}

		{
			CString strDbVersionCommand = "";
			strDbVersionCommand.Format("UPDATE %s SET %s = %d WHERE %s = 1",
				SQLTableNames::DbVersion,
				DbVersion::Version.Label,
				nTargetVersion,
				DbVersion::LineID.Label);

			m_arrayCommands.Add(strDbVersionCommand);
		}

		for (int n = 0; n < m_arrayCommands.GetSize(); n++)
		{
			if (CSQLHelpers::ExecuteSQL(m_pDatabase, m_arrayCommands.GetAt(n)) != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;

		switch (nTargetVersion)
		{
		case 12:
		{
			CSQLRepositoryEposTerminal RepoTerm;
			m_bCanCommit = RepoTerm.UpgradeLegacyData(m_pDatabase, TRUE);
		}
		break;

		case 16:
		{
			CSQLRepositoryWebPaymentOptions RepoOptions;
			m_bCanCommit = RepoOptions.ImportLegacyOptions(m_pDatabase);
		}
		break;

		case 19:
		{
			CSQLRepositoryCCNo RepoCC;
			m_bCanCommit = RepoCC.ImportLegacyCCNo(m_pDatabase);
		}
		break;
		}
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV2()
{
	CString strTable = SQLTableNames::ClosingBalances;
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, ClosingBalance::HaveAdjust.Label, "BIT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, ClosingBalance::Purse1LiabilityAdjust.Label, "FLOAT", TRUE, "0.0");
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, ClosingBalance::Purse1CreditAdjust.Label, "FLOAT", TRUE, "0.0");
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, ClosingBalance::Purse3LiabilityAdjust.Label, "FLOAT", TRUE, "0.0");
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, ClosingBalance::Purse3CreditAdjust.Label, "FLOAT", TRUE, "0.0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV3()
{
	{
		CStringArray arrayTables;
		arrayTables.Add(SQLTableNames::Accounts);
		arrayTables.Add(SQLTableNames::Leavers);

		for (int n = 0; n < arrayTables.GetSize(); n++)
		{
			CString strTable = arrayTables.GetAt(n);
			m_arrayCommands.Add(CSQLHelpers::GetCommandDropColumn(strTable, Account::TotalSpendToDate.Label));
			m_arrayCommands.Add(CSQLHelpers::GetCommandDropColumn(strTable, Account::Purse1Balance.Label));
			m_arrayCommands.Add(CSQLHelpers::GetCommandDropColumn(strTable, Account::Purse3Balance.Label));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse1Liability.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse1Credit.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse1LastSpend.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse1SpendToDate.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse2Balance.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse2LastSpend.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse2SpendToDate.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse3Liability.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse3Credit.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse3LastSpend.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse3SpendToDate.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::CashSpendToDate.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse1Period1.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse1Period2.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse1Period3.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse1Period4.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse1Period5.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse1Period6.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse3Period1.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse3Period2.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse3Period3.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse3Period4.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse3Period5.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpendPurse3Period6.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse1Period1.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse1Period2.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse1Period3.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse1Period4.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse1Period5.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse1Period6.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse3Period1.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse3Period2.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse3Period3.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse3Period4.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse3Period5.Label, 16, 2));
			m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraftPurse3Period6.Label, 16, 2));

			{
				CString strAddColumn = "";
				strAddColumn.Format("%s AS ([%s] + [%s] + [%s] + [%s])",
					(const char*)CSQLHelpers::GetCommandAddColumn(strTable, Account::TotalSpendToDate.Label),
					Account::Purse1SpendToDate.Label,
					Account::Purse2SpendToDate.Label,
					Account::Purse3SpendToDate.Label,
					Account::CashSpendToDate.Label);
				m_arrayCommands.Add(strAddColumn);
			}

			{
				CString strAddColumn = "";
				strAddColumn.Format("%s AS ([%s] + [%s])",
					(const char*)CSQLHelpers::GetCommandAddColumn(strTable, Account::Purse1Balance.Label),
					Account::Purse1Liability.Label,
					Account::Purse1Credit.Label);
				m_arrayCommands.Add(strAddColumn);
			}

			{
				CString strAddColumn = "";
				strAddColumn.Format("%s AS ([%s] + [%s])",
					(const char*)CSQLHelpers::GetCommandAddColumn(strTable, Account::Purse3Balance.Label),
					Account::Purse3Liability.Label,
					Account::Purse3Credit.Label);
				m_arrayCommands.Add(strAddColumn);
			}
		}
	}

	{
		CString strTable = SQLTableNames::AccountGroups;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValue.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValuePeriod1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValuePeriod2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValuePeriod3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValuePeriod4.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValuePeriod5.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValuePeriod6.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse1Period1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse1Period2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse1Period3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse1Period4.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse1Period5.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse1Period6.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse3Period1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse3Period2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse3Period3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse3Period4.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse3Period5.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPurse3Period6.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse1Period1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse1Period2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse1Period3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse1Period4.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse1Period5.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse1Period6.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse3Period1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse3Period2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse3Period3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse3Period4.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse3Period5.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPurse3Period6.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::LowBalancePurse1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::LowBalancePurse2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::LowBalancePurse3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RevalueTrip1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RevalueTrip2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RevalueBonus.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardTrip.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardBonus.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardLimit.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardTrigger.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RedeemTripAsDouble.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RedeemValue.Label, 16, 2));
	}

	{
		CString strTable = SQLTableNames::PeriodRefresh;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, PeriodRefresh::Period1Balance.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, PeriodRefresh::Period2Balance.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, PeriodRefresh::Period3Balance.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, PeriodRefresh::Period4Balance.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, PeriodRefresh::Period5Balance.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, PeriodRefresh::Period6Balance.Label, 16, 2));
	}

	{
		CString strTable = SQLTableNames::ClosingBalances;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse1LiabilityAudit.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse1CreditAudit.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse2BalanceAudit.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse3LiabilityAudit.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse3CreditAudit.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse1LiabilityAdjust.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse1CreditAdjust.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse3LiabilityAdjust.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, ClosingBalance::Purse3CreditAdjust.Label, 16, 2));
	}

	{
		CString strTable = SQLTableNames::BromComPendingItems;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, BromComPendingItem::Amount.Label, 16, 2));
	}

	{
		CString strTable = SQLTableNames::BromComItems;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, BromComItem::Amount.Label, 16, 2));
	}

	{
		CString strTable = SQLTableNames::BromComTransactions;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, BromComTransaction::Amount.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, BromComTransaction::Balance.Label, 16, 2));
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV4()
{
	{
		CString strTable = SQLTableNames::Accounts;
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftStatus.Label, "INT", TRUE, "0");
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftStartDate.Label, "VARCHAR(10)", FALSE);
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftEndDate.Label, "VARCHAR(10)", FALSE);
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftFutureGroup.Label, "INT", TRUE, "0");
	}

	{
		CString strTable = SQLTableNames::Leavers;
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftStatus.Label, "INT", TRUE, "0");
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftStartDate.Label, "VARCHAR(10)", FALSE);
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftEndDate.Label, "VARCHAR(10)", FALSE);
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Account::GroupShiftFutureGroup.Label, "INT", TRUE, "0");
	}

	{
		CString strTable = SQLTableNames::AccountGroups;
		AddAlterTableAddColumnCommand(m_arrayCommands, strTable, Group::ShiftGroups.Label, "VARCHAR(29)", FALSE);
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV5()
{
	CString strTable = SQLTableNames::BiometricRegister;
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, BioRegister::RegDate.Label, "VARCHAR(8)", FALSE);
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, BioRegister::RegTime.Label, "VARCHAR(8)", FALSE);
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, BioRegister::UnRegDate.Label, "VARCHAR(8)", FALSE);
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, BioRegister::UnRegTime.Label, "VARCHAR(8)", FALSE);
	AddAlterTableAddColumnCommand(m_arrayCommands, strTable, BioRegister::UnRegFlag.Label, "BIT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV6()
{
	{
		CString strTable = SQLTableNames::Accounts;
		AddAlterTableNotNullCommand(m_arrayCommands, strTable, Account::GroupShiftStatus.Label, "INT", "0");
		AddAlterTableNotNullCommand(m_arrayCommands, strTable, Account::GroupShiftFutureGroup.Label, "INT", "0");
	}

	{
		CString strTable = SQLTableNames::Leavers;
		AddAlterTableNotNullCommand(m_arrayCommands, strTable, Account::GroupShiftStatus.Label, "INT", "0");
		AddAlterTableNotNullCommand(m_arrayCommands, strTable, Account::GroupShiftFutureGroup.Label, "INT", "0");
	}

	{
		CString strTable = SQLTableNames::ClosingBalances;
		AddAlterTableNotNullCommand(m_arrayCommands, strTable, ClosingBalance::HaveAdjust.Label, "BIT", "0");
	}

	{
		CString strTable = SQLTableNames::BiometricRegister;
		AddAlterTableNotNullCommand(m_arrayCommands, strTable, BioRegister::UnRegFlag.Label, "BIT", "0");
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV7()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreatePurchaseControlRuleTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreatePurchaseControlStampTable());
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PluInfo, PluInfo::PCtrlRuleID.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PluInfo, PluInfo::StampWeight.Label, "INT", TRUE, "1");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::PurchaseControlFlag.Label, "BIT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV8()
{
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::BromComTransactions, BromComTransaction::WebComment.Label, "VARCHAR(500)", FALSE);
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV9()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateImportFilenameCacheTable());
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV10()
{
	CSQLBuilderCreateTable TableCreator{};

	{
		CString strCommand = "";
		strCommand.Format("DROP INDEX %s.IX_%s",
			SQLTableNames::Accounts,
			Account::MemberID.Label);

		m_arrayCommands.Add(strCommand);

		strCommand.Format("DROP INDEX %s.IX_%s",
			SQLTableNames::Leavers,
			Account::MemberID.Label);

		m_arrayCommands.Add(strCommand);
	}

	{
		AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::Accounts, Account::PebblePurseID.Label, "VARCHAR(26)", FALSE);
		AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::Leavers, Account::PebblePurseID.Label, "VARCHAR(26)", FALSE);

		CString strCommand = "";
		strCommand.Format("ALTER TABLE %s ALTER COLUMN %s VARCHAR(80)",
			SQLTableNames::Accounts,
			Account::MemberID.Label);

		m_arrayCommands.Add(strCommand);

		strCommand.Format("ALTER TABLE %s ALTER COLUMN %s VARCHAR(80)",
			SQLTableNames::Leavers,
			Account::MemberID.Label);

		m_arrayCommands.Add(strCommand);
	}

	{
		CString strCommand = "";
		strCommand.Format("CREATE NONCLUSTERED INDEX IX_%s ON %s(%s)",
			Account::MemberID.Label,
			SQLTableNames::Accounts,
			Account::MemberID.Label);

		m_arrayCommands.Add(strCommand);

		strCommand.Format("CREATE NONCLUSTERED INDEX IX_%s ON %s(%s)",
			Account::MemberID.Label,
			SQLTableNames::Leavers,
			Account::MemberID.Label);

		m_arrayCommands.Add(strCommand);
	}

	m_arrayCommands.Add(TableCreator.GetCommandCreatePebbleConfigTable(TRUE));
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebblePendingTransactionTable(FALSE));
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebblePendingTransactionTable(TRUE));
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebbleRecentTransactionTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebbleExportTransactionTable(TRUE));
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebbleExportPaymentTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebblePaymentNameTable());
	TableCreator.AddPebblePaymentNames(m_arrayCommands);
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV11()
{
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PluInfo, PluInfo::ModType.Label, "INT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV12()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateEposTerminalTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreateChartwellExportPaymentTable());
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::DbVersion, DbVersion::ExternalVersion1.Label, "INT", TRUE, "1");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV13()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebbleExportTransactionSentTable());
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_A.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_B.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_C.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_D.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_E.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_F.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_G.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_H.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_I.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_J.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_K.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleConfig, PebbleConfig::VATCODE_L.Label, "INT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV14()
{
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PebbleExportTransactions, PebbleExportTransaction::XFerUserID.Label, "BIGINT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV15()
{
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::AllowRevalKioskFlag.Label, "BIT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV16()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateWebPaymentOptionTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreatePebbleLinkTimeTable());
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV17()
{
	// No changes in this version, just updating the version number
	// This is because the business logic now allows larger spend limits and overdrafts.
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV18()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreatePINNumberTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreatePINNumberIndex());
	m_arrayCommands.Add(TableCreator.GetCommandCreatePINNumberConstraint());
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV19()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateCCNoTable());
}

/**********************************************************************/

CString CSQLTranMigrateSchema::GetPrimaryKeyName(CString strTableName)
{
	CString strPK = "";
	{
		CSQLRecordSetConnectionPool rsFindPK(m_pDatabase);
		CString strCommand = "";
		strCommand.Format("SELECT name FROM sys.key_constraints WHERE type = 'PK' AND SCHEMA_NAME(schema_id) = 'dbo' AND OBJECT_NAME(parent_object_id) = '%s' ",
			(const char*)strTableName);

		rsFindPK.Open(CRecordset::forwardOnly, strCommand, CRecordset::executeDirect);

		int nPass = 0;
		while (rsFindPK.IsEOF() == FALSE)
		{
			if (0 == nPass)
			{
				int nIndex = 0;
				rsFindPK.GetFieldValue(nIndex, strPK);
				rsFindPK.MoveNext();
			}
			else
			{
				strPK = "";
				break;
			}
		}
	}
	return strPK;
}

/**********************************************************************/

CString CSQLTranMigrateSchema::GetDropConstraintCommand(CString strTableName, CString strConstraint)
{
	CString strCommand = "";

	strCommand.Format("ALTER TABLE %s DROP CONSTRAINT %s",
		(const char*)strTableName,
		(const char*)strConstraint);

	return strCommand;
}

/**********************************************************************/

void CSQLTranMigrateSchema::AddAlterTableAddColumnCommand(CStringArray& arrayCommand, CString strTable, CString strField, CString strType, bool bNotNull, CString strDefault)
{
	CString strCommand = "";
	strCommand += "ALTER TABLE ";
	strCommand += strTable;
	strCommand += " ADD ";
	strCommand += strField;
	strCommand += " ";
	strCommand += strType;

	if (FALSE == bNotNull)
	{
		arrayCommand.Add(strCommand);
	}
	else
	{
		strCommand += " NOT NULL CONSTRAINT TEMP_";
		strCommand += strField;
		strCommand += " DEFAULT ";
		strCommand += strDefault;
		arrayCommand.Add(strCommand);

		strCommand = "";
		strCommand += "ALTER TABLE ";
		strCommand += strTable;
		strCommand += " DROP CONSTRAINT TEMP_";
		strCommand += strField;
		arrayCommand.Add(strCommand);
	}
}

/**********************************************************************/

bool CSQLTranMigrateSchema::ColumnExists(CString strTableName, CString strColumnName)
{
	bool bGotColumn = FALSE;
	{
		CSQLRecordSetConnectionPool rsCount(NULL);
		CString strColumnCheck = "";
		strColumnCheck.Format("SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '%s' AND COLUMN_NAME = '%s'",
			(const char*) strTableName,
			(const char*) strColumnName);

		rsCount.Open(CRecordset::forwardOnly, strColumnCheck, CRecordset::executeDirect);

		bGotColumn = !rsCount.IsEOF();
		rsCount.Close();
	}

	return bGotColumn;
}

/**********************************************************************/