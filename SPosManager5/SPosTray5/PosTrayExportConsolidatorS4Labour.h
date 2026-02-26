#pragma once
/**********************************************************************/
#include "EntityTree.h"
#include "LocationSelectorEntity.h"
#include "PosTrayExportConsolidator.h"
#include "TimeMap.h"
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/

class CPosTrayExportConsolidatorS4Labour : public CPosTrayExportConsolidator
{
public:
	CPosTrayExportConsolidatorS4Labour( CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg = NULL );
		
public:
	__int64 CreateExport( const char* szFilename, bool& bResult );
	bool PrepareFields();
	void Consolidate();

public:
	void ResetS4Labour( COleDateTime dateExport, CSessionCSVRecord& ReportSession );
	void SetUseTermIDFlag( bool b ){ m_bUseTermID = b; }
	
private:
	void GetNodeTotals( int nBranchIdx, int nLeafIdx );
	bool WantHeader(){ return FALSE; }
	bool UseDirectLocIdx(){ return TRUE; }

private:		
	//CONSOLIDATED SALES	
	CEntityTreeExportS4Labour m_SalesTree;
	bool m_bUseTermID;
};

/**********************************************************************/
#endif
/**********************************************************************/
