#pragma once
/**********************************************************************/

struct CEposReportConsolEODStatus
{
public:
	CEposReportConsolEODStatus();
	void Reset();

public:
	int Compare ( CEposReportConsolEODStatus& source, int nHint = 0 );
	void Add	( CEposReportConsolEODStatus& source );
	
public:
	int m_nLocIdx;
	
public:
	int m_nEODCount;
	CString m_strLatestEOD;
};

/**********************************************************************/
