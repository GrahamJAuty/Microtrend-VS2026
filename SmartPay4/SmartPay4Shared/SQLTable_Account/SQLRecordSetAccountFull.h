#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLBuilderBind.h"
#include "..\SQLRecordSetConnectionPool.h"
#include "..\SQLDefines.h"
//**********************************************************************
#include "SQLRowAccountFind.h"
#include "SQLRowAccountFull.h"
//**********************************************************************

enum class RSDataType_AccountFull : int
{
	Normal = 0,
	Leaver = 1,
	GroupSet = 2
};

//**********************************************************************

struct RSParams_AccountFull_ById
{
public:
	RSParams_AccountFull_ById(CString str) : 
		strUserID(str) {}

public:
	CString strUserID;
};

//**********************************************************************

struct RSParams_AccountFull_CountRowsByRange
{
public:
	RSParams_AccountFull_CountRowsByRange(CString str1, CString str2) :
		strUserIDFrom(str1), strUserIDTo(str2) {}

public:
	CString strUserIDFrom;
	CString strUserIDTo;
};

//**********************************************************************

struct RSParams_AccountFull_NoParams
{
public:
	RSParams_AccountFull_NoParams() {}
};

//**********************************************************************

struct RSParams_AccountFull_Backup
{
public:
	RSParams_AccountFull_Backup() {}
};

//**********************************************************************

struct RSParams_AccountFull_ByParams
{
public:
	RSParams_AccountFull_ByParams(CSQLBuilderBind* p, CString str) :
		pBind(p), strWhere(str) {}

public:
	CSQLBuilderBind* pBind;
	CString strWhere;
};

//**********************************************************************

struct CSQLServerSearchInfo
{
public:
	CSQLServerSearchInfo();

public:
	CString m_strForename;
	CString m_strSurname;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strInfo5;
	CString m_strInfo6;
	CString m_strInfo7;
	CString m_strInfo8;
	__int64 m_nStartID;
	int m_nGroupType;
	int m_nMaxToFind;
};

//**********************************************************************

class CSQLRecordSetMetadataAccountFull : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAccountFull(bool bGroupSetMode = FALSE, bool bForExistenceCheck = FALSE);
	~CSQLRecordSetMetadataAccountFull(){}
	void AddNoParams();
	void AddParamsByUserID(const CString strUserID);
	void AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo);
	
private:
	CString ACLabel(CString str);

public:
	int m_nLeaverYear;
	__int64 m_nUserID;
	CString m_strUsername;
	CString m_strForename;
	CString m_strSurname;
	CString m_strDOB;
	CString m_strExpiryDate;
	CString m_strGender;
	int m_nGroupNo;
	CString m_strDayGroups;
	int m_nGroupShiftStatus;
	CString m_strGroupShiftStartDate;
	CString m_strGroupShiftEndDate;
	int m_nGroupShiftFutureGroup;
	CString m_strEmail;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strInfo5;
	CString m_strInfo6;
	CString m_strInfo7;
	CString m_strInfo8;
	BOOL m_bTick1;
	BOOL m_bTick2;
	BOOL m_bTick3;
	BOOL m_bTick4;
	int m_nAllergyCodes;
	CString m_strDietary;
	BOOL m_bInactive;
	/****/
	CString m_strMemberID;
	CString m_strExternalRef1;
	CString m_strExternalRef2;
	__int64 m_nPreviousUserID;
	__int64 m_nNextUserID;
	CString m_strLegacyPebblePurseID;
	/****/
	int m_nPurse1Allow;
	CString m_strPurse1Liability;
	CString m_strPurse1Credit;
	CString m_strPurse1LastSpendDate;
	CString m_strPurse1LastSpendTime;
	CString m_strPurse1LastSpend;
	CString m_strPurse1SpendToDate;
	/****/
	int m_nPurse2Allow;
	CString m_strPurse2Balance;
	CString m_strPurse2LastSpendDate;
	CString m_strPurse2LastSpendTime;
	CString m_strPurse2LastSpend;
	CString m_strPurse2SpendToDate;
	int m_nPurse2RefreshFlags;
	CString m_strPurse2RefreshedDate;
	CString m_strPurse2RefreshedTime;
	/****/
	int m_nPurse3Allow;
	CString m_strPurse3Liability;
	CString m_strPurse3Credit;
	CString m_strPurse3LastSpendDate;
	CString m_strPurse3LastSpendTime;
	CString m_strPurse3LastSpend;
	CString m_strPurse3SpendToDate;
	/****/
	CString m_strCashSpendToDate;
	/****/
	int m_nPoints;
	int m_nPointsToDate;
	/****/
	BOOL m_bOwnMaxSpend;
	/****/
	CString m_strMaxSpend[2][6];
	CString m_strMaxOverdraft[2][6];
	/****/
	int m_nAlertCode;
	CString m_strAlertDate;
	CString m_strAlertTime;
	/*****/
	int m_nGroupSet;

public:
	bool m_bGroupSetMode;
	bool m_bForExistenceCheck;
};

//**********************************************************************

class CSQLRecordSetAccountFull : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_NoParams params, bool bGroupSetMode);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_Backup params);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ById params, bool bForExistenceCheck = FALSE);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_CountRowsByRange params);
	CSQLRecordSetAccountFull(CDatabase* pDatabase, RSParams_AccountFull_ByParams params);

public:
	virtual CString GetDefaultSQL() { return m_strMyDefaultSQL; }
	void DoFieldExchange(CFieldExchange* pFX);

public:
	void LoadFromAccountRow(CSQLRowAccountFull& RowAccount);
	void SaveToAccountRow(CSQLRowAccountFull& RowAccount);
	void SaveToAccountRow(CSQLRowAccountFind& RowAccount);

public:
	bool StepSelectAll(CSQLRowAccountFull& RowAccount);
	bool StepSelectAll(CSQLRowAccountFull& RowAccount, int& nGroupSet);
	bool StepSelectAll(CSQLRowAccountFind& RowAccount);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataAccountFull m_MD;

private:
	CByteArray m_arrayRowVersion;
	CString m_strAccountTableName;
	CString m_strMyDefaultSQL;

private:
	bool m_bGroupSetMode;
	bool m_bForExistenceCheck;
};

//**********************************************************************

