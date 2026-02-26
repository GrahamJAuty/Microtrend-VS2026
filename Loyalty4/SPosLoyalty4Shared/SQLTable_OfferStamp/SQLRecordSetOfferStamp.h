#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowOfferStamp.h"
//**********************************************************************

struct RSParams_OfferStamp_NormalNoParams
{
public:
	RSParams_OfferStamp_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_OfferStamp_NormalByUserId
{
public:
	RSParams_OfferStamp_NormalByUserId(CString str) 
		: strUserId(str) {}

public:
	CString strUserId;
};

//**********************************************************************

struct RSParams_OfferStamp_RowCountByUserId
{
public:
	RSParams_OfferStamp_RowCountByUserId(CString str)
		: strUserId(str) {
	}

public:
	CString strUserId;
};

//**********************************************************************

struct RSParams_OfferStamp_NormalByUserIdOfferId
{
public:
	RSParams_OfferStamp_NormalByUserIdOfferId(CString str1, int n1) 
		: strUserId(str1), nOfferId(n1) {}

public:
	CString strUserId;
	int nOfferId;
};

//**********************************************************************

struct RSParams_OfferStamp_NormalByUserIdOfferIdEdition
{
public:
	RSParams_OfferStamp_NormalByUserIdOfferIdEdition(CString str1, int n1, int n2) 
		: strUserId(str1), nOfferId(n1), nEdition(n2) {}

public:
	CString strUserId;
	int nOfferId;
	int nEdition;
};

//**********************************************************************

struct RSParams_OfferStamp_NormalByFullKey
{
public:
	RSParams_OfferStamp_NormalByFullKey(CString str1, int n1, int n2, CString str2, CString str3) 
		: strUserId(str1), nOfferId(n1), nEdition(n2), strAwardDate(str2), strExpireDate(str3) {}

public:
	CString strUserId;
	int nOfferId;
	int nEdition;
	CString strAwardDate;
	CString strExpireDate;
};

//**********************************************************************

class CSQLRecordSetMetadataOfferStamp : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataOfferStamp(bool bForRowCount = FALSE);
	void AddParamsByUserID(const CString strUserID);
	void AddParamsByUserIDRowCount(const CString strUserID);
	void AddParamsByUserIDOfferID(const CString strUserID, const int nOfferID);
	void AddParamsByUserIDOfferIDEdition(const CString strUserID, const int nOfferID, const int nEdition);
	void AddParamsByUserIDOfferIDEditionDates(const CString strUserID, const int nOfferID, const int nEdition, const CString strAwardDate, const CString strExpireDate);
	
public:
	__int64 m_nUserID;
	int m_nStampOfferID;
	int m_nEdition;
	CString m_strAwardDate;
	CString m_strExpireDate;
	int m_nStampCount;
};

//**********************************************************************

class CSQLRecordSetOfferStamp : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalNoParams params);
	CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByUserId params);
	CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_RowCountByUserId params);
	CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByUserIdOfferId params);
	CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByUserIdOfferIdEdition params);
	CSQLRecordSetOfferStamp(CDatabase* pDatabase, RSParams_OfferStamp_NormalByFullKey params);
	
public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromOfferStampRow(CSQLRowOfferStamp& RowOS);
	void SaveToOfferStampRow(CSQLRowOfferStamp& RowOS);

public:
	bool StepSelectAll(CSQLRowOfferStamp& RowOS);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataOfferStamp m_MD;
};

//**********************************************************************

