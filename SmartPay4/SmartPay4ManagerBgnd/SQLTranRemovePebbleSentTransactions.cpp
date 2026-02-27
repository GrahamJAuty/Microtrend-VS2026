/**********************************************************************/
#include "..\SmartPay4Shared\SQLHelpers.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRepositoryPebbleExportPayment.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRepositoryPebbleExportTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRecordSetPebbleExportTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransactionSent\SQLRecordSetPebbleExportTransactionSent.h"
/**********************************************************************/
#include "SQLTranRemovePebbleSentTransactions.h"
/**********************************************************************/

CSQLTranRemovePebbleSentTransactions::CSQLTranRemovePebbleSentTransactions() : CSQLTranBase()
{
	m_strError = "";
}

/**********************************************************************/

void CSQLTranRemovePebbleSentTransactions::DoWork()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		CReportConsolidationArray<CSortedInt64Item> arraySent;

		{
			CSQLRecordSetPebbleExportTransactionSent RecordSet(m_pDatabase, RSParams_PebbleTransactionSent_NormalNoParams());
			CSQLRowPebbleExportTransactionSent RowTran;
	
			while (RecordSet.StepSelectAll(RowTran) == TRUE)
			{
				CSortedInt64Item item;
				item.m_nItem = RowTran.GetTranLineID();
				arraySent.Consolidate(item);
			}
		}

		CReportConsolidationArray<CSortedInt64Item> arrayToDelete;

		{
			CSQLRecordSetPebbleExportTransaction RecordSet(m_pDatabase, RSParams_PebbleExportTransaction_NormalNoParams());
			CSQLRowPebbleExportTransaction RowTran;

			while (RecordSet.StepSelectAll(RowTran) == TRUE)
			{
				CSortedInt64Item item;
				item.m_nItem = RowTran.GetLineID();

				int nPos = 0;
				if (arraySent.Find(item, nPos) == TRUE)
				{
					arrayToDelete.Consolidate(item);
				}
			}
		}

		for (int n = 0; n < arrayToDelete.GetSize(); n++)
		{
			CSortedInt64Item item;
			arrayToDelete.GetAt(n, item);

			CSQLRowPebbleExportTransaction RowTran;
			RowTran.SetLineID(item.m_nItem);

			CSQLRepositoryPebbleExportTransaction PrepStatTran;
			if (PrepStatTran.SelectRow(RowTran, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}

			if (PrepStatTran.DeleteRow(RowTran, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}

			CSQLRowPebbleExportPayment RowPayment;
			RowPayment.SetUserID(RowTran.GetUserID());
			RowPayment.SetDate(RowTran.GetDate());
			RowPayment.SetTime(RowTran.GetTime());
			RowPayment.SetCCNo(RowTran.GetCCNo());

			CSQLRepositoryPebbleExportPayment PrepStatPayment;
			if (PrepStatPayment.RowExists(RowPayment.GetUserID(), RowPayment.GetDate(), RowPayment.GetTime(), RowPayment.GetCCNo(), m_pDatabase).GetSQLResult() != 0)
			{
				if (PrepStatPayment.DeleteRow(RowPayment, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}
			}
		}

		CString strSQLCommand = "";
		strSQLCommand.Format("DELETE FROM %s",
			SQLTableNames::PebbleExportTransactionsSent);

		if (CSQLHelpers::ExecuteSQL(m_pDatabase, strSQLCommand) != SQLCRUD_ERR_NONE)
		{
			return;
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

void CSQLTranRemovePebbleSentTransactions::AfterTransaction()
{
}

//******************************************************************************
