/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRecordSetPINNumber.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
/**********************************************************************/
#include "SQLTranLinkPINNumbers.h"
/**********************************************************************/

void CSQLTranLinkPINNumbers::DoWork(bool bKeep, int nGroup, __int64 nFrom, __int64 nTo)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = 2;

		m_nAdded = 0;
		m_nRetained = 0;

		CSQLBuilderBind BuilderBind;
		CSQLWhereBuilder WhereBuilder;

		if (nGroup != 0)
		{
			WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupNo.Label, szEQUAL, nGroup);
			WhereBuilder.AppendAnd();
		}

		WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szGREATEROREQUAL, nFrom);
		WhereBuilder.AppendAnd();
		WhereBuilder.AppendConditionInt64(BuilderBind, Account::UserID.Label, szLESSTHANOREQUAL, nTo);

		CArray<__int64, __int64> arrayUserIDs;

		CWorkingDlg dlgWorking("Finding Accounts to Check", FALSE);
		dlgWorking.Create(NULL);

		{
			CSQLRecordSetAccountUserID RecordSet(m_pDatabase,
				RSParams_AccountUserId_NormalByParams{ &BuilderBind, WhereBuilder.GetWhereString() });

			int nChecked = 0;

			__int64 nUserID = 0;
			while (RecordSet.StepSelect(nUserID) == TRUE)
			{
				dlgWorking.SetCaption2RecordsChecked(++nChecked, FALSE);
				arrayUserIDs.Add(nUserID);
			}
		}

		bool bErrorReturn = FALSE;
		dlgWorking.SetCaption1("Assigning PIN Numbers to Accounts");

		for (int n = 0; n < arrayUserIDs.GetCount(); n++)
		{
			dlgWorking.SetCaption2RecordsOfTotal(n, arrayUserIDs.GetCount(), FALSE);

			__int64 nUserID = arrayUserIDs.GetAt(n);

			bool bAssignNewPIN = TRUE;

			if (TRUE == bKeep)
			{
				CSQLRowPINNumber RowPIN;
				RowPIN.SetUserID(FormatInt64Value(nUserID));

				CSQLRepositoryPINNumber repoPIN;
				if (repoPIN.SelectRowByUserID(RowPIN, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					bAssignNewPIN = FALSE;
					m_nRetained++;
				}
			}

			if (TRUE == bAssignNewPIN)
			{
				CSQLRepositoryPINNumber repoPIN;
				if (repoPIN.LinkAccountIDToPINNumber(0, nUserID, m_pDatabase) == FALSE)
				{
					bErrorReturn = TRUE;
					break;
				}
				else
				{
					m_nAdded++;
				}
			}
		}

		dlgWorking.DestroyWindow();

		if (FALSE == bErrorReturn)
		{
			m_bCanCommit = TRUE;
		}
	}
}

/**********************************************************************/
