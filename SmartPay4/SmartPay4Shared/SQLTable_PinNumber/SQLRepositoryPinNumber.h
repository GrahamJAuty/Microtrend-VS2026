#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRecordSetPINNumber.h"
//**********************************************************************

class CSQLRepositoryPINNumber
{
public:
	CSQLRepositoryPINNumber();

public:
	CSQLResultInfo SelectRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase);
	CSQLResultInfo SelectRowByUserID(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowPINNumber& RowPIN, CDatabase* pDatabase);
	CSQLResultInfo RowExists(CString strPINNumber, CDatabase* pDatabase);	

	CSQLResultInfo CountRows(CDatabase* pDatabase);
	CSQLResultInfo CountRowsByRange(CString strPINNumberFrom, CString strPINNumberTo, CDatabase* pDatabase);

	bool LinkAccountIDToPINNumber(CString strPINNumber, __int64 nUserID, CDatabase* pDatabase);
	bool RemoveAccountIDLink(__int64 nUserID, CDatabase* pDatabase);
	CString GetNewPINNumber(CString& strError, CDatabase* pDatabase);
	void UpdatePINNumberUsage(CString strPINNumber, CDatabase* pDatabase);

private:
	bool LinkAccountIDToPINNumberInternal(CString strPINNumber, __int64 nUserID, CDatabase* pDatabase);
	bool RemoveAccountIDLinkInternal(__int64 nUserID, CDatabase* pDatabase);
	CString GetNewPINNumberInternal(CString& strError, CDatabase* pDatabase);
	void UpdatePINNumberUsageInternal(CString strPINNumber, CDatabase* pDatabase);
};

//**********************************************************************
