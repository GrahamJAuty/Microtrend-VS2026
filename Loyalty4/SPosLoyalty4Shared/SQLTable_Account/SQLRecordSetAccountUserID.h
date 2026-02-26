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
	WithQRInfo = 1,
	WithExtRef = 2
};

//**********************************************************************

struct RSParams_AccountUserId_NormalByParams
{
public:
	RSParams_AccountUserId_NormalByParams(CSQLBuilderBind* p, CString str)
		: pBind(p), strWhere(str) {}

public:
	CSQLBuilderBind* pBind;
	CString strWhere;
};

//**********************************************************************

struct RSParams_AccountUserId_NormalByRange
{
public:
	RSParams_AccountUserId_NormalByRange(CString str1, CString str2)
		: strFrom(str1), strTo(str2) {}

public:
	CString strFrom;
	CString strTo;
};

//**********************************************************************

struct RSParams_AccountUserId_NormalByInfo2
{
public:
	RSParams_AccountUserId_NormalByInfo2(CString str) 
		: strInfo2(str) {}

public:
	CString strInfo2;
};

//**********************************************************************

struct RSParams_AccountUserId_NormalByExtRef
{
public:
	RSParams_AccountUserId_NormalByExtRef(CString str) 
		: strExtRef(str) {}

public:
	CString strExtRef;
};

//**********************************************************************

struct RSParams_AccountUserId_NormalByMemberID
{
public:
	RSParams_AccountUserId_NormalByMemberID(CString str) 
		: strMemberID(str) {}

public:
	CString strMemberID;
};

//**********************************************************************

struct RSParams_AccountUserId_NormalNoParams
{
public:
	RSParams_AccountUserId_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_AccountUserId_ResetExtRefByExtRef
{
public:
	RSParams_AccountUserId_ResetExtRefByExtRef(CString str) 
		: strExtRef(str) {}

public:
	CString strExtRef;
};

//**********************************************************************

struct RSParams_AccountUserId_ResetQRInfoByQRInfo
{
public:
	RSParams_AccountUserId_ResetQRInfoByQRInfo(CString str)
		: strQRInfo(str) {}

public:
	CString strQRInfo;
};

//**********************************************************************

class CSQLRecordSetMetadataAccountUserID : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAccountUserID(RSDataType_AccountUserId type);
	void AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo);
	void AddParamsByInfo2(const CString strInfo2);
	void AddParamsByExtRef(const CString strExtRef);
	void AddParamsByQRInfo(const CString strQRInfo);
	void AddParamsByMemberId(const CString strMemberId);
	
public:
	__int64 m_nUserID;
	CString m_strMemberID;
};

//**********************************************************************

class CSQLRecordSetAccountUserID : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByParams Params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByRange Params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByInfo2 Params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByExtRef Params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalByMemberID Params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_NormalNoParams Params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetExtRefByExtRef Params);
	CSQLRecordSetAccountUserID(CDatabase* pDatabase, RSParams_AccountUserId_ResetQRInfoByQRInfo Params);
	
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

