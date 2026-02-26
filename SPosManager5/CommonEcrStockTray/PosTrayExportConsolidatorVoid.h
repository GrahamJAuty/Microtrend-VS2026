#pragma once
/**********************************************************************/
#include "EntityTree.h"
#include "EposReportPMSModes.h"
#include "PosTrayExportConsolidator.h"
/**********************************************************************/

class CPosTrayExportConsolidatorVoid : public CPosTrayExportConsolidator
{
public:
	CPosTrayExportConsolidatorVoid( int nReportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg = NULL );
	void SetSaleTimeFlag( bool bFlag ) { m_bSaleTime = bFlag; }
			
public:
	__int64 CreateExport( const char* szFilename );
	bool PrepareFields();
	void Consolidate();

private:
	void GetNodeTotals( int nBranchIdx, int nLeafIdx );
	void ResetTaskSpecific( CPosTrayTask& EmailTask );

private:	
	CEposReportPMSModes m_PMSModes;
	bool m_bSaleTime;
	CEntityTreeExportVoid m_VoidTree;
};

/**********************************************************************/

