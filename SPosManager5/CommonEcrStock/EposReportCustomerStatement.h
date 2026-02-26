#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "EposReportBase.h"
#include "EposReportConsolCustomerStatement.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CEposReportCustomerStatement : public CEposReport
{
public:
	CEposReportCustomerStatement( CEposSelectArray& SelectArray, int nNameTableIdx );

	virtual void Consolidate();
	virtual bool CreateReport();

public:
	void SetHeaderLine1( const char* sz ){ m_strHeaderLine1 = sz; }
	void SetHeaderLine2( const char* sz ){ m_strHeaderLine2 = sz; }
	void SetShowRollingBalanceFlag( bool b ){ m_bShowRollingBalance = b; }
	int GetConsolidationError(){ return m_nConsolidationError; }

private:
	void AppendBalance( CCSV& csv, double dBalance );
	void AppendRollingBalance( CCSV& csv, double dBalance );
	const char* GetReportTitle( bool bDummy );

private:
	CString m_strHeaderLine1;
	CString m_strHeaderLine2;
	int m_nConsolidationError;
	int m_nNameTableIdx;
	bool m_bShowRollingBalance;
	CReportConsolidationArray<CEposReportConsolCustomerStatement> m_ConsolArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
