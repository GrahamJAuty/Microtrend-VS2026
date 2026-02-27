#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPluInfo.h"
//**********************************************************************

struct RSParams_PluInfo_NormalNoParams
{
public:
	RSParams_PluInfo_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PluInfo_NormalByPluNo
{
public:
	RSParams_PluInfo_NormalByPluNo(CString str)
		: strPluNo(str){}

public:
	CString strPluNo;
};

//**********************************************************************

class CSQLRecordSetMetadataPluInfo : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPluInfo(bool bForExistenceCheck = FALSE);
	void AddParamsByPluNo(const CString strPluNo);

public:
	__int64 m_nPluNo;
	CString m_strDescription;
	int m_nDeptNo;
	CString m_strTaxCode;
	int m_nModType;
	int m_nPoints;
	int m_nPCtrlRuleID;
	int m_nStampWeight;
};

//**********************************************************************

class CSQLRecordSetPluInfo : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalNoParams params);
	CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalByPluNo params, bool bForExistenceCheck = FALSE);
	
public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPluInfoRow(CSQLRowPluInfo& RowPlu);
	void SaveToPluInfoRow(CSQLRowPluInfo& RowPlu);

public:
	bool StepSelectAll(CSQLRowPluInfo& RowPlu);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPluInfo m_MD;
};

//**********************************************************************

