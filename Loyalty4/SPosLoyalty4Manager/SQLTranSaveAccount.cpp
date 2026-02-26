/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
/**********************************************************************/
#include "SQLTranSaveAccount.h"
/**********************************************************************/

void CSQLTranSaveAccount::DoWork(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternalOld, CSQLRowExternalAccount& RowExternalNew)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;
		
		CSQLRepositoryAccount repoAccount;
		if (repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if ((RowExternalOld.GetAESUsername() != RowExternalNew.GetAESUsername()) || (RowExternalOld.GetPasswordHash() != RowExternalNew.GetPasswordHash()))
		{
			CSQLRepositoryExternalAccount repoExt;
			switch (repoExt.DeleteRowByUserID(RowExternalOld, m_pDatabase).GetSQLError())
			{
			case SQLCRUD_ERR_NONE:
			case SQLCRUD_ERR_NOMATCH:
				break;

			default:
				return;
			}

			if (repoExt.UpsertRow(RowExternalNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
