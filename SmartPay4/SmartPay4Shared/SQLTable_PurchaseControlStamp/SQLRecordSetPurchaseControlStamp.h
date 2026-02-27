#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPurchaseControlStamp.h"
//**********************************************************************

struct RSParams_PurchaseControlStamp_NormalNoParams
{
public:
	RSParams_PurchaseControlStamp_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PurchaseControlStamp_NormalByUserID
{
public:
	RSParams_PurchaseControlStamp_NormalByUserID(CString str1)
		: strUserID(str1) {}

public:
	CString strUserID;
};

//**********************************************************************

struct RSParams_PurchaseControlStamp_CountRowsByUserID
{
public:
	RSParams_PurchaseControlStamp_CountRowsByUserID(CString str1)
		: strUserID(str1) {
	}

public:
	CString strUserID;
};

//**********************************************************************

struct RSParams_PurchaseControlStamp_NormalByUserIDRuleID
{
public:
	RSParams_PurchaseControlStamp_NormalByUserIDRuleID(CString str1, int n1) 
		: strUserID(str1), nRuleID(n1) {}

public:
	CString strUserID;
	int nRuleID;
};

//**********************************************************************

struct RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates
{
public:
	RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates(CString str1, int n1, CString str2, CString str3)
		: strUserID(str1), nRuleID(n1), strAwardDate(str2), strExpireDate(str3) {}

public:
	CString strUserID;
	int nRuleID;
	CString strAwardDate;
	CString strExpireDate;
};

//**********************************************************************

class CSQLRecordSetMetadataPurchaseControlStamp : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPurchaseControlStamp(bool bForExistenceCheck = FALSE);
	void AddParamsByUserID(const CString strUserID);
	void AddParamsForRowCount(const CString strUserID);
	void AddParamsByUserIDRuleID(const CString strUserID, const int nRuleID);
	void AddParamsByUserIDRuleIDDates(const CString strUserID, const int nRuleID, const CString strAwardDate, const CString strExpireDate);

public:
	__int64 m_nUserID;
	int m_nRuleID;
	CString m_strAwardDate;
	CString m_strExpireDate;
	int m_nStampCount;
};

//**********************************************************************

class CSQLRecordSetPurchaseControlStamp : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalNoParams params);
	CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalByUserID params);
	CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_CountRowsByUserID params);
	CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalByUserIDRuleID params);
	CSQLRecordSetPurchaseControlStamp(CDatabase* pDatabase, RSParams_PurchaseControlStamp_NormalByUserIDRuleIDDates params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPurchaseControlStampRow(CSQLRowPurchaseControlStamp& RowOS);
	void SaveToPurchaseControlStampRow(CSQLRowPurchaseControlStamp& RowOS);

public:
	bool StepSelectAll(CSQLRowPurchaseControlStamp& RowOS);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPurchaseControlStamp m_MD;
};

//**********************************************************************

