/**********************************************************************/
#include "CashRSPImporter.h"
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "PMSOptions.h"
#include "PriceHelpers.h"
#include "ReportHelpers.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportTransactionSorted.h"
/**********************************************************************/

CEposReportTransactionBufferItem::CEposReportTransactionBufferItem()
{
	m_nLineType = TRANBUFFER_LINETYPE_NORMAL;
	m_nBlankLineCount = 0;
	m_nServerNo = 0;
	m_strText = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportTransactionSortedTranInfo::CEposReportTransactionSortedTranInfo()
{
	Reset();
}

/**********************************************************************/

void CEposReportTransactionSortedTranInfo::Reset()
{
	m_bGotFirstSaleTime = FALSE;
	m_oleFirstSaleTime = COleDateTime::GetCurrentTime();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportTransactionBuffer::CEposReportTransactionBuffer()
{
	Reset();
}

/**********************************************************************/

void CEposReportTransactionBuffer::Reset()
{
	m_array.RemoveAll();
	m_dUnderpaid = 0.0;
	m_dOverpaid = 0.0;
}

/**********************************************************************/

void CEposReportTransactionBuffer::GetAt(int n, CEposReportTransactionBufferItem& item)
{
	if ((n >= 0) && (n < GetSize()))
	{
		item = m_array.GetAt(n);
	}
	else
	{
		item.m_nLineType = TRANBUFFER_LINETYPE_NORMAL;
		item.m_nBlankLineCount = 0;
		item.m_nServerNo = 0;
		item.m_strText = "";
	}
}

/**********************************************************************/

void CEposReportTransactionBuffer::SetAt(int n, CEposReportTransactionBufferItem& item)
{
	if ((n >= 0) && (n < GetSize()))
	{
		m_array.SetAt(n, item);
	}
}

/**********************************************************************/

void CEposReportTransactionBuffer::AddBlanks(int n)
{
	CEposReportTransactionBufferItem item;
	item.m_nLineType = TRANBUFFER_LINETYPE_BLANKS;
	item.m_nBlankLineCount = n;
	item.m_nServerNo = 0;
	item.m_strText = "";
	m_array.Add(item);
}

/**********************************************************************/

void CEposReportTransactionBuffer::AddReportLine(const char* sz, int nServerNo)
{
	CEposReportTransactionBufferItem item;
	item.m_nLineType = TRANBUFFER_LINETYPE_NORMAL;
	item.m_nBlankLineCount = 0;
	item.m_nServerNo = nServerNo;
	item.m_strText = sz;
	m_array.Add(item);
}

/**********************************************************************/

void CEposReportTransactionBuffer::AddNoSearchLine(const char* sz)
{
	CEposReportTransactionBufferItem item;
	item.m_nLineType = TRANBUFFER_LINETYPE_NOSEARCH;
	item.m_nBlankLineCount = 0;
	item.m_nServerNo = 0;
	item.m_strText = sz;
	m_array.Add(item);
}

/**********************************************************************/

void CEposReportTransactionBuffer::AddHiddenLine(const char* sz)
{
	CEposReportTransactionBufferItem item;
	item.m_nLineType = TRANBUFFER_LINETYPE_HIDDEN;
	item.m_nBlankLineCount = 0;
	item.m_nServerNo = 0;
	item.m_strText = sz;
	m_array.Add(item);
}

/**********************************************************************/

CEposReportTransactionSorted::CEposReportTransactionSorted(CEposSelectArray& SelectArray, int nReportType) : CEposReport(SelectArray)
{
	m_bPMSTransaction = FALSE;
	m_bWastageTransaction = FALSE;
	m_dTransactionTotalNonCashbackPayments = 0.0;
	m_dTransactionTotalNonCashbackSales = 0.0;

	switch (nReportType)
	{
	case REPORT_TYPE_TRANSACTION_REBUILD:
	case REPORT_TYPE_TRANSACTION_DISCREPANCY:
	case REPORT_TYPE_TRANSACTION_NORMAL:
		m_nReportType = nReportType;
		break;

	default:
		m_nReportType = REPORT_TYPE_TRANSACTION_NORMAL;
		break;
	}

	switch (m_nReportType)
	{
	case REPORT_TYPE_TRANSACTION_REBUILD:
		SetPMSAllowMode(PMS_ALLOWMODE_NONE);
		m_ReportSettings.SetDiscrepancyFlag(FALSE);
		break;

	case REPORT_TYPE_TRANSACTION_DISCREPANCY:
		SetPMSAllowMode(PMS_ALLOWMODE_ACCOUNT);
		m_ReportSettings.SetDiscrepancyFlag(TRUE);
		break;

	case REPORT_TYPE_TRANSACTION_NORMAL:
		SetPMSAllowMode(PMS_ALLOWMODE_ALL);
		m_ReportSettings.SetDiscrepancyFlag(FALSE);
		break;
	}

	m_ReportSettings.SetSortType(0);

	m_bNoReadDatabase = FALSE;
	m_dTotalOverPaid = 0.0;
	m_dTotalUnderPaid = 0.0;
	SetPMSDepositsFlag(TRUE);
}

/**********************************************************************/

CEposReportTransactionSorted::CEposReportTransactionSorted(CEposSelectArray& SelectArray, int nReportType, const char* szCustomSettings) : CEposReport(SelectArray)
{
	m_nReportType = nReportType;
	m_bPMSTransaction = FALSE;
	m_bWastageTransaction = FALSE;
	m_dTransactionTotalNonCashbackPayments = 0.0;
	m_dTransactionTotalNonCashbackSales = 0.0;

	CString strParams = szCustomSettings;
	m_ReportSettings.SetSettingsCSVLine(strParams);

	if (m_ReportSettings.GetDiscrepancyFlag() == TRUE)
	{
		SetPMSAllowMode(PMS_ALLOWMODE_ACCOUNT);
	}
	else
	{
		SetPMSAllowMode(PMS_ALLOWMODE_ALL);
	}

	int nSortType = m_ReportSettings.GetSortType();
	if ((nSortType < 0) || (nSortType > 2))
	{
		m_ReportSettings.SetSortType(0);
	}

	m_bNoReadDatabase = FALSE;
	m_dTotalOverPaid = 0.0;
	m_dTotalUnderPaid = 0.0;
	SetPMSDepositsFlag(TRUE);
}

/**********************************************************************/

void CEposReportTransactionSorted::Consolidate()
{
}

/**********************************************************************/

bool CEposReportTransactionSorted::CreateReport()
{
	bool bResult = CreateReportInternal();
	
	CDataManagerInfo infoDM;
	DataManagerNonDb.CloseTableNames(infoDM);

	return bResult;
}

/**********************************************************************/

bool CEposReportTransactionSorted::CreateReportInternal()
{
	if (m_ReportFile.Open(Super.ReportFilename()) == FALSE)
	{
		return FALSE;
	}

	CString strTitle = GetReportTitle(FALSE);

	m_ReportFile.SetStyle1(strTitle);
	m_ReportFile.AddColumn("Date", TA_LEFT, 250);
	m_ReportFile.AddColumn("Time", TA_LEFT, 200);
	m_ReportFile.AddColumn("Seq No", TA_LEFT, 150);

	switch (m_ReportSettings.GetTimeType())
	{
	case 1:
		m_ReportFile.AddColumn("First Sale", TA_LEFT, 250);
		break;

	case 2:
		m_ReportFile.AddColumn("Sale Time", TA_LEFT, 250);
		break;
	}

	m_ReportFile.AddColumn("Type", TA_LEFT, 330);
	m_ReportFile.AddColumn("Info", TA_LEFT, 1200);

	int nTotalFileSize = 0;
	m_arrayFileInfo.RemoveAll();

	if (REPORT_TYPE_TRANSACTION_REBUILD == m_nReportType)
	{
		CEposReportFileListMaker FileListMaker(m_bEODMode, m_bSaleTime, m_PMSModes);
		FileListMaker.GetTermFileListRebuild(m_arrayFileInfo, m_SelectArray, nTotalFileSize);
	}
	else
	{
		CEposReportFileListMaker FileListMaker(m_bEODMode, m_bSaleTime, m_PMSModes);
		FileListMaker.GetTermFileList(m_arrayFileInfo, m_SelectArray, FALSE, nTotalFileSize);
	}

	int nProgress = 0;
	StatusProgress.Lock(TRUE, "Creating report");

	bool bInclude = FALSE;
	double dTransactionTotal = 0.0;

	int nCurrentDbIdx = -1;
	int nCurrentLocIdx = -1;
	int nCurrentTerminalIdx = -1;
	int nCurrentCashRSPMode = -1;

	for (int nFileIdx = 0; (nFileIdx < m_arrayFileInfo.GetSize()) && (m_CashRSPVersionChecker.GetCashRSPFormatFailFlag() == FALSE); nFileIdx++)
	{
		CTermFileListInfo infoFile;
		m_arrayFileInfo.GetAt(nFileIdx, infoFile);

		int nPMSStatus = m_PMSModes.GetPMSLocationStatus(infoFile);

		if (0 == nPMSStatus)
		{
			continue;
		}

		bool bIsPMSLocation = (1 == nPMSStatus);

		int nCashRSPMode = infoFile.m_nCashRSPMode;

		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			continue;
		}

		CEposSelectArrayListItem ListItem;
		m_SelectArray.GetListItem(infoFile.m_nSelectArrayIdx, ListItem);

		if (m_bNoReadDatabase == FALSE)
		{
			CDataManagerInfo info;
			DataManager.OpenDatabaseReadOnly(ListItem.m_nDbIdx, info, FALSE);
		}

		if ((TRUE == bIsPMSLocation) || (nCurrentLocIdx != ListItem.m_nLocIdx) || (nCurrentTerminalIdx != ListItem.m_nTermIdx) || ((REPORT_TYPE_TRANSACTION_REBUILD == m_nReportType) && (nCashRSPMode != nCurrentCashRSPMode)))
		{
			WriteTransactions(nCurrentDbIdx, nCurrentLocIdx);

			{
				CDataManagerInfo infoDM;
				DataManagerNonDb.CloseTableNames(infoDM);
				DataManagerNonDb.OpenTableNames(DB_READONLY, ListItem.m_nLocIdx, infoDM);
			}

			m_arrayIndex.RemoveAll();
			m_arrayLines.RemoveAll();

			CString strType = "";
			if (REPORT_TYPE_TRANSACTION_REBUILD == m_nReportType)
			{
				switch (nCashRSPMode)
				{
				case CASHRSP_OBSOLETE_TERMSAVE:
					strType = " (Legacy data for Trading & Training Servers)";
					break;

				case CASHRSP_SALES_TRADING:
					strType = " (Trading Servers)";
					break;
				}
			}

			CString strHeader = "";
			if ((strType != "") || (DealerFlags.GetCostleyTransactionLogFlag() == FALSE))
			{
				strHeader.Format("<..>%s, %s%s",
					dbLocation.GetName(ListItem.m_nLocIdx),
					dbLocation.GetTerminalName(ListItem.m_nLocIdx, ListItem.m_nTermIdx),
					(const char*)strType);
			}
			else
			{
				strHeader.Format("%s\t%s",
					dbLocation.GetName(ListItem.m_nLocIdx),
					dbLocation.GetTerminalName(ListItem.m_nLocIdx, ListItem.m_nTermIdx));
			}

			m_ReportFile.RequestBlankLines(2);
			m_ReportFile.WriteReportMiscLine(strHeader);
			m_ReportFile.WriteReportMiscLine("<LI>");

			nCurrentDbIdx = ListItem.m_nDbIdx;
			nCurrentLocIdx = ListItem.m_nLocIdx;
			nCurrentTerminalIdx = ListItem.m_nTermIdx;
			nCurrentCashRSPMode = nCashRSPMode;
		}

		if (m_ReportSettings.GetFilterTextType() != 2)
		{
			m_ReportFile.RequestBlankLines(1);
		}

		bool bGotFileTransaction = FALSE;
		bool bGotTransactionHeader = FALSE;
		bool bTrailingNoSaleOrAgeCheck = FALSE;
		m_bWastageTransaction = FALSE;

		m_arrayBufferPMSHeader.RemoveAll();
		m_arrayBufferGeneral.RemoveAll();
		m_arrayBufferPayments.RemoveAll();
		m_arrayBufferDiscounts.RemoveAll();
		m_arrayBufferFooter.RemoveAll();
		m_strTransactionHeader = "";
		m_bPMSTransaction = FALSE;
		m_dTransactionTotalNonCashbackSales = 0.0;
		m_dTransactionTotalNonCashbackPayments = 0.0;

		CString strCheckDate = "";
		CString strCheckTime = "";

		m_PMSModes.GetPMSTransactionDate(bIsPMSLocation, infoFile, strCheckDate, strCheckTime);

		int nLinesToRead = 0;
		CString strBuffer = "";
		if (::FindFirstTermLine(fileSales, strBuffer, nLinesToRead) == TRUE)
		{
			bInclude = FALSE;

			m_CashRSPVersionChecker.ResetTransactionVersions();
			bool bDepositSkip = FALSE;
			bool bDepositEnd = FALSE;

			do
			{
				StatusProgress.SetPos(nProgress + fileSales.Tell(), nTotalFileSize);

				CCSVEposTermLine csvIn(strBuffer);

				int nCashRSPLineType = csvIn.GetLineTypeInt();

				switch (nCashRSPLineType)
				{
				case CASHRSP_LINETYPE_DEPOSITSTART:
					bDepositSkip = TRUE;
					bDepositEnd = FALSE;
					break;

				case CASHRSP_LINETYPE_DEPOSITEND:
					bDepositSkip = FALSE;
					bDepositEnd = TRUE;
					break;

				case CASHRSP_LINETYPE_EODHEADER:
				case CASHRSP_LINETYPE_TRAN_NORMAL:
				case CASHRSP_LINETYPE_TRAN_VOID:
				case CASHRSP_LINETYPE_WASTAGE:
				case CASHRSP_LINETYPE_NOSALE:
				case CASHRSP_LINETYPE_AGEVERIFY:
					bDepositSkip = FALSE;
					bDepositEnd = FALSE;
					break;

				default:
					bDepositEnd = FALSE;
					break;
				}

				if ((TRUE == bDepositSkip) || (TRUE == bDepositEnd))
				{
					continue;
				}

				if (CASHRSP_LINETYPE_EODHEADER == nCashRSPLineType)
				{
					bInclude = FALSE;
					continue;
				}

				if (m_ReportSettings.GetDiscrepancyFlag() == TRUE)
				{
					switch (nCashRSPLineType)
					{
					case CASHRSP_LINETYPE_TRAN_VOID:
					case CASHRSP_LINETYPE_WASTAGE:
					case CASHRSP_LINETYPE_NOSALE:
					case CASHRSP_LINETYPE_AGEVERIFY:
						bInclude = FALSE;
						continue;
					}
				}

				if (REPORT_TYPE_TRANSACTION_REBUILD == m_nReportType)
				{
					switch (nCashRSPLineType)
					{
					case CASHRSP_LINETYPE_NOSALE:
					case CASHRSP_LINETYPE_AGEVERIFY:
						bInclude = FALSE;
						continue;
					}
				}

				if (m_PMSModes.CheckPMSTransactionType(bIsPMSLocation, nCashRSPLineType) == FALSE)
				{
					continue;
				}

				switch (nCashRSPLineType)
				{
				case CASHRSP_LINETYPE_HEADER_PMS:
					BufferPMSHeader(csvIn);
					break;

				case CASHRSP_LINETYPE_TRAN_NORMAL:
				case CASHRSP_LINETYPE_TRAN_VOID:
				case CASHRSP_LINETYPE_WASTAGE:
				{
					bInclude = FALSE;

					bool bAcceptDateTime = FALSE;

					if ((TRUE == bIsPMSLocation) && (m_PMSModes.GetDateMode() == PMS_DATEMODE_ACCOUNT))
					{
						bAcceptDateTime = TRUE;
					}
					else
					{
						CString strTime = csvIn.GetTransactionTime();
						if (strTime.GetLength() != 8)
						{
							continue;
						}

						CString strDate = csvIn.GetTransactionDate();
						if (strDate.GetLength() != 8)
						{
							continue;
						}

						strCheckDate.Format("20%s%s%s",
							(const char*)strDate.Right(2),
							(const char*)strDate.Mid(3, 2),
							(const char*)strDate.Left(2));

						strCheckTime.Format("%s%s%s",
							(const char*)strTime.Left(2),
							(const char*)strTime.Mid(3, 2),
							(const char*)strTime.Right(2));

						if (REPORT_TYPE_TRANSACTION_REBUILD == m_nReportType)
						{
							bAcceptDateTime = DataManagerNonDb.SessionDateTimeFilter.CheckDate(strCheckDate);
						}
						else
						{
							bAcceptDateTime = SimpleTimeCheck(infoFile.m_nLocIdx, strCheckDate, strCheckTime);
						}
					}

					if (TRUE == bAcceptDateTime)
					{
						bInclude = TRUE;

						COleDateTime oleDate = COleDateTime(
							atoi(strCheckDate.Left(4)),
							atoi(strCheckDate.Mid(4, 2)),
							atoi(strCheckDate.Right(2)),
							0, 0, 0);

						m_SelectArray.ProcessStartDate(infoFile.m_nSelectArrayIdx, oleDate);
						m_SelectArray.ProcessEndDate(infoFile.m_nSelectArrayIdx, oleDate);

						m_TransactionHeaderIndexNext.m_strDisplayDate.Format("%s/%s/%s",
							(const char*)strCheckDate.Right(2),
							(const char*)strCheckDate.Mid(4, 2),
							(const char*)strCheckDate.Left(4));

						m_TransactionHeaderIndexNext.m_strDisplayTime.Format("%s:%s:%s",
							(const char*)strCheckTime.Left(2),
							(const char*)strCheckTime.Mid(2, 2),
							(const char*)strCheckTime.Right(2));

						m_TransactionHeaderIndexNext.m_nSequenceNo = csvIn.GetTransactionNumber();
						m_TransactionHeaderIndexNext.m_nServerNo = csvIn.GetTransactionServer();
						m_TransactionHeaderIndexNext.m_strDateTime = strCheckDate + strCheckTime;
						m_TransactionHeaderIndexNext.m_nFileIdx = nFileIdx;
						m_TransactionHeaderIndexNext.m_nCashRSPVersionNo = m_CashRSPVersionChecker.GetCurrentTransactionVersionNo();
					}
				}
				break;

				case CASHRSP_LINETYPE_NOSALE:
				{
					bInclude = FALSE;

					if (TRUE == bIsPMSLocation)
					{
						continue;
					}
					CString strDate = csvIn.GetNoSaleDate();
					CString strTime = csvIn.GetNoSaleTime();

					//FILTER FOR DATE AND TIME
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

					if (SimpleTimeCheck(infoFile.m_nLocIdx, strCheckDate, strCheckTime) == FALSE)
					{
						continue;
					}

					bInclude = TRUE;

					COleDateTime oleDate = COleDateTime(
						atoi(strCheckDate.Left(4)),
						atoi(strCheckDate.Mid(4, 2)),
						atoi(strCheckDate.Right(2)),
						0, 0, 0);

					m_SelectArray.ProcessStartDate(infoFile.m_nSelectArrayIdx, oleDate);
					m_SelectArray.ProcessEndDate(infoFile.m_nSelectArrayIdx, oleDate);

					m_TransactionHeaderIndexNext.m_strDisplayDate.Format("%s/%s/%s",
						(const char*)strCheckDate.Right(2),
						(const char*)strCheckDate.Mid(4, 2),
						(const char*)strCheckDate.Left(4));

					m_TransactionHeaderIndexNext.m_strDisplayTime = strTime;
					m_TransactionHeaderIndexNext.m_nSequenceNo = csvIn.GetNoSaleNumber();
					m_TransactionHeaderIndexNext.m_nServerNo = csvIn.GetNoSaleServer();
					m_TransactionHeaderIndexNext.m_strDateTime = strCheckDate + strCheckTime;
					m_TransactionHeaderIndexNext.m_nFileIdx = nFileIdx;
					m_TransactionHeaderIndexNext.m_nCashRSPVersionNo = 0;
				}
				break;

				case CASHRSP_LINETYPE_AGEVERIFY:
				{
					bInclude = FALSE;

					if (TRUE == bIsPMSLocation)
					{
						continue;
					}

					CString strDate = csvIn.GetAgeVerifyDate();
					CString strTime = csvIn.GetAgeVerifyTime();

					//FILTER FOR DATE AND TIME
					if ((strDate.GetLength() != 8) || (strTime.GetLength() != 8))
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

					if (SimpleTimeCheck(infoFile.m_nLocIdx, strCheckDate, strCheckTime) == FALSE)
					{
						continue;
					}

					bInclude = TRUE;

					COleDateTime oleDate = COleDateTime(
						atoi(strCheckDate.Left(4)),
						atoi(strCheckDate.Mid(4, 2)),
						atoi(strCheckDate.Right(2)),
						0, 0, 0);

					m_SelectArray.ProcessStartDate(infoFile.m_nSelectArrayIdx, oleDate);
					m_SelectArray.ProcessEndDate(infoFile.m_nSelectArrayIdx, oleDate);

					m_TransactionHeaderIndexNext.m_strDisplayDate.Format("%s/%s/%s",
						(const char*)strCheckDate.Right(2),
						(const char*)strCheckDate.Mid(4, 2),
						(const char*)strCheckDate.Left(4));

					m_TransactionHeaderIndexNext.m_strDisplayTime = strTime;
					m_TransactionHeaderIndexNext.m_nSequenceNo = csvIn.GetAgeVerifyNumber();
					m_TransactionHeaderIndexNext.m_nServerNo = csvIn.GetAgeVerifyServer();
					m_TransactionHeaderIndexNext.m_strDateTime = strCheckDate + strCheckTime;
					m_TransactionHeaderIndexNext.m_nFileIdx = nFileIdx;
					m_TransactionHeaderIndexNext.m_nCashRSPVersionNo = 0;
				}
				break;
				}

				if (bInclude == TRUE)
				{
					CCSV csvOut('\t');

					switch (nCashRSPLineType)
					{
					case CASHRSP_LINETYPE_TRAN_NORMAL:
					case CASHRSP_LINETYPE_TRAN_VOID:
					case CASHRSP_LINETYPE_WASTAGE:
					case CASHRSP_LINETYPE_NOSALE:
					case CASHRSP_LINETYPE_AGEVERIFY:
					{
						m_bWastageTransaction = (CASHRSP_LINETYPE_WASTAGE == nCashRSPLineType);

						if (bGotTransactionHeader == TRUE)
						{
							ConsolidateTransaction(dTransactionTotal);

							if (m_ReportSettings.GetSortType() == 0)
							{
								WriteTransactions(nCurrentDbIdx, nCurrentLocIdx);
								m_arrayIndex.RemoveAll();
								m_arrayLines.RemoveAll();
							}
						}

						m_TransactionHeaderIndex = m_TransactionHeaderIndexNext;
						m_CashRSPVersionChecker.ApplyPendingTransactionInfo();
						m_TransactionHeaderIndex.m_nCashRSPVersionNo = m_CashRSPVersionChecker.GetCurrentTransactionVersionNo();

						if ((TRUE == bTrailingNoSaleOrAgeCheck) && (m_ReportSettings.GetFilterTextType() != 2))
						{
							m_ReportFile.RequestBlankLines(1);
						}

						switch (nCashRSPLineType)
						{
						case CASHRSP_LINETYPE_NOSALE:
						case CASHRSP_LINETYPE_AGEVERIFY:
							bGotTransactionHeader = FALSE;
							dTransactionTotal = 0.0;
							bTrailingNoSaleOrAgeCheck = TRUE;
							break;

						default:
							bGotTransactionHeader = TRUE;
							dTransactionTotal = ((double)csvIn.GetTransactionValue()) / 100.0;
							bTrailingNoSaleOrAgeCheck = FALSE;
							break;
						}
					}
					break;

					default:
						csvOut.Add("");
						csvOut.Add("");
						csvOut.Add("");
						break;
					}

					switch (nCashRSPLineType)
					{
					case CASHRSP_LINETYPE_TRAN_NORMAL:
					case CASHRSP_LINETYPE_TRAN_VOID:
					case CASHRSP_LINETYPE_WASTAGE:
						GetTransactionHeaders(csvIn, nCashRSPLineType);
						break;

					case CASHRSP_LINETYPE_NOSALE:
						BufferNoSale(csvIn);
						break;

					case CASHRSP_LINETYPE_AGEVERIFY:
						BufferAgeVerify(csvIn);
						break;

					case CASHRSP_LINETYPE_PAIDIO:
						BufferPaidIO(csvIn);
						break;

					case CASHRSP_LINETYPE_PAYMENT:
					case CASHRSP_LINETYPE_DEPOSIT_PMS:
					{
						BufferPayment(csvIn);
						int nPaymentNo = csvIn.GetPaymentNumber();
						m_bPMSTransaction = DataManager.Payment.IsPMSPaymentType(nPaymentNo);
					}
					break;

					case CASHRSP_LINETYPE_ITEM:
						BufferSale(csvIn, m_bWastageTransaction);
						break;

					case CASHRSP_LINETYPE_SSFT_DELIVERY:
						BufferDelivery(csvIn);
						break;

					case CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL:
						BufferSubtotalDiscount(csvIn, FALSE);
						break;

					case CASHRSP_LINETYPE_DISCOUNT_LOYALTY:
						BufferSubtotalDiscount(csvIn, TRUE);
						break;

					case CASHRSP_LINETYPE_DISCOUNT_ITEM:
						BufferItemDiscount(csvIn);
						break;

					case CASHRSP_LINETYPE_DISCOUNT_VOID:
						BufferVoidDiscount(csvIn);
						break;

					case CASHRSP_LINETYPE_DISCOUNTTAX_SUBTOTAL:
						//BufferSubtotalDiscountTax( csvIn, FALSE );
						break;

					case CASHRSP_LINETYPE_DISCOUNTTAX_LOYALTY:
						//BufferSubtotalDiscountTax( csvIn, TRUE );
						break;

					case CASHRSP_LINETYPE_COMMENT_PMS:
						if (PMS_DATEMODE_ACCOUNT == m_PMSModes.GetDateMode())
						{
							BufferFooter(csvIn);
						}
						break;

					case CASHRSP_LINETYPE_COMMENT_GENERAL:
						BufferFooter(csvIn);
						break;

					case CASHRSP_LINETYPE_HEADER_PMS:
					case CASHRSP_LINETYPE_VERSION:
					case CASHRSP_LINETYPE_APPLICATION:
					case CASHRSP_LINETYPE_DISCOUNT_PROMO:
					case CASHRSP_LINETYPE_DISCOUNT_MIXMATCH:
					case CASHRSP_LINETYPE_MIXMATCH_USE:
						break;

					default:
						BufferUnknown(csvIn);
						break;
					}
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

			if (bGotTransactionHeader == TRUE)
			{
				ConsolidateTransaction(dTransactionTotal);

				if (m_ReportSettings.GetSortType() == 0)
				{
					WriteTransactions(nCurrentDbIdx, nCurrentLocIdx);
					m_arrayIndex.RemoveAll();
					m_arrayLines.RemoveAll();
				}
			}
		}

		nProgress += fileSales.GetLength();
		fileSales.Close();
	}
	WriteTransactions(nCurrentDbIdx, nCurrentLocIdx);
	StatusProgress.Unlock();

	if (m_ReportSettings.GetDiscrepancyFlag() == TRUE)
	{
		CCSV csvTotal('\t');
		csvTotal.Add("");
		csvTotal.Add("<..>Total Discrepancy (Over Paid)");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add(m_dTotalOverPaid, 2);
		m_ReportFile.WriteReportMiscLine(csvTotal.GetLine());

		csvTotal.RemoveAll();
		csvTotal.Add("");
		csvTotal.Add("<..>Total Discrepancy (Under Paid)");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add(m_dTotalUnderPaid, 2);
		m_ReportFile.WriteReportMiscLine(csvTotal.GetLine());

		m_ReportFile.RequestBlankLines(1);

		csvTotal.RemoveAll();
		csvTotal.Add("");
		csvTotal.Add("<..>Total Discrepancy (Balance)");
		csvTotal.Add("");
		csvTotal.Add("");
		csvTotal.Add(m_dTotalOverPaid - m_dTotalUnderPaid, 2);
		m_ReportFile.WriteReportMiscLine(csvTotal.GetLine());
	}

	ShowEposDetails();
	m_ReportFile.Close();

	return TRUE;
}

/**********************************************************************/

void CEposReportTransactionSorted::ConsolidateTransaction(double dTransactionTotal)
{
	bool bWriteTransaction = TRUE;

	if ((TRUE == m_bPMSTransaction) && (m_PMSModes.GetEposFlag() == FALSE))
	{
		bWriteTransaction = FALSE;
	}
	else if (m_ReportSettings.GetDiscrepancyFlag() == TRUE)
	{
		bWriteTransaction = (CPriceHelpers::CompareDoubles(m_dTransactionTotalNonCashbackPayments, m_dTransactionTotalNonCashbackSales, 3) != 0);
	}

	if (TRUE == bWriteTransaction)
	{
		int nSeqNo = m_TransactionHeaderIndex.m_nSequenceNo % 1000000;
		if ((nSeqNo < m_ReportSettings.GetSeqNoFrom()) || (nSeqNo > m_ReportSettings.GetSeqNoTo()))
		{
			bWriteTransaction = FALSE;
		}
	}

	if (TRUE == bWriteTransaction)
	{
		int nServerNo = m_TransactionHeaderIndex.m_nServerNo;
		if ((nServerNo < 0) || (nServerNo > 9999))
		{
			nServerNo = 0;
		}

		if ((nServerNo < m_ReportSettings.GetTranServerFrom()) || (nServerNo > m_ReportSettings.GetTranServerTo()))
		{
			bWriteTransaction = FALSE;
		}
	}

	if (TRUE == bWriteTransaction)
	{
		m_TransactionHeaderIndex.m_nInternalNo = m_arrayIndex.GetSize();
		m_TransactionHeaderIndex.m_nTranStartIdx = m_arrayLines.GetSize();
		int nIndexPos = m_arrayIndex.ConsolidateWithHint(m_TransactionHeaderIndex, m_ReportSettings.GetSortType());

		{
			int nLastLine = m_arrayBufferPMSHeader.GetSize() - 1;

			for (int p = 0; p <= nLastLine; p++)
			{
				CEposReportTransactionSortedLine line = m_arrayBufferPMSHeader.GetAt(p);

				line.m_strData += (0 == p) ? ",1" : ",0";
				line.m_strData += (nLastLine == p) ? ",1" : ",0";

				m_arrayLines.Add(line);
			}
		}

		{
			CString strTime = "";
			if ((m_ReportSettings.GetTimeType() == 1) && (TRUE == m_TransactionInfo.m_bGotFirstSaleTime))
			{
				CString strDate = "";
				if (m_ReportSettings.GetItemSaleDateFlag() == TRUE)
				{
					strDate.Format("%2.2d/%2.2d/%4.4d   ",
						m_TransactionInfo.m_oleFirstSaleTime.GetDay(),
						m_TransactionInfo.m_oleFirstSaleTime.GetMonth(),
						m_TransactionInfo.m_oleFirstSaleTime.GetYear());
				}

				strTime.Format("%s%2.2d:%2.2d:%2.2d",
					(const char*)strDate,
					m_TransactionInfo.m_oleFirstSaleTime.GetHour(),
					m_TransactionInfo.m_oleFirstSaleTime.GetMinute(),
					m_TransactionInfo.m_oleFirstSaleTime.GetSecond());
			}

			CCSV csvHeader;
			csvHeader.Add(m_dTransactionTotalNonCashbackPayments, 3);
			csvHeader.Add(m_dTransactionTotalNonCashbackSales, 3);
			csvHeader.Add(strTime);

			CEposReportTransactionSortedLine line;
			line.m_nLineType = m_TransactionHeaderLine.m_nLineType;

			if (TRUE == m_bPMSTransaction)
			{
				line.m_strData = "1,";
				line.m_strData += m_TransactionHeaderLine.m_strData;
			}
			else
			{
				line.m_strData = "0,";
				line.m_strData += m_TransactionHeaderLine.m_strData;
			}

			line.m_strData += ",";
			line.m_strData += csvHeader.GetLine();

			m_arrayLines.Add(line);
		}

		for (int g = 0; g < m_arrayBufferGeneral.GetSize(); g++)
		{
			CEposReportTransactionSortedLine line = m_arrayBufferGeneral.GetAt(g);
			m_arrayLines.Add(line);
		}

		for (int d = 0; d < m_arrayBufferDiscounts.GetSize(); d++)
		{
			CEposReportTransactionSortedLine line = m_arrayBufferDiscounts.GetAt(d);
			m_arrayLines.Add(line);
		}

		for (int p = 0; p < m_arrayBufferPayments.GetSize(); p++)
		{
			CEposReportTransactionSortedLine line = m_arrayBufferPayments.GetAt(p);
			m_arrayLines.Add(line);
		}

		{
			int nLastLine = m_arrayBufferFooter.GetSize() - 1;

			for (int f = 0; f <= nLastLine; f++)
			{
				CEposReportTransactionSortedLine line = m_arrayBufferFooter.GetAt(f);
				line.m_strData += (nLastLine == f) ? ",1" : ",0";
				m_arrayLines.Add(line);
			}
		}

		m_TransactionHeaderIndex.m_nTranEndIdx = m_arrayLines.GetSize() - 1;
		m_arrayIndex.SetAt(nIndexPos, m_TransactionHeaderIndex);
	}

	m_TransactionInfo.Reset();
	m_arrayBufferPMSHeader.RemoveAll();
	m_arrayBufferGeneral.RemoveAll();
	m_arrayBufferPayments.RemoveAll();
	m_arrayBufferDiscounts.RemoveAll();
	m_arrayBufferFooter.RemoveAll();
	m_strTransactionHeader = "";
	m_bPMSTransaction = FALSE;
	m_dTransactionTotalNonCashbackSales = 0.0;
	m_dTransactionTotalNonCashbackPayments = 0.0;
}

/**********************************************************************/

void CEposReportTransactionSorted::GetTransactionHeaders(CCSVEposTermLine& csvIn, int nCashRSPLineType)
{
	CCSV csvData;
	csvData.Add(CASHRSP_LINETYPE_TRAN_VOID == nCashRSPLineType ? 1 : 0);
	csvData.Add(csvIn.GetTransactionServer());
	csvData.Add(csvIn.GetTransactionTable());
	csvData.Add(csvIn.GetTransactionCovers());
	csvData.Add(csvIn.GetTransactionValue());
	csvData.Add(csvIn.GetTransactionLoyaltyIDAsString(m_CashRSPVersionChecker.GetCurrentTransactionVersionNo()));
	m_TransactionHeaderLine.m_nLineType = nCashRSPLineType;
	m_TransactionHeaderLine.m_strData = csvData.GetLine();
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferPMSHeader(CCSVEposTermLine& csvIn)
{
	CCSV csvData;
	csvData.Add(csvIn.GetString(1));

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_HEADER_PMS;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferPMSHeader.Add(saleLine);
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferSale(CCSVEposTermLine& csvIn, bool bWastageTransaction)
{
	CString strVoid = csvIn.GetItemVoidFlag();
	CString strRefund = csvIn.GetItemRefundFlag();
	bool bVoid = (strVoid == "V");
	bool bRefund = (strRefund == "R");
	double dValue = csvIn.GetItemLineValueReport();

	int nItemType = 0;
	int nID32 = 0;
	__int64 nID64 = 0;
	csvIn.GetItemTypeAndID(m_CashRSPVersionChecker, nItemType, nID32, nID64);

	int nTranVersionNo = m_CashRSPVersionChecker.GetCurrentTransactionVersionNo();

	CString strDate = csvIn.GetItemSaleDate(nTranVersionNo);
	CString strTime = csvIn.GetItemSaleTime(nTranVersionNo);

	CCSV csvData;
	csvData.Add(csvIn.GetItemPluNo());
	csvData.Add(csvIn.GetItemQty(), 5);
	csvData.Add(dValue, 3);
	csvData.Add(bWastageTransaction);
	csvData.Add(bVoid);
	csvData.Add(bRefund);
	csvData.Add(nItemType);
	csvData.Add(nID32);
	csvData.Add(nID64);
	csvData.Add(csvIn.GetItemWastageVoidFlag(nTranVersionNo));
	csvData.Add(strTime);
	csvData.Add(csvIn.GetItemServerNo(nTranVersionNo));
	csvData.Add(strDate);
	csvData.Add(csvIn.GetItemFreeText(nTranVersionNo));

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_ITEM;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferGeneral.Add(saleLine);

	if (FALSE == bVoid)
	{
		m_dTransactionTotalNonCashbackSales += dValue;

		if (m_ReportSettings.GetTimeType() == 1)
		{
			if ((strDate.GetLength() == 8) && (strTime.GetLength() == 8))
			{
				COleDateTime saleTime = COleDateTime(
					2000 + atoi(strDate.Right(2)),
					atoi(strDate.Mid(3, 2)),
					atoi(strDate.Left(2)),
					atoi(strTime.Left(2)),
					atoi(strTime.Mid(3, 2)),
					atoi(strTime.Right(2)));

				if (saleTime.m_status == COleDateTime::valid)
				{
					if ((FALSE == m_TransactionInfo.m_bGotFirstSaleTime) || (saleTime < m_TransactionInfo.m_oleFirstSaleTime))
					{
						m_TransactionInfo.m_bGotFirstSaleTime = TRUE;
						m_TransactionInfo.m_oleFirstSaleTime = saleTime;
					}
				}
			}
		}
	}
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferPayment(CCSVEposTermLine& csvIn)
{
	bool bIsCash = FALSE;
	int nItemType = 0;
	int nID32 = 0;
	__int64 nID64 = 0;
	csvIn.GetPaymentAccountTypeAndID(m_CashRSPVersionChecker, nItemType, nID32, nID64, bIsCash);

	CCSV csvData;
	csvData.Add(csvIn.GetPaymentNumber());
	csvData.Add(csvIn.GetPaymentName());
	csvData.Add(csvIn.GetPaymentTender());
	csvData.Add(csvIn.GetPaymentAmount());
	csvData.Add(csvIn.GetPaymentGratuity());
	csvData.Add(csvIn.GetPaymentCashback());
	csvData.Add(nItemType);
	csvData.Add(nID32);
	csvData.Add(nID64);

	CEposReportTransactionSortedLine paymentLine;
	paymentLine.m_nLineType = CASHRSP_LINETYPE_PAYMENT;
	paymentLine.m_strData = csvData.GetLine();
	m_arrayBufferPayments.Add(paymentLine);

	m_dTransactionTotalNonCashbackPayments += ((double)csvIn.GetPaymentAmount() / 100.0);
	m_dTransactionTotalNonCashbackPayments -= ((double)csvIn.GetPaymentCashback() / 100.0);
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferNoSale(CCSVEposTermLine& csvIn)
{
	bool bWriteTransaction = TRUE;
	{
		int nSeqNo = m_TransactionHeaderIndex.m_nSequenceNo % 1000000;
		if ((nSeqNo < m_ReportSettings.GetSeqNoFrom()) || (nSeqNo > m_ReportSettings.GetSeqNoTo()))
		{
			bWriteTransaction = FALSE;
		}
	}

	if (TRUE == bWriteTransaction)
	{
		int nServerNo = m_TransactionHeaderIndex.m_nServerNo;

		if ((nServerNo < 0) || (nServerNo > 9999))
		{
			nServerNo = 0;
		}

		if ((nServerNo < m_ReportSettings.GetTranServerFrom()) || (nServerNo > m_ReportSettings.GetTranServerTo()))
		{
			bWriteTransaction = FALSE;
		}
		else if ((nServerNo < m_ReportSettings.GetItemServerFrom()) || (nServerNo > m_ReportSettings.GetItemServerTo()))
		{
			bWriteTransaction = FALSE;
		}
	}

	if (TRUE == bWriteTransaction)
	{
		CEposReportConsolTransactionSorted indexNoSale;
		indexNoSale.m_nSequenceNo = m_TransactionHeaderIndex.m_nSequenceNo;
		indexNoSale.m_nInternalNo = m_arrayIndex.GetSize();
		indexNoSale.m_strDateTime = m_TransactionHeaderIndex.m_strDateTime;
		indexNoSale.m_strDisplayDate = m_TransactionHeaderIndex.m_strDisplayDate;
		indexNoSale.m_strDisplayTime = m_TransactionHeaderIndex.m_strDisplayTime;
		indexNoSale.m_nTranStartIdx = m_arrayLines.GetSize();
		indexNoSale.m_nTranEndIdx = m_arrayLines.GetSize();
		indexNoSale.m_nFileIdx = m_TransactionHeaderIndex.m_nFileIdx;
		m_arrayIndex.ConsolidateWithHint(indexNoSale, m_ReportSettings.GetSortType());

		CCSV csvData;
		csvData.Add(csvIn.GetNoSaleServer());
		CEposReportTransactionSortedLine lineNoSale;
		lineNoSale.m_nLineType = CASHRSP_LINETYPE_NOSALE;
		lineNoSale.m_strData = csvData.GetLine();
		m_arrayLines.Add(lineNoSale);
	}
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferAgeVerify(CCSVEposTermLine& csvIn)
{
	bool bWriteTransaction = TRUE;
	{
		int nSeqNo = m_TransactionHeaderIndex.m_nSequenceNo % 1000000;
		if ((nSeqNo < m_ReportSettings.GetSeqNoFrom()) || (nSeqNo > m_ReportSettings.GetSeqNoTo()))
		{
			bWriteTransaction = FALSE;
		}
	}

	if (TRUE == bWriteTransaction)
	{
		int nServerNo = m_TransactionHeaderIndex.m_nServerNo;

		if ((nServerNo < 0) || (nServerNo > 9999))
		{
			nServerNo = 0;
		}

		if ((nServerNo < m_ReportSettings.GetTranServerFrom()) || (nServerNo > m_ReportSettings.GetTranServerTo()))
		{
			bWriteTransaction = FALSE;
		}
		else if ((nServerNo < m_ReportSettings.GetItemServerFrom()) || (nServerNo > m_ReportSettings.GetItemServerTo()))
		{
			bWriteTransaction = FALSE;
		}
	}

	if (TRUE == bWriteTransaction)
	{
		CEposReportConsolTransactionSorted indexAgeVerify;
		indexAgeVerify.m_nSequenceNo = m_TransactionHeaderIndex.m_nSequenceNo;
		indexAgeVerify.m_nInternalNo = m_arrayIndex.GetSize();
		indexAgeVerify.m_strDateTime = m_TransactionHeaderIndex.m_strDateTime;
		indexAgeVerify.m_strDisplayDate = m_TransactionHeaderIndex.m_strDisplayDate;
		indexAgeVerify.m_strDisplayTime = m_TransactionHeaderIndex.m_strDisplayTime;
		indexAgeVerify.m_nTranStartIdx = m_arrayLines.GetSize();
		indexAgeVerify.m_nTranEndIdx = m_arrayLines.GetSize();
		indexAgeVerify.m_nFileIdx = m_TransactionHeaderIndex.m_nFileIdx;
		m_arrayIndex.ConsolidateWithHint(indexAgeVerify, m_ReportSettings.GetSortType());

		CCSV csvData;
		csvData.Add(csvIn.GetAgeVerifyServer());
		csvData.Add(csvIn.GetAgeVerifyLimit());
		csvData.Add(csvIn.GetAgeVerifyResult());

		CEposReportTransactionSortedLine lineAgeVerify;
		lineAgeVerify.m_nLineType = CASHRSP_LINETYPE_AGEVERIFY;
		lineAgeVerify.m_strData = csvData.GetLine();
		m_arrayLines.Add(lineAgeVerify);
	}
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferPaidIO(CCSVEposTermLine& csvIn)
{
	CString strVoid = csvIn.GetPaidIOVoidFlag();
	int nValue = csvIn.GetPaidIOValue();

	CCSV csvData;
	csvData.Add(csvIn.GetPaidIODirection());
	csvData.Add(csvIn.GetPaidIONumber());
	csvData.Add(strVoid == "V");
	csvData.Add(csvIn.GetPaidIOQty());
	csvData.Add(nValue);

	CEposReportTransactionSortedLine paidioLine;
	paidioLine.m_nLineType = CASHRSP_LINETYPE_PAIDIO;
	paidioLine.m_strData = csvData.GetLine();
	m_arrayBufferGeneral.Add(paidioLine);

	m_dTransactionTotalNonCashbackSales += ((double)nValue) / 100.0;
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferItemDiscount(CCSVEposTermLine& csvIn)
{
	CCSV csvData;
	csvData.Add(csvIn.GetItemDiscountPluNo());
	csvData.Add(csvIn.GetItemDiscountType());
	csvData.Add(csvIn.GetItemDiscountAmount());
	csvData.Add(csvIn.GetItemDiscountRate());
	csvData.Add(csvIn.GetItemDiscountServerNo(m_CashRSPVersionChecker.GetCurrentTransactionVersionNo()));

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_DISCOUNT_ITEM;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferDiscounts.Add(saleLine);

	m_dTransactionTotalNonCashbackSales += ((double)csvIn.GetItemDiscountAmount() / 100.0);
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferSubtotalDiscount(CCSVEposTermLine& csvIn, bool bLoyalty)
{
	CCSV csvData;
	csvData.Add(bLoyalty);
	csvData.Add(csvIn.GetSubtotalDiscountType());
	csvData.Add(csvIn.GetSubtotalDiscountAmount());
	csvData.Add(csvIn.GetSubtotalDiscountRate());
	csvData.Add(csvIn.GetSubtotalDiscountPromo());

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferDiscounts.Add(saleLine);

	m_dTransactionTotalNonCashbackSales += ((double)csvIn.GetSubtotalDiscountAmount() / 100.0);
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferSubtotalDiscountTax( CCSVEposTermLine& csvIn, bool bLoyalty )
{	
	/*
	CCSV csvData;
	csvData.Add( csvIn.GetDiscountTaxBand() );
	csvData.Add( csvIn.GetDiscountTaxBandAmount(), 5 );
	
	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = ( bLoyalty ) ? CASHRSP_LINETYPE_DISCOUNTTAX_LOYALTY : CASHRSP_LINETYPE_DISCOUNTTAX_SUBTOTAL;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferGeneral.Add( saleLine );
	*/
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferVoidDiscount(CCSVEposTermLine& csvIn)
{
	CCSV csvData;
	csvData.Add(csvIn.GetVoidDiscountUsage());
	csvData.Add(csvIn.GetVoidDiscountType());
	csvData.Add(csvIn.GetVoidDiscountAmount());
	csvData.Add(csvIn.GetVoidDiscountRate());
	csvData.Add(csvIn.GetVoidDiscountPromo());

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_DISCOUNT_VOID;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferDiscounts.Add(saleLine);
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferDelivery(CCSVEposTermLine& csvIn)
{
	CString strVoid = csvIn.GetItemVoidFlag();
	CString strRefund = csvIn.GetItemRefundFlag();

	CCSV csvData;
	csvData.Add(csvIn.GetItemPluNo());
	csvData.Add(csvIn.GetItemQty(), 5);
	csvData.Add(strVoid == "V");
	csvData.Add(strRefund == "R");

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_SSFT_DELIVERY;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferGeneral.Add(saleLine);
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferFooter(CCSVEposTermLine& csvIn)
{
	CCSV csvData;
	csvData.Add(csvIn.GetString(1));
	csvData.Add(csvIn.GetString(2));

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_SSFT_FOOTER;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferGeneral.Add(saleLine);
}

/**********************************************************************/

void CEposReportTransactionSorted::BufferUnknown(CCSVEposTermLine& csvIn)
{
	CCSV csvData;
	csvData.Add(csvIn.GetLineTypeString());

	CEposReportTransactionSortedLine saleLine;
	saleLine.m_nLineType = CASHRSP_LINETYPE_UNKNOWN;
	saleLine.m_strData = csvData.GetLine();
	m_arrayBufferGeneral.Add(saleLine);
}

/**********************************************************************/

void CEposReportTransactionSorted::GetServerName(int nDbIdx, int nLocIdx, int nServerNo, CString& strServerName)
{
	CServerSelectionInfo info;
	info.m_nServerNo = nServerNo;

	strServerName = "Unknown";
	switch (EcrmanOptions.GetFeaturesServerSettingsType())
	{
	case SERVER_SYS:
	{
		info.m_nDbNo = 0;
		info.m_nNwkNo = 0;
		info.m_nLocNo = 0;

		int nPos = 0;
		if (DataManagerNonDb.ServerNameTable.FindServer(info, nPos) == TRUE)
		{
			DataManagerNonDb.ServerNameTable.GetServerStatus(nPos, info);
			strServerName = info.m_strServerName;
		}
	}
	break;

	case SERVER_DB:
	{
		info.m_nDbNo = dbDatabase.GetDbNo(nDbIdx);
		info.m_nNwkNo = 0;
		info.m_nLocNo = 0;

		int nPos = 0;
		if (DataManagerNonDb.ServerNameTable.FindServer(info, nPos) == TRUE)
		{
			DataManagerNonDb.ServerNameTable.GetServerStatus(nPos, info);
			strServerName = info.m_strServerName;
		}
	}
	break;

	case SERVER_LOC:
	{
		info.m_nDbNo = dbDatabase.GetDbNo(nDbIdx);
		info.m_nNwkNo = dbLocation.GetNetworkNo(nLocIdx);
		info.m_nLocNo = dbLocation.GetLocNo(nLocIdx);

		int nPos = 0;
		if (DataManagerNonDb.ServerNameTable.FindServer(info, nPos) == TRUE)
		{
			DataManagerNonDb.ServerNameTable.GetServerStatus(nPos, info);
			strServerName = info.m_strServerName;
		}
	}
	break;
	}
}

/**********************************************************************/

void CEposReportTransactionSorted::WriteTransactions(int nDbIdx, int nLocIdx)
{
	int nCurrentFileIdx = -1;

	int nConnectionType = CONNECTION_TYPE_STANDARD_NONE;
	{
		int nDummy = 0;
		dbLocation.GetLocSPOSVersion(nLocIdx, nDummy, nConnectionType);
	}

	CEposReportTransactionBuffer TransactionBuffer;

	for (int n = 0; n < m_arrayIndex.GetSize(); n++)
	{
		WriteBufferedTransaction(TransactionBuffer);
		TransactionBuffer.Reset();

		CEposReportConsolTransactionSorted indexEntry;
		m_arrayIndex.GetAt(n, indexEntry);

		CString strSeqNo;
		int nSeqNo = indexEntry.m_nSequenceNo % 1000000;
		strSeqNo.Format("%6.6d", nSeqNo);

		bool bGotFooter = FALSE;
		bool bDeliveryTransaction = FALSE;
		double dTransactionTotalValue = 0.0;
		double dTransactionTotalPayments = 0.0;
		double dTransactionTotalSales = 0.0;

		if (indexEntry.m_nFileIdx != nCurrentFileIdx)
		{
			CTermFileListInfo infoFile;
			m_arrayFileInfo.GetAt(indexEntry.m_nFileIdx, infoFile);
			CString strHidden = "<HIDE2><..>TRANSACTION DATA FROM ";
			strHidden += infoFile.m_strFilename;
			TransactionBuffer.AddHiddenLine(strHidden);
			nCurrentFileIdx = indexEntry.m_nFileIdx;
		}

		int nCustomerID = 0;
		__int64 nCashlessID = 0;	
		__int64 nSmartPayID = 0;
		__int64 nLoyaltyID = 0;

		int nVNoToWrite = indexEntry.m_nCashRSPVersionNo;
		for (int x = indexEntry.m_nTranStartIdx; x <= indexEntry.m_nTranEndIdx; x++)
		{
			CEposReportTransactionSortedLine line = m_arrayLines.GetAt(x);

			switch (line.m_nLineType)
			{
			case CASHRSP_LINETYPE_ITEM:
			case CASHRSP_LINETYPE_PAYMENT:
			{
				CCSV csvData(line.m_strData);
				int nAccountType = csvData.GetInt(6);

				switch (nAccountType)
				{
				case CASHRSP_ITEMPAYTYPE_CUSTOMER:
					nCustomerID = csvData.GetInt(7);
					break;

				case CASHRSP_ITEMPAYTYPE_SMARTPAY:
					nSmartPayID = csvData.GetInt64(8);
					break;
				}
			}
			}

			if ((nCustomerID != 0) || (nSmartPayID != 0) )
			{
				break;
			}
		}

		for (int x = indexEntry.m_nTranStartIdx; x <= indexEntry.m_nTranEndIdx; x++)
		{
			CEposReportTransactionSortedLine line = m_arrayLines.GetAt(x);

			switch (line.m_nLineType)
			{
			case CASHRSP_LINETYPE_HEADER_PMS:
			{
				CCSV csvData(line.m_strData);

				if (csvData.GetBool(1))
				{
					TransactionBuffer.AddBlanks(1);
					TransactionBuffer.AddNoSearchLine("<LI>");
				}

				CString str = "<..>";
				str += csvData.GetString(0);
				TransactionBuffer.AddReportLine(str, 0);

				if (csvData.GetBool(2))
				{
					TransactionBuffer.AddBlanks(1);
					TransactionBuffer.AddNoSearchLine("<LI>");
				}
			}
			break;

			case CASHRSP_LINETYPE_TRAN_NORMAL:
			case CASHRSP_LINETYPE_TRAN_VOID:
			case CASHRSP_LINETYPE_WASTAGE:
			{
				CCSV csvData(line.m_strData);

				CString strTrnType = "Transaction";
				if (csvData.GetBool(1))
				{
					strTrnType += " Void";
				}

				CCSV csvExtra;

				bool bPMS = csvData.GetBool(0);
				nCashlessID = csvData.GetInt64(6);

				if (nCashlessID != 0)
				{
					switch (SysInfo.GetLoyaltyOrSmartPayType())
					{
					case LOYALTY_TYPE_SMP_V1:
					case LOYALTY_TYPE_SMP_V2:
					case LOYALTY_TYPE_SMP_V4:
						nSmartPayID = nCashlessID;
						nLoyaltyID = 0;
						nCashlessID = 0;
						break;

					case LOYALTY_TYPE_LOY_V2:
					case LOYALTY_TYPE_LOY_V3:
					case LOYALTY_TYPE_LOY_V4:
						nSmartPayID = 0;
						nLoyaltyID = nCashlessID;
						nCashlessID = 0;
						break;
					}
				}

				if (FALSE == bPMS)
				{
					int nServer = csvData.GetInt(2);

					if (nServer != 0)
					{
						CString strServerName = "";
						GetServerName(nDbIdx, nLocIdx, nServer, strServerName);

						CString strServer = "";
						strServer.Format("Server %d (%s)", nServer, (const char*)strServerName);
						csvExtra.Add(strServer);
					}

					int nTable = csvData.GetInt(3);

					if (nTable != 0)
					{
						CString strTable = "";
						strTable.Format(" Table %d", nTable);

						int nTableIdx = 0;
						if (DataManagerNonDb.TableNames.FindTableByNumber(nTable, nTableIdx) == TRUE)
						{
							CTableNameCSVRecord TableName;
							DataManagerNonDb.TableNames.GetAt(nTableIdx, TableName);
							CString strTableName = TableName.GetTableName();
							::TrimSpaces(strTableName, FALSE);

							if (strTableName != "")
							{
								strTable += " (";
								strTable += strTableName;
								strTable += ")";
							}
						}

						csvExtra.Add(strTable);
					}
				}

				{
					int nCovers = csvData.GetInt(4);

					if (nCovers != 0)
					{
						CString strCovers = "";
						strCovers.Format(" Covers %d", nCovers);
						csvExtra.Add(strCovers);
					}
				}

				CCSV csvReport('\t');
				csvReport.Add(indexEntry.m_strDisplayDate);
				csvReport.Add(indexEntry.m_strDisplayTime);
				csvReport.Add(strSeqNo);

				switch (m_ReportSettings.GetTimeType())
				{
				case 1:
					csvReport.Add(csvData.GetString(8));
					break;

				case 2:
					if (m_ReportSettings.GetCashRSPVersionFlag() == FALSE)
					{
						csvReport.Add("");
					}
					else
					{
						CString strVersion = "";
						strVersion.Format("(V%2.2d)", nVNoToWrite);
						csvReport.Add(strVersion);
					}
					break;
				}

				csvReport.Add(strTrnType);
				csvReport.Add(csvExtra.GetLine());
				TransactionBuffer.AddReportLine(csvReport.GetLine(), 0);

				if (nCustomerID != 0)
				{
					int nNodeType, nPos;
					CCustomerSelectionListAccountInfo infoCustomer;
					infoCustomer.m_nLocIdx = nLocIdx;
					infoCustomer.m_nCustomerID = nCustomerID;
					DataManagerNonDb.CustomerNameTable.GetCustomerStatus(infoCustomer, nNodeType, nPos);

					CCSV csvReport('\t');
					csvReport.Add("");
					csvReport.Add("");
					csvReport.Add("");

					switch (nNodeType)
					{
					case NODE_CUSTOMER_ACCOUNT:
						csvReport.Add("Account Customer");
						break;

					case NODE_CUSTOMER_ONEOFF:
						csvReport.Add("One Off Customer");
						break;

					default:
						csvReport.Add("Unknown Customer");
						break;
					}

					csvReport.Add(infoCustomer.GetReportName());
					TransactionBuffer.AddReportLine(csvReport.GetLine(), 0);
				}
				else if (nCashlessID != 0)
				{
					CString strID = "";
					strID.Format("%10.10I64d", nCashlessID);
					TransactionBuffer.AddReportLine(GetTransactionReportLine("", "Cashless ID", strID, nVNoToWrite), 0);
				}
				else if (nSmartPayID != 0)
				{
					CString strID = "";
					strID.Format("%10.10I64d", nSmartPayID);
					TransactionBuffer.AddReportLine(GetTransactionReportLine("", "SmartPay ID", strID, nVNoToWrite), 0);
				}
				else if (nLoyaltyID != 0)
				{
					CString strID = "";
					strID.Format("%10.10I64d", nLoyaltyID);
					TransactionBuffer.AddReportLine(GetTransactionReportLine("", "Loyalty ID", strID, nVNoToWrite), 0);
				}

				dTransactionTotalValue = ((double)csvData.GetInt(5)) / 100.0;
				dTransactionTotalPayments = csvData.GetDouble(7);
				dTransactionTotalSales = csvData.GetDouble(8);
			}
			break;

			case CASHRSP_LINETYPE_NOSALE:
			{
				CCSV csvData(line.m_strData);

				CString strServerName = "";
				int nServer = csvData.GetInt(0);
				GetServerName(nDbIdx, nLocIdx, nServer, strServerName);

				CString strServer = "";
				strServer.Format("Server %d (%s)", nServer, (const char*)strServerName);

				CCSV csvReport('\t');
				csvReport.Add(indexEntry.m_strDisplayDate);
				csvReport.Add(indexEntry.m_strDisplayTime);
				csvReport.Add(strSeqNo);

				switch (m_ReportSettings.GetTimeType())
				{
				case 1:
				case 2:
					csvReport.Add("");
					break;
				}

				csvReport.Add("NO SALE");
				csvReport.Add(strServer);
				TransactionBuffer.AddReportLine(csvReport.GetLine(), 0);
			}
			break;

			case CASHRSP_LINETYPE_AGEVERIFY:
			{
				CCSV csvData(line.m_strData);

				CString strLimit = "Age Check ";
				switch (csvData.GetInt(1))
				{
				case 1:
					strLimit += " 16";
					break;

				case 2:
					strLimit += " 18";
					break;

				case 3:
					strLimit += " 21";
					break;

				case 4:
					strLimit += " 25";
					break;

				default:
					strLimit += " ??";
					break;
				}

				CString strResult = "";
				switch (csvData.GetInt(2))
				{
				case 0:
					strResult = "Declined";
					break;

				case 1:
					strResult = "OK";
					break;

				default:
					strResult = "Unknown result";
					break;
				}

				CString strServerName = "";
				int nServerNo = csvData.GetInt(0);
				GetServerName(nDbIdx, nLocIdx, nServerNo, strServerName);

				CString strServer = "";
				strServer.Format("     Server %d (%s)", nServerNo, (const char*)strServerName);

				CCSV csvReport('\t');
				csvReport.Add(indexEntry.m_strDisplayDate);
				csvReport.Add(indexEntry.m_strDisplayTime);
				csvReport.Add(strSeqNo);

				switch (m_ReportSettings.GetTimeType())
				{
				case 1:
				case 2:
					csvReport.Add("");
					break;
				}

				csvReport.Add(strLimit);
				csvReport.Add(strResult + strServer);
				TransactionBuffer.AddReportLine(csvReport.GetLine(), 0);
			}
			break;

			case CASHRSP_LINETYPE_ITEM:
			{
				CCSV csvData(line.m_strData);

				CSQLPluNoInfo infoPluNo;
				infoPluNo.m_nEposPluNo = csvData.GetInt64(0);
				::ProcessPluNo(infoPluNo, TRUE, TRUE);

				if (TRUE == infoPluNo.m_bValid)
				{
					m_PluInfoFinder.SetPluNo(infoPluNo.m_nBasePluNo);
					CString strDescription = m_PluInfoFinder.GetPluInfoRepText(TRUE, infoPluNo.m_nModifier, m_ReportSettings.GetModifierTextFlag());

					CString strPluNo = ReportHelpers.GetDisplayPluNo(infoPluNo.m_nEposPluNo, TRUE);

					double dLineQty = csvData.GetDouble(1);
					double dLineValue = csvData.GetDouble(2);

					bool bItemRefund = csvData.GetBool(5);
					if (TRUE == bItemRefund)
					{
						dLineValue *= -1;
					}

					CString strFreeText = "";
					{
						CString strLineText = csvData.GetString(13);

						if (strLineText != "")
						{
							strFreeText = " (";
							strFreeText += strLineText;
							strFreeText += ")";
						}
					}

					CString strPluInfo;
					strPluInfo.Format("%s, %s%s, %.2f, %.2f",
						(const char*)strPluNo,
						(const char*)strDescription,
						(const char*)strFreeText,
						dLineQty,
						dLineValue);

					if (m_ReportSettings.GetShowItemServerFlag() == TRUE)
					{
						int nServerNo = csvData.GetInt(11);
						if (nServerNo != 0)
						{
							CString strServerName = "";
							GetServerName(nDbIdx, nLocIdx, nServerNo, strServerName);

							CString strTemp;
							strTemp.Format("Server %d (%s), %s", nServerNo, (const char*)strServerName, (const char*)strPluInfo);
							strPluInfo = strTemp;
						}
					}

					CString strType = "";

					{
						bool bWasteTran = csvData.GetBool(3);
						bool bItemVoid = csvData.GetBool(4);
						bool bWasteVoid = csvData.GetBool(9);

						if (TRUE == bWasteTran)
						{
							strType = "Item Wastage";
						}
						else if (TRUE == bItemVoid)
						{
							if (TRUE == bItemRefund)
							{
								strType = "Void Refund";
							}
							else if (TRUE == bWasteVoid)
							{
								strType = "Void Waste";
							}
							else
							{
								strType = "Void Sale";
							}
						}
						else if (TRUE == bItemRefund)
						{
							strType = "Item Refund";
						}
						else
						{
							strType = "Item Sale";
						}
					}

					CString strTime = "";
					if (m_ReportSettings.GetTimeType() == 2)
					{
						if (m_ReportSettings.GetItemSaleDateFlag() == TRUE)
						{
							CString strDate = csvData.GetString(12);
							if (strDate.GetLength() == 8)
							{
								strTime = strDate.Left(6);
								strTime += "20";
								strTime += strDate.Right(2);
								strTime += "   ";
							}
						}
						strTime += csvData.GetString(10);
					}
					TransactionBuffer.AddReportLine(GetTransactionReportLine(strTime, strType, strPluInfo, nVNoToWrite), csvData.GetInt(11));
				}
			}
			break;

			case CASHRSP_LINETYPE_DISCOUNT_ITEM:
			{
				CCSV csvData(line.m_strData);

				CSQLPluNoInfo infoPluNo;
				infoPluNo.m_nEposPluNo = csvData.GetInt64(0);
				::ProcessPluNo(infoPluNo, TRUE, TRUE);

				if (TRUE == infoPluNo.m_bValid)
				{
					CString strDiscountType = "";
					CString strType = csvData.GetString(1);

					if (strType == "M")
					{
						strDiscountType = "Item Markdown";
					}
					else if (strType == "D")
					{
						strDiscountType = "Item Discount";
					}
					else if (strType == "P")
					{
						strDiscountType = "Item Premium";
					}
					else
					{
						strDiscountType = "Item ??Discount";
					}

					CString strDescription = "**Unknown";

					int nPluIdx = 0;
					if (DataManager.Plu.FindPluByNumber(infoPluNo.m_nBasePluNo, nPluIdx) == TRUE)
					{
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt(nPluIdx, PluRecord);
						strDescription = PluRecord.GetModifierReportText(infoPluNo.m_nModifier);
					}

					CString strPluNo = ReportHelpers.GetDisplayPluNo(infoPluNo.m_nEposPluNo, TRUE);

					CString strPluInfo;
					strPluInfo.Format("%s, %s, ",
						(const char*)strPluNo,
						(const char*)strDescription);

					if (m_ReportSettings.GetShowItemServerFlag() == TRUE)
					{
						int nServerNo = csvData.GetInt(4);
						if (nServerNo != 0)
						{
							CString strServerName = "";
							GetServerName(nDbIdx, nLocIdx, nServerNo, strServerName);

							CString strTemp;
							strTemp.Format("Server %d (%s), %s", nServerNo, (const char*)strServerName, (const char*)strPluInfo);
							strPluInfo = strTemp;
						}
					}

					double dDiscountAmount = ((double)csvData.GetInt(2)) / 100.0;

					CString strAmount = "";
					strAmount.Format("%.2f", dDiscountAmount);

					CString strRate = "";
					if (strType != "M")
					{
						strRate.Format(", %.2f%%", ((double)csvData.GetInt(3)) / 100.0);
					}

					TransactionBuffer.AddReportLine(GetTransactionReportLine("", strDiscountType, strPluInfo + strAmount + strRate, nVNoToWrite), csvData.GetInt(4));
				}
			}
			break;

			case CASHRSP_LINETYPE_DISCOUNT_SUBTOTAL:
			{
				CCSV csvData(line.m_strData);

				CString strDiscountType = (csvData.GetBool(0)) ? "Loyalty " : "Subtotal ";
				CString strType = csvData.GetString(1);

				if (strType == "M")
				{
					strDiscountType += "Markdown";
				}
				else if (strType == "D")
				{
					strDiscountType += "Discount";
				}
				else if (strType == "P")
				{
					strDiscountType += "Premium";
				}
				else
				{
					strDiscountType += "??Discount";
				}

				double dDiscountAmount = ((double)csvData.GetInt(2)) / 100.0;

				CString strAmount = "";
				strAmount.Format("%.2f", dDiscountAmount);

				CString strRate = "";
				if (strType != "M")
				{
					strRate.Format(", %.2f%%", ((double)csvData.GetInt(3)) / 100.0);
				}

				int nPromoNo = csvData.GetInt(4);

				CString strPromo = "";
				strPromo.Format(", Promo %d, ", nPromoNo);

				int nPromoIdx;
				if (DataManager.Promotion.FindTableByNumber(nPromoNo, nPromoIdx) == TRUE)
				{
					CPromotionsCSVRecord Promo;
					DataManager.Promotion.GetAt(nPromoIdx, Promo);
					strPromo += Promo.GetDisplayName();
				}
				else
				{
					strPromo += "Unknown Promotion";
				}

				TransactionBuffer.AddReportLine(GetTransactionReportLine("", strDiscountType, strAmount + strRate + strPromo, nVNoToWrite), 0);
			}
			break;

			case CASHRSP_LINETYPE_DISCOUNTTAX_SUBTOTAL:
			case CASHRSP_LINETYPE_DISCOUNTTAX_LOYALTY:
			{
				/*
				CCSV csvData( line.m_strData );
				CString strInfo;
				strInfo.Format( "Band %s, %.2f", csvData.GetString(0), csvData.GetDouble(1) );
				TransactionBuffer.AddReportLine( GetTransactionReportLine ( "", "Subtotal Discount Tax", strInfo, nVNoToWrite ) );
				*/
			}
			break;

			case CASHRSP_LINETYPE_DISCOUNT_VOID:
			{
				CCSV csvData(line.m_strData);

				CString strDiscountType = "Void ";
				switch (csvData.GetInt(0))
				{
				case 1:
					strDiscountType += "Subtotal";
					break;

				case 2:
					strDiscountType += "Loyalty";
					break;

				case 3:
					strDiscountType += "Differ";
					break;

				default:
					strDiscountType += "??Usage";
					break;
				}

				CString strType = csvData.GetString(1);

				if (strType == "M")
				{
					strDiscountType += " Markdown";
				}
				else if (strType == "D")
				{
					strDiscountType += " Discount";
				}
				else if (strType == "P")
				{
					strDiscountType += " Premium";
				}
				else
				{
					strDiscountType += " ??Discount";
				}

				double dDiscountAmount = ((double)csvData.GetInt(2)) / 100.0;

				CString strAmount = "";
				strAmount.Format("%.2f", dDiscountAmount);

				CString strRate = "";
				if (strType != "M")
				{
					strRate.Format(", %.2f%%", ((double)csvData.GetInt(3)) / 100.0);
				}

				CString strPromo = "";
				strPromo.Format(", Promo %d", csvData.GetInt(4));

				TransactionBuffer.AddReportLine(GetTransactionReportLine("", strDiscountType, strAmount + strRate + strPromo, nVNoToWrite), 0);
			}
			break;

			case CASHRSP_LINETYPE_PAYMENT:
			{
				CCSV csvData(line.m_strData);

				int nPaymentNoSB = 0;
				int nPaymentNoSE = 0;
				int nPaymentNoSPOS = 0;

				switch (nConnectionType)
				{
				case CONNECTION_TYPE_SPTBOOK_NONE:
					nPaymentNoSB = csvData.GetInt(0);
					nPaymentNoSPOS = DataManager.SBPaymentMap.GetSharpPosPaymentNo(nPaymentNoSB);
					break;

				case CONNECTION_TYPE_SMARTENT_NONE:
					nPaymentNoSE = csvData.GetInt(0);
					nPaymentNoSPOS = DataManager.SEPaymentMap.GetSharpPosPaymentNo(nPaymentNoSE);
					break;

				case CONNECTION_TYPE_STANDARD_NONE:
				default:
					nPaymentNoSPOS = csvData.GetInt(0);
					break;
				}

				CString strPaymentType = "";

				strPaymentType.Format("P%2.2d %s",
					nPaymentNoSPOS,
					csvData.GetString(1));

				int nTender = csvData.GetInt(2);
				int nAmount = csvData.GetInt(3);
				int nGratuity = csvData.GetInt(4);
				int nCashback = csvData.GetInt(5);

				CString strTender = "";
				strTender.Format("Tender %.2f", ((double)nTender) / 100.0);

				double dAmount = ((double)nAmount) / 100.0;

				CString strAmount = "";
				if (nAmount != nTender)
					strAmount.Format(", Amount %.2f", dAmount);

				CString strGratuity = "";
				if (nGratuity != 0)
				{
					strGratuity.Format(", Gratuity %.2f", ((double)nGratuity) / 100.0);
				}

				CString strCashback = "";
				if (nCashback != 0)
				{
					strCashback.Format(", Cashback %.2f", ((double)nCashback) / 100.0);
				}

				if ((strAmount == "") && (strGratuity == "") && (strCashback == ""))
				{
					strTender.Format("%.2f", ((double)nTender) / 100.0);
				}

				switch (nConnectionType)
				{
				case CONNECTION_TYPE_SPTBOOK_NONE:
					TransactionBuffer.AddReportLine(GetTransactionReportLine("", "Sports Booker Payment", DataManager.SBPaymentMap.GetSportsBookerText(nPaymentNoSB), nVNoToWrite), 0);
					break;

				case CONNECTION_TYPE_SMARTENT_NONE:
					TransactionBuffer.AddReportLine(GetTransactionReportLine("", "Smart Ent Payment", DataManager.SEPaymentMap.GetSmartEntPaymentName(nPaymentNoSE), nVNoToWrite), 0);
					break;
				}

				CString strRoom = "";
				switch (csvData.GetInt(6))
				{
				case 3:
					strRoom.Format(" (Room %d)", csvData.GetInt(7));
					break;
				}

				TransactionBuffer.AddReportLine(GetTransactionReportLine("", strPaymentType + strRoom, strTender + strAmount + strGratuity + strCashback, nVNoToWrite), 0);
			}
			break;

			case CASHRSP_LINETYPE_PAIDIO:
			{
				CCSV csvData(line.m_strData);

				CString strType = "";

				bool bOut = FALSE;
				int nDirection = csvData.GetInt(0);

				switch (nDirection)
				{
				case 0:
					strType = "Paid Out";
					bOut = TRUE;
					break;

				case 1:
					strType = "Paid In";
					break;

				default:
					strType = "Paid ??";
					break;
				}

				if (csvData.GetBool(2))
				{
					strType += " (Void)";
				}

				//REASON
				int nNo = csvData.GetInt(1);
				CString strReason = "";

				switch (nDirection)
				{
				case 0:
				{
					CPaidInOutTextCSVRecord Text;
					DataManager.PaidOutText.GetReportPaidInOutText(nNo, Text);
					strReason = Text.GetEposText();
				}
				break;

				case 1:
				{
					CPaidInOutTextCSVRecord Text;
					DataManager.PaidInText.GetReportPaidInOutText(nNo, Text);
					strReason = Text.GetEposText();
				}
				break;

				default:
					strReason.Format("Reason %d", nNo);
					break;
				}

				//QTY
				int nQty = csvData.GetInt(3);

				//VAL
				double dVal = ((double)csvData.GetInt(4)) / 100.0;

				if (TRUE == bOut)
				{
					dVal *= -1;
				}

				CString strDetail = "";

				if (nQty != 1)
				{
					strDetail.Format("%.2f  (%s, Qty %d)",
						dVal,
						(const char*)strReason,
						nQty);
				}
				else
				{
					strDetail.Format("%.2f  (%s)",
						dVal,
						(const char*)strReason);
				}

				TransactionBuffer.AddReportLine(GetTransactionReportLine("", strType, strDetail, nVNoToWrite), 0);
			}
			break;

			case CASHRSP_LINETYPE_SSFT_DELIVERY:
			{
				CCSV csvData(line.m_strData);

				CSQLPluNoInfo infoPluNo;
				infoPluNo.m_nEposPluNo = csvData.GetInt(0);
				::ProcessPluNo(infoPluNo, TRUE, TRUE);

				if (TRUE == infoPluNo.m_bValid)
				{
					CString strDescription = "**Unknown";

					int nPluIdx = 0;
					if (DataManager.Plu.FindPluByNumber(infoPluNo.m_nBasePluNo, nPluIdx) == TRUE)
					{
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt(nPluIdx, PluRecord);
						strDescription = PluRecord.GetModifierReportText(infoPluNo.m_nModifier);
					}

					double dQty = csvData.GetDouble(1);

					CString strPluNo = ReportHelpers.GetDisplayPluNo(infoPluNo.m_nEposPluNo, TRUE);

					CString strPluInfo;
					strPluInfo.Format("%s, %s, %.2f",
						(const char*)strPluNo,
						(const char*)strDescription,
						dQty);

					CString strType = "";
					if (csvData.GetBool(2) == TRUE)
					{
						strType = "Item Delivery Void";
					}
					else if (csvData.GetBool(3) == TRUE)
					{
						strType = "Item Delivery Revoke";
					}
					else
					{
						strType = "Item Delivery";
					}

					TransactionBuffer.AddReportLine(GetTransactionReportLine("", strType, strPluInfo, nVNoToWrite), 0);

					bDeliveryTransaction = TRUE;
				}
			}
			break;

			case CASHRSP_LINETYPE_SSFT_FOOTER:
			{
				CCSV csvData(line.m_strData);

				TransactionBuffer.AddReportLine(GetTransactionReportLine("", csvData.GetString(0), csvData.GetString(1), nVNoToWrite), 0);

				if (csvData.GetBool(2))
				{
					TransactionBuffer.AddBlanks(2);
				}

				bGotFooter = TRUE;
			}
			break;

			case CASHRSP_LINETYPE_UNKNOWN:
			{
				CCSV csvData(line.m_strData);
				TransactionBuffer.AddReportLine(GetTransactionReportLine("", "Unknown", csvData.GetString(0), nVNoToWrite), 0);
			}
			break;
			}
		}

		if (FALSE == bDeliveryTransaction)
		{
			CString strTotalValue = "";
			strTotalValue.Format("%.2f", dTransactionTotalValue);
			TransactionBuffer.AddReportLine(GetTransactionReportLine("", "Total Value", strTotalValue, nVNoToWrite), 0);
		}

		if (m_ReportSettings.GetDiscrepancyFlag() == TRUE)
		{
			bool bOverPaid = TRUE;
			double dDiscrepancy = 0.0;
			CString strDirection = "";

			if (dTransactionTotalPayments > dTransactionTotalSales)
			{
				bOverPaid = TRUE;
				dDiscrepancy = dTransactionTotalPayments - dTransactionTotalSales;
				strDirection = "** Over paid";
			}
			else
			{
				bOverPaid = FALSE;
				dDiscrepancy = dTransactionTotalSales - dTransactionTotalPayments;
				strDirection = "** Under paid";
			}

			if (FALSE == bGotFooter)
			{
				TransactionBuffer.AddBlanks(1);

				CString strDiscrepancy;
				strDiscrepancy.Format("%.2f", dDiscrepancy);
				TransactionBuffer.AddNoSearchLine(GetTransactionReportLine("", strDirection, strDiscrepancy, nVNoToWrite));

				TransactionBuffer.AddBlanks(2);
			}

			if (TRUE == bOverPaid)
			{
				TransactionBuffer.SetOverpaid(dDiscrepancy);
			}
			else
			{
				TransactionBuffer.SetUnderpaid(dDiscrepancy);
			}
		}

		TransactionBuffer.AddBlanks(1);
	}

	WriteBufferedTransaction(TransactionBuffer);

	if (m_ReportSettings.GetFilterTextType() != 2)
	{
		m_ReportFile.RequestBlankLines(1);
	}
}

/**********************************************************************/

const char* CEposReportTransactionSorted::GetTransactionReportLine( const char* szTime, const char* szInfo1, const char* szInfo2, int& nVNoToWrite )
{
	CString strInfo1 = szInfo1;
	CString strInfo2 = szInfo2;

	CString strVersion = "";
	if ((nVNoToWrite != 0) && (m_ReportSettings.GetCashRSPVersionFlag() == TRUE))
	{
		strVersion.Format("(V%2.2d)", nVNoToWrite);
	}

	nVNoToWrite = 0;

	CCSV csvLine ( '\t' );
	csvLine.Add( "" );
	csvLine.Add( "" );

	switch( m_ReportSettings.GetTimeType() )
	{
	case 0:
		csvLine.Add( strVersion );
		break;

	case 1:
		csvLine.Add( "" );
		csvLine.Add( strVersion );
		break;

	case 2:
		csvLine.Add( "" );
		csvLine.Add( szTime );
		break;
	}

	csvLine.Add( strInfo1 );
	csvLine.Add( strInfo2 );

	m_strTransactionReportLine = csvLine.GetLine();
	return m_strTransactionReportLine;
}

/**********************************************************************/

void CEposReportTransactionSorted::WriteBufferedTransaction( CEposReportTransactionBuffer& buffer )
{
	bool bWrite = TRUE;
	bool bFirstNormalLine = TRUE;
	CString strFilterText = m_ReportSettings.GetFilterText();
	
	if (strFilterText != "")
	{
		bWrite = FALSE;
		for (int n = 0; n < buffer.GetSize(); n++)
		{
			CEposReportTransactionBufferItem item;
			buffer.GetAt(n, item);

			if (TRANBUFFER_LINETYPE_NORMAL == item.m_nLineType)
			{
				CString strItemUpper = item.m_strText;
				CString strFilterUpper = strFilterText;
				strItemUpper.MakeUpper();
				strFilterUpper.MakeUpper();

				if (strItemUpper.Find(strFilterUpper) != -1)
				{
					bWrite = TRUE;

					if (m_ReportSettings.GetFilterTextType() == 0)
					{
						break;
					}
				}
				else
				{
					bool bIncludeLine = TRUE;
					switch (m_ReportSettings.GetFilterTextType())
					{
					case 1:
						bIncludeLine = bFirstNormalLine;
						break;

					case 2:
						bIncludeLine = FALSE;
						break;
					}

					if (FALSE == bIncludeLine)
					{
						item.m_nLineType = TRANBUFFER_LINETYPE_FILTERED;
						buffer.SetAt(n, item);
					}
				}

				bFirstNormalLine = FALSE;
			}
		}
	}

	if ( TRUE == bWrite )
	{
		int nServerFrom = m_ReportSettings.GetItemServerFrom();
		int nServerTo = m_ReportSettings.GetItemServerTo();

		if ((nServerFrom > 0) || (nServerTo < 9999))
		{
			bWrite = FALSE;
			bool bGotServer = FALSE;

			for (int n = 0; n < buffer.GetSize(); n++)
			{
				CEposReportTransactionBufferItem item;
				buffer.GetAt(n, item);

				if (TRANBUFFER_LINETYPE_NORMAL == item.m_nLineType)
				{
					if ((item.m_nServerNo >= nServerFrom) && (item.m_nServerNo <= nServerTo))
					{
						bWrite = TRUE;
						break;
					}

					if (item.m_nServerNo != 0)
					{
						bGotServer = TRUE;
					}
				}
			}

			if ((FALSE == bGotServer) && (0 == nServerFrom))
			{
				bWrite = TRUE;
			}
		}
	}

	if ( TRUE == bWrite )
	{
		m_dTotalOverPaid += buffer.GetOverpaid();
		m_dTotalUnderPaid += buffer.GetUnderpaid();

		for ( int n = 0; n < buffer.GetSize(); n++ )
		{
			CEposReportTransactionBufferItem item;
			buffer.GetAt( n, item );
			
			switch( item.m_nLineType )
			{
			case TRANBUFFER_LINETYPE_FILTERED:	//EXCLUDED BY TEXT SEARCH
				break;

			case TRANBUFFER_LINETYPE_HIDDEN:	//HIDDEN LINE
				m_ReportFile.WriteHiddenLine( item.m_strText );		
				break;

			case TRANBUFFER_LINETYPE_NOSEARCH:	//VISIBLE LINE, NOT CONSIDERED FOR FILTER
			case TRANBUFFER_LINETYPE_NORMAL:	//VISIBLE LINE, CONSIDERED FOR FILTER
				m_ReportFile.WriteReportDataLine( item.m_strText );		
				break;

			case TRANBUFFER_LINETYPE_BLANKS:	//BLANK LINES
				if (m_ReportSettings.GetFilterTextType() == 0)
				{
					m_ReportFile.RequestBlankLines(item.m_nBlankLineCount);
				}
				break;
			}
		}
	}
}

/**********************************************************************/
