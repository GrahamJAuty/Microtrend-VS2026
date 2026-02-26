#pragma once
/**********************************************************************/
#include "EntityTree.h"
#include "LocationSelectorEntity.h"
#include "PosTrayExportConsolidator.h"
#include "ReportFilters.h"
#include "TimeMap.h"
/**********************************************************************/

class CPosTrayExportConsolidatorSales : public CPosTrayExportConsolidator
{
public:
	CPosTrayExportConsolidatorSales( int nReportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg = NULL );
	
public:	
	void SetPluRangeFlag ( bool bFlag ) { m_bItemRange = bFlag; }
	void SetPluFrom ( __int64 nPluNo ) { m_nItemFrom = nPluNo; }
	void SetPluTo (  __int64 nPluNo ) { m_nItemTo = nPluNo; }
	void SetDepartmentByItemFlag( bool b ) { m_bDepartmentByItem = b; }
	void SetSaleTimeFlag( bool bFlag ) { m_bSaleTime = bFlag; }
	void SetTaxFilter( int nFilter ) { m_nTaxFilter = nFilter; }
	void SetPriceLevelFilter( int nFilter ) { m_nPriceLevelFilter = nFilter; }
	void SetModifierFilter( int nFilter ) { m_nModifierFilter = nFilter; }
	
	void SetCustomerFlag( bool bFlag ){ m_bCustomer = bFlag; }
	void SetVIPSalesFlag( bool bFlag ){ m_bVIPSales = bFlag; }
	
public:
	__int64 CreateExport( const char* szFilename );
	bool PrepareFields();
	void Consolidate();
	
private:
	void GetNodeTotals( int nBranchIdx, int nLeafIdx );
	void ResetTaskSpecific( CPosTrayTask& EmailTask );
		
public:
	CReportFilters m_ReportFilters;
	
private:	
	//PLU AND DEPARTMENT FILTERS
	bool m_bItemRange;
	__int64 m_nItemFrom;
	__int64 m_nItemTo;
	bool m_bDepartmentByItem;
	bool m_bSaleTime;
	int m_nTaxFilter;
	int m_nPriceLevelFilter;
	int m_nModifierFilter;
	
	//LOCATION AND SALES TO INCLUDE	
	bool m_bEposSales;
	bool m_bManualSales;
	bool m_bImportSales;
	bool m_bGuestToTerminal;
	
	//REPORT TYPE AND MISC SETTINGS
	bool m_bCustomer;
	bool m_bVIPSales;

	//CONSOLIDATED SALES	
	CEntityTreeExportSales m_SalesTree;
};

/**********************************************************************/

