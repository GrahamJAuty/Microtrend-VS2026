#pragma once
/**********************************************************************/

struct CEposReportConsolAgeVerifyNoSale
{
public:
	CEposReportConsolAgeVerifyNoSale();
	void Reset();

public:
	int Compare ( CEposReportConsolAgeVerifyNoSale& source, int nHint = 0 );
	void Add	( CEposReportConsolAgeVerifyNoSale& source );

public:
	bool CompareServer( CEposReportConsolAgeVerifyNoSale& source );
	
public:
	int m_nServerLocOrDbSort;
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	CString m_strDate;
	CString m_strTime;
	
public:
	int m_nLimit;
	int m_nResult;
	int m_nReasonIdx;
};

/**********************************************************************/
