/**********************************************************************/
 
/**********************************************************************/
#include "StockOptionsIni.h"
/**********************************************************************/
#include "StockReportNames.h"
/**********************************************************************/

const char* CStockReportNames::GetComboName( int nReportType )
{
	m_strComboName = "Unknown";

	switch ( nReportType )
	{
	case STOCK_SALES:				m_strComboName = "Sales Breakdown";					break;
	case STOCK_TOP_100_SALES:		m_strComboName = "Top/Bottom Sellers";				break;
	case STOCK_ESTIMATED_PROFIT:	m_strComboName = "Estimated Profit";				break;
	case STOCK_APPARENT_QTY:		m_strComboName = "Apparent Stock";					break;
	case STOCK_APPARENT_QTY_DU:		m_strComboName = "Apparent Stock by DU";			break;
	case STOCK_APPARENT_SUMMARY:	m_strComboName = "Apparent Stock Summary";			break;
	case STOCK_APPARENT_SUMMARY_DU:	m_strComboName = "Apparent Stock Summary by DU";	break;
	case STOCK_TOP_100_APPARENT:	m_strComboName = "Top/Bottom Apparent";				break;
	case STOCK_RETAIL_SUMMARY:		m_strComboName = "Retail Valuation Summary";		break;
	case STOCK_COST_SUMMARY:		m_strComboName = "Cost Valuation Summary";			break;
	case STOCK_APPARENT_VALUE:		m_strComboName = "Apparent Stock Valuation";		break;
	case STOCK_OPENING_VALUE:		m_strComboName = "Opening Stock Valuation";			break;
	case STOCK_OVERSTOCK_VALUE:		m_strComboName = "Overstock Valuation";				break;
	case STOCK_CLOSING_VALUE:		m_strComboName = "Closing Stock Valuation";			break;
	case STOCK_TRANSFER_VALUE:		m_strComboName = "Transfer Stock Valuation";		break;
	case STOCK_VARIANCE:			m_strComboName = "Closing Stock Variance";			break;
	case STOCK_ESTIMATE_ALLOWANCE:	m_strComboName = "Estimated Allowances";			break;
	case STOCK_RECONCILE:			m_strComboName = "Item Reconciliation";				break;
	case STOCK_RECONCILE_CATEGORY:	m_strComboName = "Category Reconciliation";			break;
	case STOCK_RECONCILE_CASH:		m_strComboName = "Cash Reconciliation";				break;
	case STOCK_SUPPLIER:			m_strComboName = "Stock Items by Supplier";			break;
	case STOCK_PROFIT:				m_strComboName = "Profit by Supplier";				break;
	case STOCK_REORDER:				m_strComboName = "Stock Reorder";					break;
	case STOCK_DELIVERY:			m_strComboName = "Delivery Summary";				break;
	case STOCK_RETURNS:				m_strComboName = "Returns Summary";					break;
	case STOCK_SHEET:				m_strComboName = "Stock Sheet";						break;
	case STOCK_PRICE_LIST:			m_strComboName = "Stock Price List";				break;
	case ORDER_ONORDER_SUPPLIER:	m_strComboName = "Stock On Order by Supplier";		break;
	case ORDER_ONORDER_CATEGORY:	m_strComboName = "Stock On Order by Category";		break;
	case ORDER_DELIVERY:			m_strComboName = "Processed Deliveries";			break;
	case STOCK_ADJUST:				m_strComboName = "Adjustment Summary";				break;
	case STOCK_CUSTOM_LIST:			m_strComboName = "Stock Custom List";				break;
	case STOCK_TRANSFER_CLOSING:	m_strComboName = "Closing Stock Transfer Detail";	break;
	case STOCK_TRANSFER_PERIOD:		m_strComboName = "Transfer Stock Detail";			break;
	}

	return m_strComboName;
}

/**********************************************************************/
