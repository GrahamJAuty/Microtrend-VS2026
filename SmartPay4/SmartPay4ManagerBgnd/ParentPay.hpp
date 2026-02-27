#pragma once
//**********************************************************************
#include "ParentPayData.h"
//**********************************************************************
#define	nPARENTPAY_TXDINERMATCH		1
#define	nPARENTPAY_TXPAYMENTECHO	2
#define	nPARENTPAY_TXFINANCIAL		3
//**********************************************************************

class CSSParentPay
{
public:
	CSSParentPay ( CParentPayData* pData );
	CString GetPayments( int nServiceNo, const char* szLoginReply, const char* szCsvFilename );
	CString RequestDinerData ( const char* szLoginReply, const char* szCsvFilename );

	CString SendDinerMatchAdvice ( const char* szSource, const char* szLoginReply );
	CString SendPaymentEchoData ( const char* szSource, const char* szLoginReply );
	CString SendFinancialData ( const char* szSource, const char* szLoginReply );

//	int GetMaxBatchSize()	{ return m_nUploadBatchSize; }
	int GetTxCount()		{ return m_nQueueLength; }

	CString GetDataValidationError() { return m_strValidateError; }

private:
	void AddLoginDetails ( CSSSoap* pSoap );
	CString ValidateReply ( const char* szTitle, CSSSoap* pSoap, const char* szLabel );

	CString GetUploadToken ( const char* szMainTitle, int nQueueLength );

	void AddTransactionDataHeader ( CSSSoap* pSoap, int nQueueLength );
	void AddNullProductSaleData ( CSSSoap* pSoap );
	void AddNullTxnEchoData ( CSSSoap* pSoap );
	void AddNullRewardPointData ( CSSSoap* pSoap );
	void AddNullFinancialTxnData ( CSSSoap* pSoap );
	void AddNullCancellationData ( CSSSoap* pSoap );
	void AddNullControlRecordData ( CSSSoap* pSoap );
	void AddNullDinerMatchAdviceData ( CSSSoap* pSoap );

private:
	CParentPayData* m_pData;
	CString m_strSendError;
	CString m_strValidateError;

	CString m_strUploadToken;
	int	m_nUploadBatchSize;
	int m_nQueueLength;
};

//**********************************************************************
