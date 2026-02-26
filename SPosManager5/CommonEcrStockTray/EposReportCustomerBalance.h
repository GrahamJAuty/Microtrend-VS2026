#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolCustomerBalanceID
{
public:
	CEposReportConsolCustomerBalanceID();
	void Reset();

public:
	int Compare ( CEposReportConsolCustomerBalanceID& source, int nHint = 0 );
	void Add	( CEposReportConsolCustomerBalanceID& source );

public:
	bool CompareLocIdx ( CEposReportConsolCustomerBalanceID& source );
	
public:
	int m_nLocIdx;
	int m_nSortType;
	int m_nSortIdx;
	int m_nCustomerID;
	
public:
	double m_dBalance;
};

/**********************************************************************/

class CEposReportCustomerBalance : public CEposReport
{
public:
	CEposReportCustomerBalance( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	const char* GetReportTitle( bool bDummy );

private:
	void WriteTotal( const char* szTotal, double dTotal );

private:
	CReportConsolidationArray<CEposReportConsolCustomerBalanceID> m_ConsolArrayStage1;
	CReportConsolidationArray<CEposReportConsolCustomerBalanceID> m_ConsolArrayStage2;
};

/**********************************************************************/
