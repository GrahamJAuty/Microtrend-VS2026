#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebbleExportTransactionSent.h"
//**********************************************************************

struct RSParams_PebbleTransactionSent_NormalNoParams
{
public:
	RSParams_PebbleTransactionSent_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebbleTransactionSent_NormalByLineId
{
public:
	RSParams_PebbleTransactionSent_NormalByLineId(CString str)
		: strLineId(str) {
	}

public:
	CString strLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataPebbleExportTransactionSent : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebbleExportTransactionSent(bool bRecordSetExists = FALSE);
	void AddParamsByLineID(const CString strLineID);

public:
	__int64 m_nLineID;
	__int64 m_nTranLineID;
};

//**********************************************************************

class CSQLRecordSetPebbleExportTransactionSent : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebbleExportTransactionSent(CDatabase* pDatabase, RSParams_PebbleTransactionSent_NormalNoParams);
	CSQLRecordSetPebbleExportTransactionSent(CDatabase* pDatabase, RSParams_PebbleTransactionSent_NormalByLineId, bool bRecordSetExists = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebbleExportTransactionSentRow(CSQLRowPebbleExportTransactionSent& RowBC);
	void SaveToPebbleExportTransactionSentRow(CSQLRowPebbleExportTransactionSent& RowBC);

public:
	bool StepSelectAll(CSQLRowPebbleExportTransactionSent& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPebbleExportTransactionSent m_MD;
};

//**********************************************************************

