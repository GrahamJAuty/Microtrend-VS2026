#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLDefines.h"
#include "..\SPOSLoyalty4Shared\SQLRecordSetConnectionPool.h"
#include "..\SPOSLoyalty4Shared\SQLTable_Account\SQLRowAccountList.h"
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

struct RSParams_AccountList_NormalById
{
public:
	RSParams_AccountList_NormalById(CString str) : strUserID(str) {}

public:
	CString strUserID;
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
	void AddParamsById(const CString strUserId);

public:
	__int64 m_nUserID;
	CString m_strFullname;
	int m_nGroupNo;
	CString m_strExpiryDate;
	CString m_strInfo1;
	CString m_strInfo2;
	CString m_strInfo3;
	CString m_strInfo4;
	CString m_strAddress1;
	CString m_strAddress5;
	BOOL m_bInactive;
	CString m_strPurse1Balance;
	CString m_strPurse1SpendToDate;
	CString m_strPurse2Balance;
	CString m_strPurse2SpendToDate;
	CString m_strCashSpendToDate;
	int m_nPoints;
	int m_nPointsToDate;
	int m_nHotlistCode;
	CString m_strAlertCodes;

public:
	RSDataType_AccountList m_type;
};

//**********************************************************************

class CSQLRecordSetAccountList : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_NormalNoParams params);
	CSQLRecordSetAccountList(CDatabase* pDatabase, RSParams_AccountList_NormalById params);
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

