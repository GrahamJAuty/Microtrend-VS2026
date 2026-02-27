#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPurchaseControlRule.h"
//**********************************************************************

struct RSParams_PurchaseControlRule_NormalNoParams
{
public:
	RSParams_PurchaseControlRule_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PurchaseControlRule_NormalByRuleId
{
public:
	RSParams_PurchaseControlRule_NormalByRuleId(int n)
		: nRuleId(n) {
	}

public:
	int nRuleId;
};

//**********************************************************************

class CSQLRecordSetMetadataPurchaseControlRule : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPurchaseControlRule(bool bForExistenceCheck = FALSE);
	void AddParamsByRuleID(const int nRuleID);

public:
	int m_nRuleID;
	BOOL m_bEnabled;
	int m_nThreshold;
	int m_nExpiryDays;
	CString m_strDescription;
};

//**********************************************************************

class CSQLRecordSetPurchaseControlRule : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPurchaseControlRule(CDatabase* pDatabase, RSParams_PurchaseControlRule_NormalNoParams params);
	CSQLRecordSetPurchaseControlRule(CDatabase* pDatabase, RSParams_PurchaseControlRule_NormalByRuleId params, bool bForExistenceCheck = FALSE);
	
public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPurchaseControlRuleRow(CSQLRowPurchaseControlRule& RowOS);
	void SaveToPurchaseControlRuleRow(CSQLRowPurchaseControlRule& RowOS);

public:
	bool StepSelectAll(CSQLRowPurchaseControlRule& RowOS);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPurchaseControlRule m_MD;
};

//**********************************************************************

