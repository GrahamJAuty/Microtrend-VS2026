#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLBuilderBind.h"
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************

enum class RSDataType_AccountUserId : int
{
	Normal = 0,
	ResetMemberId = 1,
	ResetExtRef1 = 2,
	ResetExtRef2 = 3,
};

//**********************************************************************

struct RSParams_AccountUserId_NormalNoParams
{
public:
	RSParams_AccountUserId_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_AccountUserId_DetailByRange
{
public:
	RSParams_AccountUserId_DetailByRange(CString str1, CString str2) : 
		strUserIDFrom(str1), strUserIDTo(str2) {}

public:
	CString strUserIDFrom;
	CString strUserIDTo;
};

//**********************************************************************

struct RSParams_AccountUserId_NormalByParams
{
public:
	RSParams_AccountUserId_NormalByParams(CSQLBuilderBind* p, CString str) : 
		pBind(p), strWhere(str) {}

public:
	CSQLBuilderBind* pBind;
	CString strWhere;
};

//**********************************************************************

struct RSParams_AccountUserId_NormalByMemberId
{
public:
	RSParams_AccountUserId_NormalByMemberId(CString str) : 
		strMemberID(str) {}

public:
	CString strMemberID;
};

//**********************************************************************

struct RSParams_AccountUserId_ResetMemberIdByMemberId
{
public:
	RSParams_AccountUserId_ResetMemberIdByMemberId(CString str) :
		strMemberID(str) {
	}

public:
	CString strMemberID;
};

//**********************************************************************

struct RSParams_AccountUserId_ResetExtRef1ByExtRef1
{
public:
	RSParams_AccountUserId_ResetExtRef1ByExtRef1(CString str) :
		strExtRef1(str) {}

public:
	CString strExtRef1;
};

//**********************************************************************

struct RSParams_AccountUserId_ResetExtRef2ByExtRef2
{
public:
	RSParams_AccountUserId_ResetExtRef2ByExtRef2(CString str) :
		strExtRef2(str) {}

public:
	CString strExtRef2;
};

//**********************************************************************

class CSQLRecordSetMetadataAccountUserID : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAccountUserID(RSDataType_AccountUserId type);
	void AddNoParams();
	void AddParamsByMemberID(const CString strMemberID);
	void AddParamsByExtRef1(const CString strExtRef1);
	void AddParamsByExtRef2(const CString strExtRef2);
	void AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo);

public:
	__int64 m_nUserID;
	CString m_strMemberID;
};

//**********************************************************************

class CSQLRecordSetAccountUserID : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalNoParams params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_DetailByRange params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByParams params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByMemberId params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetMemberIdByMemberId params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetExtRef1ByExtRef1 params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetExtRef2ByExtRef2 params);

public:
	virtual CString GetDefaultSQL();
	
public:
	bool StepSelect(__int64& nUserID, bool bUpdate = FALSE);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataAccountUserID m_MD;
};

//**********************************************************************

