/**********************************************************************/
#include "LocationCSVArray.h"
#include "MaxLengths.h"
#include "SysInfo.h"
/**********************************************************************/
#include "StockOptionsIni.h"
/**********************************************************************/

CStockOptions::CStockOptions()
{
	//PLU TAB
	m_PluManualImport.Create	( "PluCSVManualImportStock", TRUE );
	m_PluManualExport.Create	( "PluCSVManualExportStock", TRUE );
	m_PluSimpleEdit.Create		( "PluSimplifiedEditing", FALSE );
	m_PluDelUnlinked.Create		( "PluDeleteUnlinkedStock", FALSE );
	m_PluBrowseStock.Create		( "PluBrowseStockCodes", FALSE);
	m_PluRedirect.Create		( "PluRedirectToStockpoint", FALSE );
	
	//STOCK1 TAB
	m_StockSuppliers.Create		( "StockMaxSuppliersPerItem", 1, 5, 1 );
	m_StockSubUnits.Create		( "StockSubUnits", FALSE );
	m_StockCreate.Create		( "StockAutoCreate", TRUE );
	m_StockAddZeros.Create	 	( "StockAddLeadingZeros", TRUE );
	m_StockFinalise.Create	 	( "StockFinaliseMovements", FALSE );
	m_StockSales.Create 		( "StockSalesEntryType", 0, 2, 0 );
	m_StockAdjust.Create 		( "StockAdjustmentsEntryType", 0, 2, 1 );
	m_StockBelowMinFlag.Create 	( "StockBelowMinFlag", FALSE );
	m_StockBelowMinColour.Create( "StockBelowMinColour", 0x0, 0xFFFFFF, 0xFF0000 );
	m_StockAboveMaxFlag.Create 	( "StockAboveMaxFlag", FALSE );
	m_StockAboveMaxColour.Create( "StockAboveMaxColour", 0x0, 0xFFFFFF, 0x00FF00 );
	m_StockmateLink.Create		( "StockmateLink", FALSE );

	//STOCK1 TAB (ORDERS)
	m_OrderGenerate.Create 		( "OrderGenerationMethod", 0, 2, 0 );
	m_OrderTemplate.Create 		( "OrderTemplate", FALSE );
	m_OrderSeparate.Create		( "OrderSeparationType", 0, 2, 0 );
	m_OrderRestrict.Create		( "OrderRestrict", FALSE );
	m_OrderPrint.Create			( "OrderAutoPrintOrder", FALSE );
	m_OrderPrintPurchase.Create	( "OrderAutoPrintPurchase", FALSE );
	m_OrderEmailPurchase.Create	( "OrderAutoEmailPurchase", FALSE );
	m_OrderSuggest.Create 		( "OrderSuggestionMethod", 0, 2, 0 );

	//STOCK2 TAB
	m_StockPriceChange.Create	( "StockAdhocPriceChangeType", 0, 2, 0 );
	m_StockSuppChange.Create	( "StockAdhocSupplierChangeType", 0, 2, 0 );

	//STOCK2 TAB (IMPORTS)
	m_ImportAdhocVal.Create		( "ImportDeliveryValue", FALSE );
	m_ImportAdhocCost.Create	( "ImportUpdateCost", FALSE );
	m_ImportAdjustVal.Create	( "ImportAdjustmentsValue", FALSE );
	m_ImportReturnVal.Create	( "ImportReturnsValue", FALSE );
	m_ImportException.Create	( "ImportUnlinkedException", FALSE );

	//STOCK3 TAB (AUTO NUM)
	m_AutoNumAdhoc.Create		( "AutoNumAdhoc", FALSE );
	m_AutoNumAdjust.Create		( "AutoNumAdjustments", FALSE );
	m_AutoNumReturn.Create		( "AutoNumReturns", FALSE );
	m_AutoNumXfer.Create		( "AutoNumTransfers", FALSE );
	m_AutoNumAdhocStr.Create	( "AutoNumAdhocPrefix", MAX_LENGTH_ADHOC_PREFIX, "" );
	m_AutoNumAdjustStr.Create	( "AutoNumAdjustmentsPrefix", MAX_LENGTH_ADHOC_PREFIX, "" );
	m_AutoNumReturnStr.Create	( "AutoNumReturnsPrefix", MAX_LENGTH_ADHOC_PREFIX, ""  );
	m_AutoNumXferStr.Create		( "AutoNumTransfersPrefix", MAX_LENGTH_ADHOC_PREFIX, "" );

	//STOCK2 TAB (IMPORTS)
	m_LegacyImportTypeClosing.Create( "ImportTypeClosing", 0, 3, 0 );
	m_LegacyImportTypeOpening.Create( "ImportTypeOpening", 0, 3, 0 );
	m_LegacyImportTypeAdhoc.Create	( "ImportTypeAdhoc", 0, 2, 0 );
	m_LegacyImportTypeReturn.Create	( "ImportTypeReturns", 0, 2, 0 );
	m_LegacyImportTypeXfer.Create	( "ImportTypeTransfers", 0, 2, 0 );
	m_LegacyImportTypeSales.Create	( "ImportTypeSales", 0, 2, 0 );
	m_LegacyImportTypeReorder.Create( "ImportTypeReorder", 0, 2, 0 );

	m_ImportClosingPlu.Create( "ImportClosingPlu", -1, 1, -1 );
	m_ImportClosingWrapper.Create( "ImportClosingWrapper", -1, 1, -1 );
	m_ImportClosingSheet.Create( "ImportClosingSheet", -1, 1, -1 );
	m_ImportOpeningPlu.Create( "ImportOpeningPlu", -1, 1, -1 );
	m_ImportOpeningWrapper.Create( "ImportOpeningWrapper", -1, 1, -1 );
	m_ImportOpeningSheet.Create( "ImportOpeningSheet", -1, 1, -1 );
	m_ImportAdhocPlu.Create( "ImportAdhocPlu", -1, 1, -1 );
	m_ImportAdhocWrapper.Create( "ImportAdhocWrapper", -1, 1, -1 );
	m_ImportReturnPlu.Create( "ImportReturnsPlu", -1, 1, -1 );
	m_ImportReturnWrapper.Create( "ImportReturnsWrapper", -1, 1, -1 );
	m_ImportXferPlu.Create( "ImportTransfersPlu", -1, 1, -1 );
	m_ImportXferWrapper.Create( "ImportTransfersWrapper", -1, 1, -1 );
	m_ImportSalesPlu.Create( "ImportSalesPlu", -1, 1, -1 );
	m_ImportSalesWrapper.Create( "ImportSalesWrapper", -1, 1, -1 );
	m_ImportReorderPlu.Create( "ImportReorderPlu", -1, 1, -1 );
	m_ImportReorderWrapper.Create( "ImportReorderWrapper", -1, 1, -1 );
	m_ImportAdjustPlu.Create( "ImportAdjustPlu", TRUE );
	
	m_ImportAdhocPluName.Create		( "ImportAdhocPluName", 25, "delivery" );
	m_ImportReturnPluName.Create	( "ImportReturnsPluName", 25, "returns" );
	m_ImportAdjustPluName.Create	( "ImportAdjustmentsPluName", 25, "adjust" );
	m_ImportXferPluName.Create		( "ImportTransfersPluName", 25, "transfer" );
	m_ImportOpeningPluName.Create	( "ImportOpeningPluName", 25, "opening" );
	m_ImportClosingPluName.Create	( "ImportClosingPluName", 25, "closing" );
	m_ImportSalesPluName.Create		( "ImportSalesPluName", 25, "sales" );
	m_ImportReorderPluName.Create	( "ImportReorderPluName", 25, "reorder" );

	m_ImportAdhocWrpName.Create		( "ImportAdhocWrpName", 25, "delivery" );
	m_ImportReturnWrpName.Create	( "ImportReturnsWrpName", 25, "returns" );
	m_ImportXferWrpName.Create		( "ImportTransfersWrpName", 25, "transfer" );
	m_ImportOpeningWrpName.Create	( "ImportOpeningWrpName", 25, "opening" );
	m_ImportClosingWrpName.Create	( "ImportClosingWrpName", 25, "closing" );
	m_ImportSalesWrpName.Create		( "ImportSalesWrpName", 25, "sales" );
	m_ImportReorderWrpName.Create	( "ImportReorderWrpName", 25, "reorder" );

	//GLOBAL TAB
	m_GlobalAllow.Create		( "GlobalAllowance", FALSE );
	m_GlobalCat.Create			( "GlobalCategory", FALSE );
	m_GlobalCustom.Create		( "GlobalCustomField", FALSE );

	//REPORTS1 TAB
	m_ReportSite.Create			( "ReportsSite", FALSE );
	m_ReportTrimZero.Create		( "ReportsTrimStockZeros", FALSE );

	//AUTO EXPIRE TAB
	m_ExpireSales.Create		( "AutoExpireTimeSalesHistory", 0, 999, 0 );
	m_ExpireAudit.Create		( "AutoExpireTimeStockAudit", 0, 999, 0 );
	m_ExpireRetry.Create		( "AutoExpireTimeSalesRetry", 0, 999, 0 );

	//LEGACY (AUTO NUM)
	m_LegacyNumAdhoc.Create 	( "AutoNumLegacyAdhoc", 0, 999999, 0 );
	m_LegacyNumAdjust.Create	( "AutoNumLegacyAdjustments", 0, 999999, 0 );
	m_LegacyNumReturn.Create	( "AutoNumLegacyReturns", 0, 999999, 0 );
	m_LegacyNumXfer.Create		( "AutoNumLegacyTransfers", 0, 999999, 0 );
	
	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CStockOptions::CopyFrom( CStockOptions& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CStockOptions::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	//PLU TAB
	iniFile.ReadBool( m_PluManualImport );
	iniFile.ReadBool( m_PluManualExport );
	iniFile.ReadBool( m_PluSimpleEdit );
	iniFile.ReadBool( m_PluDelUnlinked );
	iniFile.ReadBool( m_PluBrowseStock );
	iniFile.ReadBool( m_PluRedirect );

	//STOCK1 TAB
	iniFile.ReadInt( m_StockSuppliers );
	iniFile.ReadBool( m_StockSubUnits );
	iniFile.ReadBool( m_StockCreate );
	iniFile.ReadBool( m_StockAddZeros );
	iniFile.ReadBool( m_StockFinalise );
	iniFile.ReadInt( m_StockSales );
	iniFile.ReadInt( m_StockAdjust );
	iniFile.ReadBool( m_StockBelowMinFlag );
	iniFile.ReadInt( m_StockBelowMinColour );
	iniFile.ReadBool( m_StockAboveMaxFlag );
	iniFile.ReadInt( m_StockAboveMaxColour );
	iniFile.ReadBool(m_StockmateLink);

	//STOCK1 TAB (ORDERS)
	iniFile.ReadInt( m_OrderGenerate );
	iniFile.ReadBool( m_OrderTemplate );
	iniFile.ReadInt( m_OrderSeparate );
	iniFile.ReadBool( m_OrderRestrict );
	iniFile.ReadBool( m_OrderPrint );
	iniFile.ReadBool( m_OrderPrintPurchase );
	iniFile.ReadBool( m_OrderEmailPurchase );
	iniFile.ReadInt( m_OrderSuggest );

	//STOCK2 TAB
	iniFile.ReadInt( m_StockPriceChange );
	iniFile.ReadInt( m_StockSuppChange );

	//STOCK2 TAB (IMPORTS)
	iniFile.ReadBool( m_ImportAdhocVal );
	iniFile.ReadBool( m_ImportAdhocCost );
	iniFile.ReadBool( m_ImportAdjustVal );
	iniFile.ReadBool( m_ImportReturnVal );
	iniFile.ReadBool( m_ImportException );

	//STOCK3 TAB (AUTO NUM)
	iniFile.ReadBool( m_AutoNumAdhoc );
	iniFile.ReadBool( m_AutoNumAdjust );
	iniFile.ReadBool( m_AutoNumReturn );
	iniFile.ReadBool( m_AutoNumXfer );
	iniFile.ReadString( m_AutoNumAdhocStr );
	iniFile.ReadString( m_AutoNumAdjustStr );
	iniFile.ReadString( m_AutoNumReturnStr );
	iniFile.ReadString( m_AutoNumXferStr );

	//STOCK2 TAB (IMPORTS)
	iniFile.ReadInt( m_LegacyImportTypeClosing );
	iniFile.ReadInt( m_LegacyImportTypeOpening );
	iniFile.ReadInt( m_LegacyImportTypeAdhoc );
	iniFile.ReadInt( m_LegacyImportTypeReturn );
	iniFile.ReadInt( m_LegacyImportTypeXfer );
	iniFile.ReadInt( m_LegacyImportTypeSales );
	iniFile.ReadInt( m_LegacyImportTypeReorder );
	
	iniFile.ReadInt( m_ImportClosingPlu );
	iniFile.ReadInt( m_ImportClosingWrapper );
	iniFile.ReadInt( m_ImportClosingSheet );
	iniFile.ReadInt( m_ImportOpeningPlu );
	iniFile.ReadInt( m_ImportOpeningWrapper );
	iniFile.ReadInt( m_ImportOpeningSheet );
	iniFile.ReadInt( m_ImportAdhocPlu );
	iniFile.ReadInt( m_ImportAdhocWrapper );
	iniFile.ReadInt( m_ImportReturnPlu );
	iniFile.ReadInt( m_ImportReturnWrapper );
	iniFile.ReadInt( m_ImportXferPlu );
	iniFile.ReadInt( m_ImportXferWrapper );
	iniFile.ReadInt( m_ImportSalesPlu );
	iniFile.ReadInt( m_ImportSalesWrapper );
	iniFile.ReadInt( m_ImportReorderPlu );
	iniFile.ReadInt( m_ImportReorderWrapper );
	iniFile.ReadBool( m_ImportAdjustPlu );

	iniFile.ReadString( m_ImportAdhocPluName );
	iniFile.ReadString( m_ImportReturnPluName );
	iniFile.ReadString( m_ImportAdjustPluName );
	iniFile.ReadString( m_ImportXferPluName );
	iniFile.ReadString( m_ImportOpeningPluName );
	iniFile.ReadString( m_ImportClosingPluName );
	iniFile.ReadString( m_ImportSalesPluName );
	iniFile.ReadString( m_ImportReorderPluName );

	iniFile.ReadString( m_ImportAdhocWrpName );
	iniFile.ReadString( m_ImportReturnWrpName );
	iniFile.ReadString( m_ImportXferWrpName );
	iniFile.ReadString( m_ImportOpeningWrpName );
	iniFile.ReadString( m_ImportClosingWrpName );
	iniFile.ReadString( m_ImportSalesWrpName );
	iniFile.ReadString( m_ImportReorderWrpName );

	//GLOBAL TAB
	iniFile.ReadBool( m_GlobalAllow );
	iniFile.ReadBool( m_GlobalCat );
	iniFile.ReadBool( m_GlobalCustom );
	
	//REPORTS1 TAB
	iniFile.ReadBool( m_ReportSite );
	iniFile.ReadBool( m_ReportTrimZero );
	
	//AUTO EXPIRE TAB
	iniFile.ReadInt( m_ExpireSales );
	iniFile.ReadInt( m_ExpireAudit );
	iniFile.ReadInt( m_ExpireRetry );

	//LEGACY (AUTO NUM)
	iniFile.ReadInt( m_LegacyNumAdhoc );
	iniFile.ReadInt( m_LegacyNumAdjust );
	iniFile.ReadInt( m_LegacyNumReturn );
	iniFile.ReadInt( m_LegacyNumXfer );

	CIniFileInt Dummy;
	CheckImportOptions( m_LegacyImportTypeClosing, m_ImportClosingPlu, m_ImportClosingWrapper, m_ImportClosingSheet );
	CheckImportOptions( m_LegacyImportTypeOpening, m_ImportOpeningPlu, m_ImportOpeningWrapper, m_ImportOpeningSheet );
	CheckImportOptions( m_LegacyImportTypeAdhoc, m_ImportAdhocPlu, m_ImportAdhocWrapper, Dummy );
	CheckImportOptions( m_LegacyImportTypeReturn, m_ImportReturnPlu, m_ImportReturnWrapper, Dummy );
	CheckImportOptions( m_LegacyImportTypeXfer, m_ImportXferPlu, m_ImportXferWrapper, Dummy );
	CheckImportOptions( m_LegacyImportTypeSales, m_ImportSalesPlu, m_ImportSalesWrapper, Dummy );
	CheckImportOptions( m_LegacyImportTypeReorder, m_ImportReorderPlu, m_ImportReorderWrapper, Dummy );
}

/**********************************************************************/

void CStockOptions::CheckImportOptions( CIniFileInt& Legacy, CIniFileInt& Plu, CIniFileInt& Wrapper, CIniFileInt& Sheet )
{
	if ( Plu.GetValue() == -1 )
	{
		Sheet.SetValue(0);
			
		switch( Legacy.GetValue() )
		{
		case 1:
			Plu.SetValue(1);
			Wrapper.SetValue(0);
			break;

		case 2:
			Plu.SetValue(0);
			Wrapper.SetValue(1);
			break;

		case 3:
			Plu.SetValue(0);
			Wrapper.SetValue(0);
			break;

		case 0:
		default:
			Plu.SetValue(1);
			Wrapper.SetValue(1);
			break;
		}
	}
}

/**********************************************************************/

void CStockOptions::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	//PLU TAB 
	iniFile.WriteBool	( m_PluManualImport );
	iniFile.WriteBool	( m_PluManualExport );
	iniFile.WriteBool	( m_PluSimpleEdit );
	iniFile.WriteBool	( m_PluDelUnlinked );
	iniFile.WriteBool	( m_PluBrowseStock );
	iniFile.WriteBool	( m_PluRedirect );
	
	//STOCK1 TAB
	iniFile.WriteInt	( m_StockSuppliers );
	iniFile.WriteBool	( m_StockSubUnits );
	iniFile.WriteBool	( m_StockCreate );
	iniFile.WriteBool	( m_StockAddZeros );
	iniFile.WriteBool	( m_StockFinalise );
	iniFile.WriteInt	( m_StockSales );
	iniFile.WriteInt	( m_StockAdjust );
	iniFile.WriteBool	( m_StockBelowMinFlag );
	iniFile.WriteInt	( m_StockBelowMinColour );
	iniFile.WriteBool	( m_StockAboveMaxFlag );
	iniFile.WriteInt	( m_StockAboveMaxColour );
	iniFile.WriteBool	( m_StockmateLink );

	//STOCK1 TAB (ORDERS)
	iniFile.WriteInt	( m_OrderGenerate );
	iniFile.WriteBool	( m_OrderTemplate );
	iniFile.WriteInt	( m_OrderSeparate );
	iniFile.WriteBool	( m_OrderRestrict );
	iniFile.WriteBool	( m_OrderPrint );
	iniFile.WriteBool	( m_OrderPrintPurchase );
	iniFile.WriteBool	( m_OrderEmailPurchase );
	iniFile.WriteInt	( m_OrderSuggest );

	//STOCK2 TAB
	iniFile.WriteInt	( m_StockPriceChange );
	iniFile.WriteInt	( m_StockSuppChange );

	//STOCK2 TAB (IMPORT)
	iniFile.WriteBool	( m_ImportAdhocVal );
	iniFile.WriteBool	( m_ImportAdhocCost );
	iniFile.WriteBool	( m_ImportAdjustVal );
	iniFile.WriteBool	( m_ImportReturnVal );
	iniFile.WriteBool	( m_ImportException );
	
	//STOCK3 TAB
	iniFile.WriteBool	( m_AutoNumAdhoc );
	iniFile.WriteBool	( m_AutoNumAdjust );
	iniFile.WriteBool	( m_AutoNumReturn );
	iniFile.WriteBool	( m_AutoNumXfer );
	iniFile.WriteString	( m_AutoNumAdhocStr );
	iniFile.WriteString	( m_AutoNumAdjustStr );
	iniFile.WriteString	( m_AutoNumReturnStr );
	iniFile.WriteString	( m_AutoNumXferStr );

	//STOCK2 TAB (IMPORTS)
	iniFile.WriteInt	( m_ImportClosingPlu );
	iniFile.WriteInt	( m_ImportClosingWrapper );
	iniFile.WriteInt	( m_ImportClosingSheet );
	iniFile.WriteInt	( m_ImportOpeningPlu );
	iniFile.WriteInt	( m_ImportOpeningWrapper );
	iniFile.WriteInt	( m_ImportOpeningSheet );
	iniFile.WriteInt	( m_ImportAdhocPlu );
	iniFile.WriteInt	( m_ImportAdhocWrapper );
	iniFile.WriteInt	( m_ImportReturnPlu );
	iniFile.WriteInt	( m_ImportReturnWrapper );
	iniFile.WriteInt	( m_ImportXferPlu );
	iniFile.WriteInt	( m_ImportXferWrapper );
	iniFile.WriteInt	( m_ImportSalesPlu );
	iniFile.WriteInt	( m_ImportSalesWrapper );
	iniFile.WriteInt	( m_ImportReorderPlu );
	iniFile.WriteInt	( m_ImportReorderWrapper );
	iniFile.WriteBool	( m_ImportAdjustPlu );
	
	iniFile.WriteString	( m_ImportAdhocPluName );
	iniFile.WriteString	( m_ImportReturnPluName );
	iniFile.WriteString	( m_ImportAdjustPluName );
	iniFile.WriteString	( m_ImportXferPluName );
	iniFile.WriteString	( m_ImportOpeningPluName );
	iniFile.WriteString	( m_ImportClosingPluName );
	iniFile.WriteString	( m_ImportSalesPluName );
	iniFile.WriteString	( m_ImportReorderPluName );

	iniFile.WriteString	( m_ImportAdhocWrpName );
	iniFile.WriteString	( m_ImportReturnWrpName );
	iniFile.WriteString	( m_ImportXferWrpName );
	iniFile.WriteString	( m_ImportOpeningWrpName );
	iniFile.WriteString	( m_ImportClosingWrpName );
	iniFile.WriteString	( m_ImportSalesWrpName );
	iniFile.WriteString	( m_ImportReorderWrpName );

	//GLOBAL TAB
	iniFile.WriteBool	( m_GlobalAllow );
	iniFile.WriteBool	( m_GlobalCat );
	iniFile.WriteBool	( m_GlobalCustom );
	
	//REPORTS1 TAB
	iniFile.WriteBool	( m_ReportSite );
	iniFile.WriteBool	( m_ReportTrimZero );
	
	//AUTO EXPIRE TAB
	iniFile.WriteInt	( m_ExpireSales );
	iniFile.WriteInt	( m_ExpireAudit );
	iniFile.WriteInt	( m_ExpireRetry );

	//LEGACY (AUTO NUM)
	iniFile.WriteInt	( m_LegacyNumAdhoc );
	iniFile.WriteInt	( m_LegacyNumAdjust );
	iniFile.WriteInt	( m_LegacyNumReturn );
	iniFile.WriteInt	( m_LegacyNumXfer );
}

/**********************************************************************/

void CStockOptions::Reset()
{
	//PLU TAB
	m_PluManualImport.Reset();
	m_PluManualExport.Reset();
	m_PluSimpleEdit.Reset();
	m_PluDelUnlinked.Reset();
	m_PluBrowseStock.Reset();
	m_PluRedirect.Reset();

	//STOCK1 TAB
	m_StockSuppliers.Reset();
	m_StockSubUnits.Reset();
	m_StockCreate.Reset();
	m_StockAddZeros.Reset();
	m_StockFinalise.Reset();
	m_StockSales.Reset();
	m_StockAdjust.Reset();
	m_StockBelowMinFlag.Reset();
	m_StockBelowMinColour.Reset();
	m_StockAboveMaxFlag.Reset();
	m_StockAboveMaxColour.Reset();
	m_StockmateLink.Reset();

	//STOCK1 TAB (ORDERS)
	m_OrderGenerate.Reset();
	m_OrderTemplate.Reset();
	m_OrderSeparate.Reset();
	m_OrderRestrict.Reset();
	m_OrderPrint.Reset();
	m_OrderPrintPurchase.Reset();
	m_OrderEmailPurchase.Reset();
	m_OrderSuggest.Reset();

	//STOCK2 TAB
	m_StockPriceChange.Reset();
	m_StockSuppChange.Reset();

	//STOCK2 TAB (IMPORTS)
	m_ImportAdhocVal.Reset();
	m_ImportAdhocCost.Reset();
	m_ImportAdjustVal.Reset();
	m_ImportReturnVal.Reset();
	m_ImportException.Reset();
	
	//STOCK3 TAB (AUTO NUM)
	m_AutoNumAdhoc.Reset();
	m_AutoNumAdjust.Reset();
	m_AutoNumReturn.Reset();
	m_AutoNumXfer.Reset();
	m_AutoNumAdhocStr.Reset();
	m_AutoNumAdjustStr.Reset();
	m_AutoNumReturnStr.Reset();
	m_AutoNumXferStr.Reset();

	//STOCK2 TAB (IMPORTS)
	m_LegacyImportTypeClosing.Reset();
	m_LegacyImportTypeOpening.Reset();
	m_LegacyImportTypeAdhoc.Reset();
	
	m_ImportClosingPlu.Reset();
	m_ImportClosingWrapper.Reset();
	m_ImportClosingSheet.Reset();
	m_ImportOpeningPlu.Reset();
	m_ImportOpeningWrapper.Reset();
	m_ImportOpeningSheet.Reset();
	m_ImportAdhocPlu.Reset();
	m_ImportAdhocWrapper.Reset();
	m_ImportReturnPlu.Reset();
	m_ImportReturnWrapper.Reset();
	m_ImportXferPlu.Reset();
	m_ImportXferWrapper.Reset();
	m_ImportSalesPlu.Reset();
	m_ImportSalesWrapper.Reset();
	m_ImportReorderPlu.Reset();
	m_ImportReorderWrapper.Reset();
	m_ImportAdjustPlu.Reset();

	m_ImportAdhocPluName.Reset();
	m_ImportReturnPluName.Reset();
	m_ImportAdjustPluName.Reset();
	m_ImportXferPluName.Reset();
	m_ImportOpeningPluName.Reset();
	m_ImportClosingPluName.Reset();
	m_ImportSalesPluName.Reset();
	m_ImportReorderPluName.Reset();

	m_ImportAdhocWrpName.Reset();
	m_ImportReturnWrpName.Reset();
	m_ImportXferWrpName.Reset();
	m_ImportOpeningWrpName.Reset();
	m_ImportClosingWrpName.Reset();
	m_ImportSalesWrpName.Reset();
	m_ImportReorderWrpName.Reset();

	//GLOBAL TAB
	m_GlobalAllow.Reset();
	m_GlobalCat.Reset();
	m_GlobalCustom.Reset();

	//REPORTS1 TAB
	m_ReportSite.Reset();
	m_ReportTrimZero.Reset();
	
	//AUTO EXPIRE TAB
	m_ExpireSales.Reset();
	m_ExpireAudit.Reset();
	m_ExpireRetry.Reset();

	//LEGACY (AUTO NUM)
	m_LegacyNumAdhoc.Reset();
	m_LegacyNumAdjust.Reset();
	m_LegacyNumReturn.Reset();
	m_LegacyNumXfer.Reset();
}

/**********************************************************************/

void CStockOptions::SetDefaults()
{
	Reset();

	if (Sysset.GetMaxDatabases() >= 2)
	{
		m_GlobalAllow.SetValue(TRUE);
		m_GlobalCat.SetValue(TRUE);
		m_GlobalCustom.SetValue(TRUE);
	}

	if (Sysset.IsWetStockSystem() == TRUE)
	{
		m_StockSubUnits.SetValue(TRUE);
	}
}

/**********************************************************************/

bool CStockOptions::Read()
{
	CFilenameUpdater FnUpOld( SysFiles::LegacyStockOptions );
	CString strFilenameOld = FnUpOld.GetFilenameToUse();
	
	CFilenameUpdater FnUpNew( SysFiles::StockOptions );
	CString strFilenameNew = FnUpNew.GetFilenameToUse();
	
	if ( ::FileExists( strFilenameNew ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( strFilenameNew ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}
	else
	{
		CSSFile fileOptions;
		if (fileOptions.Open(strFilenameOld, "rb") == FALSE)
		{
			if (GetLastError() != ERROR_FILE_NOT_FOUND)
			{
				m_bFatalReadError = TRUE;
			}

			return FALSE;
		}

		CString strBuffer;
		if (fileOptions.ReadString(strBuffer) == FALSE)
		{
			return TRUE;
		}

		LegacyReloadFromString( strBuffer );
		return TRUE;
	}
}

/**********************************************************************/

bool CStockOptions::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::StockOptions );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
//LEGACY FUNCTION FOR READING OLD OPTIONS FILE FORMAT
/**********************************************************************/

void CStockOptions::LegacyReloadFromString( CString& strBuffer )
{
	CCSV csv ( strBuffer );

	//PLU TAB
	m_PluManualImport.Reset( csv.GetBool(30) == FALSE );
	m_PluManualExport.Reset( csv.GetBool(31) == FALSE );
	m_PluSimpleEdit.Reset( csv.GetBool(25) );
	m_PluDelUnlinked.Reset( csv.GetBool(34) );
	m_PluBrowseStock.Reset( csv.GetBool(35) ) ;
	m_PluRedirect.Reset( csv.GetBool(49) );

	//STOCK1 TAB
	m_StockSuppliers.Reset( csv.GetInt(0) );
	m_StockSubUnits.Reset( csv.GetBool(1) );
	m_StockCreate.Reset( csv.GetBool(5) );
	m_StockAddZeros.Reset( csv.GetBool(6) );
	m_StockFinalise.Reset( csv.GetBool(45) );
	m_StockSales.Reset( csv.GetInt(48) );
	m_StockAdjust.Reset( csv.GetInt(47) );

	//STOCK1 TAB (ORDERS)
	m_OrderGenerate.Reset( csv.GetInt(9) );
	m_OrderTemplate.Reset( csv.GetBool(46) );
	m_OrderSeparate.Reset( csv.GetInt(23) );
	m_OrderRestrict.Reset( csv.GetBool(24) );
	m_OrderPrint.Reset( csv.GetBool(7) );
	m_OrderPrintPurchase.Reset( csv.GetBool(8) );
	m_OrderEmailPurchase.Reset( csv.GetBool(11) );

	//STOCK2 TAB
	m_StockPriceChange.Reset( csv.GetInt(22) );
	m_StockSuppChange.Reset( csv.GetInt(36) );

	//STOCK2 TAB (IMPORTS)
	m_ImportAdhocVal.Reset( csv.GetBool(19) );
	m_ImportAdhocCost.Reset( csv.GetBool(20) );
	m_ImportAdjustVal.Reset( csv.GetBool(40) );
	m_ImportReturnVal.Reset( csv.GetBool(41) );
	m_ImportException.Reset( csv.GetBool(21) == FALSE );

	//STOCK3 TAB (AUTO NUM)
	m_AutoNumAdhoc.Reset( csv.GetBool(13) );
	m_AutoNumAdjust.Reset( csv.GetBool(37) );
	m_AutoNumReturn.Reset( csv.GetBool(16) );
	m_AutoNumXfer.Reset( csv.GetBool(27) );
	m_AutoNumAdhocStr.Reset( csv.GetString(14) );
	m_AutoNumAdjustStr.Reset( csv.GetString(38) );
	m_AutoNumReturnStr.Reset( csv.GetString(17) );
	m_AutoNumXferStr.Reset( csv.GetString(28) );

	//GLOBAL TAB
	m_GlobalAllow.Reset( csv.GetBool(3) );
	m_GlobalCat.Reset( csv.GetBool(4) );
	m_GlobalCustom.Reset( csv.GetBool(50) );
	
	//REPORTS1 TAB
	m_ReportSite.Reset( csv.GetBool(26) );
	m_ReportTrimZero.Reset( csv.GetBool(10) );
	
	//AUTO EXPIRE TAB
	m_ExpireSales.Reset( csv.GetInt(32) );
	m_ExpireAudit.Reset( csv.GetInt(33) );
	m_ExpireRetry.Reset( csv.GetInt(42) );

	//LEGACY (AUTO NUM)
	m_LegacyNumAdhoc.Reset( csv.GetInt(15) );
	m_LegacyNumAdjust.Reset( csv.GetInt(39) );
	m_LegacyNumReturn.Reset( csv.GetInt(18) );
	m_LegacyNumXfer.Reset( csv.GetInt(29) );
};

/**********************************************************************/

int CStockOptions::GetAutoNumAdhoc()
{
	CSSFile file;
	CFilenameUpdater FnUp( SysFiles::DeliveryRef );
	
	if (file.Open(FnUp.GetFilenameToUse(), "rb") == FALSE)
	{
		return m_LegacyNumAdhoc.GetValue();
	}

	CString str = "";
	file.ReadString( str );
	CCSV csv( str );
	
	return csv.GetInt(0);
}

/**********************************************************************/

int CStockOptions::GetAutoNumAdjust()
{
	CSSFile file;
	CFilenameUpdater FnUp( SysFiles::AdjustmentRef );
	
	if (file.Open(FnUp.GetFilenameToUse(), "rb") == FALSE)
	{
		return m_LegacyNumAdjust.GetValue();
	}

	CString str = "";
	file.ReadString( str );
	CCSV csv( str );
	
	return csv.GetInt(0);
}

/**********************************************************************/

int CStockOptions::GetAutoNumReturn()
{
	CSSFile file;
	CFilenameUpdater FnUp( SysFiles::ReturnRef );
	
	if (file.Open(FnUp.GetFilenameToUse(), "rb") == FALSE)
	{
		return m_LegacyNumReturn.GetValue();
	}

	CString str = "";
	file.ReadString( str );
	CCSV csv( str );
	
	return csv.GetInt(0);
}

/**********************************************************************/

int CStockOptions::GetAutoNumXfer()
{
	CSSFile file;
	CFilenameUpdater FnUp( SysFiles::TransferRef );
	
	if (file.Open(FnUp.GetFilenameToUse(), "rb") == FALSE)
	{
		return m_LegacyNumXfer.GetValue();
	}

	CString str = "";
	file.ReadString( str );
	CCSV csv( str );
	
	return csv.GetInt(0);
}

/**********************************************************************/

bool CStockOptions::SetAutoNumAdhoc( int n )
{
	CSSFile file;
	CFilenameUpdater FnUp( SysFiles::DeliveryRef );
	
	if (file.Open(FnUp.GetFilenameToUse(), "wb") == FALSE)
	{
		return FALSE;
	}

	CCSV csv;
	csv.Add( n );
	file.WriteLine( csv.GetLine() );
	return TRUE;
}

/**********************************************************************/

bool CStockOptions::SetAutoNumAdjust(int n)
{
	CSSFile file;
	CFilenameUpdater FnUp(SysFiles::AdjustmentRef);

	if (file.Open(FnUp.GetFilenameToUse(), "wb") == FALSE)
	{
		return FALSE;
	}

	CCSV csv;
	csv.Add(n);
	file.WriteLine(csv.GetLine());
	return TRUE;
}

/**********************************************************************/

bool CStockOptions::SetAutoNumReturn(int n)
{
	CSSFile file;
	CFilenameUpdater FnUp(SysFiles::ReturnRef);

	if (file.Open(FnUp.GetFilenameToUse(), "wb") == FALSE)
	{
		return FALSE;
	}

	CCSV csv;
	csv.Add(n);
	file.WriteLine(csv.GetLine());
	return TRUE;
}

/**********************************************************************/

bool CStockOptions::SetAutoNumXfer(int n)
{
	CSSFile file;
	CFilenameUpdater FnUp(SysFiles::TransferRef);

	if (file.Open(FnUp.GetFilenameToUse(), "wb") == FALSE)
	{
		return FALSE;
	}

	CCSV csv;
	csv.Add(n);
	file.WriteLine(csv.GetLine());
	return TRUE;
}

/**********************************************************************/
