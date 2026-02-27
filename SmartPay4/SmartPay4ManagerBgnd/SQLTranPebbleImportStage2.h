#pragma once
/**********************************************************************/
#include "WebPaymentLogRecord.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLTranBase.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
/**********************************************************************/

class CSQLTranPebbleImportStage2 : public CSQLTranBase
{
public:
	CSQLTranPebbleImportStage2(CWorkingDlg& WorkingDlg, bool bNewPayments, int& nValidPayments, CWnd* pParent = NULL);

public:
	void DoWork();
	
public:
	CString GetPebbleError() { return m_strPebbleError; }
	void SetPebbleError(CString str) { m_strPebbleError = str; }

public:
	bool GetNewPaymentsFlag() { return m_bNewPayments; }
	CString GetExceptionName() { return m_strExceptionName; }
	CString GetNewTempExceptionFile() { return m_strNewTempExceptionFile; }
	CString GetTempLogFile() { return m_strTempLogFile; }
	CString GetTempExceptionFile() { return m_strTempExceptionFile; }
	CString GetLogFileName() { return m_strLogFileName; }
	int GetNewExceptionCount() { return m_nNewExceptionCount;  }
	int GetExceptionCount() { return m_nExceptionCount; }

private:
	void LogException(CWebPaymentLogRecord* pLogRecord, const char* szErrorText );

private:
	CWorkingDlg& m_WorkingDlg;
	CWnd* m_pParent;

private:
	CString m_strExceptionIndicator;
	CString m_strLogFileName;
	CString m_strTempLogFile;
	CString m_strExceptionName;
	CString m_strNewExceptionName;
	CString m_strTempExceptionFile;
	CString m_strNewTempExceptionFile;
	int m_nExceptionCount;
	int m_nNewExceptionCount;
	int& m_nValidCount;
	bool m_bNewPayments;

private:
	CString m_strPebbleError;
};

/**********************************************************************/
