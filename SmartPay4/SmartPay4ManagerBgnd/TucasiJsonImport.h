#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
/**********************************************************************/
#include "WebPaymentList.h"
#include "WebPaymentList2.h"
#include "WebPaymentLogRecord.h"
/**********************************************************************/

class CTucasiJsonImport  
{
public:
	CTucasiJsonImport ( CWnd* pParent = NULL );

/**********************************************************************/
#ifdef SYSTEMTYPE_BACKGROUND
/**********************************************************************/

private:
	void DisplayMessage ( CString strMsg, UINT nType ){}
	void LogMessage( CString strMsg );

/**********************************************************************/
#else
/**********************************************************************/

private:
	void DisplayMessage ( CString strMsg, UINT nType );
	void LogMessage( CString strMsg ){}

/**********************************************************************/
#endif
/**********************************************************************/

public:
	bool Import ( bool bNewPayments );
	const char* GetError() { return m_strError; }
	int GetPaymentCount() { return m_nValidCount; }

private:
	CString GetWebPayments();
	bool ImportPayments ( const char* szFilename );

	bool SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord );
	void LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText );

	void DisplayImportResults( int nRecordCount );
	
private:
	CWnd* m_pParent;
	CString m_strError;
	CString m_strTitle;
	
	CTucasiJsonData m_data;

	CWebPaymentListDatabase m_WebPaymentList;
	CWebPaymentList2Database m_WebPaymentList2;

	CString m_strLogFileName;
	CString m_strTempLogFile;

	CString m_strExceptionName;
	CString m_strTempExceptionFile;
	CString m_strExceptionIndicator;

	bool m_bNewPayments;
	int m_nValidCount;
	int m_nExceptionCount;

	__int64 m_nHighestTransactionNo;
};

/**********************************************************************/
