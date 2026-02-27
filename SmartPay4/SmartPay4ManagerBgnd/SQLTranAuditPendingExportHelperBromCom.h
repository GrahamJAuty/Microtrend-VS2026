#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_AuditPendingExport\SQLRowAuditPendingExport.h"
#include "..\SmartPay4Shared\SQLTable_BromComTransaction\SQLRowBromComTransaction.h"
//*******************************************************************
#include "BromComData.h"
//*******************************************************************

class CSQLTranAuditPendingExportHelperBromCom
{
public:
	CSQLTranAuditPendingExportHelperBromCom();
	void CreateBromComLine(CSQLRowAuditPendingExport& RowPending, CDatabase* pDatabase);
	void FinaliseLog();
	const char* GetError() { return m_strError; }
	
private:
	bool LogPendingTransaction ( CString strLine );
	void GetTransactionDetail(CSQLRowAccountFull& RowAccount, CSQLRowAuditPendingExport& RowPending, CString& strTransactionDetail, CSQLRowBromComTransaction& RowTran );
	bool LogUpdates ( const char* szPrefix );

private:
	CString m_strError;	
	CString m_strTempLogFilename;
	CString m_strTempLogHeader;

private:
	CBromComData m_BromComData;
};

/**********************************************************************/
