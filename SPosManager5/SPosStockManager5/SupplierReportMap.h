#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CSupplierReportMap  
{
public:
	CSupplierReportMap();
	void Clear();
	void CreateMap( CWordArray& arraySuppIdx );

	int GetSortIdxBySuppNo( int nSuppNo );
	int GetSuppIdxBySortIdx( int nSortIdx );

public:
	CArray<int,int> m_arraySuppIdxToSortIdx;
	CArray<int,int> m_arraySortIdxToSuppIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
