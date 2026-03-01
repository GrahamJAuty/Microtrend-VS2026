/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
/**********************************************************************/
#include "AuditRecord.h"
/**********************************************************************/
#include "SQLTranRetryStaffGift.h"
/**********************************************************************/

void CSQLTranRetryStaffGift::DoWork(CSQLRowStaffGiftException& RowSG, CSQLRowAccountFull& RowAccount)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		RowAccount.AddToPurse1(RowSG.GetValue());

		CSQLRepositoryAccount repoAccount;
		if (repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			m_strProcessError = "(Unable to update staff account)";
			return;
		}

		CSQLRepositoryStaffGiftException repoGift;
		if (repoGift.DeleteRow(RowSG, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			m_strProcessError = "(Unable to delete staff gift exception)";
			return;
		}

		CAuditRecord atc;
		atc.SetSQL(RowAccount);
		atc.SetSourceType(AUDIT_PC);
		atc.SetApplicationNo(APPNO_ADDGIFT);
		atc.SetComment("");
		atc.SetPurse1Transaction(RowSG.GetValue());
		atc.SaveLineToArray(m_arrayAuditBuffer);

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
