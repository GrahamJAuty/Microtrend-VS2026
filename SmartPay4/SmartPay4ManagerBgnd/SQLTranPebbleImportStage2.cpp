/**********************************************************************/
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRepositoryPebblePendingTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRecordSetPebblePendingTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleRecentTransaction\SQLRepositoryPebbleRecentTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleRecentTransaction\SQLRecordSetPebbleRecentTransaction.h"
/**********************************************************************/
#include "WebPaymentDefines.h"
/**********************************************************************/
#include "WebPaymentLogRecord.h"
/**********************************************************************/
#include <memory>
/**********************************************************************/
#include "SQLTranPebbleImportStage2.h"
/**********************************************************************/

CSQLTranPebbleImportStage2::CSQLTranPebbleImportStage2(CWorkingDlg& WorkingDlg, bool bNewPayments, int& nValidCount, CWnd* pParent) : CSQLTranBase(),
	m_WorkingDlg(WorkingDlg),
	m_nValidCount(nValidCount)
{
	m_pParent = pParent;
	m_strPebbleError = "";
	m_nExceptionCount = 0;
	m_nNewExceptionCount = 0;
	m_nValidCount = 0;
	m_bNewPayments = bNewPayments;

	CWebPaymentLogRecord LogRecord;

	// "InYYYYMM"
	m_strLogFileName = LogRecord.GetLogFileName();
	m_strTempLogFile.Format("%s.$$$",
		(const char*)m_strLogFileName);

	m_strTempLogFile = System.GetWebPaymentPath(m_strTempLogFile);

	// "ExYYYYMM"
	m_strExceptionName = LogRecord.GetExceptionFileName();
	m_strTempExceptionFile.Format("%s.$$$",
		(const char*)m_strExceptionName);
	m_strTempExceptionFile = System.GetWebPaymentPath(m_strTempExceptionFile);

	// "NewExYYYYMM"
	m_strNewExceptionName = LogRecord.GetNewExceptionFileName();
	m_strNewTempExceptionFile.Format("%s.$$$",
		(const char*)m_strNewExceptionName);
	m_strNewTempExceptionFile = System.GetWebPaymentPath(m_strNewTempExceptionFile);

	// web payment exception indicator file
	m_strExceptionIndicator = Filenames.GetWebPaymentExceptionIndicator();
}

/**********************************************************************/

void CSQLTranPebbleImportStage2::DoWork()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		CWebPaymentLogRecord LogRecord;

		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		m_WorkingDlg.SetCaption1("Buffering Pending Pebble Transactions");
		CArray<CSQLRowPebblePendingTransaction, CSQLRowPebblePendingTransaction> arrayPendingTran;

		{
			CSQLRecordSetPebblePendingTransaction RecordSetPendingTran(m_pDatabase,
				RSParams_PebblePendingTransaction_NormalNoParams(),
				FALSE == m_bNewPayments);

			CSQLRowPebblePendingTransaction RowPendingTransaction;

			int nBufferedSoFar = 0;

			while (RecordSetPendingTran.StepSelectAll(RowPendingTransaction) == TRUE)
			{
				arrayPendingTran.Add(RowPendingTransaction);
				nBufferedSoFar++;

				CString strCaption = "";
				strCaption.Format("Pending Transactions Buffered %d",
					nBufferedSoFar);

				m_WorkingDlg.SetCaption2(strCaption);
			}
		}

		m_WorkingDlg.SetCaption1("Processing Pebble Transactions");
		CArray<int, int> arrayExceptionTranIdx;
		CArray<int, int> arrayReconciledTranIdx;

		for (int nPendingTranIdx = 0; nPendingTranIdx < arrayPendingTran.GetSize(); nPendingTranIdx++)
		{
			m_WorkingDlg.SetCaption2RecordsOfTotal(nPendingTranIdx, arrayPendingTran.GetSize(), FALSE);

			CSQLRowPebblePendingTransaction RowPendingTransaction = arrayPendingTran.GetAt(nPendingTranIdx);

			double dTranVal = (double(RowPendingTransaction.GetAmount())) / 100.0;

			CString strLogDateTime = "";

			{
				CString strDateTime = RowPendingTransaction.GetDateTime();
				if (strDateTime.GetLength() == 14)
				{
					strLogDateTime.Format("%s/%s/%s @ %s:%s:%s",
						(const char*)strDateTime.Mid(6, 2),
						(const char*)strDateTime.Mid(4, 2),
						(const char*)strDateTime.Left(4),
						(const char*)strDateTime.Mid(8, 2),
						(const char*)strDateTime.Mid(10, 2),
						(const char*)strDateTime.Right(2));
				}
			}

			LogRecord.SetPaymentID("0");
			LogRecord.SetTransactionRef(RowPendingTransaction.GetTransactionID());
			LogRecord.SetPupilID(RowPendingTransaction.GetMemberID());
			LogRecord.SetPupilName(RowPendingTransaction.GetName());
			LogRecord.SetPaidAmount(dTranVal);
			LogRecord.SetPaidDate(strLogDateTime);
			LogRecord.SetNotes(RowPendingTransaction.GetIsSaleFlag() ? "Sale" : "Top-up");

			bool bException = FALSE;

			CSQLRepositoryAccount RepoAccount;
			__int64 nUserID = RepoAccount.LookupMemberID(RowPendingTransaction.GetMemberID(), m_pDatabase).GetSQLResult();

			if (nUserID != 0)
			{
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(nUserID);

				if (RepoAccount.SelectRow(RowAccount, m_pDatabase).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					int nAppNo = APPNO_PEBBLE_TOPUP;
					if (RowPendingTransaction.GetIsSaleFlag() == TRUE)
					{
						if (dTranVal < 0.0)
						{
							nAppNo = APPNO_PEBBLE_ORDER;
						}
						else
						{
							nAppNo = APPNO_PEBBLE_ORDER_REFUND;
						}
					}
					else
					{
						if (dTranVal < 0.0)
						{
							nAppNo = APPNO_PEBBLE_TOPUP_REFUND;
						}
						else
						{
							nAppNo = APPNO_PEBBLE_TOPUP;
						}
					}

					m_atc.Set(&RowAccount);
					m_atc.SetOperatorID(Passwords.GetPasswordInUse());
					m_atc.SetApplicationNo(nAppNo);
					m_atc.SetLineComment(m_atc.GetLineOrDefaultComment());

					RowAccount.AddToPurse1Liability(dTranVal);
					if (RepoAccount.UpdateRow(RowAccount, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						m_strPebbleError = "Unable to update SmartPay account";
						return;
					}

					if (m_atc.AuditChanges(&RowAccount) == TRUE)
					{
						m_atc.BufferNewLine();
					}

					LogRecord.SetPupilName(RowAccount.GetAccountDisplayName());
					LogRecord.LogValidImport(m_strTempLogFile, RowAccount.GetUserIDString(), m_nValidCount);

					arrayReconciledTranIdx.Add(nPendingTranIdx);
				}
				else
				{
					bException = TRUE;
					LogException(&LogRecord, szMSG_NOACCOUNT);
					arrayExceptionTranIdx.Add(nPendingTranIdx);
				}
			}
			else
			{
				bException = TRUE;
				LogException(&LogRecord, szMSG_NOMEMBER);
				arrayExceptionTranIdx.Add(nPendingTranIdx);
			}
		}

		//PROCESS RECONCILED TRANSACTIONS

		m_WorkingDlg.SetCaption1("Removing Reconciled Transactions From Pending List");

		for (int n = 0; n < arrayReconciledTranIdx.GetSize(); n++)
		{
			m_WorkingDlg.SetCaption2RecordsOfTotal(n + 1, arrayReconciledTranIdx.GetSize());

			int nPendingTranIdx = arrayReconciledTranIdx.GetAt(n);
			if ((nPendingTranIdx < 0) || (nPendingTranIdx >= arrayPendingTran.GetSize()))
			{
				continue;
			}

			CSQLRowPebblePendingTransaction RowPendingTransaction = arrayPendingTran.GetAt(nPendingTranIdx);

			{
				CSQLRepositoryPebblePendingTransaction PrepStatPending(FALSE == m_bNewPayments);
				if (PrepStatPending.DeleteRow(RowPendingTransaction, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					m_strPebbleError = "Unable to delete pending transaction (1)";
					return;
				}
			}

			if (TRUE == m_bNewPayments)
			{
				CSQLRowPebbleRecentTransaction RowRecent;
				RowRecent.SetTransactionID(RowPendingTransaction.GetTransactionID());
				RowRecent.SetDateTime(RowPendingTransaction.GetDateTime());

				CSQLRepositoryPebbleRecentTransaction PrepStatRecent;
				if (PrepStatRecent.UpsertRow(RowRecent, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					m_strPebbleError = "Unable to add transaction to recents list (1)";
					return;
				}
			}
		}

		//PROCESS EXCEPTIONS

		if (TRUE == m_bNewPayments)
		{
			m_WorkingDlg.SetCaption1("Removing Exceptions From Pending List");

			for (int n = 0; n < arrayExceptionTranIdx.GetSize(); n++)
			{
				m_WorkingDlg.SetCaption2RecordsOfTotal(n + 1, arrayExceptionTranIdx.GetSize());

				int nPendingTranIdx = arrayExceptionTranIdx.GetAt(n);
				if ((nPendingTranIdx < 0) || (nPendingTranIdx >= arrayPendingTran.GetSize()))
				{
					continue;
				}

				CSQLRowPebblePendingTransaction RowPendingTransaction = arrayPendingTran.GetAt(nPendingTranIdx);

				{
					CSQLRepositoryPebblePendingTransaction PrepStatPending(FALSE);
					if (PrepStatPending.DeleteRow(RowPendingTransaction, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						m_strPebbleError = "Unable to delete pending transaction (2)";
						return;
					}
				}

				{
					CSQLRowPebbleRecentTransaction RowRecent;
					RowRecent.SetTransactionID(RowPendingTransaction.GetTransactionID());
					RowRecent.SetDateTime(RowPendingTransaction.GetDateTime());

					CSQLRepositoryPebbleRecentTransaction PrepStatRecent;
					if (PrepStatRecent.UpsertRow(RowRecent, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						m_strPebbleError = "Unable to add transaction to recents list (2)";
						return;
					}
				}

				{
					CSQLRepositoryPebblePendingTransaction PrepStatPending(TRUE);

					if (PrepStatPending.UpsertRow(RowPendingTransaction, m_pDatabase).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						m_strPebbleError = "Unable to add transaction to exceptions list";
						return;
					}
				}
			}
		}

		m_bCanCommit = TRUE;
	}
}

/**********************************************************************/

void CSQLTranPebbleImportStage2::LogException(CWebPaymentLogRecord* pLogRecord, const char* szErrorText)
{
	pLogRecord->LogException(m_strTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nExceptionCount);

	if (TRUE == m_bNewPayments)
	{
		pLogRecord->LogException(m_strNewTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nNewExceptionCount);
	}
}

//*******************************************************************
