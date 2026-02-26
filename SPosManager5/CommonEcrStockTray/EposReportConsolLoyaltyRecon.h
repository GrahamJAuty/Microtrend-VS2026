#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "EposReportBlockMap.h"
#include "EposReportConsolBase.h"
/**********************************************************************/

struct CEposReportConsolLoyaltyRecon : CEposReportConsolBase
{
public:
	CEposReportConsolLoyaltyRecon();
	void Reset();

public:
	int Compare(CEposReportConsolLoyaltyRecon& source, int nHint = 0);
	void Add(CEposReportConsolLoyaltyRecon& source);

public:
	CString m_strDate;
	CString m_strTime;
	int m_nSequenceNo;
	
public:
	int m_nValue;
};

/**********************************************************************/

struct CEposReportConsolLoyaltyReconLog : CEposReportConsolBase
{
public:
	CEposReportConsolLoyaltyReconLog();
	void Reset();

public:
	int Compare(CEposReportConsolLoyaltyReconLog& source, int nHint = 0);
	void Add(CEposReportConsolLoyaltyReconLog& source);

public:
	int m_nDbNo;
	int m_nFolderSet;
	int m_nTerminalNo;
	int m_nSequenceNo;
	CString m_strDate;
	CString m_strTime;
};

/**********************************************************************/

struct CEposReportLoyaltyReconBlock : public CEposReportBlockMapEntity
{
public:
	CEposReportLoyaltyReconBlock();

public:
	bool GotData();

public:
	CReportConsolidationArray<CEposReportConsolLoyaltyRecon>* m_pSales;
	CEposReportConsolLoyaltyRecon m_BlockTotal;
};

/**********************************************************************/
#endif
/**********************************************************************/
