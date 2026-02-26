#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowGroup.h"
//**********************************************************************

struct RSParams_Group_NormalNoParams
{
public:
	RSParams_Group_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_Group_NormalByGroupNo
{
public:
	RSParams_Group_NormalByGroupNo(int n) 
		: nGroupNo(n) {}

public:
	int nGroupNo;
};

//**********************************************************************

class CSQLRecordSetMetadataGroup : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataGroup(bool bForExistenceCheck = FALSE);
	void AddParamsByGroupNo(const int nGroupNo);

public:
	int m_nGroupNo;
	CString m_strGroupName;
	int m_nSchemeNo;
	BOOL m_bStampOfferFlag;
	/****/
	int m_nRefreshType;
	int m_nRefreshFlags;
	CString m_strRefreshValue;
	int m_nRefreshAppend;
	/****/
	int m_nPurse1Allowed;
	int m_nPurse2Allowed;
	/****/
	BOOL m_bNoPluBonus;
	BOOL m_bNoDeptBonus;
	/****/
	int m_nMaxSpendType;
	/****/
	CString m_strMaxSpend[6];
	CString m_strMaxOverdraft[6];
	/****/
	BOOL m_bRewardFlag;
	BOOL m_bRewardDisable;
	int m_nRewardType;
	CString m_strRewardTrip;
	int m_nRewardPoints;
	CString m_strRewardBonus;
	CString m_strRewardLimit;
	CString m_strRewardTrigger;
	/*****/
	BOOL m_bRedeemFlag;
	BOOL m_bRedeemDisable;
	int m_nRedeemType;
	int m_nRedeemTrip;
	CString m_strRedeemValue;
};

//**********************************************************************

class CSQLRecordSetGroup : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetGroup(CDatabase* pDatabase, RSParams_Group_NormalNoParams params);
	CSQLRecordSetGroup(CDatabase* pDatabase, RSParams_Group_NormalByGroupNo params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromGroupRow(CSQLRowGroup& RowGroup);
	void SaveToGroupRow(CSQLRowGroup& RowGroup);

public:
	bool StepSelectAll(CSQLRowGroup& RowGroup);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataGroup m_MD;
};

//**********************************************************************

