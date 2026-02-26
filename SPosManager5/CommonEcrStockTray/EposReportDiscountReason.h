#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolDiscount.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportDiscountReason : public CEposReport
{
public:
	CEposReportDiscountReason( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void WriteTotals( bool bGrandTotal, int nDiscount, int nMarkdown, int nPremium, int nUnknown );
	bool ConsolidateSubtotalDiscount( CTermFileListInfo& infoFile, CString& strThisSaleDate, CReasonInfo& ReasonInfo );
	bool ConsolidateItemDiscount( CTermFileListInfo& infoFile, CString& strThisSaleDate, CReasonInfo& ReasonInfo );
	bool CheckServer( CTermFileListInfo& infoFile, CEposReportConsolDiscount& infoConsol, int nServerNo );

private:
	CReportConsolidationArray<CEposReportConsolDiscount> m_ConsolArray;
	int m_nLineNo;
};

/**********************************************************************/
