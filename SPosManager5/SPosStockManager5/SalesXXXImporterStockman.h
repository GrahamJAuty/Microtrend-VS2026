#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "StockAuditRetrySales.h"
/**********************************************************************/

struct CSalesXXXFileInfo
{
	int m_nSpIdx;
	CString m_strFilename;
};

/**********************************************************************/

class CSalesXXXImporterStockman  
{
public:
	CSalesXXXImporterStockman( bool bDiscard );
	
public:
	void ProcessSales( CWordArray& arraySpIdx, COleDateTime& dateFrom, COleDateTime& dateTo );

private:
	void ProcessSalesInternal();
	void ProcessSale( CStringArray& StockDetails, bool bPMS );
	void ProcessDelivery( CStringArray& StockDetails );
	
	void DiscardSalesInternal();
	
private:
	void WriteHistoryBuffer();
	void WriteAuditBuffers();

private:
	CStringArray m_arraySalesAuditBuffer;
	CStringArray m_arrayWasteAuditBuffer;
	CStringArray m_arrayDeliveryAuditBuffer;
	CStringArray m_arraySalesHistoryBuffer;

private:
	CStockAuditRetrySales m_RetryAudit;
	CString m_strReportDate;
	int m_nSpIdx;

private:
	CArray<CSalesXXXFileInfo,CSalesXXXFileInfo> m_arrayFileInfo;
	__int64 m_nTotalFileSize;
	__int64 m_nProgress;

public:
	int m_nFilesFound;
	int m_nFilesOpened;
	int m_nDataLines;
	int m_nSalesLines;
	int m_nWasteLines;
	int m_nDeliveryLines;
	int m_nFiledLines;

private:
	bool m_bDiscard;
};

/**********************************************************************/
#endif
/**********************************************************************/
