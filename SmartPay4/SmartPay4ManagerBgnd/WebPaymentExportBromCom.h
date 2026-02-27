#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\SQLDb.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//*******************************************************************

#define nWEBEXPORT_BALANCES		1			// export balances only	( also used by export log )
#define nWEBEXPORT_UPDATES		2			// export transaction updates only ( also used by export log )

//*******************************************************************

class CWebPaymentExportBromCom  
{
public:
	CWebPaymentExportBromCom ( CWnd* pParent = NULL );

public:
	void SendAccountBalances();
	bool SendAccountUpdates ( bool bIsEndOfDay );

	int GetBalanceCount() { return m_nBalanceCount; }
	const char* GetError() { return m_strError; }

private:
	void ExportTransactions();
	void ExportBalances ( __int64 nCount);

private:
	CWnd* m_pParent;
	CString m_strError;
	CSQLRepositoryWebPaymentOptions m_webOptions;
	bool m_bKeepXmlFiles;

private:
	int m_nBalanceCount;
};

//*******************************************************************
