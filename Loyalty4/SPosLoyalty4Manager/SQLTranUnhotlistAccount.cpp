/**********************************************************************/
#include "..\SposLoyalty4Shared\LoyaltyAuditAppend.h"
#include "..\SposLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "PasswordData.h"
/**********************************************************************/
#include "SQLTranUnhotlistAccount.h"
/**********************************************************************/

void CSQLTranUnhotlistAccount::DoWork(__int64 nUserID, CString strComment)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(nUserID);

		CSQLRepositoryAccount repoAccount;

		if (repoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		RowAccount.SetHotlistCard(0);

		CAuditRecord atc;
		atc.SetSQL(RowAccount);				// audit hotlisted details
		atc.SetApplicationNo(APPNO_UNHOTLIST);
		atc.SetComment(strComment);
		atc.SaveLineToArray(m_arrayAuditBuffer);

		bool bHavePrevUserID = RowAccount.HavePreviousUserID();
		bool bHaveNextUserID = RowAccount.HaveNextUserID();
		__int64 nPrevUserID = RowAccount.GetPreviousUserID();
		__int64 nNextUserID = RowAccount.GetNextUserID();

		RowAccount.SetNextUserID(0);
		RowAccount.SetPreviousUserID(0);

		if (repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if (TRUE == bHavePrevUserID)
		{
			CSQLRowAccountFull RowPrev;
			RowPrev.SetUserID(nPrevUserID);

			switch (repoAccount.SelectRow(RowPrev, m_pDatabase).GetSQLError())
			{
			case SQLCRUD_ERR_NOMATCH:
				break;

			case SQLCRUD_ERR_NONE:
				RowPrev.SetNextUserID(nNextUserID);
				if (repoAccount.UpdateRow(RowPrev, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}
				break;

			default:
				break;
			}
		}

		if (TRUE == bHaveNextUserID)
		{
			CSQLRowAccountFull RowNext;
			RowNext.SetUserID(nNextUserID);

			switch (repoAccount.SelectRow(RowNext, m_pDatabase).GetSQLError())
			{
			case SQLCRUD_ERR_NOMATCH:
				break;

			case SQLCRUD_ERR_NONE:
				RowNext.SetPreviousUserID(nPrevUserID);
				if (repoAccount.UpdateRow(RowNext, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}
				break;

			default:
				break;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
