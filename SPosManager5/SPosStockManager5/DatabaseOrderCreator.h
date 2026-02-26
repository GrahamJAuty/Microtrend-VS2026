/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#pragma once
/**********************************************************************/
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/

struct CDbOrderCreatorInfo
{
	int m_nSuppIdx;
	int m_nOrderIdx;
};

/**********************************************************************/

class CDatabaseOrderCreator
{
public:
	CDatabaseOrderCreator( CProcessedOrderPreviewCSVArray& arrayPreview );

	int GetOrderSize(){ return m_arrayOrder.GetSize(); }
	void GetOrderInfo( int n, CDbOrderCreatorInfo& info ){ info = m_arrayOrder.GetAt( n ); }

private:
	bool FindOrder( int nSuppIdx, int nOrderIdx, int &nIdx );

private:
	CArray<CDbOrderCreatorInfo,CDbOrderCreatorInfo> m_arrayOrder;
};

/**********************************************************************/
#endif
/**********************************************************************/
