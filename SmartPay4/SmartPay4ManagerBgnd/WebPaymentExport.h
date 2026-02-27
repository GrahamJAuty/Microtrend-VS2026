#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//*******************************************************************

#define nWEBEXPORT_BALANCES		1			// export balances only	( also used by export log )
#define nWEBEXPORT_UPDATES		2			// export transaction updates only ( also used by export log )

//*******************************************************************

class CWebPaymentExport  
{
public:
	CWebPaymentExport ( CWnd* pParent = NULL );

//******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//******************************************************************

public:
	void SendOpeningBalanceList (  const char* szListFilename );
	void SendDinerMatchList (  const char* szListFilename );
	
//******************************************************************
#endif 
//******************************************************************

public:
	void SendAccountBalances();
	bool SendAccountUpdates ( bool bIsEndOfDay );

	void SpecialSalesExport();

	int GetBalanceCount() { return m_nBalanceCount; }
	int GetTransactionCount() { return m_nPurchaseCount; }
	const char* GetError() { return m_strError; }

private:
	void PostBalances ( const char* szCSVFilename );
	void ExportTransactions();
	void WebPaymentExportBalances ( const char* szDbBalanceList = "" );
	void PostTransactions ( const char* szCSVFile, const char* szCSVFile2 );

	bool VerifyWebAccess ( const char* szMessage );

private:
	CWnd* m_pParent;
	CString m_strError;

	CSQLRepositoryWebPaymentOptions m_webOptions;

	int m_nBalanceCount;
	int m_nPurchaseCount;
	bool m_bKeepXmlFiles;
	bool m_bHaveService2;
};

//*******************************************************************
