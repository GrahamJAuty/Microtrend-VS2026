#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowWebPaymentOptions.h"
//**********************************************************************

struct RSParams_WebPaymentOptions_NormalNoParams
{
public:
	RSParams_WebPaymentOptions_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_WebPaymentOptions_NormalByLineId
{
public:
	RSParams_WebPaymentOptions_NormalByLineId(int n)
		: nLineId(n){}

public:
	int nLineId;
};

//**********************************************************************

class CSQLRecordSetMetadataWebPaymentOptions : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataWebPaymentOptions(bool bForExistenceCheck = FALSE);
	void AddParamsByLineID(const int nLineID);
	
public:
	int m_nLineID = 1;
	int m_nAccountLinkDays = 0x3E;
	CString m_strAccountLinkStart = "00:00";
	CString m_strAccountLinkEnd = "23:59";
	CString m_strAccountLinkLastDate = "";
	CString m_strAccountLinkLastTime = "";
	int m_nPaymentDays = 0x3E;
	CString m_strPaymentStart = "00:00";
	CString m_strPaymentEnd = "23:59";
	CString m_strPaymentLastDate = "";
	CString m_strPaymentLastTime = "";
	CString m_strBalanceLastDate = "";
	CString m_strBalanceLastTime = "";
	CString m_strTransactionLastDate = "";
	CString m_strTransactionLastTime = "";
	BOOL m_bExclude1Flag = FALSE;
	CString m_strExclude1Start = "00:00";
	CString m_strExclude1End = "00:00";
	BOOL m_bExclude2Flag = FALSE;
	CString m_strExclude2Start = "00:00";
	CString m_strExclude2End = "00:00";
	BOOL m_bExclude3Flag = FALSE;
	CString m_strExclude3Start = "00:00";
	CString m_strExclude3End = "00:00";
	BOOL m_bExclude4Flag = FALSE;
	CString m_strExclude4Start = "00:00";
	CString m_strExclude4End = "00:00";
	CString m_strExcludeUploadStart = "00:00";
	CString m_strExcludeUploadEnd = "00:00";
	BOOL m_bEODAfterPaymentFlag = TRUE;
	CString m_strEODAfterTime = "16:00";
	BOOL m_bPromptBeforeAccessFlag = TRUE;
	BOOL m_bImportAccountsFlag = TRUE;
	bool m_bForExistenceCheck = FALSE;
};

//**********************************************************************

class CSQLRecordSetWebPaymentOptions : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetWebPaymentOptions(CDatabase* pDatabase, RSParams_WebPaymentOptions_NormalNoParams params);
	CSQLRecordSetWebPaymentOptions(CDatabase* pDatabase, RSParams_WebPaymentOptions_NormalByLineId params, bool bForExistenceCheck = FALSE);

public:
	virtual CString GetDefaultSQL();
	void DoFieldExchange(CFieldExchange* pFX);

public:
	void LoadFromWebPaymentOptionsRow(CSQLRowWebPaymentOptions& RowPB);
	void SaveToWebPaymentOptionsRow(CSQLRowWebPaymentOptions& RowPB);

private:
	void CommonInit();

public:
	CByteArray m_arrayRowVersion;
	bool m_bForExistenceCheck = FALSE;
	CSQLRecordSetMetadataWebPaymentOptions m_MD;
};

//**********************************************************************

