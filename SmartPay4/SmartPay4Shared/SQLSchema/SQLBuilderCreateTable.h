#pragma once
/**********************************************************************/
#define SQLFIELD_FLAG_INDEX 1
#define SQLFIELD_FLAG_NULLABLEINT 1
/**********************************************************************/
#include "..\\SQLDefines.h"
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
	CString GetCommandCreateLeaversTable();
	CString GetCommandCreateExternalAccountTable();
	CString GetCommandCreatePeriodRefreshTable();
	CString GetCommandCreateBioRegisterTable();
	CString GetCommandCreateCCNoTable();
	CString GetCommandCreatePluInfoTable();
	CString GetCommandCreateAuditPeriodTable();
	CString GetCommandCreateUserTextTable();
	CString GetCommandCreateClosingBalanceTable();
	CString GetCommandCreateBromComBalanceChangesTable();
	CString GetCommandCreateBromComTransactionTable();
	CString GetCommandCreateBromComPendingOrderTable();
	CString GetCommandCreateBromComPendingItemTable();
	CString GetCommandCreateBromComItemTable();
	CString GetCommandCreatePurchaseControlRuleTable();
	CString GetCommandCreatePurchaseControlStampTable();
	CString GetCommandCreateImportFilenameCacheTable();
	CString GetCommandCreatePebbleConfigTable(bool bOriginal);
	CString GetCommandCreatePebblePendingTransactionTable(bool bUnmatched);
	CString GetCommandCreatePebbleRecentTransactionTable();
	CString GetCommandCreatePebbleExportTransactionTable(bool bOriginal);
	CString GetCommandCreatePebbleExportTransactionSentTable();
	CString GetCommandCreatePebbleExportPaymentTable();
	CString GetCommandCreatePebbleLinkTimeTable();
	CString GetCommandCreateChartwellExportPaymentTable();
	CString GetCommandCreatePebblePaymentNameTable();
	//CString GetCommandCreateOperatorNameTable();
	CString GetCommandCreateEposTerminalTable();
	CString GetCommandCreateWebPaymentOptionTable();
	CString GetCommandCreatePurchaseHistoryHeaderTable();
	CString GetCommandCreatePINNumberTable();
	CString GetCommandCreatePINNumberIndex();
	CString GetCommandCreatePINNumberConstraint();
	void AddPebblePaymentNames(CStringArray& arrayCommands);

private:
	void AddFieldInternal(CString strName, int nDataType, int nFlags = 0, int nFieldLen1 = 0, int nFieldLen2 = 0);

	void AddDecimal(const recDouble label, int nWidth = 16, int nDPs = 2)
	{
		AddDecimal(label.Label, nWidth, nDPs);
	}

	void AddDecimal(CString strName, int nWidth = 16, int nDPs = 2)
	{
		AddFieldInternal(strName, SQLSRVR_DECIMAL, 0, nWidth, nDPs);
	}

	/*****/

	void AddInt64(const recInt64 label, int nFlags = 0)
	{
		AddInt64(label.Label, nFlags);
	}

	void AddInt64(CString strName, int nFlags = 0)
	{
		AddFieldInternal(strName, SQLSRVR_INT64, nFlags);
	}

	/*****/

	void AddInt32(const recInt label, int nFlags = 0)
	{
		AddInt32(label.Label, nFlags);
	}

	void AddInt32(CString strName, int nFlags = 0)
	{
		AddFieldInternal(strName, SQLSRVR_INT32, nFlags);
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

	void AddFixLenText(const recString label, int nFlags = 0)
	{
		AddFixLenText(label.Label, label.Max, nFlags);
	}

	void AddFixLenText(CString strName, int nMaxLen, int nFlags = 0)
	{
		AddFieldInternal(strName, SQLSRVR_FIXLENTEXT, nFlags, nMaxLen);
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
	void SetupAccountFields( int nTableType );
	CString BuildCommandString(CString strTableName, bool bIfNotExists = FALSE);
	CString GetDataTypeString(CSQLCreateFieldInfo& FieldInfo);

private:
	CArray<CSQLCreateFieldInfo, CSQLCreateFieldInfo> m_arrayFieldInfo;

private:
	int m_nPKFieldCount;
	bool m_bAutoID;
};

/**********************************************************************/
