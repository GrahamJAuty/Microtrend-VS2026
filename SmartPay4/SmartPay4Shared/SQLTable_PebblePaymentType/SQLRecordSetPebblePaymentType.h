#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebblePaymentType.h"
//**********************************************************************

struct RSParams_PebblePaymentType_NormalNoParams
{
public:
	RSParams_PebblePaymentType_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebblePaymentType_NormalByPaymentType
{
public:
	RSParams_PebblePaymentType_NormalByPaymentType(int n)
		: nPaymentType(n) {}

public:
	int nPaymentType;
};

//**********************************************************************

class CSQLRecordSetMetadataPebblePaymentType : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebblePaymentType(bool bForExistenceCheck = FALSE);
	void AddParamsByPaymentType(const int nPaymentType);

public:
	int m_nSPOSPaymentType;
	CString m_strPebblePaymentName;
	int m_nPebbleTranType;
};

//**********************************************************************

class CSQLRecordSetPebblePaymentType : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebblePaymentType(CDatabase* pDatabase, RSParams_PebblePaymentType_NormalNoParams);
	CSQLRecordSetPebblePaymentType(CDatabase* pDatabase, RSParams_PebblePaymentType_NormalByPaymentType, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebblePaymentTypeRow(CSQLRowPebblePaymentType& RowPB);
	void SaveToPebblePaymentTypeRow(CSQLRowPebblePaymentType& RowPB);

public:
	bool StepSelectAll(CSQLRowPebblePaymentType& RowPB);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPebblePaymentType m_MD;
};

//**********************************************************************

