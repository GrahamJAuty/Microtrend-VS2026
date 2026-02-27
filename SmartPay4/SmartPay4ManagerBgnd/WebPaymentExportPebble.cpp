//*******************************************************************
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\WaitDlg.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportPayment\SQLRepositoryPebbleExportPayment.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRepositoryPebbleExportTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransactionSent\SQLRepositoryPebbleExportTransactionSent.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRecordSetPebbleExportTransaction.h"
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRowPebblePaymentType.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\SQLTranAuditPendingExportPebble.h"
#include "..\SmartPay4ManagerBgnd\SQLTranRemovePebbleSentTransactions.h"
//*******************************************************************
#include "PebbleHelpers.h"
#include "WebPaymentAtcTransactionFile.h"
#include "WebPaymentDefines.h"
#include "WebPaymentExportCsvLog.h"
#include "WebPaymentExportTnLog.h"
#include "WebPaymentExportTxLog.h"
//*******************************************************************
#include "WebPaymentExportPebble.h"
//*******************************************************************
#define PEBBLETRAN_NONE 0
#define PEBBLETRAN_SALE 1
#define PEBBLETRAN_ADJUST 2
#define PEBBLETRAN_REFUND 3
#define PEBBLETRAN_TOPUP 4
#define PEBBLETRAN_XFERIN 5
#define PEBBLETRAN_XFEROUT 6
#define PEBBLETRAN_REFUNDSKIP 7
//*******************************************************************

CWebPaymentExportPebbleDone::CWebPaymentExportPebbleDone()
{
	m_nLineID = 1;
	m_bGotPayments = FALSE;
	m_nUserID = 0;
	m_strDate = "";
	m_strTime = "";
	m_nCCNo = 0;
}

//*******************************************************************

CWebPaymentExportPebble::CWebPaymentExportPebble( CWnd* pParent )
{
	m_pParent = pParent;
	m_strError = "";
	m_webOptions.LoadRow(NULL);
}

//*******************************************************************
// Called after importing payments

bool CWebPaymentExportPebble::SendAccountUpdates()
{
	m_strError = "";
	
	CSQLRepositoryPebbleConfig PebbleConfig;
	PebbleConfig.LoadRow(NULL);
	bool bIsSetUp = PebbleConfig.IsSet();

	// check interface setup 
	if ( PebbleConfig.IsSet() == FALSE )
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

	ExportTransactions(PebbleConfig);		
	return TRUE;
}

//*******************************************************************

void CWebPaymentExportPebble::ExportTransactions(CSQLRepositoryPebbleConfig& PebbleConfig)
{
	if (pPebbleHelpers->LoadPebblePayments() == FALSE)
	{
		m_strError = "Unable to load Pebble payment types";
		return;
	}

	m_strError = "";
	CString strWebAtcUpdates = Filenames.GetWebPaymentUpdatesFilename("dat");	// ..\{web}\webupdates.dat

	{
		CSQLTranAuditPendingExportPebble Tran(strWebAtcUpdates);

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
			strMsg += "Unable to prepare Pebble export transactions.\n\n";
			strMsg += m_strError;
			Prompter.Error(strMsg);
		}
		return;
		}
	}

	{
		CSQLTranRemovePebbleSentTransactions Tran;

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
			strMsg += "Unable to check for previously sent transactions.\n\n";
			strMsg += m_strError;
			Prompter.Error(strMsg);
		}
		return;
		}
	}

	CArray<CWebPaymentExportPebbleDone,CWebPaymentExportPebbleDone> arrayTransactionsDone;

	CWorkingDlg WorkingDlg("Updating Pebble Transactions");
	WorkingDlg.Create();

	{
		CString strTmpLogName = ::GetUniqueTempFilename("$31");
		CString strTempLogFilename = System.GetWebPaymentPath(strTmpLogName, nWEBPAYMENT_PEBBLE);

		CCSV csvTmpLogHeader{};
		csvTmpLogHeader.Add(PebbleExportTransaction::MemberID.Label);
		csvTmpLogHeader.Add("Date");
		csvTmpLogHeader.Add("SmartPay Name");
		csvTmpLogHeader.Add(PebbleExportTransaction::Amount.Label);
		csvTmpLogHeader.Add("Comment");
		csvTmpLogHeader.Add(PebbleExportTransaction::UserID.Label);

		CString strTempLogHeader = csvTmpLogHeader.GetLine();

		int nRecordsChecked = 0;
		CSQLRecordSetPebbleExportTransaction RecordSet(NULL, RSParams_PebbleExportTransaction_NormalNoParams());

		CTLogCache TLogCache;
		
		CSQLRowPebbleExportTransaction RowTran;
		while (RecordSet.StepSelectAll(RowTran) == TRUE)
		{
			if (WorkingDlg.SetCaption2RecordsChecked(++nRecordsChecked, TRUE) == TRUE)
			{
				break;
			}

			CArray<CPebbleTransactionItem, CPebbleTransactionItem> arrayTransactionItems;
			CArray<CPebbleTransactionPayment, CPebbleTransactionPayment> arrayTransactionPayments;
			CWebPaymentExportPebbleDone Done;

			int nTranType = PEBBLETRAN_NONE;
			bool bGotInvalidPayments = FALSE;

			switch (RowTran.GetAppNo())
			{
			case APPNO_INSERT:
			case APPNO_DELETE:
			case APPNO_BALANCE:
			case APPNO_REFRESH:
			case APPNO_ADDPOINTS:
			case APPNO_HOTLIST:
			case APPNO_UNHOTLIST:
			case APPNO_REVALUEBONUSPOINTS:
			case APPNO_REWARDBONUSPOINTS:
			case APPNO_REFUNDREWARDBONUSPOINTS:
			case APPNO_IMPORTINSERT:
			case APPNO_BATCHDELETE:
			case APPNO_PARENTPAY:
			case APPNO_WISEPAY:
			case APPNO_TUCASI:
			case APPNO_HOTLISTREPLACE:
			case APPNO_INSERTRECORDSIMS:
			case APPNO_INSERTRECORDPPAY:
			case APPNO_INSERTRECORDWISEPAY:
			case APPNO_IMPORTHOTLIST:
			case APPNO_IMPORTDELETE:
			case APPNO_SCHOOLCOMMS:
			case APPNO_TUCASIv2:
			case APPNO_SQUID:
			case APPNO_PARENTMAIL:
			case APPNO_WPOPENINGBALANCE:
			case APPNO_DELETELEAVER:
			case APPNO_BATCHLEAVER:
			case APPNO_IMPORTLEAVER:
			case APPNO_INSERTRECORDWONDE:
			case APPNO_BROMCOM:
			case APPNO_BIOREG:
			case APPNO_BIOUNREG:
			case APPNO_PEBBLE_TOPUP:
			case APPNO_PEBBLE_TOPUP_REFUND:
			case APPNO_PEBBLE_ORDER:
			case APPNO_PEBBLE_ORDER_REFUND:
			case APPNO_REVALUEBONUSCASH:
			case APPNO_REWARDBONUSCASH:
			case APPNO_REFUNDREWARDBONUSCASH:
			case APPNO_EDIT:
			case APPNO_REDEEM:
			case APPNO_IMPORTUPDATE:
			case APPNO_BATCHUPDATE:
			case APPNO_IMPORTCUSTOM:
			case APPNO_BATCHREDEEM:
			case APPNO_CREDIT:
				break;

			case APPNO_XFERIN:
				if (CompareDoubles(RowTran.GetPurse1(), 0.0, 3) != 0)
				{
					nTranType = PEBBLETRAN_XFERIN;
				}
				break;

			case APPNO_XFEROUT:
				if (CompareDoubles(RowTran.GetPurse1(), 0.0, 3) != 0)
				{
					nTranType = PEBBLETRAN_XFEROUT;
				}
				break;

			case APPNO_ADDCASH:
			case APPNO_IMPORTTOPUP:
				nTranType = PEBBLETRAN_TOPUP;

				if (AUDIT_PC == RowTran.GetSourceType())
				{
					CPebbleTransactionPayment payment;
					payment.m_nType = 0;
					payment.m_dAmount = RowTran.GetAmount();
					arrayTransactionPayments.Add(payment);
				}
				else
				{
					GetTransactionPayments(RowTran, arrayTransactionPayments, Done, TRUE, bGotInvalidPayments);
				}
				break;

			case APPNO_SALE:
				nTranType = PEBBLETRAN_SALE;
				GetTransactionItems(TLogCache, RowTran, arrayTransactionItems);
				GetTransactionPayments(RowTran, arrayTransactionPayments, Done, FALSE, bGotInvalidPayments);
				break;

			case APPNO_REFUNDSALE:
				if (CompareDoubles(RowTran.GetPurse2(), 0.0, 2) == 0)
				{
					nTranType = PEBBLETRAN_SALE;
				}
				else
				{
					nTranType = PEBBLETRAN_REFUNDSKIP;
				}

				GetTransactionItems(TLogCache, RowTran, arrayTransactionItems);
				GetTransactionPayments(RowTran, arrayTransactionPayments, Done, FALSE, bGotInvalidPayments);
				break;

			case APPNO_MEAL:
				{
					nTranType = PEBBLETRAN_SALE;
					
					char cVATBand = 'A';

					{
						int nBand = PebbleConfig.m_Row.GetOfficeMealVATBand();
						if ((nBand >= 1) && (nBand <= 12))
						{
							cVATBand += (nBand - 1);
						}
					}

					CPebbleTransactionItem item;
					item.m_nPluNo = PebbleConfig.m_Row.GetOfficeMealPluNo();
					item.m_strDescription = "Office Meal";
					item.m_strVATBand = cVATBand;
					item.m_dItemQty = 1;
					item.m_dItemCost = RowTran.GetAmount() * (-1);
					arrayTransactionItems.Add(item);

					double dAccountSpend = RowTran.GetPurse1() + RowTran.GetPurse2();

					if (::CompareDoubles(dAccountSpend, 0.0, 2) != 0)
					{
						CPebbleTransactionPayment payment;
						payment.m_nType = 6;
						payment.m_dAmount = dAccountSpend * (-1);
						arrayTransactionPayments.Add(payment);
					}

					if (::CompareDoubles(RowTran.GetCash(), 0.0, 2) != 0)
					{
						CPebbleTransactionPayment payment;
						payment.m_nType = 0;
						payment.m_dAmount = RowTran.GetCash() * (-1);
						arrayTransactionPayments.Add(payment);
					}
				}
				break;
			}

			CString strError = "";
			
			if (TRUE == bGotInvalidPayments)
			{
				strError = "SPOS Payment Type not available in Pebble";
			}

			CSQLRowAccountFull RowAccount;

			if ( (nTranType != PEBBLETRAN_NONE) && ( strError == "" ) )
			{
				CSQLRepositoryAccount RepoAccount;
				RowAccount.SetUserID(RowTran.GetUserID());
				if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					strError = "SmartPay Account Not Found";
				}
			}

			CString strMemberID = RowTran.GetMemberID();

			if ((strError == "") && (strMemberID != ""))
			{
				switch (nTranType)
				{
				case PEBBLETRAN_SALE:
					strError = pPebbleHelpers->SendSaleTransactionBatch(PebbleConfig, RowTran, arrayTransactionItems, arrayTransactionPayments);
					break;

				case PEBBLETRAN_TOPUP:
					Server.SetPebbleDiagnosticMessageBoxFlag(TRUE);
					strError = pPebbleHelpers->SendTopupTransactionBatch(PebbleConfig, RowTran, arrayTransactionPayments);
					break;

				case PEBBLETRAN_XFERIN:
					strError = pPebbleHelpers->SendTransferTransactionBatch(PebbleConfig, RowTran, FALSE);
					break;

				case PEBBLETRAN_XFEROUT:
					strError = pPebbleHelpers->SendTransferTransactionBatch(PebbleConfig, RowTran, TRUE);
					break;
				}

			}

			if (strMemberID == "")
			{
				strError = "No Pebble Member link for SmartPay account";
				Done.m_nLineID = RowTran.GetLineID();
				arrayTransactionsDone.Add(Done);
			}
			else if ( TRUE == bGotInvalidPayments)
			{
				Done.m_nLineID = RowTran.GetLineID();
				arrayTransactionsDone.Add(Done);

				CCSV csv;
				csv.Add(RowTran.GetDate());
				csv.Add(RowTran.GetTime());
				csv.Add(RowTran.GetUserID());
				csv.Add(RowTran.GetCCNo());

				for (int n = 0; n < arrayTransactionPayments.GetSize(); n++)
				{
					CPebbleTransactionPayment Payment = arrayTransactionPayments.GetAt(n);
					csv.Add(Payment.m_nType);
					csv.Add(Payment.m_dAmount, 2);
				}

				CSSFile fileLog;
				if (fileLog.Open(Filenames.GetPebbleInvalidPaymentLogFilename(), "ab") == TRUE)
				{
					fileLog.WriteLine(csv.GetLine());
				}
			}
			
			if (strError == "")
			{
				Done.m_nLineID = RowTran.GetLineID();
				arrayTransactionsDone.Add(Done);

				{
					CSQLRepositoryPebbleExportTransactionSent PrepStatSent;
					CSQLRowPebbleExportTransactionSent RowSent;
					RowSent.SetTranLineID(RowTran.GetLineID());
					PrepStatSent.InsertRow(RowSent, NULL);
				}

				if (nTranType != PEBBLETRAN_NONE)
				{
					CString strDateTime = "";
					strDateTime = RowTran.GetDate();
					strDateTime += " @ ";
					strDateTime += RowTran.GetTime();

					CCSV csv;
					csv.Add(RowTran.GetMemberID());
					csv.Add(strDateTime);
					csv.Add(RowAccount.GetAccountDisplayName());
					csv.Add(RowTran.GetAmount(), 2);
					csv.Add(RowTran.GetWebComment());
					csv.Add(RowTran.GetUserID());

					CString strLine = csv.GetLine();
					LogPendingTransaction(strLine, strTempLogFilename, strTempLogHeader);
				}
			}
			else
			{
				CWebPaymentExportTxLog log;
				log.SaveLog(nWEBEXPORT_UPDATES, nWEBEXPORT_ERROR, strError);

				CString strPersonID = "";

				if (strMemberID != "")
				{
					strPersonID.Format("==> MemberID %s",
						(const char*)strMemberID);
				}
				else
				{
					strPersonID.Format("==> UserID %I64d",
						RowTran.GetUserID());
				}

				log.SaveLog(nWEBEXPORT_UPDATES, nWEBEXPORT_ERROR, strPersonID);
			}
		}

		if (::FileExists(strTempLogFilename) == TRUE)
		{
			if (LogUpdates("Tn", strTempLogFilename) == TRUE)
			{
				CFileRecycler::SendToRecycleBin(strTempLogFilename);
			}
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

	CSQLRepositoryPebbleExportTransaction PrepStatTran;
	CSQLRowPebbleExportTransaction RowTran;

	for (int n = 0; n < arrayTransactionsDone.GetSize(); n++)
	{
		CWebPaymentExportPebbleDone Done = arrayTransactionsDone.GetAt(n);

		RowTran.SetLineID(Done.m_nLineID);
		PrepStatTran.DeleteRow(RowTran, NULL);

		if (TRUE == Done.m_bGotPayments)
		{
			CSQLRowPebbleExportPayment RowPayment;
			RowPayment.SetUserID(Done.m_nUserID);
			RowPayment.SetDate(Done.m_strDate);
			RowPayment.SetTime(Done.m_strTime);
			RowPayment.SetCCNo(Done.m_nCCNo);

			CSQLRepositoryPebbleExportPayment PrepStatPayment;
			PrepStatPayment.DeleteRow(RowPayment, NULL);
		}
	}
}

//******************************************************************

void CWebPaymentExportPebble::GetTransactionItems(CTLogCache& TLogCache, CSQLRowPebbleExportTransaction& RowTran, CArray<CPebbleTransactionItem, CPebbleTransactionItem>& arrayTransactionItems)
{
	arrayTransactionItems.RemoveAll();

	bool bNewTLogDatabase = FALSE;

	CTLogDatabase* pTLogDatabase = NULL;
	__int64 nUserID = RowTran.GetUserID();

	CSortedIntByInt64 item;
	item.m_nKey = nUserID;

	int nPos = 0;
	if (TLogCache.FindMapEntry(item, nPos) == TRUE)
	{
		TLogCache.GetMapEntry(nPos, item);
		pTLogDatabase = TLogCache.GetTLogDatabase(item.m_nVal);
	}
	else
	{
		item.m_nVal = TLogCache.GetTLogDatabaseCount();
		TLogCache.InsertMapEntry(nPos, item);

		pTLogDatabase = new (CTLogDatabase);
		TLogCache.AddTLogDatabase(pTLogDatabase);
		bNewTLogDatabase = TRUE;
	}

	if (pTLogDatabase != NULL)
	{
		if (TRUE == bNewTLogDatabase)
		{
			CString strUserID = "";
			strUserID.Format("%I64d", RowTran.GetUserID());
			pTLogDatabase->Open(CSQLAuditRecord::GetHistoryFilenameStatic(strUserID));
		}

		pTLogDatabase->GetPebbleTransactionItems(RowTran.GetDate(), RowTran.GetTime(), RowTran.GetCCNo(), arrayTransactionItems);
	}
}

//******************************************************************

void CWebPaymentExportPebble::GetTransactionPayments(CSQLRowPebbleExportTransaction& RowTran, CArray<CPebbleTransactionPayment, CPebbleTransactionPayment>& arrayTransactionPayments, CWebPaymentExportPebbleDone& Done, bool bTopup, bool& bGotInvalidPayment)
{
	arrayTransactionPayments.RemoveAll();
	Done.m_bGotPayments = FALSE;

	CSQLRowPebbleExportPayment RowPayments;
	RowPayments.SetUserID(RowTran.GetUserID());
	RowPayments.SetDate(RowTran.GetDate());
	RowPayments.SetTime(RowTran.GetTime());
	RowPayments.SetCCNo(RowTran.GetCCNo());

	CSQLRepositoryPebbleExportPayment PrepStatPayment;
	if (PrepStatPayment.SelectRow(RowPayments, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		Done.m_bGotPayments = TRUE;
		Done.m_nUserID = RowPayments.GetUserID();
		Done.m_strDate = RowPayments.GetDate();
		Done.m_strTime = RowPayments.GetTime();
		Done.m_nCCNo = RowPayments.GetCCNo();

		for (int n = 0; n < MAX_PEBBLE_EXPORT_PAYMENT; n++)
		{
			if (RowPayments.GetPaymentType(n) != -1)
			{
				CPebbleTransactionPayment Payment;
				Payment.m_nType = RowPayments.GetPaymentType(n);
				Payment.m_dAmount = RowPayments.GetPaymentAmount(n);
				arrayTransactionPayments.Add(Payment);
				bGotInvalidPayment |= (pPebbleHelpers->IsValidSPOSPaymentType(Payment.m_nType, bTopup) == FALSE);
			}
		}
	}
	else
	{
		bGotInvalidPayment = TRUE;
	}
}

//******************************************************************

bool CWebPaymentExportPebble::LogPendingTransaction(CString strLine, CString strTempLogFilename, CString strTempLogHeader)
{
	bool bReply = FALSE;
	bool bFileExists = ::FileExists(strTempLogFilename);

	CSSFile file;
	if ((bReply = file.Open(strTempLogFilename, "ab")) == TRUE)
	{
		if (FALSE == bFileExists)
		{
			file.WriteLine(strTempLogHeader);
		}

		file.WriteLine(strLine);
		file.Close();
	}
	return bReply;
}

//***************************************************************************

bool CWebPaymentExportPebble::LogUpdates(CString strPrefix, CString strTempLogFilename)
{
	CWebPaymentExportTnLog log(nWEBPAYMENT_PEBBLE, strPrefix);

	CString strLogFile = log.GetCurrentLogName();						// ..\{WebProvider}}\Tnyyyymm.dat
	int nExportLineCount = ::GetFileLineCount(strTempLogFilename) - 1;		// number of lines less header line

	bool bReply = ::AppendDataFile(strTempLogFilename, strLogFile, TRUE);	// append updates & save in logfile in web based csv format

	log.AddDateStamp(strLogFile, nExportLineCount);				// add date stamp line 'Sent : dd/mm/yy @ hh:mm ' ONLY if LineCount > 0

	return bReply;
}

//***************************************************************************
