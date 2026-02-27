#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowPurchaseControlStamp.h"
//**********************************************************************

class CSQLRepositoryPurchaseControlStamp
{
public:
	CSQLRepositoryPurchaseControlStamp();

public:
	CSQLResultInfo SelectRow(CSQLRowPurchaseControlStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPurchaseControlStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPurchaseControlStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo UpsertRow(CSQLRowPurchaseControlStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPurchaseControlStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CSQLRowPurchaseControlStamp& SQLRowOS, CDatabase* pDatabase);
	
public:
	void GetStampCounts(__int64 nUserID, CReportConsolidationArray<CPurchaseControlRSPBuffer>& arrayPurchaseControlStamps);
	void GetPurchaseControlStampsByUserID(__int64 nUserID, CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp>& arrayStamps, CDatabase* pDatabase = NULL);
	void GetPurchaseControlStampsByUserIDAndRuleID(__int64 nUserID, int nRuleID, CArray<CSQLRowPurchaseControlStamp, CSQLRowPurchaseControlStamp>& arrayStamps);
	CSQLResultInfo CountRowsByUserID(__int64 nUserID, CDatabase* pDatabase);
};

//**********************************************************************
