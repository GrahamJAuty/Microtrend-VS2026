#pragma once
/**********************************************************************/

struct CEposReportConsolCustomerHistory
{
public:
	CEposReportConsolCustomerHistory();
	
public:
	void Reset();
	int Compare ( CEposReportConsolCustomerHistory& source, int nHint = 0 );
	void Add	( CEposReportConsolCustomerHistory& source );
	
public:
	int m_nTransactionNo;
	int m_nReportLineType;
	__int64 m_nReportLineSubType;

public:
	double m_dQty;
	double m_dVal;
};

/**********************************************************************/
