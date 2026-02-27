/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\MessageLogger.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRowEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRowPebbleExportPayment.h"
/**********************************************************************/
#include "EposSmartPayFile.h"
#include "SQLTranSaveLogUpdates.h"
#include "TransactionCache.h"
/**********************************************************************/
#include "ServerWrapper.h"
/**********************************************************************/

void CServerWrapper::RequestCheckLog(CString strExtension)
{
	if (strExtension == "LOG")
	{
		for (int nThread = 1; nThread <= EPOSREQPATH_COUNT; nThread++)
		{
			m_LoyLogThreadManager.CheckThread(nThread);

			LONGLONG nThreadTime = m_LoyLogThreadManager.GetThreadTime();

			if (nThreadTime >= 3000)
			{
				CString strMsg = "";
				strMsg.Format("==> Folder Set %2.2d Slow LOG response (%I64d)", 
					nThread, 
					nThreadTime / 1000);

				LogAction2(strMsg);
			}

			m_LoyLogThreadManager.CreateLocalLogFile();
		}
	}

	for (int n = 1; n <= EPOSREQPATH_COUNT; n++)
	{
		Server.SetCurrentEposPathSet(n);
		CString strLogPath = Server.GetCurrentEposLogPath();
		CString strReqPath = Server.GetCurrentEposRequestPath();

		if ((strLogPath != "") && (strReqPath != ""))
		{
			RequestCheckLogInternal(strExtension);
		}
	}

	if ((GetTickCount64() > m_nLastPurgeTime + 5000) || (GetTickCount64() < m_nLastPurgeTime))
	{
		TransactionCache.PurgeCache();
		m_nLastPurgeTime = GetTickCount64();
		TransactionCache.WriteCacheToFile();
	}
}

/**********************************************************************/

void CServerWrapper::RequestCheckLogInternal(CString strExtension)
{
	CFileFind FileFinder;
	CString strSourceFileMask;

	strSourceFileMask.Format("%s\\%3.3d_LOY*.%s", 
		(const char*) Filenames.GetLogBufferFolder(), 
		Server.GetCurrentEposPathSet(), 
		(const char*) strExtension);

	BOOL bWorking = FileFinder.FindFile(strSourceFileMask);
	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		m_bIsKioskRequest = FALSE;
		m_bIsVendingRequest = FALSE;
		m_bIsEposRequest = FALSE;

		CString strFilename = FileFinder.GetFileName();

		if (strFilename.Left(6) == "LOYBIO")
		{
			m_bIsKioskRequest = TRUE;
		}
		/*
		else if (strFilename.Left(6) == "LOYVND")
		{
			m_bIsVendingRequest = TRUE;
		}
		*/
		else
		{
			m_bIsEposRequest = TRUE;
		}

		CString strLogFileTitle = FileFinder.GetFileTitle();

		CString strEposLogFile = Filenames.GetLogBufferFolder();
		strEposLogFile += "\\";
		strEposLogFile += FileFinder.GetFileName();

		CString strEposLogNoExt = Filenames.GetLogBufferFolder();
		strEposLogNoExt += "\\";
		strEposLogNoExt += strLogFileTitle;

		m_dateToday.SetCurrentDate();											// set date & time

		CString strRenamedLogFile = "";

		if (lstrcmp(strExtension, "$$1") == 0)								// see if doing $$1 pass
		{
			strRenamedLogFile = strEposLogFile;									// is already a .$$1 log file
		}
		else																	// doing a .log pass
		{
			strRenamedLogFile = "";
			strRenamedLogFile += strEposLogNoExt;
			strRenamedLogFile += ".$$1";

			if (::FileExists(strRenamedLogFile) == TRUE)					// see if already have a $$1 file waiting
			{
				if (::AppendDataFile(strEposLogFile, strRenamedLogFile) == TRUE)	// add to end of $$1 file
				{
					CFileRecycler::SendToRecycleBin(strEposLogFile);								// delete file just appended	 
				}
			}
			else if (::Rename_Trap(strEposLogFile, strRenamedLogFile) != 0)		// rename extension from .log to .$$1
			{
				continue;														// not worked - retry later
			}
		}

		if (m_bIsEposRequest == TRUE)
		{
			CString strMsg = "";
			strMsg.Format("==> LOG: %s", (const char*)strEposLogFile);
			LogAction2(strMsg);
		}

		CSmartPayLockRequest LockRequest;
		LockRequest.m_bWantLockServerInner = TRUE;				
		
		CSmartPayLockManager LockManager;
		int nLockError = LockManager.AcquireLocks(LockRequest);		

		if (LOCKMANAGER_OK == nLockError)										// see if server is pause ( //v1.05f 20/09/2015 )
		{
			LockManager.LogError(nLockError);

			CString strErrorFile = "";
			strErrorFile.Format("%s\\%s", (const char*) ::StripFilename(strEposLogNoExt), (const char*) ::GetUniqueTempFilename("ERR", strLogFileTitle));

			m_dateToday.SetCurrentDate();
			m_timeToday.SetCurrentTime();

			CString strEposUserID = "";
			CStringArray arrayError;
			if (ProcessLogFile(strRenamedLogFile, strErrorFile, Server.GetCurrentEposPathSet(), strEposUserID) == FALSE)
			{
				if (m_bIsEposRequest == TRUE && Server.GetNewBalanceOnReceiptFlag() == TRUE)
				{
					CString strUserID = strEposUserID;							// may be blank, CashAccountNo, ProperAccountNo (v1.05x 17/11/2015)
					strUserID.TrimLeft('0');									// remove leading 0's

					if (strUserID != "")
					{
						// change extension from .LOG to .RSP & save in Reply folder
						CString strRequestReplyFile = "";
						strRequestReplyFile.Format("%s\\%s.RSP",
							Server.GetCurrentEposRequestPath(),
							(const char*) strLogFileTitle.Right(strLogFileTitle.GetLength() - 4));

						m_LoyReqThreadManager.ClearResponse();
						CreateRequestReply(strEposUserID, NULL);

						CString strMsg;
						strMsg.Format("<== RSP: %s", (const char*)strRequestReplyFile);
						LogAction2(strMsg); ;

						int nThread = Server.GetCurrentEposPathSet();
						m_LoyRspThreadManager.QueueResponseHeader(nThread);
						m_LoyRspThreadManager.QueueResponseLine(nThread, strRequestReplyFile + ".tmp");
						m_LoyRspThreadManager.QueueResponseLine(nThread, strRequestReplyFile);
						m_LoyRspThreadManager.QueueResponseLine(nThread, "0");

						for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
						{
							m_LoyRspThreadManager.QueueResponseLine(nThread, m_LoyReqThreadManager.GetResponseLine(n));
						}

						m_LoyRspThreadManager.CheckThread(nThread);
					}
				}
			}
		}
		else
		{
			LockManager.LogError(nLockError);
		}
	}
}

/**********************************************************************/

// logfilepath		= "c:\xloylog\LOYnnn.$$1"	( rename from .LOG )
// errfilepath		= "c:\xloylog\LOYnnn-yyyymmddhhmmss.ERR"

// returns FALSE if the file contains multiple transactions
// strEposUserID = user id as sent from epos (inc leading 0's ) or translated if MiFare.v2 & loylog offline ID

bool CServerWrapper::ProcessLogFile(const char* szLogFilePath, const char* szErrorFile, int nFolderSet, CString& strEposUserID)
{
	// clear to indicate no valid id processed
	strEposUserID = "";		

	CPendingExportPaymentData PendingExportPaymentData;
	PendingExportPaymentData.m_bPebble = (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE);
	PendingExportPaymentData.m_bChartwell = System.GetEnableChartwellsFlag();

	CPendingEposTerminalData PendingEposTerminalData;
	PendingEposTerminalData.m_bGotChange = FALSE;

	CEposSmartPayFile logFile(szLogFilePath, nFolderSet, PendingExportPaymentData, PendingEposTerminalData);
	if (logFile.ScanSharpPos() == TRUE)
	{
		int nDbError = DB_ERR_NONE;
		SQLPurchaseControlBuffer.SetServerNo(logFile.GetServerNo());
		SQLPurchaseControlBuffer.SetFolderSet(logFile.GetFolderSet());
		SQLPurchaseControlBuffer.SetTillNo(logFile.GetTillNo());

		if (logFile.GetFullEposUserID() != Server.GetOfflineSpendID())
		{
			bool bCanProceed = FALSE;

			while (FALSE == bCanProceed)
			{
				// See if assigned to offline ID account (inc leading 0's)
					// Get AccountID for given Epos UserID
				CString strAccountID = logFile.GetEposUserID();

				// User ID as sent from epos (inc leading 0's )
				strEposUserID = logFile.GetFullEposUserID();

				if (strAccountID.GetLength() > 6)
				{
					if (System.GetInterfaceType() == nINTERFACE_MIFAREv2 || System.GetInterfaceType() == nINTERFACE_DUAL)	// check for Mifare number
					{
						CSQLRepositoryAccount RepoAccount;
						__int64 nUserID = RepoAccount.LookupExtRef1(strAccountID, NULL).GetSQLResult();

						if (nUserID != 0)
						{
							strAccountID = FormatInt64Value(nUserID);
							strEposUserID = strAccountID;
						}
					}
				}

				if (Sysset.IsNobbledDemo() == TRUE)
				{
					if ((rand() % 10) == 5)
					{
						strAccountID = "1";
					}
				}

				if (strAccountID != "")
				{
					bool bOK = TRUE;

					CSQLRowAccountFull RowAccountToUpdate;
					RowAccountToUpdate.SetUserID(_atoi64(strAccountID));

					CSQLRepositoryAccount RepoAccount;
					if (RepoAccount.SelectRow(RowAccountToUpdate, NULL).GetSQLError() == SQLCRUD_ERR_NOMATCH)
					{
						bOK = CanAddNewRecord(strAccountID);

						if (TRUE == bOK)
						{
							RowAccountToUpdate.SetGroupNo(Server.GetUnknownGroup());

							if (strAccountID == "0")
							{
								RowAccountToUpdate.SetUsername("Cash Sales");
							}

							CSQLRepositoryAccount RepoAccount;
							if (RepoAccount.InsertRow(RowAccountToUpdate, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
							{
								AuditNewRecord(RowAccountToUpdate);
							}
							else
							{
								bOK = FALSE;
							}
						}
					}

					bCanProceed = TRUE;

					if (TRUE == bOK)
					{
						int nGroupNo = 0;
						SQLPurchaseControlBuffer.SetAccount(RowAccountToUpdate);
						logFile.UpdateRecordFromLogData(RowAccountToUpdate, nGroupNo);
						
						PendingExportPaymentData.m_PaymentDataPebble.SetUserID(RowAccountToUpdate.GetUserID());
						PendingExportPaymentData.m_PaymentDataPebble.SetCCNo(logFile.GetTransactionCCNo());
						
						PendingExportPaymentData.m_PaymentDataChartwell.SetUserID(RowAccountToUpdate.GetUserID());
						PendingExportPaymentData.m_PaymentDataChartwell.SetGroupNo(nGroupNo);

						CSQLTranSaveLogUpdates Tran;
						Tran.BeginTrans();
						Tran.DoWork(RowAccountToUpdate, PendingExportPaymentData, PendingEposTerminalData);

						switch (Tran.EndTrans())
						{
						case SQLTRAN_STATE_COMMITOK:
							break;

						default:
							bCanProceed = FALSE;
							break;
						}

						/*
						CSQLRowAccountFull RowCheck;
						RowCheck.SetUserID(RowAccountToUpdate.GetUserID());

						if (SQLRepositoryAccount.SelectRow(RowCheck, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
						{
							if (RowAccountToUpdate.CompareRowVersion(RowCheck) == FALSE)
							{
								bCanProceed = FALSE;
							}
						}
						*/

						if (TRUE == bCanProceed)
						{
							logFile.SaveEposPaymentAudit();
							logFile.ProcessPurchaseHistory(RowAccountToUpdate);
							logFile.WritePendingAuditLines();
							SQLPurchaseControlBuffer.WriteAuditLines(logFile.GetTransactionCCNo());
						}
					}
				}
			}
		}

		/*if(TRUE == bOK)*/								// check there has been no database error (v1.03g 15/03/2015)
		{
			if (logFile.HaveMultipleTransactions() == TRUE)		// leave .$$1 file if file contained multiple transactions
			{
				logFile.RemoveTransactionLines(szLogFilePath);	// remove the lines that have been processed, leaving the rest to do next time
			}
			else
			{
				::remove(szLogFilePath);							// all lines processed - so remove log file
			}
		}
	}
	else
	{
		int nRenameResult = ::Rename_Trap(szLogFilePath, szErrorFile);			// rename log file to error file

		CSSFile file;
		if (file.Open(szErrorFile, "ab") == TRUE)
		{
			file.WriteLine("#ERR,*********************************");
			file.WriteLine("#ERR,Error detected by SmartPay Server");
			file.WriteLine(logFile.GetError());
			file.WriteLine("#ERR,*********************************");
			file.Close();
		}
	}

	return logFile.HaveMultipleTransactions();
}

//******************************************************************
