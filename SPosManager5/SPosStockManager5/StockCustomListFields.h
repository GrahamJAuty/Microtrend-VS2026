#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

struct CustomStockField
{
	int Type;
	int Orientation;
	int DefSize;
	const char* ListLabel;
	const char* ReportLabel;
};

/**********************************************************************/

namespace CustomStockList
{
	const CustomStockField StockCode =			{ 0, TA_LEFT,	380, "Stock Code", "" };
	const CustomStockField Description =		{ 1, TA_LEFT,	400, "Description", "" };
	const CustomStockField CatName =			{ 2, TA_LEFT,	380, "Category Name", "Category" };
	const CustomStockField StockUnitName =		{ 3, TA_LEFT,	200, "Stock Unit", "Stock Unit" };
	const CustomStockField StockUnitDetail =	{ 4, TA_LEFT,	350, "Stock / Sub Unit", "Stock / Sub Unit" };
	const CustomStockField RetailUnitName =		{ 5, TA_LEFT,	220, "Retail Unit", "Retail Unit" };
	const CustomStockField Wastage =			{ 6, TA_RIGHT,	200, "Waste Percentage", "Waste%" };
	const CustomStockField LabelsFlag =			{ 7, TA_LEFT,	150, "Product Labels Flag", "Labels" };
	const CustomStockField OrderFlag =			{ 8, TA_LEFT,	150, "Automatic Reorder Flag", "Order" };
	const CustomStockField PluRangeFlag =		{ 9, TA_LEFT,	150, "Plu Range Flag", "Range" };
	const CustomStockField SupplierName1 =		{ 10,TA_LEFT,	300, "Supplier Name 1", "Supplier1" };
	const CustomStockField SupplierName2 =		{ 11,TA_LEFT,	300, "Supplier Name 2", "Supplier2" };
	const CustomStockField SupplierName3 =		{ 12,TA_LEFT,	300, "Supplier Name 3", "Supplier3" };
	const CustomStockField SupplierName4 =		{ 13,TA_LEFT,	300, "Supplier Name 4", "Supplier4" };
	const CustomStockField SupplierName5 =		{ 14,TA_LEFT,	300, "Supplier Name 5", "Supplier5" };
	const CustomStockField SupplierRef1 =		{ 15,TA_LEFT,	300, "Supplier Reference 1", "Reference1" };
	const CustomStockField SupplierRef2 =		{ 16,TA_LEFT,	300, "Supplier Reference 2", "Reference2" };
	const CustomStockField SupplierRef3 =		{ 17,TA_LEFT,	300, "Supplier Reference 3", "Reference3" };
	const CustomStockField SupplierRef4 =		{ 18,TA_LEFT,	300, "Supplier Reference 4", "Reference4" };
	const CustomStockField SupplierRef5 =		{ 19,TA_LEFT,	300, "Supplier Reference 5", "Reference5" };
	const CustomStockField WrapperCode1 =		{ 20,TA_LEFT,	300, "Wrapper Code 1", "Wrapper1" };
	const CustomStockField WrapperCode2 =		{ 21,TA_LEFT,	300, "Wrapper Code 2", "Wrapper2" };
	const CustomStockField WrapperCode3 =		{ 22,TA_LEFT,	300, "Wrapper Code 3", "Wrapper3" };
	const CustomStockField WrapperCode4 =		{ 23,TA_LEFT,	300, "Wrapper Code 4", "Wrapper4" };
	const CustomStockField WrapperCode5 =		{ 24,TA_LEFT,	300, "Wrapper Code 5", "Wrapper5" };
	const CustomStockField DelUnitName1 =		{ 25,TA_LEFT,	200, "Delivery Unit 1", "DelUnit1" };
	const CustomStockField DelUnitName2 =		{ 26,TA_LEFT,	200, "Delivery Unit 2", "DelUnit2" };
	const CustomStockField DelUnitName3 =		{ 27,TA_LEFT,	200, "Delivery Unit 3", "DelUnit3" };
	const CustomStockField DelUnitName4 =		{ 28,TA_LEFT,	200, "Delivery Unit 4", "DelUnit4" };
	const CustomStockField DelUnitName5 =		{ 29,TA_LEFT,	200, "Delivery Unit 5", "DelUnit5" };
	const CustomStockField DelUnitDetail1 =		{ 30,TA_RIGHT,	300, "Delivery Unit Detail 1", "DU Detail1" };
	const CustomStockField DelUnitDetail2 =		{ 31,TA_RIGHT,	300, "Delivery Unit Detail 2", "DU Detail2" };
	const CustomStockField DelUnitDetail3 =		{ 32,TA_RIGHT,	300, "Delivery Unit Detail 3", "DU Detail3" };
	const CustomStockField DelUnitDetail4 =		{ 33,TA_RIGHT,	300, "Delivery Unit Detail 4", "DU Detail4" };
	const CustomStockField DelUnitDetail5 =		{ 34,TA_RIGHT,	300, "Delivery Unit Detail 5", "DU Detail5" };
	const CustomStockField DelUnitYield1 =		{ 35,TA_RIGHT,	200, "Delivery Unit Yield 1", "DU Yield1" };
	const CustomStockField DelUnitYield2 =		{ 36,TA_RIGHT,	200, "Delivery Unit Yield 2", "DU Yield2" };
	const CustomStockField DelUnitYield3 =		{ 37,TA_RIGHT,	200, "Delivery Unit Yield 3", "DU Yield3" };
	const CustomStockField DelUnitYield4 =		{ 38,TA_RIGHT,	200, "Delivery Unit Yield 4", "DU Yield4" };
	const CustomStockField DelUnitYield5 =		{ 39,TA_RIGHT,	200, "Delivery Unit Yield 5", "DU Yield5" };
	const CustomStockField DelUnitCost1 =		{ 40,TA_RIGHT,	200, "Delivery Unit Cost 1", "DU Cost1" };
	const CustomStockField DelUnitCost2 =		{ 41,TA_RIGHT,	200, "Delivery Unit Cost 2", "DU Cost2" };
	const CustomStockField DelUnitCost3 =		{ 42,TA_RIGHT,	200, "Delivery Unit Cost 3", "DU Cost3" };
	const CustomStockField DelUnitCost4 =		{ 43,TA_RIGHT,	200, "Delivery Unit Cost 4", "DU Cost4" };
	const CustomStockField DelUnitCost5 =		{ 44,TA_RIGHT,	200, "Delivery Unit Cost 5", "DU Cost5" };
	const CustomStockField StockUnitCost1 =		{ 45,TA_RIGHT,	200, "Stock Unit Cost 1", "SU Cost1" };
	const CustomStockField StockUnitCost2 =		{ 46,TA_RIGHT,	200, "Stock Unit Cost 2", "SU Cost2" };
	const CustomStockField StockUnitCost3 =		{ 47,TA_RIGHT,	200, "Stock Unit Cost 3", "SU Cost3" };
	const CustomStockField StockUnitCost4 =		{ 48,TA_RIGHT,	200, "Stock Unit Cost 4", "SU Cost4" };
	const CustomStockField StockUnitCost5 =		{ 49,TA_RIGHT,	200, "Stock Unit Cost 5", "SU Cost5" };
	const CustomStockField DelUnitRetail1 =		{ 50,TA_RIGHT,	200, "Delivery Unit Retail 1", "DU Retail1" };
	const CustomStockField DelUnitRetail2 =		{ 51,TA_RIGHT,	200, "Delivery Unit Retail 2", "DU Retail2" };
	const CustomStockField DelUnitRetail3 =		{ 52,TA_RIGHT,	200, "Delivery Unit Retail 3", "DU Retail3" };
	const CustomStockField DelUnitRetail4 =		{ 53,TA_RIGHT,	200, "Delivery Unit Retail 4", "DU Retail4" };
	const CustomStockField DelUnitRetail5 =		{ 54,TA_RIGHT,	200, "Delivery Unit Retail 5", "DU Retail5" };
	const CustomStockField CustomFieldValue1 =	{ 55,TA_LEFT,	300, "Custom Field 1", "Custom1" };
	const CustomStockField CustomFieldValue2 =	{ 56,TA_LEFT,	300, "Custom Field 2", "Custom2" };
	const CustomStockField CustomFieldValue3 =	{ 57,TA_LEFT,	300, "Custom Field 3", "Custom3" };
	const CustomStockField CustomFieldValue4 =	{ 58,TA_LEFT,	300, "Custom Field 4", "Custom4" };
	const CustomStockField CustomFieldValue5 =	{ 59,TA_LEFT,	300, "Custom Field 5", "Custom5" };
	const CustomStockField ImportSheetCode =	{ 60, TA_LEFT,	400, "ImportSheet Code" };
	const CustomStockField StockmateText =		{ 61, TA_LEFT,	300, "Stockmate Text", "Stockmate" };
	const CustomStockField Apparent =			{ 100,TA_RIGHT, 200, "Apparent Stock", "Apparent" };	
};

/**********************************************************************/
#endif
/**********************************************************************/
