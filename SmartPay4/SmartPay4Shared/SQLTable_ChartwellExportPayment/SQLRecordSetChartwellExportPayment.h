#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowChartwellExportPayment.h"
//**********************************************************************

struct RSParams_ChartwellExportPayment_NormalNoParams
{
public:
	RSParams_ChartwellExportPayment_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_ChartwellExportPayment_NormalByTranDetail
{
public:
	RSParams_ChartwellExportPayment_NormalByTranDetail(CString str1, CString str2, int n1, CString str3)
		: strUserId(str1), strTransactionId(str2), nTerminalNo(n1), strDate(str3) {}

public:
	CString strUserId;
	CString strTransactionId;
	int nTerminalNo;
	CString strDate;
};

//**********************************************************************

class CSQLRecordSetMetadataChartwellExportPayment : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataChartwellExportPayment(bool bForExistenceCheck = FALSE);
	void AddParamsByTranDetail(const CString strUSerID, const CString strTransactionID, const int nTerminalNo, const CString strDate);

public:
	__int64 m_nUserID;
	__int64 m_nTransactionID;
	int m_nTerminalNo;
	CString m_strDate;
	CString m_strCreateDateTime;
	int m_nGroupNo;
	CString m_strPaymentPurse1;
	CString m_strPaymentPurse2;
	CString m_strPaymentPurse3;
};

//**********************************************************************

class CSQLRecordSetChartwellExportPayment : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetChartwellExportPayment(CDatabase* pDatabase, RSParams_ChartwellExportPayment_NormalNoParams params);
	CSQLRecordSetChartwellExportPayment(CDatabase* pDatabase, RSParams_ChartwellExportPayment_NormalByTranDetail params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();

public:
	void LoadFromChartwellExportPaymentRow(CSQLRowChartwellExportPayment& RowPB);
	void SaveToChartwellExportPaymentRow(CSQLRowChartwellExportPayment& RowPB);

public:
	bool StepSelectAll(CSQLRowChartwellExportPayment& RowPB);

private:
	void CommonInit();	

public:
	CSQLRecordSetMetadataChartwellExportPayment m_MD;
};

//**********************************************************************

