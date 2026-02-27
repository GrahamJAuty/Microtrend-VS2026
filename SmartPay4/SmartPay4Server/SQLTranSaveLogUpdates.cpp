/**********************************************************************/
#include "..\SmartPay4Shared\SQLResultInfo.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_ChartwellExportPayment\SQLRepositoryChartwellExportPayment.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRepositoryPebbleExportPayment.h"
/**********************************************************************/
#include "EposSmartPayFile.h"
#include "SQLTranSaveLogUpdates.h"
/**********************************************************************/

void CSQLTranSaveLogUpdates::DoWork(CSQLRowAccountFull& RowAccount, CPendingExportPaymentData& PendingExportPaymentData, CPendingEposTerminalData& PendingEposTerminalData)
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		//MAKE SURE CUSTOMER ACCOUNT HAS NOT BEEN CHANGED ELSEWHERE 

		CSQLRowAccountFull RowCheck;
		RowCheck.SetUserID(RowAccount.GetUserID());

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowCheck, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowAccount.CompareRowVersion(RowCheck) == FALSE)
			{
				return;
			}
		}

		//UPDATE CUSTOMER ACCOUNT

		if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return;
		}

		//CHECK PURCHASE CONTROL STAMP AWARDS

		for (int n = 0; n < SQLPurchaseControlBuffer.GetPurchaseControlStampAwardCount(); n++)
		{
			if (SQLPurchaseControlBuffer.ApplyPurchaseControlStampAward(n, m_pDatabase) == FALSE)
			{
				return;
			}
		}

		CString strPaymentTime = GetSimpleDateTimeNowString();

		//ADD PAYMENT INFORMATION FOR PEBBLE EXPORT
		if (TRUE == PendingExportPaymentData.m_bPebble)
		{
			double dPaymentAmount = 0.0;
			for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
			{
				if (PendingExportPaymentData.m_PaymentDataPebble.GetPaymentType(n) == -1)
				{
					break;
				}

				dPaymentAmount += PendingExportPaymentData.m_PaymentDataPebble.GetPaymentAmount(n);
			}

			//if (::CompareDoubles(dPaymentAmount, 0.0, 2) >= 0)
			{
				PendingExportPaymentData.m_PaymentDataPebble.SetCreateDateTime(strPaymentTime);

				CSQLRepositoryPebbleExportPayment PrepStatPayment;

				if (PrepStatPayment.RowExists(
					PendingExportPaymentData.m_PaymentDataPebble.GetUserID(),
					PendingExportPaymentData.m_PaymentDataPebble.GetDate(),
					PendingExportPaymentData.m_PaymentDataPebble.GetTime(),
					PendingExportPaymentData.m_PaymentDataPebble.GetCCNo(),
					m_pDatabase).GetSQLResult() == 0)
				{

					if (PrepStatPayment.InsertRow(PendingExportPaymentData.m_PaymentDataPebble, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						return;
					}
				}
			}
		}
		
		//ADD PAYMENT INFORMATION FOR CHARTWELL EXPORT
		if (TRUE == PendingExportPaymentData.m_bChartwell)
		{
			PendingExportPaymentData.m_PaymentDataChartwell.SetCreateDateTime(strPaymentTime);

			CSQLRepositoryChartwellExportPayment PrepStatPayment;

			if (PrepStatPayment.RowExists(
				PendingExportPaymentData.m_PaymentDataChartwell.GetUserID(),
				PendingExportPaymentData.m_PaymentDataChartwell.GetTransactionID(),
				PendingExportPaymentData.m_PaymentDataChartwell.GetTerminalNo(),
				PendingExportPaymentData.m_PaymentDataChartwell.GetDate(),
				m_pDatabase).GetSQLResult() == 0)
			{

				if (PrepStatPayment.InsertRow(PendingExportPaymentData.m_PaymentDataChartwell, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					return;
				}
			}
		}

		//UPDATE EPOS TERMINAL
		if (TRUE == PendingEposTerminalData.m_bGotChange)
		{
			CSQLRepositoryEposTerminal RepoTerm;
			if (RepoTerm.UpsertRow(PendingEposTerminalData.m_RowEposTerminal, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				return;
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/
