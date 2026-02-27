/**********************************************************************/
#include "SQLSchema/SQLNamespaces.h"
/**********************************************************************/
#include "SQLTable_Account/SQLRepositoryAccount.h"
#include "SQLTable_Account/SQLRecordSetAccountUserID.h"
/**********************************************************************/
#include "GroupShiftValidator.h"
#include "SQLHelpers.h"
#include "SQLWhereBuilder.h"
/**********************************************************************/
#include "SQLTranProcessGroupShifts.h"
/**********************************************************************/

void CSQLTranProcessGroupShifts::DoWork(COleDateTime& oleDateToday)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = 2;

		/*
		CSQLBuilderBind BuilderBind;
		CSQLWhereBuilder WhereBuilder;
		WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupShiftStatus.Label, szNOTEQUAL, 0);
		
		CSQLRecordSetAccountUserID RecordSet(SQLTableNames::Accounts, m_pDatabase, ACCOUNTSET_SEARCH, BuilderBind.GetFieldCount());
		RecordSet.m_strFilter = WhereBuilder.GetWhereString();
		RecordSet.SetSearchBindings(BuilderBind);

		__int64 nUserID;
		while (RecordSet.StepSelect(nUserID) == TRUE)
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(nUserID);
			if (SQLRepositoryAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}

			CGroupShiftValidator GroupShiftValidator(oleDateToday, RowAccount);

			if ( GroupShiftValidator.UpdateAccount(RowAccount) == TRUE )
			{ 
				if (SQLRepositoryAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}
			}
		}
		*/
		m_bCanCommit = TRUE;
		
	}
}

/**********************************************************************/
