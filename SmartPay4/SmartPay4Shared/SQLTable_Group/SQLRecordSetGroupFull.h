#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowGroupFull.h"
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

struct RSParams_Group_CountRowsByRange
{
public:
	RSParams_Group_CountRowsByRange(int n1, int n2)
		: nGroupNoFrom(n1), nGroupNoTo(n2) {}

public:
	int nGroupNoFrom;
	int nGroupNoTo;
};

//**********************************************************************

class CSQLRecordSetMetadataGroupFull : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataGroupFull(bool bForExistenceCheck = FALSE);
	void AddParamsByGroupNo(const int nGroupNo);
	void AddParamsByRange(const int nGroupNoFrom, const int nGroupNoTo);

public:
	int m_nGroupNo;
	CString m_strGroupName;
	int m_nUserType;
	int m_nSetNo;
	CString m_strShiftGroups;
	int m_nPriceLevel;
	int m_nVATShiftFlag;
	BOOL m_bPurchaseControlFlag;
	BOOL m_bAllowRevalKioskFlag;
	/****/
	int m_nRefreshType;
	int m_nRefreshFlags;
	CString m_strRefreshValue;
	int m_nRefreshAppendFlag;
	int m_nRefreshTypePeriod1;
	int m_nRefreshTypePeriod2;
	int m_nRefreshTypePeriod3;
	int m_nRefreshTypePeriod4;
	int m_nRefreshTypePeriod5;
	int m_nRefreshTypePeriod6;
	CString m_strRefreshValuePeriod1;
	CString m_strRefreshValuePeriod2;
	CString m_strRefreshValuePeriod3;
	CString m_strRefreshValuePeriod4;
	CString m_strRefreshValuePeriod5;
	CString m_strRefreshValuePeriod6;
	/****/
	int m_nPurse1Allowed;
	int m_nPurse2Allowed;
	int m_nPurse3Allowed;
	/****/
	int m_nMaxSpendType;
	/****/
	CString m_strMaxSpend[2][6];
	CString m_strMaxOverdraft[2][6];
	/****/
	CString m_strDeptInhibitPurse1;
	CString m_strDeptInhibitPurse2;
	CString m_strDeptInhibitPurse3;
	CString m_strDeptInhibitCash;
	/****/
	CString m_strLowBalancePurse1;
	CString m_strLowBalancePurse2;
	CString m_strLowBalancePurse3;
	/****/
	int m_nPointsAchievement;
	/****/
	BOOL m_bRevalueFlag;
	BOOL m_bRevalueDisable;
	int m_nRevalueType;
	CString m_strRevalueTrip1;
	CString m_strRevalueTrip2;
	int m_nRevaluePoints;
	CString m_strRevalueBonus;
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
	int m_nRedeemTrip;
	CString m_strRedeemValue;
	int m_nRedeemPurse;
};

//**********************************************************************

class CSQLRecordSetGroupFull : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_NormalNoParams params);
	CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_NormalByGroupNo params, bool bForExistenceCheck = FALSE);
	CSQLRecordSetGroupFull(CDatabase* pDatabase, RSParams_Group_CountRowsByRange params);

public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromGroupRow(CSQLRowGroupFull& RowGroup);
	void SaveToGroupRow(CSQLRowGroupFull& RowGroup);

public:
	bool StepSelectAll(CSQLRowGroupFull& RowGroup);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataGroupFull m_MD;
};

//**********************************************************************

