#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowClosingBalance.h"
//**********************************************************************

struct RSParams_ClosingBalance_NormalNoParams
{
public:
	RSParams_ClosingBalance_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_ClosingBalance_NormalByDateSingle
{
public:
	RSParams_ClosingBalance_NormalByDateSingle(CString str)
		: strDate(str) {}

public:
	CString strDate;
};

//**********************************************************************

struct RSParams_ClosingBalance_DetailByRange
{
public:
	RSParams_ClosingBalance_DetailByRange(CString str1, CString str2)
		: strDateFrom(str1), strDateTo(str2){}

public:
	CString strDateFrom;
	CString strDateTo;
};

//**********************************************************************

struct RSParams_ClosingBalance_CountRowsByRange
{
public:
	RSParams_ClosingBalance_CountRowsByRange(CString str1, CString str2)
		: strDateFrom(str1), strDateTo(str2) {
	}

public:
	CString strDateFrom;
	CString strDateTo;
};

//**********************************************************************

class CSQLRecordSetMetadataClosingBalance : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataClosingBalance(bool bForExistenceCheck = FALSE);
	void AddParamsByDateSingle(const CString strDate);
	void AddParamsByDateRange(const CString strDateFrom, const CString strDateTo);

public:
	CString m_strKeyDate;
	/*****/
	CString m_strDate;
	CString m_strTime;
	/*****/
	CString m_strArchiveDate;
	CString m_strArchiveTime;
	/*****/
	CString m_strPurse1LiabilityAudit;
	CString m_strPurse1CreditAudit;
	/*****/
	CString m_strPurse2BalanceAudit;
	/*****/
	CString m_strPurse3LiabilityAudit;
	CString m_strPurse3CreditAudit;
	/*****/
	BOOL m_bHaveAdjust;
	/*****/
	CString m_strPurse1LiabilityAdjust;
	CString m_strPurse1CreditAdjust;
	/*****/
	CString m_strPurse3LiabilityAdjust;
	CString m_strPurse3CreditAdjust;
};

//**********************************************************************

class CSQLRecordSetClosingBalance : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_NormalNoParams params);
	CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_NormalByDateSingle params, bool bForExistenceCheck = FALSE );
	CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_DetailByRange params);
	CSQLRecordSetClosingBalance(CDatabase* pDatabase, RSParams_ClosingBalance_CountRowsByRange params);
	
public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromClosingBalanceRow(CSQLRowClosingBalance& RowClosing);
	void SaveToClosingBalanceRow(CSQLRowClosingBalance& RowClosing);

public:
	bool StepSelectAll(CSQLRowClosingBalance& RowClosing);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataClosingBalance m_MD;
};

//**********************************************************************

