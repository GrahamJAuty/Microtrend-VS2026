#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowBromComPendingItem.h"
//**********************************************************************

struct RSParams_BromComPendingItem_NormalNoParams
{
public:
	RSParams_BromComPendingItem_NormalNoParams(){}
};

//**********************************************************************

struct RSParams_BromComPendingItem_NormalByOrderId
{
public:
	RSParams_BromComPendingItem_NormalByOrderId(int n) 
		: nOrderID(n) {}

public:
	int nOrderID;
};

//**********************************************************************

struct RSParams_BromComPendingItem_NormalByOrderIdPersonId
{
public:
	RSParams_BromComPendingItem_NormalByOrderIdPersonId(int n1, int n2) 
		: nOrderID(n1), nPersonID(n2) {}

public:
	int nOrderID;
	int nPersonID;
};

//**********************************************************************

class CSQLRecordSetMetadataBromComPendingItem : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataBromComPendingItem(bool bForExistenceCheck = FALSE);
	void AddParamsByOrderID(const int nOrderID);
	void AddParamsByOrderIDPersonID(const int nOrderID, const int nPersonID);

public:
	int m_nOrderID;
	int m_nPersonID;
	CString m_strAmount;
};

//**********************************************************************

class CSQLRecordSetBromComPendingItem : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetBromComPendingItem(CDatabase* pDatabase, RSParams_BromComPendingItem_NormalNoParams params);
	CSQLRecordSetBromComPendingItem(CDatabase* pDatabase, RSParams_BromComPendingItem_NormalByOrderId params);
	CSQLRecordSetBromComPendingItem(CDatabase* pDatabase, RSParams_BromComPendingItem_NormalByOrderIdPersonId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromBromComPendingItemRow(CSQLRowBromComPendingItem& RowBC);
	void SaveToBromComPendingItemRow(CSQLRowBromComPendingItem& RowBC);

public:
	bool StepSelectAll(CSQLRowBromComPendingItem& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataBromComPendingItem m_MD;
};

//**********************************************************************

