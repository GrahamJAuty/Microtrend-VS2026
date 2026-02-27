#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowBromComPendingOrder.h"
//**********************************************************************

struct RSParams_BromComPendingOrder_NormalNoParams
{
public:
	RSParams_BromComPendingOrder_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_BromComPendingOrder_NormalByOrderId
{
public:
	RSParams_BromComPendingOrder_NormalByOrderId(int n) 
		: nOrderID(n) {}

public:
	int nOrderID;
};

//**********************************************************************

struct RSParams_BromComPendingOrder_CountRowsByRange
{
public:
	RSParams_BromComPendingOrder_CountRowsByRange(int n1, int n2)
		: nOrderIDFrom(n1), nOrderIDTo(n2) {}

public:
	int nOrderIDFrom;
	int nOrderIDTo;
};

//**********************************************************************

class CSQLRecordSetMetadataBromComPendingOrder : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataBromComPendingOrder(bool bForExistenceCheck = FALSE);
	void AddParamsByOrderID(const int nOrderID);
	void AddParamsForRowCount(const int nOrderIDFrom, const int nOrderIDTo);
	
public:
	int m_nOrderID;
	CString m_strStatus;
	CString m_strLastModified;
	BOOL m_bNewOrder;
};

//**********************************************************************

class CSQLRecordSetBromComPendingOrder : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetBromComPendingOrder(CDatabase* pDatabase, RSParams_BromComPendingOrder_NormalNoParams params);
	CSQLRecordSetBromComPendingOrder(CDatabase* pDatabase, RSParams_BromComPendingOrder_NormalByOrderId params, bool bForExistenceCheck = FALSE);
	CSQLRecordSetBromComPendingOrder(CDatabase* pDatabase, RSParams_BromComPendingOrder_CountRowsByRange params);

public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromBromComPendingOrderRow(CSQLRowBromComPendingOrder& RowBC);
	void SaveToBromComPendingOrderRow(CSQLRowBromComPendingOrder& RowBC);

public:
	bool StepSelectAll(CSQLRowBromComPendingOrder& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataBromComPendingOrder m_MD;
};

//**********************************************************************

