#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

struct CStockCodeSearch
{
public:
	CStockCodeSearch();
	
public:
	CString m_strStockCodeEntered;
	CString m_strStockCodeToUse;
	bool m_bSearchFromStartTick;
	bool m_bSearchFromStart;
};

/**********************************************************************/
#endif
/**********************************************************************/
