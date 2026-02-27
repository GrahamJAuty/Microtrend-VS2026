/**********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
/**********************************************************************/
#include "SQLTranStampManualEdit.h"
/**********************************************************************/

CSQLTranStampManualEdit::CSQLTranStampManualEdit(int nAppNo) : CSQLTranBase()
{
	m_nAppNo = nAppNo;
}

/**********************************************************************/

void CSQLTranStampManualEdit::DoWork(CSQLRowAccountFull& RowAccount, CSQLRowPurchaseControlStamp& PurchaseControlStampOld, CSQLRowPurchaseControlStamp& PurchaseControlStampNew, CString strRuleName)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CSQLRepositoryPurchaseControlStamp Repo;
		if (Repo.DeleteRow(PurchaseControlStampOld, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}
		else
		{
			SQLPurchaseControlBuffer.BufferPCAction(
				m_nAppNo,
				RowAccount,
				PurchaseControlStampOld,
				strRuleName,
				PurchaseControlStampOld.GetStampCount() * (-1),
				0
			);
		}

		int nNewStamps = PurchaseControlStampNew.GetStampCount();

		if (Repo.SelectRow(PurchaseControlStampNew, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			PurchaseControlStampNew.SetStampCount(PurchaseControlStampNew.GetStampCount() + nNewStamps);

			if (Repo.UpdateRow(PurchaseControlStampNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
			else
			{
				SQLPurchaseControlBuffer.BufferPCAction(
					m_nAppNo,
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
			if (Repo.InsertRow(PurchaseControlStampNew, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
			else
			{
				SQLPurchaseControlBuffer.BufferPCAction(
					m_nAppNo,
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

void CSQLTranStampManualEdit::AfterTransaction()
{
	SQLPurchaseControlBuffer.WritePCAuditLines(m_pDatabase);
}

/**********************************************************************/
