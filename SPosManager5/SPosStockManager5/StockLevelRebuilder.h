#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CStockLevelRebuilderInfo
{
public:
	CStockLevelRebuilderInfo();
	void Reset();

public:
	int Compare ( CStockLevelRebuilderInfo& source, int nHint = 0 );
	void Add	( CStockLevelRebuilderInfo& source );
	
public:
	CString m_strStockCode;
	
public:
	CString m_strDescription;
	CString m_strActions;
};

/**********************************************************************/

struct CStockLevelRebuilderAdjust
{
public:
	double m_dAdjustQty;
	double m_dAdjustVal;
	double m_dStocktakeAdjust;
};

/**********************************************************************/

class CStockLevelRebuilder
{
public:
	CStockLevelRebuilder();

public:
	bool RebuildStockLevels();

private:
	bool MatchStockpoint ( int nSpIdx );
	void ProcessStockpoint( int nSpIdx, int nPass );
	
private:
	bool ProcessHeader( const char* szHeader, int nPass );
	void ProcessLine( int nSpIdx, bool bTransferIn, const char* szLine, int nPass );
	void ProcessAction( int nSpIdx, int nType, int nMode, double dQty, double dVal, const char* szStockCode );
	
private:
	void GetAuditFileList( int nSpIdx, CReportConsolidationArray<CSortedStringItem>& arrayFiles, __int64& nTotalFileSize );
	
private:
	int m_nDbIdx;
	int m_nConLevel;
	int m_nTableNo;

private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;

private:
	CReportConsolidationArray<CStockLevelRebuilderInfo> m_arrayMovements;
	CReportConsolidationArray<CSortedStringByString> m_arrayStockChangeMap;
	CArray<__int64,__int64> m_arrayNeverRedirect;
	CStringArray m_arrayErrors;
	CStringArray m_arrayRedirects;
	int m_nSalesType;
	bool m_bStocktakeAdjust;
	
};

/**********************************************************************/
#endif
/**********************************************************************/
