#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolSptBookCascade.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportSptBookCascade : public CEposReport
{
public:
	CEposReportSptBookCascade( CEposSelectArray& SelectArray );
	~CEposReportSptBookCascade();

	virtual void Consolidate();
	virtual bool CreateReport();
	
private:
	void CreateSalesSection( CEposReportSptBookCascadeBlock& Block );

private:	
	void ShowDeptTotal( int nLastTotalLevel, CEposReportConsolSptBookCascade& ConsolFields, CEposReportConsolSptBookCascade& ConsolValue );
	void ShowTotal( int nLevel, CEposReportConsolSptBookCascade& ConsolFields, CEposReportConsolSptBookCascade& ConsolValue );

private:
	void InitialiseConsolidationBlocks();
	
private:
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportSptBookCascadeBlock,CEposReportSptBookCascadeBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
};

/**********************************************************************/

