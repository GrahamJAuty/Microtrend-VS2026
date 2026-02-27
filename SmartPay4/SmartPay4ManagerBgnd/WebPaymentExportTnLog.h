#pragma once

class CWebPaymentExportTnLog
{
public:
	CWebPaymentExportTnLog ( int m_nWebPaymentType, const char* szPrefix = "Tn" );
	bool SaveLog (  const char* szUpdatesFile, const char* szHeader = "" );

	CString GetCurrentLogName() { return m_strLogFilename; }
	void AddDateStamp ( const char* szLogFile, int nLineCount );

private:
	CSSDate m_date;
	CString m_strLogFilename;
};
