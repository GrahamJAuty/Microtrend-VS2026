#pragma once
/**********************************************************************/
#define SQLFIELD_FLAG_INDEX 1
/**********************************************************************/
#include "..\..\SPosLoyalty4Shared\SQLDefines.h"
/**********************************************************************/

struct CSQLCreateFieldInfo
{
public:
	CSQLCreateFieldInfo();

public:
	CString m_strFieldName;
	int m_nDataType;
	int m_nFieldFlags;
	int m_nFieldLen1;			//VARCHAR OR DECIMAL
	int m_nFieldLen2;			//DECIMAL DPS
};

/**********************************************************************/

class CSQLBuilderCreateTable  
{
public:
	CSQLBuilderCreateTable();

public:
	CString GetCommandCreateVersionTable();
	CString GetCommandCreateAccountTable();
	CString GetCommandCreateGroupTable();
	CString GetCommandCreateGroupTopupBonusTable();
	CString GetCommandCreateExternalAccountTable();
	CString GetCommandCreatePluInfoTable();
	CString GetCommandCreateDeptInfoTable(bool bOriginalVersion = FALSE);
	CString GetCommandCreateStaffCardTable(bool bOriginalVersion = FALSE);
	CString GetCommandCreateStaffGiftExceptionTable(bool bOriginalVersion = FALSE);
	CString GetCommandCreateCCNoTable();
	CString GetCommandCreateAuditPeriodTable();
	CString GetCommandCreateOfferStampTable();
	CString GetCommandCreateStampOfferInfoTable();
	CString GetCommandCreateEcrmanDatabaseTable();
	CString GetCommandCreateSchemeTable();

private:
	void AddFieldInternal(CString strName, int nDataType, int nFlags = 0, int nFieldLen1 = 0, int nFieldLen2 = 0 );
	
	/*****/

	void AddDecimal(const recDouble label, int nWidth = 16, int nDPs = 2)
	{
		AddDecimal(label.Label, nWidth, nDPs);
	}

	void AddDecimal(CString strName, int nWidth = 16, int nDPs = 2) 
	{ 
		AddFieldInternal(strName, SQLSRVR_DECIMAL, 0, nWidth, nDPs);
	}

	/*****/

	void AddLegacyFloat(const recDouble label)
	{
		AddLegacyFloat(label.Label);
	}

	void AddLegacyFloat(CString strName)
	{
		AddFieldInternal(strName, SQLSRVR_LEGACY_FLOAT);
	}

	/*****/

	void AddInt64(const recInt64 label)
	{
		AddInt64(label.Label);
	}

	void AddInt64(CString strName)
	{
		AddFieldInternal(strName, SQLSRVR_INT64);
	}

	/*****/

	void AddInt32(const recInt label)
	{
		AddInt32(label.Label);
	}

	void AddInt32(CString strName)
	{
		AddFieldInternal(strName, SQLSRVR_INT32);
	}

	/*****/

	void AddBit(const recBool label)
	{
		AddBit(label.Label);
	}

	void AddBit(CString strName)
	{
		AddFieldInternal(strName, SQLSRVR_BIT);
	}

	/*****/

	void AddText(const recString label, int nFlags = 0)
	{
		AddText(label.Label, label.Max, nFlags);
	}

	void AddText(CString strName, int nMaxLen, int nFlags = 0)
	{
		AddFieldInternal(strName, SQLSRVR_TEXT, nFlags, nMaxLen);
	}

	/*****/

	void AddRowVersion(const recInt64 label)
	{
		AddRowVersion(label.Label);
	}

	void AddRowVersion(CString strName)
	{
		AddFieldInternal(strName, SQLSRVR_ROWVERSION);
	}

	/*****/

	void AddComputed(const recString label)
	{
		AddComputed(label.Label);
	}

	void AddComputed(CString strName)
	{
		AddFieldInternal(strName, SQLSRVR_COMPUTED);
	}

	/*****/
	

private:
	void SetupAccountFields();
	CString BuildCommandString(CString strTableName, bool bIfNotExists = FALSE);
	CString GetDataTypeString(CSQLCreateFieldInfo& FieldInfo);

private:
	CArray<CSQLCreateFieldInfo, CSQLCreateFieldInfo> m_arrayFieldInfo;

private:
	int m_nPKFieldCount;
	bool m_bAutoID;
};

/**********************************************************************/
