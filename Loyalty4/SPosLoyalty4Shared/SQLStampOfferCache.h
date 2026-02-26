#pragma once
//**********************************************************************
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
//**********************************************************************
#include "SQLTable_StampOfferInfo/SQLRowStampOfferInfo.h"
//**********************************************************************

class CSQLStampOfferCache
{
public:
	void Reset()
	{
		m_arrayStatus.RemoveAll();
		m_arrayOffers.RemoveAll();
	}

	bool GetStampOfferRow(int nStampOfferID, CSQLRowStampOfferInfo& Row);
	void InsertRow(CSQLRowStampOfferInfo& Row);

private:
	CReportConsolidationArray<CSortedIntByInt> m_arrayStatus;
	CArray<CSQLRowStampOfferInfo> m_arrayOffers;
};

//**********************************************************************