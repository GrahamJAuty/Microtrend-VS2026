#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
/**********************************************************************/
#include "WebPaymentList.h"
#include "WebPaymentLogRecord.h"
#include "WisePayData.h"
/**********************************************************************/

class CWisePayImport  
{
public:
	CWisePayImport ( CWnd* pParent = NULL );

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
	CString GetWebPayments ( int nAccountNo );
	bool ImportPayments ( int AccountNo, const char* szFilename );

	bool SavePendingUpdate ( CSSUnsortedDatabase* pDbPayments, CWebPaymentLogRecord* pLogRecord );
	void LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText );

	void DisplayImportResults( int nRecordCount, int nAccountID );

private:
	CWnd* m_pParent;
	CString m_strTitle;
	CString m_strError;
	
	CWisePayData m_data;
	CWebPaymentListDatabase m_WebPaymentList;
	
	CString m_strLogFileName;
	CString m_strTempLogFile;

	CString m_strExceptionName;
	CString m_strTempExceptionFile;
	CString m_strExceptionIndicator;

	bool m_bNewPayments;
	int m_nAccountPurse;
	__int64 m_nHighestPaymentID;
	int m_nValidCount;
	int m_nExceptionCount;
};

//**********************************************************************
