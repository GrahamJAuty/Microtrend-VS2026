#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowStaffGiftException.h"
//**********************************************************************

struct RSParams_StaffGiftException_NormalNoParams
{
public:
	RSParams_StaffGiftException_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_StaffGiftException_NormalByLineId
{
public:
	RSParams_StaffGiftException_NormalByLineId(CString str)
		: strLineId(str) {}

public:
	CString strLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataStaffGiftException : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataStaffGiftException();
	void AddParamsByLineId( const CString strLineId);

public:
	__int64 m_nLineID;
	__int64 m_nCustomerID;
	int m_nServerNo;
	int m_nFolderIndexDbNo;
	int m_nFolderIndexSetNo;
	__int64 m_nServerID;
	CString m_strDate;
	CString m_strTime;
	CString m_strValue;
	int m_nErrorCode;
};

//**********************************************************************

class CSQLRecordSetStaffGiftException : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetStaffGiftException(CDatabase* pDatabase, RSParams_StaffGiftException_NormalNoParams params);
	CSQLRecordSetStaffGiftException(CDatabase* pDatabase, RSParams_StaffGiftException_NormalByLineId params);

public:
	virtual CString GetDefaultSQL();
	void DoFieldExchange(CFieldExchange* pFX);

public:
	void LoadFromStaffGiftExceptionRow(CSQLRowStaffGiftException& RowBC);
	void SaveToStaffGiftExceptionRow(CSQLRowStaffGiftException& RowBC);

public:
	bool StepSelectAll(CSQLRowStaffGiftException& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataStaffGiftException m_MD;
};

//**********************************************************************

