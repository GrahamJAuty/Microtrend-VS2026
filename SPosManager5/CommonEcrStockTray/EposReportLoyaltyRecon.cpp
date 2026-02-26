/**********************************************************************/
#include "EposReportConsolParams.h"
#include "FileRemover.h"
#include "HistoryFileTypes.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
#include "UnderlineMaker.h"
#include "WaitLockDlg.h"
/**********************************************************************/
#include "EposReportLoyaltyRecon.h"
/**********************************************************************/

CEposReportLoyaltyRecon::CEposReportLoyaltyRecon(CEposSelectArray& SelectArray, const char* szCustomSettings) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_LOYALTYRECON;
	CString strParams = szCustomSettings;
	m_ReportSettings.SetSettingsCSVLine(strParams);
	m_nTotalFileSize = 0;
	m_strLogError = "";

	switch (SysInfo.GetLoyaltyOrSmartPayType())
	{
	case LOYALTY_TYPE_SMP_V2:
	case LOYALTY_TYPE_SMP_V4:
		m_strLoyaltyorSmartPay = "SmartPay";
		break;

	case LOYALTY_TYPE_LOY_V3:
	case LOYALTY_TYPE_LOY_V4:
	default:
		m_strLoyaltyorSmartPay = "Loyalty";
		break;
	}

#ifdef STOCKMAN_SYSTEM
	m_strEcrmanOrStockman = "Stock Manager";
#else
	m_strEcrmanOrStockman = "Ecr Manager";
#endif

}

/**********************************************************************/

CEposReportLoyaltyRecon::~CEposReportLoyaltyRecon()
{
	for ( int n = 0; n < m_arrayReportBlocks.GetSize(); n++ )
	{
		m_arrayReportBlocks[n].m_pSales -> RemoveAll();
		delete ( m_arrayReportBlocks[n].m_pSales );
	}
}

/**********************************************************************/

bool CEposReportLoyaltyRecon::SpecialPrepare()
{
	bool bResult = FALSE;

	m_SelectArray.MakeList();

	m_nTotalFileSize = 0;
	CEposReportFileListMaker FileListMaker(m_bEODMode, m_bSaleTime, m_PMSModes);
	FileListMaker.GetTermFileList(m_arrayFileInfo, m_SelectArray, TRUE, m_nTotalFileSize);
	
	COleDateTime timeNow = COleDateTime::GetCurrentTime();

	PurgeOldRequestAndResponseFiles(timeNow);
	
	CString strRequestFile = "";
	strRequestFile.Format("%s\\RECON_%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		(const char*) m_ReportSettings.GetLoyaltyRequestPath(),
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	CCSV csvRequest;
	csvRequest.Add(1);
	csvRequest.Add(FileListMaker.GetCheckDateTimeFrom());
	csvRequest.Add(FileListMaker.GetCheckDateTimeTo());

	CSSFile fileRequest;
	if (fileRequest.Open(strRequestFile + ".REQ", "wb") == TRUE)
	{
		fileRequest.WriteLine(csvRequest.GetLine());
		fileRequest.Close();

		CWaitLoyaltyReportDlg dlgWait(strRequestFile + ".RSP", NULL);
		bResult = dlgWait.LockLoyalty();
	}

	if (TRUE == bResult)
	{
		ConsolidateLogData(strRequestFile);

		if (m_strLogError != "")
		{
			CString strError = "";
			strError += m_strEcrmanOrStockman;
			strError += " was unable to retrieve ";
			strError += m_strLoyaltyorSmartPay;
			strError += " Log data for the reconciliation report.\n\n";
			strError += "Error detail => ";
			strError += m_strLogError;
			Prompter.Error(strError);

			bResult = FALSE;
		}
	}

	return bResult;

}

/**********************************************************************/

void CEposReportLoyaltyRecon::ConsolidateLogData(CString strRequestFile)
{
	m_strLogError = "";

	CString strResponseFile = strRequestFile + ".RSP";

	{
		CSSFile fileResponse;
		if (fileResponse.Open(strResponseFile, "rb"))
		{
			bool bFirstLine = TRUE;

			CString strBuffer = "";
			while (fileResponse.ReadString(strBuffer) == TRUE)
			{
				CCSV csvLine(strBuffer);

				if (TRUE == bFirstLine)
				{
					switch (csvLine.GetInt(0))
					{
					case 1:
						m_strLogError = "Unknown request format";
						break;

					case 2:
						m_strLogError = "Unable to open audit file";
						break;

					case 3:
						m_strLogError = "Invalid date range in request";
						break;
					}

					if (m_strLogError != "")
					{
						break;
					}

					bFirstLine = FALSE;
				}
				else if (csvLine.GetInt(0) == 1)
				{
					CEposReportConsolLoyaltyReconLog Log;
					Log.m_nDbNo = 1;
					Log.m_nFolderSet = csvLine.GetInt(1);
					Log.m_nTerminalNo = csvLine.GetInt(2);
					Log.m_nSequenceNo = csvLine.GetInt(3);
					Log.m_strDate = csvLine.GetString(4);
					Log.m_strTime = csvLine.GetString(5);
					m_ConsolLog.Consolidate(Log);
				}
				else if (csvLine.GetInt(0) == 2)
				{
					CEposReportConsolLoyaltyReconLog Log;
					Log.m_nDbNo = csvLine.GetInt(1);
					Log.m_nFolderSet = csvLine.GetInt(2);
					Log.m_nTerminalNo = csvLine.GetInt(3);
					Log.m_nSequenceNo = csvLine.GetInt(4);
					Log.m_strDate = csvLine.GetString(5);
					Log.m_strTime = csvLine.GetString(6);
					m_ConsolLog.Consolidate(Log);
				}
				else
				{
					m_strLogError = "";
					m_strLogError += "Unknown response format from ";
					m_strLogError += m_strLoyaltyorSmartPay;
					m_strLogError += " server";
					break;
				}
			}
		}
	}

	CFileRemover FileRemover;
	FileRemover.RemoveFile(strResponseFile);
}

/**********************************************************************/

void CEposReportLoyaltyRecon::InitialiseConsolidationBlocks()
{
	m_BlockMap.SetConsolidateByTerminalFlag( TRUE );
	m_BlockMap.SetConsolidateByLocationFlag( FALSE );
	m_BlockMap.SetConsolidateByDatabaseFlag( FALSE );
	m_BlockMap.SetConsolidateBySystemFlag( FALSE );
	m_BlockMap.BuildMap( m_SelectArray );
	
	for ( int n = 0; n < m_BlockMap.GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		m_BlockMap.GetBlockMapEntity( n, entity );

		CEposReportLoyaltyReconBlock block;
		block.Copy( entity );
		block.m_pSales = new CReportConsolidationArray<CEposReportConsolLoyaltyRecon>;
		
		m_arrayReportBlocks.Add( block );
	}
}

/**********************************************************************/

void CEposReportLoyaltyRecon::Consolidate()
{
	InitialiseConsolidationBlocks();

	int nProgress = 0;
	StatusProgress.Lock(TRUE, "Reconciling Transactions");

	m_ReportFilters.ResetDbIdx();
	for (int nFileIdx = 0; (nFileIdx < m_arrayFileInfo.GetSize()) && (m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE); nFileIdx++)
	{
		CTermFileListInfo infoFile;
		m_arrayFileInfo.GetAt(nFileIdx, infoFile);

		if (dbLocation.GetLoyaltyFolderSet(infoFile.m_nLocIdx) == 0)
		{
			continue;
		}

		m_BlockMap.GetBlockListForTerminal(infoFile.m_nSelectArrayIdx, m_arrayTerminalBlocks);

		//PMS LOCATION IS NOT RELEVANT FOR LOYALTY
		if ( m_PMSModes.GetPMSLocationStatus(infoFile) != 2 )
		{
			continue;
		}

		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			continue;
		}
		int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
		{
			int nDummy = 0;
			dbLocation.GetLocSPOSVersion(infoFile.m_nLocIdx, nDummy, nConnectionType);
		}

		m_ReportFilters.SetDatabase(infoFile.m_nDbIdx);

		int nLinesToRead;
		CString strBuffer;
		bool bIsVoidTran = FALSE;
		if (::FindFirstTermLine(fileSales, strBuffer, nLinesToRead) == TRUE)
		{
			m_CashRSPVersionChecker.ResetTransactionVersions();

			do
			{
				StatusProgress.SetPos(nProgress + fileSales.Tell(), m_nTotalFileSize);

				CCSVEposTermLine csvIn(strBuffer);

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				switch (nCashRSPLineType)
				{
				case CASHRSP_LINETYPE_TRAN_NORMAL:
				{
					m_CashRSPVersionChecker.ApplyPendingTransactionInfo();

					if (csvIn.GetTransactionLoyaltyIDAsInt64(m_CashRSPVersionChecker.GetCurrentTransactionVersionNo()) != 0)
					{
						CString strDate = csvIn.GetTransactionDate();
						CString strTime = csvIn.GetTransactionTime();

						if (strDate.GetLength() != 8 || strTime.GetLength() != 8)
						{
							continue;
						}

						CString strCheckDate;
						strCheckDate.Format("20%s%s%s",
							(const char*)strDate.Right(2),
							(const char*)strDate.Mid(3, 2),
							(const char*)strDate.Left(2));

						CString strCheckTime;
						strCheckTime.Format("%s%s%s",
							(const char*)strTime.Left(2),
							(const char*)strTime.Mid(3, 2),
							(const char*)strTime.Right(2));

						if (m_BusinessDateInfo.ValidateBusinessDate(infoFile.m_nLocIdx, m_bEODMode, strCheckDate, strCheckTime) == TRUE)
						{
							COleDateTime oleDate;
							::GetOleDateFromString(strCheckDate, oleDate);
							m_SelectArray.ProcessStartDate(infoFile.m_nSelectArrayIdx, oleDate);
							m_SelectArray.ProcessEndDate(infoFile.m_nSelectArrayIdx, oleDate);

							for (int n = 0; n < m_arrayTerminalBlocks.GetSize(); n++)
							{
								int nBlock = m_arrayTerminalBlocks.GetAt(n);

								CEposReportConsolLoyaltyRecon ConsolInfo;
								ConsolInfo.m_strDate = strCheckDate;
								ConsolInfo.m_strTime = strCheckTime;
								ConsolInfo.m_nSequenceNo = csvIn.GetTransactionNumber();
								ConsolInfo.m_nValue = csvIn.GetTransactionValue();

								m_arrayReportBlocks[nBlock].m_pSales->Consolidate(ConsolInfo);
							}
						}
					}
				}
				break;
				}
				
				switch (nCashRSPLineType)
				{
				case CASHRSP_LINETYPE_VERSION:
					m_CashRSPVersionChecker.ProcessCashRSPVersionLine(csvIn);
					break;

				case CASHRSP_LINETYPE_APPLICATION:
					m_CashRSPVersionChecker.ProcessAppTypeLine(csvIn);
					break;

				default:
					m_CashRSPVersionChecker.ResetPendingTransactionInfo();
					break;
				}
			} while ((fileSales.ReadString(strBuffer) == TRUE) && (--nLinesToRead != 0) && (m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE));
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportLoyaltyRecon::CreateReport()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	m_ReportFile.SetStyle1(GetReportTitle(FALSE));
	m_ReportFile.AddColumn("Date", TA_LEFT, 250);
	m_ReportFile.AddColumn("Time", TA_LEFT, 250);
	m_ReportFile.AddColumn("Seq No", TA_LEFT, 250);
	m_ReportFile.AddColumn("Value", TA_RIGHT, 250);
	m_ReportFile.AddColumn("", TA_LEFT, 100);
	m_ReportFile.AddColumn("Loyalty Time", TA_LEFT, 350);
	
	m_nCreateReportCount = 0;
	m_nCreateReportTarget = 0;
	m_nCreateReportMiniCount = 0;
	m_nCreateReportMiniTarget = 0;
	for (int n = 0; n < m_arrayReportBlocks.GetSize(); n++)
	{
		CEposReportLoyaltyReconBlock ReportBlock = m_arrayReportBlocks.GetAt(n);
		m_nCreateReportTarget += ReportBlock.m_pSales->GetSize();
	}

	m_nCreateReportMiniTarget = m_nCreateReportTarget / 100;
	if (m_nCreateReportMiniTarget < 1)
	{
		m_nCreateReportMiniTarget = 1;
	}

	StatusProgress.Lock(TRUE, "Creating report");

	m_BlockMap.BuildSortArray();
	for (int n = 0; n < m_BlockMap.GetSortArraySize(); n++)
	{
		int nIdx = m_BlockMap.GetSortArrayIdx(n);
		CEposReportLoyaltyReconBlock ReportBlock = m_arrayReportBlocks.GetAt(nIdx);

		if (ReportBlock.GotData() == FALSE)
		{
			continue;
		}

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly(ReportBlock.m_nDbIdx, info, FALSE);

		m_ReportFile.HandleNewPage();

		CString strHeader;
		ReportBlock.GetHeaderText(strHeader);
		m_ReportFile.WriteReportMiscLine(strHeader);
		m_ReportFile.WriteReportMiscLine("<LI>");

		CreateSalesSection(ReportBlock);
		WriteBlockTotals(ReportBlock);
	}

	StatusProgress.Unlock();
	ShowEposDetails();

	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportLoyaltyRecon::CreateSalesSection( CEposReportLoyaltyReconBlock& Block )
{
	int nFolderSet = dbLocation.GetLoyaltyFolderSet(Block.m_nLocIdx);
	int nTerminalNo = dbLocation.GetTNo(Block.m_nLocIdx, Block.m_nTermIdx);

	for (int n = 0; n < Block.m_pSales->GetSize(); n++)
	{
		CEposReportConsolLoyaltyRecon ConsolInfo;
		Block.m_pSales->GetAt(n, ConsolInfo);

		CString strTranDate = "";
		strTranDate.Format("%s/%s/%s",
			(const char*)ConsolInfo.m_strDate.Right(2),
			(const char*)ConsolInfo.m_strDate.Mid(4, 2),
			(const char*)ConsolInfo.m_strDate.Left(4));

		CString strTranTime = "";
		strTranTime.Format("%s:%s:%s",
			(const char*)ConsolInfo.m_strTime.Left(2),
			(const char*)ConsolInfo.m_strTime.Mid(2, 2),
			(const char*)ConsolInfo.m_strTime.Right(2));

		CString strLogTime = "";
		CString strColour = "";
		bool bGotRecon = FALSE;

		{
			CEposReportConsolLoyaltyReconLog Log;
			Log.m_nDbNo = dbLocation.GetDbNo(Block.m_nLocIdx);
			Log.m_nFolderSet = nFolderSet;
			Log.m_nTerminalNo = nTerminalNo;
			Log.m_nSequenceNo = ConsolInfo.m_nSequenceNo;

			int nPos = 0;
			if (m_ConsolLog.Find(Log, nPos) == TRUE)
			{
				bGotRecon = TRUE;

				m_ConsolLog.GetAt(nPos, Log);

				strLogTime = Log.m_strTime;

				if (Log.m_strDate != strTranDate)
				{
					strLogTime += " (";
					strLogTime += Log.m_strDate;
					strLogTime += ")";
				}
			}
			else if (m_ReportSettings.GetUnreconOnlyFlag() == FALSE)
			{
				strColour = "<CF#FF0000>";
			}
		}

		CString strValue = "";
		strValue.Format("%.2f",
			((double)ConsolInfo.m_nValue) / 100.0);

		CString strSEQNo = "";
		strSEQNo.Format("%d", ConsolInfo.m_nSequenceNo);

		if ((FALSE == bGotRecon) || (m_ReportSettings.GetUnreconOnlyFlag() == FALSE))
		{
			CCSV csv('\t');
			csv.Add(strColour + strTranDate);
			csv.Add(strColour + strTranTime);
			csv.Add(strColour + strSEQNo);
			csv.Add(strColour + strValue);
			csv.Add("");
			csv.Add(strColour + strLogTime);

			m_ReportFile.WriteReportDataLine(csv.GetLine());
		}
	}
}

/**********************************************************************/

void CEposReportLoyaltyRecon::WriteBlockTotals(CEposReportLoyaltyReconBlock& Block)
{
}

/**********************************************************************/

void CEposReportLoyaltyRecon::PurgeOldRequestAndResponseFiles(COleDateTime timeNow)
{
	CString strTimeStampREQ = "";
	CString strTimeStampRSP = "";

	{
		COleDateTime timeREQCutOff = timeNow - COleDateTimeSpan(0, 0, 2, 0);

		strTimeStampREQ.Format("%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
			timeREQCutOff.GetYear(),
			timeREQCutOff.GetMonth(),
			timeREQCutOff.GetDay(),
			timeREQCutOff.GetHour(),
			timeREQCutOff.GetMinute(),
			timeREQCutOff.GetSecond());

		COleDateTime timeRSPCutOff = timeNow - COleDateTimeSpan(0, 0, 10, 0);

		strTimeStampRSP.Format("%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
			timeRSPCutOff.GetYear(),
			timeRSPCutOff.GetMonth(),
			timeRSPCutOff.GetDay(),
			timeRSPCutOff.GetHour(),
			timeRSPCutOff.GetMinute(),
			timeRSPCutOff.GetSecond());
	}

	CFileFind finder;

	CString strMask = "";
	strMask += m_ReportSettings.GetLoyaltyRequestPath();
	strMask += "\\RECON_*.*";

	BOOL bWorking = finder.FindFile(strMask);

	CStringArray arrayFilesToDelete;

	while (bWorking == TRUE)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots() == TRUE)
		{
			continue;
		}

		if (finder.IsDirectory() == TRUE)
		{
			continue;
		}

		CString strFilename = finder.GetFileName();

		if ( strFilename.GetLength() != 25 )
		{
			continue;
		}

		CString strExt = strFilename.Right(4);
		strExt.MakeUpper();

		bool bIsREQFile = FALSE;
		bool bIsRSPFile = FALSE;

		if ( strExt == ".REQ")
		{
			bIsREQFile = TRUE;
		}
		else if (strExt == ".RSP")
		{
			bIsRSPFile = TRUE;
		}

		if ((TRUE == bIsREQFile) || (TRUE == bIsRSPFile))
		{
			CString strTest = strFilename.Mid(6, 15);
			
			bool bCanDelete = FALSE;
			
			if (TRUE == bIsREQFile)
			{
				bCanDelete = (strTest <= strTimeStampREQ);
			}
			else
			{
				bCanDelete = (strTest <= strTimeStampRSP);
			}

			if ( TRUE == bCanDelete )
			{
				arrayFilesToDelete.Add(strFilename);
			}
		}
	}

	CString strFolder = "";
	strFolder += m_ReportSettings.GetLoyaltyRequestPath();
	strFolder += "\\";
	
	for (int n = 0; n < arrayFilesToDelete.GetSize(); n++)
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile(strFolder + arrayFilesToDelete.GetAt(n));
	}
}

/**********************************************************************/
