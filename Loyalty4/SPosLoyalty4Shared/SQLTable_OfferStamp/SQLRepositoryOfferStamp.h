#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\ReportConsolidationArray.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowOfferStamp.h"
//**********************************************************************

class CSQLRepositoryOfferStamp
{
public:
	CSQLRepositoryOfferStamp();

public:
	CSQLResultInfo SelectRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowOfferStamp& SQLRowOS, CDatabase* pDatabase);
	
public:
	void GetStampCounts(__int64 nUserID, CReportConsolidationArray<CStampOfferRSPBuffer>& arrayOfferStamps);
	void GetOfferStampsByUserID(__int64 nUserID, CArray<CSQLRowOfferStamp, CSQLRowOfferStamp>& arrayStamps, CDatabase* pDatabase = NULL);
	void GetOfferStampsByUserIDAndOfferID(__int64 nUserID, int nOfferID, CArray<CSQLRowOfferStamp, CSQLRowOfferStamp>& arrayStamps);
	CSQLResultInfo CountRowsByUserID(__int64 nUserID, CDatabase* pDatabase);
};

//**********************************************************************

