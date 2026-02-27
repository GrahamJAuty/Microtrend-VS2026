#pragma once
/**********************************************************************/
#include "TLogCache.h"
#include "SQLTranAuditPendingExportHelperPebble.h"
#include "SQLTranAuditPendingExportHelperBromCom.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_AuditPendingExport\SQLRowAuditPendingExport.h"
#include "..\SmartPay4Shared\SQLTranBase.h"
/**********************************************************************/

class CSQLTranAuditPendingExport: public CSQLTranBase
{
public:
	CSQLTranAuditPendingExport(CString strAuditFilename);

public:
	void DoWork();
	void AfterTransaction();

public:
	CString GetError() { return m_strError; }

private:
	void DoWorkInternal();
	int CheckTopUp(CSQLAuditRecord& AuditRecord);
	int CheckSpend(CSQLAuditRecord& AuditRecord);
	int CheckAdjustment(CSQLAuditRecord& AuditRecord);
	int CheckPebbleOther(CSQLAuditRecord& AuditRecord);
	
private:
	bool SaveTransactionLine(CSQLAuditRecord& AuditRecord, CString strText, bool bUseFiledTime);
	void GetPurchaseText(CSQLAuditRecord& AuditRecord, CString& strPurchaseText);
	void ExtractPurchaseText(CSQLAuditRecord& AuditRecord, CTLogDatabase* pDatabase, bool bNewDatabase, CString& strPurchaseText);
	CString GetHistoryFilename(CString strUserID);

private:
	void CreatePendingPebbleLine();
	void CreatePendingBromComLine();

private:
	CString m_strAuditFilename;
	CString m_strError;

private:
	int m_nActionType;

private:
	CTLogCache m_TLogCache;
	CSQLRowAuditPendingExport m_RowPending;
	CSQLTranAuditPendingExportHelperPebble m_PebbleHelper;
	CSQLTranAuditPendingExportHelperBromCom m_BromComHelper;
};

/**********************************************************************/
