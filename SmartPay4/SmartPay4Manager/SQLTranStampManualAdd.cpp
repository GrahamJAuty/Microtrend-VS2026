/**********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
/**********************************************************************/
#include "SQLTranStampManualAdd.h"
/**********************************************************************/

void CSQLTranStampManualAdd::DoWork( CSQLRowAccountFull& RowAccount, CSQLRowPurchaseControlStamp& PurchaseControlStampNew, CString strRuleName)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		int nNewStamps = PurchaseControlStampNew.GetStampCount();
		
		CSQLRepositoryPurchaseControlStamp RepoStamp;
		if (RepoStamp.SelectRow(PurchaseControlStampNew, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			PurchaseControlStampNew.SetStampCount(PurchaseControlStampNew.GetStampCount() + nNewStamps);

			if (RepoStamp.UpdateRow(PurchaseControlStampNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
			else
			{
				SQLPurchaseControlBuffer.BufferPCAction(
					APPNO_SO_MADD,
					RowAccount,
					PurchaseControlStampNew,
					strRuleName,
					nNewStamps,
					PurchaseControlStampNew.GetStampCount()
				);
			}
		}
		else
		{
			if (RepoStamp.InsertRow(PurchaseControlStampNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
			else
			{
				SQLPurchaseControlBuffer.BufferPCAction(
					APPNO_SO_MADD,
					RowAccount,
					PurchaseControlStampNew,
					strRuleName,
					nNewStamps,
					nNewStamps
				);
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

void CSQLTranStampManualAdd::AfterTransaction()
{
	SQLPurchaseControlBuffer.WritePCAuditLines(m_pDatabase);
}

/**********************************************************************/
