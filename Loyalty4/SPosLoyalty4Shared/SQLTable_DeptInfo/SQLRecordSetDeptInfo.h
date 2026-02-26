#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowDeptInfo.h"
//**********************************************************************

struct RSParams_DeptInfo_NormalNoParams
{
public:
	RSParams_DeptInfo_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_DeptInfo_NormalByDbNo
{
public:
	RSParams_DeptInfo_NormalByDbNo(int n) 
		: nDbNo(n) {}

public:
	int nDbNo;
};

//**********************************************************************

struct RSParams_DeptInfo_NormalByDbNoDeptNo
{
public:
	RSParams_DeptInfo_NormalByDbNoDeptNo(int n1, int n2) 
		: nDbNo(n1), nDeptNo(n2) {}

public:
	int nDbNo;
	int nDeptNo;
};

//**********************************************************************

class CSQLRecordSetMetadataDeptInfo : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataDeptInfo();
	void AddParamsByDbNo(const int nDbNo);
	void AddParamsByDbNoDeptNo(const int nDbNo, const int nDeptNo);

public:
	int m_nDbNo;
	int m_nDeptNo;
	CString m_strName;
	CString m_strLoyaltyBonusTrip;
	int m_nLoyaltyBonusType;
	int m_nLoyaltyBonusPoints;
	CString m_strLoyaltyBonusValue;
	int m_nLoyaltyBonusPercent;
	BOOL m_bGeneralSpendBonusFlag;
};

//**********************************************************************

class CSQLRecordSetDeptInfo : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetDeptInfo(CDatabase* pDatabase, RSParams_DeptInfo_NormalNoParams params);
	CSQLRecordSetDeptInfo(CDatabase* pDatabase, RSParams_DeptInfo_NormalByDbNo params);
	CSQLRecordSetDeptInfo(CDatabase* pDatabase, RSParams_DeptInfo_NormalByDbNoDeptNo params);
	
public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromDeptInfoRow(CSQLRowDeptInfo& RowDept);
	void SaveToDeptInfoRow(CSQLRowDeptInfo& RowDept);

public:
	bool StepSelectAll(CSQLRowDeptInfo& RowDept);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataDeptInfo m_MD;
};

//**********************************************************************

