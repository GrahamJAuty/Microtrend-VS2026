#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowPebbleExportPayment.h"
//**********************************************************************

struct RSParams_PebbleExportPayment_NormalNoParams
{
public:
	RSParams_PebbleExportPayment_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_PebbleExportPayment_NormalByFullParams
{
public:
	RSParams_PebbleExportPayment_NormalByFullParams(CString str1, CString str2, CString str3, int n1)
		: strUserId(str1), strDate(str2), strTime(str3), nCCNo(n1){}

public:
	CString strUserId;
	CString strDate;
	CString strTime;
	int nCCNo;
};

//**********************************************************************

class CSQLRecordSetMetadataPebbleExportPayment : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataPebbleExportPayment(bool bForExistenceCheck = FALSE);
	void AddParamsFull( const CString strUserID, const CString strDate, const CString strTime, const int nCCNo);

public:
	__int64 m_nUserID;
	CString m_strDate;
	CString m_strTime;
	CString m_strCreateDateTime;
	int m_nCCNo;
	int m_nPaymentType[MAX_PEBBLE_EXPORT_PAYMENT];
	CString m_strPaymentAmount[MAX_PEBBLE_EXPORT_PAYMENT];
};

//**********************************************************************

class CSQLRecordSetPebbleExportPayment : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetPebbleExportPayment(CDatabase* pDatabase, RSParams_PebbleExportPayment_NormalNoParams params);
	CSQLRecordSetPebbleExportPayment(CDatabase* pDatabase, RSParams_PebbleExportPayment_NormalByFullParams params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromPebbleExportPaymentRow(CSQLRowPebbleExportPayment& RowPB);
	void SaveToPebbleExportPaymentRow(CSQLRowPebbleExportPayment& RowPB);

public:
	bool StepSelectAll(CSQLRowPebbleExportPayment& RowPB);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataPebbleExportPayment m_MD;
};

//**********************************************************************

