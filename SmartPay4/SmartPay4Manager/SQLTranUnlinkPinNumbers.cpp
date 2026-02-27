/**********************************************************************/
#include "..\SmartPay4Shared\GlobalFunctions.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRecordSetPINNumber.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
/**********************************************************************/
#include "SQLTranUnlinkPINNumbers.h"
/**********************************************************************/

void CSQLTranUnlinkPINNumbers::DoWork(int nGroupNo, __int64 nFrom, __int64 nTo)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = 2;

		m_nRemoved = 0;

		CSQLBuilderBind BuilderBind;
		CSQLWhereBuilder WhereBuilder;

		if (nGroupNo != 0)
		{
			WhereBuilder.AppendConditionInt(BuilderBind, Account::GroupNo.Label, szEQUAL, nGroupNo);
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
		dlgWorking.SetCaption1("Removing PIN Numbers from Accounts");

		for (int n = 0; n < arrayUserIDs.GetCount(); n++)
		{
			dlgWorking.SetCaption2RecordsOfTotal(n, arrayUserIDs.GetCount(), FALSE);

			CSQLRepositoryPINNumber repoPIN;

			CSQLRowPINNumber RowPIN;
			RowPIN.SetUserID(FormatInt64Value(arrayUserIDs.GetAt(n)));

			if (repoPIN.SelectRowByUserID(RowPIN, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				continue;
			}

			if (repoPIN.RemoveAccountIDLink(arrayUserIDs.GetAt(n), m_pDatabase) == FALSE)
			{
				bErrorReturn = TRUE;
				break;
			}
			else
			{
				m_nRemoved++;
			}
		}

		dlgWorking.DestroyWindow();

		if (FALSE == bErrorReturn)
		{
			m_bCanCommit = TRUE;
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
