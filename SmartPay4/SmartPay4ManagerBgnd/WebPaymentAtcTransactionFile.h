#pragma once
//*******************************************************************
#include "ATCWebDefines.h"
#include "TLogCache.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRowBromComTransaction.h"
//*******************************************************************

namespace AtcWebUpdate
{
const recString		CardNo			= { "Acc",	 0, 0 };
const recString		CardName		= { "Nme",	 0, 0 };
const recInt		GroupNo			= { "Gno",	 0, 0 };
const recString		Date			= { "Dat",	 0, 0 };
const recString		Time			= { "Tim",	 0, 0 };
const recInt		Act				= { "Act",	 0, 0 };		// action, see nATCWEB_xxx above
const recInt		Type			= { "Typ",	 0, 0 };		// audit source type
const recInt		AppNo			= { "App",	 0, 0 };		// audit AppNo
const recInt		TerminalNo		= { "Tno",	 0, 0 };
const recInt		CCNo			= { "Cno",	 0, 0 };
const recString		Text			= { "Txt",	 0, 0 };
const recLong		PointsAdded		= { "PtA",	 0, 0 };
const recLong		PointsBalance	= { "PtB",	 0, 0 };
const recDouble		Purse1Value		= { "P1A",	 0, 0 };
const recDouble		Purse2Value		= { "P2A",	 0, 0 };
const recDouble		Purse3Value		= { "P3A",	 0, 0 };		// v2
const recDouble		CashValue		= { "CA",	 0, 0 };
const recDouble		Purse1Balance	= { "P1B",	 0, 0 };
const recDouble		Purse2Balance	= { "P2B",	 0, 0 };
const recDouble		Purse3Balance	= { "P3B",	 0, 0 };		// v2
const recString		Notes			= { "Nts",	 0, 0 };		// webpaymentID
};

//*******************************************************************

class CWebPaymentAtcTransactionFile : public CSSUnsortedDatabase
{
public:
	CWebPaymentAtcTransactionFile();
	int Open ( const char* szFilename );
	bool AddNewAtcUpdates ( const char* szAtcSourceFile );

	const char* GetCardNo()		{ return GetString ( AtcWebUpdate::CardNo.Label ); }
	const char* GetCardName()	{ return GetString ( AtcWebUpdate::CardName.Label ); }
	int GetGroupNo()			{ return GetInt	   ( AtcWebUpdate::GroupNo.Label ); }
	int GetActionType()			{ return GetInt    ( AtcWebUpdate::Act.Label ); }		
	int GetFullCCNo()			{ return GetInt    ( AtcWebUpdate::CCNo.Label ); }
	int GetShortCCNo()			{ return GetInt	   ( AtcWebUpdate::CCNo.Label) % 10000; }
	int GetTerminalNo()			{ return GetInt    ( AtcWebUpdate::TerminalNo.Label ); }		
	int GetApplicationNo()		{ return GetInt    ( AtcWebUpdate::AppNo.Label ); }		
	int GetSourceType()			{ return GetInt    ( AtcWebUpdate::Type.Label ); }		
	const char* GetSalesText()	{ return GetString ( AtcWebUpdate::Text.Label ); }
	const char* GetDate()		{ return GetString ( AtcWebUpdate::Date.Label ); } 	
	const char* GetTime()		{ return GetString ( AtcWebUpdate::Time.Label ); } 	
	double GetPurse1Value()		{ return GetDouble ( AtcWebUpdate::Purse1Value.Label ); }
	double GetPurse2Value()		{ return GetDouble ( AtcWebUpdate::Purse2Value.Label ); }		// FSM portion
	double GetPurse3Value()		{ return GetDouble ( AtcWebUpdate::Purse3Value.Label ); }
	double GetCashValue()		{ return GetDouble ( AtcWebUpdate::CashValue.Label ); }

	double GetPurse1Balance()	{ return GetDouble ( AtcWebUpdate::Purse1Balance.Label ); }
	double GetPurse2Balance()	{ return GetDouble ( AtcWebUpdate::Purse2Balance.Label ); }
	double GetPurse3Balance()	{ return GetDouble ( AtcWebUpdate::Purse3Balance.Label ); }

	double GetTransactionValue();
	double GetTransactionValue ( int nPurseNo );

	CString GetXmlDate ( int nWebPaymentType );

	CString GetWebPaymentIDString();

private:
	bool AddNewAtcUpdatesInternal(const char* szAtcSourceFile);
	const char* GetNotes() { return GetString ( AtcWebUpdate::Notes.Label ); }

	int CheckSpend ( CSQLAuditRecord* pAtcRecord );
	int CheckTopup ( CSQLAuditRecord* pAtcRecord );
	int CheckAdjustment ( CSQLAuditRecord* pAtcRecord );

	bool SaveWebPaymentAtcTransactionLine ( CSQLAuditRecord* pAtcRecord, const char*szText, bool bUseFiledTime );

private:
	void GetPurchaseText(CSQLAuditRecord* pAtcRecord, CString& strPurchaseText);
	void ExtractPurchaseText(CSQLAuditRecord* pAtcRecord, CTLogDatabase* pDatabase, bool bNewDatabase, CString& strPurchaseText);
	CString GetHistoryFilename(CString strUserID);

private:
	int m_nActionType;

private:
	CTLogCache m_TLogCache;
};

//*******************************************************************
