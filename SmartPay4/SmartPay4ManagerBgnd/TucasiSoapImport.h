#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
/**********************************************************************/
#include "TucasiSOAPData.h"
#include "WebPaymentList.h"
#include "WebPaymentLogRecord.h"
/**********************************************************************/

class CTucasiSoapImport  
{
public:
	CTucasiSoapImport ( CWnd* pParent = NULL );

//**********************************************************************
#ifdef SYSTEMTYPE_BACKGROUND
//**********************************************************************

private:
	int DisplayMessage( CString strMsg, UINT nType, int nBackgroundReply ){ return nBackgroundReply; }
	void LogMessage( CString strMsg );

//**********************************************************************
#else
//**********************************************************************

private:
	int DisplayMessage( CString strMsg, UINT nType, int nBackgroundReply );
	void LogMessage( CString strMsg ){}

//**********************************************************************
#endif
//**********************************************************************

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
	CString m_strTitle;
	CString m_strError;
	
	CTucasiSoapData m_data;
	CWebPaymentListDatabase m_WebPaymentList;

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

//**********************************************************************
