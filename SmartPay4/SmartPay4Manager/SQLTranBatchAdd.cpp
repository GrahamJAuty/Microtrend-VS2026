/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/
#include "SQLTranBatchAdd.h"
/**********************************************************************/

void CSQLTranBatchAdd::DoWork(__int64 nFirstNewUserID, __int64 nLastNewUserID, __int64 nRecordsToAdd,  CArray<__int64, __int64>& arrayInUse, CString strComment)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_nUserIDToSelect = nFirstNewUserID;

		__int64 nUserIDToTry = nFirstNewUserID;
		__int64 nAdded = 0;

		CWorkingDlg dlgWorking("Batch Add Accounts");
		dlgWorking.Create();
		dlgWorking.SetCaption1("Creating Accounts");

		while (nUserIDToTry <= nLastNewUserID)
		{
			bool bAccountExists = FALSE;
			if (arrayInUse.GetSize() != 0)
			{
				if (nUserIDToTry == arrayInUse.GetAt(0))
				{
					arrayInUse.RemoveAt(0);
					bAccountExists = TRUE;
				}
			}

			if (FALSE == bAccountExists)
			{
				CSQLRowAccountFull NewRow;
				NewRow.SetUserID(nUserIDToTry);

				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.InsertRow(NewRow, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}

				m_atc.Set(&NewRow);
				m_atc.SetApplicationNo(APPNO_INSERT);
				m_atc.SetLineComment(strComment);
				m_atc.BufferNewLine();

				m_nUserIDToSelect = nUserIDToTry;

				if (dlgWorking.SetCaption2RecordsOfTotal(++nAdded, nRecordsToAdd) == TRUE)
				{
					break;
				}
			}

			nUserIDToTry++;
		}

		dlgWorking.DestroyWindow();

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
