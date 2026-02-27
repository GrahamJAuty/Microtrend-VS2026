#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebbleExportTransaction.h"
//**********************************************************************

struct RSParams_PebbleExportTransaction_NormalNoParams
{
public:
	RSParams_PebbleExportTransaction_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebbleExportTransaction_NormalByLineId
{
public:
	RSParams_PebbleExportTransaction_NormalByLineId(CString str)
		: strLineId(str) {
	}

public:
	CString strLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataPebbleExportTransaction : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebbleExportTransaction(bool bForExistenceCheck = FALSE);
	void AddParamsByLineID(const CString strLineID);

public:
	__int64 m_nLineID;
	__int64 m_nUserID;
	__int64 m_nXFerUserID;
	CString m_strMemberID;
	CString m_strDate;
	CString m_strTime;
	int m_nCCNo;
	int m_nSourceType;
	int m_nAppNo;
	CString m_strAmount;
	CString m_strPurse1;
	CString m_strPurse2;
	CString m_strCash;
	CString m_strWebComment;
};

//**********************************************************************

class CSQLRecordSetPebbleExportTransaction : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebbleExportTransaction(CDatabase* pDatabase, RSParams_PebbleExportTransaction_NormalNoParams params);
	CSQLRecordSetPebbleExportTransaction(CDatabase* pDatabase, RSParams_PebbleExportTransaction_NormalByLineId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebbleExportTransactionRow(CSQLRowPebbleExportTransaction& RowBC);
	void SaveToPebbleExportTransactionRow(CSQLRowPebbleExportTransaction& RowBC);

public:
	bool StepSelectAll(CSQLRowPebbleExportTransaction& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPebbleExportTransaction m_MD;
};

//**********************************************************************

