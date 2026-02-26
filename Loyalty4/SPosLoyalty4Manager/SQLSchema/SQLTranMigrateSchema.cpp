/**********************************************************************/
#include "..\..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\..\SPosLoyalty4Shared\SQLRecordSetConnectionPool.h"
/**********************************************************************/
#include "..\..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRepositoryAuditPeriod.h"
#include "..\..\SPosLoyalty4Shared\SQLTable_CCNo\SQLRepositoryCCNo.h"
#include "..\..\SPosLoyalty4Shared\SQLTable_Group\SQLRecordSetGroupLegacy.h"
#include "..\..\SPosLoyalty4Shared\SQLTable_Group\SQLRowGroupLegacy.h"
/**********************************************************************/
#include "SQLBuilderCreateTable.h"
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
			bCanContinue = MigrateV12();
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

		switch (nTargetVersion)
		{
		case 5:
			m_bCanCommit = PostMigrateV5();
			break;

		default:
			m_bCanCommit = TRUE;
			break;
		}
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV2()
{
	m_arrayCommands.Add("ALTER TABLE Accounts ADD RowVersion ROWVERSION");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV3()
{
	CString strCommand = "";

	strCommand.Format("ALTER TABLE Accounts ALTER COLUMN %s VARCHAR(%d)",
		Account::Purse1LastSpendDate.Label,
		Account::Purse1LastSpendDate.Max);
	m_arrayCommands.Add(strCommand);

	strCommand.Format("ALTER TABLE Accounts ALTER COLUMN %s VARCHAR(%d)",
		Account::Purse2LastSpendDate.Label,
		Account::Purse2LastSpendDate.Max);
	m_arrayCommands.Add(strCommand);

	strCommand.Format("ALTER TABLE Accounts ALTER COLUMN %s VARCHAR(%d)",
		Account::Purse2RefreshedDate.Label,
		Account::Purse2RefreshedDate.Max);
	m_arrayCommands.Add(strCommand);

	strCommand.Format("ALTER TABLE Accounts ALTER COLUMN %s VARCHAR(%d)",
		Account::CashLastSpendDate.Label,
		Account::CashLastSpendDate.Max);
	m_arrayCommands.Add(strCommand);
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV4()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateStaffCardTable(TRUE));
	m_arrayCommands.Add(TableCreator.GetCommandCreateStaffGiftExceptionTable(TRUE));
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV5()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateAuditPeriodTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreateCCNoTable());
}

/**********************************************************************/

bool CSQLTranMigrateSchema::PostMigrateV5()
{
	CSQLRepositoryCCNo repoCC;
	if (repoCC.ImportLegacyCCNo(m_pDatabase) == FALSE)
	{
		return FALSE;
	}

	CSQLRepositoryAuditPeriod repoPeriod;
	if (repoPeriod.ImportLegacyAuditPeriod(m_pDatabase) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV6()
{
	{
		CString strTable = SQLTableNames::Accounts;
		m_arrayCommands.Add(CSQLHelpers::GetCommandDropColumn(strTable, "TotalSpendToDate"));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse1Balance.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse1LastSpend.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse1SpendToDate.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse2Balance.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse2LastSpend.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::Purse2SpendToDate.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::CashLastSpend.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::CashSpendToDate.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxSpend.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Account::MaxOverdraft.Label, 16, 2));

		{
			CString strAddColumn = "";
			strAddColumn.Format("%s AS ([%s] + [%s] + [%s])",
				(const char*)CSQLHelpers::GetCommandAddColumn(strTable, Account::TotalSpendToDate.Label),
				Account::Purse1SpendToDate.Label,
				Account::Purse2SpendToDate.Label,
				Account::CashSpendToDate.Label);
			m_arrayCommands.Add(strAddColumn);
		}
	}

	{
		CString strTable = SQLTableNames::AccountGroups;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RefreshValue.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPeriod1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPeriod2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPeriod3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPeriod4.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPeriod5.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxSpendPeriod6.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPeriod1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPeriod2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPeriod3.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPeriod4.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPeriod5.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::MaxOverdraftPeriod6.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::LegacyRevalueTrip1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::LegacyRevalueTrip2.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::LegacyRevalueBonus1.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardTrip.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardBonus.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardLimit.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RewardTrigger.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RedeemTripAsDouble.Label, 16, 2));
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, Group::RedeemValue.Label, 16, 2));
	}

	{
		CString strTable = SQLTableNames::StaffGiftExceptions;
		m_arrayCommands.Add(CSQLHelpers::GetCommandAlterColumnDecimal(strTable, StaffGiftExceptions::Value.Label, 16, 2));
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV7()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateOfferStampTable());
	m_arrayCommands.Add(TableCreator.GetCommandCreateStampOfferInfoTable());
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PluInfo, PluInfo::StampOfferID.Label, "INT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV8()
{
	//WORK AROUND FOR PREVIOUS BUG WHERE COLUMN WAS ADDED WITHOUT CHANGING DB VERSION
	if (ColumnExists(SQLTableNames::AccountGroups, Group::StampOfferFlag.Label) == FALSE)
	{
		AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::StampOfferFlag.Label, "BIT", TRUE, "0");
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV9()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateDeptInfoTable(TRUE));
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::NoDeptBonus.Label, "BIT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV10()
{
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueFlag2.Label, "BIT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueDisable2.Label, "BIT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueType2.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevaluePoints2.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueBonus2.Label, "DECIMAL(16,2)", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevaluePercent2.Label, "INT", TRUE, "0");

	CString strUpdate = "";
	strUpdate.Format("UPDATE %s SET %s = %s, %s = %s, %s = %s, %s = %s, %s = %s, %s = %s",
		SQLTableNames::AccountGroups,
		Group::LegacyRevalueFlag2.Label, Group::LegacyRevalueFlag1.Label,
		Group::LegacyRevalueDisable2.Label, Group::LegacyRevalueDisable1.Label,
		Group::LegacyRevalueType2.Label, Group::LegacyRevalueType1.Label,
		Group::LegacyRevaluePoints2.Label, Group::LegacyRevaluePoints1.Label,
		Group::LegacyRevaluePercent2.Label, Group::LegacyRevaluePercent1.Label,
		Group::LegacyRevalueBonus2.Label, Group::LegacyRevalueBonus1.Label);

	m_arrayCommands.Add(strUpdate);
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV11()
{
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueFlag3.Label, "BIT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueDisable3.Label, "BIT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueTrip3.Label, "DECIMAL(16,2)", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueType3.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevaluePoints3.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueBonus3.Label, "DECIMAL(16,2)", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevaluePercent3.Label, "INT", TRUE, "0");
	/*****/
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueFlag4.Label, "BIT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueDisable4.Label, "BIT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueTrip4.Label, "DECIMAL(16,2)", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueType4.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevaluePoints4.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevalueBonus4.Label, "DECIMAL(16,2)", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::AccountGroups, Group::LegacyRevaluePercent4.Label, "INT", TRUE, "0");
}

/**********************************************************************/

bool CSQLTranMigrateSchema::MigrateV12()
{
	CString strPKPlu = GetPrimaryKeyName(SQLTableNames::PluInfo);

	if (strPKPlu == "")
	{
		return FALSE;
	}

	CString strPKDept = GetPrimaryKeyName(SQLTableNames::DeptInfo);

	if (strPKDept == "")
	{
		return FALSE;
	}

	CString strPKStaffCard = GetPrimaryKeyName(SQLTableNames::StaffCards);

	if (strPKStaffCard == "")
	{
		return FALSE;
	}

	CSQLBuilderCreateTable TableCreator{};
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::PluInfo, PluInfo::DbNo.Label, "INT", TRUE, "1");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::DeptInfo, DeptInfo::DbNo.Label, "INT", TRUE, "1");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::StaffCards, StaffCards::FolderIndexDbNo.Label, "INT", TRUE, "0");
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::StaffGiftExceptions, StaffGiftExceptions::FolderIndexDbNo.Label, "INT", TRUE, "0");
	
	m_arrayCommands.Add(GetDropConstraintCommand(SQLTableNames::PluInfo, strPKPlu));
	{
		CString strCommand = "";
		strCommand.Format("ALTER TABLE %s ADD CONSTRAINT PK_%s PRIMARY KEY (%s, %s)",
			SQLTableNames::PluInfo,
			SQLTableNames::PluInfo,
			PluInfo::DbNo.Label,
			PluInfo::PluNo.Label);

		m_arrayCommands.Add(strCommand);
	}

	m_arrayCommands.Add(GetDropConstraintCommand(SQLTableNames::DeptInfo, strPKDept));
	{
		CString strCommand = "";
		strCommand.Format("ALTER TABLE %s ADD CONSTRAINT PK_%s PRIMARY KEY (%s, %s)",
			SQLTableNames::DeptInfo,
			SQLTableNames::DeptInfo,
			DeptInfo::DbNo.Label,
			DeptInfo::DeptNo.Label);
		m_arrayCommands.Add(strCommand);
	}

	m_arrayCommands.Add(GetDropConstraintCommand(SQLTableNames::StaffCards, strPKStaffCard));
	{
		CString strCommand = "";
		strCommand.Format("ALTER TABLE %s ADD CONSTRAINT PK_%s PRIMARY KEY (%s, %s, %s)",
			SQLTableNames::StaffCards,
			SQLTableNames::StaffCards,
			StaffCards::FolderIndexDbNo.Label,
			StaffCards::FolderIndexSetNo.Label,
			StaffCards::ServerNo.Label);
		m_arrayCommands.Add(strCommand);
	}

	return TRUE;
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV13()
{
	CString strCommand = "";

	strCommand.Format("ALTER TABLE %s ALTER COLUMN %s VARCHAR(%d)",
		SQLTableNames::AccountGroups,
		Group::GroupName.Label,
		Group::GroupName.Max);

	m_arrayCommands.Add(strCommand);
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV14()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateEcrmanDatabaseTable());
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV15()
{
	CString strCommand = "";
	strCommand.Format("CREATE NONCLUSTERED INDEX IX_%s ON %s (%s)",
		Account::Info2.Label,
		SQLTableNames::Accounts,
		Account::Info2.Label);

	m_arrayCommands.Add(strCommand);
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV16()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateGroupTopupBonusTable());

	CStringArray arrayColumnsToCheck;
	arrayColumnsToCheck.Add(Group::LegacyRevalueFlag1.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueFlag2.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueFlag3.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueFlag4.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueDisable1.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueDisable2.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueDisable3.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueDisable4.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueTrip1.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueTrip2.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueTrip3.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueTrip4.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueType1.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueType2.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueType3.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueType4.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueBonus1.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueBonus2.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueBonus3.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevalueBonus4.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePoints1.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePoints2.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePoints3.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePoints4.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePercent1.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePercent2.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePercent3.Label);
	arrayColumnsToCheck.Add(Group::LegacyRevaluePercent4.Label);

	bool bGotAllColumns = TRUE;
	for (int n = 0; n < arrayColumnsToCheck.GetSize(); n++)
	{
		if (ColumnExists(SQLTableNames::AccountGroups, arrayColumnsToCheck.GetAt(n)) == FALSE)
		{
			bGotAllColumns = FALSE;
			break;
		}
	}

	if (TRUE == bGotAllColumns)
	{
		CSQLRowGroupLegacy RowGroup;
		CSQLRecordSetGroupLegacy RecordSetGroup(m_pDatabase, RSParams_GroupLegacy_NormalNoParams{});

		while (RecordSetGroup.StepSelectAll(RowGroup))
		{
			for (int n = 1; n <= 4; n++)
			{
				CString strCommand = "";

				strCommand.Format("INSERT INTO %s (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s) VALUES (%d, 1, %d, %d, %d, %.2f, %d, %.2f, %d, %d)",
					SQLTableNames::GroupTopupBonuses,
					GroupTopupBonus::GroupNo.Label,
					GroupTopupBonus::PurseNo.Label,
					GroupTopupBonus::TopupNo.Label,
					GroupTopupBonus::BonusFlag.Label,
					GroupTopupBonus::BonusDisable.Label,
					GroupTopupBonus::BonusTrip.Label,
					GroupTopupBonus::BonusType.Label,
					GroupTopupBonus::BonusValue.Label,
					GroupTopupBonus::BonusPoints.Label,
					GroupTopupBonus::BonusPercent.Label,
					RowGroup.GetGroupNo(),
					n,
					RowGroup.GetRevalueFlag(n) ? 1 : 0,
					RowGroup.GetRevalueDisable(n) ? 1 : 0,
					RowGroup.GetRevalueTrip(n),
					RowGroup.GetRevalueType(n),
					RowGroup.GetRevalueBonus(n),
					RowGroup.GetRevaluePoints(n),
					RowGroup.GetRevaluePercent(n));

				m_arrayCommands.Add(strCommand);
			}
		}
	}

	for (int n = 0; n < arrayColumnsToCheck.GetSize(); n++)
	{
		m_arrayCommands.Add(GetDropColumnIfExistsCommand(SQLTableNames::AccountGroups, arrayColumnsToCheck.GetAt(n)));
	}
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV17()
{
	CSQLBuilderCreateTable TableCreator{};
	m_arrayCommands.Add(TableCreator.GetCommandCreateSchemeTable());

	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::Accounts, Account::SchemeNo.Label, "INT", TRUE, "0");
}

/**********************************************************************/

void CSQLTranMigrateSchema::MigrateV18()
{
	AddAlterTableAddColumnCommand(m_arrayCommands, SQLTableNames::DeptInfo, DeptInfo::GeneralSpendBonusFlag.Label, "BIT", TRUE, "1");
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

CString CSQLTranMigrateSchema::GetDropColumnIfExistsCommand(CString strTableName, CString strColumn)
{
	CString strCommand = "";
	strCommand.Format("ALTER TABLE %s DROP COLUMN IF EXISTS %s",
		(const char*)strTableName,
		(const char*)strColumn);
	return strCommand;
}

/**********************************************************************/
