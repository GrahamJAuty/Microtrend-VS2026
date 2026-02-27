#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowExternalAccount.h"
//**********************************************************************

struct RSParams_ExternalAccount_NormalNoParams
{
public:
	RSParams_ExternalAccount_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_ExternalAccount_NormalByUsername
{
public:
	RSParams_ExternalAccount_NormalByUsername(CString str) 
		: strUsername(str) {}

public:
	CString strUsername;
};

//**********************************************************************

struct RSParams_ExternalAccount_NormalByUserId
{
public:
	RSParams_ExternalAccount_NormalByUserId(CString str) 
		: strUserId(str) {}

public:
	CString strUserId;
};

//**********************************************************************

class CSQLRecordSetMetadataExternalAccount : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataExternalAccount(bool bForExistenceCheck = FALSE);
	void AddParamsByUsername(const CString strUsername);
	void AddParamsByUserID(const CString strUserID);
	
public:
	CString m_strAESUsername;
	CString m_strPasswordHash;
	CString m_strAESUserID;
};

//**********************************************************************

class CSQLRecordSetExternalAccount : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetExternalAccount(CDatabase* pDatabase, RSParams_ExternalAccount_NormalNoParams params);
	CSQLRecordSetExternalAccount(CDatabase* pDatabase, RSParams_ExternalAccount_NormalByUsername params, bool bForExistenceCheck = FALSE );
	CSQLRecordSetExternalAccount(CDatabase* pDatabase, RSParams_ExternalAccount_NormalByUserId params);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromExternalAccountRow(CSQLRowExternalAccount& RowAccount);
	void SaveToExternalAccountRow(CSQLRowExternalAccount& RowAccount);

public:
	bool StepSelectAll(CSQLRowExternalAccount& RowAccount);

private:
	void CommonInit();	

public:
	CSQLRecordSetMetadataExternalAccount m_MD;
};

//**********************************************************************

