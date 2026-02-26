#pragma once
/**********************************************************************/
#define PLUCUSTOMLIST_FULL 0
#define PLUCUSTOMLIST_PRICE 1
#define PLUCUSTOMLIST_PROGRAM 2
/**********************************************************************/

struct CustomPluField
{
	int Type;
	int Orientation;
	int DefSize;
	const char* ListLabel;
	const char* ReportLabel;
};

/**********************************************************************/

namespace CustomPluList
{
	const CustomPluField PluNo = { 0, TA_LEFT,	350, "Plu No", "" };
	const CustomPluField RepText = { 1, TA_LEFT,	400, "Report Description", "" };
	const CustomPluField EposText = { 2, TA_LEFT,	400, "Epos Description", "" };
	const CustomPluField DeptNo = { 3, TA_RIGHT,	100, "Dept No", "Dp" };
	const CustomPluField TaxBand = { 4, TA_RIGHT,	100, "Tax Band", "" };
#ifdef STOCKMAN_SYSTEM
	const CustomPluField StockCode = { 5, TA_LEFT,	380, "Stock Code", "" };
#endif
	const CustomPluField PurchaseCost = { 6, TA_RIGHT,	280, "Purchase Cost Price", "Purchase Cost" };
	const CustomPluField UnitPrice = { 7, TA_LEFT,	400, "Unit Price", "" };
	const CustomPluField GP = { 8, TA_RIGHT,	180, "GP", "" };
	const CustomPluField GPPercent = { 9,TA_RIGHT,	180, "GP%", "" };
	const CustomPluField Price1 = { 10,TA_RIGHT,	180, "Price1", "" };
	const CustomPluField Price2 = { 11,TA_RIGHT,	180, "Price2", "" };
	const CustomPluField Price3 = { 12,TA_RIGHT,	180, "Price3", "" };
	const CustomPluField Price4 = { 13,TA_RIGHT,	180, "Price4", "" };
#ifndef STOCKMAN_SYSTEM
	const CustomPluField ApparentLevel = { 14,TA_RIGHT,	220, "Apparent Level", "Apparent" };
	const CustomPluField MinimumLevel = { 15,TA_RIGHT,	220, "Minimum Level", "Minimum" };
#endif
	const CustomPluField PluType = { 16,TA_LEFT,	220, "Plu Type", "" };
	const CustomPluField Category = { 17,TA_LEFT,	240, "Analysis Category", "Category" };
	const CustomPluField PriceType = { 18,TA_LEFT,	220, "Price Type", "" };
	const CustomPluField Menu = { 20,TA_RIGHT,	150, "Menu", "" };
	const CustomPluField Extra = { 21,TA_RIGHT,	150, "Extra", "" };
	const CustomPluField Qualifier = { 22,TA_RIGHT,	200, "Qualifier", "" };
	const CustomPluField OrderPrinters = { 25,TA_LEFT,	300, "Order Printers", "" };
	const CustomPluField Priority = { 26,TA_RIGHT,	120, "Priority", "Prty" };
	const CustomPluField FreeText = { 27,TA_LEFT,	150, "Free Text", "Text" };
	const CustomPluField EmptyColumn = { 28,TA_LEFT,	200, "Empty Column", " " };
#ifndef STOCKMAN_SYSTEM
	const CustomPluField SupplierRef = { 29,TA_LEFT,	380, "Supplier Reference", "Supp Ref" };
	const CustomPluField SupplierNo = { 30,TA_RIGHT,	200, "Supplier No", "Supp No" };
	const CustomPluField NonStockItem = { 31,TA_LEFT,	200, "Non Stock Flag", "Non Stock" };
#endif
	const CustomPluField ModifiedDatePlu = { 32,TA_LEFT,	350, "Last Modified (Plu)", "" };
	const CustomPluField KPImagePrint = { 33,TA_LEFT,	300, "KP Image Print", "" };
	const CustomPluField KPImageModifier = { 34,TA_LEFT,	350, "KP Image Modifier", "" };
	const CustomPluField Stockpoint = { 35,TA_LEFT,	300, "Stockpoint", "" };
	const CustomPluField PrintShelf = { 36,TA_LEFT,	300, "Print Shelf Label", "Prt.Shelf" };
	const CustomPluField PrintProduct = { 37,TA_LEFT,	300, "Print Product Label", "Prt.Product" };
	const CustomPluField OrderPrinter1 = { 38,TA_LEFT,	300, "Order Printer 1", "" };
	const CustomPluField OrderPrinter2 = { 39,TA_LEFT,	300, "Order Printer 2", "" };
	const CustomPluField OrderPrinter3 = { 40,TA_LEFT,	300, "Order Printer 3", "" };
	const CustomPluField OrderPrinter4 = { 41,TA_LEFT,	300, "Order Printer 4", "" };
	const CustomPluField OrderPrinter5 = { 42,TA_LEFT,	300, "Order Printer 5", "" };
	const CustomPluField OrderPrinter6 = { 43,TA_LEFT,	300, "Order Printer 6", "" };
	const CustomPluField OrderPrinter7 = { 44,TA_LEFT,	300, "Order Printer 7", "" };
	const CustomPluField OrderPrinter8 = { 45,TA_LEFT,	300, "Order Printer 8", "" };
	const CustomPluField OrderPrinter9 = { 46,TA_LEFT,	300, "Order Printer 9", "" };
	const CustomPluField Points = { 47,TA_RIGHT,	180, "Points", "" };
	const CustomPluField SalesTracking = { 48,TA_LEFT,	150, "Sales Tracking", "Track" };
	const CustomPluField EANOnReceipt = { 49,TA_LEFT,	250, "EAN on Receipt", "EAN on Rcpt" };
	const CustomPluField FreeTextDescription = { 50,TA_LEFT,	220, "Free Text with Description", "Text Desc" };
	const CustomPluField AgeVerificationType = { 51,TA_LEFT,	220, "Age Verification Type", "Age Check" };
	const CustomPluField Price5 = { 52,TA_RIGHT,	180, "Price5", "" };
	const CustomPluField Price6 = { 53,TA_RIGHT,	180, "Price6", "" };
	const CustomPluField Price7 = { 54,TA_RIGHT,	180, "Price7", "" };
	const CustomPluField Price8 = { 55,TA_RIGHT,	180, "Price8", "" };
	const CustomPluField Price9 = { 56,TA_RIGHT,	180, "Price9", "" };
	const CustomPluField Price10 = { 57,TA_RIGHT,	180, "Price10", "" };
	const CustomPluField KeyText = { 58,TA_LEFT,	400, "Key Text", "" };
	const CustomPluField ExternalProcess = { 59,TA_LEFT,	250, "External Process", "Ext. Proc" };
	const CustomPluField AutoCoversQty = { 60,TA_LEFT,	250, "Auto Covers Qty", "Auto Qty" };
	const CustomPluField KPConsolidateTopItem = { 61,TA_LEFT,	250, "KP Consolidate Top Item", "KP Top Item" };
	const CustomPluField PluInfoModifier = { 62,TA_LEFT,	350, "Plu Info Modifier", "" };
	const CustomPluField ModifiedDateInfo = { 63,TA_LEFT,	350, "Last Modified (Info)", "" };
	const CustomPluField MinimumSPOSVersion = { 64,TA_LEFT,	800, "Minimum Epos Version", "Minimum Version" };
	const CustomPluField MinimumEposReason = { 65,TA_LEFT,	800, "Minimum Epos Reason", "Reason" };
	const CustomPluField DownloadDatePlu = { 66,TA_LEFT,	350, "Download Date", "" };
	const CustomPluField Allergen1 = { 67,TA_LEFT,	240, "Allergen1", "" };
	const CustomPluField Allergen2 = { 68,TA_LEFT,	240, "Allergen2", "" };
	const CustomPluField Allergen3 = { 69,TA_LEFT,	240, "Allergen3", "" };
	const CustomPluField Allergen4 = { 70,TA_LEFT,	240, "Allergen4", "" };
	const CustomPluField Allergen5 = { 71,TA_LEFT,	240, "Allergen5", "" };
	const CustomPluField Allergen6 = { 72,TA_LEFT,	240, "Allergen6", "" };
	const CustomPluField SortCode = { 73,TA_LEFT,	400, "Sort Code", "" };
	const CustomPluField ReportCost = { 74,TA_RIGHT,	280, "Report Cost Price", "Report Cost" };
	const CustomPluField OrderPrinterGroup = { 75,TA_LEFT,	240, "Order Printer Group", "Print Grp" };
	const CustomPluField AgressoGLCode = { 76,TA_LEFT,	200, "Agresso GL Code", "GL Code" };
	const CustomPluField AgressoCostCentre = { 77,TA_LEFT,	300, "Agresso Cost Centre", "Cost Centre" };
	const CustomPluField AgressoProject = { 78,TA_LEFT,	300, "Agresso Project", "Project" };
	const CustomPluField SoldOut = { 79,TA_LEFT,	180, "Sold Out", "" };
	const CustomPluField ImageFilename = { 80,TA_LEFT,	400, "Image Filename", "" };

	//FOR MODIFIER PROGRAMMING BY RANGE ONLY
	const CustomPluField ModifierEnable = { 81,TA_LEFT,	400, "Enable Modifier", "" };
	const CustomPluField ModifierEposText = { 82,TA_LEFT,	400, "Set Own Epos Text", "" };
	const CustomPluField ModifierKeyText = { 83,TA_LEFT,	400, "Set Own Key Text", "" };

	const CustomPluField ActiveItem = { 84,TA_LEFT,	180, "Active Item", "Active" };
	const CustomPluField KPNotAlone = { 85,TA_LEFT,	250, "KP Not Alone", "" };
	const CustomPluField Discountable = { 86,TA_LEFT,	300, "Discountable", "" };
	const CustomPluField StaffDiscount = { 87,TA_LEFT,	350, "Staff Discountable", "" };

	const CustomPluField OrderPrinter10 = { 88,TA_LEFT,	300, "Order Printer 10", "" };
	const CustomPluField OrderPrinter11 = { 89,TA_LEFT,	300, "Order Printer 11", "" };
	const CustomPluField OrderPrinter12 = { 90,TA_LEFT,	300, "Order Printer 12", "" };
	const CustomPluField OrderPrinter13 = { 91,TA_LEFT,	300, "Order Printer 13", "" };
	const CustomPluField OrderPrinter14 = { 92,TA_LEFT,	300, "Order Printer 14", "" };
	const CustomPluField OrderPrinter15 = { 93,TA_LEFT,	300, "Order Printer 15", "" };
	const CustomPluField OrderPrinter16 = { 94,TA_LEFT,	300, "Order Printer 16", "" };

	const CustomPluField StampWeight = { 95,TA_RIGHT,300, "Stamp Weight", "" };

	const CustomPluField AllowModifier = { 96,TA_LEFT,350, "Allow Modifier", "" };

	const CustomPluField AccountRequired = { 97,TA_LEFT,350, "Account Required", "" };
};

/**********************************************************************/
