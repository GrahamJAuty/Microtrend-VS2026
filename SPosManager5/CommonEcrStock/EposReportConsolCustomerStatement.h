#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

struct CEposReportConsolCustomerStatement
{
public:
	CEposReportConsolCustomerStatement();
	void Reset();

public:
	int Compare ( CEposReportConsolCustomerStatement& source, int nHint = 0 );
	void Add	( CEposReportConsolCustomerStatement& source );
	
public:
	int m_nBlock;
	CString m_strDate;
	CString m_strTime;
	int m_nLineNo;
	int m_nType;
	
public:
	double m_dBalance;
};

/**********************************************************************/
#endif
/**********************************************************************/
