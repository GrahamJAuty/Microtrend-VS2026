#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowBioRegister.h"
//**********************************************************************

struct RSParams_BioRegister_NormalNoParams
{
public:
	RSParams_BioRegister_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_BioRegister_NormalByUserId
{
public:
	RSParams_BioRegister_NormalByUserId(CString str)
		: strUserId(str) {}

public:
	CString strUserId;
};

//**********************************************************************

class CSQLRecordSetMetadataBioRegister : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataBioRegister(bool bForExistenceCheck = FALSE);
	void AddParamsByUserID(const CString strUserID);
	void AddParamsByRange(const CString strUserIDFrom, const CString strUserIDTo);

public:
	__int64 m_nUserID;
	CString m_strUsername;
	CString m_strRegDate;
	CString m_strRegTime;
	CString m_strUnRegDate;
	CString m_strUnRegTime;
	BOOL m_bUnReg;
	CString m_strFirstDate;
	CString m_strFirstTime;
	CString m_strLastDate;
	CString m_strLastTime;
	int m_nBioCounter;
};

//**********************************************************************

class CSQLRecordSetBioRegister : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetBioRegister(CDatabase* pDatabase, RSParams_BioRegister_NormalNoParams params);
	CSQLRecordSetBioRegister(CDatabase* pDatabase, RSParams_BioRegister_NormalByUserId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromBioRegisterRow(CSQLRowBioRegister& RowBio);
	void SaveToBioRegisterRow(CSQLRowBioRegister& RowBio);

public:
	bool StepSelectAll(CSQLRowBioRegister& RowBio);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataBioRegister m_MD;
};

//**********************************************************************

