#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "ProcessedOrderBaseReport.h"
/**********************************************************************/

class CProcessedOrderReportDelivery : public CProcessedOrderBaseReport
{
public:
	CProcessedOrderReportDelivery( CWordArray& wSuppSelectArray );
	void SetPageBreakFlag( bool bFlag ) { m_bPageBreakFlag = bFlag; }
	void SetLatestFlag( bool bFlag ) { m_bLatestFlag = bFlag; }
	void SetStockCodeFrom( const char* sz ) { m_strStockCodeFrom = sz; }
	void SetStockCodeTo( const char* sz ) { m_strStockCodeTo = sz; }
	virtual bool CreateReport( int nDbIdx );

private:
	void WriteDeliveries( CReportFile& ReportFile );
	void WriteOrderTotal( CReportFile& ReportFile, double dTotal );

private:
	int m_nDbIdx;
	CString m_strSupplierName;
	CWordArray m_wSuppSelectArray;
	bool m_bInRange;
	bool m_bLegacyOrder;
	bool m_bLatestFlag;
	bool m_bPageBreakFlag;
	int m_nOrderNum;
	CString m_strStockCodeFrom;
	CString m_strStockCodeTo;
};

/**********************************************************************/
#endif
/**********************************************************************/
