#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\CommonEcrStock\DataAccessHelpers.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CDataAccessHelpersStockData : public CDataAccessHelpers
{
public:
	CDataAccessHelpersStockData();

public:
	void EditStock( int nDbNo, bool bFromButton );
	void EditStockLevel ( int nSpIdx, int nType, bool bFromButton );
	void EditManualSales( int nSpIdx, int nType, bool bFromButton );
	void EditMiscStockLevels ( int nSpIdx, int nType, bool bFromButton );

public:
	void HandleStockReports( int nPresetNo, bool bAutoRunPreset, CLocationSelectorEntity& LocSelEntity );
	void HandleStockAudits( int nPresetNo, bool bAutoRunPreset, CLocationSelectorEntity& LocSelEntity);

private:
	void EditStockInternal( int nDbIdx, bool bReadOnly );
	int EditStockLevelInternal( int nSpIdx, int nType );
	int EditManualSalesInternal( int nSpIdx, int nType );
	int EditMiscStockLevelsInternal( int nSpIdx, int nType );

public:
	void BuildDbTransferCounts();
	int GetDbTransferTargets( int nDbIdx );

private:
	CArray<int,int> m_arrayDbTransferTargets;
};

/**********************************************************************/
#endif
/**********************************************************************/
