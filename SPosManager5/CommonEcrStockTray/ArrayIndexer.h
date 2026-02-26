#pragma once
/**********************************************************************/
#include "..\SPosStockManager5\StockFilterArray.h"
/**********************************************************************/

class CArrayIndexer
{
public:
	CArrayIndexer();

public:
	void SetNativeOrderFlag( bool b ){ m_bNativeOrder = b; }
	
public:
	int GetSortedIdxForNativeIdx( int nIdx );
	int GetNativeIdxForSortedIdx( int nIdx );

protected:
	bool m_bNativeOrder;
	CArray<int,int> m_arrayNativeToSorted;
	CArray<int,int> m_arraySortedToNative;
};

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CStockArrayIndexer : public CArrayIndexer
{
public:
	CStockArrayIndexer();

public:
	bool RebuildIndex( int nDbIdx );
	bool RebuildIndex( CStockFilterArray* pFilter );

private:
	int m_nCurrentDbIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
