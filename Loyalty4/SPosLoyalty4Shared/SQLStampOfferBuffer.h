#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "SQLTable_Account\SQLRowAccountFull.h"
#include "SQLTable_OfferStamp\SQLRowOfferStamp.h"
//**********************************************************************
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
//**********************************************************************

struct CSQLStampOfferPendingInfo
{
public:
	CSQLStampOfferPendingInfo()
	{
		Reset();
	}

	void Reset()
	{
		m_nOfferID = 0;
		m_nEdition = 1;
		m_strOfferName = "";
		m_nLineStamps = 0;
		m_nExpiry = 0;
		m_nPluNo = 0;
		m_nPluQty = 0;
		m_strPluDescription = "";
	}

	int Compare(CSQLStampOfferPendingInfo& source, int nHint = 0);
	void Add(CSQLStampOfferPendingInfo& source);

public:
	int m_nOfferID;
	int m_nEdition;
	CString m_strOfferName;
	int m_nLineStamps;
	int m_nExpiry;
	__int64 m_nPluNo;
	int m_nPluQty;
	CString m_strPluDescription;
};

//**********************************************************************

class CSQLStampOfferAuditLine
{
public:
	CSQLStampOfferAuditLine()
	{
		Reset();
	}

	void Reset()
	{
		m_strEventDate = "";
		m_strEventTime = "";
		m_nSourceType = 0;
		m_nAppNo = 0;
		m_nDbNo = 0;
		m_nFolderSet = 0;
		m_nTNo = 0;
		m_nCCNo = 0;
		m_nServerNo = 0;
		m_nUserID = 1;
		m_strUserName = "";
		m_nGroupNo = 1;
		m_nOfferID = 0;
		m_nEdition = 1;
		m_strOfferName = "";
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
	int m_nDbNo;
	int m_nFolderSet;
	int m_nTNo;
	int m_nCCNo;
	int m_nServerNo;
	__int64 m_nUserID;
	CString m_strUserName;
	int m_nGroupNo;
	int m_nOfferID;
	int m_nEdition;
	CString m_strOfferName;
	CString m_strAwardDate;
	CString m_strExpiryDate;
	__int64 m_nPluNo;
	int m_nPluQty;
	CString m_strPluDescription;
	int m_nLineStamps;
	int m_nNewStampCount;
};

//*******************************************************************

class CSQLStampOfferBuffer
{
public:
	CSQLStampOfferBuffer();

public:
	void Reset();
	void SetAccount(CSQLRowAccountFull& RowAccount);
	void SetAwardDate(CString strDate) { m_strAwardDate = strDate; }
	void SetServerNo(int n) { m_nServerNo = n; }
	void SetDbNo(int n) { m_nDbNo = n; }
	void SetFolderSet(int n) { m_nFolderSet = n; }
	void SetTillNo(int n) { m_nTillNo = n; }

public:
	void BufferAward(CSQLStampOfferPendingInfo& line);
	void BufferRedeem(CSQLStampOfferPendingInfo& line);
	void BufferPCAction(int nAppNo, CSQLRowAccountFull& RowAccount, CSQLRowOfferStamp& RowStamps, CString strOfferName, int nNewStamps, int nStampCount);

public:
	int GetOfferStampAwardCount() { return m_arrayAwards.GetSize(); }
	bool ApplyOfferStampAward(int n, CDatabase* pDatabase);

public:
	int GetOfferStampRedeemCount() { return m_arrayRedeem.GetSize(); }
	bool ApplyOfferStampRedeem(int n, int& nLastOfferID, __int64& nLastPluNo, CDatabase* pDatabase);
	
public:
	void BufferAuditLine(CSQLStampOfferAuditLine& Line) { m_arrayAuditLines.Add(Line); }
	void WritePCAuditLines(CDatabase* pDatabase = NULL);
	void WriteAuditLines(int nCCNo);
	
private:
	void ValidateAwardDate();
	
private:
	__int64 m_nUserID;
	CString m_strUsername;
	int m_nGroupNo;
	int m_nServerNo;
	int m_nDbNo;
	int m_nFolderSet;
	int m_nTillNo;
	CReportConsolidationArray<CSQLStampOfferPendingInfo> m_arrayAwards;
	CReportConsolidationArray<CSQLStampOfferPendingInfo> m_arrayRedeem;
	CArray<CSQLStampOfferAuditLine, CSQLStampOfferAuditLine> m_arrayAuditLines;
	COleDateTime m_oleAwardDate;
	CString m_strAwardDate;
	CString m_strSortDate;
};

//**********************************************************************
extern CSQLStampOfferBuffer SQLStampOfferBuffer;
//**********************************************************************
