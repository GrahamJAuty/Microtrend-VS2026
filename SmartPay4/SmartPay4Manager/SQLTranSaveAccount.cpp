/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLTable_ExternalAccount\SQLRepositoryExternalAccount.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
/**********************************************************************/
#include "SQLTranSaveAccount.h"
/**********************************************************************/

void CSQLTranSaveAccount::DoWork(CSQLRowAccountFull& RowAccount, CSQLRowExternalAccount& RowExternalOld, CSQLRowExternalAccount& RowExternalNew, CString strNewPINNumber)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if ((RowExternalOld.GetAESUsername() != RowExternalNew.GetAESUsername()) || (RowExternalOld.GetPasswordHash() != RowExternalNew.GetPasswordHash()))
		{
			CSQLRepositoryExternalAccount RepoExt;
			switch (RepoExt.DeleteRowByUserID(RowExternalOld, m_pDatabase).GetSQLError())
			{
			case SQLCRUD_ERR_NONE:
			case SQLCRUD_ERR_NOMATCH:
				break;

			default:
				return;
			}

			if (RepoExt.UpsertRow(RowExternalNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		if (CSQLRowPINNumber::IsValidPINNumber(strNewPINNumber) == TRUE)
		{
			CSQLRepositoryPINNumber RepoPIN;
			if (RepoPIN.LinkAccountIDToPINNumber(strNewPINNumber, RowAccount.GetUserID(), m_pDatabase) == false)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
