#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#define STOCK_SALES 1
#define STOCK_TOP_100_SALES 2
#define STOCK_ESTIMATED_PROFIT 3
#define STOCK_REORDER 4
#define ORDER_ONORDER_SUPPLIER 5
#define STOCK_APPARENT_QTY 6
#define STOCK_APPARENT_VALUE 7
#define STOCK_OPENING_VALUE 8
#define STOCK_CLOSING_VALUE 9
#define STOCK_TRANSFER_VALUE 10
#define STOCK_VARIANCE 11
#define STOCK_RECONCILE 12
#define STOCK_RECONCILE_CATEGORY 13
#define STOCK_RECONCILE_CASH 14
#define STOCK_SUPPLIER 15
#define STOCK_DELIVERY 16
#define STOCK_RETURNS 17
#define STOCK_ADJUST 18
#define STOCK_SHEET 19
#define STOCK_PRICE_LIST 20
#define STOCK_PROFIT 21
#define ORDER_DELIVERY 22
#define STOCK_RETAIL_SUMMARY 23
#define STOCK_COST_SUMMARY 24
#define STOCK_TOP_100_APPARENT 25
#define STOCK_CUSTOM_LIST 26
#define STOCK_TRANSFER_CLOSING 27
#define STOCK_TRANSFER_PERIOD 28
#define ORDER_ONORDER_CATEGORY 29
#define STOCK_APPARENT_SUMMARY 30
#define STOCK_OVERSTOCK_VALUE 31
#define STOCK_APPARENT_QTY_DU 32
#define STOCK_APPARENT_SUMMARY_DU 33
#define STOCK_ESTIMATE_ALLOWANCE 34
/**********************************************************************/

class CStockReportNames  
{
public:
	const char* GetComboName( int nReportType );

private:
	CString m_strComboName;
};

/**********************************************************************/
#endif
/**********************************************************************/

