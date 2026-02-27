//***************************************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ****************************
//***************************************************************************************

#pragma once

#include "GlobalFunctions.h"

#define szTEXT_EMPTYCASH			"EMPTY"
#define szTEXT_STARTUP_POSTINGOK	"Auto topup"			// KioskCardLink / KiosBioLink
#define szTEXT_STARTUP_POSTINGFAIL	"Auto topup FAILED"		// KioskCardLink
#define szTEXT_STARTUP_UPDATEFAIL	"Card removed early"	// KioskCardLink(Standalone)
#define szTEXT_PENDINGDONE			"Update Done"			// KioskCardLink
#define szTEXT_KIOSKTEST			"Kiosk Test"			// KioskSmartPayLink

/**********************************************************************/

class CPayLinkAuditRecord
{
public:
	CPayLinkAuditRecord();
	bool ReadLine( CSSFile* file );
	bool SaveLine ( CSSFile* file );

	const char* Date() { return m_date.GetDate(); }
	const char* Time() { return m_time.GetTime(); }

	CString UserID() { return m_strUserID; }
	void SetUserID ( const char* szUserID ) { m_strUserID = szUserID; }

	CString UserName()						{ return m_strUserName; }
	void SetUserName ( const char* szUserName )	{ m_strUserName = szUserName; }

	int GroupNo()							{ return m_nGroupNo; }
	void SetGroupNo ( int nGroupNo )		{ m_nGroupNo = nGroupNo; }

	int CCNo()								{ return m_nCCNo; }
	void SetCCNo ( int nCCNo )				{ m_nCCNo = nCCNo; }	// Terminal CCNo

	long  CoinageAdded()  { return m_lValue1; }						// Get cash added in pence
	double ValueAdded();											// get cash added in pounds\pence
	void SetCoinageAdded ( long lValue1 ) { m_lValue1 = lValue1; }	// Coinage added \ total added in transaction (pence)

	long CoinageGT()	 { return m_lValue2; }						// Get GT pence
	void SetGTValue ( long lValue2 )	 { m_lValue2 = lValue2; }	// Set GT in pence
	double ValueGT();												// get gt in pounds\pence

	void SetDifferValue ( long lValue1 ) { m_lValue1 = lValue1; }	// Paylink StarttupDiffer value
	double ValueDiffer();											// get differ value pounds\pence

	void SetText ( const char* szText ) { m_strText = szText; };
	CString GetText() { return m_strText; }

	void SetTopupPurse ( int n )	{ m_nTopupPurse = n; }
	int GetTopupPurse()				{ return m_nTopupPurse; }

private:
	double MakeDouble ( long lValue );

private:
	CSSDate m_date;				// Date
	CSSTime m_time;				// Time
	CString m_strUserID;		// User ID
	CString m_strUserName;		// User Name
	int m_nGroupNo;				// USer Group
	int m_nCCNo;				// ccno
	long m_lValue1;				// log value1
	long m_lValue2;				// log value2
	CString m_strText;			// additional info text
	int m_nTopupPurse;			// account purse that was topup
};
