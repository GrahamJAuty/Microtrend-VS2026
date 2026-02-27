#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPurchaseHistoryHeader.h"
//**********************************************************************

struct RSParams_PurchaseHistoryHeader_NormalNoParams
{
public:
	RSParams_PurchaseHistoryHeader_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PurchaseHistoryHeader_AddWithAutoLineID
{
public:
	RSParams_PurchaseHistoryHeader_AddWithAutoLineID() {}
};

//**********************************************************************

struct RSParams_PurchaseHistoryHeader_NormalByLineId
{
public:
	RSParams_PurchaseHistoryHeader_NormalByLineId(CString str)
		: strLineId(str) {}

public:
	CString strLineId;
};

//**********************************************************************

struct RSParams_PurchaseHistoryHeader_GetLastId
{
public:
	RSParams_PurchaseHistoryHeader_GetLastId() {}
};

//**********************************************************************

class CSQLRecordSetMetadataPurchaseHistoryHeader : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPurchaseHistoryHeader(bool bForExistenceCheck = FALSE, bool bGetLastId = FALSE);
	void AddParamsByLineID(const CString strLineID);
	void AddLineIDField();

public:
	void AddTextParam(CString strLabel, const CString strDef = "");
	void AddIntParam(CString strLabel, const int nDef = 0);

public:
	__int64 m_nLineID = 0;
	CString m_strDate = "";
	CString m_strTime = "";
};

//**********************************************************************

class CSQLRecordSetPurchaseHistoryHeader : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_NormalNoParams params);
	CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_AddWithAutoLineID params);
	CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_GetLastId params);
	CSQLRecordSetPurchaseHistoryHeader(CDatabase* pDatabase, RSParams_PurchaseHistoryHeader_NormalByLineId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPurchaseHistoryHeaderRow(CSQLRowPurchaseHistoryHeader& RowBC);
	void SaveToPurchaseHistoryHeaderRow(CSQLRowPurchaseHistoryHeader& RowBC);

public:
	bool StepSelectAll(CSQLRowPurchaseHistoryHeader& RowBC);
	void UpdateParamCount() { m_nParams = m_MD.GetParamCount(); }

private:
	void CommonInit();
	
public:
	CSQLRecordSetMetadataPurchaseHistoryHeader m_MD;
};

//**********************************************************************
