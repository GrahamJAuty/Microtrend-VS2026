#pragma once
/**********************************************************************/

struct CEposReportConsolTransactionSorted
{
public:
	CEposReportConsolTransactionSorted();
	void Reset();

public:
	int Compare ( CEposReportConsolTransactionSorted& source, int nHint = 0 );
	void Add	( CEposReportConsolTransactionSorted& source );
	
public:
	CString m_strDateTime;
	int m_nSequenceNo;
	int m_nServerNo;
	__int64 m_nInternalNo;
	
public:
	CString m_strDisplayDate;
	CString m_strDisplayTime;
	int m_nTranStartIdx;
	int m_nTranEndIdx;
	int m_nFileIdx;
	int m_nCashRSPVersionNo;
};

/**********************************************************************/

struct CEposReportTransactionSortedLine
{
public:
	CEposReportTransactionSortedLine() { m_nLineType = 0; m_strData = ""; }

public:
	int m_nLineType;
	CString m_strData;
};

/**********************************************************************/
