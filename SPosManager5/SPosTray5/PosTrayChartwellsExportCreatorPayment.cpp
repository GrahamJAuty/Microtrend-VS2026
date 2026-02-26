/**********************************************************************/
#include "..\CommonEcrStockTray\PriceHelpers.h"
/**********************************************************************/
#include "..\SmartPaySQL\SQLTable_ChartwellExportPayment\SQLPrepStatChartwellExportPayment.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "PosTrayChartwellsExportCreator.h"
/**********************************************************************/

bool CPosTrayChartwellsExportCreator::CreatePaymentFile(CString& strFolder, CArray<CTermFileListInfo, CTermFileListInfo>& arrayFileInfo)
{
	m_TransactionInfo.m_nCurrentTransactionID = -1;

	CString strOutputFile;
	strOutputFile.Format("%s\\%s_%s_PAYMENT.CSV",
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
		csvHeader.Add("payment_id");
		csvHeader.Add("payment_name");
		csvHeader.Add("paymenttype_id");
		csvHeader.Add("paymenttype_name");
		csvHeader.Add("payment_amount");
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

		m_PaymentBuffer.Reset();

		CString strBuffer = "";
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
				if (TRUE == m_PaymentBuffer.m_bInTransaction)
				{
					WritePaymentBuffer(fileOutput, infoFile);
				}
				m_PaymentBuffer.Reset();
				break;
			}

			if (CASHRSP_LINETYPE_TRAN_NORMAL == nCashRSPLineType)
			{
				CString strAccountID = csvIn.GetTransactionLoyaltyIDAsString(44);

				m_PaymentBuffer.Reset();
				m_PaymentBuffer.m_bInTransaction = TRUE;
				m_TransactionInfo.m_strNewAccountID = strAccountID;
				m_TransactionInfo.m_nNewTransactionID = csvIn.GetTransactionNumber();
				m_TransactionInfo.m_nNewTerminalNo = dbLocation.GetTNo(infoFile.m_nLocIdx, infoFile.m_nTermIdx);
				m_TransactionInfo.m_strNewDate = strPayDate;
				m_TransactionInfo.CheckNewSettings(TRUE);

				m_PaymentBuffer.m_strDate = csvIn.GetTransactionDate();
				m_PaymentBuffer.m_strTime = csvIn.GetTransactionTime();
				m_PaymentBuffer.m_nServerNo = csvIn.GetTransactionServer();
			}

			if (CASHRSP_LINETYPE_PAYMENT == nCashRSPLineType)
			{
				if (m_PaymentBuffer.m_bInTransaction == TRUE)
				{
					int nPaymentNo = csvIn.GetPaymentNumber();
					double dPaymentAmount = (double)csvIn.GetPaymentAmount() / 100.0;

					switch (nPaymentNo)
					{
					case 20:
					case 21:
					{
						m_PaymentBuffer.m_dPursePayment += dPaymentAmount;
					}
					break;

					default:
					{
						CPosTrayChartwellsPaymentInfo infoPayment;
						infoPayment.m_nPaymentNo = csvIn.GetPaymentNumber();
						infoPayment.m_strPaymentName = csvIn.GetPaymentName();
						infoPayment.m_nPaymentType = csvIn.GetPaymentType();
						infoPayment.m_dAmount = (double)csvIn.GetPaymentAmount() / 100.0;
						m_PaymentBuffer.m_arrayPayment.Add(infoPayment);
					}
					break;
					}
				}
			}
		}

		if (TRUE == m_PaymentBuffer.m_bInTransaction)
		{
			WritePaymentBuffer(fileOutput, infoFile);
		}

		fileSales.Close();
	}

	fileOutput.Close();
	RedirectToEmail(strOutputFile, "PAYMENT");

	return TRUE;
}

/**********************************************************************/

void CPosTrayChartwellsExportCreator::WritePaymentBuffer(CSSFile& fileOut, CTermFileListInfo& infoFile)
{
	CString strOutputDate = GetOutputDate(m_PaymentBuffer.m_strDate);
	
	for (int n = 0; n < m_PaymentBuffer.m_arrayPayment.GetSize(); n++)
	{
		CPosTrayChartwellsPaymentInfo infoPayment = m_PaymentBuffer.m_arrayPayment.GetAt(n);
		
		CCSV csvOut;
		csvOut.Add(m_TransactionInfo.m_nCurrentTransactionID);
		csvOut.Add(strOutputDate);
		csvOut.Add(m_PaymentBuffer.m_strTime);
		csvOut.Add(m_TransactionInfo.m_strCurrentAccountID);
		csvOut.Add(m_TransactionInfo.m_nGroupNo);
		csvOut.Add(m_TransactionInfo.m_strGroupName);
		csvOut.Add(m_TransactionInfo.m_strClass);
		csvOut.Add(m_TransactionInfo.m_strYear);
		csvOut.Add(dbLocation.GetNetworkLocNo(infoFile.m_nLocIdx));
		csvOut.Add(dbLocation.GetName(infoFile.m_nLocIdx));
		csvOut.Add(m_TransactionInfo.m_nCurrentTerminalNo);
		csvOut.Add(dbLocation.GetTerminalName(infoFile.m_nLocIdx, infoFile.m_nTermIdx));
		csvOut.Add(m_PaymentBuffer.m_nServerNo);
		csvOut.Add(GetServerName(m_PaymentBuffer.m_nServerNo));
		csvOut.Add(infoPayment.m_nPaymentNo);
		csvOut.Add(infoPayment.m_strPaymentName);
		csvOut.Add(infoPayment.m_nPaymentType);
		csvOut.Add(GetPaymentTypeName(infoPayment.m_nPaymentType));
		csvOut.Add(infoPayment.m_dAmount,2);
		fileOut.WriteLine(csvOut.GetLine());
	}

	if (CPriceHelpers::CompareDoubles(m_PaymentBuffer.m_dPursePayment, 0.0, 3) != 0)
	{
		double dLogged = 0.0;

		if (::TestNumeric(m_TransactionInfo.m_strCurrentAccountID) == TRUE)
		{
			CSQLRowChartwellExportPayment RowPayment;
			RowPayment.SetUserID(_atoi64(m_TransactionInfo.m_strCurrentAccountID));
			RowPayment.SetTransactionID(m_TransactionInfo.m_nCurrentTransactionID);
			RowPayment.SetTerminalNo(m_TransactionInfo.m_nCurrentTerminalNo);
			RowPayment.SetDate(m_TransactionInfo.m_strCurrentDate);

			CSQLPrepStatChartwellExportPayment SQLPrepStatChartwellExportPayment;
			if (SQLPrepStatChartwellExportPayment.SelectRow(RowPayment, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				LogCanteenPayment(fileOut, infoFile, strOutputDate, 1, RowPayment.GetPaymentPurse1(), "CanteenPurse1", dLogged);
				LogCanteenPayment(fileOut, infoFile, strOutputDate, 2, RowPayment.GetPaymentPurse2(), "CanteenFSMPurse", dLogged);
				LogCanteenPayment(fileOut, infoFile, strOutputDate, 3, RowPayment.GetPaymentPurse3(), "CanteenPurse3", dLogged);
			}
		}

		if (CPriceHelpers::CompareDoubles(dLogged, m_PaymentBuffer.m_dPursePayment, 3) != 0)
		{
			LogCanteenPayment(fileOut, infoFile, strOutputDate, 0, m_PaymentBuffer.m_dPursePayment - dLogged, "CanteenPurse", dLogged);
		}
	}
}

/**********************************************************************/

void CPosTrayChartwellsExportCreator::LogCanteenPayment(CSSFile& fileOut, CTermFileListInfo& infoFile, CString strOutputDate, int nPurse, double dAmount, CString strName, double& dLogged)
{
	if (CPriceHelpers::CompareDoubles(dAmount, 0.0, 3) != 0)
	{
		CCSV csvOut;
		csvOut.Add(m_TransactionInfo.m_nCurrentTransactionID);
		csvOut.Add(strOutputDate);
		csvOut.Add(m_PaymentBuffer.m_strTime);
		csvOut.Add(m_TransactionInfo.m_strCurrentAccountID);
		csvOut.Add(m_TransactionInfo.m_nGroupNo);
		csvOut.Add(m_TransactionInfo.m_strGroupName);
		csvOut.Add(m_TransactionInfo.m_strClass);
		csvOut.Add(m_TransactionInfo.m_strYear);
		csvOut.Add(dbLocation.GetNetworkLocNo(infoFile.m_nLocIdx));
		csvOut.Add(dbLocation.GetName(infoFile.m_nLocIdx));
		csvOut.Add(m_TransactionInfo.m_nCurrentTerminalNo);
		csvOut.Add(dbLocation.GetTerminalName(infoFile.m_nLocIdx, infoFile.m_nTermIdx));
		csvOut.Add(m_PaymentBuffer.m_nServerNo);
		csvOut.Add(GetServerName(m_PaymentBuffer.m_nServerNo));
		csvOut.Add(1000 + nPurse);
		csvOut.Add(strName);
		csvOut.Add(1000 + nPurse);
		csvOut.Add(strName);
		csvOut.Add(dAmount, 2);
		fileOut.WriteLine(csvOut.GetLine());
		dLogged += dAmount;
	}
}

/**********************************************************************/

CString CPosTrayChartwellsExportCreator::GetPaymentTypeName(int n)
{
	CString strName = "";
	switch (n)
	{
	case 0:
		strName = "Cash";
		break;

	case 1:
		strName = "Cheque";
		break;

	case 2:
	case 3:
	case 9:
		strName = "EFT";
		break;

	case 4:
	case 5:
		strName = "Post To Room";
		break;

	case 6:
		strName = "CanteenPurse";
		break;

	case 7:
		strName = "Currency";
		break;

	case 8:
		strName = "Deposit";
		break;

	case 10:
		strName = "Customer Account";
		break;

	case 11:
		strName = "Credit Note";
		break;

	case 12:
		strName = "Gift Card";
		break;

	case 13:
		strName = "External App";
		break;

	case 14:
		strName = "Guestline";
		break;

	case 15:
		strName = "External Payment";
		break;

	case 99:
		strName = "Other";
		break;

	default:
		strName = "Unknown";
		break;
	}

	return strName;
}

/**********************************************************************/

