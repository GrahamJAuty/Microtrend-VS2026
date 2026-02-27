#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLRecordSetConnectionPool.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountList.h"
//**********************************************************************

enum class RSDataType_AccountList : int
{
	Normal = 0,
	CardPicker = 1,
};

//**********************************************************************

struct RSParams_AccountList_NormalNoParams
{
public:
	RSParams_AccountList_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_AccountList_CardPickerNoParams
{
public:
	RSParams_AccountList_CardPickerNoParams() {}
};

//**********************************************************************

class CSQLRecordSetMetadataAccountList : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataAccountList(RSDataType_AccountList type);

public:
	__int64 m_nUserID;
	CString m_strUsername;
	CString m_strForename;
	CString m_strSurname;
	CString m_strExpiryDate;
	int m_nGroupNo;
	int m_nGroupShiftStatus;
	CString m_strGroupShiftStartDate;
	CString m_strGroupShiftEndDate;
	int m_nGroupShiftFutureGroup;
	BOOL m_bInactive;
	CString m_strPurse1Credit;
	CString m_strPurse1Liability;
	CString m_strPurse2Balance;
	CString m_strPurse3Credit;
	CString m_strPurse3Liability;
	int m_nPoints;
	int m_nAlertCode;
	CString m_strMemberID;
	CString m_strInfo1;

public:
	RSDataType_AccountList m_type;
};

//**********************************************************************

class CSQLRecordSetAccountList : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_NormalNoParams params);
	CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_CardPickerNoParams params);
	
public:
	virtual CString GetDefaultSQL();

public:
	void SaveToAccountRow(CSQLRowAccountList& RowAccount);
	bool StepSelectAll(CSQLRowAccountList& RowAccount);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataAccountList m_MD;

private:
	RSDataType_AccountList m_type;
};

//**********************************************************************

