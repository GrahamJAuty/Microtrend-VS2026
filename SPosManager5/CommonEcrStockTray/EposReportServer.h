#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportBlockMap.h"
#include "EposReportConsolServer.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportServer : public CEposReport
{
public:
	CEposReportServer( CEposSelectArray& SelectArray );
	~CEposReportServer();

	virtual void Consolidate();
	virtual bool CreateReport();
	virtual bool CreateGraph();
	virtual bool CheckGraph();

private:
	void CreateSalesSection( CEposReportServerBlock& Block );

private:
	void WriteBlockTotals( CEposReportServerBlock& Block );
	void WriteConsolidatedTotals( CEposReportConsolServerTax& Block, const char* szType, bool bSystemTotal );
	void PostConsolidateSalesTax( CEposReportServerBlock& Block, CArray<CEposReportConsolServerTax,CEposReportConsolServerTax>& arraySales );
	void GetServerName( CEposReportConsolServerTax& Sales, CString& strServerName );


private:
	void InitialiseConsolidationBlocks();

private:
	bool m_bAllowSystemSupplierLines;
	bool m_bAllowDatabaseSupplierLines;
	bool m_bDiscountDetail;
	bool m_bPremiumDetail;
	bool m_bNetOfTax;
	CEposReportBlockMap m_BlockMap;
	CArray<CEposReportServerBlock,CEposReportServerBlock> m_arrayReportBlocks;
	CArray<int,int> m_arrayTerminalBlocks;
	CReportConsolidationArray<CEposReportConsolTranServer> m_arrayTranServer;
};

/**********************************************************************/
