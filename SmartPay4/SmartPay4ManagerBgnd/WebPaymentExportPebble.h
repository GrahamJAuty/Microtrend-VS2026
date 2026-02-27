#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\SQLDb.h"
//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//*******************************************************************
#include "TLogCache.h"
//*******************************************************************
#define nWEBEXPORT_UPDATES		2			// export transaction updates only ( also used by export log )
//*******************************************************************

struct CWebPaymentExportPebbleDone
{
public:
	CWebPaymentExportPebbleDone();

public:
	__int64 m_nLineID;
	bool m_bGotPayments;
	__int64 m_nUserID;
	CString m_strDate;
	CString m_strTime;
	int m_nCCNo;
};

//*******************************************************************

class CWebPaymentExportPebble
{
public:
	CWebPaymentExportPebble(CWnd* pParent = NULL);

public:
	bool SendAccountUpdates();

public:
	const char* GetError() { return m_strError; }

private:
	void ExportTransactions(CSQLRepositoryPebbleConfig& PebbleConfig);
	void GetTransactionItems(CTLogCache& TLogCache, CSQLRowPebbleExportTransaction& RowTran, CArray<CPebbleTransactionItem, CPebbleTransactionItem>& arrayTransactionItems);
	void GetTransactionPayments(CSQLRowPebbleExportTransaction& RowTran, CArray<CPebbleTransactionPayment, CPebbleTransactionPayment>& arrayTransactionPayments, CWebPaymentExportPebbleDone& Done, bool bTopup, bool& bGotInvalidPayment);
	bool LogPendingTransaction(CString strLine, CString strTempLogFilename, CString strTempLogHeader);
	bool LogUpdates(CString strPrefix, CString strTempLogFilename);

private:
	CWnd* m_pParent;
	CString m_strError;
	CSQLRepositoryWebPaymentOptions m_webOptions;
};

//*******************************************************************
