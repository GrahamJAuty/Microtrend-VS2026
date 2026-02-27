#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "ParentMailData.h"
#include "WebPaymentList.h"
#include "WebPaymentLogRecord.h"
/**********************************************************************/

class CParentMailImport  
{
public:
	CParentMailImport ( CWnd* pParent = NULL );

/**********************************************************************/
#ifdef SYSTEMTYPE_BACKGROUND
/**********************************************************************/

public:
	void LogMessage( CString strMsg );
	int DisplayMessage( CString strMsg, UINT nType, int nBackgroundReply ){ return nBackgroundReply; }

private:
	void DisplayImportResults( int nRecordCount ){}
	
/**********************************************************************/
#else
/**********************************************************************/

public:
	void LogMessage( CString strMsg ){}
	int DisplayMessage( CString strMsg, UINT nType, int nBackgroundReply );

private:
	void DisplayImportResults( int nRecordCount );

/**********************************************************************/
#endif
/**********************************************************************/

public:
	bool Import ( bool bNewPayments );
	CString GetError() { return m_strError; }
	int GetPaymentCount() { return m_nValidCount; }

private:
	CString GetWebPayments();
	bool ImportPayments ( const char* szFilename );

	bool SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord );
	void LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText );

	int DisplayMessage ( const char* szMsg, UINT nType );

private:
	CWnd* m_pParent;
	CString m_strTitle;
	CString m_strError;
	
	CParentMailData m_data;
	CWebPaymentListDatabase m_WebPaymentList;

	CString m_strLogFileName;
	CString m_strTempLogFile;
	CString m_strExceptionName;
	CString m_strTempExceptionFile;
	CString m_strExceptionIndicator;

	bool m_bNewPayments;
	int m_nValidCount;
	int m_nExceptionCount;
	__int64 m_nHighestTransactionID;
	int m_nAccountPurse;
};

/**********************************************************************/
