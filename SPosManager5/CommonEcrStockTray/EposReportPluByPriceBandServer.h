#pragma once
/**********************************************************************/
#include "EposReportPluByPriceBandBase.h"
#include "EposSelectArray.h"
#include "PluCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportPluByPriceBandServer : public CEposReportPluByPriceBandBase
{
public:
	CEposReportPluByPriceBandServer(CEposSelectArray& SelectArray, CString strCustomSettings);

private:
	virtual void InitialiseConsolidationBlocks();
	virtual void SetConsolidationServer( int nDbIdx, int nLocIdx, int nServerNo );

public:
	virtual bool CreateReport();

private:
	CReportConsolidationArray<CEposReportConsolServerBlockMap> m_ServerBlockMap;
	CEposReportPluByPriceBandInfo m_Totals2;
};

/**********************************************************************/
