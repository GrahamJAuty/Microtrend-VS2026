#pragma once
/**********************************************************************/
#define MISCREASON_SERVER_SALES_LINE 1
#define MISCREASON_SERVER_SALES_TOTAL 2
/**********************************************************************/
#define MISCREASON_TOTAL_SALES 3
/**********************************************************************/

struct CEposReportConsolMiscReason
{
public:
	CEposReportConsolMiscReason();
	void Reset();
	
public:
	int Compare ( CEposReportConsolMiscReason& source, int nHint = 0 );
	void Add	( CEposReportConsolMiscReason& source );

public:
	bool CompareServer( CEposReportConsolMiscReason& source );
	
public:
	bool m_bGrandTotal;
	int m_nServerLocOrDbSort;
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	int m_nBlock;
	CString m_strReportDate;
	int m_nTableNo;
	int m_nSeqNo;
	CString m_strReportTime;
	CString m_strOtherTime;
	int m_nLineNo;
	
public:
	__int64 m_nPluNo;
	int m_nModifier;
	int m_nDeptNo;
	CString m_strPluText;
	double m_dModQty;
	double m_dPluQty;
	double m_dValue;
	CString m_strReason;	
};

/**********************************************************************/
