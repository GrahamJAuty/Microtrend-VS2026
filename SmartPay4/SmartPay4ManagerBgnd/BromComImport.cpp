//*******************************************************************
#include "..\SmartPay4ManagerBgNd\SQLTranBromComImport.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_BromComItem\SQLRepositoryBromComItem.h"
#include "..\SmartPay4Shared\SQLTable_BromComBalanceChanges\SQLRepositoryBromComBalanceChanges.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRepositoryBromComPendingOrder.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingItem\SQLRepositoryBromComPendingItem.h"
//*******************************************************************
#include "WebPaymentDefines.h"
//*******************************************************************
#include "BromComImport.h"
//*******************************************************************

CBromComImport::CBromComImport(CWnd* pParent) : m_WorkingDlg( "Import BromCom Orders")
{
	m_pParent = pParent;
	m_bNewPayments = TRUE;
	m_bDisplayImportResults = FALSE;

	// Read BromCom System data
	m_BromComData.Read();													
	m_strBromComError = (m_BromComData.IsSet() == FALSE) ? szINTERFACENOTSET : "";

	m_strTitle = "BromCom Web Payment Import";

	m_nExceptionCount = 0;
	m_nNewExceptionCount = 0;
	m_nValidPaymentCount = 0;
	
	m_strLogFileName = "";
	m_strTempLogFile = "";
	m_strExceptionName = "";
	m_strNewExceptionName = "";
	m_strTempExceptionFile = "";
	m_strExceptionIndicator = Filenames.GetWebPaymentExceptionIndicator();		// web payment exception indicator file

	m_strFilenamePendingOrders = "";
	m_strFilenamePendingItems = "";
}

//*******************************************************************

bool CBromComImport::Import(bool bNewPayments)
{
	m_bDisplayImportResults = FALSE;

	bool bAttemptedTransaction = FALSE;
	CSQLTranBromComImport Tran( m_WorkingDlg, m_BromComData, bNewPayments, m_pParent);
	Tran.SetBromComError(m_strBromComError);
	Tran.SetValidPaymentCount(m_nValidPaymentCount);

	//DO NOT ATTEMPT TRANSACTION IF NO NEW PAYMENTS OR IF BROM COM SETTINGS NOT LOADED
	if ( (m_strBromComError == "") && ( TRUE == bNewPayments ) )
	{
		if ( Tran.BeginTrans() == SQLTRAN_STATE_ACTIVE)
		{
			m_WorkingDlg.Create();
			bAttemptedTransaction = TRUE;

			Tran.DoWork();
	
			switch (Tran.EndTrans())
			{
			case SQLTRAN_STATE_COMMITOK:
				break;

			default:
				Tran.SetBromComError("Unable to process SQL transaction");
				break;
			}
		}
		else
		{
			Tran.SetBromComError("Unable to create SQL transaction");
			return FALSE;
		}
	}

	if (FALSE == bAttemptedTransaction)
	{
		m_WorkingDlg.Create();
	}

	bool bResult = Tran.GetImportOKFlag();

	m_strBromComError = Tran.GetBromComError();
	m_strFilenamePendingOrders = Tran.GetFilenamePendingOrders();
	m_strFilenamePendingItems = Tran.GetFilenamePendingItems();
	m_nValidPaymentCount = Tran.GetValidPaymentCount();

	if (TRUE == bResult)
	{
		ProcessPendingOrders();
	}

	m_WorkingDlg.DestroyWindow();
	
	bResult = (m_strBromComError == "");

	// tidy up temp files
	CFileRecycler::SendToRecycleBin(m_strFilenamePendingItems);
	CFileRecycler::SendToRecycleBin(m_strFilenamePendingOrders);

	if (TRUE == m_bDisplayImportResults)
	{
		DisplayImportResults(m_nValidPaymentCount);

		// only log exceptions on importing New Payments
		if ((TRUE == m_bNewPayments) && (m_nNewExceptionCount > 0))
		{
			CString strFile = "";
			strFile.Format("%s.dat", 
				(const char*) m_strExceptionName);

			strFile = System.GetWebPaymentPath(strFile);
			::AppendDataFile(m_strNewTempExceptionFile, strFile);
		}
	}

	// delete session exceptions
	CFileRecycler::SendToRecycleBin(m_strNewTempExceptionFile);
	CFileRecycler::SendToRecycleBin(m_strTempLogFile);					// delete session import log
	CFileRecycler::SendToRecycleBin(m_strTempExceptionFile);			// delete session exceptions

	return bResult;
}

//*******************************************************************

void CBromComImport::ProcessPendingOrders()
{
	CWebPaymentLogRecord LogRecord;

	// "InYYYYMM"
	m_strLogFileName = LogRecord.GetLogFileName();
	m_strTempLogFile.Format("%s.$$$", 
		(const char*) m_strLogFileName);

	m_strTempLogFile = System.GetWebPaymentPath(m_strTempLogFile);

	// "ExYYYYMM"
	m_strExceptionName = LogRecord.GetExceptionFileName();
	m_strTempExceptionFile.Format("%s.$$$",
		(const char*) m_strExceptionName);
	m_strTempExceptionFile = System.GetWebPaymentPath(m_strTempExceptionFile);

	// "NewExYYYYMM"
	m_strNewExceptionName = LogRecord.GetNewExceptionFileName();
	m_strNewTempExceptionFile.Format("%s.$$$", 
		(const char*) m_strNewExceptionName);
	m_strNewTempExceptionFile = System.GetWebPaymentPath(m_strNewTempExceptionFile);

	CSQLRecordSetBromComPendingOrder RecordSetPendingOrder(NULL, RSParams_BromComPendingOrder_NormalNoParams{});
	CSQLRepositoryBromComPendingItem PrepStatPendingItem{};
	CSQLRepositoryBromComItem PrepStatItem{};

	CReportConsolidationArray<CSortedIntItem> arrayPendingOrdersToDelete;

	m_WorkingDlg.SetCaption1("Processing Pending BromCom Orders");

	int nProcessedSoFar = 0;

	CSQLRowBromComPendingOrder RowPendingOrder;
	while (RecordSetPendingOrder.StepSelectAll(RowPendingOrder) == TRUE)
	{
		m_WorkingDlg.SetCaption2RecordsChecked(++nProcessedSoFar, FALSE);

		CString strStatusPending = RowPendingOrder.GetStatus();
		bool bNewOrder = RowPendingOrder.GetNewOrderFlag();

		CReportConsolidationArray<CConsolidatedDoubleByInt> arrayItemDelta;
		GetItemDelta(RowPendingOrder, arrayItemDelta);

		bool bException = FALSE;

		for (int x = 0; x < arrayItemDelta.GetSize(); x++)
		{
			CConsolidatedDoubleByInt item;
			arrayItemDelta.GetAt(x, item);

			if ( (item.m_dVal < 0.001) && ( item.m_dVal > -0.001 ) )
			{
				continue;
			}

			LogRecord.SetPaymentID(RowPendingOrder.GetOrderID());
			LogRecord.SetPupilID(item.m_nKey);
			LogRecord.SetPaidAmount(item.m_dVal);

			{
				CString strSQLDate = RowPendingOrder.GetLastModified();

				if (strSQLDate.GetLength() >= 19)
				{
					CString strDate = "";
					strDate += strSQLDate.Mid(8, 2);
					strDate += "/";
					strDate += strSQLDate.Mid(5, 2);
					strDate += "/";
					strDate += strSQLDate.Left(4);
					strDate += " @ ";
					strDate += strSQLDate.Mid(11, 8);
					LogRecord.SetPaidDate(strDate);
				}
			}

			LogRecord.SetPupilName("");

			CString strPupilID = "";
			strPupilID.Format("%d", item.m_nKey);

			CSQLRepositoryAccount RepoAccount;
			__int64 nUserID = RepoAccount.LookupMemberID(strPupilID, NULL).GetSQLResult();
			
			if (nUserID != 0 )
			{
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserID(nUserID);

				if ( RepoAccount.SelectRow(RowAccount,NULL).GetSQLError() == SQLCRUD_ERR_NONE )
				{
					CSQLAuditRecord atc;
					atc.Set(&RowAccount);
					atc.SetOperatorID(Passwords.GetPasswordInUse());
					atc.SetApplicationNo(APPNO_BROMCOM);

					{
						CString strComment = "";

						strComment.Format("bcm: order %d %s",
							RowPendingOrder.GetOrderID(),
							(const char*)RowPendingOrder.GetStatus());

						atc.SetLineComment(strComment);
					}

					RowAccount.AddToPurse1Liability(item.m_dVal);
					RepoAccount.UpdateRow(RowAccount,NULL);
					
					if (atc.AuditChanges(&RowAccount) == TRUE)
					{
						atc.SaveNewLine();
					}

					CSQLRowBromComPendingItem RowPendingItem;
					RowPendingItem.SetOrderID(RowPendingOrder.GetOrderID());
					RowPendingItem.SetPersonID(item.m_nKey);
					PrepStatPendingItem.SelectRow(RowPendingItem,NULL);
					
					CSQLRowBromComItem RowItem;
					RowItem.SetOrderID(RowPendingItem.GetOrderID());
					RowItem.SetPersonID(RowPendingItem.GetPersonID());
					RowItem.SetAmount(RowPendingItem.GetAmount());
					RowItem.SetStatus(strStatusPending);

					PrepStatItem.UpsertRow(RowItem, NULL);

					//	pLogRecord->SetServiceID(strAccount);			
					//	pLogRecord->SetPrefix ( szText )					
					//	pLogRecord->SetPaidDate(strDateTime);
					//	pLogRecord->SetTransactionRef(strBatchID);
					//	pLogRecord->SetReceiptCode ( const char* szText );
					//	pLogRecord->SetDOB ( const char* szText );			
					//	pLogRecord->SetNotes ( const char* szText );

					LogRecord.SetPupilName(RowAccount.GetUsername());
					LogRecord.LogValidImport(m_strTempLogFile, RowAccount.GetUserIDString(), m_nValidPaymentCount);

					/*
					m_WebPaymentList.AddBlankRecord(RowItem.GetOrderID());
					m_WebPaymentList.SetServiceID(0);
					m_WebPaymentList.SetAmount(item.m_dVal);
					m_WebPaymentList.SetDate(LogRecord.GetDate());
					m_WebPaymentList.SetTime(LogRecord.GetTime());
					m_WebPaymentList.SetUserID(strUserID);
					m_WebPaymentList.SetName(m_pDbSmartPay->GetUserName());
					*/

					{
						CSQLRowBromComBalanceChanges RowBalance;
						RowBalance.SetPersonID(RowPendingItem.GetPersonID());
						RowBalance.SetAccountID(_atoi64(RowAccount.GetUserIDString()));

						CSQLRepositoryBromComBalanceChanges PrepStatBalance;
						if (PrepStatBalance.RowExists(RowPendingItem.GetPersonID(),NULL).GetSQLResult() == 0)
						{
							CSQLRowBromComBalanceChanges RowBalance;
							RowBalance.SetPersonID(RowPendingItem.GetPersonID());
							RowBalance.SetAccountID(RowAccount.GetUserID());
							PrepStatBalance.InsertRow(RowBalance,NULL);
						}
					}
				}
				else
				{
					bException = TRUE;
					LogException(&LogRecord, szMSG_NOACCOUNT, bNewOrder);
				}
			}
			else
			{
				bException = TRUE;
				LogException(&LogRecord, szMSG_NOMEMBER, bNewOrder);
			}
		}

		if (arrayItemDelta.GetSize() == 0)
		{
			CSortedIntItem item{};
			item.m_nItem = RowPendingOrder.GetOrderID();
			arrayPendingOrdersToDelete.Consolidate(item);
		}
	}

	m_WorkingDlg.SetCaption1("Removing Reconciled Items from Pending List");

	for (int n = 0; n < arrayPendingOrdersToDelete.GetSize(); n++)
	{
		m_WorkingDlg.SetCaption2RecordsOfTotal(n + 1, arrayPendingOrdersToDelete.GetSize());

		CSortedIntItem item{};
		arrayPendingOrdersToDelete.GetAt(n, item);
		int nOrderID = item.m_nItem;

		CSQLRepositoryBromComPendingOrder PrepStatOrder;
		CSQLRowBromComPendingOrder RowOrder;
		RowOrder.SetOrderID(nOrderID);
		
		PrepStatOrder.DeleteRow(RowOrder, NULL);
		
		CSQLRepositoryBromComPendingItem PrepStatItem;
		PrepStatItem.DeletePendingItemOrder(nOrderID, NULL);
	}

	m_bDisplayImportResults = TRUE;
}

//*******************************************************************

void CBromComImport::GetItemDelta( CSQLRowBromComPendingOrder& RowPendingOrder, CReportConsolidationArray<CConsolidatedDoubleByInt>& arrayItemDelta)
{
	arrayItemDelta.RemoveAll();

	bool bPendingPaid = FALSE;
	{
		CString strStatus = RowPendingOrder.GetStatus();
		strStatus.MakeUpper();
		bPendingPaid = (strStatus == "PAID");
	}

	//GET PENDING ORDER ITEMS
	CReportConsolidationArray<CConsolidatedDoubleByInt> arrayPendingItems;
	{
		CSQLRecordSetBromComPendingItem RecordSet(NULL,
			RSParams_BromComPendingItem_NormalByOrderId{ RowPendingOrder.GetOrderID() });

		CSQLRowBromComPendingItem RowItem;
		while (RecordSet.StepSelectAll(RowItem) == TRUE)
		{
			CConsolidatedDoubleByInt item;
			item.m_nKey = RowItem.GetPersonID();
			item.m_dVal = RowItem.GetAmount();
			arrayPendingItems.Consolidate(item);
		}
		RecordSet.Close();
	}

	//GET EXISTING ORDER ITEMS
	CReportConsolidationArray<CConsolidatedDoubleAndStringByInt> arrayCurrentItems;
	{
		CSQLRecordSetBromComItem RecordSet2(NULL, RSParams_BromComItem_NormalByOrderId{ RowPendingOrder.GetOrderID() });

		CSQLRowBromComItem RowItem;
		while (RecordSet2.StepSelectAll(RowItem) == TRUE)
		{
			CConsolidatedDoubleAndStringByInt item;
			item.m_nKey = RowItem.GetPersonID();
			item.m_dVal = RowItem.GetAmount();
			item.m_strVal = RowItem.GetStatus();
			arrayCurrentItems.Consolidate(item);
		}
	}

	if (TRUE == bPendingPaid)
	{
		for (int n = 0; n < arrayPendingItems.GetSize(); n++)
		{
			CConsolidatedDoubleByInt itemPending{};
			arrayPendingItems.GetAt(n, itemPending);

			CConsolidatedDoubleAndStringByInt itemCurrent{};
			itemCurrent.m_nKey = itemPending.m_nKey;

			int nPos;
			if (arrayCurrentItems.Find(itemCurrent, nPos) == FALSE)
			{
				arrayItemDelta.Consolidate(itemPending);
			}
			else
			{
				arrayCurrentItems.GetAt(nPos, itemCurrent);
				CString strStatus = itemCurrent.m_strVal;
				strStatus.MakeUpper();

				if (strStatus != "PAID")
				{
					arrayItemDelta.Consolidate(itemPending);
				}
				else if (itemPending.m_dVal != itemCurrent.m_dVal)
				{
					itemPending.m_dVal -= itemCurrent.m_dVal;
					arrayItemDelta.Consolidate(itemPending);
				}
			}
		}

		//ADJUST FOR ANY ITEMS THAT HAVE BEEN REMOVED
		for (int n = 0; n < arrayCurrentItems.GetSize(); n++)
		{
			CConsolidatedDoubleAndStringByInt itemCurrent{};
			arrayCurrentItems.GetAt(n, itemCurrent);

			CConsolidatedDoubleByInt itemPending{};
			itemPending.m_nKey = itemCurrent.m_nKey;

			int nPos;
			if (arrayPendingItems.Find(itemPending, nPos) == FALSE)
			{
				CString strStatus = itemCurrent.m_strVal;
				strStatus.MakeUpper();

				if (strStatus == "PAID")
				{
					itemPending.m_dVal = itemCurrent.m_dVal * (-1);
					arrayItemDelta.Consolidate(itemPending);
				}
			}
		}
	}
	else
	{
		//NEW ORDER STATUS IS NOT PAID, SO REVERSE ALL CURRENT PAID ITEMS
		for (int n = 0; n < arrayCurrentItems.GetSize(); n++)
		{
			CConsolidatedDoubleAndStringByInt itemCurrent{};
			arrayCurrentItems.GetAt(n, itemCurrent);

			CString strStatus = itemCurrent.m_strVal;
			strStatus.MakeUpper();

			if (strStatus == "PAID")
			{
				CConsolidatedDoubleByInt itemPending{};
				itemPending.m_nKey = itemCurrent.m_nKey;
				itemPending.m_dVal = itemCurrent.m_dVal * (-1);
				arrayItemDelta.Consolidate(itemPending);
			}
		}
	}
}

//*******************************************************************

void CBromComImport::LogException(CWebPaymentLogRecord* pLogRecord, const char* szErrorText, bool bNewOrder)
{
	pLogRecord->LogException(m_strTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nExceptionCount);

	if ( (TRUE == bNewOrder) && ( TRUE == m_bNewPayments ) )
	{
		pLogRecord->LogException(m_strNewTempExceptionFile, m_strExceptionIndicator, szErrorText, m_nNewExceptionCount);
	}
}

//*******************************************************************
