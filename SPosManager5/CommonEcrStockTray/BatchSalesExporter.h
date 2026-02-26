#pragma once
/**********************************************************************/
#include "CashRSPCSVLines.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CBatchSalesExporterItem
{
public:
	CBatchSalesExporterItem();

public:
	void Reset();
	int Compare ( CBatchSalesExporterItem& source, int nHint = 0 );
	void Add	( CBatchSalesExporterItem& source );
	
public:
	CString m_strDate;
	__int64 m_nPluNo;
	double m_dQty;
	double m_dVal;
};

/**********************************************************************/

class CBatchSalesExporter  
{
public:
	CBatchSalesExporter();
	
public:
	void ExportBatchSalesIdleTime();
	void ExportBatchSalesExternal( int nNwkIdx );
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }
	void GotoNextNetwork();
	
private:
	void CheckNwkIdx();

private:
	void ProcessTransactionItem( CCSVEposTermLine& csvIn, CReportConsolidationArray<CBatchSalesExporterItem>& arraySales, CString& strSaleDate );
	
private:
	int m_nNwkIdx;
	bool m_bReadyToGo;
};

/**********************************************************************/
extern CBatchSalesExporter BatchSalesExporter;
/**********************************************************************/

