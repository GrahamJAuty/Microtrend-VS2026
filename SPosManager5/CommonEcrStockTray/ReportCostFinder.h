#pragma once
/**********************************************************************/

struct CReportCostFinderPluInfo
{
public:
	CReportCostFinderPluInfo() {
		m_nCostType = 0;
		m_dMasterCost = 0.0;
		m_strLocCosts = "";
	};

public:
	int m_nCostType;
	double m_dMasterCost;
	CString m_strLocCosts;
};

/**********************************************************************/

struct CReportCostFinderLocInfo
{
public:
	int m_nLocIdx;
	int m_nColumn;
};

/**********************************************************************/

class CReportCostFinder
{
public:
	CReportCostFinder(void);

public:
	void Init();
	double GetPluCostByPluNo( __int64 nPluNo, int nLocIdx );
	double GetPluCostByPluIdx( int PluIdx, int nLocIdx );

private:
	void AddNoPriceSetLocation();
	
private:
	CArray<CReportCostFinderPluInfo,CReportCostFinderPluInfo> m_arrayPluInfo;
	CArray<CReportCostFinderLocInfo,CReportCostFinderLocInfo> m_arrayLocations;
	
private:
	__int64 m_nPreviousPluNo;
	int m_nPreviousPluIdx;
	int m_nPreviousLocIdx;
	int m_nPreviousLocInfoIdx;
	int m_nLocationsWithPriceSets;
};

/**********************************************************************/
