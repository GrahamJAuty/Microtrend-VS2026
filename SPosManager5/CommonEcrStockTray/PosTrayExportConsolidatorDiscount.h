#pragma once
/**********************************************************************/
#include "ReportFilters.h"
/**********************************************************************/
#include "PosTrayExportConsolidator.h"
/**********************************************************************/

class CPosTrayExportConsolidatorDiscount : public CPosTrayExportConsolidator
{
public:
	CPosTrayExportConsolidatorDiscount( int nReportDateType, CEposSelectArray& SelectArray, bool& bQuitNow, void* pHostDlg = NULL );
	
public:
	void SetPluRangeFlag ( bool bFlag ) { m_bItemRange = bFlag; }
	void SetPluFrom ( __int64 nPluNo ) { m_nItemFrom = nPluNo; }
	void SetPluTo (  __int64 nPluNo ) { m_nItemTo = nPluNo; }
	void SetDepartmentByItemFlag( bool b ) { m_bDepartmentByItem = b; }
	void SetSaleTimeFlag( bool bFlag ) { m_bSaleTime = bFlag; }
	void SetTaxFilter( int nFilter ) { m_nTaxFilter = nFilter; }
	void SetPriceLevelFilter( int nFilter ) { m_nPriceLevelFilter = nFilter; }
	void SetModifierFilter( int nFilter ) { m_nModifierFilter = nFilter; }
	void SetOfferFilter( int nFilter ) { m_nOfferFilter = nFilter; }
	
	void SetCustomerFlag( bool bFlag ){ m_bCustomer = bFlag; }
	void SetVIPSalesFlag( bool bFlag ){ m_bVIPSales = bFlag; }
	
public:
	__int64 CreateExport( const char* szFilename );
	bool PrepareFields();
	void Consolidate();

public:
	CReportFilters m_ReportFilters;
	CTableReportHelpers m_TableReportHelpers;
	
private:
	void GetNodeTotals( int nBranchIdx, int nLeafIdx );
	void ResetTaskSpecific( CPosTrayTask& EmailTask );
			
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
	int m_nOfferFilter;
	
	//LOCATION AND SALES TO INCLUDE	
	bool m_bEposSales;
	bool m_bManualSales;
	bool m_bImportSales;
	bool m_bGuestToTerminal;
	
	//REPORT TYPE AND MISC SETTINGS
	bool m_bCustomer;
	bool m_bVIPSales;

	//CONSOLIDATED SALES	
	CEntityTreeExportDiscount m_DiscountTree;
};

/**********************************************************************/

