//******************************************************************************
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\TLogFile.h"
//******************************************************************************
#include "WebPaymentAtcTransactionFile.h"
//******************************************************************************

//static const char* szHEADER_V1 = "Acc,Nme,Gno,Dat,Tim,Act,Typ,App,Tno,Cno,Txt,PtA,PtB,P1A,P2A,CA,P1B,P2B,Nts";
  static const char* szHEADER_V2 = "Acc,Nme,Gno,Dat,Tim,Act,Typ,App,Tno,Cno,Txt,PtA,PtB,P1A,P2A,P3A,CA,P1B,P2B,P3B,Nts";

  static int nRETRIES = 10;

//******************************************************************************

CWebPaymentAtcTransactionFile::CWebPaymentAtcTransactionFile()
{
	m_nActionType = nATCWEB_SALE;
}

//******************************************************************************

int CWebPaymentAtcTransactionFile::Open ( const char* szFilename )
{
	CreateSSDatabase ( szFilename, "", szHEADER_V2, nRETRIES );						// create db if not found, 10 retries
	Close();

	int nReply = OpenSSDatabaseReadWrite ( szFilename, "", nRETRIES );

	int nIndex = GetHeaderIndex ( "P3A" );									// check for latest header
	if ( nIndex == -1 )														// not found
	{
		Close();																// close file
		nReply = OpenSSDatabaseReadWriteNewHeader ( szFilename, "", szHEADER_V2, nRETRIES );	// read and write and modify header (file must exist)
	}

	return nReply;
}	

//******************************************************************************

bool CWebPaymentAtcTransactionFile::SaveWebPaymentAtcTransactionLine(CSQLAuditRecord* pAtcRecord, const char* szText, bool bUseFiledTime)
{
	CSSDate date = (bUseFiledTime == TRUE) ? pAtcRecord->m_dateFiled : pAtcRecord->m_dateLastUsed;
	if (date.IsSet() == FALSE)
	{
		return FALSE;
	}

	CSSTime time = (bUseFiledTime == TRUE) ? pAtcRecord->m_timeFiled : pAtcRecord->m_timeLastUsed;
	if (time.IsSet() == FALSE)
	{
		return FALSE;
	}

	int nRecordIndex = AddBlankRecord();		// add to unsorted database - returns index position of new record

	Set(AtcWebUpdate::CardNo.Label, pAtcRecord->GetUserID());
	Set(AtcWebUpdate::CardName.Label, pAtcRecord->GetUserName());
	Set(AtcWebUpdate::GroupNo.Label, pAtcRecord->GetGroupNo());
	Set(AtcWebUpdate::Date.Label, date.GetDate());
	Set(AtcWebUpdate::Time.Label, time.GetTime());
	Set(AtcWebUpdate::Act.Label, m_nActionType);
	Set(AtcWebUpdate::Type.Label, pAtcRecord->GetSourceType());
	Set(AtcWebUpdate::AppNo.Label, pAtcRecord->GetApplicationNo());
	Set(AtcWebUpdate::TerminalNo.Label, pAtcRecord->GetTerminalNo());
	Set(AtcWebUpdate::CCNo.Label, pAtcRecord->GetCCNoFull());
	Set(AtcWebUpdate::Text.Label, szText);									// description
	Set(AtcWebUpdate::PointsAdded.Label, pAtcRecord->GetTransactionPoints());
	Set(AtcWebUpdate::PointsBalance.Label, pAtcRecord->GetCurrentPoints());
	Set(AtcWebUpdate::Purse1Value.Label, pAtcRecord->GetTransactionPurse1Total(), 2);	// cash added ( -ve = sale )
	Set(AtcWebUpdate::Purse2Value.Label, pAtcRecord->GetTransactionPurse2Balance(), 2);
	Set(AtcWebUpdate::Purse3Value.Label, pAtcRecord->GetTransactionPurse3Total(), 2);
	Set(AtcWebUpdate::CashValue.Label, pAtcRecord->GetTransactionCash(), 2);
	Set(AtcWebUpdate::Purse1Balance.Label, pAtcRecord->GetCurrentPurse1Total(), 2);				// purse balance after transaction
	Set(AtcWebUpdate::Purse2Balance.Label, pAtcRecord->GetCurrentPurse2Balance(), 2);
	Set(AtcWebUpdate::Purse3Balance.Label, pAtcRecord->GetCurrentPurse3Total(), 2);
	Set(AtcWebUpdate::Notes.Label, pAtcRecord->GetWebPaymentID());

	return (nRecordIndex >= 0);
}

//******************************************************************************

CString CWebPaymentAtcTransactionFile::GetXmlDate(int nWebPaymentType)
{
	CSSDate date(GetDate());
	CSSTime time(GetTime());

	CString strXmlDate = "";

	switch (nWebPaymentType)
	{
	case nWEBPAYMENT_TUCASI:
	case nWEBPAYMENT_TUCASIv2:
		strXmlDate.Format("%4.4d-%2.2d-%2.2d %2.2d:%2.2d:00:000", date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute());
		break;

	default:
		strXmlDate = ::FormatXmlDate(&date, &time);
		break;
	}

	return strXmlDate;
}

//******************************************************************************

double CWebPaymentAtcTransactionFile::GetTransactionValue()
{
	double dSalesValue = GetPurse1Value();
	dSalesValue += GetPurse2Value();
	dSalesValue += GetPurse3Value();
	dSalesValue += GetCashValue();

	return dSalesValue;
}

//******************************************************************************

double CWebPaymentAtcTransactionFile::GetTransactionValue ( int nPurseNo )
{
	double dSalesValue = 0;
		
	switch ( nPurseNo )
	{
	case 1:				
		dSalesValue = GetPurse1Value();	
		break;

	case 2:				
		dSalesValue = GetPurse2Value();	
		break;

	case 3:			
		dSalesValue = GetPurse3Value();	
		break;
	}

	return dSalesValue;
}

//******************************************************************************

CString CWebPaymentAtcTransactionFile::GetWebPaymentIDString()
{
	CString strID = GetNotes();
	return strID;
}

//******************************************************************************
// Work thru atc updates & add to web atc file of purchases * topups
//******************************************************************************

bool CWebPaymentAtcTransactionFile::AddNewAtcUpdates(const char* szAtcUpdates)
{
	m_TLogCache.Reset();
	bool bReply = AddNewAtcUpdatesInternal(szAtcUpdates);
	m_TLogCache.Reset();
	return bReply;
}

//******************************************************************************

bool CWebPaymentAtcTransactionFile::AddNewAtcUpdatesInternal(const char* szAtcUpdates)
{
	bool bReply = TRUE;

	if (::FileExists(szAtcUpdates) == TRUE)						// check for new audit updates pending
	{
		CSmartPayAuditFile auditFile(szAtcUpdates);					// ATC updates
		if ((bReply = auditFile.Open()) == TRUE)
		{
			CSQLAuditRecord atcRecord;
			while (auditFile.ReadLine(&atcRecord) == TRUE)
			{
				if (CheckTopup(&atcRecord) != 0)					// may be opening balance ( there will be no purse\cash transaction values )
				{
					continue;
				}

				if (atcRecord.GetTransactionPurse1Total() == 0 && atcRecord.GetTransactionPurse2Balance() == 0 && atcRecord.GetTransactionPurse3Total() == 0 && atcRecord.GetTransactionCash() == 0)
				{
					continue;
				}

				if (CheckSpend(&atcRecord) != 0)
				{
					continue;
				}

				CheckAdjustment(&atcRecord);
			}
			auditFile.Close();
		}
	}

	return bReply;
}

//******************************************************************************
// nReply -1=file error, 0=not found, 1=found

int CWebPaymentAtcTransactionFile::CheckSpend(CSQLAuditRecord* pAtcRecord)
{
	CString strText = "";
	bool bValid = FALSE;
	bool bUseFiledTime = FALSE;
	int nAppNo = pAtcRecord->GetApplicationNo();

	switch (pAtcRecord->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_EXT:
		if (nAppNo == APPNO_SALE || nAppNo == APPNO_REFUNDSALE)
		{
			GetPurchaseText(pAtcRecord, strText);
			bValid = TRUE;										// or 'Ecr sale' if purchase not found 
		}
		break;

	case AUDIT_PC:
		if (nAppNo == APPNO_MEAL)
		{
			bUseFiledTime = TRUE;								// don't use Date \ Time last used
			strText = pAtcRecord->GetLineOrDefaultComment();	// "Office meal".
			bValid = TRUE;
		}
		break;

	case AUDIT_VENDING:
		if (nAppNo == APPNO_SALE || nAppNo == APPNO_REFUNDSALE)
		{
			strText = pAtcRecord->GetLineOrDefaultComment();	// "Vending".
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

	return (SaveWebPaymentAtcTransactionLine(pAtcRecord, strText, bUseFiledTime) == TRUE) ? 1 : -1;
}

//******************************************************************************
// nReply -1=file error, 0=not found, 1=found

int CWebPaymentAtcTransactionFile::CheckTopup(CSQLAuditRecord* pAtcRecord)
{
	bool bValid = FALSE;
	int nAppNo = pAtcRecord->GetApplicationNo();
	bool bUseFiledTime = FALSE;

	CString strText = "";
	CString strTopupText = " topup";

	m_nActionType = nATCWEB_TOPUP;

	switch (pAtcRecord->GetSourceType())
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
			
		//case APPNO_BUCKINGHAM:	
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

	double dValue = pAtcRecord->GetTransactionPurse1Total() + pAtcRecord->GetTransactionPurse2Balance() + pAtcRecord->GetTransactionPurse3Total() + pAtcRecord->GetTransactionCash();
	if (dValue < 0)											// see if cash was taken off on an ADD CASH function - then its a Refund
	{
		if (m_nActionType == nATCWEB_TOPUP)					// action may already have been set
		{
			strText = "Deduct " + strText;						// add 'Refund' text to description
			m_nActionType = nATCWEB_TOPUPREFUND;				// topup refund
		}
	}

	return (SaveWebPaymentAtcTransactionLine(pAtcRecord, strText, bUseFiledTime) == TRUE) ? 1 : -1;
}

//******************************************************************************
// nReply -1=file error, 0=not found, 1=found

int CWebPaymentAtcTransactionFile::CheckAdjustment ( CSQLAuditRecord* pAtcRecord  )
{
	// not a cash added - but no error - so ok
	int nResult = 0;

	// see if database record changed manually - ie not via add cash button
	if (pAtcRecord->GetApplicationNo() == APPNO_EDIT)
	{
		//topup refund
		m_nActionType = nATCWEB_TOPUPREFUND;
		nResult = (SaveWebPaymentAtcTransactionLine(pAtcRecord, "Office adjustment", TRUE) == TRUE) ? 1 : -1;											 
	}

	return nResult;
}

//******************************************************************************

void CWebPaymentAtcTransactionFile::GetPurchaseText(CSQLAuditRecord* pAtcRecord, CString& strPurchaseText)
{
	strPurchaseText = "";

	bool bNewTLogDatabase = FALSE;

	CTLogDatabase* pDatabase = NULL;
	__int64 nUserID = _atoi64(pAtcRecord->GetCardNo());

	CSortedIntByInt64 item;
	item.m_nKey = nUserID;

	int nPos = 0;
	if ( m_TLogCache.FindMapEntry(item,nPos) == TRUE)
	{
		m_TLogCache.GetMapEntry(nPos, item);
		pDatabase = m_TLogCache.GetTLogDatabase(item.m_nVal);
	}
	else
	{
		item.m_nVal = m_TLogCache.GetTLogDatabaseCount();
		m_TLogCache.InsertMapEntry(nPos,item)
			;
		pDatabase = new (CTLogDatabase);
		m_TLogCache.AddTLogDatabase(pDatabase);

		bNewTLogDatabase = TRUE;
	}

	ExtractPurchaseText(pAtcRecord, pDatabase, bNewTLogDatabase, strPurchaseText);
}

//$$******************************************************************

void CWebPaymentAtcTransactionFile::ExtractPurchaseText(CSQLAuditRecord* pAtcRecord, CTLogDatabase* pDatabase, bool bNewDatabase, CString& strPurchaseText)
{
	strPurchaseText = "Ecr sale";

	if (pDatabase != NULL)
	{
		if (TRUE == bNewDatabase)
		{
			pDatabase->Open(GetHistoryFilename(pAtcRecord -> GetUserID()));
		}

		if (pDatabase->FindSalesText(pAtcRecord ->GetDateLastUsed(), pAtcRecord ->GetTimeLastUsed(), pAtcRecord -> GetCCNoFull()) == TRUE)
		{
			strPurchaseText = pDatabase->GetSalesText();			// extract purchase text
		}
	}
}

//**********************************************************************

CString CWebPaymentAtcTransactionFile::GetHistoryFilename(CString strUserID)
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
