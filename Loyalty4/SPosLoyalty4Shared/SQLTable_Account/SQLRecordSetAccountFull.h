#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLBuilderBind.h"
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
#include "..\SQLRecordSetMetadata.h"
//**********************************************************************
#include "SQLRowAccountFind.h"
#include "SQLRowAccountFull.h"
//**********************************************************************

enum class RSDataType_AccountFull : int
{
	Normal = 0,
	Export = 1,
};

//**********************************************************************

struct RSParams_AccountFull_NormalById 
{
public:
	RSParams_AccountFull_NormalById(CString str) :
		strUserID(str) {}

public:
	CString strUserID;
};

//**********************************************************************

struct RSParams_AccountFull_NormalByRange
{
public:
	RSParams_AccountFull_NormalByRange(CString str1, CString str2) :
		strUserIDFrom(str1), strUserIDTo(str2) {}

public:
	CString strUserIDFrom;
	CString strUserIDTo;
};

//**********************************************************************

struct RSParams_AccountFull_RowCountByRange
{
public:
	RSParams_AccountFull_RowCountByRange(CString str1, CString str2) :
		strUserIDFrom(str1), strUserIDTo(str2) {}

public:
	CString strUserIDFrom;
	CString strUserIDTo;
};

//**********************************************************************

struct RSParams_AccountFull_NormalByParams
{
public:
	RSParams_AccountFull_NormalByParams(CSQLBuilderBind* pBind, CString str)
		: pBind(pBind), strWhere(str) {}

public:
	CSQLBuilderBind* pBind;
	CString strWhere;
};

//**********************************************************************

struct RSParams_AccountFull_ExportByParams
{
public:
	RSParams_AccountFull_ExportByParams(CSQLBuilderBind* pBind, CString str)
		: pBind(pBind), strWhere(str) {}

public:
	CSQLBuilderBind* pBind;
	CString strWhere;
};

//**********************************************************************

struct RSParams_AccountFull_NormalNoParams
{
public:
	RSParams_AccountFull_NormalNoParams() {}
};

//**********************************************************************

class CSQLRecordSetMetadataAccountFull : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAccountFull(RSDataType_AccountFull type, bool bForExistenceCheck = FALSE);
	void AddParamsById(const CString strUserID);
	void AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo);
	
public:
	__int64 m_nUserID;
	CString m_strFullname;
	CString m_strForename;
	CString m_strSurname;
	CString m_strDOB;
	CString m_strExpiryDate;
	CString m_strGender;
	int m_nGroupNo;
	int m_nSchemeNo;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strQRInfo;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strAddress4;
	CString m_strAddress5;
	CString m_strPhone1;
	CString m_strPhone2;
	BOOL m_bInactive;
	/****/
	CString m_strExternalRef;
	LONGLONG m_nPreviousUserID;
	LONGLONG m_nNextUserID;
	/****/
	CString m_strPurse1Balance;
	CString m_strPurse1LastSpendDate;
	CString m_strPurse1LastSpendTime;
	CString m_strPurse1LastSpend;
	CString m_strPurse1SpendToDate;
	/****/
	CString m_strPurse2Balance;
	CString m_strPurse2LastSpendDate;
	CString m_strPurse2LastSpendTime;
	CString m_strPurse2LastSpend;
	CString m_strPurse2SpendToDate;
	CString m_strPurse2RefreshedDate;
	CString m_strPurse2RefreshedTime;
	/****/
	CString m_strCashLastSpendDate;
	CString m_strCashLastSpendTime;
	CString m_strCashLastSpend;
	CString m_strCashSpendToDate;
	/****/
	int m_nPoints;
	int m_nPointsToDate;
	/****/
	CString m_strMaxSpend;
	CString m_strMaxOverdraft;
	/****/
	int m_nHotlistCode;
	CString m_strHotlistDate;
	CString m_strHotlistTime;
	/****/
	CString m_strAlertCodes;
	/****/
	CByteArray m_arrayRowVersion;
	/****/
	CString m_strSQLTotalSpendToDate;
};

//**********************************************************************

class CSQLRecordSetAccountFull : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalById Params, bool bForExistenceCheck = FALSE );
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalByRange Params);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_RowCountByRange Params);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalByParams Params);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ExportByParams Params);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NormalNoParams Params);

public:
	virtual CString GetDefaultSQL();
	void DoFieldExchange(CFieldExchange* pFX);

public:
	void LoadFromAccountRow(CSQLRowAccountFull& RowAccount);
	void SaveToAccountRow(CSQLRowAccountFull& RowAccount);
	void SaveToAccountRow(CSQLRowAccountFind& RowAccount);

public:
	bool StepSelectAll(CSQLRowAccountFull& RowAccount);
	bool StepSelectAll(CSQLRowAccountFind& RowFind);

public:
	CSQLRecordSetMetadataAccountFull m_MD;
	CByteArray m_arrayRowVersion;

private:
	void CommonInit();

private:
	bool m_bForExistenceCheck;
};

//**********************************************************************

