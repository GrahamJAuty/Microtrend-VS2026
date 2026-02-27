#pragma once
//*******************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "WebPaymentList.h"
#include "WebPaymentLogRecord.h"
//*******************************************************************

class CParentPayImport  
{
public:
	CParentPayImport ( CParentPayData* pData, CWnd* pParent = NULL );

/**********************************************************************/
#ifdef SYSTEMTYPE_BACKGROUND
/**********************************************************************/

public:
	void LogMessage( CString strMsg );
	void DisplayMessage( CString strMsg, UINT nType ){}
	
/**********************************************************************/
#else
/**********************************************************************/

public:
	void LogMessage( CString strMsg ){}
	void DisplayMessage( CString strMsg, UINT nType );
	
/**********************************************************************/
#endif
/**********************************************************************/

public:
	bool ImportPayments ( bool bNewPayments );
	CString GetError() { return m_strError; }
	int GetPaymentCount() { return m_nValidCount; }

private:
	CString GetWebPayments ( int nServiceNo );
	CString NoNewPaymentMessage ( int nServiceNo );
	bool CanIgnorePayment ( CString strPayerInfo );

	bool ImportPayments(  int nServiceNo, const char* szFilename );
	bool SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord, int nMealSession );
	void LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText );

	void SaveEchoPaymentZeroBalance (CSSUnsortedDatabase* pDbPayments, int nServiceNo);
	void SaveEchoPaymentRealBalance (CSSUnsortedDatabase* pDbPayments, int nServiceNo, CSQLRowAccountFull& RowAccount);
	void SaveEchoPaymentInternal(CSSUnsortedDatabase* pDbPayments, int nServiceNo, double dBalance);

	void EchoNewPayments();

	void DisplayImportResults( int nServiceNo, int nRecordCount );

private:
	CWnd* m_pParent;
	CString m_strTitle;
	CString m_strError;
	
	CParentPayData* m_pData;
	CWebPaymentListDatabase m_WebPaymentList;
	
	CString m_strLogFileName;
	CString m_strTempLogFile;

	CString m_strExceptionName;
	CString m_strTempExceptionFile;
	CString m_strExceptionIndicator;
	CString m_strPaymentEchoFile;

	bool m_bNewPayments;
	int m_nValidCount;
	int m_nExceptionCount;
	__int64 m_nHighestPaymentID;
	int m_nAccountPurse;
};
