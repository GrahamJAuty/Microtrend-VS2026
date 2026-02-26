/**********************************************************************/
#include "..\SPosLoyalty4Shared\LoyaltyAuditAppend.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "AuditRecord.h"
#include "WorkingDlg.h"
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

				CSQLRepositoryAccount repoAccount;
				if (repoAccount.InsertRow(NewRow, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}

				CAuditRecord atc;
				atc.SetSQL(NewRow);
				atc.SetApplicationNo(APPNO_INSERT);
				atc.SetComment(strComment);
				atc.SaveLineToArray(m_arrayAuditBuffer);

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
