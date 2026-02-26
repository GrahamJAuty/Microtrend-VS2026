/**********************************************************************/
#include "Append.h"
#include "CashRSPCSVLines.h"
#include "CashRSPSpooler.h"
#include "CommsHandshaker.h"
#include "FileRemover.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NetworkTerminalMap.h"
#include "PriceHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "CashRSPImporter.h"
/**********************************************************************/

CCashRSPImporter::CCashRSPImporter() 
{
	m_bRebuildRemoveDuplicates = FALSE;
	m_nDuplicatedTransactions = 0;
	m_pOutputFileDailyAccount = NULL;
	m_pOutputFileDailyItemPay = NULL;
	m_pOutputFileDailyReasons = NULL;
	m_pOutputFileDailySales = NULL;
	m_pOutputFileDailyTerm = NULL;
	m_pOutputFileStockDelivery = NULL;
	m_pOutputFileStockSales = NULL;
	m_pOutputFileTermNonDuplicates = NULL;
}

/**********************************************************************/

int CCashRSPImporter::ValidateSSFileTransactionFilename( CString& strFilename )
{
	int nFileType = CASHRSP_SALES_UNKNOWN;

	int nLength = strFilename.GetLength();
	
	if ( nLength >= 26 )
	{
		if ( strFilename.Left(7) == "SSFILE_" )
		{
			if ( ::TestNumeric( strFilename.Mid(7,8) ) == TRUE )
			{
				if ( strFilename.Mid(15,1) == "_" )
				{
					if ( ::TestNumeric( strFilename.Mid(16,6) ) == TRUE )
					{
						if ( strFilename.Mid(22,1) == "_" )
						{
							nLength -= 23;
							CString strFileType = strFilename.Right( nLength );

							if (strFileType == "TERM")
							{
								nFileType = CASHRSP_SALES_PENDING;
							}
							else if ((8 == nLength) && ((::TestNumericHex(strFileType))))
							{
								nFileType = CASHRSP_SALES_PENDING;
							}
							else if ( strFileType.Left(2) == "TA" )
							{
								int nEventEditLen = nLength - 2;
								CString strEventEdit = strFileType.Right( nEventEditLen );
								
								CString strEventID = strEventEdit.SpanExcluding( "_" );
								int nEventIDLen = strEventID.GetLength();

								if ( nEventIDLen <= nEventEditLen - 2 )
								{
									CString strEditCount = strFileType.Right( nEventEditLen - ( nEventIDLen + 1 ) );

									if (::TestNumeric(strEventID) == TRUE)
									{
										if (::TestNumeric(strEditCount) == TRUE)
										{
											nFileType = CASHRSP_SALES_TIMEATTEND;
										}
									}
								}
							}
							else if (strFileType.Left(2) == "DP")
							{
								if (::TestNumeric(strFileType.Right(nLength - 2)))
								{
									nFileType = CASHRSP_SALES_DEPOSIT;
								}
							}
							else if (strFileType.Left(1) == "E")
							{
								if (::TestNumericHex(strFileType.Right(nLength - 1)))
								{
									nFileType = CASHRSP_SALES_DEPOSIT;
								}
							}
						}
					}
				}
			}
		}
	}

	return nFileType;
}

/**********************************************************************/

bool CCashRSPImporter::ProcessNetworkSales( int nNetworkIdx, CLockManager& LockManager, CCashRSPImporterStatusDisplay& StatusDisplay )
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseServer( info );
	m_BatchInfo.m_nCurrentServerDbIdx = -2;
	m_BatchInfo.m_nCurrentServerLocIdx = -2;

	CString strSourcePath = dbNetwork.GetFolderPathSyssetCashRSP( nNetworkIdx ); 
	strSourcePath += "\\";
	
	CString strDestPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
	strDestPath += "\\";

	CString strPendingFilename;
	::GetTermSaleFilename( CASHRSP_SALES_PENDING, -1, strPendingFilename );
	strPendingFilename.MakeUpper();

	CString strPMSFilename;
	::GetTermSaleFilename( CASHRSP_SALES_PMS, -1, strPMSFilename );
	strPMSFilename.MakeUpper();

	CString strDepositFilename;
	::GetTermSaleFilename( CASHRSP_SALES_DEPOSIT, -1, strDepositFilename );
	strDepositFilename.MakeUpper();

	CString strEODDateFilename;
	::GetTermSaleFilename( CASHRSP_SALES_EOD, -1, strEODDateFilename );
	strEODDateFilename.MakeUpper();

	CFileFind FileFinder;	
	bool bWorking = ( FileFinder.FindFile ( strSourcePath + "*.*" ) != 0 );

	CSSFile fileLock;
	CArray<bool,bool> arrayRetryFlag;
	CStringArray arraySalesFilename;
	CStringArray arrayDepositFilename;
	CStringArray arrayEODDateFilename;
	
	int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
	{
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		nConnectionType = CONNECTION_TYPE_SPTBOOK_NONE;
		break;

	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		nConnectionType = CONNECTION_TYPE_SMARTENT_NONE;
		break;
	}

	while (bWorking)   
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if (FileFinder.IsDots() == TRUE)
		{
			continue;
		}

		//WE ONLY NEED TO CHECK FILES
		if (FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = FileFinder.GetFileName();

		int nLength = strFilename.GetLength();	
		if (nLength < 5)
		{
			continue;
		}

		strFilename.MakeUpper();

		CString strName = strFilename.Left( nLength - 4 );
		CString strDot ( strFilename.GetAt ( nLength - 4 ) );
		CString strExt = strFilename.Right(3);

		if (strDot != ".")
		{
			continue;
		}

		bool bRetry = FALSE;
		int nFileType = CASHRSP_SALES_UNKNOWN;

		switch( nConnectionType )
		{
		case CONNECTION_TYPE_SPTBOOK_NONE:
			{
				if (strName.GetLength() == 18)
				{
					if (strName.Left(3) == "SB_")
					{
						if (::TestNumeric(strName.Mid(3, 8)) == TRUE)
						{
							if (strName.Mid(11, 1) == "_")
							{
								if (::TestNumeric(strName.Mid(12, 6)) == TRUE)
								{
									nFileType = CASHRSP_SALES_PENDING;
								}
							}
						}
					}
				}
			}
			break;

		case CONNECTION_TYPE_SMARTENT_NONE:
			{
				if (strName.GetLength() == 18)
				{
					if (strName.Left(3) == "SE_")
					{
						if (::TestNumeric(strName.Mid(3, 8)) == TRUE)
						{
							if (strName.Mid(11, 1) == "_")
							{
								if (::TestNumeric(strName.Mid(12, 6)) == TRUE)
								{
									nFileType = CASHRSP_SALES_PENDING;
								}
							}
						}
					}
				}
			}
			break;

		case CONNECTION_TYPE_STANDARD_NONE:
		default:
			{
				if (strName == strPendingFilename)
				{
					nFileType = CASHRSP_SALES_PENDING;
				}
				else if (strName == strDepositFilename)
				{
					nFileType = CASHRSP_SALES_DEPOSIT;
				}
				else if (strName == strEODDateFilename)
				{
					nFileType = CASHRSP_SALES_EOD;
				}
				else if ((SysInfo.IsPMSSystem() == TRUE) && (strName == strPMSFilename))
				{
					nFileType = CASHRSP_SALES_PMS;
				}

				if ( CASHRSP_SALES_UNKNOWN == nFileType )
				{
					if (DealerFlags.GetAutoRetryTermFileImportFlag() == TRUE)
					{
						if (strName.GetLength() == strPendingFilename.GetLength() + 16)
						{
							if (strName.Right(strName.GetLength() - 16) == strPendingFilename)
							{
								if (::TestNumeric(strName.Left(8)) == TRUE)
								{
									if (strName.Mid(8, 1) == "_")
									{
										if (::TestNumeric(strName.Mid(9, 6)) == TRUE)
										{
											if (strName.Mid(15, 1) == "_")
											{
												nFileType = CASHRSP_SALES_PENDING;
												bRetry = TRUE;
											}
										}
									}
								}
							}
						}
					}
				}
					
				if ( CASHRSP_SALES_UNKNOWN == nFileType )
				{
					int nSSFileType = ValidateSSFileTransactionFilename( strName );

					switch( nSSFileType )
					{
					case CASHRSP_SALES_DEPOSIT:
					case CASHRSP_SALES_EOD:
						if ( ::TestNumeric( strExt ) == TRUE )
						{
							CString strSource = "";
							strSource += strSourcePath;
							strSource += strFilename;

							CString strDest = "";
							strDest += strSourcePath;
							strDest += ( CASHRSP_SALES_DEPOSIT == nSSFileType ) ? "TERMDPST" : "TERMEOD";
							strDest += strDot;
							strDest += strExt;

							if (Append(strSource, strDest) == TRUE)
							{
								CFileRemover FileRemover(strSource);
							}
						}
					}
				}
			}
			break;
		}
		
		if (CASHRSP_SALES_UNKNOWN == nFileType)
		{
			continue;
		}
		
		if (::TestNumeric(strExt) == FALSE)
		{
			continue;
		}

		if ( ( arraySalesFilename.GetSize() == 0 ) && ( arrayDepositFilename.GetSize() == 0 ) && ( arrayEODDateFilename.GetSize() == 0 ) )
		{
			CFilenameUpdater FnUp( SysFiles::NetworkCommsLock, nNetworkIdx );
			if (fileLock.Open(FnUp.GetFilenameToUse(), "wb") == FALSE)
			{
				return FALSE;
			}
		}

		switch( nFileType )
		{
		case CASHRSP_SALES_DEPOSIT:
			arrayDepositFilename.Add( strFilename );
			break;

		case CASHRSP_SALES_EOD:
			arrayEODDateFilename.Add( strFilename );
			arraySalesFilename.Add( strFilename );
			arrayRetryFlag.Add( bRetry );
			break;

		case CASHRSP_SALES_PENDING:
		default:
			arraySalesFilename.Add( strFilename );
			arrayRetryFlag.Add( bRetry );
			break;
		}
	}

	if ((arraySalesFilename.GetSize() == 0) && (arrayDepositFilename.GetSize() == 0) && (arrayEODDateFilename.GetSize() == 0))
	{
		return TRUE;
	}

	if ( LockManager.GetLockResult() == LOCK_RESULT_INACTIVE )
	{
		if ( LockManager.GetLock() == FALSE )
		{
			fileLock.Close();
			return FALSE;
		}
	}

	StatusProgress.Lock();

	//PROCESS EOD DATES BEFORE VARIANCE TOTALS
	//PROCESSING VARIANCE TOTALS WILL RENAME AND THEN DELETE FILE
	//POSSIBLE DUPLICATION OF EOD DATE RECORD DOES NOT MATTER
	for (int n = 0; n < arrayEODDateFilename.GetSize(); n++)
	{
		ProcessEODDateFile(strSourcePath, strDestPath, arrayEODDateFilename.GetAt(n), nNetworkIdx);
	}

	for ( int n = 0; ( n < arraySalesFilename.GetSize() ) && ( GetCashRSPVersionCheckerFailFlag() == FALSE ); n++ )
	{
		CString strFilename = arraySalesFilename.GetAt(n);
		strFilename.MakeUpper();
		
		int nExt = atoi( strFilename.Right(3) );

		int nServerDbIdx, nServerLocIdx;
		GetServerFileInfo( nNetworkIdx, nExt, nServerDbIdx, nServerLocIdx );
		OpenServerFile( nServerDbIdx, nServerLocIdx );

		int nFileDbIdx, nFileLocIdx;
		GetDbAndLocIdx( nNetworkIdx, nExt, nFileDbIdx, nFileLocIdx );

		int nProcessType = arrayRetryFlag.GetAt(n) ? CASHRSP_PROCESSTYPE_RETRY : CASHRSP_PROCESSTYPE_NEWSALE;

		m_arrayDepartments.Open( DataManager.GetFilePathDepartment( nFileDbIdx ), DB_READONLY );
		m_arrayPromotions.Open( DataManager.GetFilePathPromotion( nFileDbIdx ), DB_READONLY );
		
		if (DealerFlags.Get15DigitKludgeDeptGroupFlag() == TRUE)
		{
			m_arrayPluItems.Open(DataManager.GetFilePathPlu(nFileDbIdx), DB_READONLY);
		}

		ProcessCashRSPFile ( strSourcePath, strDestPath, strFilename, nNetworkIdx, CASHRSP_SALES_PENDING, nProcessType, StatusDisplay );
		
		m_arrayDepartments.Close();
		m_arrayPromotions.Close();

		if (DealerFlags.Get15DigitKludgeDeptGroupFlag() == TRUE)
		{
			m_arrayPluItems.Close();
		}
	}

	DataManagerNonDb.CloseServer( info );

	for (int n = 0; n < arrayDepositFilename.GetSize(); n++)
	{
		ProcessDepositFile(strSourcePath, strDestPath, arrayDepositFilename.GetAt(n), nNetworkIdx);
	}

	StatusProgress.Unlock();

	fileLock.Close();

	return TRUE;
}

/**********************************************************************/

void CCashRSPImporter::RebuildLocationSales( int nLocIdx, CString& strFrom, CString& strTo )
{
	int nNetworkIdx = 0;
	if ( dbNetwork.FindNetworkByNumber( dbLocation.GetNetworkNo( nLocIdx ), nNetworkIdx ) == TRUE )
	{
		int nDbIdx = 0;
		if ( dbDatabase.FindDatabaseByNumber( dbLocation.GetDbNo( nLocIdx ), nDbIdx ) == TRUE )
		{
			m_BatchInfo.m_strRebuildDateFrom = strFrom;
			m_BatchInfo.m_strRebuildDateTo = strTo;

			CString strPath = dbNetwork.GetFolderPathData ( nNetworkIdx );
			strPath += "\\";

			CString strLegacyFilename, strTradingFilename, strTrainingFilename;
			::GetTermSaleFilename( CASHRSP_OBSOLETE_TERMSAVE, -1, strLegacyFilename );
			::GetTermSaleFilename( CASHRSP_SALES_TRADING, -1, strTradingFilename );
			strLegacyFilename.MakeUpper();
			strTradingFilename.MakeUpper();

			CReportConsolidationArray<CSortedIntItem> arrayTNo;
			for( int n = 0; n < dbLocation.GetTerminalCount( nLocIdx ); n++ )
			{
				CSortedIntItem item;
				item.m_nItem = dbLocation.GetTNo( nLocIdx, n );
				arrayTNo.Consolidate( item );
			}
			
			CFileFind FileFinder;	
			bool bWorking = ( FileFinder.FindFile ( strPath + "*.*" ) != 0 );

			while ( (bWorking) && ( GetCashRSPVersionCheckerFailFlag() == FALSE ) )  
			{
				( bWorking = FileFinder.FindNextFile() != 0 );

				//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
				if (FileFinder.IsDots() == TRUE)
				{
					continue;
				}

				//WE ONLY NEED TO CHECK FILES
				if (FileFinder.IsDirectory() == TRUE)
				{
					continue;
				}

				CString strFilename = FileFinder.GetFileName();
				int nLength = strFilename.GetLength();
					
				if (nLength < 5)
				{
					continue;
				}

				CString strName = strFilename.Left( nLength - 4 );
				CString strDot ( strFilename.GetAt ( nLength - 4 ) );
				CString strExt = strFilename.Right(3);

				if (strDot != ".")
				{
					continue;
				}

				strName.MakeUpper();
				
				int nFileType = CASHRSP_SALES_UNKNOWN;

				if (strName == strLegacyFilename)
				{
					nFileType = CASHRSP_OBSOLETE_TERMSAVE;
				}
				else if (strName == strTradingFilename)
				{
					nFileType = CASHRSP_SALES_TRADING;
				}

				if (nFileType == CASHRSP_SALES_UNKNOWN)
				{
					continue;
				}

				if (::TestNumeric(strExt) == FALSE)
				{
					continue;
				}

				int nExt = atoi( strExt );

				{
					int nPos = 0;
					CSortedIntItem item;
					item.m_nItem = nExt;
					if (arrayTNo.Find(item, nPos) == FALSE)
					{
						continue;
					}
				}

				CString strLocation;
				strLocation.Format( "(%s, TNo %3.3d)", 
					dbLocation.GetName( nLocIdx ),
					nExt);

				StatusProgress.Lock( TRUE, "Rebuilding sales history" );
				StatusProgress.SetDialogText( strLocation );

				CCashRSPImporterStatusDisplay StatusDisplay;
				m_BatchInfo.m_arrayRebuildHeaders.RemoveAll();
				m_arrayDepartments.Open( DataManager.GetFilePathDepartment( nDbIdx ), DB_READONLY );
				m_arrayPromotions.Open( DataManager.GetFilePathPromotion( nDbIdx ), DB_READONLY );
				
				if (DealerFlags.Get15DigitKludgeDeptGroupFlag() == TRUE)
				{
					m_arrayPluItems.Open(DataManager.GetFilePathPlu(nDbIdx), DB_READONLY);
				}

				/*
				{
					CSSFile fileDupes;
					fileDupes.Open("E:\\dupes.txt", "ab");
					fileDupes.WriteLine(strPath + strFilename);
				}
				*/

				ProcessCashRSPFile ( strPath, strPath, strFilename, nNetworkIdx, nFileType, CASHRSP_PROCESSTYPE_REBUILD, StatusDisplay );	
				
				m_arrayDepartments.Close();
				m_arrayPromotions.Close();

				if (DealerFlags.Get15DigitKludgeDeptGroupFlag() == TRUE)
				{
					m_arrayPluItems.Close();
				}

				StatusProgress.Unlock();
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessCashRSPFile(const char* szSourcePath, const char* szDestPath, const char* szFilename, int nNetwkIdx, int nFileType, int nProcessType, CCashRSPImporterStatusDisplay& StatusDisplay)
{
	CString strFilename = szFilename;

	int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	switch (dbNetwork.GetConnectionType(nNetwkIdx))
	{
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		nConnectionType = CONNECTION_TYPE_SPTBOOK_NONE;
		break;

	case CONNECTION_TYPE_SMARTENT_NONE:
	case CONNECTION_TYPE_SMARTENT_WEB:
		nConnectionType = CONNECTION_TYPE_SMARTENT_NONE;
		break;
	}

	//SET OPTIONS RELATING TO THE BATCH OF SALES FROM THIS FILE
	m_BatchInfo.m_strLocalPath = szDestPath;
	m_BatchInfo.m_nFileType = nFileType;
	m_BatchInfo.m_nProcessType = nProcessType;
	m_BatchInfo.m_nNetworkIdx = nNetwkIdx;
	m_BatchInfo.m_strTNoOriginal = strFilename.Right(3);

	//CREATE AS LOCAL OBJECTS SO DESTRUCTORS WILL CLOSE OPEN FILES
	CCashRSPImporterOutputFile OutputFileDailyTerm(CASHRSP_DAILYFILE_TERM, m_BatchInfo, m_TransactionInfo);
	CCashRSPImporterOutputFile OutputFileDailySales(CASHRSP_DAILYFILE_SALES_TRAN, m_BatchInfo, m_TransactionInfo);
	CCashRSPImporterOutputFile OutputFileDailyReasons(CASHRSP_DAILYFILE_REASONS_TRAN, m_BatchInfo, m_TransactionInfo);
	CCashRSPImporterOutputFile OutputFileDailyAccount(CASHRSP_DAILYFILE_ACCOUNT, m_BatchInfo, m_TransactionInfo);
	CCashRSPImporterOutputFile OutputFileDailyItemPay(CASHRSP_DAILYFILE_ITEMPAY, m_BatchInfo, m_TransactionInfo);
	CCashRSPImporterOutputFile OutputFileStockSales(CASHRSP_STOCKFILE_SALES, m_BatchInfo, m_TransactionInfo);
	CCashRSPImporterOutputFile OutputFileStockDelivery(CASHRSP_STOCKFILE_DELIVERY, m_BatchInfo, m_TransactionInfo);
	CCashRSPImporterOutputFile OutputFileNonDuplicates(CASHRSP_TERMFILE_NONDUPLICATE, m_BatchInfo, m_TransactionInfo);

	m_pOutputFileDailyTerm = &OutputFileDailyTerm;
	m_pOutputFileDailySales = &OutputFileDailySales;
	m_pOutputFileDailyReasons = &OutputFileDailyReasons;
	m_pOutputFileDailyAccount = &OutputFileDailyAccount;
	m_pOutputFileDailyItemPay = &OutputFileDailyItemPay;
	m_pOutputFileStockSales = &OutputFileStockSales;
	m_pOutputFileStockDelivery = &OutputFileStockDelivery;
	m_pOutputFileTermNonDuplicates = &OutputFileNonDuplicates;

	//CREATE FOLDERS FOR APPARENT STOCK PENDING FILES
	::CreateSubdirectory(dbNetwork.GetFolderPathSysset(m_BatchInfo.m_nNetworkIdx));
	::CreateSubdirectory(dbNetwork.GetFolderPathSyssetStock(m_BatchInfo.m_nNetworkIdx));

	//OPEN CONSOLIDATED LOG FILES FOR APPEND UNLESS WE ARE REBUILDING DAILY SALES
	CSSFile fileTermTrade;
	CSSFile fileTermTimeAttend;

	switch (m_BatchInfo.m_nProcessType)
	{
	case CASHRSP_PROCESSTYPE_NEWSALE:
	case CASHRSP_PROCESSTYPE_RETRY:
	{
		CString strLocalPath = m_BatchInfo.m_strLocalPath;
		if ((strLocalPath.GetLength() > 0) && (strLocalPath.Right(1) != "\\"))
		{
			strLocalPath += "\\";
		}

		CString strTradingFilename;
		::GetTermSaleFilename(CASHRSP_SALES_TRADING, -1, strTradingFilename);
		CString strTradingPath = strLocalPath;
		strTradingPath += strTradingFilename;
		strTradingPath += strFilename.Right(4);

		CString strTimeAttendFilename;
		::GetTermSaleFilename(CASHRSP_SALES_TIMEATTEND, -1, strTimeAttendFilename);
		CString strTimeAttendPath = strLocalPath;
		strTimeAttendPath += strTimeAttendFilename;
		strTimeAttendPath += strFilename.Right(4);

		if (fileTermTrade.Open(strTradingPath, "ab") == FALSE)
		{
			return;
		}

		if (fileTermTimeAttend.Open(strTimeAttendPath, "ab") == FALSE)
		{
			return;
		}
	}
	break;
	}

	//SCAN INCOMING FILE FOR UNKNOWN CASHRSP VERSION NUMBER
	//DO THIS BEFORE RENAMING FILE SO THAT RETRY WILL BE POSSIBLE LATER
	CString strOriginalFilename = szSourcePath;
	strOriginalFilename += strFilename;

	switch (m_BatchInfo.m_nProcessType)
	{
	case CASHRSP_PROCESSTYPE_REBUILD:
		break;

	default:
	{
		CSSFile InputFile;
		if (InputFile.Open(strOriginalFilename, "rb") == FALSE)
		{
			return;
		}

		CString strBuffer = "";
		while (InputFile.ReadString(strBuffer) == TRUE)
		{
			StatusProgress.SetPos(InputFile);
			CCSVEposTermLine csvIn(strBuffer);

			switch (csvIn.GetLineTypeInt())
			{
			case CASHRSP_LINETYPE_VERSION:
			{
				int nVersionNo = csvIn.GetVersionNo();

				if (CCashRSPVersionChecker::CheckCashRSPVersion(nVersionNo, 14, TRUE) == FALSE)
				{
					m_TransactionInfo.m_CashRSPVersionChecker.SetFailedCashRSPVersion(nVersionNo);
					return;
				}
			}
			break;

			case CASHRSP_LINETYPE_APPLICATION:
			{
				int nAppType = csvIn.GetApplicationType();

				if (CCashRSPVersionChecker::CheckAppType(nAppType, 1, TRUE) == FALSE)
				{
					m_TransactionInfo.m_CashRSPVersionChecker.SetFailedAppType(nAppType);
					return;
				}
			}
			break;
			}
		}
	}
	}

	//NOTE LENGTH OF ORIGINAL FILE
	CFileStatus FileStatus1;
	CFile::GetStatus(strOriginalFilename, FileStatus1);

	//RENAME FILE TO AVOID DUPLICATION UNLESS WE ARE REBUILDING DAILY SALES
	CString strFilenameToProcess = strOriginalFilename;
	if (CASHRSP_PROCESSTYPE_NEWSALE == m_BatchInfo.m_nProcessType)
	{
		if (CONNECTION_TYPE_STANDARD_NONE == nConnectionType)
		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();

			CString strInfo;
			strInfo.Format("%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d_",
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				timeNow.GetHour(),
				timeNow.GetMinute(),
				timeNow.GetSecond());

			strFilenameToProcess = szSourcePath;
			strFilenameToProcess += strInfo;
			strFilenameToProcess += strFilename;
		}
		else
		{
			strFilenameToProcess = strOriginalFilename + ".tmp";
		}

		//GIVE UP IF UNABLE TO RENAME FILE
		if (rename(strOriginalFilename, strFilenameToProcess) != 0)
		{
			return;
		}
	}

	//GIVE UP IF UNABLE TO OPEN RENAMED FILE FOR READING
	CSSFile CashRSPInputFile;
	if (CashRSPInputFile.Open(strFilenameToProcess, "rb") == FALSE)
	{
		return;
	}

	//GIVE UP IF LENGTH OF RENAMED FILE IS NOT SAME AS LENGTH OF ORIGINAL FILE
	CFileStatus FileStatus2;
	CFile::GetStatus(strFilenameToProcess, FileStatus2);
	if (FileStatus1.m_size != FileStatus2.m_size)
	{
		return;
	}

	//RESET CASHRSP VERSION INFORMATION FOR NEXT TRANSACTION
	m_TransactionInfo.m_CashRSPVersionChecker.ResetTransactionVersions();
	m_TransactionInfo.m_CashRSPVersionChecker.ResetApplicationTypes();

	m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_NONE, "");

	bool bDepoaitSkip = FALSE;
	CString strBuffer = "";
	__int64 nFileLength = CashRSPInputFile.GetLength();

	bool bEODMode = FALSE;

	CCashRSPSpooler CashRSPSpooler(CashRSPInputFile);

	bool bFirstTimeRound = TRUE;

	while ((TRUE == bFirstTimeRound) || (CashRSPSpooler.GotManualLines() == TRUE))
	{
		bFirstTimeRound = FALSE;

		while (CashRSPSpooler.ReadString(strBuffer) == TRUE)
		{
			//FIDDLE
			if (strBuffer == "$QUIT")
			{
				break;
			}

			StatusProgress.SetPos(CashRSPInputFile);

			if (StatusDisplay.CanDisplayStatus() == TRUE)
			{
				CString strStatus;
				strStatus.Format("Processing %s, %d%% of %I64d bytes", (const char*)strFilename, CashRSPInputFile.GetPercentPosition(), nFileLength);
				StatusDisplay.DisplayStatus(strStatus);
			}

			CCSVEposTermLine csvIn(strBuffer);

			int nCASHRSPLineType = csvIn.GetLineTypeInt();
			bool bDuplicate = FALSE;

			switch (nCASHRSPLineType)
			{
			case CASHRSP_LINETYPE_EODHEADER:
				FinaliseTransaction(nConnectionType, CashRSPSpooler);
				m_TransactionInfo.m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_EOD, strBuffer);
				m_TransactionInfo.ProcessTransactionHeader(csvIn, m_BatchInfo, CashRSPSpooler, m_bRebuildRemoveDuplicates);
				bDuplicate = m_TransactionInfo.m_bGotDuplicate;
				bEODMode = TRUE;
				break;

			case CASHRSP_LINETYPE_TRAN_NORMAL:
				if (FALSE == bEODMode)
				{
					FinaliseTransaction(nConnectionType, CashRSPSpooler);
					m_TransactionInfo.m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
					m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_SALE, strBuffer);
					m_TransactionInfo.ProcessTransactionHeader(csvIn, m_BatchInfo, CashRSPSpooler, m_bRebuildRemoveDuplicates);
					bDuplicate = m_TransactionInfo.m_bGotDuplicate;
				}
				break;

			case CASHRSP_LINETYPE_TRAN_VOID:
				FinaliseTransaction(nConnectionType, CashRSPSpooler);
				m_TransactionInfo.m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_VOID, strBuffer);
				m_TransactionInfo.ProcessTransactionHeader(csvIn, m_BatchInfo, CashRSPSpooler, m_bRebuildRemoveDuplicates);
				bDuplicate = m_TransactionInfo.m_bGotDuplicate;
				bEODMode = FALSE;
				break;

			case CASHRSP_LINETYPE_WASTAGE:
				FinaliseTransaction(nConnectionType, CashRSPSpooler);
				m_TransactionInfo.m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_WASTE, strBuffer);
				m_TransactionInfo.ProcessTransactionHeader(csvIn, m_BatchInfo, CashRSPSpooler, m_bRebuildRemoveDuplicates);
				bDuplicate = m_TransactionInfo.m_bGotDuplicate;
				bEODMode = FALSE;
				break;

			case CASHRSP_LINETYPE_NOSALE:
				FinaliseTransaction(nConnectionType, CashRSPSpooler);
				m_TransactionInfo.m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_NOSALE, "");
				m_TransactionInfo.ProcessTransactionHeader(csvIn, m_BatchInfo, CashRSPSpooler, m_bRebuildRemoveDuplicates);
				bDuplicate = m_TransactionInfo.m_bGotDuplicate;

				if (FALSE == bDuplicate)
				{
					m_TransactionInfo.m_bufferDailyLogTerm.Add(strBuffer);
					FinaliseTransaction(nConnectionType, CashRSPSpooler);
				}

				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_NONE, "");
				bEODMode = FALSE;
				break;

			case CASHRSP_LINETYPE_AGEVERIFY:
				FinaliseTransaction(nConnectionType, CashRSPSpooler);
				m_TransactionInfo.m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_AGEVERIFY, "");
				m_TransactionInfo.ProcessTransactionHeader(csvIn, m_BatchInfo, CashRSPSpooler, m_bRebuildRemoveDuplicates);
				bDuplicate = m_TransactionInfo.m_bGotDuplicate;

				if (FALSE == bDuplicate)
				{
					m_TransactionInfo.m_bufferDailyLogTerm.Add(strBuffer);
					FinaliseTransaction(nConnectionType, CashRSPSpooler);
				}

				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_NONE, "");
				bEODMode = FALSE;
				break;

			case CASHRSP_LINETYPE_ATTENDANCE:
				FinaliseTransaction(nConnectionType, CashRSPSpooler);
				m_TransactionInfo.m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
				m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_NONE, "");
				bEODMode = FALSE;
				break;
			}

			//IGNORE OPEN TRANSACTIONS AT EOD
			if (TRUE == bEODMode)
			{
				switch (nCASHRSPLineType)
				{
				case CASHRSP_LINETYPE_EODHEADER:
				case CASHRSP_LINETYPE_EODVAR:
				case CASHRSP_LINETYPE_EODVARTOT:
				case CASHRSP_LINETYPE_EODVARFX:
				case CASHRSP_LINETYPE_EODVARFXTOT:
				case CASHRSP_LINETYPE_VERSION:
				case CASHRSP_LINETYPE_APPLICATION:
					break;

				case CASHRSP_LINETYPE_EODEND:
					bEODMode = FALSE;
					break;

				default:
					continue;
				}
			}

			if ((TRUE == bDuplicate) && (TRUE == m_TransactionInfo.m_bProcessTransaction))
			{
				m_nDuplicatedTransactions++;
				m_TransactionInfo.m_bProcessTransaction = FALSE;
			}

			if ((TRUE == m_TransactionInfo.m_bProcessTransaction) &&
				(CASHRSP_TRANSACTION_TYPE_NONE != m_TransactionInfo.m_nTransactionType) &&
				(CASHRSP_LINETYPE_VERSION != nCASHRSPLineType) &&
				(CASHRSP_LINETYPE_APPLICATION != nCASHRSPLineType))
			{
				{
					CString strVersion = m_TransactionInfo.m_CashRSPVersionChecker.GetCurrentTransactionVersionLine();

					if (strVersion != "")
					{
						m_TransactionInfo.m_bufferDailyLogTerm.Add(strVersion);
					}
				}

				{
					CString strAppName = m_TransactionInfo.m_CashRSPVersionChecker.GetCurrentTransactionAppLine();

					if (strAppName != "")
					{
						m_TransactionInfo.m_bufferDailyLogTerm.Add(strAppName);
					}
				}

				m_TransactionInfo.m_CashRSPVersionChecker.ResetCurrentLines();

				if (CASHRSP_LINETYPE_COMMENT_GENERAL != nCASHRSPLineType)
				{
					m_TransactionInfo.m_bufferDailyLogTerm.Add(strBuffer);

					int nPos = m_TransactionInfo.m_bufferDailyLogTerm.GetSize() - 1;

					switch (m_TransactionInfo.m_nTransactionType)
					{
					case CASHRSP_TRANSACTION_TYPE_SALE:
					case CASHRSP_TRANSACTION_TYPE_WASTE:
						switch (nCASHRSPLineType)
						{
						case CASHRSP_LINETYPE_DISCOUNT_ITEM:
							m_TransactionInfo.m_arrayLinePosDiscountItem.Add(nPos);
							break;

						case CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL:
						case CASHRSP_LINETYPE_DISCOUNTTAX_SUBTOTAL:
							m_TransactionInfo.m_arrayLinePosDiscountSubtotal.Add(nPos);
							break;

						case CASHRSP_LINETYPE_DISCOUNT_PROMO:
							m_TransactionInfo.m_arrayLinePosDiscountPromo.Add(nPos);
							break;

						case CASHRSP_LINETYPE_DISCOUNT_MIXMATCH:
							if ((csvIn.GetMixMatchDiscountType() != MIXMATCH_ACTION_FREE_ITEM) || (DealerFlags.GetCOFAllDiscountFlag() == FALSE))
							{
								m_TransactionInfo.m_arrayLinePosDiscountMixMatch.Add(nPos);
							}
							else
							{
								m_TransactionInfo.m_bufferDailyLogTerm.RemoveAt(nPos);
							}
							break;

						case CASHRSP_LINETYPE_MIXMATCH_USE:
							ProcessMixMatchUseLine(csvIn);
							break;

						case CASHRSP_LINETYPE_DISCOUNT_LOYALTY:
						case CASHRSP_LINETYPE_DISCOUNTTAX_LOYALTY:
							m_TransactionInfo.m_arrayLinePosDiscountLoyalty.Add(nPos);
							break;

						case CASHRSP_LINETYPE_DISCOUNT_VOID:
							m_TransactionInfo.m_arrayLinePosDiscountVoid.Add(nPos);
							break;

						case CASHRSP_LINETYPE_ITEM:
							m_TransactionInfo.m_arrayLinePosItem.Add(nPos);

							if ( DealerFlags.GetCOFAllDiscountFlag() == TRUE )
							{
								int nCashRSPVersionNo = m_TransactionInfo.m_CashRSPVersionChecker.GetCurrentTransactionVersionNo();
								if (csvIn.GetItemMixMatchType(nCashRSPVersionNo) == MIXMATCH_ACTION_FREE_ITEM)
								{
									CCSV MyMMDiscLine;
									MyMMDiscLine.Add("$MMDISC");
									MyMMDiscLine.Add(csvIn.GetItemPluNo());
									MyMMDiscLine.Add(csvIn.GetItemMixMatchNumber(nCashRSPVersionNo));
									MyMMDiscLine.Add("Cheapest One Free");
									MyMMDiscLine.Add(-csvIn.GetItemLinePrice());
									MyMMDiscLine.Add(1);
									MyMMDiscLine.Add(-csvIn.GetItemLinePrice());
									MyMMDiscLine.Add(MIXMATCH_ACTION_FREE_ITEM);
									MyMMDiscLine.Add(0);
									MyMMDiscLine.Add(csvIn.GetItemAnalysisCategory(nCashRSPVersionNo));
									MyMMDiscLine.Add(csvIn.GetItemTaxBand());

									m_TransactionInfo.m_bufferDailyLogTerm.Add(MyMMDiscLine.GetLine());
									m_TransactionInfo.m_arrayLinePosDiscountMixMatch.Add(++nPos);
								}
							}
							break;

						case CASHRSP_LINETYPE_PAYMENT:
						case CASHRSP_LINETYPE_DEPOSIT_PMS:
							m_TransactionInfo.m_arrayLinePosPayment.Add(nPos);
							break;

						case CASHRSP_LINETYPE_PAIDIO:
							m_TransactionInfo.m_bIsPaidIO = TRUE;
							break;
						}
						break;

					case CASHRSP_TRANSACTION_TYPE_VOID:
						switch (nCASHRSPLineType)
						{
						case CASHRSP_LINETYPE_ITEM:
							m_TransactionInfo.m_arrayLinePosItem.Add(nPos);
							break;
						}
						break;
					}
				}
			}

			//ORIGINAL VERSION OF LINE GOES INTO MASTER TRANSACTION FILE
			switch (m_BatchInfo.m_nProcessType)
			{
			case CASHRSP_PROCESSTYPE_NEWSALE:
			case CASHRSP_PROCESSTYPE_RETRY:
			{
				if (CASHRSP_LINETYPE_ATTENDANCE != nCASHRSPLineType)
				{
					if (FALSE == m_TransactionInfo.m_bManualMode)
					{
						fileTermTrade.WriteLine(strBuffer);
					}
				}
				else
				{
					CCSV csv(strBuffer);
					csv.SetAt(0, "$A");
					fileTermTimeAttend.WriteLine(csv.GetLine());
				}
			}
			break;
			}

			switch (nCASHRSPLineType)
			{
			case CASHRSP_LINETYPE_VERSION:
				m_TransactionInfo.m_CashRSPVersionChecker.ProcessCashRSPVersionLine(csvIn, strBuffer);
				bEODMode = FALSE;
				break;

			case CASHRSP_LINETYPE_APPLICATION:
				m_TransactionInfo.m_CashRSPVersionChecker.ProcessAppTypeLine(csvIn, strBuffer);
				bEODMode = FALSE;
				break;

			case CASHRSP_LINETYPE_COMMENT_GENERAL:
				break;

			default:
				m_TransactionInfo.m_CashRSPVersionChecker.ResetPendingTransactionInfo();
				break;
			}

			if (m_TransactionInfo.m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == TRUE)
			{
				break;
			}
		}

		FinaliseTransaction(nConnectionType, CashRSPSpooler);
		m_TransactionInfo.ResetTransaction(CASHRSP_TRANSACTION_TYPE_NONE, "");
	}

	CashRSPSpooler.CloseFile();

	switch (m_BatchInfo.m_nProcessType)
	{
	case CASHRSP_PROCESSTYPE_NEWSALE:
	case CASHRSP_PROCESSTYPE_RETRY:
		CFileRemover FileRemover(strFilenameToProcess);
		break;
	}
}

/**********************************************************************/

void CCashRSPImporter::FinaliseTransaction( int nConnectionType, CCashRSPSpooler& CashRSPSpooler)
{		
	if ( TRUE == m_TransactionInfo.m_bProcessTransaction )
	{
		if ( Sysset.IsNobbledDemo() == TRUE )
		{
			if ( ( rand() % 10 ) == 5 )
			{
				int nBufferSize = m_TransactionInfo.m_bufferDailyLogTerm.GetSize();
				
				if ( nBufferSize != 0 )
				{
					if ( m_pOutputFileDailyTerm -> OpenFile() == TRUE )
					{
						bool bStop = FALSE;

						for ( int n = 0; ( n < nBufferSize ) && ( FALSE == bStop ); n++ )
						{
							CString strLine = m_TransactionInfo.m_bufferDailyLogTerm.GetAt(n);
							
							CCSVEposTermLine csvIn ( strLine );

							switch( csvIn.GetLineTypeInt() )
							{
							case CASHRSP_LINETYPE_VERSION:
							case CASHRSP_LINETYPE_APPLICATION:
							case CASHRSP_LINETYPE_TRAN_NORMAL:
							case CASHRSP_LINETYPE_TRAN_VOID:
							case CASHRSP_LINETYPE_WASTAGE:
							case CASHRSP_LINETYPE_NOSALE:
							case CASHRSP_LINETYPE_AGEVERIFY:
							case CASHRSP_LINETYPE_ATTENDANCE:
							case CASHRSP_LINETYPE_EODHEADER:
								m_pOutputFileDailyTerm -> WriteLine( strLine );
								break;

							default:
								bStop = TRUE;
								break;
							}
						}
	
						m_pOutputFileDailyTerm -> WriteLine( "$,NOTICE,UNLICENSED DEMO : TRANSACTION DETAIL REMOVED" );
					}
				}

				return;
			}
		}

		if ((DealerFlags.GetIgnoreUnpaidTransactionsFlag() == TRUE) && (m_TransactionInfo.m_nTransactionType != CASHRSP_TRANSACTION_TYPE_VOID))
		{
			if (m_TransactionInfo.m_arrayLinePosPayment.GetSize() == 0)
			{
				if (m_TransactionInfo.m_arrayLinePosItem.GetSize() != 0)
				{
					for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogTerm.GetSize(); n++)
					{
						CString strLine = m_TransactionInfo.m_bufferDailyLogTerm.GetAt(n);

						CString strTran = strLine.Left(6);
						strTran.MakeUpper();

						if (strTran == "$TRAN,")
						{
							CCSV csvTran(strLine);

							if (csvTran.GetInt(6) != 0)
							{
								return;
							}
							else
							{
								break;
							}
						}
					}
				}
			}
		}
	
		bool bGotDuplicate = FALSE;

		if ( TRUE == m_TransactionInfo.m_bIsPaidIO )
		{
			WriteDailyTermFile( bGotDuplicate );
		}
		else
		{
			switch( m_TransactionInfo.m_nTransactionType )
			{
			case CASHRSP_TRANSACTION_TYPE_AGEVERIFY:
			case CASHRSP_TRANSACTION_TYPE_NOSALE:
			case CASHRSP_TRANSACTION_TYPE_EOD:
				WriteDailyTermFile( bGotDuplicate );
				break;

			case CASHRSP_TRANSACTION_TYPE_SALE:
			case CASHRSP_TRANSACTION_TYPE_VOID:
			case CASHRSP_TRANSACTION_TYPE_WASTE:
				KludgeDailyItemLines();
				BuildItemBuffer();

				if (TRUE == m_TransactionInfo.m_bIsDelivery)
				{
					ProcessItemBufferDelivery();
					WriteDailyTermFile(bGotDuplicate);

					if (FALSE == bGotDuplicate)
					{
						WritePendingStockFiles(nConnectionType);
					}
				}
				else
				{
					bool bDiscount = ( CASHRSP_TRANSACTION_TYPE_SALE == m_TransactionInfo.m_nTransactionType );
				
					ProcessPaymentsBuffer(CashRSPSpooler);
					ProcessDiscountVoidBuffer();
					ProcessItemBufferSale();

					if ( TRUE == bDiscount )
					{
						PrepareDiscounts();
						ProcessDiscounts();
					}

					WriteDailyTermFile( bGotDuplicate );

					if ( FALSE == bGotDuplicate )
					{
						WriteDailySalesFiles();
						WritePendingStockFiles( nConnectionType );
					}
				}
				break;
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::KludgeDailyItemLines()
{
	int nPluDeliveryType = EcrmanOptions.GetFeaturesEposDeliveryType();
	bool bCheckForDelivery = ((nPluDeliveryType >= 1) && (nPluDeliveryType <= 10));
	bool bGotNonVoidItemNormal = FALSE;
	bool bGotNonVoidItemDelivery = FALSE;
	CArray<int, int> arrayDeliveryLines;

	int nItemCount = m_TransactionInfo.m_arrayLinePosItem.GetSize();

	int nFirstPassItemValue = 0;
	int nFirstPassItemVariance = 0;
	int nFirstPassPayment = 0;

	if (m_TransactionInfo.m_nTransactionType != CASHRSP_TRANSACTION_TYPE_VOID)
	{
		if (DealerFlags.GetLinkPluFixFlag() == TRUE)
		{
			for (int n = 0; n < nItemCount; n++)
			{
				CCashRSPImporterLinePosItem LinePosItem = m_TransactionInfo.m_arrayLinePosItem.GetAt(n);
				CCSVEposTermLine csvItem(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(LinePosItem.m_nLinePos));

				CString strVoid = csvItem.GetItemVoidFlag();

				if (strVoid != "V")
				{
					double dDatabasePrice = csvItem.GetItemDatabasePrice();
					double dExpected = csvItem.GetItemQty() * dDatabasePrice;
					int nExpected = int(floor(dExpected));
					int nActual = csvItem.GetItemLinePrice();

					nFirstPassItemValue += nActual;

					if (nExpected > nActual)
					{
						nFirstPassItemVariance += nExpected - nActual;
					}
				}
			}

			for (int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountItem.GetSize(); n++)
			{
				CCashRSPImporterLinePosItem LinePosItem = m_TransactionInfo.m_arrayLinePosDiscountItem.GetAt(n);
				CCSVEposTermLine csvItem(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(LinePosItem.m_nLinePos));
				nFirstPassItemValue += csvItem.GetItemDiscountAmount();
			}

			for (int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetSize(); n++)
			{
				CCashRSPImporterLinePosItem LinePosItem = m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetAt(n);
				CCSVEposTermLine csvItem(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(LinePosItem.m_nLinePos));

				if (csvItem.GetLineTypeInt() == CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL)
				{
					nFirstPassItemValue += csvItem.GetSubtotalDiscountAmount();
				}
			}

			for (int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountLoyalty.GetSize(); n++)
			{
				CCashRSPImporterLinePosItem LinePosItem = m_TransactionInfo.m_arrayLinePosDiscountLoyalty.GetAt(n);
				CCSVEposTermLine csvItem(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(LinePosItem.m_nLinePos));

				if (csvItem.GetLineTypeInt() == CASHRSP_LINETYPE_DISCOUNT_LOYALTY)
				{
					nFirstPassItemValue += csvItem.GetSubtotalDiscountAmount();
				}
			}

			for (int n = 0; n < m_TransactionInfo.m_arrayLinePosPayment.GetSize(); n++)
			{
				CCashRSPImporterLinePosItem LinePosItem = m_TransactionInfo.m_arrayLinePosPayment.GetAt(n);
				CCSVEposTermLine csvItem(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(LinePosItem.m_nLinePos));
				nFirstPassPayment += csvItem.GetPaymentAmount();
			}
		}
	}

	bool bCanDoLinkAdjust = ((nFirstPassItemValue != nFirstPassPayment) && (nFirstPassItemValue + nFirstPassItemVariance == nFirstPassPayment));

	for (int n = 0; n < nItemCount; n++)
	{
		CCashRSPImporterLinePosItem LinePosItem = m_TransactionInfo.m_arrayLinePosItem.GetAt(n);

		CCSVEposTermLine csvItem(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(LinePosItem.m_nLinePos));

		bool bIsDeliveryLine = FALSE;

		if ((TRUE == bCheckForDelivery) && (FALSE == bGotNonVoidItemNormal))
		{
			int nItemValue = csvItem.GetItemUnitPrice();
			int nLineValue = csvItem.GetItemLinePrice();
			int nPriceBand = csvItem.GetItemPriceBand();

			bIsDeliveryLine = TRUE;

			switch (nPluDeliveryType)
			{
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				bIsDeliveryLine &= (m_TransactionInfo.m_nTransactionType != CASHRSP_TRANSACTION_TYPE_WASTE);
				bIsDeliveryLine &= ((nPriceBand + 1) == nPluDeliveryType);
				bIsDeliveryLine &= (0 == nItemValue);
				bIsDeliveryLine &= (0 == nLineValue);
				break;

			case 1:
				bIsDeliveryLine &= (m_TransactionInfo.m_nTransactionType == CASHRSP_TRANSACTION_TYPE_WASTE);
				break;
			}

			CString strVoid = csvItem.GetItemVoidFlag();

			if (TRUE == bIsDeliveryLine)
			{
				arrayDeliveryLines.Add(LinePosItem.m_nLinePos);
				bGotNonVoidItemDelivery |= ((strVoid != "V") && (m_TransactionInfo.m_nTransactionType != CASHRSP_TRANSACTION_TYPE_VOID));
			}
			else
			{
				bGotNonVoidItemNormal |= ((strVoid != "V") && (m_TransactionInfo.m_nTransactionType != CASHRSP_TRANSACTION_TYPE_VOID));
			}
		}

		//KLUDGE EXTENDED PLU NUMBERS DOWN TO 14 DIGITS
		CString strPluNo = csvItem.GetItemPluNoString();
		if (strPluNo.GetLength() > Super.MaxPluLen())
		{
			CString strTemp = strPluNo.Right(Super.MaxPluLen() - 1);
			strPluNo = "9";
			strPluNo += strTemp;
			csvItem.SetAt(1, strPluNo);

			if (DealerFlags.Get15DigitKludgeDeptGroupFlag() == TRUE)
			{
				if (csvItem.GetItemDeptNo() == 0)
				{
					int nPluIdx = 0;
					if (m_arrayPluItems.FindPluByNumber(_atoi64(strPluNo), nPluIdx) == TRUE)
					{
						CPluCSVRecord PluRecord;
						m_arrayPluItems.GetAt(nPluIdx, PluRecord);
						int nDeptNo = PluRecord.GetBaseDeptNo();
						csvItem.SetAt(2, nDeptNo);

						int nDeptIdx = 0;
						if (m_arrayDepartments.FindDeptByNumber(nDeptNo, nDeptIdx) == TRUE)
						{
							CDepartmentCSVRecord DeptRecord;
							m_arrayDepartments.GetAt(nDeptIdx, DeptRecord);
							csvItem.SetAt(9, DeptRecord.GetEposGroup());
						}

						if (csvItem.GetSize() > 27)
						{
							CString strDescription = PluRecord.GetEposText();
							if (strDescription != "")
							{
								csvItem.SetAt(27, strDescription);
							}
						}
					}
				}
			}

			m_TransactionInfo.m_bufferDailyLogTerm.SetAt(LinePosItem.m_nLinePos, csvItem.GetLine());

			LinePosItem.m_b15Digit = TRUE;
			m_TransactionInfo.m_arrayLinePosItem.SetAt(n, LinePosItem);
		}

		if ((DealerFlags.GetLinkPluFixFlag() == TRUE) && (TRUE == bCanDoLinkAdjust))
		{
			int nDatabasePrice = csvItem.GetItemDatabasePrice();
			double dExpected = csvItem.GetItemQty() * double(nDatabasePrice);
			int nExpected = int(floor(dExpected));
			int nActual = csvItem.GetItemLinePrice();

			if (nExpected > nActual)
			{
				csvItem.SetItemLinePrice(nExpected);
				m_TransactionInfo.m_bufferDailyLogTerm.SetAt(LinePosItem.m_nLinePos, csvItem.GetLine());
			}
		}

		//KLUDGE TO REMOVE LOYALTY TOPUP MARKER IF HIDDEN OPTION FLAG IS SET
		if ( ( DealerFlags.GetRemoveLoyaltyTopupFlag() == TRUE ) && ( FALSE == bIsDeliveryLine ) )
		{
			if (DealerFlags.CheckNoLoyaltyTopupPLU(_atoi64(strPluNo)) == TRUE)
			{
				int nType, nDummyID32;
				__int64 nDummyID64;
				csvItem.GetItemTypeAndID(m_TransactionInfo.m_CashRSPVersionChecker, nType, nDummyID32, nDummyID64);

				if (nType == CASHRSP_ITEMPAYTYPE_LOYALTY)
				{
					csvItem.SetAt(16, 0);
					m_TransactionInfo.m_bufferDailyLogTerm.SetAt(LinePosItem.m_nLinePos, csvItem.GetLine());
				}
			}
		}
	}

	//CHANGE LABEL FOR DELIVERY LINES IN DAILY TRANSACTION FILE
	m_TransactionInfo.m_bIsDelivery = ( ( TRUE == bGotNonVoidItemDelivery ) && ( FALSE == bGotNonVoidItemNormal ) );

	if ( TRUE == m_TransactionInfo.m_bIsDelivery )
	{
		for ( int n = 0; n < arrayDeliveryLines.GetSize(); n++ )
		{
			int nPos = arrayDeliveryLines.GetAt(n);
			CCSVEposTermLine csvItem( m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos) );
			csvItem.SetAt( 0, "$SSFT_DELIVERY" );
			m_TransactionInfo.m_bufferDailyLogTerm.SetAt(nPos, csvItem.GetLine() );
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::BuildItemBuffer()
{
	m_TransactionInfo.m_bufferItems.RemoveAll();

	int nLastNormalBufferItem = -1;
	int nCashRSPVersionNo = GetCurrentTransactionVersionNo();

	for (int n = 0; n < m_TransactionInfo.m_arrayLinePosItem.GetSize(); n++)
	{
		CCashRSPImporterLinePosItem LinePosItem = m_TransactionInfo.m_arrayLinePosItem.GetAt(n);

		CCSVEposTermLine csvItem(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(LinePosItem.m_nLinePos));

		CString strVoid = csvItem.GetItemVoidFlag();
		CString strRefund = csvItem.GetItemRefundFlag();

		if (m_TransactionInfo.m_nTransactionType == CASHRSP_TRANSACTION_TYPE_VOID)
		{
			strVoid = "V"; //FORCE VOID FLAG ON ALL ITEMS IN A VOID TRANSACTION
		}

		double dQty = csvItem.GetItemQty();
		int nItemLineValue = csvItem.GetItemLinePrice();
		int nDbValue = csvItem.GetItemDatabasePrice();
		CString strItemDescription = csvItem.GetItemDescription(nCashRSPVersionNo);

		if ((0 == nItemLineValue) && (strItemDescription.GetLength() > 4) && (strItemDescription.Left(4) == "##1_") && (nLastNormalBufferItem != -1))
		{
			//COPY DESCRIPTION OF QUALIFIER TO ATTRIBUTE FIELD OF LAST PARENT ITEM
			CCashRSPImporterItem CashRSPItem = m_TransactionInfo.m_bufferItems.GetAt(nLastNormalBufferItem);
			CashRSPItem.SetAttributeText(strItemDescription.Right(strItemDescription.GetLength() - 4));
			m_TransactionInfo.m_bufferItems.SetAt(nLastNormalBufferItem, CashRSPItem);
		}
		else
		{
			if (DealerFlags.GetCashRSPPromoFixFlag() == TRUE)
			{
				if ((dQty > 0.0) && (nItemLineValue < 0) && (nDbValue > 0))
				{
					dQty = 0.0;
				}
			}

			bool bRefund = FALSE;
			if (strRefund == "R")
			{
				bRefund = TRUE;
				dQty *= -1;
			}

			int nType, nID32;
			__int64 nID64;
			csvItem.GetItemTypeAndID(m_TransactionInfo.m_CashRSPVersionChecker, nType, nID32, nID64);

			bool bRA = FALSE;
			switch (nType)
			{
			case CASHRSP_ITEMPAYTYPE_DEPOSIT:
			case CASHRSP_ITEMPAYTYPE_CUSTOMER:
			case CASHRSP_ITEMPAYTYPE_ROOM:
			case CASHRSP_ITEMPAYTYPE_LOYALTY:
			case CASHRSP_ITEMPAYTYPE_SMARTPAY:
			case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
			case CASHRSP_ITEMPAYTYPE_SPTBOOK:

				bRA = TRUE;

				if (CASHRSP_ITEMPAYTYPE_NORMAL == m_TransactionInfo.m_nItemPayRAType)
				{
					//CString strVoid = csvItem.GetItemVoidFlag();
					if (strVoid != "V")
					{
						m_TransactionInfo.m_nItemPayRAType = nType;
					}
				}

				break;
			}

			CString strSaleDate = m_TransactionInfo.m_strDateTran;
			CString strSaleTime = m_TransactionInfo.m_strTimeTran;

			{
				CString strDate = csvItem.GetItemSaleDate(nCashRSPVersionNo);
				CString strTime = csvItem.GetItemSaleTime(nCashRSPVersionNo);

				if ((strDate.GetLength() == 8) && (strTime.GetLength() == 8))
				{
					strSaleDate = "20";
					strSaleDate += strDate.Right(2) + strDate.Mid(3, 2) + strDate.Left(2);
					strSaleTime = strTime.Left(2) + strTime.Mid(3, 2) + strTime.Right(2);
				}
			}

			CCashRSPImporterItem CashRSPItem;
			CashRSPItem.SetPluNo(csvItem.GetItemPluNo());
			CashRSPItem.SetVoidFlag(strVoid == "V");
			CashRSPItem.SetRefundFlag(bRefund);
			CashRSPItem.SetWastageVoidFlag(csvItem.GetItemWastageVoidFlag(nCashRSPVersionNo));
			CashRSPItem.Set15DigitFlag(LinePosItem.m_b15Digit);
			CashRSPItem.SetRAFlag(bRA);
			CashRSPItem.SetAccountType(nType);
			CashRSPItem.SetAccountID64(nID64);
			CashRSPItem.SetPriceBand(csvItem.GetItemPriceBand());
			CashRSPItem.SetDeptNo(csvItem.GetItemDeptNo());
			CashRSPItem.SetGroupNo(csvItem.GetItemGroupNo());
			CashRSPItem.SetTaxBand(csvItem.GetItemTaxBand());
			CashRSPItem.SetServerNo(csvItem.GetItemServerNo(nCashRSPVersionNo));
			CashRSPItem.SetQty(dQty);
			CashRSPItem.SetLineValueAsInt(nItemLineValue);
			CashRSPItem.SetLineValueAsDouble(csvItem.GetItemLineValueReport());
			CashRSPItem.SetACatNo(csvItem.GetItemAnalysisCategory(nCashRSPVersionNo));
			CashRSPItem.SetMixMatchNo(csvItem.GetItemMixMatchNumber(nCashRSPVersionNo));
			CashRSPItem.SetMixMatchType(csvItem.GetItemMixMatchType(nCashRSPVersionNo));
			CashRSPItem.SetSaleDate(strSaleDate);
			CashRSPItem.SetSaleTime(strSaleTime);
			CashRSPItem.SetVoidBAType(csvItem.GetItemVoidBAType(nCashRSPVersionNo));
			CashRSPItem.SetAuthServerNo(csvItem.GetItemAuthServerNo(nCashRSPVersionNo));
			CashRSPItem.SetVIPStatus(csvItem.GetItemVIPStatus(nCashRSPVersionNo));
			CashRSPItem.SetPromoExportType(csvItem.GetItemPromoExportType(nCashRSPVersionNo));
			CashRSPItem.SetReasonCode(csvItem.GetItemGeneralReasonCode(nCashRSPVersionNo));
			CashRSPItem.SetReasonText(csvItem.GetItemGeneralReasonText(nCashRSPVersionNo));
			CashRSPItem.SetWastageCode(csvItem.GetItemWastageReasonCode(nCashRSPVersionNo));
			CashRSPItem.SetWastageText(csvItem.GetItemWastageReasonText(nCashRSPVersionNo));
			CashRSPItem.SetAttributeText("");

			if (EcrmanOptions.GetReportsCashRSPItemTextFlag() == TRUE)
			{
				CashRSPItem.SetDescription(csvItem.GetItemDescription(nCashRSPVersionNo));
			}

			m_TransactionInfo.m_bufferItems.Add(CashRSPItem);
			nLastNormalBufferItem = m_TransactionInfo.m_bufferItems.GetSize() - 1;

			switch (CashRSPItem.GetMixMatchType())
			{
			case MIXMATCH_ACTION_DISCOUNT_PLU:
			case MIXMATCH_ACTION_FIXCOST_AMOUNT:
			case MIXMATCH_ACTION_FIXCOST_BUCKET:
			case MIXMATCH_ACTION_FIXCOST_PLU:
			case MIXMATCH_ACTION_FREE_BUCKET:
			case MIXMATCH_ACTION_FREE_ITEM:
			{
				CCashRSPMixMatchTotals MixMatchTotal;
				MixMatchTotal.m_nPromoNo = 0;
				MixMatchTotal.m_nOfferNo = CashRSPItem.GetMixMatchNo();
				MixMatchTotal.SetAmountAsInt(CashRSPItem.GetLineValueAsInt());
				MixMatchTotal.m_nPluNo = CashRSPItem.GetPluNoInt();
				m_TransactionInfo.m_arrayMixMatchTotals.Consolidate(MixMatchTotal);
			}
			break;
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessPaymentsBuffer(CCashRSPSpooler& CashRSPSpooler)
{
	int nPaymentCount = m_TransactionInfo.m_arrayLinePosPayment.GetSize();

	int nTotalTender = 0;
	int nTotalAmount = 0;
	int nTotalGratuity = 0;
	int nTotalSurplus = 0;

	int nPossibleGratuityLine = -1;
	int nGratuityLineTender = 0;
	int nGratuityLineAmount = 0;
	int nGratuityLineGratuity = 0;
	
	if ((DealerFlags.GetGratuityFixFlag() == TRUE) || (DealerFlags.GetGratuityFix2Flag() == TRUE))
	{
		for (int n = 0; n < nPaymentCount; n++)
		{
			int nPos = m_TransactionInfo.m_arrayLinePosPayment.GetAt(n);
			CCSVEposTermLine csvPayment(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));

			int nAmount = csvPayment.GetPaymentAmount();
			int nTender = csvPayment.GetPaymentTender();
			int nGratuity = csvPayment.GetPaymentGratuity();
			int nSurplus = csvPayment.GetPaymentSurplus(m_TransactionInfo.m_CashRSPVersionChecker.GetCurrentTransactionVersionNo());

			if ( ( nTender > nAmount ) && ( nSurplus == 0 ) && ( nGratuity != nTender - nAmount ) )
			{
				if ( -1 != nPossibleGratuityLine )
				{
					nPossibleGratuityLine = -1;
					break;
				}

				nGratuityLineTender = nTender;
				nGratuityLineAmount = nAmount;
				nGratuityLineGratuity = nGratuity;
				nPossibleGratuityLine = n;
			}

			nTotalTender += nTender;
			nTotalAmount += nAmount;
			nTotalGratuity += nGratuity;
			nTotalSurplus += nSurplus;
		}
	}

	if ( (DealerFlags.GetFixDepositRecycleFlag() == TRUE) && ( FALSE == m_TransactionInfo.m_bManualMode ) )
	{
		for (int n = 0; n < nPaymentCount; n++)
		{
			int nPos = m_TransactionInfo.m_arrayLinePosPayment.GetAt(n);
			CCSVEposTermLine csvRecycle(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));

			if (csvRecycle.GetPaymentNumber() == DealerFlags.GetFixDepositRecyclePayNo())
			{
				CString strCashRSPVersion = "$CASHRSP_VERSION 14";
				int nPayAdjust = csvRecycle.GetPaymentTender();

				/*****/

				CashRSPSpooler.AddManualLine(strCashRSPVersion);
				
				CCSV csvHeader(m_TransactionInfo.m_strTransactionHeader);

				if (csvHeader.GetSize() > 10)
				{
					csvHeader.RemoveAt(10, csvHeader.GetSize() - 10);
				}
					
				csvHeader.SetAt(3, 0);
				csvHeader.SetAt(5, 0);
				csvHeader.SetAt(6, 0);
				CashRSPSpooler.AddManualLine(csvHeader.GetLine());
				
				{
					CCSV csvPayment;
					csvPayment.Add("$PAYM");
					csvPayment.Add(DealerFlags.GetFixDepositRedeemPayNo());
					csvPayment.Add("Deposit Redeem");
					csvPayment.Add(-nPayAdjust);
					csvPayment.Add(0);
					csvPayment.Add(0);
					csvPayment.Add(0);
					csvPayment.Add(DEPOSIT_ADJUST_ID);
					csvPayment.Add(8);
					csvPayment.Add(DEPOSIT_ADJUST_ID);
					CashRSPSpooler.AddManualLine(csvPayment.GetLine());
				}

				CashRSPSpooler.AddManualLine(strCashRSPVersion);

				csvHeader.SetAt(3, 1);
				csvHeader.SetAt(5, 1);
				csvHeader.SetAt(6, -nPayAdjust);
				CashRSPSpooler.AddManualLine(csvHeader.GetLine());
				
				{
					CCSV csvPayment;
					csvPayment.Add("$PAYM");
					csvPayment.Add(DealerFlags.GetFixDepositRecyclePayNo());
					csvPayment.Add("Deposit Recycle");
					csvPayment.Add(-nPayAdjust);
					csvPayment.Add(-nPayAdjust);
					csvPayment.Add(0);
					csvPayment.Add(0);
					csvPayment.Add(0);
					csvPayment.Add(99);
					csvPayment.Add(0);
					CashRSPSpooler.AddManualLine(csvPayment.GetLine());
				}

				{
					CCSV csvItem;
					csvItem.Add("$ITEM");
					csvItem.Add(DealerFlags.GetFixDepositRecyclePluNo());
					csvItem.Add(0);
					csvItem.Add(-1);
					csvItem.Add(0);
					csvItem.Add(nPayAdjust);
					csvItem.Add("A");
					csvItem.Add("");
					csvItem.Add("");
					csvItem.Add(0);
					csvItem.Add(0);
					csvItem.Add(0);
					csvItem.Add(-nPayAdjust);
					csvItem.Add("");
					csvItem.Add(DEPOSIT_ADJUST_ID);
					csvItem.Add(0);
					csvItem.Add(3);
					csvItem.Add(DEPOSIT_ADJUST_ID);
					CashRSPSpooler.AddManualLine(csvItem.GetLine());
				}
			}
		}
	}

	for ( int n = 0; n < nPaymentCount; n++ )
	{
		int nPos = m_TransactionInfo.m_arrayLinePosPayment.GetAt(n);
		CCSVEposTermLine csvPayment( m_TransactionInfo.m_bufferDailyLogTerm.GetAt( nPos ) );

		bool bDoneRefundFix = FALSE;

		if (DealerFlags.GetRefundFixFlag() == TRUE)
		{
			int nAmount = csvPayment.GetPaymentAmount();
			int nTender = csvPayment.GetPaymentTender();

			if ((nAmount > 0 ) && ( nAmount + nTender == 0 ) )
			{
				csvPayment.SetPaymentAmount(nTender);
				m_TransactionInfo.m_bufferDailyLogTerm.SetAt(nPos, csvPayment.GetLine());
				bDoneRefundFix = TRUE;
			}
		}

		if (DealerFlags.GetRefund2FixFlag() == TRUE)
		{
			if ( 1 == nPaymentCount )
			{
				int nAmount = csvPayment.GetPaymentAmount();
				int nTender = csvPayment.GetPaymentTender();

				if ((nAmount > 0) && (nAmount == nTender) && (nAmount + m_TransactionInfo.m_nHeaderValue == 0))
				{
					csvPayment.SetPaymentAmount(m_TransactionInfo.m_nHeaderValue);
					csvPayment.SetPaymentTender(m_TransactionInfo.m_nHeaderValue);
					m_TransactionInfo.m_bufferDailyLogTerm.SetAt(nPos, csvPayment.GetLine());
					bDoneRefundFix = TRUE;
				}
			}
		}

		if (TRUE == bDoneRefundFix)
		{
			continue;
		}

		if ( n == nPossibleGratuityLine )
		{
			if (DealerFlags.GetGratuityFixFlag() == TRUE)
			{
				if (
					(nGratuityLineGratuity > 0) &&
					(nGratuityLineAmount + (nGratuityLineGratuity * 2) == nGratuityLineTender) &&
					(nTotalAmount + nTotalGratuity == m_TransactionInfo.m_nHeaderValue)
					)
				{
					csvPayment.SetPaymentAmount(nGratuityLineAmount + nGratuityLineGratuity);
					m_TransactionInfo.m_bufferDailyLogTerm.SetAt(nPos, csvPayment.GetLine());
					
					nPossibleGratuityLine = -1;
				}
			}
		}

		if ( n == nPossibleGratuityLine )
		{
			if (DealerFlags.GetGratuityFix2Flag() == TRUE)
			{
				int nOverTender = nGratuityLineTender - nGratuityLineAmount;

				if (
					(nOverTender > 0) &&
					(nOverTender == 2 * (m_TransactionInfo.m_nHeaderValue - nTotalAmount))&&
					(0 == nGratuityLineGratuity) )
				{
					nGratuityLineGratuity = nOverTender / 2;
					csvPayment.SetPaymentAmount(nGratuityLineAmount + nGratuityLineGratuity);
					csvPayment.SetPaymentGratuity(nGratuityLineGratuity);
					m_TransactionInfo.m_bufferDailyLogTerm.SetAt(nPos, csvPayment.GetLine());
					nPossibleGratuityLine = -1;
				}
			}
		}

		switch( m_TransactionInfo.m_nTransactionType )
		{
		case CASHRSP_TRANSACTION_TYPE_SALE:
			m_TransactionInfo.m_nPaymentNo = csvPayment.GetPaymentNumber();
			break;
		}

		int nType, nID32;
		__int64 nID64;
		bool bIsCash;
		csvPayment.GetPaymentAccountTypeAndID( m_TransactionInfo.m_CashRSPVersionChecker, nType, nID32, nID64, bIsCash );

		//FIDDLE : CODE HERE TO REDIRECT OTHER PAYMENTS TO OTHER
		//IF (REDIRECTION ENABLED)
		//IF (CASHRSP PAYMENT TYPE IS OTHER)
		//IF (PAYMENT NUMBER EXISTS IN DATABASE)
		//IF (DATABASE PAYMENT TYPE IS OTHER)
		//IF (CASHRSP VERSION 21 OR GREATER)
		//CHANGE CSV PAYMENT TO LOYALTY TYPE, ACCOUNT ID 0

		switch( nType )
		{
		case CASHRSP_ITEMPAYTYPE_DEPOSIT:
		case CASHRSP_ITEMPAYTYPE_CUSTOMER:
		case CASHRSP_ITEMPAYTYPE_ROOM:
		case CASHRSP_ITEMPAYTYPE_LOYALTY:
		case CASHRSP_ITEMPAYTYPE_SMARTPAY:
		case CASHRSP_ITEMPAYTYPE_SMARTPHONE:
		case CASHRSP_ITEMPAYTYPE_SPTBOOK:
			ProcessTransactionPostToAccount( csvPayment, nType, nID64 );
			break;
		}

		switch( nType )
		{
		case CASHRSP_ITEMPAYTYPE_CUSTOMER:
			m_TransactionInfo.m_nCustomerNo = nID32;
			break;
		}

		int nAmount = csvPayment.GetPaymentAmount();

		if ( nAmount != 0 )
		{
			CCSV csvOut ( ',', '"', FALSE, FALSE, TRUE );
			csvOut.Add( csvPayment.GetPaymentNumber() );
			csvOut.Add( double( nAmount ) / 100.0, 2 );
			m_TransactionInfo.m_bufferDailyLogAccountsPay.Add( csvOut.GetLine() );
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessDiscountVoidBuffer()
{
	for ( int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountVoid.GetSize(); n++ )
	{
		int nPos = m_TransactionInfo.m_arrayLinePosDiscountVoid.GetAt(n);
		CCSVEposTermLine csvDiscountVoid( m_TransactionInfo.m_bufferDailyLogTerm.GetAt( nPos )  );

		CString strSaleDate = m_TransactionInfo.m_strDateTran;
		CString strSaleTime = m_TransactionInfo.m_strTimeTran;

		{
			CString strDate = csvDiscountVoid.GetVoidDiscountReasonDate( GetCurrentTransactionVersionNo() );
			CString strTime = csvDiscountVoid.GetVoidDiscountReasonTime( GetCurrentTransactionVersionNo() );

			if ( ( strDate.GetLength() == 8 ) && ( strTime.GetLength() == 8 ) )
			{
				strSaleDate = "20";
				strSaleDate += strDate.Right(2) + strDate.Mid(3,2) + strDate.Left(2);
				strSaleTime = strTime.Left(2) + strTime.Mid(3,2) + strTime.Right(2);
			}
		}

		CReasonInfo ReasonInfo;
		ReasonInfo.SetReasonType( SS_REASON_DISCOUNT_VOID );
		ReasonInfo.SetReasonCode( csvDiscountVoid.GetVoidDiscountReasonCode( GetCurrentTransactionVersionNo() ) );
		ReasonInfo.SetReasonText( csvDiscountVoid.GetVoidDiscountReasonText( GetCurrentTransactionVersionNo() ) );
		ReasonInfo.SetTimeTran( m_TransactionInfo.m_strTimeTran );
		ReasonInfo.SetTimeAction( strSaleTime );
		ReasonInfo.SetTranSeqNo( m_TransactionInfo.m_nSeqNo );
		ReasonInfo.SetTranTableNo( m_TransactionInfo.m_nTableNo );
		ReasonInfo.SetTranCovers( m_TransactionInfo.m_nCovers );
		ReasonInfo.SetTranServerNo( m_TransactionInfo.m_nServerNo );
		ReasonInfo.SetVoidDiscountAmount( csvDiscountVoid.GetVoidDiscountAmount() );
		ReasonInfo.SetVoidDiscountType( csvDiscountVoid.GetVoidDiscountType() );
		ReasonInfo.SetVoidDiscountRate( csvDiscountVoid.GetVoidDiscountRate() );
		ReasonInfo.SetVoidDiscountPromoNo( csvDiscountVoid.GetVoidDiscountPromo() );
		ReasonInfo.SetVoidDiscountUsage( csvDiscountVoid.GetVoidDiscountUsage() );
		ReasonInfo.SetVoidDiscountBAType( csvDiscountVoid.GetVoidDiscountBAType( GetCurrentTransactionVersionNo() ) );
		
		CCSV csvReason( ',', '"', FALSE, FALSE, TRUE );
		ReasonInfo.SaveCSVLine( csvReason );

		{
			CCashRSPImporterDailyLogPluItem item;
			item.m_strLineSale = csvReason.GetLine();
			item.m_strActionDate = strSaleDate;
			m_TransactionInfo.m_bufferDailyLogReasons.Add( item );
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessItemBufferSale()
{
	if ( m_TransactionInfo.m_strDateTran.GetLength() == 8 )
	{
		for ( int n = 0; n < m_TransactionInfo.m_bufferItems.GetSize(); n++ )
		{
			CCashRSPImporterItem CashRSPItem( m_TransactionInfo.m_bufferItems.GetAt(n) );

			int nItemServerNo = CashRSPItem.GetServerNo();
			if ((0 == nItemServerNo) && (DealerFlags.GetCashRSPFixItemServerZeroFlag() == TRUE))
			{
				nItemServerNo = m_TransactionInfo.m_nServerNo;
			}

			if (CashRSPItem.GetPromoExportType() != 2)
			{
				ProcessSingleItemSales(CashRSPItem, nItemServerNo);
			}

			ProcessSingleItemReasons( CashRSPItem, nItemServerNo );
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessSingleItemSales(CCashRSPImporterItem& CashRSPItem, int nItemServerNo)
{
	if (CashRSPItem.GetRAFlag() == TRUE)
	{
		if ( (CashRSPItem.GetVoidFlag() == FALSE) && ( m_TransactionInfo.m_nTransactionType != CASHRSP_TRANSACTION_TYPE_VOID))
		{
			CCSV csvOut(',', '"', FALSE, FALSE, TRUE);

			int nAccountType = CashRSPItem.GetAccountType();
			if ((CASHRSP_ITEMPAYTYPE_DEPOSIT == nAccountType) && (CashRSPItem.GetRefundFlag()))
			{
				csvOut.Add(ACCOUNT_LINETYPE_REFUND);
				csvOut.Add(CASHRSP_ITEMPAYTYPE_DEPOSIT);
				csvOut.Add(CashRSPItem.GetAccountID64());
				csvOut.Add(-CashRSPItem.GetLineValueAsDouble(), 2);
			}
			else
			{
				csvOut.Add(ACCOUNT_LINETYPE_RECEIVE);
				csvOut.Add(nAccountType);
				csvOut.Add(CashRSPItem.GetAccountID64());
				csvOut.Add(CashRSPItem.GetLineValueAsDouble(), 2);
			}

			m_TransactionInfo.m_bufferDailyLogAccounts.Add(csvOut.GetLine());
		}
	}
	else
	{
		//FIND OUT IF ANY LOGGING IS REQUIRED
		bool bLogAsWastage = FALSE;
		bool bLogAsVoid = FALSE;
		bool bLogAsStock = FALSE;
		if ((CashRSPItem.GetVoidFlag() == TRUE) || (m_TransactionInfo.m_nTransactionType == CASHRSP_TRANSACTION_TYPE_VOID))
		{
			bLogAsVoid = TRUE;
			if (CashRSPItem.GetWastageVoidFlag() == TRUE)
			{
				bLogAsStock = TRUE;
				bLogAsWastage = TRUE;
			}
		}
		else
		{
			bLogAsWastage = (CASHRSP_TRANSACTION_TYPE_WASTE == m_TransactionInfo.m_nTransactionType);
			bLogAsStock = TRUE;
		}

		//QUICK RETURN IF NO LOGGING REQUIRED
		if ((FALSE == bLogAsStock) && (FALSE == bLogAsVoid))
		{
			return;
		}

		CCSV csvSale(',', '"', FALSE, FALSE, TRUE);
		csvSale.Add(CSVPLUDATA_PLU_SALE);					//LINE TYPE
		csvSale.Add(CASHRSP_SALES_TRADING);					//LEGACY TRAINING MODE
		csvSale.Add(m_TransactionInfo.m_strTimeTran);		//TRAN TIME
		csvSale.Add(CashRSPItem.GetSaleTime());				//SALE TIME
		csvSale.Add(CashRSPItem.GetPluNoInt());				//PLU NO
		csvSale.Add(0);										//PROMO NO
		csvSale.Add(CashRSPItem.GetPriceBand());			//PRICE BAND
		csvSale.Add(CashRSPItem.GetQty(), 5);				//SALES QTY
		csvSale.Add(0);										//DISCOUNT QTY
		csvSale.Add(CashRSPItem.GetLineValueAsDouble(), 2);	//VALUE
		csvSale.Add(CashRSPItem.GetTaxBand());				//TAX BAND
		csvSale.Add(m_TransactionInfo.m_nCovers);			//COVERS
		csvSale.Add(CashRSPItem.GetDeptNo());				//DEPT NO
		csvSale.Add(CashRSPItem.GetGroupNo() + 1);			//GROUP NO
		csvSale.Add(CashRSPItem.GetACatNo());				//CATEGORY
		csvSale.Add(m_TransactionInfo.m_nServerNo);			//TRAN SERVER
		csvSale.Add(nItemServerNo);							//ITEM SERVER
		csvSale.Add(m_TransactionInfo.m_nCustomerNo);		//CUSTOMER NO
		csvSale.Add(m_TransactionInfo.m_nPaymentNo);		//PAYMENT NO
		csvSale.Add(CashRSPItem.GetMixMatchNo());			//MIX MATCH NO
		csvSale.Add(CashRSPItem.GetMixMatchType());			//MIX MATCH TYPE
		csvSale.Add(bLogAsWastage ? 1 : 0);					//WASTAGE
		csvSale.Add(0);										//DISCOUNT MODE
		csvSale.Add("");									//DISCOUNT TYPE
		csvSale.Add(0);										//DISCOUNT RATE
		csvSale.Add(CashRSPItem.GetVIPStatus());			//VIP STATUS
		csvSale.Add(m_TransactionInfo.m_nSeqNo);			//TRAN SEQ NO
		csvSale.Add(CashRSPItem.GetDescription());			//ITEM DESCRIPTION
		csvSale.Add(m_TransactionInfo.m_nTableNo);			//TABLE NO
		csvSale.Add(CashRSPItem.GetAttributeText());		//ATTRIBUTE TEXT
		csvSale.Add(m_TransactionInfo.m_strLoyaltyID);		//LOYALTY ID
		csvSale.Add(m_TransactionInfo.m_nLineTNo);			//LINE TNO
		csvSale.Add(CashRSPItem.GetRefundFlag() ? 1 : 0);	//REFUND FLAG
		csvSale.Add(bLogAsVoid ? 1 : 0);					//VOID FLAG

		CCSV csvItemPay(',', '"', FALSE, FALSE, TRUE);

		//WASTAGE AND VOID ARE NOT LOGGED AS PAID FOR ITEMS
		if ( ( FALSE == bLogAsVoid ) && ( FALSE == bLogAsWastage))
		{
			csvItemPay.Add("1");
			csvItemPay.Add(CSVPLUDATA_PLU_SALE);
			csvItemPay.Add(1);										//VERSION
			csvItemPay.Add(CashRSPItem.GetPluNoInt());				//PLU NO
			csvItemPay.Add(CashRSPItem.GetDeptNo());				//DEPT NO
			csvItemPay.Add(CashRSPItem.GetGroupNo() + 1);			//GROUP NO
			csvItemPay.Add(CashRSPItem.GetLineValueAsDouble(), 2);	//VALUE
			csvItemPay.Add(CashRSPItem.GetPriceBand());				//PRICE BAND
			csvItemPay.Add(CashRSPItem.GetTaxBand());				//TAX BAND
		}

		//BUFFER ITEM INFORMATION FOR FUTURE PROCESSING
		{
			CCashRSPImporterDailyLogPluItem item;
			item.m_strLineSale = csvSale.GetLine();
			item.m_strLineItemPay = csvItemPay.GetLine();
			item.m_strActionDate = CashRSPItem.GetSaleDate();
			item.m_strTaxBand = CashRSPItem.GetTaxBand();
			item.m_nMixMatchType = CashRSPItem.GetMixMatchType();
			item.m_nMixMatchOfferNo = CashRSPItem.GetMixMatchNo();
			item.m_nPluNo = CashRSPItem.GetPluNoInt();
			m_TransactionInfo.m_bufferDailyLogSales.Add(item);
		}

		if (TRUE == bLogAsStock)
		{
			//BUFFER SALE FOR DISCOUNT CALCULATIONS
			if ((FALSE == bLogAsWastage) && (FALSE == bLogAsVoid))
			{
				CCashRSPImporterDiscountHelperSales infoDiscount;
				infoDiscount.m_nPluNo = CashRSPItem.GetPluNoInt();
				infoDiscount.m_strDescription = CashRSPItem.GetDescription();
				infoDiscount.m_strAttributeText = CashRSPItem.GetAttributeText();
				infoDiscount.m_nAnalysisCategory = CashRSPItem.GetACatNo();
				infoDiscount.m_nVIPStatus = CashRSPItem.GetVIPStatus();
				infoDiscount.m_nItemServerNo = nItemServerNo;
				infoDiscount.m_strSaleDate = CashRSPItem.GetSaleDate();
				infoDiscount.m_strSaleTime = CashRSPItem.GetSaleTime();
				infoDiscount.m_nPriceBand = CashRSPItem.GetPriceBand();
				infoDiscount.m_nDeptNo = CashRSPItem.GetDeptNo();
				infoDiscount.m_nGroupNo = CashRSPItem.GetGroupNo();
				infoDiscount.m_strTaxBand = CashRSPItem.GetTaxBand();
				infoDiscount.m_nMixMatchType = CashRSPItem.GetMixMatchType();
				infoDiscount.m_nMixMatchNo = CashRSPItem.GetMixMatchNo();
				infoDiscount.m_dQty = CashRSPItem.GetQty();
				infoDiscount.m_dVal = CashRSPItem.GetLineValueAsDouble();

				m_TransactionInfo.m_arrayDiscountHelperSales.Consolidate(infoDiscount);
				m_TransactionInfo.m_dTotalSalesValue += CashRSPItem.GetLineValueAsDouble();
			}

			m_TransactionInfo.m_bGotNormalItem = TRUE;

			//DON'T CHANGE STOCK LEVEL FOR REBUILD OR FOR SPECIAL 15 DIGIT ITEMS
			switch (m_BatchInfo.m_nProcessType)
			{
			case CASHRSP_PROCESSTYPE_NEWSALE:
			case CASHRSP_PROCESSTYPE_RETRY:
				if (CashRSPItem.Get15DigitFlag() == FALSE)
				{
					CCSV csvOut(',', '"', FALSE, FALSE, TRUE);
					csvOut.Add(m_TransactionInfo.m_strDateTran);
					csvOut.Add(m_TransactionInfo.m_strTimeTran);
					csvOut.Add(CashRSPItem.GetPluNoInt());
					csvOut.Add(CashRSPItem.GetPriceBand());
					csvOut.Add(CashRSPItem.GetQty(), 5);
					csvOut.Add(CashRSPItem.GetLineValueAsDouble(), 2);
					csvOut.Add(bLogAsWastage ? 1 : 0);
					m_TransactionInfo.m_bufferPendingStockSales.Add(csvOut.GetLine());
				}
				break;
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessSingleItemReasons( CCashRSPImporterItem& CashRSPItem, int nItemServerNo )
{
	int nReasonType = -1;

	if (CASHRSP_TRANSACTION_TYPE_WASTE == m_TransactionInfo.m_nTransactionType)
	{
		if (GetCurrentTransactionVersionNo() >= 26)
		{
			nReasonType = SS_REASON_WASTAGE;
		}
	}
	else
	{
		if (GetCurrentTransactionVersionNo() >= 19)
		{
			if (CashRSPItem.GetVoidFlag() || CashRSPItem.GetRefundFlag())
			{
				nReasonType = SS_REASON_REFUNDVOID;
			}
		}

		if ( ( -1 == nReasonType ) && ( GetCurrentTransactionVersionNo() >= 26 ) )
		{
			if (CashRSPItem.GetVIPStatus() == 2)
			{
				nReasonType = SS_REASON_VIP;
			}
		}

		if ( ( -1 == nReasonType ) && ( GetCurrentTransactionVersionNo() >= 26 ) )
		{
			if (CashRSPItem.GetPromoExportType() != 0)
			{
				nReasonType = SS_REASON_DISCOUNT_SUBT;
			}
		}
	}

	switch( nReasonType )
	{
	case SS_REASON_REFUNDVOID:
	case SS_REASON_VIP:
	case SS_REASON_WASTAGE:
		{
			CReasonInfo ReasonInfo;
			ReasonInfo.SetReasonType( nReasonType );

			if ( nReasonType != SS_REASON_WASTAGE )
			{
				ReasonInfo.SetReasonCode( CashRSPItem.GetReasonCode() );
				ReasonInfo.SetReasonText( CashRSPItem.GetReasonText() );
			}
			else
			{
				ReasonInfo.SetReasonCode( CashRSPItem.GetWastageCode() );
				ReasonInfo.SetReasonText( CashRSPItem.GetWastageText() );
			}

			ReasonInfo.SetTimeTran( m_TransactionInfo.m_strTimeTran );
			ReasonInfo.SetTimeAction( CashRSPItem.GetSaleTime() );
			ReasonInfo.SetTranSeqNo( m_TransactionInfo.m_nSeqNo );
			ReasonInfo.SetTranTableNo( m_TransactionInfo.m_nTableNo );
			ReasonInfo.SetTranCovers( m_TransactionInfo.m_nCovers );
			ReasonInfo.SetTranServerNo( m_TransactionInfo.m_nServerNo );
			ReasonInfo.SetItemVoidFlag( CashRSPItem.GetVoidFlag() );
			ReasonInfo.SetItemRefundFlag( CashRSPItem.GetRefundFlag() );
			ReasonInfo.SetItemWastageVoidFlag( CashRSPItem.GetWastageVoidFlag() );
			ReasonInfo.SetItemServerNo( nItemServerNo );
			ReasonInfo.SetItemPluNo( CashRSPItem.GetPluNoInt() );
			ReasonInfo.SetItemPriceBand( CashRSPItem.GetPriceBand() );
			ReasonInfo.SetItemQty( CashRSPItem.GetQty() );
			ReasonInfo.SetItemVal( CashRSPItem.GetLineValueAsDouble() );
			ReasonInfo.SetItemTaxBand( CashRSPItem.GetTaxBand() );
			ReasonInfo.SetItemDeptNo( CashRSPItem.GetDeptNo() );
			ReasonInfo.SetItemGroupNo( CashRSPItem.GetGroupNo() + 1 );
			ReasonInfo.SetItemACatNo( CashRSPItem.GetACatNo() );
			ReasonInfo.SetItemVoidBAType( CashRSPItem.GetVoidBAType() );
			ReasonInfo.SetItemAuthServerNo( CashRSPItem.GetAuthServerNo() );

			CCSV csvReason( ',', '"', FALSE, FALSE, TRUE );
			ReasonInfo.SaveCSVLine( csvReason );

			{
				CCashRSPImporterDailyLogPluItem item;
				item.m_strLineSale = csvReason.GetLine();
				item.m_strActionDate = CashRSPItem.GetSaleDate();
				m_TransactionInfo.m_bufferDailyLogReasons.Add( item );
			}
		}
		break;


	case SS_REASON_DISCOUNT_SUBT:
		{
			int nDiscountRate = 0;
			CString strDiscountType = "?";
			
			int nPromoNo = int ( CashRSPItem.GetPluNoInt() % 100 );

			for ( int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetSize(); n++ )
			{
				int nPos = m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetAt(n);
				CCSVEposTermLine csvDiscount( m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos) );

				if (csvDiscount.GetLineTypeInt() == CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL)
				{
					if (csvDiscount.GetSubtotalDiscountPromo() == nPromoNo)
					{
						nDiscountRate = csvDiscount.GetSubtotalDiscountRate();
						strDiscountType = csvDiscount.GetSubtotalDiscountType();
						break;
					}
				}
			}
		
			CReasonInfo ReasonInfo;
			ReasonInfo.SetReasonType( SS_REASON_DISCOUNT_SUBT );
			ReasonInfo.SetReasonCode( CashRSPItem.GetReasonCode() );
			ReasonInfo.SetReasonText( CashRSPItem.GetReasonText() );
			ReasonInfo.SetTimeTran( m_TransactionInfo.m_strTimeTran );
			ReasonInfo.SetTimeAction( m_TransactionInfo.m_strTimeTran );
			ReasonInfo.SetTranSeqNo( m_TransactionInfo.m_nSeqNo );
			ReasonInfo.SetTranTableNo( m_TransactionInfo.m_nTableNo );
			ReasonInfo.SetTranCovers( m_TransactionInfo.m_nCovers );
			ReasonInfo.SetTranServerNo( m_TransactionInfo.m_nServerNo );
			ReasonInfo.SetSDiscPromoNo( nPromoNo );
			ReasonInfo.SetSDiscType( strDiscountType );
			ReasonInfo.SetSDiscRate( nDiscountRate );
			ReasonInfo.SetSDiscAmount( CashRSPItem.GetLineValueAsInt() );

			CCSV csvReason( ',', '"', FALSE, FALSE, TRUE );
			ReasonInfo.SaveCSVLine( csvReason );

			{
				CCashRSPImporterDailyLogPluItem item;
				item.m_strLineSale = csvReason.GetLine();
				item.m_strActionDate = m_TransactionInfo.m_strDateTran;
				m_TransactionInfo.m_bufferDailyLogReasons.Add( item );
			}
		}
		break;
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessItemBufferDelivery()
{
	switch( m_BatchInfo.m_nProcessType )
	{
	case CASHRSP_PROCESSTYPE_NEWSALE:
	case CASHRSP_PROCESSTYPE_RETRY:
		{
			for ( int n = 0; n < m_TransactionInfo.m_bufferItems.GetSize(); n++ )
			{
				CCashRSPImporterItem csvItem( m_TransactionInfo.m_bufferItems.GetAt(n) );

				if ( csvItem.GetVoidFlag() == FALSE )
				{
					CCSV csvOut ( ',', '"', FALSE, FALSE, TRUE );
					csvOut.Add( m_TransactionInfo.m_strDateTran );
					csvOut.Add( m_TransactionInfo.m_strTimeTran );
					csvOut.Add( csvItem.GetPluNoInt() );
					csvOut.Add( csvItem.GetQty(), 5 );
					m_TransactionInfo.m_bufferPendingStockDelivery.Add( csvOut.GetLine() );
				}
			}
		}
		break;
	}
}

/**********************************************************************/

void CCashRSPImporter::PrepareDiscounts()
{
	for (int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountItem.GetSize(); n++)
	{
		int nPos = m_TransactionInfo.m_arrayLinePosDiscountItem.GetAt(n);

		CCSVEposTermLine csvDiscount(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));
		int nAnalysisCategory = csvDiscount.GetItemDiscountAnalysisCategory(GetCurrentTransactionVersionNo());
		int nPromoNo = csvDiscount.GetItemDiscountPromoNo(GetCurrentTransactionVersionNo());

		int nDiscountServerNo = csvDiscount.GetItemDiscountServerNo(GetCurrentTransactionVersionNo());
		if ((0 == nDiscountServerNo) && (DealerFlags.GetCashRSPFixItemServerZeroFlag() == TRUE))
		{
			nDiscountServerNo = m_TransactionInfo.m_nServerNo;
		}

		CCashRSPImporterDiscountHelperPendingItem infoDiscount;
		infoDiscount.m_nDiscountServerNo = nDiscountServerNo;
		infoDiscount.m_nPluNo = csvDiscount.GetItemDiscountPluNoInt();
		infoDiscount.m_nAnalysisCategory = nAnalysisCategory;
		infoDiscount.m_nPromoNo = nPromoNo;
		infoDiscount.m_nAmountAsInt = csvDiscount.GetItemDiscountAmount() * -1;
		infoDiscount.m_dAmountAsDouble = ((double)infoDiscount.m_nAmountAsInt);
		infoDiscount.m_dAmountAsDouble /= 100.0;
		infoDiscount.m_strDiscountType = csvDiscount.GetItemDiscountType();
		infoDiscount.m_nDiscountRate = csvDiscount.GetItemDiscountRate();
		infoDiscount.m_nReasonCode = csvDiscount.GetItemDiscountReasonCode(GetCurrentTransactionVersionNo());
		infoDiscount.m_strReasonText = csvDiscount.GetItemDiscountReasonText(GetCurrentTransactionVersionNo());
		m_TransactionInfo.m_arrayDiscountHelperPendingItem.Consolidate(infoDiscount);

		m_TransactionInfo.m_dTotalSalesValue -= infoDiscount.m_dAmountAsDouble;
	}

	for (int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountMixMatch.GetSize(); n++)
	{
		int nPos = m_TransactionInfo.m_arrayLinePosDiscountMixMatch.GetAt(n);
		CCSVEposTermLine csvDiscount(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));

		CCashRSPMixMatchItems MixMatchItem;
		MixMatchItem.m_nOfferNo = csvDiscount.GetMixMatchDiscountMixMatchNo();
		MixMatchItem.m_nOfferType = csvDiscount.GetMixMatchDiscountType();
		MixMatchItem.m_nDiscountRate = csvDiscount.GetMixMatchDiscountRate();
		MixMatchItem.m_strTaxBand = csvDiscount.GetMixMatchDiscountTaxBand();
		MixMatchItem.m_nPluNo = _atoi64(csvDiscount.GetMixMatchDiscountPluNo());

		if (MixMatchItem.m_nOfferType == MIXMATCH_ACTION_PROMO)
		{
			MixMatchItem.m_nPromoNo = csvDiscount.GetMixMatchDiscountPromoNo(GetCurrentTransactionVersionNo());
		}
		else
		{
			MixMatchItem.m_nPromoNo = 0;
		}

		double dAmount = csvDiscount.GetMixMatchDiscountAmount();
		int nSplit = csvDiscount.GetMixMatchDiscountNSplit(GetCurrentTransactionVersionNo());

		if (nSplit > 1)
		{
			dAmount /= ((double)nSplit);
		}

		MixMatchItem.m_dAmount = dAmount;
		m_TransactionInfo.m_arrayMixMatchItems.Consolidate(MixMatchItem);

		if (MixMatchItem.m_nPromoNo != 0)
		{
			CCashRSPMixMatchTotals MixMatchTotal;
			MixMatchTotal.m_nPromoNo = MixMatchItem.m_nPromoNo;
			MixMatchTotal.m_nOfferNo = MixMatchItem.m_nOfferNo;
			MixMatchTotal.SetAmountAsDouble(MixMatchItem.m_dAmount * (-1));
			m_TransactionInfo.m_arrayMixMatchTotals.Consolidate(MixMatchTotal);
		}
	}

	//CALCULATE HOW MUCH OF EACH PROMOTION IS ACCOUNTED FOR BY MIX MATCH
	for (int n = 0; n < m_TransactionInfo.m_arrayMixMatchTotals.GetSize(); n++)
	{
		CCashRSPMixMatchTotals MixMatchTotal;
		m_TransactionInfo.m_arrayMixMatchTotals.GetAt(n,MixMatchTotal);

		if (MixMatchTotal.m_nPromoNo != 0)
		{
			CCashRSPPromoTotals PromoTotal;
			PromoTotal.m_nPromoNo = MixMatchTotal.m_nPromoNo;
			PromoTotal.m_nPromoMMTotal = MixMatchTotal.GetAmountAsInt() * (-1);
			m_TransactionInfo.m_arrayPromoTotals.Consolidate(PromoTotal);
		}
	}

	//CALCULATE TOTAL DISCOUNT BY PROMO TYPE
	for (int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetSize(); n++)
	{
		int nPos = m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetAt(n);
		CCSVEposTermLine csvDiscount(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));

		switch (csvDiscount.GetLineTypeInt())
		{
		case CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL:
		{
			CCashRSPPromoTotals PromoTotal;
			PromoTotal.m_nPromoNo = csvDiscount.GetSubtotalDiscountPromo();
			PromoTotal.m_nPromoTotal = csvDiscount.GetSubtotalDiscountAmount() * (-1);
			m_TransactionInfo.m_arrayPromoTotals.Consolidate(PromoTotal);
		}
		break;
		}
	}

	for (int n = 0; n < m_TransactionInfo.m_arrayLinePosDiscountLoyalty.GetSize(); n++)
	{
		int nPos = m_TransactionInfo.m_arrayLinePosDiscountLoyalty.GetAt(n);
		CCSVEposTermLine csvDiscount(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));

		switch (csvDiscount.GetLineTypeInt())
		{
		case CASHRSP_LINETYPE_DISCOUNT_LOYALTY:
		{
			CCashRSPPromoTotals PromoTotal;
			PromoTotal.m_nPromoNo = csvDiscount.GetSubtotalDiscountPromo();
			PromoTotal.m_nPromoTotal = csvDiscount.GetSubtotalDiscountAmount() * (-1);
			m_TransactionInfo.m_arrayPromoTotals.Consolidate(PromoTotal);
		}
		break;
		}
	}

	//CREATE PLU INFORMATION FOR NON MIX MATCH PROMOS
	for (int n = m_TransactionInfo.m_arrayLinePosDiscountPromo.GetSize() - 1; n >= 0; n--)
	{
		int nPos = m_TransactionInfo.m_arrayLinePosDiscountPromo.GetAt(n);
		CCSVEposTermLine csvDiscount(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));

		CString strAppliedAs = csvDiscount.GetPromoDiscountAppliedAs();
		if (strAppliedAs != "MM")
		{
			CCashRSPPromoItems PromoItem;
			PromoItem.m_nPromoNo = csvDiscount.GetPromoDiscountPromoNo();
			PromoItem.m_nPluNo = _atoi64(csvDiscount.GetPromoDiscountPluNo());
			PromoItem.m_strDiscountMode = csvDiscount.GetPromoDiscountAppliedAs();
			PromoItem.m_strDiscountType = csvDiscount.GetPromoDiscountType();
			PromoItem.m_nDiscountRate = csvDiscount.GetPromoDiscountRate();
			PromoItem.m_bMicrotrendTaxFlag = csvDiscount.GetPromoDiscountMicrotrendTaxFlag();
			PromoItem.m_dAmount = csvDiscount.GetPromoDiscountPromoAmount();

			m_TransactionInfo.m_arrayPromoItems.Consolidate(PromoItem);
		}
	}

	//FILTER OUT SPECIAL PLU ITEMS FOR MIX MATCH OFFERS THAT ARE COVERED BY MMDISC
	for (int n = m_TransactionInfo.m_bufferDailyLogSales.GetSize() - 1; n >= 0; n--)
	{
		CCashRSPImporterDailyLogPluItem SaleItem = m_TransactionInfo.m_bufferDailyLogSales.GetAt(n);

		switch (SaleItem.m_nMixMatchType)
		{
		//MIXMATCH_ACTION_PROMO				DOES NOT CREATE SPECIAL PLU ITEM FOR DISCOUNT
		//MIXMATCH_ACTION_FIXCOST_PLU_2		ASSIGNS ALL VALUE TO SPECIAL ITEM (SO LEAVE AS IS)
		case MIXMATCH_ACTION_DISCOUNT_PLU:
		case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		case MIXMATCH_ACTION_FREE_ITEM:
		case MIXMATCH_ACTION_FIXCOST_PLU:
		case MIXMATCH_ACTION_FREE_BUCKET:
		case MIXMATCH_ACTION_FIXCOST_BUCKET:
		{
			if (GotMMDiscLines(0, SaleItem.m_nMixMatchOfferNo) == TRUE)
			{
				m_TransactionInfo.m_bufferDailyLogSales.RemoveAt(n);
			}
		}
		break;
		}
	}

	//FILTER OUT DISCOUNT HELPER INFO FOR MIX MATCH OFFERS THAT ARE COVERED BY MMDISC
	for (int n = m_TransactionInfo.m_arrayDiscountHelperSales.GetSize() - 1; n >= 0; n--)
	{
		CCashRSPImporterDiscountHelperSales infoSales;
		m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(n, infoSales);

		switch (infoSales.m_nMixMatchType)
		{
		//MIXMATCH_ACTION_PROMO				DOES NOT CREATE SPECIAL PLU ITEM FOR DISCOUNT
		//MIXMATCH_ACTION_FIXCOST_PLU_2		ASSIGNS ALL VALUE TO SPECIAL ITEM (SO LEAVE AS IS)
		case MIXMATCH_ACTION_DISCOUNT_PLU:
		case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		case MIXMATCH_ACTION_FREE_ITEM:
		case MIXMATCH_ACTION_FIXCOST_PLU:
		case MIXMATCH_ACTION_FREE_BUCKET:
		case MIXMATCH_ACTION_FIXCOST_BUCKET:
			if (GotMMDiscLines(0, infoSales.m_nMixMatchNo) == TRUE)
			{
				if (MIXMATCH_ACTION_FREE_ITEM == infoSales.m_nMixMatchType)
				{
					m_TransactionInfo.m_arrayCheapestOneFree.DirectAdd(infoSales);
				}

				m_TransactionInfo.m_arrayDiscountHelperSales.RemoveAt(n);
				m_TransactionInfo.m_dTotalSalesValue -= infoSales.m_dVal;
			}
			break;
		}
	}

	if ( CheckPromoRules() == FALSE )
	{
		bool bLoyalty = FALSE;
		int nSubtotalDiscountCount = m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetSize();
		int nLoyaltyDiscountCount = m_TransactionInfo.m_arrayLinePosDiscountLoyalty.GetSize();

		for (int n = 0; n < nSubtotalDiscountCount + nLoyaltyDiscountCount; n++)
		{
			int nPos;
			if (n < nSubtotalDiscountCount)
			{
				nPos = m_TransactionInfo.m_arrayLinePosDiscountSubtotal.GetAt(n);
				bLoyalty = FALSE;
			}
			else
			{
				nPos = m_TransactionInfo.m_arrayLinePosDiscountLoyalty.GetAt(n - nSubtotalDiscountCount);
				bLoyalty = TRUE;
			}

			if ((n == 0) || (n == nSubtotalDiscountCount))
			{
				m_TransactionInfo.m_nCurrentPromoNo = 0;
				m_TransactionInfo.m_nCurrentPromoPluType = PROMO_REPORTASPLU_NONVATABLE;
				m_TransactionInfo.m_bCurrentPromoSCTaxFix = FALSE;
				m_TransactionInfo.m_bCurrentPromoDiscountForceTaxable = FALSE;
				m_TransactionInfo.m_nCurrentPromoDeptNo = 0;
				m_TransactionInfo.m_nCurrentPromoSalesQty = 0;
			}

			int nDiscountMode = (bLoyalty) ? DISCOUNT_MODE_LOYALTY : DISCOUNT_MODE_SUBTOTAL;

			CCSVEposTermLine csvDiscount(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nPos));

			switch (csvDiscount.GetLineTypeInt())
			{
			case CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL:
			case CASHRSP_LINETYPE_DISCOUNT_LOYALTY:
			{
				bool bTaxableDiscount = TRUE;

				if (CCashRSPVersionChecker::CheckCashRSPVersion(GetCurrentTransactionVersionNo(), 15, FALSE) == TRUE)
				{
					bTaxableDiscount = csvDiscount.GetSubtotalDiscountMicrotrendTaxFlag();
				}

				double dDiscountAmount = ((double)csvDiscount.GetSubtotalDiscountAmount());
				dDiscountAmount /= 100.0;
				dDiscountAmount *= -1;

				CString strDiscountType = csvDiscount.GetSubtotalDiscountType();
				int nDiscountRate = csvDiscount.GetSubtotalDiscountRate();

				m_TransactionInfo.SetCurrentPromoNo(csvDiscount.GetSubtotalDiscountPromo(), m_arrayPromotions);

				if (CanUsePromoRules(m_TransactionInfo.m_nCurrentPromoNo) == FALSE)
				{
					if (FALSE == bTaxableDiscount)
					{
						if (TRUE == m_TransactionInfo.m_bCurrentPromoDiscountForceTaxable)
						{
							if (strDiscountType != "P")
							{
								bTaxableDiscount = TRUE;
							}
						}
					}

					if (TRUE == m_TransactionInfo.m_bCurrentPromoSCTaxFix)
					{
						ProcessSeparateDiscount(dDiscountAmount, nDiscountMode, strDiscountType, nDiscountRate, TRUE, "A");
					}
					else if ((FALSE == bTaxableDiscount) && (PROMO_REPORTASPLU_NEVER != m_TransactionInfo.m_nCurrentPromoPluType))
					{
						ProcessSeparateDiscount(dDiscountAmount, nDiscountMode, strDiscountType, nDiscountRate, FALSE);
					}
					else if (m_TransactionInfo.m_nCurrentPromoPluType != PROMO_REPORTASPLU_ALWAYS)
					{
						CCashRSPImporterDiscountHelperPendingSubtotal infoSubtotal;
						infoSubtotal.m_strCategories = csvDiscount.GetSubtotalDiscountAnalysisCategories(GetCurrentTransactionVersionNo());
						infoSubtotal.m_dVal = dDiscountAmount;
						infoSubtotal.m_nPromoNo = m_TransactionInfo.m_nCurrentPromoNo;
						infoSubtotal.m_nDiscountMode = nDiscountMode;
						infoSubtotal.m_strDiscountType = csvDiscount.GetSubtotalDiscountType();
						infoSubtotal.m_nDiscountRate = csvDiscount.GetSubtotalDiscountRate();
						infoSubtotal.m_bVATable = bTaxableDiscount;
						m_TransactionInfo.m_arrayDiscountHelperPendingSubtotal.Consolidate(infoSubtotal);

						if (FALSE == bLoyalty)
						{
							m_TransactionInfo.m_dSubtotalDiscountValue += dDiscountAmount;
						}
						else
						{
							m_TransactionInfo.m_dLoyaltyDiscountValue += dDiscountAmount;
						}
					}
				}
			}
			break;

			case CASHRSP_LINETYPE_DISCOUNTTAX_SUBTOTAL:
			case CASHRSP_LINETYPE_DISCOUNTTAX_LOYALTY:
			{
				if (CanUsePromoRules(m_TransactionInfo.m_nCurrentPromoNo) == FALSE)
				{
					if ((PROMO_REPORTASPLU_ALWAYS == m_TransactionInfo.m_nCurrentPromoPluType) && (FALSE == m_TransactionInfo.m_bCurrentPromoSCTaxFix))
					{
						double dDiscountAmount = ((double)csvDiscount.GetDiscountTaxBandAmount());
						dDiscountAmount /= 100.0;
						dDiscountAmount *= -1;

						CString strTaxBand = csvDiscount.GetDiscountTaxBand();

						ProcessSeparateDiscount(dDiscountAmount, nDiscountMode, "", 0, TRUE, strTaxBand);
					}
				}
			}
			break;
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ConsolidateItemDiscountToWrite( CCashRSPImporterDiscountShareList& DiscountShare, int nIdx, CCashRSPImporterDiscountHelperPendingItem& infoPending )
{
	ConsolidateItemDiscountToWrite( DiscountShare, nIdx, infoPending, DiscountShare.GetDiscountShare(nIdx) );
}

/**********************************************************************/

void CCashRSPImporter::ConsolidateItemDiscountToWrite( CCashRSPImporterDiscountShareList& DiscountShare, int nIdx, CCashRSPImporterDiscountHelperPendingItem& infoPending, double dVal )
{
	int nSaleIdx = DiscountShare.GetIndex( nIdx );

	CCashRSPImporterDiscountHelperSales infoSales;
	m_TransactionInfo.m_arrayDiscountHelperSales.GetAt( nSaleIdx, infoSales );
	infoSales.m_dVal -= dVal;
	m_TransactionInfo.m_arrayDiscountHelperSales.SetAt( nSaleIdx, infoSales );
	
	CCashRSPImporterDiscountHelperToWrite info;
	info.m_nSaleIdx				= nSaleIdx;
	info.m_nPluNo				= infoPending.m_nPluNo;
	info.m_strItemDescription	= infoSales.m_strDescription;
	info.m_strAttributeText		= infoSales.m_strAttributeText;
	info.m_nAnalysisCategory	= infoPending.m_nAnalysisCategory;
	info.m_nVIPStatus			= infoSales.m_nVIPStatus;
	info.m_nPromoNo				= infoPending.m_nPromoNo;
	info.m_dDiscountedItemQty	= infoSales.m_dQty;
	info.m_dVal					= dVal;
	info.m_nDiscountMode		= DISCOUNT_MODE_ITEM;
	info.m_strDiscountType		= infoPending.m_strDiscountType;
	info.m_nDiscountRate		= infoPending.m_nDiscountRate;
	info.m_bVATable				= TRUE;
	m_TransactionInfo.m_arrayDiscountHelperToWrite.Consolidate( info );
}

/**********************************************************************/

void CCashRSPImporter::ConsolidateDiscountToWrite( int nSaleIdx, CCashRSPImporterDiscountHelperPendingSubtotal& infoSubtotal, double dVal )
{
	CCashRSPImporterDiscountHelperSales infoSales;
	m_TransactionInfo.m_arrayDiscountHelperSales.GetAt( nSaleIdx, infoSales );

	CCashRSPImporterDiscountHelperToWrite info;
	info.m_nSaleIdx				= nSaleIdx;
	info.m_nPluNo				= infoSales.m_nPluNo;
	info.m_strItemDescription	= infoSales.m_strDescription;
	info.m_strAttributeText		= infoSales.m_strAttributeText;
	info.m_nAnalysisCategory	= infoSales.m_nAnalysisCategory;
	info.m_nVIPStatus			= infoSales.m_nVIPStatus;
	info.m_nPromoNo				= infoSubtotal.m_nPromoNo;
	info.m_dDiscountedItemQty	= infoSales.m_dQty;
	info.m_dVal					= dVal;
	info.m_nDiscountMode		= infoSubtotal.m_nDiscountMode;
	info.m_strDiscountType		= infoSubtotal.m_strDiscountType;
	info.m_nDiscountRate		= infoSubtotal.m_nDiscountRate;
	info.m_bVATable				= infoSubtotal.m_bVATable;
	m_TransactionInfo.m_arrayDiscountHelperToWrite.Consolidate( info );
}

/**********************************************************************/

void CCashRSPImporter::ProcessDiscounts()
{
	ProcessDiscounts_Item();
	ProcessDiscounts_LegacySubtotal();
	ProcessDiscounts_MMDisc();
	ProcessDiscounts_PDisc();
}

/**********************************************************************/

void CCashRSPImporter::ProcessDiscounts_Item()
{
	m_TransactionInfo.m_arrayDiscountHelperToWrite.RemoveAll();

	//APPLY ITEM DISCOUNTS BEFORE DISTRIBUTING SUBTOTAL DISCOUNT
	for (int n = 0; n < m_TransactionInfo.m_arrayDiscountHelperPendingItem.GetSize(); n++)
	{
		CCashRSPImporterDiscountHelperPendingItem infoDiscount;
		m_TransactionInfo.m_arrayDiscountHelperPendingItem.GetAt(n, infoDiscount);

		//FIND THE FIRST AND LAST SALES LINES THAT MATCH THE PLU NUMBER
		//AND CATEGORY, IRRESPECTIVE OF SERVER NUMBER
		int nStartIdx;
		CCashRSPImporterDiscountHelperSales infoStart;
		infoStart.m_nPluNo = infoDiscount.m_nPluNo;
		infoStart.m_nAnalysisCategory = infoDiscount.m_nAnalysisCategory;
		infoStart.m_nItemServerNo = 0;
		m_TransactionInfo.m_arrayDiscountHelperSales.Find(infoStart, nStartIdx);

		int nEndIdx;
		CCashRSPImporterDiscountHelperSales infoEnd = infoStart;
		infoEnd.m_nAnalysisCategory++;
		m_TransactionInfo.m_arrayDiscountHelperSales.Find(infoEnd, nEndIdx);
		nEndIdx--;

		if (nEndIdx >= nStartIdx)
		{
			CCashRSPImporterDiscountShareList DiscountShare;

			for (int nIdx = nStartIdx; nIdx <= nEndIdx; nIdx++)
			{
				CCashRSPImporterDiscountHelperSales infoSales;
				m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(nIdx, infoSales);
				DiscountShare.AddItem(nIdx, infoSales.m_dVal, TRUE);
			}

			if (DiscountShare.GotDiscountableSales() == TRUE)
			{
				DiscountShare.ShareDiscount(infoDiscount.m_dAmountAsDouble);

				for (int n = 0; n < DiscountShare.GetItemCount(); n++)
				{
					ConsolidateItemDiscountToWrite(DiscountShare, n, infoDiscount);
				}
			}
			else
			{
				ConsolidateItemDiscountToWrite(DiscountShare, 0, infoDiscount, infoDiscount.m_dAmountAsDouble);
			}
		}

		if (GetCurrentTransactionVersionNo() >= 26)
		{
			CReasonInfo ReasonInfo;
			ReasonInfo.SetReasonType(SS_REASON_DISCOUNT_ITEM);
			ReasonInfo.SetReasonCode(infoDiscount.m_nReasonCode);
			ReasonInfo.SetReasonText(infoDiscount.m_strReasonText);
			ReasonInfo.SetTimeTran(m_TransactionInfo.m_strTimeTran);
			ReasonInfo.SetTimeAction(m_TransactionInfo.m_strTimeTran);
			ReasonInfo.SetTranSeqNo(m_TransactionInfo.m_nSeqNo);
			ReasonInfo.SetTranTableNo(m_TransactionInfo.m_nTableNo);
			ReasonInfo.SetTranCovers(m_TransactionInfo.m_nCovers);
			ReasonInfo.SetTranServerNo(m_TransactionInfo.m_nServerNo);
			ReasonInfo.SetIDiscServerNo(infoDiscount.m_nDiscountServerNo);
			ReasonInfo.SetIDiscPluNo(infoDiscount.m_nPluNo);
			ReasonInfo.SetIDiscPromoNo(infoDiscount.m_nPromoNo);
			ReasonInfo.SetIDiscACatNo(infoDiscount.m_nAnalysisCategory);
			ReasonInfo.SetIDiscType(infoDiscount.m_strDiscountType);
			ReasonInfo.SetIDiscRate(infoDiscount.m_nDiscountRate);
			ReasonInfo.SetIDiscAmount(infoDiscount.m_nAmountAsInt);

			CCSV csvReason(',', '"', FALSE, FALSE, TRUE);
			ReasonInfo.SaveCSVLine(csvReason);

			{
				CCashRSPImporterDailyLogPluItem item;
				item.m_strLineSale = csvReason.GetLine();
				item.m_strActionDate = m_TransactionInfo.m_strDateTran;
				m_TransactionInfo.m_bufferDailyLogReasons.Add(item);
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessDiscounts_LegacySubtotal()
{
	//DISTRIBUTE SUBTOTAL DISCOUNTS BY ITEM
	if ((m_TransactionInfo.m_dTotalSalesValue != 0.0) && (m_TransactionInfo.m_dSubtotalDiscountValue + m_TransactionInfo.m_dLoyaltyDiscountValue != 0.0))
	{
		CString str = "";
		str.Format("Legacy subtotal discount %.2f", m_TransactionInfo.m_dSubtotalDiscountValue + m_TransactionInfo.m_dLoyaltyDiscountValue);
		Diagnostic(str);

		for (int d = 0; d < m_TransactionInfo.m_arrayDiscountHelperPendingSubtotal.GetSize(); d++)
		{
			CCashRSPImporterDiscountHelperPendingSubtotal infoSubtotal;
			m_TransactionInfo.m_arrayDiscountHelperPendingSubtotal.GetAt(d, infoSubtotal);

			bool bIncludeCategory[MAX_ACAT_NO + 1];
			for (int c = 0; c <= MAX_ACAT_NO; c++)
			{
				bIncludeCategory[c] = FALSE;
			}

			bool bAllCategories = TRUE;

			if ((infoSubtotal.m_strCategories != "0") && (infoSubtotal.m_strCategories != ""))
			{
				CCSV csvCategories(infoSubtotal.m_strCategories, ';');

				for (int x = 0; (x < csvCategories.GetSize()) && (x <= MAX_ACAT_NO); x++)
				{
					int c = csvCategories.GetInt(x);
					if ((c >= 1) && (c <= MAX_ACAT_NO))
					{
						bIncludeCategory[c] = TRUE;
						bAllCategories = FALSE;
					}
				}
			}

			CCashRSPImporterDiscountShareList DiscountShare;

			for (int nSaleIdx = 0; nSaleIdx < m_TransactionInfo.m_arrayDiscountHelperSales.GetSize(); nSaleIdx++)
			{
				CCashRSPImporterDiscountHelperSales infoSales;
				m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(nSaleIdx, infoSales);

				if (infoSales.m_dVal != 0.0)
				{
					bool bAllowDiscount = TRUE;

					if (FALSE == bAllCategories)
					{
						if ((infoSales.m_nAnalysisCategory < 0) || (infoSales.m_nAnalysisCategory > MAX_ACAT_NO))
						{
							bAllowDiscount = FALSE;
						}
						else
						{
							bAllowDiscount = bIncludeCategory[infoSales.m_nAnalysisCategory];
						}
					}

					DiscountShare.AddItem(nSaleIdx, infoSales.m_dVal, bAllowDiscount);
				}
			}

			DiscountShare.FilterList();

			if (DiscountShare.GotDiscountableSales() == TRUE)
			{
				DiscountShare.ShareDiscount(infoSubtotal.m_dVal);

				for (int n = 0; n < DiscountShare.GetItemCount(); n++)
				{
					ConsolidateDiscountToWrite(DiscountShare.GetIndex(n), infoSubtotal, DiscountShare.GetDiscountShare(n));
				}
			}
			else
			{
				ProcessSeparateDiscount(infoSubtotal.m_dVal, infoSubtotal.m_nDiscountMode, infoSubtotal.m_strDiscountType, infoSubtotal.m_nDiscountRate, FALSE);
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessDiscounts_PDisc()
{
	//RECALCULATE PROMO DISCOUNTS WITH REFERENCE TO NON MM TOTALS
	{
		int nCurrentPromoNo = -1;
		CCashRSPImporterDiscountShareList DiscountShare;

		if (m_TransactionInfo.m_arrayPromoItems.GetSize() > 0)
		{
			CString str = "";
			str.Format("Process $PDISC items %d", m_TransactionInfo.m_arrayPromoItems.GetSize());
			Diagnostic(str);
		}

		for (int n = 0; n <= m_TransactionInfo.m_arrayPromoItems.GetSize(); n++)
		{
			CCashRSPPromoItems PromoItem;

			if (n < m_TransactionInfo.m_arrayPromoItems.GetSize())
			{
				m_TransactionInfo.m_arrayPromoItems.GetAt(n, PromoItem);
			}
			else
			{
				PromoItem.m_nPromoNo = -2;
			}

			if (PromoItem.m_nPromoNo != nCurrentPromoNo)
			{
				if (nCurrentPromoNo != -1)
				{
					double dDiscountTotal = 0.0;
					CCashRSPPromoTotals PromoTotal;
					PromoTotal.m_nPromoNo = nCurrentPromoNo;

					int nPos = 0;
					if (m_TransactionInfo.m_arrayPromoTotals.Find(PromoTotal, nPos) == TRUE)
					{
						//DISCOUNT AMOUNT AT THIS STAGE IS IN PENNIES
						m_TransactionInfo.m_arrayPromoTotals.GetAt(nPos, PromoTotal);
						dDiscountTotal = PromoTotal.m_nPromoTotal - PromoTotal.m_nPromoMMTotal;
					}

					DiscountShare.ShareDiscount(dDiscountTotal);

					for (int x = 0; x < DiscountShare.GetItemCount(); x++)
					{
						int nIndex = DiscountShare.GetIndex(x);
						CCashRSPPromoItems PromoItem2;
						m_TransactionInfo.m_arrayPromoItems.GetAt(nIndex, PromoItem2);
						PromoItem2.m_dAmount = DiscountShare.GetDiscountShare(x);
						m_TransactionInfo.m_arrayPromoItems.SetAt(nIndex, PromoItem2);
					}
				}

				DiscountShare.Reset();
				nCurrentPromoNo = PromoItem.m_nPromoNo;
			}

			DiscountShare.AddItem(n, PromoItem.m_dAmount, TRUE);
		}
	}

	for (int n = 0; n < m_TransactionInfo.m_arrayPromoItems.GetSize(); n++)
	{
		CCashRSPPromoItems PromoItem;
		m_TransactionInfo.m_arrayPromoItems.GetAt(n, PromoItem);
		double dDiscountAmount = PromoItem.m_dAmount / 100.0;

		int nDiscountMode = DISCOUNT_MODE_NONE;
		{
			CString strMode = PromoItem.m_strDiscountMode;
			strMode.MakeUpper();

			if (strMode == "ST")
			{
				nDiscountMode = DISCOUNT_MODE_SUBTOTAL;
			}
			else if (strMode == "LO")
			{
				nDiscountMode = DISCOUNT_MODE_LOYALTY;
			}
			else if (strMode == "DF")
			{
				nDiscountMode = DISCOUNT_MODE_DIFFER;
			}
			else if (strMode == "MM")
			{
				nDiscountMode = DISCOUNT_MODE_MIXMATCH;
			}
		}

		m_TransactionInfo.SetCurrentPromoNo(PromoItem.m_nPromoNo, m_arrayPromotions);

		__int64 nPluNo = PromoItem.m_nPluNo;
		CCashRSPImporterDiscountHelperSales infoSale;
		infoSale.m_nPluNo = nPluNo;
		infoSale.m_nAnalysisCategory = 0;
		infoSale.m_nItemServerNo = 0;
		infoSale.m_strSaleDate = "";
		infoSale.m_strSaleTime = "";

		int nSaleIdxStart = 0;
		m_TransactionInfo.m_arrayDiscountHelperSales.Find(infoSale, nSaleIdxStart);

		int nSaleIdxEnd = 0;
		infoSale.m_nPluNo++;
		m_TransactionInfo.m_arrayDiscountHelperSales.Find(infoSale, nSaleIdxEnd);
		nSaleIdxEnd--;

		bool bReportAsPlu = FALSE;
		bool bUseMMPromo = FALSE;
		bool bTaxablePromo = PromoItem.m_bMicrotrendTaxFlag;

		if (FALSE == bTaxablePromo)
		{
			if (TRUE == m_TransactionInfo.m_bCurrentPromoDiscountForceTaxable)
			{
				CString strDiscountType = PromoItem.m_strDiscountType;
				if (strDiscountType != "P")
				{
					bTaxablePromo = TRUE;
				}
			}
		}

		if (FALSE == bTaxablePromo)
		{
			if (m_TransactionInfo.m_nCurrentPromoPluType != PROMO_REPORTASPLU_NEVER)
			{
				bReportAsPlu = TRUE;
			}
		}
		
		if ( (nSaleIdxEnd < nSaleIdxStart) && ( FALSE == bReportAsPlu ) )
		{
			infoSale.m_nMMPluNo = PromoItem.m_nPluNo;

			CString strMMPluNo = "";
			strMMPluNo.Format("%I64d", infoSale.m_nMMPluNo);

			if (strMMPluNo.GetLength() > Super.MaxPluLen())
			{
				CString strTmp = "";
				strTmp += "9";
				strTmp += strMMPluNo.Right(Super.MaxPluLen() - 1);
				infoSale.m_nMMPluNo = _atoi64(strTmp);
			}

			infoSale.m_nPluNo = 0;
			m_TransactionInfo.m_arrayMMPromoDiscountHelperSales.Find(infoSale, nSaleIdxStart);

			infoSale.m_nMMPluNo++;
			m_TransactionInfo.m_arrayMMPromoDiscountHelperSales.Find(infoSale, nSaleIdxEnd);
			nSaleIdxEnd--;

			if (nSaleIdxEnd < nSaleIdxStart)
			{
				bReportAsPlu = TRUE;
				Diagnostic("Substitute $PDISC line NO MATCH");
			}
			else
			{
				Diagnostic("Substitute $PDISC line MATCH");
				bUseMMPromo = TRUE;
			}
		}

		if (TRUE == m_TransactionInfo.m_bCurrentPromoSCTaxFix)
		{
			ProcessSeparateDiscount(dDiscountAmount, nDiscountMode, PromoItem.m_strDiscountType, PromoItem.m_nDiscountRate, TRUE, "A");
		}
		else if (TRUE == bReportAsPlu)
		{
			ProcessSeparateDiscount(dDiscountAmount, nDiscountMode, PromoItem.m_strDiscountType, PromoItem.m_nDiscountRate, FALSE);
		}
		else
		{
			CCashRSPImporterDiscountShareList DiscountShare;

			for (int nSaleIdx = nSaleIdxStart; nSaleIdx <= nSaleIdxEnd; nSaleIdx++)
			{
				CCashRSPImporterDiscountHelperSales infoSale;
				
				if (FALSE == bUseMMPromo)
				{
					m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(nSaleIdx, infoSale);
				}
				else
				{
					m_TransactionInfo.m_arrayMMPromoDiscountHelperSales.GetAt(nSaleIdx, infoSale);
				}
				
				DiscountShare.AddItem(nSaleIdx, infoSale.m_dVal, TRUE);
			}

			int nShareEndIdx = DiscountShare.GetItemCount() - 1;

			if (DiscountShare.GotDiscountableSales() == TRUE)
			{
				DiscountShare.ShareDiscount(dDiscountAmount);
			}
			else
			{
				nShareEndIdx = -1;
			}

			for (int nIdx = 0; nIdx <= nShareEndIdx; nIdx++)
			{
				int nSaleIdx = DiscountShare.GetIndex(nIdx);

				CCashRSPImporterDiscountHelperSales infoSale;
				
				if (FALSE == bUseMMPromo)
				{
					m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(nSaleIdx, infoSale);
				}
				else
				{
					m_TransactionInfo.m_arrayMMPromoDiscountHelperSales.GetAt(nSaleIdx, infoSale);
				}

				double dDiscountShare = (0 == nShareEndIdx) ? dDiscountAmount : DiscountShare.GetDiscountShare(nIdx);

				if (PROMO_REPORTASPLU_ALWAYS == m_TransactionInfo.m_nCurrentPromoPluType)
				{
					ProcessSeparateDiscount(dDiscountShare, DISCOUNT_MODE_SUBTOTAL, PromoItem.m_strDiscountType, PromoItem.m_nDiscountRate, TRUE, infoSale.m_strTaxBand);
				}
				else
				{
					CString strTaxBand = infoSale.m_strTaxBand;

					if (FALSE == bTaxablePromo)
					{
						strTaxBand = "";
					}

					CCSV csvSale(',', '"', FALSE, FALSE, TRUE);
					csvSale.Add(CSVPLUDATA_PLU_DISCOUNT);				//LINE TYPE
					csvSale.Add(CASHRSP_SALES_TRADING);					//LEGACY TRAINING MODE
					csvSale.Add(m_TransactionInfo.m_strTimeTran);		//TRAN TIME
					csvSale.Add(infoSale.m_strSaleTime);				//SALE TIME
					csvSale.Add(infoSale.m_nPluNo);						//PLU NO
					csvSale.Add(PromoItem.m_nPromoNo);					//PROMO NO
					csvSale.Add(infoSale.m_nPriceBand);					//PRICE BAND
					csvSale.Add(0);										//SALES QTY
					csvSale.Add(infoSale.m_dQty, 5);					//DISCOUNT QTY
					csvSale.Add(dDiscountShare, 4);						//VALUE
					csvSale.Add(strTaxBand);							//TAX BAND
					csvSale.Add(m_TransactionInfo.m_nCovers);			//COVERS
					csvSale.Add(infoSale.m_nDeptNo);					//DEPT NO
					csvSale.Add(infoSale.m_nGroupNo + 1);				//GROUP NO
					csvSale.Add(infoSale.m_nAnalysisCategory);			//CATEGORY
					csvSale.Add(m_TransactionInfo.m_nServerNo);			//TRAN SERVER
					csvSale.Add(infoSale.m_nItemServerNo);				//ITEM SERVER
					csvSale.Add(m_TransactionInfo.m_nCustomerNo);		//CUSTOMER NO
					csvSale.Add(m_TransactionInfo.m_nPaymentNo);		//PAYMENT NO
					csvSale.Add(0);										//MIX MATCH NO
					csvSale.Add(0);										//MIX MATCH TYPE
					csvSale.Add(0);										//WASTAGE
					csvSale.Add(nDiscountMode);							//DISCOUNT MODE
					csvSale.Add(PromoItem.m_strDiscountType);			//DISCOUNT TYPE
					csvSale.Add(PromoItem.m_nDiscountRate);				//DISCOUNT RATE
					csvSale.Add(infoSale.m_nVIPStatus);					//VIP FLAG
					csvSale.Add(m_TransactionInfo.m_nSeqNo);			//TRAN SEQ NO
					csvSale.Add(infoSale.m_strDescription);				//ITEM DESCRIPTION
					csvSale.Add(m_TransactionInfo.m_nTableNo);			//TABLE NO
					csvSale.Add(infoSale.m_strAttributeText);			//ATTRIBUTE TEXT
					csvSale.Add(m_TransactionInfo.m_strLoyaltyID);		//LOYALTY ID
					csvSale.Add(m_TransactionInfo.m_nLineTNo);			//LINE TNO
					csvSale.Add(0);										//REFUND FLAG
					csvSale.Add(0);										//VOID FLAG

					CCSV csvItemPay(',', '"', FALSE, FALSE, TRUE);
					csvItemPay.Add(1);
					csvItemPay.Add(CSVPLUDATA_PLU_DISCOUNT);				//LINE TYPE
					csvItemPay.Add(1);										//VERSION
					csvItemPay.Add(infoSale.m_nPluNo);						//PLU NO
					csvItemPay.Add(infoSale.m_nDeptNo);						//DEPT NO
					csvItemPay.Add(infoSale.m_nGroupNo + 1);				//GROUP NO
					csvItemPay.Add(dDiscountShare, 4);						//VALUE
					csvItemPay.Add(infoSale.m_nPriceBand);					//PRICE BAND
					csvItemPay.Add(strTaxBand);								//VAT BAND

					CCashRSPImporterDailyLogPluItem item;
					item.m_strLineSale = csvSale.GetLine();
					item.m_strLineItemPay = csvItemPay.GetLine();
					item.m_strActionDate = infoSale.m_strSaleDate;
					item.m_nMixMatchType = 0;
					item.m_nMixMatchOfferNo = 0;
					m_TransactionInfo.m_bufferDailyLogSales.Add(item);
				}
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessDiscounts_MMDisc()
{
	//RECALCULATE MMDISC AMOUNTS WITH REFERENCE TO SPECIAL $ITEM LINES
	{
		int nCurrentMMOfferNo = -1;
		int nCurrentMMPromoNo = -1;
		CCashRSPImporterDiscountShareList DiscountShare;

		if (m_TransactionInfo.m_arrayMixMatchItems.GetSize() > 0)
		{
			CString str = "";
			str.Format("Process $MMDISC items %d", m_TransactionInfo.m_arrayMixMatchItems.GetSize());
			Diagnostic(str);
		}

		for (int n = 0; n <= m_TransactionInfo.m_arrayMixMatchItems.GetSize(); n++)
		{
			CCashRSPMixMatchItems MixMatchItem1;
			
			if (n < m_TransactionInfo.m_arrayMixMatchItems.GetSize())
			{
				m_TransactionInfo.m_arrayMixMatchItems.GetAt(n, MixMatchItem1);
			}
			else
			{
				MixMatchItem1.m_nOfferNo = -2;
			}

			if ( (MixMatchItem1.m_nOfferNo != nCurrentMMOfferNo) || (MixMatchItem1.m_nPromoNo != nCurrentMMPromoNo ) )
			{
				if (nCurrentMMOfferNo != -1)
				{
					double dDiscountTotal = 0.0;
					
					CCashRSPMixMatchTotals MixMatchTotal;
					MixMatchTotal.m_nPromoNo = nCurrentMMPromoNo;
					MixMatchTotal.m_nOfferNo = nCurrentMMOfferNo;

					{
						int nPos = 0;
						if (m_TransactionInfo.m_arrayMixMatchTotals.Find(MixMatchTotal, nPos) == TRUE)
						{
							m_TransactionInfo.m_arrayMixMatchTotals.GetAt(nPos, MixMatchTotal);
							dDiscountTotal = MixMatchTotal.GetAmountAsDouble() / 100.0;
							dDiscountTotal *= -1;
						}
					}

					DiscountShare.ShareDiscount(dDiscountTotal);

					for (int x = 0; x < DiscountShare.GetItemCount(); x++)
					{
						int nIndex = DiscountShare.GetIndex(x);
						CCashRSPMixMatchItems MixMatchItem2;
						m_TransactionInfo.m_arrayMixMatchItems.GetAt(nIndex, MixMatchItem2);
						MixMatchItem2.m_dAmount = DiscountShare.GetDiscountShare(x);
						m_TransactionInfo.m_arrayMixMatchItems.SetAt(nIndex, MixMatchItem2);
					}
				}

				DiscountShare.Reset();
				nCurrentMMOfferNo = MixMatchItem1.m_nOfferNo;
				nCurrentMMPromoNo = MixMatchItem1.m_nPromoNo;
			}

			DiscountShare.AddItem(n, MixMatchItem1.m_dAmount, TRUE);
		}
	}

	//PROCESS MIX MATCH DISCOUNTS
	for (int n = 0; n < m_TransactionInfo.m_arrayMixMatchItems.GetSize(); n++)
	{
		CCashRSPMixMatchItems MixMatchItem;
		m_TransactionInfo.m_arrayMixMatchItems.GetAt(n, MixMatchItem);

		int nMixMatchType = MixMatchItem.m_nOfferType;
		int nMixMatchNo = MixMatchItem.m_nOfferNo;
		double dDiscountAmount = MixMatchItem.m_dAmount;
		
		__int64 nPluNo = MixMatchItem.m_nPluNo;
		CCashRSPImporterDiscountHelperSales infoSale;
		infoSale.m_nPluNo = nPluNo;
		infoSale.m_nAnalysisCategory = 0;
		infoSale.m_nItemServerNo = 0;
		infoSale.m_strSaleDate = "";
		infoSale.m_strSaleTime = "";

		int nSaleIdxStart;
		m_TransactionInfo.m_arrayDiscountHelperSales.Find(infoSale, nSaleIdxStart);

		int nSaleIdxEnd;
		infoSale.m_nPluNo++;
		m_TransactionInfo.m_arrayDiscountHelperSales.Find(infoSale, nSaleIdxEnd);
		nSaleIdxEnd--;

		CString strTaxBand = MixMatchItem.m_strTaxBand;

		if (strTaxBand == "0")
		{
			ProcessSeparateDiscount(dDiscountAmount, DISCOUNT_MODE_MIXMATCH, "M", MixMatchItem.m_nDiscountRate, FALSE, "", nMixMatchType, nMixMatchNo);
		}
		else if (nSaleIdxEnd < nSaleIdxStart)
		{
			ProcessSeparateDiscount(dDiscountAmount, DISCOUNT_MODE_MIXMATCH, "M", MixMatchItem.m_nDiscountRate, FALSE, "", nMixMatchType, nMixMatchNo);
		}
		else
		{
			CCashRSPImporterDiscountShareList DiscountShare;

			for (int nSaleIdx = nSaleIdxStart; nSaleIdx <= nSaleIdxEnd; nSaleIdx++)
			{
				CCashRSPImporterDiscountHelperSales infoSale;
				m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(nSaleIdx, infoSale);
				DiscountShare.AddItem(nSaleIdx, infoSale.m_dVal, TRUE);
			}

			int nShareEndIdx = DiscountShare.GetItemCount() - 1;

			if (DiscountShare.GotDiscountableSales() == TRUE)
			{
				DiscountShare.ShareDiscount(dDiscountAmount);
			}
			else
			{
				nShareEndIdx = -1;
			}

			__int64 nMMPluNo = 0;

			if ( 0 == MixMatchItem.m_nPromoNo)
			{
				CCashRSPMixMatchTotals MixMatchTotal;
				MixMatchTotal.m_nPromoNo = 0;
				MixMatchTotal.m_nOfferNo = MixMatchItem.m_nOfferNo;
				
				int nPos = 0;
				if (m_TransactionInfo.m_arrayMixMatchTotals.Find(MixMatchTotal, nPos) == TRUE)
				{
					m_TransactionInfo.m_arrayMixMatchTotals.GetAt(nPos, MixMatchTotal);
					nMMPluNo = MixMatchTotal.m_nPluNo;
				}
			}

			for (int nIdx = 0; nIdx <= nShareEndIdx; nIdx++)
			{
				int nSaleIdx = DiscountShare.GetIndex(nIdx);

				CCashRSPImporterDiscountHelperSales infoSale;
				m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(nSaleIdx, infoSale);

				double dDiscountShare = (0 == nShareEndIdx) ? dDiscountAmount : DiscountShare.GetDiscountShare(nIdx);

				if (nMMPluNo != 0)
				{
					CCashRSPImporterDiscountHelperSales MMDiscountItem = infoSale;
					MMDiscountItem.m_nMMPluNo = nMMPluNo;
					MMDiscountItem.m_dVal = dDiscountShare;
					MMDiscountItem.m_dQty = 0.0;
					m_TransactionInfo.m_arrayMMPromoDiscountHelperSales.Consolidate(MMDiscountItem);
				}

				{
					CCSV csvSale(',', '"', FALSE, FALSE, TRUE);
					csvSale.Add(CSVPLUDATA_PLU_MIXMATCH);						//LINE TYPE
					csvSale.Add(CASHRSP_SALES_TRADING);						//LEGACY TRAINING MODE
					csvSale.Add(m_TransactionInfo.m_strTimeTran);				//TRAN TIME
					csvSale.Add(infoSale.m_strSaleTime);						//SALE TIME
					csvSale.Add(infoSale.m_nPluNo);							//PLU NO
					csvSale.Add(0);											//PROMO NO
					csvSale.Add(infoSale.m_nPriceBand);						//PRICE BAND
					csvSale.Add(0);											//SALES QTY
					csvSale.Add(infoSale.m_dQty, 5);							//DISCOUNTED QTY
					csvSale.Add(dDiscountShare, 4);							//VALUE
					csvSale.Add(infoSale.m_strTaxBand);						//TAX BAND
					csvSale.Add(m_TransactionInfo.m_nCovers);					//COVERS
					csvSale.Add(infoSale.m_nDeptNo);							//DEPT NO
					csvSale.Add(infoSale.m_nGroupNo + 1);						//GROUP NO
					csvSale.Add(infoSale.m_nAnalysisCategory);				//CATEGORY
					csvSale.Add(m_TransactionInfo.m_nServerNo);				//TRAN SERVER
					csvSale.Add(infoSale.m_nItemServerNo);					//ITEM SERVER
					csvSale.Add(m_TransactionInfo.m_nCustomerNo);				//CUSTOMER NO
					csvSale.Add(m_TransactionInfo.m_nPaymentNo);				//PAYMENT NO
					csvSale.Add(nMixMatchNo);									//MIX MATCH NO
					csvSale.Add(nMixMatchType);								//MIX MATCH TYPE
					csvSale.Add(0);											//WASTAGE
					csvSale.Add(DISCOUNT_MODE_MIXMATCH);						//DISCOUNT MODE
					csvSale.Add("M");											//DISCOUNT TYPE
					csvSale.Add(MixMatchItem.m_nDiscountRate);					//DISCOUNT RATE
					csvSale.Add(infoSale.m_nVIPStatus);						//VIP FLAG
					csvSale.Add(m_TransactionInfo.m_nSeqNo);				//TRAN SEQ NO
					csvSale.Add(infoSale.m_strDescription);					//DESCRIPTION
					csvSale.Add(m_TransactionInfo.m_nTableNo);				//TABLE NO
					csvSale.Add(infoSale.m_strAttributeText);				//ATTRIBUTE TEXT
					csvSale.Add(m_TransactionInfo.m_strLoyaltyID);		//LOYALTY ID
					csvSale.Add(m_TransactionInfo.m_nLineTNo);			//LINE TNO
					csvSale.Add(0);											//REFUND FLAG
					csvSale.Add(0);										//VOID FLAG

					CCSV csvItemPay(',', '"', FALSE, FALSE, TRUE);
					csvItemPay.Add(1);
					csvItemPay.Add(CSVPLUDATA_PLU_MIXMATCH);						//LINE TYPE
					csvItemPay.Add(1);											//VERSION
					csvItemPay.Add(infoSale.m_nPluNo);							//PLU NO
					csvItemPay.Add(infoSale.m_nDeptNo);							//DEPT NO
					csvItemPay.Add(infoSale.m_nGroupNo + 1);						//GROUP NO
					csvItemPay.Add(dDiscountShare, 4);							//VALUE
					csvItemPay.Add(infoSale.m_nPriceBand);						//PRICE BAND
					csvItemPay.Add(infoSale.m_strTaxBand);						//TAX BAND

					CCashRSPImporterDailyLogPluItem item;
					item.m_strLineSale = csvSale.GetLine();
					item.m_strLineItemPay = csvItemPay.GetLine();
					item.m_strActionDate = infoSale.m_strSaleDate;
					m_TransactionInfo.m_bufferDailyLogSales.Add(item);
				}
			}
		}
	}

	//WRITE ITEM DISCOUNT LINES TO SALES HISTORY
	for (int n = 0; n < m_TransactionInfo.m_arrayDiscountHelperToWrite.GetSize(); n++)
	{
		CCashRSPImporterDiscountHelperToWrite infoDiscount;
		m_TransactionInfo.m_arrayDiscountHelperToWrite.GetAt(n, infoDiscount);

		if (infoDiscount.m_dVal != 0.0)
		{
			int nPriceBand = 0;
			int nDeptNo = 0;
			int nGroupNo = 0;
			int nItemServerNo = 0;
			CString strTaxBand = "";
			CString strSaleDate = "";
			CString strSaleTime = "";

			if (infoDiscount.m_nSaleIdx != -1)
			{
				CCashRSPImporterDiscountHelperSales infoSale;
				m_TransactionInfo.m_arrayDiscountHelperSales.GetAt(infoDiscount.m_nSaleIdx, infoSale);
				nPriceBand = infoSale.m_nPriceBand;
				nDeptNo = infoSale.m_nDeptNo;
				nGroupNo = infoSale.m_nGroupNo;

				if (TRUE == infoDiscount.m_bVATable)
				{
					strTaxBand = infoSale.m_strTaxBand;
				}

				nItemServerNo = infoSale.m_nItemServerNo;
				strSaleDate = infoSale.m_strSaleDate;
				strSaleTime = infoSale.m_strSaleTime;
			}

			{
				CCSV csvSale(',', '"', FALSE, FALSE, TRUE);
				csvSale.Add(CSVPLUDATA_PLU_DISCOUNT);				//LINE TYPE
				csvSale.Add(CASHRSP_SALES_TRADING);				//LEGACY TRAINING MODE
				csvSale.Add(m_TransactionInfo.m_strTimeTran);		//TRAN TIME
				csvSale.Add(strSaleTime);							//SALE TIME
				csvSale.Add(infoDiscount.m_nPluNo);				//PLU NO
				csvSale.Add(infoDiscount.m_nPromoNo);				//PROMO NO
				csvSale.Add(nPriceBand);							//PRICE BAND
				csvSale.Add(0);									//SALES QTY
				csvSale.Add(infoDiscount.m_dDiscountedItemQty, 5);//DISCOUNT QTY
				csvSale.Add(infoDiscount.m_dVal, 4);				//VALUE
				csvSale.Add(strTaxBand);							//TAX BAND
				csvSale.Add(m_TransactionInfo.m_nCovers);			//COVERS
				csvSale.Add(nDeptNo);								//DEPT NO
				csvSale.Add(nGroupNo + 1);							//GROUP NO
				csvSale.Add(infoDiscount.m_nAnalysisCategory);	//CATEGORY
				csvSale.Add(m_TransactionInfo.m_nServerNo);		//TRAN SERVER
				csvSale.Add(nItemServerNo);						//ITEM SERVER
				csvSale.Add(m_TransactionInfo.m_nCustomerNo);		//CUSTOMER NO
				csvSale.Add(m_TransactionInfo.m_nPaymentNo);		//PAYMENT NO
				csvSale.Add(0);									//MIX MATCH NO
				csvSale.Add(0);									//MIX MATCH TYPE
				csvSale.Add(0);									//WASTAGE
				csvSale.Add(infoDiscount.m_nDiscountMode);		//DISCOUNT MODE
				csvSale.Add(infoDiscount.m_strDiscountType);		//DISCOUNT TYPE
				csvSale.Add(infoDiscount.m_nDiscountRate);		//DISCOUNT RATE
				csvSale.Add(infoDiscount.m_nVIPStatus);			//VIP FLAG
				csvSale.Add(m_TransactionInfo.m_nSeqNo);			//TRAN SEQ NO
				csvSale.Add(infoDiscount.m_strItemDescription);	//ITEM DESCRIPTION
				csvSale.Add(m_TransactionInfo.m_nTableNo);		//TABLE NO
				csvSale.Add(infoDiscount.m_strAttributeText);	//ATTRIBUTE TEXT
				csvSale.Add(m_TransactionInfo.m_strLoyaltyID);		//LOYALTY ID
				csvSale.Add(m_TransactionInfo.m_nLineTNo);			//LINE TNO
				csvSale.Add(0);									//REFUND FLAG
				csvSale.Add(0);										//VOID FLAG

				CCSV csvItemPay(',', '"', FALSE, FALSE, TRUE);
				csvItemPay.Add(1);
				csvItemPay.Add(CSVPLUDATA_PLU_DISCOUNT);					//LINE TYPE
				csvItemPay.Add(1);										//VERSION
				csvItemPay.Add(infoDiscount.m_nPluNo);					//PLU NO
				csvItemPay.Add(nDeptNo);									//DEPT NO
				csvItemPay.Add(nGroupNo + 1);								//GROUP NO
				csvItemPay.Add(infoDiscount.m_dVal, 4);					//VALUE
				csvItemPay.Add(nPriceBand);								//PRICE BAND
				csvItemPay.Add(strTaxBand);								//TAX BAND

				CCashRSPImporterDailyLogPluItem item;
				item.m_strLineSale = csvSale.GetLine();
				item.m_strLineItemPay = csvItemPay.GetLine();
				item.m_strActionDate = strSaleDate;
				m_TransactionInfo.m_bufferDailyLogSales.Add(item);
			}
		}
	}

	for (int n = 0; n < m_TransactionInfo.m_arrayCheapestOneFree.GetSize(); n++)
	{
		CCashRSPImporterDiscountHelperSales infoDiscount;
		m_TransactionInfo.m_arrayCheapestOneFree.GetAt(n, infoDiscount);

		CCSV csvSale(',', '"', FALSE, FALSE, TRUE);
		csvSale.Add(CSVPLUDATA_PLU_CHEAPESTFREE);				//LINE TYPE
		csvSale.Add(CASHRSP_SALES_TRADING);					//LEGACY TRAINING MODE
		csvSale.Add(m_TransactionInfo.m_strTimeTran);			//TRAN TIME
		csvSale.Add(infoDiscount.m_strSaleTime);				//SALE TIME
		csvSale.Add(infoDiscount.m_nPluNo);					//PLU NO
		csvSale.Add(0);										//PROMO NO
		csvSale.Add(infoDiscount.m_nPriceBand);				//PRICE BAND
		csvSale.Add(0);										//SALES QTY
		csvSale.Add(infoDiscount.m_dQty, 5);					//DISCOUNT QTY
		csvSale.Add(infoDiscount.m_dVal, 4);					//VALUE
		csvSale.Add(infoDiscount.m_strTaxBand);				//TAX BAND
		csvSale.Add(m_TransactionInfo.m_nCovers);				//COVERS
		csvSale.Add(infoDiscount.m_nDeptNo);					//DEPT NO
		csvSale.Add(infoDiscount.m_nGroupNo + 1);				//GROUP NO
		csvSale.Add(infoDiscount.m_nAnalysisCategory);		//CATEGORY
		csvSale.Add(m_TransactionInfo.m_nServerNo);			//TRAN SERVER
		csvSale.Add(infoDiscount.m_nItemServerNo);			//ITEM SERVER
		csvSale.Add(m_TransactionInfo.m_nCustomerNo);			//CUSTOMER NO
		csvSale.Add(m_TransactionInfo.m_nPaymentNo);			//PAYMENT NO
		csvSale.Add(infoDiscount.m_nMixMatchNo);				//MIX MATCH NO
		csvSale.Add(MIXMATCH_ACTION_FREE_ITEM);				//MIX MATCH TYPE
		csvSale.Add(0);										//WASTAGE
		csvSale.Add(0);										//DISCOUNT MODE
		csvSale.Add("");										//DISCOUNT TYPE
		csvSale.Add(0);										//DISCOUNT RATE
		csvSale.Add(1);										//VIP FLAG
		csvSale.Add(m_TransactionInfo.m_nSeqNo);				//TRAN SEQ NO
		csvSale.Add(infoDiscount.m_strDescription);			//DESCRIPTION
		csvSale.Add(m_TransactionInfo.m_nTableNo);			//TABLE NO
		csvSale.Add(infoDiscount.m_strAttributeText);			//ATTRIBUTE TEXT
		csvSale.Add(m_TransactionInfo.m_strLoyaltyID);		//LOYALTY ID
		csvSale.Add(m_TransactionInfo.m_nLineTNo);			//LINE TNO
		csvSale.Add(0);										//REFUND FLAG
		csvSale.Add(0);										//VOID FLAG

		CCashRSPImporterDailyLogPluItem item;
		item.m_strLineSale = csvSale.GetLine();
		item.m_strLineItemPay = "";
		item.m_strActionDate = infoDiscount.m_strSaleDate;
		m_TransactionInfo.m_bufferDailyLogSales.Add(item);
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessTransactionPostToAccount( CCSVEposTermLine& csvIn, int nType, __int64 nID )
{
	int nTender = csvIn.GetPaymentTender();
	int nAmount = csvIn.GetPaymentAmount();

	if ( ( nTender != 0 ) || ( nAmount != 0 ) )
	{
		double dTender = ( (double) nTender ) / 100.0;
		double dAmount = ( (double) nAmount ) / 100.0;

		CCSV csvOut ( ',', '"', FALSE, FALSE, TRUE );
		csvOut.Add( ACCOUNT_LINETYPE_CHARGE );
		csvOut.Add( nType );
		csvOut.Add( nID );
		csvOut.Add( dTender, 2 );
		csvOut.Add( dAmount, 2 );

		m_TransactionInfo.m_bufferDailyLogAccounts.Add( csvOut.GetLine() );
	}
}

/**********************************************************************/

void CCashRSPImporter::ProcessSeparateDiscount(double dValue, int nDiscountMode, const char* szDiscountType, int nDiscountRate, bool bTaxable, const char* szTaxBand, int nMixMatchType, int nMixMatchNo)
{
	int nDeptNo = 0;
	int nGroupNo = 0;
	int nPromoNo = 0;
	int nSalesQty = 0;

	int nLineType = 0;
	CString strTaxBand = "";

	if (nMixMatchType != 0)
	{
		nLineType = CSVPLUDATA_MIXMATCH_NONTAXABLE;
		strTaxBand = "";
	}
	else
	{
		nDeptNo = m_TransactionInfo.m_nCurrentPromoDeptNo;
		nPromoNo = m_TransactionInfo.m_nCurrentPromoNo;
		nSalesQty = m_TransactionInfo.m_nCurrentPromoSalesQty;

		int nDeptIdx;
		if (m_arrayDepartments.FindDeptByNumber(nDeptNo, nDeptIdx) == TRUE)
		{
			CDepartmentCSVRecord Dept;
			m_arrayDepartments.GetAt(nDeptIdx, Dept);
			nGroupNo = Dept.GetEposGroup();
		}

		if (TRUE == bTaxable)
		{
			nLineType = CSVPLUDATA_PROMO_TAXABLE;
			strTaxBand = szTaxBand;
		}
		else
		{
			nLineType = CSVPLUDATA_PROMO_NONTAXABLE;
			strTaxBand = "";
		}
	}

	{
		CCSV csvSale(',', '"', FALSE, FALSE, TRUE);
		csvSale.Add(nLineType);								//LINE TYPE
		csvSale.Add(CASHRSP_SALES_TRADING);					//LEGACY TRAINING MODE
		csvSale.Add(m_TransactionInfo.m_strTimeTran);			//TRAN TIME
		csvSale.Add(m_TransactionInfo.m_strTimeTran);			//SALE TIME (SAME AS TRAN TIME)
		csvSale.Add(0);										//PLU NO
		csvSale.Add(nPromoNo);								//PROMO NO
		csvSale.Add(0);										//PRICE BAND
		csvSale.Add(nSalesQty);								//SALES QTY
		csvSale.Add(0);										//DISCOUNT QTY
		csvSale.Add(dValue, 2);								//VALUE
		csvSale.Add(strTaxBand);								//TAX BAND
		csvSale.Add(m_TransactionInfo.m_nCovers);				//COVERS
		csvSale.Add(nDeptNo);									//DEPT NO
		csvSale.Add(nGroupNo + 1);							//GROUP NO
		csvSale.Add(0);										//CATEGORY
		csvSale.Add(m_TransactionInfo.m_nServerNo);			//TRAN SERVER
		csvSale.Add(m_TransactionInfo.m_nServerNo);			//ITEM SERVER
		csvSale.Add(m_TransactionInfo.m_nCustomerNo);			//CUSTOMER NO
		csvSale.Add(0);										//PAYMENT NO
		csvSale.Add(nMixMatchNo);							//MIX MATCH NO
		csvSale.Add(nMixMatchType);							//MIX MATCH TYPE
		csvSale.Add(0);										//WASTAGE
		csvSale.Add(nDiscountMode);							//DISCOUNT MODE
		csvSale.Add(szDiscountType);							//DISCOUNT TYPE
		csvSale.Add(nDiscountRate);							//DISCOUNT RATE
		csvSale.Add(1);										//VIP FLAG
		csvSale.Add(m_TransactionInfo.m_nSeqNo);				//TRAN SEQ NO
		csvSale.Add("");										//DESCRIPTION
		csvSale.Add(m_TransactionInfo.m_nTableNo);			//TABLE NO
		csvSale.Add("");										//ATTRIBUTE TEXT
		csvSale.Add(m_TransactionInfo.m_strLoyaltyID);		//LOYALTY ID
		csvSale.Add(m_TransactionInfo.m_nLineTNo);			//LINE TNO
		csvSale.Add(0);										//REFUND FLAG
		csvSale.Add(0);										//VOID FLAG

		CCSV csvItemPay(',', '"', FALSE, FALSE, TRUE);
		csvItemPay.Add(1);
		csvItemPay.Add(nLineType);								//LINE TYPE
		csvItemPay.Add(1);										//VERSION
		csvItemPay.Add(m_TransactionInfo.m_nCurrentPromoNo);		//PROMO NO
		csvItemPay.Add(m_TransactionInfo.m_nCurrentPromoDeptNo);	//DEPT NO
		csvItemPay.Add(nGroupNo + 1);								//GROUP NO
		csvItemPay.Add(dValue, 2);								//VALUE
		csvItemPay.Add(0);										//PRICE BAND
		csvItemPay.Add(strTaxBand);								//TAX BAND

		CCashRSPImporterDailyLogPluItem item;
		item.m_strLineSale = csvSale.GetLine();
		item.m_strLineItemPay = csvItemPay.GetLine();
		item.m_strActionDate = m_TransactionInfo.m_strDateTran;
		item.m_nMixMatchType = 0;
		item.m_nMixMatchOfferNo = 0;
		m_TransactionInfo.m_bufferDailyLogSales.Add(item);
	}
}

/**********************************************************************/

void CCashRSPImporter::OpenServerFile(int nServerDbIdx, int nServerLocIdx)
{
	if (nServerDbIdx == m_BatchInfo.m_nCurrentServerDbIdx)
	{
		if (nServerLocIdx == m_BatchInfo.m_nCurrentServerLocIdx)
		{
			return;
		}
	}

	CDataManagerInfo info;
	DataManagerNonDb.CloseServer(info);

	if (nServerDbIdx != -2)
	{
		DataManagerNonDb.OpenServer(DB_READONLY, nServerDbIdx, nServerLocIdx, info);
	}

	m_BatchInfo.m_nCurrentServerDbIdx = nServerDbIdx;
	m_BatchInfo.m_nCurrentServerLocIdx = nServerLocIdx;
}

/**********************************************************************/

bool CCashRSPImporter::GetDbAndLocIdx( int nNetworkIdx, int nExt, int& nDbIdx, int& nLocIdx )
{
	nLocIdx = NetworkTerminalMap.GetLocIdx( dbNetwork.GetNetworkNo( nNetworkIdx ), nExt );

	if ( nLocIdx < 0 )
	{
		nLocIdx = 0;
		nDbIdx = 0;
		return FALSE;
	}

	int nDbNo = dbLocation.GetDbNo( nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
	{
		nLocIdx = 0;
		nDbIdx = 0;
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CCashRSPImporter::GetServerFileInfo( int nNetworkIdx, int nExt, int& nServerDbIdx, int& nServerLocIdx )
{
	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_LOC:
	case SERVER_DB:
		break;

	case SERVER_SYS:
	default:
		nServerDbIdx = -1;
		nServerLocIdx = -1;
		return;
	}

	int nDbIdx, nLocIdx;
	if ( GetDbAndLocIdx( nNetworkIdx, nExt, nDbIdx, nLocIdx ) == FALSE )
	{
		nServerLocIdx = -2;
		nServerDbIdx = -2;
		return;
	}
	
	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_LOC:
		nServerLocIdx = nLocIdx;
		nServerDbIdx = nDbIdx;
		break;

	case SERVER_DB:
		nServerLocIdx = -1;
		nServerDbIdx = nDbIdx;
		break;

	default:
		nServerLocIdx = -2;
		nServerDbIdx = -2;
		break;
	}
}

/**********************************************************************/

void CCashRSPImporter::WriteDailyTermFile( bool& bGotDuplicate )
{
	bGotDuplicate = FALSE;
	bool bCatchNewDuplicates = ( CASHRSP_PROCESSTYPE_REBUILD != m_BatchInfo.m_nProcessType ) && ( DealerFlags.GetCashRSPCatchDuplicatesFlag() == TRUE );
	bool bLogNonDuplicates = ( m_bRebuildRemoveDuplicates || bCatchNewDuplicates ) && DealerFlags.GetLogNonDuplicatesFlag();
	
	if ( m_TransactionInfo.m_bufferDailyLogTerm.GetSize() != 0 )
	{
		if ( m_pOutputFileDailyTerm -> OpenFile() == TRUE )
		{
			if ( TRUE == bCatchNewDuplicates )
			{
				if ( m_pOutputFileDailyTerm -> GetLastOpenNewFileFlag() == TRUE )
				{
					m_BatchInfo.m_arrayRebuildHeaders.RemoveAll();

					CSSFile fileDailyTran;
					if ( fileDailyTran.Open( m_pOutputFileDailyTerm -> GetCurrentFilename(), "rb" ) == TRUE )
					{
						CString strBuffer;
						while( fileDailyTran.ReadString( strBuffer ) == TRUE )
						{
							CCSVEposTermLine csvIn ( strBuffer );

							if (DealerFlags.GetDuplicateCheckIgnoreTimeFlag() == TRUE)
							{
								switch (csvIn.GetLineTypeInt())
								{
								case CASHRSP_LINETYPE_TRAN_NORMAL:
								case CASHRSP_LINETYPE_TRAN_VOID:
								case CASHRSP_LINETYPE_WASTAGE:
									csvIn.SetAt(8, "");
									strBuffer = csvIn.GetLine();
									break;
								}
							}

							switch( csvIn.GetLineTypeInt() )
							{
							case CASHRSP_LINETYPE_EODHEADER:
							case CASHRSP_LINETYPE_TRAN_NORMAL:
							case CASHRSP_LINETYPE_TRAN_VOID:
							case CASHRSP_LINETYPE_WASTAGE:
							case CASHRSP_LINETYPE_NOSALE:
							case CASHRSP_LINETYPE_AGEVERIFY:
							case CASHRSP_LINETYPE_ATTENDANCE:
								{
									int nPos = 0;
									CSortedStringItem item;
									item.m_strItem = strBuffer;

									if (m_BatchInfo.m_arrayRebuildHeaders.Find(item, nPos) == FALSE)
									{
										m_BatchInfo.m_arrayRebuildHeaders.InsertAt(nPos, item);
									}
								}
								break;
							}
						}
					}
				}
				
				CString strHeader = "";
				for ( int n = 0; ( n < m_TransactionInfo.m_bufferDailyLogTerm.GetSize() ) && ( strHeader == "" ); n++ )
				{
					CString strLine = m_TransactionInfo.m_bufferDailyLogTerm.GetAt(n);
					CCSVEposTermLine csvIn ( strLine );

					switch( csvIn.GetLineTypeInt() )
					{
					case CASHRSP_LINETYPE_TRAN_NORMAL:
					case CASHRSP_LINETYPE_TRAN_VOID:
					case CASHRSP_LINETYPE_WASTAGE:
						if (DealerFlags.GetDuplicateCheckIgnoreTimeFlag() == TRUE)
						{
							csvIn.SetAt(8, "");
							strLine = csvIn.GetLine();
						}
						strHeader = strLine;
						break;
					
					case CASHRSP_LINETYPE_EODHEADER:
					case CASHRSP_LINETYPE_NOSALE:
					case CASHRSP_LINETYPE_AGEVERIFY:
					case CASHRSP_LINETYPE_ATTENDANCE:
						strHeader = strLine;
						break;
					}
				}

				if ( strHeader != "" )
				{
					int nPos = 0;
					CSortedStringItem item;
					item.m_strItem = strHeader;

					if (m_BatchInfo.m_arrayRebuildHeaders.Find(item, nPos) == FALSE)
					{
						m_BatchInfo.m_arrayRebuildHeaders.InsertAt(nPos, item);
					}
					else
					{
						bGotDuplicate = TRUE;

						CString strRepeatLogFile = m_pOutputFileDailyTerm->GetCurrentFilename();
						strRepeatLogFile.Insert(strRepeatLogFile.GetLength() - 4, "_REPEAT");

						CSSFile fileRepeat;
						if (fileRepeat.Open(strRepeatLogFile, "ab") == TRUE)
						{
							fileRepeat.WriteLine(strHeader);
						}
					}
				}
			}
				
			if ( FALSE == bGotDuplicate )
			{
				for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogTerm.GetSize(); n++)
				{
					m_pOutputFileDailyTerm->WriteLine(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(n));
				}

				if (TRUE == bLogNonDuplicates)
				{
					m_pOutputFileTermNonDuplicates->OpenFile();

					for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogTerm.GetSize(); n++)
					{
						m_pOutputFileTermNonDuplicates->WriteLine(m_TransactionInfo.m_bufferDailyLogTerm.GetAt(n));
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CCashRSPImporter::WriteDailySalesFiles()
{
	switch( m_BatchInfo.m_nProcessType )
	{
	case CASHRSP_PROCESSTYPE_NEWSALE:
	case CASHRSP_PROCESSTYPE_RETRY:
	case CASHRSP_PROCESSTYPE_REBUILD:
	{
		CReportConsolidationArray<CSortedIntByInt> arrayMMTypeByNo;

		//SALES AND DISCOUNTS BY ITEM
		if (m_TransactionInfo.m_bufferDailyLogSales.GetSize() != 0)
		{
			CCashRSPImporterOutputFile FileOtherDateSales(CASHRSP_DAILYFILE_SALES_ITEM, m_BatchInfo, m_TransactionInfo);

			if (m_pOutputFileDailySales->OpenFile() == TRUE)
			{
				CString strLastSaleDate = "";

				for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogSales.GetSize(); n++)
				{
					CCashRSPImporterDailyLogPluItem SaleItem = m_TransactionInfo.m_bufferDailyLogSales.GetAt(n);

					if ((m_TransactionInfo.m_strDateTran == SaleItem.m_strActionDate) || (SaleItem.m_strActionDate.GetLength() != 8))
					{
						m_pOutputFileDailySales->WriteLine(SaleItem.m_strLineSale);
					}
					else
					{
						m_TransactionInfo.m_strDateItem = SaleItem.m_strActionDate;

						if ((FileOtherDateSales.IsOpen() == FALSE) || (SaleItem.m_strActionDate != strLastSaleDate))
						{
							strLastSaleDate = SaleItem.m_strActionDate;
							FileOtherDateSales.OpenFile();
						}

						if (FileOtherDateSales.IsOpen() == TRUE)
						{
							FileOtherDateSales.WriteLine(SaleItem.m_strLineSale);
						}
					}

					if (m_TransactionInfo.m_bufferDailyLogMixMatchUsage.GetSize() != 0)
					{
						CPluSalesLine PluSale(FALSE);
						PluSale.ParseLine(SaleItem.m_strLineSale);

						int nMMNo = PluSale.GetMixMatchNo();
						if (nMMNo != 0)
						{
							CSortedIntByInt item;
							item.m_nKey = nMMNo;
							item.m_nVal = PluSale.GetMixMatchType();
							arrayMMTypeByNo.Consolidate(item);
						}
					}
				}
			}

			FileOtherDateSales.CloseFile();
		}

		//MIX MATCH USAGE
		if (m_TransactionInfo.m_bufferDailyLogMixMatchUsage.GetSize() != 0)
		{
			if (m_pOutputFileDailySales->OpenFile() == TRUE)
			{
				for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogMixMatchUsage.GetSize(); n++)
				{
					CString strLineToWrite = m_TransactionInfo.m_bufferDailyLogMixMatchUsage.GetAt(n);
					
					{
						CCSV csv(strLineToWrite);

						CSortedIntByInt item;
						item.m_nKey = csv.GetInt(7);
						
						int nPos = 0;
						if (arrayMMTypeByNo.Find(item, nPos) == TRUE)
						{
							arrayMMTypeByNo.GetAt(nPos, item);
							csv.SetAt(8, item.m_nVal);
							strLineToWrite = csv.GetLine();
						}
					}

					m_pOutputFileDailySales->WriteLine(strLineToWrite);
				}
			}
		}

		//REFUNDS AND VOIDS BY ITEM
		if (m_TransactionInfo.m_bufferDailyLogReasons.GetSize() != 0)
		{
			CCashRSPImporterOutputFile FileOtherDateReasons(CASHRSP_DAILYFILE_REASONS_ITEM, m_BatchInfo, m_TransactionInfo);

			if (m_pOutputFileDailyReasons->OpenFile() == TRUE)
			{
				CString strLastReasonDate = "";

				for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogReasons.GetSize(); n++)
				{
					CCashRSPImporterDailyLogPluItem ReasonItem = m_TransactionInfo.m_bufferDailyLogReasons.GetAt(n);

					if ((m_TransactionInfo.m_strDateTran == ReasonItem.m_strActionDate) || (ReasonItem.m_strActionDate.GetLength() != 8))
					{
						m_pOutputFileDailyReasons->WriteLine(ReasonItem.m_strLineSale);
					}
					else
					{
						m_TransactionInfo.m_strDateItem = ReasonItem.m_strActionDate;

						if ((FileOtherDateReasons.IsOpen() == FALSE) || (ReasonItem.m_strActionDate != strLastReasonDate))
						{
							strLastReasonDate = ReasonItem.m_strActionDate;
							FileOtherDateReasons.OpenFile();
						}

						if (FileOtherDateReasons.IsOpen() == TRUE)
						{
							FileOtherDateReasons.WriteLine(ReasonItem.m_strLineSale);
						}
					}
				}
			}

			FileOtherDateReasons.CloseFile();
		}

		//ACCOUNT FILES
		if ((m_TransactionInfo.m_bufferDailyLogAccounts.GetSize() != 0) || (m_TransactionInfo.m_nCustomerNo != 0))
		{
			if (m_pOutputFileDailyAccount->OpenFile() == TRUE)
			{
				CCSV csvOut(',', '"', FALSE, FALSE, TRUE);
				csvOut.Add(ACCOUNT_LINETYPE_HEADER);
				csvOut.Add(m_TransactionInfo.m_strTimeTran);
				csvOut.Add(m_TransactionInfo.m_nServerNo);
				m_pOutputFileDailyAccount->WriteLine(csvOut.GetLine());

				//CUSTOMER MARKER MUST BE FIRST LINE IN TRANSACTION BLOCK
				//SO THAT NON-CHARGE PAYMENTS MAY BE RECONCILED
				if (m_TransactionInfo.m_nCustomerNo != 0)
				{
					CCSV csvOut(',', '"', FALSE, FALSE, TRUE);
					csvOut.Add(ACCOUNT_LINETYPE_MARKER);
					csvOut.Add(CASHRSP_ITEMPAYTYPE_CUSTOMER);
					csvOut.Add(m_TransactionInfo.m_nCustomerNo);
					m_pOutputFileDailyAccount->WriteLine(csvOut.GetLine());
				}

				for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogAccounts.GetSize(); n++)
				{
					m_pOutputFileDailyAccount->WriteLine(m_TransactionInfo.m_bufferDailyLogAccounts.GetAt(n));
				}

				CString strPaymentLineStart = "";
				strPaymentLineStart.Format("%d,",
					(m_TransactionInfo.m_bGotNormalItem) ? ACCOUNT_LINETYPE_PAYMENT_MIXED : ACCOUNT_LINETYPE_PAYMENT_RA);

				for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogAccountsPay.GetSize(); n++)
				{
					CString strLine = strPaymentLineStart + m_TransactionInfo.m_bufferDailyLogAccountsPay.GetAt(n);
					m_pOutputFileDailyAccount->WriteLine(strLine);
				}
			}
		}

		//ITEM PAY FILES
		if (FALSE == m_TransactionInfo.m_bIsPaidIO)
		{
			if ((m_TransactionInfo.m_arrayLinePosPayment.GetSize() != 0) || (m_TransactionInfo.m_bufferDailyLogSales.GetSize() != 0))
			{
				if (m_pOutputFileDailyItemPay->OpenFile() == TRUE)
				{
					CCSV csvOut(',', '"', FALSE, FALSE, TRUE);
					csvOut.Add(0);
					csvOut.Add(1);
					csvOut.Add(m_TransactionInfo.m_strTimeTran);
					csvOut.Add(m_TransactionInfo.m_CashRSPVersionChecker.GetCurrentTransactionAppType());
					csvOut.Add(m_TransactionInfo.m_CashRSPVersionChecker.GetCurrentTransactionVersionNo());
					m_pOutputFileDailyItemPay->WriteLine(csvOut.GetLine());

					for (int n = 0; n < m_TransactionInfo.m_arrayLinePosPayment.GetSize(); n++)
					{
						int nLinePos = m_TransactionInfo.m_arrayLinePosPayment.GetAt(n);
						if ((nLinePos >= 0) && (nLinePos < m_TransactionInfo.m_bufferDailyLogTerm.GetSize()))
						{
							CString strLine = m_TransactionInfo.m_bufferDailyLogTerm.GetAt(nLinePos);

							if (strLine.Left(6) == "$PAYM,")
							{
								CString strTemp = strLine;
								strLine = "2";
								strLine += strTemp.Right(strTemp.GetLength() - 5);
								m_pOutputFileDailyItemPay->WriteLine(strLine);
							}
						}
					}

					for (int n = 0; n < m_TransactionInfo.m_bufferDailyLogSales.GetSize(); n++)
					{
						CCashRSPImporterDailyLogPluItem SaleItem = m_TransactionInfo.m_bufferDailyLogSales.GetAt(n);

						if (SaleItem.m_strLineItemPay != "")
						{
							m_pOutputFileDailyItemPay->WriteLine(SaleItem.m_strLineItemPay);
						}
					}

					if (m_TransactionInfo.m_nItemPayRAType != CASHRSP_ITEMPAYTYPE_NORMAL)
					{
						CCSV csvOut(',', '"', FALSE, FALSE, TRUE);
						csvOut.Add(3);
						csvOut.Add(1);
						csvOut.Add(m_TransactionInfo.m_nItemPayRAType);
						m_pOutputFileDailyItemPay->WriteLine(csvOut.GetLine());
					}
				}
			}
		}
	}
	break;
	}
}

/**********************************************************************/

void CCashRSPImporter::WritePendingStockFiles( int nConnectionType )
{
	if ( CONNECTION_TYPE_STANDARD_NONE == nConnectionType )
	{
		switch( m_BatchInfo.m_nProcessType )
		{
		case CASHRSP_PROCESSTYPE_NEWSALE:
		case CASHRSP_PROCESSTYPE_RETRY:
			{
				//SALES
				if ( m_TransactionInfo.m_bufferPendingStockSales.GetSize() != 0 )
				{
					if ( m_pOutputFileStockSales -> OpenFile() == TRUE )
					{
						for ( int n = 0; n < m_TransactionInfo.m_bufferPendingStockSales.GetSize(); n++ )
							m_pOutputFileStockSales -> WriteLine( m_TransactionInfo.m_bufferPendingStockSales.GetAt(n) );
					}
				}

				//DELIVERY
				if ( m_TransactionInfo.m_bufferPendingStockDelivery.GetSize() != 0 )
				{
					if ( m_pOutputFileStockDelivery -> OpenFile() == TRUE )
					{
						for ( int n = 0; n < m_TransactionInfo.m_bufferPendingStockDelivery.GetSize(); n++ )
							m_pOutputFileStockDelivery -> WriteLine( m_TransactionInfo.m_bufferPendingStockDelivery.GetAt(n) );
					}
				}
			}
			break;
		}
	}
}

/**********************************************************************/

int CCashRSPImporter::GetLocIdxByFilename( const char* szFilename, int nNetwkIdx )
{
	int nStartLocIdx, nEndLocIdx;
	dbLocation.GetNetworkLocIdxRange( nNetwkIdx, nStartLocIdx, nEndLocIdx );

	CString strFilename = szFilename;
	int nTNo = atoi( strFilename.Right(3) );

	int nLocIdx = 0;
	bool bGotMatch = FALSE;
	
	for ( nLocIdx = nStartLocIdx; nLocIdx <= nEndLocIdx; nLocIdx++ )
	{
		for ( int nTermIdx = 0; nTermIdx < dbLocation.GetTerminalCount( nLocIdx ); nTermIdx++ )
		{
			if ( dbLocation.GetTNo( nLocIdx, nTermIdx ) == nTNo )
			{
				bGotMatch = TRUE;
				break;
			}
		}

		if (TRUE == bGotMatch)
		{
			break;
		}
	}

	if (FALSE == bGotMatch)
	{
		nLocIdx = -1;
	}

	return nLocIdx;
}

/**********************************************************************/

void CCashRSPImporter::ProcessDepositFile( const char* szSourcePath, const char* szDestPath, const char* szFilename, int nNetwkIdx )
{
	int nLocIdx = GetLocIdxByFilename( szFilename, nNetwkIdx );

	if (nLocIdx != -1)
	{
		CString strSourceFile = "";
		strSourceFile = szSourcePath;
		strSourceFile += szFilename;

		CString strDestFile = "";
		strDestFile = szDestPath;
		strDestFile += szFilename;

		CSSFile fileSource;
		if (fileSource.Open(strSourceFile, "rb") == FALSE)
		{
			return;
		}

		CSSFile fileDest;
		if (fileDest.Open(strDestFile, "ab") == FALSE)
		{
			return;
		}

		bool bSavedInfo = FALSE;

		CDataManagerInfo info;
		if (DataManagerNonDb.OpenDepositNames(DB_READWRITE, nLocIdx, info) == FALSE)
		{
			return;
		}

		bool bNeedWrite = FALSE;
		CCashRSPImporterDepositInfo DepositInfo;
			
		CString strBuffer;
		while ( fileSource.ReadString( strBuffer ) == TRUE )
		{
			DepositInfo.ProcessLine( strBuffer );

			if (DepositInfo.GotCurrentDeposit() == TRUE)
			{
				if (ProcessDepositInfo(DepositInfo) == TRUE)
				{
					bNeedWrite = TRUE;
				}
			}

			DepositInfo.PostProcessLine();
		}

		fileSource.Close();

		DepositInfo.ForceDepositEnd();
		if (DepositInfo.GotCurrentDeposit() == TRUE)
		{
			if (ProcessDepositInfo(DepositInfo) == TRUE)
			{
				bNeedWrite = TRUE;
			}
		}

		if (FALSE == bNeedWrite)
		{
			bSavedInfo = TRUE;
		}
		else
		{
			bSavedInfo = DataManagerNonDb.WriteDepositNames(info);
		}

		DataManagerNonDb.CloseDepositNames( info );
			
		if (bSavedInfo == TRUE)
		{
			CFileRemover FileRemover(strSourceFile);
		}
	}
}

/**********************************************************************/

bool CCashRSPImporter::ProcessDepositInfo( CCashRSPImporterDepositInfo& DepositInfo )
{
	bool bResult = FALSE;

	CStringArray arrayTokens;
	CString strDate = "";
	CString strTime = "";
	CString strBooking = DepositInfo.GetBooking();
	::TokeniseSpacedLine( strBooking, arrayTokens );

	if (arrayTokens.GetSize() >= 1)
	{
		strDate = arrayTokens.GetAt(0);
	}

	if (arrayTokens.GetSize() >= 2)
	{
		strTime = arrayTokens.GetAt(1);
	}

	int nDepositID = DepositInfo.GetDepositID();
	if ((nDepositID > 0) && (nDepositID <= DepositName::DepositID.Max))
	{
		int nIdx = 0;
		if (DataManagerNonDb.DepositNames.FindDepositByID(nDepositID, nIdx) == FALSE)
		{
			CDepositNameCSVRecord Name;
			Name.SetDepositID(nDepositID);
			Name.SetServerNo(DepositInfo.GetServerNo());
			Name.SetDepositAmount(DepositInfo.GetDepositAmount());
			Name.SetTaxBand(DepositInfo.GetTaxBand());
			Name.SetTaxAmount(DepositInfo.GetTaxAmount());
			Name.SetBuyerName(DepositInfo.GetCustName());
			Name.SetBookingDate(strDate);
			Name.SetBookingTime(strTime);

			for (int n = 0; n < DEPOSITINFO_LINES; n++)
			{
				Name.SetCustomerInfo(n, DepositInfo.GetCustInfo(n));
			}

			DataManagerNonDb.DepositNames.InsertAt(nIdx, Name);
			bResult = TRUE;
		}
	}
	
	return bResult;
}

/**********************************************************************/

void CCashRSPImporter::ProcessEODDateFile(const char* szSourcePath, const char* szDestPath, const char* szFilename, int nNetwkIdx)
{
	int nLocIdx = GetLocIdxByFilename(szFilename, nNetwkIdx);

	if (nLocIdx != -1)
	{
		CString strSourceFile = "";
		strSourceFile = szSourcePath;
		strSourceFile += szFilename;

		CSSFile fileIn;
		if (fileIn.Open(strSourceFile, "rb") == FALSE)
		{
			return;
		}

		CSSFile fileOut;
		CFilenameUpdater FnUp(SysFiles::EODDates, nLocIdx);
		if (fileOut.Open(FnUp.GetFilenameToUse(), "ab") == FALSE)
		{
			return;
		}

		CString strBuffer = "";
		while (fileIn.ReadString(strBuffer) == TRUE)
		{
			int nLength = strBuffer.GetLength();
			if (strBuffer.GetLength() > 4)
			{
				strBuffer.MakeUpper();
				if (strBuffer.Left(4) == "EOD ")
				{
					fileOut.WriteLine(strBuffer.Right(nLength - 4));
				}
			}
		}

		fileIn.Close();
		fileOut.Close();
	}
}

/**********************************************************************/

bool CCashRSPImporter::CheckPromoRules()
{
	bool bResult = TRUE;

	for (int n = 0; n < m_TransactionInfo.m_arrayPromoTotals.GetSize(); n++)
	{
		CCashRSPPromoTotals PromoTotals;
		m_TransactionInfo.m_arrayPromoTotals.GetAt(n, PromoTotals);

		PromoTotals.m_bCanUsePromoRules = TRUE;

		if (PromoTotals.m_nPromoTotal != PromoTotals.m_nPromoMMTotal)
		{
			CCashRSPPromoItems PromoItem;
			PromoItem.m_nPromoNo = PromoTotals.m_nPromoNo;
			PromoItem.m_nPluNo = 0;

			int nPos1 = 0;
			m_TransactionInfo.m_arrayPromoItems.Find(PromoItem, nPos1);

			int nPos2 = 0;
			PromoItem.m_nPromoNo++;
			m_TransactionInfo.m_arrayPromoItems.Find(PromoItem, nPos2);

			if (nPos2 == nPos1)
			{
				PromoTotals.m_bCanUsePromoRules = FALSE;
				bResult = FALSE;
			}
		}

		if ((PromoTotals.m_nPromoMMTotal != 0) && (TRUE == PromoTotals.m_bCanUsePromoRules))
		{
			if (GotMMDiscLines(PromoTotals.m_nPromoNo, 0) == FALSE)
			{
				PromoTotals.m_bCanUsePromoRules = FALSE;
				bResult = FALSE;
			}
		}

		{
			CString str = "";
			str.Format("Promo rules check %d %d %d %s",
				PromoTotals.m_nPromoNo,
				PromoTotals.m_nPromoTotal,
				PromoTotals.m_nPromoMMTotal,
				PromoTotals.m_bCanUsePromoRules ? "YES" : "NO");

			Diagnostic(str);
		}

		m_TransactionInfo.m_arrayPromoTotals.SetAt(n, PromoTotals);

		if (FALSE == PromoTotals.m_bCanUsePromoRules)
		{
			for (int n = m_TransactionInfo.m_arrayPromoItems.GetSize() - 1; n >= 0; n--)
			{
				CCashRSPPromoItems PromoItem;
				m_TransactionInfo.m_arrayPromoItems.GetAt(n, PromoItem);

				if (PromoItem.m_nPromoNo == PromoTotals.m_nPromoNo)
				{
					m_TransactionInfo.m_arrayPromoItems.RemoveAt(n);
				}
			}

			for (int n = m_TransactionInfo.m_arrayMixMatchItems.GetSize() - 1; n >= 0; n--)
			{
				CCashRSPMixMatchItems MixMatchItem;
				m_TransactionInfo.m_arrayMixMatchItems.GetAt(n, MixMatchItem);

				if (MixMatchItem.m_nPromoNo == PromoTotals.m_nPromoNo)
				{
					m_TransactionInfo.m_arrayMixMatchItems.RemoveAt(n);
				}
			}
		}
	}

	return bResult;
}

/**********************************************************************/

bool CCashRSPImporter::CanUsePromoRules( int nPromoNo )
{
	bool bResult = FALSE;

	int nPos = 0;
	CCashRSPPromoTotals PromoTotals;
	PromoTotals.m_nPromoNo = nPromoNo;

	if (m_TransactionInfo.m_arrayPromoTotals.Find(PromoTotals, nPos) == TRUE)
	{
		m_TransactionInfo.m_arrayPromoTotals.GetAt(nPos, PromoTotals);
		bResult = PromoTotals.m_bCanUsePromoRules;
	}

	return bResult;
}

/**********************************************************************/

bool CCashRSPImporter::GotMMDiscLines(int nPromoNo, int nOfferNo )
{
	CCashRSPMixMatchItems MixMatchItem;
	MixMatchItem.m_nPromoNo = nPromoNo;
	MixMatchItem.m_nOfferNo = nOfferNo;
	MixMatchItem.m_nPluNo = 0;

	int nPos1 = 0;
	m_TransactionInfo.m_arrayMixMatchItems.Find(MixMatchItem, nPos1);

	int nPos2 = 0;
	
	if (0 == nOfferNo)
	{
		MixMatchItem.m_nPromoNo++;
	}
	else
	{
		MixMatchItem.m_nOfferNo++;
	}

	m_TransactionInfo.m_arrayMixMatchItems.Find(MixMatchItem, nPos2);

	return (nPos2 > nPos1);
}

/**********************************************************************/

void CCashRSPImporter::ProcessMixMatchUseLine(CCSVEposTermLine& csvIn)
{
	CCSV csvMMUsage(',', '"', FALSE, FALSE, TRUE);
	csvMMUsage.Add(CSVPLUDATA_MIXMATCH_USAGE);			//LINE TYPE
	csvMMUsage.Add(m_TransactionInfo.m_strTimeTran);	//TRAN TIME
	csvMMUsage.Add(m_TransactionInfo.m_nCovers);		//COVERS
	csvMMUsage.Add(m_TransactionInfo.m_nServerNo);		//TRAN SERVER
	csvMMUsage.Add(m_TransactionInfo.m_nCustomerNo);	//CUSTOMER NO
	csvMMUsage.Add(m_TransactionInfo.m_nSeqNo);			//TRAN SEQ NO
	csvMMUsage.Add(m_TransactionInfo.m_nTableNo);		//TABLE NO
	csvMMUsage.Add(csvIn.GetMMNo());					//MIX MATCH NO
	csvMMUsage.Add(0);									//PLACEHOLODER FOR MIX MATCH TYPE
	csvMMUsage.Add(csvIn.GetMMUseID());					//MIX MATCH USAGE ID
	csvMMUsage.Add(csvIn.GetMMUseCount());				//MIX MATCH COUNT

	m_TransactionInfo.m_bufferDailyLogMixMatchUsage.Add(csvMMUsage.GetLine());
}

/**********************************************************************/
