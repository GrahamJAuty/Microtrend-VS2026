/**********************************************************************/
#include "LocationCSVArray.h"
#include "NetworkCSVArray.h"
#include "RepCSV.h"
#include "StringDateChecker.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayChartwellsExportCreator.h"
/**********************************************************************/

CPosTrayChartwellsExportTranItem::CPosTrayChartwellsExportTranItem()
{
	Reset();
}

/**********************************************************************/

void CPosTrayChartwellsExportTranItem::Reset()
{
	m_nTransactionID = 0;
	m_strDate = "";
	m_strTime = "";
	m_strAccountID = "";
	/*****/
	m_nQty = 0;
	m_nValue = 0;
	m_nDiscount = 0;
	m_nVAT = 0;
	m_bGotDiscount = FALSE;
}

/**********************************************************************/

void CPosTrayChartwellsExportTranItem::Add(CPosTrayChartwellsExportTranItem& source)
{
	m_nQty += source.m_nQty;
	m_nValue += source.m_nValue;
	m_nDiscount += source.m_nDiscount;
	m_nVAT += source.m_nVAT;
	m_bGotDiscount |= source.m_bGotDiscount;
}

/**********************************************************************/

int CPosTrayChartwellsExportTranItem::Compare(CPosTrayChartwellsExportTranItem& source, int nHint)
{
	if (m_nTransactionID != source.m_nTransactionID)
	{
		return (m_nTransactionID > source.m_nTransactionID ? 1 : -1);
	}
	else if (m_strDate != source.m_strDate)
	{
		return (m_strDate > source.m_strDate ? 1 : -1);
	}
	else if (m_strTime != source.m_strTime)
	{
		return (m_strTime > source.m_strTime ? 1 : -1);
	}
	else if (m_strAccountID != source.m_strAccountID)
	{
		return (m_strAccountID > source.m_strAccountID ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CPosTrayChartwellsExportCreator::CreateHeaderFile(CString& strFolder, CArray<CTermFileListInfo, CTermFileListInfo>& arrayFileInfo)
{
	m_TransactionInfo.m_nCurrentTransactionID = -1;

	CString strOutputFile;
	strOutputFile.Format("%s\\%s_%s_HEADER.CSV",
		(const char*)strFolder,
		(const char*)m_strCheckDate,
		(const char*)ChartwellsOptions.GetChartwellsSiteNo());

	CString strPayDate = "";
	strPayDate.Format("%s/%s/%s",
		(const char*)m_strCheckDate.Right(2),
		(const char*)m_strCheckDate.Mid(4, 2),
		(const char*)m_strCheckDate.Left(4));

	CSSFile fileOutput;
	if (fileOutput.Open(strOutputFile, "wb") == FALSE)
	{
		return FALSE;
	}

	{
		CCSV csvHeader;
		csvHeader.Add("transaction_id");
		csvHeader.Add("date");
		csvHeader.Add("time");
		csvHeader.Add("account_id");
		csvHeader.Add("accountgroup_no");
		csvHeader.Add("accountgroup_name");
		csvHeader.Add("year");
		csvHeader.Add("class");
		csvHeader.Add("location_id");
		csvHeader.Add("location_name");
		csvHeader.Add("terminal_id");
		csvHeader.Add("terminal_name");
		csvHeader.Add("operator_id");
		csvHeader.Add("operator_name");
		csvHeader.Add("priceband_id");
		csvHeader.Add("priceband_name");
		csvHeader.Add("item_void");
		csvHeader.Add("item_refund");
		csvHeader.Add("item_discount");
		csvHeader.Add("tran_qty");
		csvHeader.Add("tran_grossvalue");
		csvHeader.Add("tran_discount");
		csvHeader.Add("tran_netofdiscount");
		csvHeader.Add("tran_vatamount");
		csvHeader.Add("tran_netofvat");
		fileOutput.WriteLine(csvHeader.GetLine());
	}

	for (int nFileIdx = 0; nFileIdx < arrayFileInfo.GetSize(); nFileIdx++)
	{
		CTermFileListInfo infoFile = arrayFileInfo.GetAt(nFileIdx);

		CSSFile fileSales;
		if (fileSales.Open(infoFile.m_strFilename, "rb", _SH_DENYNO) == FALSE)
		{
			continue;
		}

		CString strBuffer = "";
		CCSV csvPendingLine;
		bool bInVoidTransaction = FALSE;

		while (fileSales.ReadString(strBuffer) == TRUE)
		{
			CCSVEposTermLine csvIn(strBuffer);
			int nCashRSPLineType = csvIn.GetLineTypeInt();

			switch (nCashRSPLineType)
			{
			case CASHRSP_LINETYPE_TRAN_NORMAL:
			case CASHRSP_LINETYPE_TRAN_VOID:
			case CASHRSP_LINETYPE_NOSALE:
			case CASHRSP_LINETYPE_AGEVERIFY:
			case CASHRSP_LINETYPE_VERSION:
			case CASHRSP_LINETYPE_APPLICATION:
			{
				if (csvPendingLine.GetSize() != 0)
				{
					fileOutput.WriteLine(csvPendingLine.GetLine());
				}
				csvPendingLine.RemoveAll();
			}
			break;
			}

			switch (nCashRSPLineType)
			{
			case CASHRSP_LINETYPE_TRAN_NORMAL:
			case CASHRSP_LINETYPE_NOSALE:
			case CASHRSP_LINETYPE_AGEVERIFY:
			case CASHRSP_LINETYPE_VERSION:
			case CASHRSP_LINETYPE_APPLICATION:
				bInVoidTransaction = FALSE;
				break;

			case CASHRSP_LINETYPE_TRAN_VOID:
				bInVoidTransaction = TRUE;
				break;
			}

			switch (nCashRSPLineType)
			{
			case CASHRSP_LINETYPE_TRAN_NORMAL:
			case CASHRSP_LINETYPE_TRAN_VOID:
			{
				m_TransactionInfo.m_strNewAccountID = csvIn.GetTransactionLoyaltyIDAsString(44);
				m_TransactionInfo.m_strNewDate = strPayDate;
				m_TransactionInfo.m_nNewTerminalNo = dbLocation.GetTNo(infoFile.m_nLocIdx, infoFile.m_nTermIdx);
				m_TransactionInfo.m_nNewTransactionID = csvIn.GetTransactionNumber();
				m_TransactionInfo.CheckNewSettings(TRUE);

				int nServerNo = csvIn.GetTransactionServer();
				CString strServerName = GetServerName(nServerNo);

				CString strOutputDate = GetOutputDate(csvIn.GetTransactionDate());

				CPosTrayChartwellsExportTranItem TranItem;
				TranItem.m_nTransactionID = m_TransactionInfo.m_nCurrentTransactionID;
				TranItem.m_strDate = strOutputDate;
				TranItem.m_strTime = csvIn.GetTransactionTime();
				TranItem.m_strAccountID = m_TransactionInfo.m_strCurrentAccountID;

				{
					int nPos = 0;
					if (m_TranBuffer.Find(TranItem, nPos) == TRUE)
					{
						m_TranBuffer.GetAt(nPos, TranItem);
					}
				}

				csvPendingLine.RemoveAll();
				csvPendingLine.Add(m_TransactionInfo.m_nCurrentTransactionID);
				csvPendingLine.Add(strOutputDate);
				csvPendingLine.Add(csvIn.GetTransactionTime());
				csvPendingLine.Add(m_TransactionInfo.m_strCurrentAccountID);
				csvPendingLine.Add(m_TransactionInfo.m_nGroupNo);
				csvPendingLine.Add(m_TransactionInfo.m_strGroupName);
				csvPendingLine.Add(m_TransactionInfo.m_strClass);
				csvPendingLine.Add(m_TransactionInfo.m_strYear);
				csvPendingLine.Add(dbLocation.GetNetworkLocNo(infoFile.m_nLocIdx));
				csvPendingLine.Add(dbLocation.GetName(infoFile.m_nLocIdx));
				csvPendingLine.Add(m_TransactionInfo.m_nCurrentTerminalNo);
				csvPendingLine.Add(dbLocation.GetTerminalName(infoFile.m_nLocIdx, infoFile.m_nTermIdx));
				csvPendingLine.Add(nServerNo);
				csvPendingLine.Add(strServerName);

				if (m_TransactionInfo.m_nTranPriceBand != 0)
				{
					csvPendingLine.Add(m_TransactionInfo.m_nTranPriceBand);
					csvPendingLine.Add(DataManager.PriceText.GetReportText(m_TransactionInfo.m_nTranPriceBand - 1));
				}
				else
				{
					csvPendingLine.Add("");
					csvPendingLine.Add("");
				}

				csvPendingLine.Add("N");
				csvPendingLine.Add("N");
				csvPendingLine.Add(TranItem.m_bGotDiscount ? "Y" : "N");
				csvPendingLine.Add(QtyToString(TranItem.m_nQty));
				csvPendingLine.Add(PenceToString(TranItem.m_nValue));
				csvPendingLine.Add(PenceToString(TranItem.m_nDiscount));
				csvPendingLine.Add(PenceToString(TranItem.m_nValue - TranItem.m_nDiscount));
				csvPendingLine.Add(PenceToString(TranItem.m_nVAT));
				csvPendingLine.Add(PenceToString(TranItem.m_nValue - TranItem.m_nDiscount - TranItem.m_nVAT));
			}
			break;

			case CASHRSP_LINETYPE_ITEM:
			{
				if (csvPendingLine.GetSize() != 0)
				{
					CString strVoid = csvIn.GetItemVoidFlag();

					if ( (strVoid == "V") || ( TRUE == bInVoidTransaction))
					{
						csvPendingLine.SetAt(16, "Y");
					}
					else 
					{
						CString strRefund = csvIn.GetItemRefundFlag();

						if (strRefund == "R")
						{
							csvPendingLine.SetAt(17, "Y");
						}
					}
				}
			}
			break;
			}
		}

		if (csvPendingLine.GetSize() != 0)
		{
			fileOutput.WriteLine(csvPendingLine.GetLine());
		}

		fileSales.Close();
	}

	fileOutput.Close();
	RedirectToEmail(strOutputFile, "HEADER");

	return TRUE;
}

/**********************************************************************/

