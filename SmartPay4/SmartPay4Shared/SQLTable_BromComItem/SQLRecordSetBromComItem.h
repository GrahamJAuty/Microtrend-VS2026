#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowBromComItem.h"
//**********************************************************************

struct RSParams_BromComItem_NormalNoParams
{
public:
	RSParams_BromComItem_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_BromComItem_NormalByOrderId
{
public:
	RSParams_BromComItem_NormalByOrderId(int n) 
		: nOrderID(n) {}

public:
	int nOrderID;
};

//**********************************************************************

struct RSParams_BromComItem_NormalByOrderIdPersonId
{
public:
	RSParams_BromComItem_NormalByOrderIdPersonId(int n1, int n2) 
		: nOrderID(n1), nPersonID(n2) {}

public:
	int nOrderID;
	int nPersonID;
};

//**********************************************************************

class CSQLRecordSetMetadataBromComItem : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataBromComItem(bool bForExistenceCheck = FALSE);
	void AddParamsByOrderID(const int nOrderID);
	void AddParamsByOrderIDPersonID(const int nOrderID, const int nPersonID);

public:
	int m_nOrderID;
	int m_nPersonID;
	CString m_strAmount;
	CString m_strStatus;
};

//**********************************************************************

class CSQLRecordSetBromComItem : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetBromComItem(CDatabase* pDatabase, RSParams_BromComItem_NormalNoParams params);
	CSQLRecordSetBromComItem(CDatabase* pDatabase, RSParams_BromComItem_NormalByOrderId params);
	CSQLRecordSetBromComItem(CDatabase* pDatabase, RSParams_BromComItem_NormalByOrderIdPersonId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromBromComItemRow(CSQLRowBromComItem& RowBC);
	void SaveToBromComItemRow(CSQLRowBromComItem& RowBC);

public:
	bool StepSelectAll(CSQLRowBromComItem& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataBromComItem m_MD;
};

//**********************************************************************
