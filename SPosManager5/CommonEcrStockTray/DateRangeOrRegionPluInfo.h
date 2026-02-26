#pragma once
/**********************************************************************/
#define ZEROSKIP_NONE 0
#define ZEROSKIP_ITEM 1
#define ZEROSKIP_BAND 2
/**********************************************************************/

class CDateRangeOrRegionPluInfo
{
public:
	CDateRangeOrRegionPluInfo();
	int GetDatabaseTaxBand( int nMod );

public:
	static bool CheckTaxBand( int nTaxFilter, int nTaxBand );

public:
	bool m_bShowItem;
	int m_nDeptNo;
	double m_dCostPerPlu;
	CString m_strDisplayPluNo;
	CString m_strReportText;
	int m_nDatabaseTaxBand[10];
};

/**********************************************************************/

struct CDateRangeOrRegionColumnInfo
{
	int m_nDbIdx;
	int m_nType;
	int m_nTableIdx;
};

/**********************************************************************/

struct CDateRangeInfo
{
	CString m_strName;
	COleDateTime m_dateStart;
	COleDateTime m_dateEnd;
};

/**********************************************************************/
