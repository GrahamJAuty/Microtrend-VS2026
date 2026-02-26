#pragma once
//**********************************************************************
#include "afxdb.h"
//**********************************************************************
#include "..\SQLDefines.h"
#include "..\SQLRecordSetConnectionPool.h"
//**********************************************************************
#include "SQLRowStampOfferInfo.h"
//**********************************************************************

struct RSParams_StampOffer_NormalNoParams
{
public:
	RSParams_StampOffer_NormalNoParams() {}
};

//**********************************************************************

struct RSParams_StampOffer_NormalByOfferId
{
public:
	RSParams_StampOffer_NormalByOfferId(int n)
		: nOfferId(n) {}

public:
	int nOfferId;
};

//**********************************************************************

class CSQLRecordSetMetadataStampOfferInfo : public CSQLRecordSetMetadata
{
public:
	CSQLRecordSetMetadataStampOfferInfo();
	void AddParamsByOfferID( const int nOfferID);

public:
	int m_nStampOfferID;
	int m_nEdition;
	BOOL m_bEnabled;
	int m_nThreshold;
	int m_nExpiry;
	CString m_strDescription;
};

//**********************************************************************

class CSQLRecordSetStampOfferInfo : public CSQLRecordSetConnectionPool
{
public:
	CSQLRecordSetStampOfferInfo(CDatabase* pDatabase, RSParams_StampOffer_NormalNoParams params);
	CSQLRecordSetStampOfferInfo(CDatabase* pDatabase, RSParams_StampOffer_NormalByOfferId params);
	
public:
	virtual CString GetDefaultSQL();
	
public:
	void LoadFromStampOfferInfoRow(CSQLRowStampOfferInfo& RowOS);
	void SaveToStampOfferInfoRow(CSQLRowStampOfferInfo& RowOS);

public:
	bool StepSelectAll(CSQLRowStampOfferInfo& RowOS);

private:
	void CommonInit();

public:
	CSQLRecordSetMetadataStampOfferInfo m_MD;
};

//**********************************************************************

