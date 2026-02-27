#pragma once
//$$******************************************************************
#define nWEBEXPORT_NOERROR		0
#define nWEBEXPORT_ERROR		1
//$$******************************************************************

class CWebPaymentExportTxLog
{
public:
	CWebPaymentExportTxLog();
	bool SaveLog ( int nType, int nResultCode, const char* szComment );
	bool ReadLine ( CSSFile* fp );

	CString GetTypeString();
	CString GetResultString();

public:
	CSSDate m_date;
	CSSTime m_time;
	CString m_strComment;

private:
	 CString m_strLogFilePath;

	int m_nType;
	int m_nResultCode;
};

//$$******************************************************************
