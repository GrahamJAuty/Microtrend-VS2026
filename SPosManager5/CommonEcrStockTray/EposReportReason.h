#pragma once
/**********************************************************************/
#include "EposReportBase.h"
#include "ReportConsolidationArray.h"
#include "ReportPluInfoFinder.h"
/**********************************************************************/

struct CEposReportConsolReason
{
public:
	CEposReportConsolReason();
	void Reset();
	
public:
	int Compare ( CEposReportConsolReason& source, int nHint = 0 );
	void Add	( CEposReportConsolReason& source );

public:
	bool CompareServer( CEposReportConsolReason& source );
	
public:
	int m_nServerLocOrDbSort;
	int m_nServerLocOrDbIdx;
	int m_nServerNo;
	CString m_strDate;
	CString m_strTime;
	int m_nLineType;
	int m_nLineNo;

public:
	int m_nLocIdx;
	int m_nTrmIdx;
	__int64 m_nBasePluNo;
	int m_nModifier;
	CString m_strPluText;
	int m_nReasonCode;
	CString m_strReasonText; 
	bool m_bGotValue;
	double m_dValue;
};

/**********************************************************************/

class CEposReportReason : public CEposReport
{
public:
	CEposReportReason( CEposSelectArray& SelectArray, int nType );

	virtual void Consolidate();
	virtual bool CreateReport();

private:
	CReportConsolidationArray<CEposReportConsolReason> m_ConsolArray;

private:
	int m_nLineNo;
	CReportPluInfoFinder m_PluInfoFinder;
};

/**********************************************************************/
