#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CPluSalesSorterNonEcr 
{
public:
	CPluSalesSorterNonEcr( int nDbIdx, CStringArray& AuditedSales, const char* szLogTime );

private:
	void UpdateSales ( int nDbIdx, CStringArray& AuditedSales );

private:
	CString m_strLogTime;
	CString m_strReportDate;
	CString m_strReportTime;
};

/**********************************************************************/
#endif
/**********************************************************************/

