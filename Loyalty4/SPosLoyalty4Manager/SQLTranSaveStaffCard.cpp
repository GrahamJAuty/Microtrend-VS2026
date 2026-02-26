/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffCard\SQLRepositoryStaffCard.h"
/**********************************************************************/
#include "SQLTranSaveStaffCard.h"
/**********************************************************************/

void CSQLTranSaveStaffCard::DoWork(CSQLRowSetStaffCard& RowSet)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		if (CSQLHelpers::ExecuteSQL(m_pDatabase,"DELETE FROM StaffCards") != SQLCRUD_ERR_NONE)
		{
			return;
		}

		for (int n = 0; n < RowSet.GetSize(); n++)
		{
			CSQLRowStaffCard RowSC;
			RowSet.GetRow(n, RowSC);

			CSQLRepositoryStaffCard repoStaff;
			if (repoStaff.InsertRow(RowSC, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
