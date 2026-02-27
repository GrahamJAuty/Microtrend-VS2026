#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowBromComBalanceChanges.h"
//**********************************************************************

struct RSParams_BromComBalanceChanges_NormalNoParams
{
public:
	RSParams_BromComBalanceChanges_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_BromComBalanceChanges_NormalByPersonId
{
public:
	RSParams_BromComBalanceChanges_NormalByPersonId(int n) 
		: nPersonID(n) {}

public:
	int nPersonID;
};

//**********************************************************************

struct RSParams_BromComBalanceChanges_CountRowsByRange
{
public:
	RSParams_BromComBalanceChanges_CountRowsByRange(int n1, int n2)
		: nPersonIDFrom(n1), nPersonIDTo(n2) {}

public:
	int nPersonIDFrom;
	int nPersonIDTo;
};

//**********************************************************************

class CSQLRecordSetMetadataBromComBalanceChanges : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataBromComBalanceChanges(bool bForExistenceCheck = FALSE);
	void AddParamsByPersonID(const int nPersonID);
	void AddParamsByRange(const int nPersonIDFrom, const int nPersonIDTo);

public:
	int m_nPersonID;
	__int64 m_nAccountID;
};

//**********************************************************************

class CSQLRecordSetBromComBalanceChanges : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetBromComBalanceChanges(CDatabase* pDatabase, RSParams_BromComBalanceChanges_NormalNoParams params);
	CSQLRecordSetBromComBalanceChanges(CDatabase* pDatabase, RSParams_BromComBalanceChanges_NormalByPersonId params, bool bForExistenceCheck = FALSE);
	CSQLRecordSetBromComBalanceChanges(CDatabase* pDatabase, RSParams_BromComBalanceChanges_CountRowsByRange params);

public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromBromComBalanceChangesRow(CSQLRowBromComBalanceChanges& RowBC);
	void SaveToBromComBalanceChangesRow(CSQLRowBromComBalanceChanges& RowBC);

public:
	bool StepSelectAll(CSQLRowBromComBalanceChanges& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataBromComBalanceChanges m_MD;
};

//**********************************************************************

