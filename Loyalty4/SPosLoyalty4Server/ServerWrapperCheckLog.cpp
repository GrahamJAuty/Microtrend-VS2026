//*******************************************************************
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLStampOfferBuffer.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "EposLoyaltyFile.h"
#include "SPosLoyalty4Server.h"
#include "SQLStaffAccountBuffer.h"
#include "SQLTranSaveLogUpdates.h"
#include "TransactionCache.h"
//*******************************************************************
#include "ServerWrapper.h"
//*******************************************************************

void CServerWrapper::RequestCheckLog(CString strExtension)
{
	CServerDataFolderSetIndex FolderSetIndex;

	int nMaxDb = min(PLU_DBNO_COUNT, System.GetMaxEcrmanDatabases());

	if (strExtension == "LOG")
	{
		for (int nDbNo = 1; nDbNo <= nMaxDb; nDbNo++)
		{
			FolderSetIndex.m_nDbNo = nDbNo;

			for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
			{
				FolderSetIndex.m_nSetNo = nSetNo;

				m_LoyLogThreadManager.CheckThread(FolderSetIndex);

				LONGLONG nThreadTime = m_LoyLogThreadManager.GetThreadTime();

				if (nThreadTime >= 3000)
				{
					if (Server.GetLogActionFlag() == TRUE)
					{
						CString strMsg = "";
						strMsg.Format("==> Folder Set %2.2d:%2.2d Slow LOG response (%I64d)", 
							FolderSetIndex.m_nDbNo,
							FolderSetIndex.m_nSetNo,
							nThreadTime / 1000);

						MessageLogger.LogSystemMessage(strMsg);
					}
				}

				m_LoyLogThreadManager.CreateLocalLogFile();
			}
		}
	}

	for (int nDbNo = 1; nDbNo <= nMaxDb; nDbNo++)
	{
		FolderSetIndex.m_nDbNo = nDbNo;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			FolderSetIndex.m_nSetNo = nSetNo;

			Server.SetCurrentFolderSetIndex(FolderSetIndex);
			CString strLogPath = Server.GetCurrentEposLogPath();
			CString strReqPath = Server.GetCurrentEposRequestPath();

			if ((strLogPath != "") && (strReqPath != ""))
			{
				RequestCheckLogInternal(strExtension);
			}
		}
	}

	if ((GetTickCount64() > m_nLastPurgeTime + 5000) || (GetTickCount64() < m_nLastPurgeTime))
	{
		TransactionCache.PurgeCache();
		m_nLastPurgeTime = GetTickCount64();
		TransactionCache.WriteCacheToFile();
	}
}

//***************************************************************************************

void CServerWrapper::RequestCheckLogInternal(CString strExtension)
{
	CFileFind FileFinder;
	CString strSourceFileMask = "";

	CServerDataFolderSetIndex FolderSetIndex = Server.GetCurrentFolderSetIndex();

	strSourceFileMask.Format("%s\\%3.3d_%3.3d_LOY*.%s",
		(const char*)Filenames.GetLogBufferFolder(),
		FolderSetIndex.m_nDbNo,
		FolderSetIndex.m_nSetNo,
		(const char*)strExtension);

	BOOL bWorking = FileFinder.FindFile(strSourceFileMask);
	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strLogFileTitle = FileFinder.GetFileTitle();

		CString strEposLogFile = Filenames.GetLogBufferFolder();
		strEposLogFile += "\\";
		strEposLogFile += FileFinder.GetFileName();

		CString strEposLogNoExt = Filenames.GetLogBufferFolder();
		strEposLogNoExt += "\\";
		strEposLogNoExt += strLogFileTitle;

		CString strRenamedLogFile;
		if (lstrcmp(strExtension, "$$1") == 0)
		{
			strRenamedLogFile = strEposLogFile;								// is already a .$$1 log file
		}
		else
		{
			strRenamedLogFile = strEposLogNoExt + ".$$1";

			if (SolutionGlobalFunctions::FileExists(strRenamedLogFile) == TRUE)					// see if already have a $$1 file waiting
			{
				if (::AppendDataFile(strEposLogFile, strRenamedLogFile) == TRUE)	// add to end of $$1 file
				{
					::SendToRecycleBin(strEposLogFile);								// delete file just appended	 
				}
			}
			else if (::rename(strEposLogFile, strRenamedLogFile) != 0)		// rename extension from .log to .$$1
			{
				continue;														// not worked - retry later
			}
		}

		// no process file
		m_dateToday.SetCurrentDate();
		m_timeToday.SetCurrentTime();										// set date & time

		CString strErrorFile = strEposLogNoExt + "ERR";						// change extension from .LOG to .ERR
		CString strEposCardNo = "";											// gets filled out with cardno found in log file
		int nPointsAward = 0;												// gets filled out with data found in log file
		double dBonusCash = 0;												// gets filled out with data found in lof file

		if (Server.GetLogActionFlag() == TRUE)
		{
			CString strMsg = "";
			strMsg.Format("==> LOG: %s",
				(const char*)strEposLogFile);

			MessageLogger.LogSystemMessage(strMsg);
		}

		CSQLRowAccountFull RowAccount;
		if (ProcessLogFile(RowAccount, strRenamedLogFile, strErrorFile, FolderSetIndex, strEposCardNo, nPointsAward, dBonusCash) == FALSE)	// false =logfile has only 1 transaction
		{
			if ((Server.GetNewBalanceOnReceiptFlag() == TRUE) && strEposCardNo != "")
			{
				CString strReplyFile = "";
				strReplyFile += Server.GetCurrentEposRequestPath();
				strReplyFile += "\\";

				if (Server.GetRealTimePlusFlag() == TRUE)
				{
					strReplyFile += "LID";
					strReplyFile += strEposCardNo;
					strReplyFile += ".RSP";
				}
				else
				{
					CString strFilename = FileFinder.GetFileTitle();
					strReplyFile += strFilename.Mid(8);
					strReplyFile += ".RSP";
				}

				m_LoyReqThreadManager.ClearResponse();
				CreateBalanceReply2(RowAccount, nPointsAward, dBonusCash);

				CServerDataFolderSetIndex FolderSetIndex = Server.GetCurrentFolderSetIndex();

				m_LoyRspThreadManager.QueueResponseHeader(FolderSetIndex);
				m_LoyRspThreadManager.QueueResponseLine(FolderSetIndex, strReplyFile + ".tmp");
				m_LoyRspThreadManager.QueueResponseLine(FolderSetIndex, strReplyFile);
				m_LoyRspThreadManager.QueueResponseLine(FolderSetIndex, "0");

				for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
				{
					m_LoyRspThreadManager.QueueResponseLine(FolderSetIndex, m_LoyReqThreadManager.GetResponseLine(n));
				}

				m_LoyRspThreadManager.CheckThread(FolderSetIndex);
			}
		}
	}
}

//**********************************************************************
// logfilepath = "c:\xloylog\LOYnnn.$$1"	( rename from .LOG )
// errfilepath = "c:\xloylog\LOYnnn.ERR"

// returns FALSE if the file contains multiple transactions

bool CServerWrapper::ProcessLogFile( CSQLRowAccountFull& RowAccount, const char* szLogFilePath, const char* szErrorFilePath, CServerDataFolderSetIndex& FolderSetIndex, CString& strEposCardNo, int& nPointsAward, double& dBonusCash)
{
	::remove(szErrorFilePath);								// delete error file, if found

	SQLStaffAccountBuffer.Reset();
	SQLStampOfferBuffer.Reset();

	CEposLoyaltyFile logFile(szLogFilePath, FolderSetIndex );
	if (logFile.ScanLogFile() == TRUE)
	{
		bool bOk = TRUE;
		bool bNewDbRecord = FALSE;
		bool bCanProceed = FALSE;

		SQLStampOfferBuffer.SetServerNo(logFile.GetServerNo());
		SQLStampOfferBuffer.SetDbNo(logFile.GetDbNo());
		SQLStampOfferBuffer.SetFolderSet(logFile.GetFolderSet());
		SQLStampOfferBuffer.SetTillNo(logFile.GetTillNo());
		
		bool bSimpsInns = FALSE;
		__int64 nCustomerCardNo = 0;
		CString strCardNo = logFile.GetCustomerCardNo();
		if ((strCardNo.Left(4) == "1033") && (Server.GetSimpsInnsLookupFlag() == TRUE))
		{
			CAccountIdentifier AccountIdentifier(strCardNo, nINTERFACE_SIMPSINNS);
			CString strTranslate = System.TranslateCardNo(AccountIdentifier, strCardNo, FALSE);
			nCustomerCardNo = _atoi64(strTranslate);
			bSimpsInns = TRUE;
		}
		else
		{
			nCustomerCardNo = _atoi64(logFile.GetCustomerCardNo());
		}
		
		while (FALSE == bCanProceed)
		{		
			RowAccount.SetUserID(nCustomerCardNo);

			CSQLRepositoryAccount repoAccount;
			if ( repoAccount.SelectRow(RowAccount,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				bOk = (Server.CanAddUnknownCard() && (FALSE == bSimpsInns));

				if (TRUE == bOk)
				{
					CString strFullname = Server.GetUnknownAccountCreateName();
					::TrimSpacesFromString(strFullname, TRUE);
					RowAccount.SetFullname(strFullname);

					repoAccount.InsertRow(RowAccount, NULL);

					bNewDbRecord = bOk = (repoAccount.SelectRow(RowAccount,NULL).GetSQLError() == SQLCRUD_ERR_NONE);

					if (TRUE == bOk)
					{
						RowAccount.SetGroupNo(Server.GetUnknownCardGroupNo());
						repoAccount.UpdateRow(RowAccount,NULL);
					}
				}
			}

			bCanProceed = TRUE;

			if (TRUE == bOk)
			{
				SQLStampOfferBuffer.SetAccount(RowAccount);
				SQLStaffAccountBuffer.ValidateStaffGifts();

				strEposCardNo = logFile.GetEposCardNo();			// prefix + cardno
				logFile.UpdateLoyaltyAccount(RowAccount, bNewDbRecord);

				CSQLTranSaveLogUpdates Tran;
				Tran.BeginTrans();
				Tran.DoWork(RowAccount);

				switch (Tran.EndTrans())
				{
				case SQLTRAN_STATE_COMMITOK:
					break;

				default:
					bCanProceed = FALSE;
					break;
				}

				if (TRUE == bCanProceed)
				{
					logFile.WriteAuditBuffer();
					logFile.ProcessPurchaseHistory(RowAccount);
					SQLStampOfferBuffer.WriteAuditLines(logFile.GetTransactionCCNo());
					nPointsAward = logFile.m_nPointsAwarded;			// total points award for transaction
					dBonusCash = logFile.m_dBonusCash;					// total of bonus cash awarded for transaction
				}
			}
		}

		if (logFile.HaveMultipleTransactions() == FALSE)		// leave .$$1 file if file contained multiple transactions
		{
			::remove(szLogFilePath);
		}
	}
	else
	{
		int nRenameResult = ::rename(szLogFilePath, szErrorFilePath);		// rename log file to error file

		CSSFile file;
		if (file.Open(szErrorFilePath, "ab") == TRUE)
		{
			file.WriteLine("#ERR,************************");
			file.WriteLine("#ERR,Error detected by Server");
			file.WriteLine(logFile.GetError());
			file.WriteLine("#ERR,************************");
			file.Close();
		}
	}
	
	return logFile.HaveMultipleTransactions();
}

//**********************************************************************
