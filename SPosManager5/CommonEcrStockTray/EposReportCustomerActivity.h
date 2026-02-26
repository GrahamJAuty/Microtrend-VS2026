#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CEposReportConsolCustomerActivityID
{
public:
	CEposReportConsolCustomerActivityID();
	void Reset();

public:
	int Compare ( CEposReportConsolCustomerActivityID& source, int nHint = 0 );
	void Add	( CEposReportConsolCustomerActivityID& source );
	
public:
	int m_nLocIdx;
	int m_nCustomerID;
	CString m_strDate;
	CString m_strTime;
	int m_nAction;
	
public:
	double m_dValue;
};

/**********************************************************************/

struct CEposReportConsolCustomerActivitySortIdx
{
public:
	CEposReportConsolCustomerActivitySortIdx();
	void Reset();

public:
	int Compare ( CEposReportConsolCustomerActivitySortIdx& source, int nHint = 0 );
	void Add	( CEposReportConsolCustomerActivitySortIdx& source );

public:
	bool CompareCustomer ( CEposReportConsolCustomerActivitySortIdx& source );
	
public:
	int m_nLocSort;
	int m_nSortIdxType;
	int m_nSortIdxNum;
	CString m_strDate;
	CString m_strTime;
	int m_nAction;
	int m_nCustomerID;
	
public:
	double m_dValue;
};

/**********************************************************************/

class CEposReportCustomerActivity : public CEposReport
{
public:
	CEposReportCustomerActivity( CEposSelectArray& SelectArray );

	virtual void Consolidate();
	virtual bool CreateReport();

	void WriteTotal( const char* sz, double dTotalSpend, double dTotalPayment );
	
private:
	CReportConsolidationArray<CEposReportConsolCustomerActivityID> m_ConsolArrayID;
	CReportConsolidationArray<CEposReportConsolCustomerActivitySortIdx> m_ConsolArraySortIdx;
};

/**********************************************************************/
