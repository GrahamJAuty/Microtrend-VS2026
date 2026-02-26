#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolPaymentDetail.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportPaymentDetail : public CEposReport
{
public:
	CEposReportPaymentDetail(CEposSelectArray& SelectArray);
	CEposReportPaymentDetail(CEposSelectArray& SelectArray, const char* szCustomSettings);
	~CEposReportPaymentDetail();

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	void ConsolidateInternal();

private:
	void CreateSalesSection(CEposReportPaymentDetailBlock& Block);

private:
	void InitialiseConsolidationBlocks();
	void WritePaymentLine(CEposReportConsolPaymentDetail& infoConsol, bool bTotal);

private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportPaymentDetailBlock, CEposReportPaymentDetailBlock> m_arrayReportBlocks;
	CArray<int, int> m_arrayTerminalBlocks;

private:
	CEposReportCustomSettingsPaymentDetail m_ReportSettings;
};

/**********************************************************************/
