#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "SQLTable_Account\SQLRowAccountFull.h"
#include "SQLTable_PurchaseControlStamp\SQLRowPurchaseControlStamp.h"
//**********************************************************************
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
//**********************************************************************

struct CSQLPurchaseControlPendingInfo
{
public:
	CSQLPurchaseControlPendingInfo()
	{
		Reset();
	}

	void Reset()
	{
		m_nControlID = 0;
		m_strControlName = "";
		m_nLineStamps = 0;
		m_nExpiryDays = 0;
		m_nPluNo = 0;
		m_nPluQty = 0;
		m_strPluDescription = "";
	}

	int Compare(CSQLPurchaseControlPendingInfo& source, int nHint = 0);
	void Add(CSQLPurchaseControlPendingInfo& source);

public:
	int m_nControlID;
	CString m_strControlName;
	int m_nLineStamps;
	int m_nExpiryDays;
	__int64 m_nPluNo;
	int m_nPluQty;
	CString m_strPluDescription;
};

//**********************************************************************

class CSQLPurchaseControlAuditLine
{
public:
	CSQLPurchaseControlAuditLine()
	{
		Reset();
	}

	void Reset()
	{
		m_strEventDate = "";
		m_strEventTime = "";
		m_nSourceType = 0;
		m_nAppNo = 0;
		m_nFolderSet = 0;
		m_nTNo = 0;
		m_nCCNoFull = 0;
		m_nServerNo = 0;
		m_nUserID = 1;
		m_strUserName = "";
		m_nGroupNo = 1;
		m_nControlID = 0;
		m_strControlName = "";
		m_strAwardDate = "";
		m_strExpiryDate = "";
		m_nPluNo = 1;
		m_nPluQty = 1;
		m_strPluDescription = "";
		m_nLineStamps = 1;
		m_nNewStampCount = 1;
	}

	void ConvertFromString(CString strLine);

public:
	CString m_strEventDate;
	CString m_strEventTime;
	int m_nSourceType;
	int m_nAppNo;
	int m_nFolderSet;
	int m_nTNo;
	int m_nCCNoFull;
	int m_nServerNo;
	__int64 m_nUserID;
	CString m_strUserName;
	int m_nGroupNo;
	int m_nControlID;
	CString m_strControlName;
	CString m_strAwardDate;
	CString m_strExpiryDate;
	__int64 m_nPluNo;
	int m_nPluQty;
	CString m_strPluDescription;
	int m_nLineStamps;
	int m_nNewStampCount;
};

//*******************************************************************

class CSQLPurchaseControlBuffer
{
public:
	CSQLPurchaseControlBuffer();

public:
	void Reset();
	void SetAccount(CSQLRowAccountFull& RowAccount);
	void SetAwardDate(CString strDate) { m_strAwardDate = strDate; }
	void SetServerNo(int n) { m_nServerNo = n; }
	void SetFolderSet(int n) { m_nFolderSet = n; }
	void SetTillNo(int n) { m_nTillNo = n; }

public:
	void BufferAward(CSQLPurchaseControlPendingInfo& line);
	void BufferRedeem(CSQLPurchaseControlPendingInfo& line);
	void BufferPCAction(int nAppNo, CSQLRowAccountFull& RowAccount, CSQLRowPurchaseControlStamp& RowStamps, CString strRuleName, int nNewStamps, int nStampCount);

public:
	int GetPurchaseControlStampAwardCount() { return m_arrayAwards.GetSize(); }
	bool ApplyPurchaseControlStampAward(int n, CDatabase* pDatabase);

public:
	int GetPurchaseControlStampRedeemCount() { return m_arrayRedeem.GetSize(); }
	bool ApplyPurchaseControlStampRedeem(int n, int& nLastRuleID, __int64& nLastPluNo, CDatabase* pDatabase);
	
public:
	void BufferAuditLine(CSQLPurchaseControlAuditLine& Line) { m_arrayAuditLines.Add(Line); }
	void WritePCAuditLines(CDatabase* pDatabase = NULL);
	void WriteAuditLines(int nCCNo);
	
private:
	void ValidateAwardDate();
	
private:
	__int64 m_nUserID;
	CString m_strUsername;
	int m_nGroupNo;
	int m_nServerNo;
	int m_nFolderSet;
	int m_nTillNo;
	CReportConsolidationArray<CSQLPurchaseControlPendingInfo> m_arrayAwards;
	CReportConsolidationArray<CSQLPurchaseControlPendingInfo> m_arrayRedeem;
	CArray<CSQLPurchaseControlAuditLine, CSQLPurchaseControlAuditLine> m_arrayAuditLines;
	COleDateTime m_oleAwardDate;
	CString m_strAwardDate;
	CString m_strSortDate;
};

//**********************************************************************
extern CSQLPurchaseControlBuffer SQLPurchaseControlBuffer;
//**********************************************************************
