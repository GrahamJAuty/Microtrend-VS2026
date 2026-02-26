#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolMixMatchItem.h"
#include "EposReportConsolTran.h"
#include "EposReportCustomSettings.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
#include "UnderlineMaker.h"
/**********************************************************************/

class CEposReportMixMatchItem : public CEposReport
{
public:
	CEposReportMixMatchItem( CEposSelectArray& SelectArray, int nReportType );
	CEposReportMixMatchItem( CEposSelectArray& SelectArray, int nReportType, const char* szCustomSettings );
	~CEposReportMixMatchItem();

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	void ProcessPendingItems(int nLocIdx);
	void CreateSalesSection( CEposReportMixMatchItemBlock& Block );
	void WriteTotalLine( CEposReportConsolMMDiscountedItem& Sales, const char* szType );

private:
	void InitialiseConsolidationBlocks();
	
private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportMixMatchItemBlock,CEposReportMixMatchItemBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolMMTranByDateTime> m_arrayPendingTranByDateTime;
	CReportConsolidationArray<CEposReportConsolMMDiscountedItem> m_arrayPendingDiscountedItems;
	CReportConsolidationArray<CEposReportConsolMMTranByOfferItem> m_arrayPendingTranByOfferItem;
	CReportConsolidationArray<CEposReportConsolMMCoversByTran> m_arrayPendingCoversByTran;
	CReportConsolidationArray<CEposReportConsolMMCoversByOffer> m_arrayPendingCoversByOffer;
	CReportConsolidationArray<CEposReportConsolMMUsageCount> m_arrayPendingMMUsageCount;
	CReportConsolidationArray<CEposReportConsolMMUniqueID> m_arrayPendingMMUniqueID;
	CEposReportCustomSettingsMixMatch m_ReportSettings;
	CReportPluInfoFinder m_PluInfoFinder;

private:
	bool m_bWantCovers;
};

/**********************************************************************/

