#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CSalesHistoryRegionLocColMap
{
public:
	CSalesHistoryRegionLocColMap();
	void RemoveAll();

public:
	void MapLocToColumn( int nLocIdx, int nColumnIdx );
	int GetColumnCount( int nLocIdx );
	int GetColumnIdx( int nLocIdx, int nPos );

private:
	void ResetLastLocIdx();
	void UpdateLastLoc( int nLocIdx );

private:
	CReportConsolidationArray<CSortedIntAndInt> m_Map;
	
private:
	int m_nLastLocIdx;
	int m_nLastLocColStart;
	int m_nLastLocColCount;
};

/**********************************************************************/
#endif
/**********************************************************************/
