#pragma once
//*******************************************************************************/
#define nRECORDTYPE_EPOS	1				// Epos sale
#define nRECORDTYPE_KIOSK	2				// Kiosk record
#define nRECORDTYPE_VENDING	3				// vending record
//*******************************************************************************/

class CTLIDRecord : public CCSVRecord
{
public:
	CTLIDRecord ( int nTerminalType = nRECORDTYPE_EPOS );

	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );
	void Reset ( int nTerminalNo, int nTerminalType );

	void SetCashRemoved ( int nCCNo, double dGT, const char* szCashFilename );
	void UpdateCID ( double dValue );

	void SetTerminalType ( int nType ) { m_nTerminalType = nType; }
	int GetTerminalType() { return m_nTerminalType; }

	const char* GetTerminalText();
	void SetTerminalNo ( int nTNo ) { m_nTerminalNo = nTNo; }
	int GetTerminalNo() { return m_nTerminalNo; }

	void SetUserID ( const char* szUserID ) { m_strUserID = szUserID; }
	CString GetUserID() { return m_strUserID; }
	bool HaveUserID() { return ( m_strUserID != "" ) ? TRUE : FALSE; }

	void SetCardValue ( double dValue ) { m_dCardValue = dValue; }
	double GetCardValue() { return m_dCardValue; }

	const char* GetDate() { return m_date.GetDate(); }
	bool IsDateSet() { return m_date.IsSet(); }
	void SetDate ( const char* szDate = "" );

	const char* GetTime() { return m_time.GetTime(); }
	void SetTime ( const char* szTime = "" );

	bool IsCashEmpty()					{ return ( m_dTTLInDrawer == 0 ) ? TRUE : FALSE; }
	double GetTTLInDrawer() { return m_dTTLInDrawer; }
	int GetTTLCounter() { return m_nTTLCounter; }

	int IncrementCCNo();
	int GetCCNo() { return m_nCCNo; }

	int GetStatusFlags() { return m_nStatusFlags; }

	void SetDisabled ( bool bDisable )		{ if ( bDisable == TRUE ) m_nStatusFlags |= 0x01; else m_nStatusFlags &= ~0x01; }
	bool IsDisabled()						{ return ( (m_nStatusFlags & 0x01) == 0x01 ) ? TRUE : FALSE; }

	void SetDoorOpen ( bool bOpen )			{ if ( bOpen == TRUE ) m_nStatusFlags |= 0x02; else m_nStatusFlags &= ~0x02; }
	bool IsDoorOpen()						{ return ( (m_nStatusFlags & 0x02) == 0x02 ) ? TRUE : FALSE; }

	void SetTerminalOn ( bool bOn )			{ if ( bOn == TRUE ) m_nStatusFlags |= 0x04; else m_nStatusFlags &= ~0x04; }
	bool IsTerminalOn()						{ return ( (m_nStatusFlags & 0x04) == 0x04 ) ? TRUE : FALSE; }

	void SetInService ( bool bInService )	{ if ( bInService == TRUE ) m_nStatusFlags |= 0x08; else m_nStatusFlags &= ~0x08; }
	bool IsInService()						{ return ( (m_nStatusFlags & 0x08) == 0x08 ) ? TRUE : FALSE; }

private:
	int m_nTerminalType;
	int m_nTerminalNo;
	int m_nCCNo;
	CSSDate m_date;
	CSSTime m_time;
	CString m_strUserID;
	double m_dCardValue;
	int m_nTTLCounter;
	double m_dTTLInDrawer;
	double m_dGTValue;
	int m_nStatusFlags;
};

//**********************************************************************

class CTLIDDatabase : public CSharedCSVArray
{
public:
	CTLIDDatabase();

	bool GetRecord ( int nTerminalNo, CTLIDRecord& Record );
	bool GetRecordByIndex ( int nTerminalNo, CTLIDRecord& Record );
	bool SaveRecord ( CTLIDRecord& Record );

private:
	bool FindRecord ( int nTerminalNo, int& nIndex );
};

//**********************************************************************
//extern CTLIDDatabase dbTLID;
//**********************************************************************
