#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowGroupTopupBonus.h"
//**********************************************************************

struct RSParams_GroupTopupBonus_NormalNoParams
{
public:
	RSParams_GroupTopupBonus_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_GroupTopupBonus_NormalByGroupNo
{
public:
	RSParams_GroupTopupBonus_NormalByGroupNo(int n) 
		: m_nGroupNo(n) {}

public:
	int m_nGroupNo;
};

//**********************************************************************

struct RSParams_GroupTopupBonus_NormalByFullKey
{
public:
	RSParams_GroupTopupBonus_NormalByFullKey(int n1, int n2, int n3) 
		: m_nGroupNo(n1), m_nPurseNo(n2), m_nTopupNo(n3) {}

public:
	int m_nGroupNo;
	int m_nPurseNo;
	int m_nTopupNo;
};

//**********************************************************************

class CSQLRecordSetMetadataGroupTopupBonus : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataGroupTopupBonus(bool bForExistenceCheck = FALSE);
	void AddParamsByGroupNo(const int nGroupNo);
	void AddParamsByGroupNoPurseNoTopupNo(const int nGroupNo, const int nPurseNo, const int nTopupNo);

public:
	int m_nGroupNo;
	int m_nPurseNo;
	int m_nTopupNo;
	/****/
	BOOL m_bBonusFlag;
	BOOL m_bBonusDisable;
	int m_nBonusType;
	CString m_strBonusTrip;
	int m_nBonusPoints;
	CString m_strBonusValue;
	int m_nBonusPercent;
};

//**********************************************************************

class CSQLRecordSetGroupTopupBonus : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetGroupTopupBonus(CDatabase* pDatabase, RSParams_GroupTopupBonus_NormalNoParams params);
	CSQLRecordSetGroupTopupBonus(CDatabase* pDatabase, RSParams_GroupTopupBonus_NormalByGroupNo params);
	CSQLRecordSetGroupTopupBonus(CDatabase* pDatabase, RSParams_GroupTopupBonus_NormalByFullKey params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromGroupTopupBonusRow(CSQLRowGroupTopupBonus& RowGroup);
	void SaveToGroupTopupBonusRow(CSQLRowGroupTopupBonus& RowGroup);

public:
	bool StepSelectAll(CSQLRowGroupTopupBonus& RowGroup);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataGroupTopupBonus m_MD;
};

//**********************************************************************

