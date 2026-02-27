#pragma once
//**********************************************************************
#include "..\SmartPay4ManagerBgnd\SQLTranPebbleImportStage2.h"
//**********************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLTable_PebbleConfig\SQLRepositoryPebbleConfig.h"
#include "..\SmartPay4Shared\SQLTable_PebblePendingTransaction\SQLRowPebblePendingTransaction.h"
//**********************************************************************
#include "WebPaymentList.h"
#include "WebPaymentLogRecord.h"
//**********************************************************************

class CPebbleImport  
{
public:
	CPebbleImport ( CWnd* pParent = NULL );

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
	const char* GetPebbleError() { return m_strPebbleError; }
	int GetValidPaymentCount() { return m_nValidPaymentCount; }
	
private:
	bool FetchNewTransactions();
	void PurgeRecentTransactions();
	void PurgeRecentPayments();
	bool ProcessTransactions(CSQLTranPebbleImportStage2& Tran);
	void DisplayImportResults(CSQLTranPebbleImportStage2& Tran );

private:
	CWnd* m_pParent;
	CSQLRepositoryPebbleConfig m_PebbleConfig;
	CString m_strPebbleError;
	CString m_strTitle;

private:
	int m_nValidPaymentCount;
	int m_nTotalPaymentCount;

private:
	CWorkingDlg m_WorkingDlg;
};

//**********************************************************************
