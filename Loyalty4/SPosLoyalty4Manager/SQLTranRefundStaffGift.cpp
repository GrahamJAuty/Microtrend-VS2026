/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLResultInfo.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_StaffGiftException\SQLRepositoryStaffGiftException.h"
/**********************************************************************/
#include "AuditRecord.h"
/**********************************************************************/
#include "SQLTranRefundStaffGift.h"
/**********************************************************************/

void CSQLTranRefundStaffGift::DoWork(CSQLRowStaffGiftException& RowSG)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(RowSG.GetCustomerID());

		CSQLRepositoryAccount repoAccount;
		if (repoAccount.SelectRow(RowAccount,m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			m_strProcessError = "(Unable to access customer account)";
			return;
		}

		RowAccount.AddToPurse1(RowSG.GetValue());

		if (repoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			m_strProcessError = "(Unable to update customer account)";
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
		atc.SetApplicationNo(APPNO_ADDGIFTX);
		atc.SetComment("");
		atc.SetPurse1Transaction(RowSG.GetValue());
		atc.SaveLineToArray(m_arrayAuditBuffer);

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
