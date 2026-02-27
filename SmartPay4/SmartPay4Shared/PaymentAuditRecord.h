#pragma once
/**********************************************************************/

class CPaymentAuditRecord
{
public:
	CPaymentAuditRecord();

	bool SaveEntry();
	bool SaveEntry (const char* szFilename );
	bool ReadEntry ( CSSFile* file );
	bool ExtractData ( CCSV *pCsv );
	bool WriteLine ( CSSFile *pFile );

	bool SaveEODEntry();
	bool IsEODEntry();

	const char* GetDateFiled() { return m_dateFiled.GetDate(); }
//	const char* GetCSVDateFiled() { return m_dateFiled.GetCSVDate(); }
	const char* GetTimeFiled() { return m_timeFiled.GetTime(); }

	const char* GetUserID() { return m_strUserID; }
	void SetUserID ( const char* szUserID ) { m_strUserID = szUserID; }

	const char* GetDateLastUsed() { return m_dateLastUsed.GetDate(); }
	void SetDateLastUsed ( const char* szDate ) {  m_dateLastUsed.SetDate ( szDate ); }

	const char* GetTimeLastUsed() { return m_timeLastUsed.GetTime(); }
	void SetTimeLastUsed ( const char* szTime ) { m_timeLastUsed.SetTime ( szTime ); }

	int GetPaymentType() { return m_nPaymentType; }
	void SetPaymentType ( int nNo ) { m_nPaymentType = nNo; }

	double GetPaymentValue1() { return m_dPaymentValue1; }
	void SetPaymentValue1 ( double dValue ) { m_dPaymentValue1 = dValue; }

	double GetPaymentValue2() { return m_dPaymentValue2; }
	void SetPaymentValue2 ( double dValue ) { m_dPaymentValue2 = dValue; }

	double GetPaymentValue3() { return m_dPaymentValue3; }
	void SetPaymentValue3 ( double dValue ) { m_dPaymentValue3 = dValue; }

	CString GetTransactionNo() { return m_strTransactionNo; }
	void SetTransactionNo ( const char* szNo ) { m_strTransactionNo = szNo; }

	int GetFolderSet() { return m_nFolderSet; }
	void SetFolderSet(int n) { m_nFolderSet = n; }

	int GetTerminalNo() { return m_nTerminalNo; }
	void SetTerminalNo ( int nNo ) { m_nTerminalNo = nNo; }

	CString GetOperatorID() { return m_strOperatorID; }
	void SetOperatorID ( const char* szID )	{ m_strOperatorID = szID; }

	bool GetExternalMode() { return m_bExternalMode; }
	void SetExternalMode(bool b) { m_bExternalMode = b;  }

private:
	CSSDate m_dateFiled;				// date line save in audit file
	CSSTime m_timeFiled;				// time line savedd in audit
	CString m_strUserID;
	CSSDate m_dateLastUsed;				// date last used @ till
	CSSTime m_timeLastUsed;				// time last used @ till
	CString m_strOperatorID;
	int m_nFolderSet;
	int m_nTerminalNo;
	CString m_strTransactionNo;
	int m_nPaymentType;
	double m_dPaymentValue1;
	double m_dPaymentValue2;
	double m_dPaymentValue3;
	bool m_bExternalMode;
};

/**********************************************************************/