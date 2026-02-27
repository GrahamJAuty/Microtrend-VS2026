/**********************************************************************/
#include "..\SmartPay4Shared\PasswordData.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
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

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		int nHotlistAuditCode = RowAccount.GetAlertCode();

		RowAccount.SetUserAlert(0);

		m_atc.Set(&RowAccount);
		m_atc.SetApplicationNo(APPNO_UNHOTLIST);
		m_atc.SetOperatorID(Passwords.GetPasswordInUse());
		m_atc.SetAuditLineExtraText(nHotlistAuditCode);
		m_atc.SetLineComment(strComment);
		m_atc.BufferNewLine();

		bool bHavePrevUserID = RowAccount.HavePreviousUserID();
		bool bHaveNextUserID = RowAccount.HaveNextUserID();
		__int64 nPrevUserID = RowAccount.GetPreviousUserID();
		__int64 nNextUserID = RowAccount.GetNextUserID();

		RowAccount.SetNextUserID(0);
		RowAccount.SetPreviousUserID(0);

		if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		if (TRUE == bHavePrevUserID)
		{
			CSQLRowAccountFull RowPrev;
			RowPrev.SetUserID(nPrevUserID);

			switch (RepoAccount.SelectRow(RowPrev, m_pDatabase).GetSQLError())
			{
			case SQLCRUD_ERR_NOMATCH:
				break;

			case SQLCRUD_ERR_NONE:
				RowPrev.SetNextUserID(nNextUserID);
				if (RepoAccount.UpdateRow(RowPrev, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
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

			switch (RepoAccount.SelectRow(RowNext, m_pDatabase).GetSQLError())
			{
			case SQLCRUD_ERR_NOMATCH:
				break;

			case SQLCRUD_ERR_NONE:
				RowNext.SetPreviousUserID(nPrevUserID);
				if (RepoAccount.UpdateRow(RowNext, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
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
