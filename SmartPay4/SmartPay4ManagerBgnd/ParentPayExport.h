#pragma once
//***//*******************************************************************
#include "ParentPayData.h"
#include "WebPaymentAtcTransactionFile.h"
//*******************************************************************

class CParentPayExport  
{
public:
	CParentPayExport ( CParentPayData* pData, CWnd* pParent = NULL );

//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	bool SendOpeningBalanceList ( const char* szListFilename );
	bool SendDinerMatchList ( const char* szListFilename );

	void ResendTransactions ( const char* szCsvWebFilename );
	void ResendPaymentEcho ( const char* szCsvWebFilename );
	void ResendDinerMatch ( const char* szCsvWebFilename );

//******************************************************************
#endif
//******************************************************************

public:
	const char* GetError() { return m_strError; }

	bool SendAnyUpdates();
	bool SendPaymentEcho( const char* szEchoDataFile );
	bool SendDinerMatch ( const char* szDinerMatchFile );

private:
	void ResetTransaction ( const char* szAccountID = "" );

	bool SendTransactionData ( const char* szTransactionFile, bool bLogReqd );
	bool SendTxData ( int nTxnDataType, const char* szDataFile );

	void AddToRetryQueue ( int nTxnDataType, const char* szDataFile );
	bool SendAnyRetries();

	int ConvertWebUpdates ( const char* szWebUpdates, const char* szCsvFilename, const char* szHeader );

	bool IsValidSpend ( CWebPaymentAtcTransactionFile* pFile );
	bool IsValidCashAdded ( CWebPaymentAtcTransactionFile* pFile );
	bool SaveTransactionLine ( CWebPaymentAtcTransactionFile* pFile, CSSFile* fp );

	int CreateStartingBalancesFile ( const char* szListFilename, const char* szDestFilename );
	bool SaveStartingBalanceLine ( CSSFile* fp );

	int CreateDinerMatchFile ( const char* szListFilename, const char* szDestFilename );

	CString GetListPromptMessage ( const char* szText, int nCount );

private:
	CParentPayData* m_pData;
	CWnd* m_pParent;
	CString m_strError;

	CString m_strAccountID;
	double m_dBalance;
	double m_dSalesValue;
	CString m_strDescription;
	int m_nMealSession;
	CString m_strBalanceXmlDate;
	bool m_bIsRetryPass;
};

//*******************************************************************
