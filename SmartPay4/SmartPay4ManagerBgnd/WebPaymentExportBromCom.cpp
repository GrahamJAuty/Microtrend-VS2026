//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_BromComBalanceChanges\SQLRepositoryBromComBalanceChanges.h"
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRepositoryBromComTransaction.h"
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRecordSetBromComTransaction.h"
#include "..\SmartPay4Shared\WaitDlg.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\SQLTranAuditPendingExport.h"
//*******************************************************************
#include "BromCom.hpp"
#include "BromComData.h"
#include "WebPaymentAtcTransactionFile.h"
#include "WebPaymentDefines.h"
#include "WebPaymentExportCsvLog.h"
#include "WebPaymentExportTnLog.h"
#include "WebPaymentExportTxLog.h"
//*******************************************************************
#include "WebPaymentExportBromCom.h"
//*******************************************************************

CWebPaymentExportBromCom::CWebPaymentExportBromCom( CWnd* pParent )
{
	m_pParent = pParent;
	m_strError = "";
	m_nBalanceCount = 0;
	m_bKeepXmlFiles = FALSE;
	m_webOptions.LoadRow(NULL);
}

//*******************************************************************
// Send account balances - called from main menu

void CWebPaymentExportBromCom::SendAccountBalances()
{
	int nExportReqd = IDYES;											// assume export required
	if (TRUE == m_webOptions.m_Row.GetPromptBeforeAccessFlag())
	{
		nExportReqd = Prompter.YesNo("Please confirm - Export account balances now?", "Send Acccount Balances", TRUE, TRUE);
	}

	if (nExportReqd == IDYES)
	{
		{
			int nChecked = 0;
			CWorkingDlg WorkingDlg("Checking for BromCom accounts");
			WorkingDlg.Create();

			CSQLRecordSetAccountFull RecordSetAccount(NULL, RSParams_AccountFull_NoParams{}, FALSE);

			CSQLRowAccountFull RowAccount;

			while( RecordSetAccount.StepSelectAll(RowAccount) == TRUE )
			{
				WorkingDlg.SetCaption2RecordsChecked(++nChecked, FALSE);

				if (RowAccount.GetNextUserID() == 0)
				{
					int nPersonID = atoi(RowAccount.GetMemberID());

					if (nPersonID != 0)
					{
						int nError2 = 0;
						CSQLRepositoryBromComBalanceChanges PrepStatBalance;

						if (PrepStatBalance.RowExists(nPersonID, NULL).GetSQLResult() == 0 )
						{
							CSQLRowBromComBalanceChanges RowBalance;
							RowBalance.SetPersonID(nPersonID);
							RowBalance.SetAccountID(RowAccount.GetUserID());
							PrepStatBalance.InsertRow(RowBalance,NULL);
						}
					}
				}
			} 
		}

		__int64 nBalanceCount = 0;
		{
			int nError = 0;

			CSQLRepositoryBromComBalanceChanges PrepStat;

			nBalanceCount = PrepStat.CountRows(NULL).GetSQLResult();
			if ( 0 == nBalanceCount )
			{
				Prompter.Error("No new BromCom balances were found to export");
			}
			else
			{
				ExportBalances(nBalanceCount);
			}
		}
	}
}

//*******************************************************************
// Called after importing payments
// or
// if bIsEndOfDay == TRUE from main menu 'Send End of Day' option
//
// sets errors message

bool CWebPaymentExportBromCom::SendAccountUpdates(bool bIsEndOfDay)
{
	m_strError = "";

	bool bIsSetUp = FALSE;
	bool bExportBalances = FALSE;
	
	if (TRUE == bIsEndOfDay)
	{
		bExportBalances = TRUE;
	}
	else
	{
		if (TRUE == m_webOptions.m_Row.GetEODAfterPaymentFlag())
		{
			bExportBalances = m_webOptions.m_Row.TimeForEndOfDay();
		}
	}
	
	CBromComData data;
	data.Read();
	bIsSetUp = data.IsSet();
	m_bKeepXmlFiles = data.m_bKeepXmlFiles;
	
	// check interface setup or error message set
	if ( ( FALSE == bIsSetUp ) || ( m_strError != "") )
	{
		if (CModuleGlobals::IsBackgroundModule() == TRUE)
		{
			m_strError = "";
		}
		else if (m_strError == "")
		{
			m_strError = szINTERFACENOTSET;	
		}

		return FALSE;
	}

	// upload not allowed at this point in time
	if (m_webOptions.m_Row.TimeForUpload() == FALSE)					
	{
		return FALSE;
	}

	// assume export required
	int nExportReqd = IDYES;											
	CString strEODCaption = "";
	strEODCaption.Format("%s End of Day", 
		(const char*) System.GetWebPaymentDisplayName());

	if (bIsEndOfDay == TRUE)
	{
		if (m_webOptions.m_Row.TimeForEndOfDay() == FALSE)					// see if web access allowed for EndofDay balances
		{
			if (CModuleGlobals::IsBackgroundModule() == FALSE)
			{
				CString strMsg = m_webOptions.m_Row.GetError();
				strMsg += "\n\nDo you still wish to continue?";

				if (Prompter.YesNo(strMsg, strEODCaption, FALSE, FALSE) == IDNO)
				{
					nExportReqd = IDNO;									// EOD balances not required
				}
			}

			bExportBalances = FALSE;								// EOD balances not required
		}
	}

	if ( IDYES == nExportReqd)
	{
		if (TRUE == bIsEndOfDay)
		{
			if (m_webOptions.m_Row.GetPromptBeforeAccessFlag() == TRUE)
			{
				nExportReqd = Prompter.YesNo("Please confirm - Export End of Day now?", strEODCaption, TRUE, TRUE);
			}
		}

		if (nExportReqd == IDYES)
		{
			ExportTransactions();										// export any new purchases
			
			__int64 nBalanceCount = 0;
			{
				CSQLRepositoryBromComBalanceChanges PrepStat;				
				__int64 nBalanceCount = PrepStat.CountRows(NULL).GetSQLResult();

				if (nBalanceCount > 0)
				{
					bExportBalances = TRUE;
				}
			}
	
			if ((TRUE == bExportBalances) && (m_strError == ""))
			{
				// export balance list or EOD balances
				ExportBalances(nBalanceCount);				
			}
		}
	}

	return (IDYES == nExportReqd);
}

//*******************************************************************

void CWebPaymentExportBromCom::ExportTransactions()
{
	m_strError = "";
	CString strWebAtcUpdates = Filenames.GetWebPaymentUpdatesFilename("dat");	// ..\{web}\webupdates.dat

	CSQLTranAuditPendingExport Tran(strWebAtcUpdates);

	Tran.BeginTrans();
	Tran.DoWork();

	switch (Tran.EndTrans())
	{
		case SQLTRAN_STATE_COMMITOK:
			Tran.AfterTransaction();
			break;

		default:
		{
			m_strError = Tran.GetError();

			CString strMsg = "";
			strMsg += "Unable to prepare BromCom export transactions.\n\n";
			strMsg += m_strError;
			Prompter.Error(strMsg);
		}
		return;
	}

	CBromComData BromComData;
	BromComData.Read();

	CArray<__int64, __int64> arrayTransactionsDone;

	CWorkingDlg WorkingDlg("Updating BromCom Transactions");
	WorkingDlg.Create();

	{
		int nRecordsChecked = 0;
		int nTransactionsSent = 0;
		CSQLRecordSetBromComTransaction RecordSet(NULL, RSParams_BromComTransaction_NormalNoParams{});

		CSQLRowBromComTransaction RowTran;
		while (RecordSet.StepSelectAll(RowTran) == TRUE)
		{
#ifndef SYSTEMTYPE_BACKGROUND
			AutoShutdownHelper.ResetShutdownTime();
#endif
			if (WorkingDlg.SetCaption2RecordsChecked(++nRecordsChecked, TRUE) == TRUE)
			{
				break;
			}

			CString strLoginReply = Filenames.GetWebPaymentImportFilenameNum("XML", 4);

			CSSBromCom BromCom(&BromComData);
			CString strError = BromCom.SendBromComTransaction(strLoginReply, RowTran);

			CFileRecycler::SendToRecycleBin(strLoginReply);

			if (strError == "")
			{
				arrayTransactionsDone.Add(RowTran.GetLineID());
				nTransactionsSent++;

				CWebPaymentExportTnLog log(nWEBPAYMENT_BROMCOM, "TS");
				CString strLogFile = log.GetCurrentLogName();

				CSSFile fileLog;
				if (fileLog.Open(strLogFile, "ab") == TRUE)
				{
					CCSV csv;
					csv.Add(RowTran.GetPersonID());
					csv.Add(RowTran.GetAmount(), 2);
					csv.Add(RowTran.GetBalance(), 2);
					csv.Add(RowTran.GetDateTime());
					csv.Add(RowTran.GetType());
					csv.Add(RowTran.GetTransactionID());
					csv.Add(RowTran.GetWebComment());

					fileLog.WriteLine(csv.GetLine());
				}
			}
			else
			{
				CWebPaymentExportTxLog log;
				log.SaveLog(nWEBEXPORT_UPDATES, nWEBEXPORT_ERROR, strError);

				CString strPersonID = "";
				strPersonID.Format("==> PersonID %d", RowTran.GetPersonID());
				log.SaveLog(nWEBEXPORT_UPDATES, nWEBEXPORT_ERROR, strPersonID);
			}
		}

		if (nTransactionsSent > 0)
		{
			CWebPaymentExportTnLog log(nWEBPAYMENT_BROMCOM, "TS");
			CString strLogFile = log.GetCurrentLogName();
			log.AddDateStamp(strLogFile, nTransactionsSent);
		}
	}

	WorkingDlg.DestroyWindow();

	{
		int nTransactionCount = arrayTransactionsDone.GetSize();

		if (nTransactionCount > 0)
		{
			CString strLogComment = "";
			strLogComment.Format("%d lines", nTransactionCount);		// log number of lines sent in summary log

			CWebPaymentExportTxLog log;
			log.SaveLog(nWEBEXPORT_UPDATES, nWEBEXPORT_NOERROR, strLogComment);

			int nRetries = 0;
			while (++nRetries <= 10)
			{
				m_webOptions.LoadRow(NULL);
				m_webOptions.m_Row.SetLastTransactionDone();
				if (m_webOptions.SaveRow(NULL, TRUE) == SQLCRUD_ERR_NONE)
				{
					break;
				}
			}
		}
	}

	CSQLRepositoryBromComTransaction PrepStatTran;
	CSQLRowBromComTransaction RowTran;

	for (int n = 0; n < arrayTransactionsDone.GetSize(); n++)
	{
		RowTran.SetLineID(arrayTransactionsDone.GetAt(n));
		PrepStatTran.DeleteRow(RowTran,NULL);
	}
}

//******************************************************************

void CWebPaymentExportBromCom::ExportBalances(__int64 nBalanceCount)
{
	m_nBalanceCount = 0;

	CBromComData BromComData;
	BromComData.Read();

	CArray<int, int> arrayBalancesDone;

	CWorkingDlg WorkingDlg("Updating BromCom Balances");
	WorkingDlg.Create();

	{
		int nRecordsChecked = 0;
		CSQLRecordSetBromComBalanceChanges RecordSet(NULL, RSParams_BromComBalanceChanges_NormalNoParams{});

		CSQLRowBromComBalanceChanges RowBalance;
		while (RecordSet.StepSelectAll(RowBalance) == TRUE)
		{
#ifndef SYSTEMTYPE_BACKGROUND
			AutoShutdownHelper.ResetShutdownTime();
#endif

			if (WorkingDlg.SetCaption2RecordsChecked(++nRecordsChecked, TRUE) == TRUE)
			{
				break;
			}

			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(RowBalance.GetAccountID());

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
			{
				CString strLoginReply = Filenames.GetWebPaymentImportFilenameNum("XML", 3);

				CSSBromCom BromCom(&BromComData);
				CString strError = BromCom.SendBromComBalance(strLoginReply, RowBalance.GetPersonID(), RowAccount.GetPurse1Balance());

				CFileRecycler::SendToRecycleBin(strLoginReply);

				if (strError == "")
				{
					arrayBalancesDone.Add(RowBalance.GetPersonID());
					m_nBalanceCount++;
				}
				else
				{
					CWebPaymentExportTxLog log;
					log.SaveLog(nWEBEXPORT_BALANCES, nWEBEXPORT_ERROR, strError);

					CString strPersonID;
					strPersonID.Format("==> PersonID %d", RowBalance.GetPersonID());
					log.SaveLog(nWEBEXPORT_BALANCES, nWEBEXPORT_ERROR, strPersonID);
				}
			}
		}
	}

	WorkingDlg.DestroyWindow();

	if (m_nBalanceCount > 0)
	{
		CString strLogComment = "";
		strLogComment.Format("%d lines", m_nBalanceCount);	// summary tx log comment lines

		CWebPaymentExportCsvLog logTz;							// individual balances exported
		logTz.SaveLog("Tz", "", m_nBalanceCount);

		int nRetries = 0;
		while (++nRetries <= 10)
		{
			m_webOptions.LoadRow(NULL);
			m_webOptions.m_Row.SetLastBalanceDone();
			if (m_webOptions.SaveRow(NULL, TRUE) == SQLCRUD_ERR_NONE)
			{
				break;
			}
		}

		CWebPaymentExportTxLog log;
		log.SaveLog(nWEBEXPORT_BALANCES, nWEBEXPORT_NOERROR, strLogComment);
	}

	CSQLRepositoryBromComBalanceChanges PrepStatBalance;
	CSQLRowBromComBalanceChanges RowBalance;

	for (int n = 0; n < arrayBalancesDone.GetSize(); n++)
	{
		RowBalance.SetPersonID(arrayBalancesDone.GetAt(n));
		PrepStatBalance.DeleteRow(RowBalance,NULL);
	}

}

//*******************************************************************

