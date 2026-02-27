#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
/**********************************************************************/
#include "Squid.hpp"
#include "WebPaymentList.h"
#include "WebPaymentLogRecord.h"
/**********************************************************************/

class CSquidImport  
{
public:
	CSquidImport ( CWnd* pParent = NULL );

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
	void SendAcknowledgePayment();

	bool SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord );
	void LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText );

	void DisplayImportResults( int nRecordCount );

private:
	CWnd* m_pParent;
	CSquidData m_data;
	CString m_strError;
	CString m_strTitle;

	bool m_bNewPayments;
	bool m_bMoreRecordsAvailable;

	int m_nAccountPurse;
	int m_nValidCount;
	int m_nExceptionCount;

	CWebPaymentListDatabase m_WebPaymentList;

	CString m_strLogFileName;
	CString m_strTempLogFile;
	CString m_strExceptionName;
	CString m_strTempExceptionFile;
	CString m_strTempPendingFile;
	CString m_strPaymentAckFile;
	CString m_strExceptionIndicator;
};

//**********************************************************************
