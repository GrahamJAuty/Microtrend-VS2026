#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowStaffCard.h"
//**********************************************************************

struct RSParams_StaffCard_NormalNoParams
{
public:
	RSParams_StaffCard_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_StaffCard_NormalByDbNoSetNoServerNo
{
public:
	RSParams_StaffCard_NormalByDbNoSetNoServerNo(int n1, int n2, int n3)
		: nDbNo(n1), nSetNo(n2), nServerNo(n3) {}

public:
	int nDbNo;
	int nSetNo;
	int nServerNo;
};

//**********************************************************************

class CSQLRecordSetMetadataStaffCard : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataStaffCard();
	void AddParamsByDbNoSetNoServerNo(const int nDbNo, const int nSetNo, const int nServerNo);

public:
	int m_nServerNo;
	int m_nFolderIndexDbNo;
	int m_nFolderIndexSetNo;
	__int64 m_nUserID;
};

//**********************************************************************

class CSQLRecordSetStaffCard : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetStaffCard(CDatabase* pDatabase, RSParams_StaffCard_NormalNoParams params);
	CSQLRecordSetStaffCard(CDatabase* pDatabase, RSParams_StaffCard_NormalByDbNoSetNoServerNo params);

public:
	virtual CString GetDefaultSQL();
	void DoFieldExchange(CFieldExchange* pFX);

public:
	void LoadFromStaffCardRow(CSQLRowStaffCard& RowBC);
	void SaveToStaffCardRow(CSQLRowStaffCard& RowBC);

public:
	bool StepSelectAll(CSQLRowStaffCard& RowBC);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataStaffCard m_MD;	
};

//**********************************************************************

