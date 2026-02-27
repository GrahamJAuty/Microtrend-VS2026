#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLRecordSetConnectionPool.h"
//**********************************************************************

class CSQLRecordSetMetadataAccountSurname : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAccountSurname();

public:
	__int64 m_nUserID;
	CString m_strUsername;
	CString m_strForename;
	CString m_strSurname;
	int m_nGroupNo;
	int m_nGroupShiftStatus;
	CString m_strGroupShiftStartDate;
	CString m_strGroupShiftEndDate;
	int m_nGroupShiftFutureGroup;
};

//**********************************************************************

class CSQLRecordSetAccountSurname : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountSurname(CDatabase* pDatabase);
	
public:
	virtual CString GetDefaultSQL();

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataAccountSurname m_MD;
};

//**********************************************************************

