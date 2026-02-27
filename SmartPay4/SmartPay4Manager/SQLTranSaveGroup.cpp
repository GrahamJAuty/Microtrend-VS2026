/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
/**********************************************************************/
#include "SQLTranSaveGroup.h"
/**********************************************************************/

void CSQLTranSaveGroup::DoWork(CSQLRowSetGroupFull& RowSet)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CString strCommand = "";
		strCommand.Format("DELETE FROM %s", SQLTableNames::AccountGroups);

		if (CSQLHelpers::ExecuteSQL(m_pDatabase, strCommand) != SQLCRUD_ERR_NONE)
		{
			return;
		}

		for (int n = 0; n < RowSet.GetSize(); n++)
		{
			CSQLRowGroupFull RowGroup;
			RowSet.GetRow(n, RowGroup);

			CSQLRepositoryGroup RepoGroup;
			if ( RepoGroup.InsertRow(RowGroup,m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
