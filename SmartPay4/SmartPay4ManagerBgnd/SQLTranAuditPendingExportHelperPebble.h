#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_AuditPendingExport\SQLRowAuditPendingExport.h"
#include "..\SmartPay4Shared\SQLTable_PebbleExportTransaction\SQLRowPebbleExportTransaction.h"
//*******************************************************************
#
class CSQLTranAuditPendingExportHelperPebble
{
public:
	CSQLTranAuditPendingExportHelperPebble();
	void CreatePebbleLine(CSQLRowAuditPendingExport& RowPending, CDatabase* pDatabase);
	const char* GetError() { return m_strError; }
	
private:
	bool LogPendingTransaction ( CString strLine );
	void GetTransactionDetail(CSQLRowAccountFull& RowAccount, CSQLRowAuditPendingExport& RowPending, CSQLRowPebbleExportTransaction& RowTran );
	
private:
	CString m_strError;	
	CString m_strTempLogFilename;
	CString m_strTempLogHeader;
};

/**********************************************************************/
