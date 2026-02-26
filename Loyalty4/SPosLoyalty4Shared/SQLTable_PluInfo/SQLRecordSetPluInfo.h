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
	RSParams_PluInfo_NormalNoParams(){}
};

//**********************************************************************

struct RSParams_PluInfo_NormalByDbNo
{
public:
	RSParams_PluInfo_NormalByDbNo(int n) 
		: nDbNo(n) {}

public:
	int nDbNo;
};

//**********************************************************************

struct RSParams_PluInfo_NormalByDbNoPluNo
{
public:
	RSParams_PluInfo_NormalByDbNoPluNo(int n, CString str) 
		: nDbNo(n), strPluNo(str) {}

public:
	int nDbNo;
	CString strPluNo;
};

//**********************************************************************

class CSQLRecordSetMetadataPluInfo : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPluInfo(bool bForExistenceCheck = FALSE);
	void AddParamsByDbNo(const int nDbNo);
	void AddParamsByDbNoPluNo(const int nDbNo, const CString strPluNo);

public:
	int m_nDbNo;
	__int64 m_nPluNo;
	CString m_strDescription;
	int m_nDeptNo;
	CString m_strTaxCode;
	int m_nPoints;
	int m_nModType;
	int m_nStampOfferID;
};

//**********************************************************************

class CSQLRecordSetPluInfo : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalNoParams params);
	CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalByDbNo params);
	CSQLRecordSetPluInfo(CDatabase* pDatabase, RSParams_PluInfo_NormalByDbNoPluNo params, bool bForExistenceCheck = FALSE);
	
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

