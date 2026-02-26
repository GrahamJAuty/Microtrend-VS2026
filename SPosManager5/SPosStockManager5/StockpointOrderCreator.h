/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#pragma once
/**********************************************************************/
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/

struct CSpOrderCreatorInfo
{
	int m_nSpIdx;
	int m_nSuppIdx;
	int m_nStockIdx;
	int m_nStockSuppIdx;
	int m_nDUItems;
	double m_dDUItemSize;
	double m_dSpOrder;
};

/**********************************************************************/

class CStockpointOrderCreator
{
public:
	CStockpointOrderCreator( CProcessedOrderPreviewCSVArray& arrayPreview );

	int GetOrderSize(){ return m_arrayOrder.GetSize(); }
	void GetOrderInfo( int n, CSpOrderCreatorInfo& info ){ info = m_arrayOrder.GetAt( n ); }

private:
	bool FindOrder( int nSpIdx, int nSuppIdx, int nStockIdx, int &nIdx );

private:
	CArray<CSpOrderCreatorInfo,CSpOrderCreatorInfo> m_arrayOrder;
};

/**********************************************************************/
#endif
/**********************************************************************/
