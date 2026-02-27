/**********************************************************************/
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\TLogFile.h"
/**********************************************************************/
#include "WebPaymentAtcTransactionFile.h"
/**********************************************************************/
#include "SQLTranAuditPendingExport.h"
/**********************************************************************/

CSQLTranAuditPendingExport::CSQLTranAuditPendingExport(CString strAuditFilename) : CSQLTranBase()
{
	m_strAuditFilename = strAuditFilename;
	m_strError = "";
	m_nActionType = 0;
}

/**********************************************************************/

void CSQLTranAuditPendingExport::DoWork()
{
	m_TLogCache.Reset();
	DoWorkInternal();
	m_TLogCache.Reset();
}

/**********************************************************************/

void CSQLTranAuditPendingExport::DoWorkInternal()
{
	if (SQLTRAN_STATE_ACTIVE == m_nState)
	{
		m_bCanCommit = FALSE;
		m_nState = SQLTRAN_STATE_WAITEND;

		if (::FileExists(m_strAuditFilename) == FALSE)
		{
			m_bCanCommit = TRUE;
			return;
		}

		CSmartPayAuditFile auditFile(m_strAuditFilename);
		if (auditFile.Open() == FALSE)
		{
			m_strError = "Unable to open audit file";
			return;
		}

		CSQLAuditRecord atcRecord;
		while (auditFile.ReadLine(&atcRecord) == TRUE)
		{
			bool bGotLine = FALSE;

			// may be opening balance ( there will be no purse\cash transaction values )
			{
				int nResult = CheckTopUp(atcRecord);

				if (nResult == -1)
				{
					m_strError = "Error processing top-up line";
					return;
				}
				else if (nResult != 0)
				{
					bGotLine = TRUE;
				}
			}

			if (FALSE == bGotLine)
			{
				if ((atcRecord.GetTransactionPurse1Total() == 0) &&
					(atcRecord.GetTransactionPurse2Balance() == 0) &&
					atcRecord.GetTransactionPurse3Total() == 0 &&
					atcRecord.GetTransactionCash() == 0)
				{
					continue;
				}
			}

			if ( FALSE == bGotLine )
			{
				int nResult = CheckSpend(atcRecord);

				if (nResult == -1)
				{
					m_strError = "Error processing spend line";
					return;
				}
				else if (nResult != 0)
				{
					bGotLine = TRUE;
				}
			}

			if ( FALSE == bGotLine )
			{
				int nResult = CheckAdjustment(atcRecord);

				if (nResult == -1)
				{
					m_strError = "Error processing adjustment line";
					return;
				}
				else if (nResult != 0)
				{
					bGotLine = TRUE;
				}
			}

			if (FALSE == bGotLine)
			{
				if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
				{
					int nResult = CheckPebbleOther(atcRecord);

					if (nResult == -1)
					{
						m_strError = "Error processing Pebble adjustment line";
						return;
					}
					else if (nResult != 0)
					{
						bGotLine = TRUE;
					}
				}
			}

			if (TRUE == bGotLine)
			{
				switch (System.GetWebPaymentType())
				{
				case nWEBPAYMENT_PEBBLE:
					CreatePendingPebbleLine();
					break;

				case nWEBPAYMENT_BROMCOM:
					CreatePendingBromComLine();
					break;
				}
			}

			if (m_strError != "")
			{
				return;
			}
		}
		auditFile.Close();
	}

	m_bCanCommit = TRUE;
}


/**********************************************************************/

int CSQLTranAuditPendingExport::CheckTopUp(CSQLAuditRecord& AuditRecord)
{
	bool bValid = FALSE;
	int nAppNo = AuditRecord.GetApplicationNo();
	bool bUseFiledTime = FALSE;

	CString strText = "";
	CString strTopupText = " topup";

	m_nActionType = nATCWEB_TOPUP;

	switch (AuditRecord.GetSourceType())
	{
	case AUDIT_SERVER:
		switch (nAppNo)
		{
		case APPNO_REFRESH:
			strText = "Refresh";
			strTopupText = "";
			m_nActionType = nATCWEB_REFRESH;
			bValid = TRUE;
			break;

		case APPNO_REDEEM:
			strText = "Redeem Pts";
			strTopupText = "";
			bValid = TRUE;
			break;
		}
		break;

	case AUDIT_PC:
		// don't use Date \ Time last used
		bUseFiledTime = TRUE;

		switch (nAppNo)
		{
		case APPNO_REFRESH:
			strText = "Office Refresh";
			strTopupText = "";
			m_nActionType = nATCWEB_REFRESH;
			bValid = TRUE;
			break;

		case APPNO_REDEEM:
		case APPNO_BATCHREDEEM:
			strText = "Redeem Pts";
			strTopupText = "";
			bValid = TRUE;
			break;

		case APPNO_ADDCASH:
		case APPNO_BATCHUPDATE:
		case APPNO_BATCHDELETE:					// <== is this required ?????
		case APPNO_IMPORTTOPUP:
		case APPNO_REVALUEBONUSCASH:
		case APPNO_REWARDBONUSCASH:
		case APPNO_REFUNDREWARDBONUSCASH:
			strText = "Office";
			bValid = TRUE;
			break;

			//14/07/2016 - v1.12f
		case APPNO_XFEROUT:
		case APPNO_HOTXFEROUT:
			strText = "Office transfer (-)";
			strTopupText = "";
			m_nActionType = nATCWEB_TRANSFER;
			bValid = TRUE;
			break;

			//14/07/2016 - v1.12f
		case APPNO_XFERIN:
		case APPNO_HOTXFERIN:
			strText = "Office transfer (+)";
			strTopupText = "";
			m_nActionType = nATCWEB_TRANSFER;
			bValid = TRUE;
			break;

			// Tucasi(Json) not sent back
			// ParentPay already echoed back - so don't send again
		case APPNO_PARENTPAY:
		case APPNO_PEBBLE_TOPUP:
		case APPNO_PEBBLE_TOPUP_REFUND:
		case APPNO_PEBBLE_ORDER:
		case APPNO_PEBBLE_ORDER_REFUND:
		case APPNO_TUCASIv2:
			break;

		case APPNO_PARENTMAIL:
		case APPNO_SQUID:
		case APPNO_SCHOOLCOMMS:
		case APPNO_BROMCOM:
		case APPNO_WISEPAY:
			strText = "Web";
			bValid = TRUE;
			break;

		case APPNO_TUCASI:
			strText = "Deposit Received";
			strTopupText = "";
			bValid = TRUE;
			break;

			// WebPayment opening balance
		case APPNO_WPOPENINGBALANCE:
			strText = "Opening balance";
			strTopupText = "";
			m_nActionType = nATCWEB_STARTINGBALANCE;
			bValid = TRUE;
			break;

		default:
			break;
		}
		break;

	case AUDIT_KIOSKLINK:
		if (nAppNo == APPNO_ADDCASH)
		{
			strText = "Kiosk";
			bValid = TRUE;
		}
		break;

	case AUDIT_POS:
		if (nAppNo == APPNO_ADDCASH)
		{
			strText = "Ecr";
			bValid = TRUE;
		}
		break;

	case AUDIT_EXT:
		if (nAppNo == APPNO_ADDCASH)
		{
			strText = "Ext";
			bValid = TRUE;
		}
		break;

	case AUDIT_VENDING:
	case AUDIT_EODBALANCE:
		break;
	}

	// not a cash added action - but no error - so ok 
	if (bValid == FALSE)
	{
		return 0;
	}

	strText += strTopupText;

	double dValue = 0.0;
	dValue += AuditRecord.GetTransactionPurse1Total();
	dValue += AuditRecord.GetTransactionPurse2Balance();
	dValue += AuditRecord.GetTransactionPurse3Total();
	dValue += AuditRecord.GetTransactionCash();

	if (dValue < 0.0)										// see if cash was taken off on an ADD CASH function - then its a Refund
	{
		if (m_nActionType == nATCWEB_TOPUP)					// action may already have been set
		{
			strText = "Deduct " + strText;					// add 'Refund' text to description
			m_nActionType = nATCWEB_TOPUPREFUND;			// topup refund
		}
	}

	return (SaveTransactionLine(AuditRecord, strText, bUseFiledTime) == TRUE) ? 1 : -1;
}

/**********************************************************************/

int CSQLTranAuditPendingExport::CheckSpend(CSQLAuditRecord& AuditRecord)
{
	CString strText = "";
	bool bValid = FALSE;
	bool bUseFiledTime = FALSE;
	int nAppNo = AuditRecord.GetApplicationNo();

	switch (AuditRecord.GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		if (nAppNo == APPNO_SALE || nAppNo == APPNO_REFUNDSALE)
		{
			GetPurchaseText(AuditRecord, strText);
			bValid = TRUE;										// or 'Ecr sale' if purchase not found 
		}
		break;

	case AUDIT_PC:
		if (nAppNo == APPNO_MEAL)
		{
			bUseFiledTime = TRUE;								// don't use Date \ Time last used
			strText = AuditRecord.GetLineOrDefaultComment();	// "Office meal".
			bValid = TRUE;
		}
		break;

	case AUDIT_VENDING:
		if (nAppNo == APPNO_SALE || nAppNo == APPNO_REFUNDSALE)
		{
			strText = AuditRecord.GetLineOrDefaultComment();	// "Vending".
			bValid = TRUE;										// or 'Ecr sale' if purchase not found 
		}
		break;

	case AUDIT_SERVER:
	case AUDIT_KIOSKLINK:
	case AUDIT_EODBALANCE:
	default:
		break;
	}

	if (bValid == FALSE)								// not a spend - but no error - so ok 
	{
		return 0;
	}

	if (nAppNo == APPNO_REFUNDSALE)
	{
		strText = "Refund " + strText;					// add 'refund' text to description
		m_nActionType = nATCWEB_SALEREFUND;				// sale refund
	}
	else
	{
		m_nActionType = nATCWEB_SALE;
	}

	return (SaveTransactionLine(AuditRecord, strText, bUseFiledTime) == TRUE) ? 1 : -1;
}

/**********************************************************************/

int CSQLTranAuditPendingExport::CheckAdjustment(CSQLAuditRecord& AuditRecord)
{
	// not a cash added - but no error - so ok
	int nResult = 0;

	// see if database record changed manually - ie not via add cash button
	if (AuditRecord.GetApplicationNo() == APPNO_EDIT)
	{
		//topup refund
		m_nActionType = nATCWEB_TOPUPREFUND;
		nResult = (SaveTransactionLine(AuditRecord, "Office adjustment", TRUE) == TRUE) ? 1 : -1;
	}

	return nResult;
}

/**********************************************************************/

int CSQLTranAuditPendingExport::CheckPebbleOther(CSQLAuditRecord& AuditRecord)
{
	int nResult = 0;

	if ( AuditRecord.GetTransactionPurse1Total() != 0.0 )
	{ 
		m_nActionType = nATCWEB_PEBBLEOTHER;
		nResult = (SaveTransactionLine(AuditRecord, "Pebble Other", TRUE) == TRUE) ? 1 : -1;
	}

	return nResult;
}

/**********************************************************************/

void CSQLTranAuditPendingExport::GetPurchaseText(CSQLAuditRecord& AuditRecord, CString& strPurchaseText)
{
	strPurchaseText = "";

	bool bNewTLogDatabase = FALSE;

	CTLogDatabase* pDatabase = NULL;
	__int64 nUserID = _atoi64(AuditRecord.GetCardNo());

	CSortedIntByInt64 item;
	item.m_nKey = nUserID;

	int nPos = 0;
	if (m_TLogCache.FindMapEntry(item, nPos) == TRUE)
	{
		m_TLogCache.GetMapEntry(nPos, item);
		pDatabase = m_TLogCache.GetTLogDatabase(item.m_nVal);
	}
	else
	{
		item.m_nVal = m_TLogCache.GetTLogDatabaseCount();
		m_TLogCache.InsertMapEntry(nPos, item);

		pDatabase = new (CTLogDatabase);
		m_TLogCache.AddTLogDatabase(pDatabase);

		bNewTLogDatabase = TRUE;
	}

	ExtractPurchaseText(AuditRecord, pDatabase, bNewTLogDatabase, strPurchaseText);
}

//$$******************************************************************

void CSQLTranAuditPendingExport::ExtractPurchaseText(CSQLAuditRecord& AuditRecord, CTLogDatabase* pDatabase, bool bNewDatabase, CString& strPurchaseText)
{
	strPurchaseText = "Ecr sale";

	if (pDatabase != NULL)
	{
		if (TRUE == bNewDatabase)
		{
			pDatabase->Open(GetHistoryFilename(AuditRecord.GetUserID()));
		}

		if (pDatabase->FindSalesText(AuditRecord.GetDateLastUsed(), AuditRecord.GetTimeLastUsed(), AuditRecord.GetCCNoFull()) == TRUE)
		{
			strPurchaseText = pDatabase->GetSalesText();			// extract purchase text
		}
	}
}

//**********************************************************************

CString CSQLTranAuditPendingExport::GetHistoryFilename(CString strUserID)
{
	CString strFilename = "";

	// see if report is for the cash account "000000.dat"
	if (strUserID == "" || strUserID == System.GetBlankCardNo())			// see if report is for the cash account "000000.dat"
	{
		CString strTmp = Filenames.GetTempCashHistoryFilename();			// see if have a temp CASH tlog in use instead of actual cash history file
		if (::FileExists(strTmp) == TRUE)
		{
			// use temp file in favour of real history file ( outstanding updates already merged in 
			strFilename = strTmp;
		}
	}

	if (strFilename == "")												// no file name been set, so use the audit userid
	{
		CTLogFile tlogFile(strUserID);									// will merge in any outstanding updates
		strFilename = Filenames.GetPurchaseHistoryFilename(strUserID);
	}

	return strFilename;
}

//**********************************************************************

bool CSQLTranAuditPendingExport::SaveTransactionLine(CSQLAuditRecord& AuditRecord, CString strText, bool bUseFiledTime)
{
	CSSDate date = (bUseFiledTime) ? AuditRecord.m_dateFiled : AuditRecord.m_dateLastUsed;
	
	if (date.IsSet() == FALSE)
	{
		return FALSE;
	}

	CSSTime time = (bUseFiledTime) ? AuditRecord.m_timeFiled : AuditRecord.m_timeLastUsed;
	if (time.IsSet() == FALSE)
	{
		return FALSE;
	}

	m_RowPending.Reset();
	m_RowPending.SetUserID(AuditRecord.GetUserID());
	m_RowPending.SetXFerUserID(AuditRecord.GetXFerUserID());	
	m_RowPending.SetUserName(AuditRecord.GetUserName());
	m_RowPending.SetGroupNo(AuditRecord.GetGroupNo());
	m_RowPending.SetDate(date.GetDate());
	m_RowPending.SetTime(time.GetTime());
	m_RowPending.SetActionType(m_nActionType);
	m_RowPending.SetSourceType(AuditRecord.GetSourceType());
	m_RowPending.SetAppNo(AuditRecord.GetApplicationNo());
	m_RowPending.SetTerminalNo(AuditRecord.GetTerminalNo());
	m_RowPending.SetCCNo(AuditRecord.GetCCNoFull());
	m_RowPending.SetText(strText);
	m_RowPending.SetPointsAdded(AuditRecord.GetTransactionPoints());
	m_RowPending.SetPointsBalance(AuditRecord.GetCurrentPoints());
	m_RowPending.SetPurse1Value(AuditRecord.GetTransactionPurse1Total());
	m_RowPending.SetPurse2Value(AuditRecord.GetTransactionPurse2Balance());
	m_RowPending.SetPurse3Value(AuditRecord.GetTransactionPurse3Total());
	m_RowPending.SetCashValue(AuditRecord.GetTransactionCash());
	m_RowPending.SetPurse1Balance(AuditRecord.GetCurrentPurse1Total());
	m_RowPending.SetPurse2Balance(AuditRecord.GetCurrentPurse2Balance());
	m_RowPending.SetPurse3Balance(AuditRecord.GetCurrentPurse3Total());
	m_RowPending.SetNotes(AuditRecord.GetWebPaymentID());

	return TRUE;
}

//******************************************************************************

void CSQLTranAuditPendingExport::CreatePendingPebbleLine()
{
	m_PebbleHelper.CreatePebbleLine(m_RowPending, m_pDatabase);
	m_strError = m_PebbleHelper.GetError();
}

//******************************************************************************

void CSQLTranAuditPendingExport::CreatePendingBromComLine()
{
	m_BromComHelper.CreateBromComLine(m_RowPending, m_pDatabase);
	m_strError = m_BromComHelper.GetError();
}

//******************************************************************************

void CSQLTranAuditPendingExport::AfterTransaction()
{
	switch (System.GetWebPaymentType())
	{
	case nWEBPAYMENT_PEBBLE:
		break;

	case nWEBPAYMENT_BROMCOM:
		m_BromComHelper.FinaliseLog();
		break;
	}

	CFileRecycler::SendToRecycleBin(m_strAuditFilename);
}

//******************************************************************************
