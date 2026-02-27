#pragma once
//$$******************************************************************

class CWebPaymentLogRecord  
{
public:
	CWebPaymentLogRecord();

	bool ReadLine ( CSSFile* fpFile );

	bool LogValidImport ( const char* szFilename, const char* szCardNo, int& nCounter ); 
	bool LogException ( const char* szFilename, const char* szIndicator, const char* szErrorText, int& nExceptionCounter );
	bool LogAlreadyProcessed (  const char* szFilename, const char* szDate, const char* szTime, int& nExceptionCounter );

	CString GetLogFileName();
	CString GetExceptionFileName();
	CString GetNewExceptionFileName();

	const char* GetDate() { return m_date.GetDate(); }
	const char* GetTime() { return m_time.GetTime(); }

	void SetPrefix ( const char* szText )		{ m_strPrefix = szText; }
	void SetPrefix ( __int64 nCardNo )			{ m_strPrefix = ::FormatInt64Value(nCardNo ); }

	void SetPupilID ( const char* szText )		{ m_strPupilID = szText ; }
	void SetPupilID ( __int64 nID )				{ m_strPupilID = ::FormatInt64Value(nID); }

	void SetPaidAmount ( const char* szText )	{ m_dPaidAmount = atof(szText); }
	void SetPaidAmount ( double dValue )		{ m_dPaidAmount = dValue; }

	void SetPaymentID ( const char* szText )	{ m_nPaymentID = _atoi64(szText); }
	void SetPaymentID ( __int64 nID )			{ m_nPaymentID = nID; }

	void SetServiceID ( const char* szText )	{ m_nServiceID = _atoi64(szText); }
	void SetServiceID ( int nID )				{ m_nServiceID = nID; }
	void SetServiceID ( __int64 nID )			{ m_nServiceID = nID; }

	void SetPaidDate ( const char* szText )		{ m_strPaidDate = szText; }
	void SetReceiptCode ( const char* szText )	{ m_strReceiptCode = szText; }
	void SetTransactionRef ( const char* szText ) { m_strTransactionRef = szText; }
	void SetPupilName ( const char* szText  )	{ m_strPupilName = szText; }
	void SetDOB ( const char* szText )			{ m_strDOB = szText; }
	void SetNotes ( const char* szText )		{ m_strNotes = szText; }

	bool IsHeaderLine();
	const char* GetHeaderDate();
	const char* GetHeaderTime();

	const char* GetPrefix()						{ return m_strPrefix; }
	const char* GetPupilID()					{ return m_strPupilID; }
	__int64 GetPaymentID()						{ return m_nPaymentID; }
	__int64 GetServiceID()						{ return m_nServiceID; }
	double GetPaidAmount()						{ return m_dPaidAmount; }
	const char* GetPaidDate()					{ return m_strPaidDate; }
	const char* GetReceiptCode()				{ return m_strReceiptCode; }
	const char* GetTransactionRef()				{ return m_strTransactionRef; }
	const char* GetPupilName()					{ return m_strPupilName; }
	const char* GetDOB()						{ return m_strDOB; }
	const char* GetNotes()						{ return m_strNotes; }

private:
	bool LogImport ( const char* szFilename, const char* szText, int& nCounter );
	bool SaveLine ( CSSFile* fpFile );

private:
	CSSDate m_date;
	CSSTime m_time;
	CString m_strPrefix;						// gets set to cardno or error message
	CString m_strField1;
	CString m_strField2;
	CString m_strPupilID;
	double m_dPaidAmount;
	__int64 m_nPaymentID;
	__int64 m_nServiceID;
	CString m_strPaidDate;
	CString m_strReceiptCode;
	CString m_strTransactionRef;
	CString m_strPupilName;
	CString m_strDOB;
	CString m_strNotes;
};

//$$******************************************************************
