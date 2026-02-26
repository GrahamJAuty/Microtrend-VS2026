#pragma once
/**********************************************************************/
#include "CashRSPVersionChecker.h"
#include "EposReportPMSModes.h"
/**********************************************************************/
#include "PosTrayExportConsolidator.h"
/**********************************************************************/

class CPosTrayExportConsolidatorPayment : public CPosTrayExportConsolidator
{
public:
	CPosTrayExportConsolidatorPayment( int nReportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDialog = NULL );
	
public:
	__int64 CreateExport( const char* szFilename );
	bool PrepareFields();
	void Consolidate();

private:
	void GetNodeTotals( int nBranchIdx, int nLeafIdx );
	void ResetTaskSpecific( CPosTrayTask& EmailTask );
							
private:
	CCashRSPVersionChecker m_CashRSPVersionChecker;
	CEposReportPMSModes m_PMSModes;
	CEntityTreeExportPayment m_PaymentTree;
};

/**********************************************************************/

