#pragma once
/**********************************************************************/
#include "SQLTranAuditPendingExportHelperPebble.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_AuditPendingExport\SQLRowAuditPendingExport.h"
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranAuditPendingExportPebble: public CSQLTranBase
{
public:
	CSQLTranAuditPendingExportPebble(CString strAuditFilename);

public:
	void DoWork();
	void AfterTransaction();

public:
	CString GetError() { return m_strError; }
	
private:
	bool SaveTransactionLine(CSQLAuditRecord& AuditRecord, CString strText, bool bUseFiledTime);

private:
	void CreatePendingPebbleLine();

private:
	CString m_strAuditFilename;
	CString m_strError;

private:
	CSQLRowAuditPendingExport m_RowPending;
	CSQLTranAuditPendingExportHelperPebble m_PebbleHelper;
};

/**********************************************************************/
