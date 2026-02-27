#pragma once
//************************************************************
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
//************************************************************

class CWebPaymentImport  
{
public:
	CWebPaymentImport ( CWnd* pParent = NULL );
	int GetPaymentCount() { return m_nPaymentCount; }
	bool ImportPayments ( bool bNewPayments, bool& bTriedSendUpdates );
	bool IsExcludedByTime() { return bExcludedByTime; }

private:
	void ImportParentPay(CString& strRxError, CString& strTxError, bool& bUpdatesSent, bool& bNewPayments);
	void ImportTucasi(CString& strRxError, bool& bNewPayments);
	void ImportTucasiV2(CString & strRxError, CString & strTxError, bool& bUpdatesSent, bool& bNewPayments);
	void ImportWisePay(CString& strRxError, bool& bNewPayments);
	void ImportSchoolComms(CString& strRxError, bool& bNewPayments);
	void ImportSquid(CString& strRxError, CString& strTxError, bool& bUpdatesSent, bool& bNewPayments);
	void ImportParentMail(CString& strRxError, CString& strTxError, bool& bUpdatesSent, bool& bEODSent, bool& bNewPayments);
	void ImportBromCom(CString& strRxError, bool& bNewPayments);
	void ImportPebble(CString& strRxError, bool& bNewPayments);

private:
	CWnd* m_pParent;
	int m_nPaymentCount;
	bool bExcludedByTime;

	CSQLRepositoryWebPaymentOptions m_webOptions;
};

//************************************************************
