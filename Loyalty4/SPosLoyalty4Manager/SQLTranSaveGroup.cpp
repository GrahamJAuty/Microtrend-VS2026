/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
/**********************************************************************/
#include "SQLTranSaveGroup.h"
/**********************************************************************/

void CSQLTranSaveGroup::DoWork(CSQLRowSetGroup& RowSetGroup, CSQLRowSetGroupTopupBonus& RowSetGroupTopupBonus)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CString strCommand = "";
		strCommand.Format("DELETE FROM %s",
			(const char*)SQLTableNames::AccountGroups);

		if (CSQLHelpers::ExecuteSQL(m_pDatabase, strCommand) != SQLCRUD_ERR_NONE)
		{
			return;
		}

		for (int n = 0; n < RowSetGroup.GetSize(); n++)
		{
			CSQLRowGroup RowGroup;
			RowSetGroup.GetRow(n, RowGroup);

			CSQLRepositoryGroup repoGroup;
			if (repoGroup.InsertRow(RowGroup, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		strCommand.Format( "DELETE FROM %s",
			(const char*)SQLTableNames::GroupTopupBonuses);

		if (CSQLHelpers::ExecuteSQL(m_pDatabase, strCommand) != SQLCRUD_ERR_NONE)
		{
			return;
		}

		for (int n = 0; n < RowSetGroupTopupBonus.GetSize(); n++)
		{
			CSQLRowGroupTopupBonus RowGroup;
			RowSetGroupTopupBonus.GetRow(n, RowGroup);

			CSQLRepositoryGroupTopupBonus repoBonus;
			if (repoBonus.InsertRow(RowGroup, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
