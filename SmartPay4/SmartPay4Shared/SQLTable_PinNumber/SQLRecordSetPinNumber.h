#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPINNumber.h"
//**********************************************************************

struct RSParams_PINNumber_NormalNoParams
{
public:
	RSParams_PINNumber_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PINNumber_NormalByPINNumber
{
public:
	RSParams_PINNumber_NormalByPINNumber(CString str)
		: strPINNumber(str) {}

public:
	CString strPINNumber;
};

//**********************************************************************

struct RSParams_PINNumber_NormalByUserID
{
public:
	RSParams_PINNumber_NormalByUserID(CString strUserID)
		: strUserID(strUserID) {}

public:
	CString strUserID;
};

//**********************************************************************

struct RSParams_PINNumber_CountRowsByRange
{
public:
	RSParams_PINNumber_CountRowsByRange(CString strFrom, CString strTo)
		: strPINNumberFrom(strFrom), strPINNumberTo(strTo) {}

public:
	CString strPINNumberFrom;
	CString strPINNumberTo;
};

//**********************************************************************

class CSQLRecordSetMetadataPINNumber : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPINNumber(bool bForExistenceCheck = FALSE);
	void AddParamsByPINNumber(const CString strPINNumber);
	void AddParamsByUserID(const CString strUserID);
	void AddParamsByRange(const CString strPINNumberFrom, const CString strPINNumberTo);

public:
	CString m_strPINNumber = "0000";
	CString m_strUserID = "";
	CString m_strUsername = "";
	CString m_strFirstDate = "";
	CString m_strFirstTime = "";
	CString m_strLastDate = "";
	CString m_strLastTime = "";
	int m_nUseCounter = 0;
};

//**********************************************************************

class CSQLRecordSetPINNumber : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_NormalNoParams params);
	CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_NormalByPINNumber params, bool bForExistenceCheck = FALSE);
	CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_NormalByUserID params);
	CSQLRecordSetPINNumber(CDatabase* pDatabase, RSParams_PINNumber_CountRowsByRange params);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPINNumberRow(CSQLRowPINNumber& RowPINNumber);
	void SaveToPINNumberRow(CSQLRowPINNumber& RowPINNumber);

public:
	bool StepSelectAll(CSQLRowPINNumber& RowPINNumber);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPINNumber m_MD;
};

//**********************************************************************

