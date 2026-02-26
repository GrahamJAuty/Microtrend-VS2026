#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CustomerCSVArray.h"
/**********************************************************************/

class CIdleTimeManager
{
public:
	CIdleTimeManager(void);

public:
	void DoNextTask( CWnd* pMainWnd );
	
private:
	void AutoImportPluCSV();
	void AutoImportCustomer();
	void AutoImportBarcode();
	void IDraughtExport( CWnd* pMainWnd );
	void SageExport( CWnd* pMainWnd );
	void ExportBatchSales();

#ifdef STOCKMAN_SYSTEM
	void ProcessSalesXXX();
	void SyncStockLevels();
	void DorbiereImport();
#endif

private:
	CArray<int,int> m_arrayTasks;
	COleDateTime m_timeNextCheck;
	int m_nNextIdleTask;
};

extern CIdleTimeManager IdleTimeManager;
/**********************************************************************/
#endif
/**********************************************************************/
