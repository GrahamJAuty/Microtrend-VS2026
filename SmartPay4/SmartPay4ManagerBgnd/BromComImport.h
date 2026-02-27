#pragma once
//**********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLTable_BromComPendingOrder\SQLRowBromComPendingOrder.h"
//**********************************************************************
#include "BromComData.h"
#include "WebPaymentList.h"
#include "WebPaymentLogRecord.h"
//**********************************************************************

class CBromComImport  
{
public:
	CBromComImport ( CWnd* pParent = NULL );

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
	bool Import(bool bNewPaymants);
	const char* GetBromComError() { return m_strBromComError; }
	int GetValidPaymentCount() { return m_nValidPaymentCount; }
	
private:
	void ProcessPendingOrders();
	void LogException ( CWebPaymentLogRecord* pLogRecord, const char* szErrorText, bool bNewException );
	void DisplayImportResults( int nRecordCount );

public:
	static void GetItemDelta( CSQLRowBromComPendingOrder& RowPendingOrder, CReportConsolidationArray<CConsolidatedDoubleByInt>& arrayItemDelta);

private:
	CWnd* m_pParent;
	CBromComData m_BromComData;
	CString m_strBromComError;
	CString m_strTitle;

	bool m_bNewPayments;
	bool m_bDisplayImportResults;
	int m_nValidPaymentCount;
	int m_nExceptionCount;
	int m_nNewExceptionCount;

	CString m_strLogFileName;
	CString m_strTempLogFile;
	CString m_strNewExceptionName;
	CString m_strExceptionName;
	CString m_strTempExceptionFile;
	CString m_strNewTempExceptionFile;
	CString m_strTempPendingFile;
	CString m_strExceptionIndicator;

	CString m_strFilenamePendingOrders;
	CString m_strFilenamePendingItems;

private:
	CWorkingDlg m_WorkingDlg;
};

//**********************************************************************
