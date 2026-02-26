/**********************************************************************/
#include "..\..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\..\SPosLoyalty4Shared\SQLHelpers.h"
/**********************************************************************/
#include "SQLBuilderCreateTable.h"
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
		arrayCommands.Add(TableCreator.GetCommandCreateGroupTopupBonusTable());
		arrayCommands.Add(TableCreator.GetCommandCreateExternalAccountTable());
		arrayCommands.Add(TableCreator.GetCommandCreatePluInfoTable());
		arrayCommands.Add(TableCreator.GetCommandCreateDeptInfoTable());
		arrayCommands.Add(TableCreator.GetCommandCreateStaffCardTable());
		arrayCommands.Add(TableCreator.GetCommandCreateStaffGiftExceptionTable());
		arrayCommands.Add(TableCreator.GetCommandCreateCCNoTable());
		arrayCommands.Add(TableCreator.GetCommandCreateAuditPeriodTable());
		arrayCommands.Add(TableCreator.GetCommandCreateOfferStampTable());
		arrayCommands.Add(TableCreator.GetCommandCreateStampOfferInfoTable());
		arrayCommands.Add(TableCreator.GetCommandCreateEcrmanDatabaseTable());
		arrayCommands.Add(TableCreator.GetCommandCreateSchemeTable());

		CString strVersion = "";
		strVersion.Format("INSERT INTO DbVersion(LineId, Version) VALUES(1, %d)", LOYALTY_DBVERSION);
		arrayCommands.Add(strVersion);

		for (int n = 0; n < arrayCommands.GetSize(); n++)
		{
			if ( CSQLHelpers::ExecuteSQL(m_pDatabase,arrayCommands.GetAt(n)) != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
