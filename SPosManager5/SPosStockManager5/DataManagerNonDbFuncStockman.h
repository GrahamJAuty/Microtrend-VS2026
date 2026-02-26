#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SalesHistoryCSVArrayStock.h"
#include "StockReportSortOrderCSVArray.h"
/**********************************************************************/
#define STOCKREPORT_SORTORDER_SALES 0
#define STOCKREPORT_SORTORDER_TOP_100_SALES 1
#define STOCKREPORT_SORTORDER_ESTIMATED_PROFIT 2
#define STOCKREPORT_SORTORDER_REORDER 3
#define STOCKREPORT_SORTORDER_APPARENT_QTY 4
#define STOCKREPORT_SORTORDER_TOP_100_APPARENT 5
#define STOCKREPORT_SORTORDER_APPARENT_VALUE 6
#define STOCKREPORT_SORTORDER_OPENING_VALUE 7
#define STOCKREPORT_SORTORDER_DELIVERY 8
#define STOCKREPORT_SORTORDER_RETURNS 9
#define STOCKREPORT_SORTORDER_ADJUST 10
#define STOCKREPORT_SORTORDER_SHEET 11
#define STOCKREPORT_SORTORDER_PRICE_LIST 12
#define STOCKREPORT_SORTORDER_RETAIL_SUMMARY 13
#define STOCKREPORT_SORTORDER_COST_SUMMARY 14
#define STOCKREPORT_SORTORDER_TRANSFER_VALUE 15
#define STOCKREPORT_SORTORDER_TRANSFER_PERIOD 16
#define STOCKREPORT_SORTORDER_SUPPLIER 17
#define STOCKREPORT_SORTORDER_APPARENT_SUMMARY 18
#define STOCKREPORT_SORTORDER_OVERSTOCK_VALUE 19
#define STOCKREPORT_SORTORDER_APPARENT_QTY_DU 20
#define STOCKREPORT_SORTORDER_APPARENT_SUMMARY_DU 21
/**********************************************************************/
#define MAX_STOCKREPORT_SORTORDERS 22
/**********************************************************************/
#define STOCKTAKE_SORTORDER_CLOSING_VALUE 100
#define STOCKTAKE_SORTORDER_VARIANCE 101
#define STOCKTAKE_SORTORDER_RECONCILE 102
#define STOCKTAKE_SORTORDER_RECONCILE_CATEGORY 103
#define STOCKTAKE_SORTORDER_TRANSFER_CLOSING 104
#define STOCKTAKE_SORTORDER_SHEET 105
#define STOCKTAKE_SORTORDER_ESTIMATE_ALLOWANCE 106
/**********************************************************************/
#define MAX_STOCKTAKE_SORTORDERS 6
/**********************************************************************/
#define MAX_STOCKREPORT_PRESETS 2
/**********************************************************************/

class CDataManagerNonDbFuncStockman : virtual public CDataManagerBase
{
public:
	CDataManagerNonDbFuncStockman();

public:
	bool OpenAdjustText( int nType, CDataManagerInfo& info );
	bool OpenCustomFieldNames( int nType, CDataManagerInfo& info );
	bool OpenSalesHistoryStock( int nType, CDataManagerInfo& info );
	bool OpenStockUnit( int nType, CDataManagerInfo& info );
	bool OpenSubUnit( int nType, CDataManagerInfo& info );
	bool OpenSystemCategory( int nType, CDataManagerInfo& info );
	bool OpenSystemCategorySet( int nType, CDataManagerInfo& info );
	bool OpenSystemCustomFieldSet( int nFieldNo, int nType, CDataManagerInfo& info );
	
	bool OpenAdjustTextBuffer( CDataManagerInfo& info );
	bool OpenSalesHistoryStockBuffer( CDataManagerInfo& info );
	bool OpenStockUnitBuffer( CDataManagerInfo& info );
	bool OpenSubUnitBuffer( CDataManagerInfo& info );
	
	bool CloseAdjustText( CDataManagerInfo& info );
	bool CloseCustomFieldNames( CDataManagerInfo& info );
	bool CloseSalesHistoryStock( CDataManagerInfo& info );
	bool CloseStockUnit( CDataManagerInfo& info );
	bool CloseSubUnit( CDataManagerInfo& info );
	bool CloseSystemCategory( CDataManagerInfo& info );
	bool CloseSystemCategorySet( CDataManagerInfo& info );
	bool CloseSystemCustomFieldSet( int nFieldNo, CDataManagerInfo& info );
	
	bool CloseAdjustTextBuffer( CDataManagerInfo& info );
	bool CloseSalesHistoryStockBuffer( CDataManagerInfo& info );
	bool CloseStockUnitBuffer( CDataManagerInfo& info );
	bool CloseSubUnitBuffer( CDataManagerInfo& info );

	bool WriteAdjustText( CDataManagerInfo& info );
	bool WriteCustomFieldNames( CDataManagerInfo& info );
	bool WriteSalesHistoryStock( CDataManagerInfo& info);
	bool WriteStockUnit( CDataManagerInfo& info);
	bool WriteSubUnit( CDataManagerInfo& info);
	bool WriteSystemCategory( CDataManagerInfo& info);
	bool WriteSystemCategorySet( CDataManagerInfo& info);
	bool WriteSystemCustomFieldSet( int nFieldNo, CDataManagerInfo& info);

	bool WriteAdjustTextBuffer( CDataManagerInfo& info );
	bool WriteSalesHistoryStockBuffer( CDataManagerInfo& info);
	bool WriteStockUnitBuffer( CDataManagerInfo& info);
	bool WriteSubUnitBuffer( CDataManagerInfo& info);

public:
	void SetActiveStockReportSortOrderType( int nType );
	int GetActiveStockReportSortOrderType(){ return m_nActiveStockReportSortOrderType; }

	CStockReportSortOrderCSVArray* GetActiveStockReportSortOrder();
	CStockReportSortOrderCSVArray* GetActiveStockReportSortOrderBuffer();

	void OpenAllStockReportSortOrders();
	void CloseAllStockReportSortOrders();

	void OpenAllStockTakeSortOrders();
	void CloseAllStockTakeSortOrders();

	bool OpenActiveStockReportSortOrder( int nType, CDataManagerInfo& info );
	bool OpenActiveStockReportSortOrderBuffer( CDataManagerInfo& info );
	bool CloseActiveStockReportSortOrder( CDataManagerInfo& info );
	bool CloseActiveStockReportSortOrderBuffer( CDataManagerInfo& info );
	bool WriteActiveStockReportSortOrder( CDataManagerInfo& info );
	bool WriteActiveStockReportSortOrderBuffer( CDataManagerInfo& info );

private:
	bool OpenStockReportSortOrder( int nType, CDataManagerInfo& info, int nSOType );
	bool OpenStockReportSortOrderBuffer( CDataManagerInfo& info, int nSOType );
	bool CloseStockReportSortOrder( CDataManagerInfo& info, int nSOType );
	bool CloseStockReportSortOrderBuffer( CDataManagerInfo& info, int nSOType );
	bool WriteStockReportSortOrder( CDataManagerInfo& info, int nSOType );
	bool WriteStockReportSortOrderBuffer( CDataManagerInfo& info, int nSOType );
	
public:
	CDefTextUnit AdjustTextBuffer;
	CCustomFieldNameArray CustomFieldNames;
	CSalesHistoryCSVArrayStock SalesHistoryStock;
	CSalesHistoryCSVArrayStock SalesHistoryStockBuffer;
	CDefTextUnit StockUnitBuffer;
	CDefTextUnit SubUnitBuffer;
	CDefTextCategory SystemCategory;
	CCategorySetCSVArray SystemCategorySet;
	CDefTextSystemCustomFieldSet SystemCustomFieldSet[ MAX_ITEM_CUSTOM_FIELDS + 1 ];

public:
	CDefTextUnit AdjustText;
	CDefTextUnit StockUnit;
	CDefTextUnit SubUnit;
	
private:
	int m_nActiveStockReportSortOrderType;
	CStockReportSortOrderCSVArray StockReportSortOrder[ MAX_STOCKREPORT_SORTORDERS  + MAX_STOCKTAKE_SORTORDERS ];
	CStockReportSortOrderCSVArray StockReportSortOrderBuffer[ MAX_STOCKREPORT_SORTORDERS  + MAX_STOCKTAKE_SORTORDERS ];
	
private:
	int m_nStatusStockReportSortOrder[ MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ];
	int m_nStatusStockReportSortOrderBuffer[ MAX_STOCKREPORT_SORTORDERS + MAX_STOCKTAKE_SORTORDERS ];	
	int m_nStatusSystemCustomFieldSet[ MAX_ITEM_CUSTOM_FIELDS + 1 ];

private:
	int GetStockReportSortOrderIndexFromType( int nType );

private:
	int m_nStatusAdjustText;
	int m_nStatusCustomFieldNames;
	int m_nStatusSalesHistoryStock;
	int m_nStatusStockUnit;
	int m_nStatusSubUnit;
	int m_nStatusSystemCategory;
	int m_nStatusSystemCategorySet;
	
private:
	int m_nStatusAdjustTextBuffer;
	int m_nStatusSalesHistoryStockBuffer;
	int m_nStatusStockUnitBuffer;
	int m_nStatusSubUnitBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
