#pragma once
/**********************************************************************/

struct SimpleLabel
{
	const char* Label;
};

/**********************************************************************/

struct TableName
{
	const char* Label;
	int FileIdx;
};

/**********************************************************************/

struct ColumnName
{
	const char* szLabel;
	const int Type;
};

/**********************************************************************/

namespace TableNames
{
	const TableName Plu =				{ "PLU", 1 };
	const TableName PriceSet =			{ "PRICESET", 2 };
	const TableName Stock =				{ "STOCK", 3 };
	const TableName Opening =			{ "OPENING", 4 };
	const TableName Transfer =			{ "TRANSFER", 5 };
	const TableName Reorder =			{ "REORDER", 6 };
	const TableName Stocktake =			{ "STOCKTAKE", 7 };
	const TableName Adjustment =		{ "NEWADJUSTMENT", 8 };
	const TableName ChangeStock =		{ "CHANGESTOCK", 9 };
	const TableName DeleteStock =		{ "DELETESTOCK", 10 };
	const TableName DeletePlu =			{ "DELETEPLU", 11 };
	const TableName OrderPreview =		{ "ORDERPREVIEW", 12 };
	const TableName OrderDeliver =		{ "ORDERDELIVER", 13 };
	const TableName OrderEdit =			{ "ORDEREDIT", 14 };
	const TableName OrderNew=			{ "ORDERNEW", 15 };
	const TableName AdhocDelivery =		{ "ADHOCDELIVERY", 16 };
	const TableName Returns =			{ "RETURNS", 17 };
	const TableName ManualSales =		{ "MANUALSALES", 18 };
	const TableName OrderCost =			{ "ORDEREDIT", 19 };
	//TABLE 20 UNUSED DUE TO PREVIOUS DUPLICATION OF PMS AND IMPORT STOCK
	const TableName PMSManualSales =	{ "PMSMANUALSALES", 21 };
	const TableName ImportStock =		{ "IMPORTSTOCK", 22 };
	const TableName StockFilter =		{ "STOCKFILTER", 23 };
};

/**********************************************************************/
#define COLUMN_PLUNO 1
#define COLUMN_DEPT 2
#define COLUMN_PRICE1 3
#define COLUMN_PRICE2 4
#define COLUMN_PRICE3 5
#define COLUMN_PRICE4 6
#define COLUMN_PRICE5 7
#define COLUMN_PRICE6 8
#define COLUMN_PRICE7 9
#define COLUMN_PRICE8 10
#define COLUMN_POINTS 11			//SPOS ONLY
#define COLUMN_DESC 12
#define COLUMN_TAX 13
#define COLUMN_PLUTYPE 14
#define COLUMN_PLUSTATUS 15
#define COLUMN_STOCKCODE 16
#define COLUMN_CATEGORY 17
#define COLUMN_APPARENT 18
#define COLUMN_STOCKUNIT 19
#define COLUMN_STOCKLEVEL 20
#define COLUMN_ADJUSTMENT 21
#define COLUMN_SESSION 22
#define COLUMN_OPENING 23
#define COLUMN_CLOSING 24
#define COLUMN_MINIMUM 25
#define COLUMN_REORDER 26
#define COLUMN_STOCKCODE2 27
#define COLUMN_APPARENT2 28
#define COLUMN_SALES 29
#define COLUMN_ONORDER 30
#define COLUMN_DELETE 31
#define COLUMN_SUPPLIER 32
#define COLUMN_DUREQ 33
#define COLUMN_TOTALCOST 34
#define COLUMN_ORDERNO 35
#define COLUMN_DATE 36
#define COLUMN_REFERENCE 37
#define COLUMN_DUQTY 38
#define COLUMN_YIELD 39
#define COLUMN_DELIVERED 40
#define COLUMN_SUQTY 41
#define COLUMN_COST 42
#define COLUMN_DUCOST 43
#define COLUMN_SUCOST 44
#define COLUMN_HEIGHT 45
#define COLUMN_PLUQTY 46
#define COLUMN_PLUVALUE 47
#define COLUMN_REASON 48
#define COLUMN_SUVALUE 49
#define COLUMN_TIME 50
#define COLUMN_TYPE 51
#define COLUMN_CURRENTQTY 52
#define COLUMN_IMPORTQTY 53
#define COLUMN_NEWQTY 54
#define COLUMN_IMPORT 55
#define COLUMN_EPOSTEXT 56			
#define COLUMN_MAXIMUM 57
#define COLUMN_PRINTERS 58
#define COLUMN_STOCKFILTER 59
/**********************************************************************/

namespace ColumnNames
{
	const ColumnName PluNo =			{ "PLUNO",			COLUMN_PLUNO };
	const ColumnName Dept =				{ "DEPARTMENT",		COLUMN_DEPT };
	const ColumnName Price1 =			{ "PRICE1",			COLUMN_PRICE1 };
	const ColumnName Price2 =			{ "PRICE2",			COLUMN_PRICE2 };
	const ColumnName Price3 =			{ "PRICE3",			COLUMN_PRICE3 };
	const ColumnName Price4 =			{ "PRICE4",			COLUMN_PRICE4 };
	const ColumnName Price5 =			{ "PRICE5",			COLUMN_PRICE5 };
	const ColumnName Price6 =			{ "PRICE6",			COLUMN_PRICE6 };
	const ColumnName Price7 =			{ "PRICE7",			COLUMN_PRICE7 };
	const ColumnName Price8 =			{ "PRICE8",			COLUMN_PRICE8 };
	const ColumnName Points =			{ "POINTS",			COLUMN_POINTS };
	const ColumnName Desc =				{ "DESCRIPTION",	COLUMN_DESC };
	const ColumnName EposText =		    { "EPOSTEXT",		COLUMN_EPOSTEXT };
	const ColumnName Tax =				{ "TAX",			COLUMN_TAX };
	const ColumnName PluType =			{ "PLUTYPE",		COLUMN_PLUTYPE };
	const ColumnName PluStatus =		{ "PLUSTATUS",		COLUMN_PLUSTATUS };
	const ColumnName StockCode =		{ "STOCKCODE",		COLUMN_STOCKCODE };
	const ColumnName Category =			{ "CATEGORY",		COLUMN_CATEGORY };
	const ColumnName Apparent =			{ "APPARENT",		COLUMN_APPARENT };
	const ColumnName StockUnit =		{ "STOCKUNIT",		COLUMN_STOCKUNIT };
	const ColumnName StockLocal =		{ "STOCKLOCAL",		COLUMN_STOCKLEVEL };
	const ColumnName Adjustment =		{ "ADJUSTMENT",		COLUMN_ADJUSTMENT };
	const ColumnName Session =			{ "SESSION",		COLUMN_SESSION };
	const ColumnName Closing =			{ "CLOSING",		COLUMN_OPENING };
	const ColumnName Opening =			{ "OPENING",		COLUMN_CLOSING };
	const ColumnName Minimum =			{ "MINIMUM",		COLUMN_MINIMUM };
	const ColumnName Reorder =			{ "REORDER",		COLUMN_REORDER };
	const ColumnName StockCode2 =		{ "STOCKCODE2",		COLUMN_STOCKCODE2 };
	const ColumnName Apparent2 =		{ "APPARENT2",		COLUMN_APPARENT2 };
	const ColumnName Sales =			{ "SALES",			COLUMN_SALES };
	const ColumnName OnOrder =			{ "ONORDER",		COLUMN_ONORDER };
	const ColumnName Delete =			{ "DELETE",			COLUMN_DELETE };
	const ColumnName Supplier =			{ "SUPPLIER",		COLUMN_SUPPLIER };
	const ColumnName DUReq =			{ "DUREQ",			COLUMN_DUREQ };
	const ColumnName TotalCost =		{ "TOTALCOST",		COLUMN_TOTALCOST };
	const ColumnName OrderNo =			{ "ORDERNO",		COLUMN_ORDERNO };
	const ColumnName Date =				{ "DATE",			COLUMN_DATE };
	const ColumnName Reference =		{ "REFERENCE",		COLUMN_REFERENCE };
	const ColumnName DUQty =			{ "DUQTY",			COLUMN_DUQTY };
	const ColumnName Yield =			{ "YIELD",			COLUMN_YIELD };
	const ColumnName Delivered =		{ "DELIVERED",		COLUMN_DELIVERED };
	const ColumnName SUQty =			{ "SUQTY",			COLUMN_SUQTY};
	const ColumnName Cost =				{ "COST",			COLUMN_COST };
	const ColumnName DUCost =			{ "DU COST",		COLUMN_DUCOST };
	const ColumnName SUCost =			{ "SU COST",		COLUMN_SUCOST };
	const ColumnName Height =			{ "HEIGHT",			COLUMN_HEIGHT };
	const ColumnName PluQty =			{ "PLUQTY",			COLUMN_PLUQTY };
	const ColumnName PluValue =			{ "PLUVALUE",		COLUMN_PLUVALUE };
	const ColumnName Reason	=			{ "REASON",			COLUMN_REASON };
	const ColumnName SUValue =			{ "SUVALUE",		COLUMN_SUVALUE };
	const ColumnName Time =				{ "TIME",			COLUMN_TIME };
	const ColumnName Type =				{ "TYPE",			COLUMN_TYPE };
	const ColumnName CurrentQty =		{ "CURRENTQTY",		COLUMN_CURRENTQTY };
	const ColumnName ImportQty =		{ "IMPORTQTY",		COLUMN_IMPORTQTY };
	const ColumnName NewQty =			{ "NEWQTY",			COLUMN_NEWQTY };
	const ColumnName Import =			{ "IMPORT",			COLUMN_IMPORT };
	const ColumnName EPOSText =			{ "EPOSTEXT",		COLUMN_EPOSTEXT };
	const ColumnName Maximum =			{ "MAXIMUM",		COLUMN_MAXIMUM };
	const ColumnName Printers =			{ "PRINTERS",		COLUMN_PRINTERS };
	const ColumnName StockFilter =		{ "STOCKFILTER",	COLUMN_STOCKFILTER };
}

/**********************************************************************/
