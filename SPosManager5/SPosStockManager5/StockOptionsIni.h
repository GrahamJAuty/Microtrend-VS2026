#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#define ORDER_TYPE_MINIMUM 0
#define ORDER_TYPE_SALES 1
#define ORDER_TYPE_FIXED 2
/**********************************************************************/
#define ORDER_DUSEPARATE_NONE 0
#define ORDER_DUSEPARATE_STOCKPOINT 1
#define ORDER_DUSEPARATE_ORDER 2
/**********************************************************************/

class CStockOptions 
{
public:
	CStockOptions();
	void Reset();
	
	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CStockOptions& source );	
	void SetDefaults();

private:
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void LegacyReloadFromString( CString& strBuffer );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	
public:
	//PLU TAB
	bool GetPluCSVManualImportStockFlag()	{ return m_PluManualImport.GetValue(); }
	bool GetPluCSVManualExportStockFlag()	{ return m_PluManualExport.GetValue(); }
	bool GetPluSimplifiedEditingFlag()		{ return m_PluSimpleEdit.GetValue(); }
	bool GetPluDeleteUnlinkedStockFlag()	{ return m_PluDelUnlinked.GetValue(); }
	bool GetPluBrowseStockCodesFlag()		{ return m_PluBrowseStock.GetValue(); }
	bool GetPluRedirectToStockpointFlag()	{ return m_PluRedirect.GetValue(); }

	//STOCK1 TAB
	int GetStockMaxSuppliersPerItem()		{ return m_StockSuppliers.GetValue(); }
	bool GetStockSubUnitsFlag()				{ return m_StockSubUnits.GetValue(); }
	bool GetStockAutoCreateFlag()			{ return m_StockCreate.GetValue(); }
	bool GetStockAddLeadingZerosFlag()		{ return m_StockAddZeros.GetValue(); }
	bool GetStockFinaliseMovementsFlag()	{ return m_StockFinalise.GetValue(); }
	int GetStockSalesEntryType()			{ return m_StockSales.GetValue(); }
	int GetStockAdjustmentsEntryType()		{ return m_StockAdjust.GetValue(); }
	bool GetStockBelowMinimumFlag()			{ return m_StockBelowMinFlag.GetValue(); }
	int GetStockBelowMinimumColour()		{ return m_StockBelowMinColour.GetValue(); }
	bool GetStockAboveMaximumFlag()			{ return m_StockAboveMaxFlag.GetValue(); }
	int GetStockAboveMaximumColour()		{ return m_StockAboveMaxColour.GetValue(); }
	bool GetStockmateLinkFlag()				{ return m_StockmateLink.GetValue(); }

	//STOCK1 TAB (ORDERS)
	int GetOrderGenerationMethod()			{ return m_OrderGenerate.GetValue(); }
	bool GetOrderTemplateFlag()				{ return m_OrderTemplate.GetValue(); }
	int GetOrderSeparationType()			{ return m_OrderSeparate.GetValue(); }
	bool GetOrderRestrictPreviewFlag()		{ return m_OrderRestrict.GetValue(); }
	bool GetOrderAutoPrintOrderFlag()		{ return m_OrderPrint.GetValue(); }
	bool GetOrderAutoPrintPurchaseFlag()	{ return m_OrderPrintPurchase.GetValue(); }
	bool GetOrderAutoEmailPurchaseFlag()	{ return m_OrderEmailPurchase.GetValue(); }
	int GetOrderSuggestionMethod()			{ return m_OrderSuggest.GetValue(); }

	//STOCK2 TAB
	int GetStockAdhocPriceChangeType()		{ return m_StockPriceChange.GetValue(); }
	int GetStockAdhocSupplierChangeType()	{ return m_StockSuppChange.GetValue(); }

	//STOCK2 TAB (IMPORTS)
	bool GetImportAdhocValueFlag()			{ return m_ImportAdhocVal.GetValue(); }
	bool GetImportAdhocCostFlag()			{ return m_ImportAdhocCost.GetValue(); }
	bool GetImportAdjustValueFlag()			{ return m_ImportAdjustVal.GetValue(); }
	bool GetImportReturnValueFlag()			{ return m_ImportReturnVal.GetValue(); }
	bool GetImportSalesExceptionFlag()		{ return m_ImportException.GetValue(); }

	//STOCK3 TAB (AUTO NUM)
	bool GetAutoNumAdhocFlag()				{ return m_AutoNumAdhoc.GetValue(); }
	bool GetAutoNumAdjustFlag()				{ return m_AutoNumAdjust.GetValue(); }
	bool GetAutoNumReturnFlag()				{ return m_AutoNumReturn.GetValue(); }
	bool GetAutoNumXferFlag()				{ return m_AutoNumXfer.GetValue(); }
	const char* GetAutoNumAdhocPrefix()		{ return m_AutoNumAdhocStr.GetValue(); }
	const char* GetAutoNumAdjustPrefix()	{ return m_AutoNumAdjustStr.GetValue(); }
	const char* GetAutoNumReturnPrefix()	{ return m_AutoNumReturnStr.GetValue(); }
	const char* GetAutoNumXferPrefix()		{ return m_AutoNumXferStr.GetValue(); }

	//STOCK2 TAB (IMPORTS)
	int GetLegacyImportTypeClosing()		{ return m_LegacyImportTypeClosing.GetValue(); }
	int GetLegacyImportTypeOpening()		{ return m_LegacyImportTypeOpening.GetValue(); }
	int GetLegacyImportTypeAdhoc()			{ return m_LegacyImportTypeAdhoc.GetValue(); }
	int GetLegacyImportTypeReturn()			{ return m_LegacyImportTypeReturn.GetValue(); }
	int GetLegacyImportTypeXfer()			{ return m_LegacyImportTypeXfer.GetValue(); }
	int GetLegacyImportTypeSales()			{ return m_LegacyImportTypeSales.GetValue(); }
	int GetLegacyImportTypeReorder()		{ return m_LegacyImportTypeReorder.GetValue(); }
	
	bool GetImportClosingPluFlag()			{ return ( m_ImportClosingPlu.GetValue() == 1 ); }
	bool GetImportClosingWrapperFlag()		{ return ( m_ImportClosingWrapper.GetValue() == 1 ); }
	bool GetImportClosingSheetFlag()		{ return ( m_ImportClosingSheet.GetValue() == 1 ); }
	bool GetImportOpeningPluFlag()			{ return ( m_ImportOpeningPlu.GetValue() == 1 ); }
	bool GetImportOpeningWrapperFlag()		{ return ( m_ImportOpeningWrapper.GetValue() == 1 ); }
	bool GetImportOpeningSheetFlag()		{ return ( m_ImportOpeningSheet.GetValue() == 1 ); }
	bool GetImportAdhocPluFlag()			{ return ( m_ImportAdhocPlu.GetValue() == 1 ); }
	bool GetImportAdhocWrapperFlag()		{ return ( m_ImportAdhocWrapper.GetValue() == 1 ); }
	bool GetImportReturnPluFlag()			{ return ( m_ImportReturnPlu.GetValue() == 1 ); }
	bool GetImportReturnWrapperFlag()		{ return ( m_ImportReturnWrapper.GetValue() == 1 ); }
	bool GetImportXferPluFlag()				{ return ( m_ImportXferPlu.GetValue() == 1 ); }
	bool GetImportXferWrapperFlag()			{ return ( m_ImportXferWrapper.GetValue() == 1 ); }
	bool GetImportSalesPluFlag()			{ return ( m_ImportSalesPlu.GetValue() == 1 ); }
	bool GetImportSalesWrapperFlag()		{ return ( m_ImportSalesWrapper.GetValue() == 1 ); }
	bool GetImportReorderPluFlag()			{ return ( m_ImportReorderPlu.GetValue() == 1 ); }
	bool GetImportReorderWrapperFlag()		{ return ( m_ImportReorderWrapper.GetValue() == 1 ); }
	bool GetImportAdjustPluFlag()			{ return ( m_ImportAdjustPlu.GetValue() ); }
	
	const char* GetImportAdhocPluName()		{ return m_ImportAdhocPluName.GetValue(); }
	const char* GetImportReturnPluName()	{ return m_ImportReturnPluName.GetValue(); }
	const char* GetImportAdjustPluName()	{ return m_ImportAdjustPluName.GetValue(); }
	const char* GetImportXferPluName()		{ return m_ImportXferPluName.GetValue(); }
	const char* GetImportOpeningPluName()	{ return m_ImportOpeningPluName.GetValue(); }
	const char* GetImportClosingPluName()	{ return m_ImportClosingPluName.GetValue(); }
	const char* GetImportSalesPluName()		{ return m_ImportSalesPluName.GetValue(); }
	const char* GetImportReorderPluName()	{ return m_ImportReorderPluName.GetValue(); }
	
	const char* GetImportAdhocWrpName()		{ return m_ImportAdhocWrpName.GetValue(); }
	const char* GetImportReturnWrpName()	{ return m_ImportReturnWrpName.GetValue(); }
	const char* GetImportXferWrpName()		{ return m_ImportXferWrpName.GetValue(); }
	const char* GetImportOpeningWrpName()	{ return m_ImportOpeningWrpName.GetValue(); }
	const char* GetImportClosingWrpName()	{ return m_ImportClosingWrpName.GetValue(); }
	const char* GetImportSalesWrpName()		{ return m_ImportSalesWrpName.GetValue(); }
	const char* GetImportReorderWrpName()	{ return m_ImportReorderWrpName.GetValue(); }
	
	//GLOBAL TAB
	bool GetGlobalAllowanceFlag()			{ return m_GlobalAllow.GetValue(); }
	bool GetGlobalCategoryFlag()			{ return m_GlobalCat.GetValue(); }
	bool GetGlobalCustomFieldFlag()			{ return m_GlobalCustom.GetValue(); }
	 
	//REPORTS1 TAB	
	bool GetReportsSiteFlag()				{ return m_ReportSite.GetValue(); }
	bool GetReportsTrimStockZerosFlag()		{ return m_ReportTrimZero.GetValue(); }
	
	//AUTO EXPIRE TAB
	int GetAutoExpireTimeSalesHistory()		{ return m_ExpireSales.GetValue(); }
	int GetAutoExpireTimeStockAudit()		{ return m_ExpireAudit.GetValue(); }
	int GetAutoExpireTimeSalesRetry()		{ return m_ExpireRetry.GetValue(); }

	//LEGACY (AUTONUM)
	int GetLegacyAutoNumAdhoc()				{ return m_LegacyNumAdhoc.GetValue(); }
	int GetLegacyAutoNumAdjust()			{ return m_LegacyNumAdjust.GetValue(); }
	int GetLegacyAutoNumReturn()			{ return m_LegacyNumReturn.GetValue(); }
	int GetLegacyAutoNumXfer()				{ return m_LegacyNumXfer.GetValue(); }

	//PLU TAB
	void SetPluCSVManualImportStockFlag( bool bFlag )	{ m_PluManualImport.SetValue( bFlag ); }
	void SetPluCSVManualExportStockFlag( bool bFlag )	{ m_PluManualExport.SetValue( bFlag ); }
	void SetPluSimplifiedEditingFlag( bool bFlag )		{ m_PluSimpleEdit.SetValue( bFlag ); }
	void SetPluDeleteUnlinkedStockFlag( bool bFlag )	{ m_PluDelUnlinked.SetValue( bFlag ); }
	void SetPluBrowseStockCodesFlag( bool bFlag )		{ m_PluBrowseStock.SetValue( bFlag ); }
	void SetPluRedirectToStockpointFlag( bool bFlag )	{ m_PluRedirect.SetValue( bFlag ); }

	//STOCK1 TAB
	void SetStockMaxSuppliersPerItem( int nNewValue )	{ m_StockSuppliers.SetValue( nNewValue ); }
	void SetStockSubUnitsFlag( bool bFlag )				{ m_StockSubUnits.SetValue( bFlag ); }
	void SetStockAutoCreateFlag( bool bFlag )			{ m_StockCreate.SetValue( bFlag ); }
	void SetStockAddLeadingZerosFlag( bool bFlag )		{ m_StockAddZeros.SetValue( bFlag ); }
	void SetStockFinaliseMovementsFlag( bool bFlag )	{ m_StockFinalise.SetValue( bFlag ); }
	void SetStockSalesEntryType( int nNewValue )		{ m_StockSales.SetValue( nNewValue ); }
	void SetStockAdjustmentsEntryType( int nNewValue )	{ m_StockAdjust.SetValue( nNewValue ); }
	void SetStockBelowMinimumFlag( bool bFlag )			{ m_StockBelowMinFlag.SetValue( bFlag ); }
	void SetStockBelowMinimumColour( int nNewValue )	{ m_StockBelowMinColour.SetValue( nNewValue ); }
	void SetStockAboveMaximumFlag( bool bFlag )			{ m_StockAboveMaxFlag.SetValue( bFlag ); }
	void SetStockAboveMaximumColour( int nNewValue )	{ m_StockAboveMaxColour.SetValue( nNewValue ); }
	void SetStockmateLinkFlag(bool bFlag)				{ m_StockmateLink.SetValue(bFlag); }

	//STOCK1 TAB (ORDERS)
	void SetOrderGenerationMethod( int nNewValue )		{ m_OrderGenerate.SetValue( nNewValue ); }
	void SetOrderTemplateFlag( bool bFlag )				{ m_OrderTemplate.SetValue( bFlag ); }
	void SetOrderSeparationType( int nNewValue )		{ m_OrderSeparate.SetValue( nNewValue ); }
	void SetOrderRestrictPreviewFlag( bool bFlag )		{ m_OrderRestrict.SetValue( bFlag ); }
	void SetOrderAutoPrintOrderFlag( bool bFlag )		{ m_OrderPrint.SetValue( bFlag ); }
	void SetOrderAutoPrintPurchaseFlag( bool bFlag )	{ m_OrderPrintPurchase.SetValue( bFlag ); }
	void SetOrderAutoEmailPurchaseFlag( bool bFlag )	{ m_OrderEmailPurchase.SetValue( bFlag ); }
	void SetOrderSuggestionMethod( int nNewValue )		{ m_OrderSuggest.SetValue( nNewValue ); }

	//STOCK2 TAB
	void SetStockAdhocPriceChangeType( int nNewValue )		{ m_StockPriceChange.SetValue( nNewValue ); }
	void SetStockAdhocSupplierChangeType( int nNewValue )	{ m_StockSuppChange.SetValue( nNewValue ); }	

	//STOCK2 TAB (IMPORTS)
	void SetImportAdhocValueFlag( bool bFlag )			{ m_ImportAdhocVal.SetValue( bFlag ); }
	void SetImportAdhocCostFlag( bool bFlag )			{ m_ImportAdhocCost.SetValue( bFlag ); }
	void SetImportAdjustValueFlag( bool bFlag )			{ m_ImportAdjustVal.SetValue( bFlag ); }
	void SetImportReturnValueFlag( bool bFlag )			{ m_ImportReturnVal.SetValue( bFlag ); }
	void SetImportSalesExceptionFlag( bool bFlag )		{ m_ImportException.SetValue( bFlag ); }

	//STOCK3 TAB (AUTO NUM)
	void SetAutoNumAdhocFlag( bool bFlag )				{ m_AutoNumAdhoc.SetValue( bFlag ); }
	void SetAutoNumAdjustFlag( bool bFlag )				{ m_AutoNumAdjust.SetValue( bFlag ); }
	void SetAutoNumReturnFlag( bool bFlag )				{ m_AutoNumReturn.SetValue( bFlag ); }
	void SetAutoNumXferFlag( bool bFlag )				{ m_AutoNumXfer.SetValue( bFlag ); }
	void SetAutoNumAdhocPrefix( const char* sz )		{ m_AutoNumAdhocStr.SetValue( sz ); }
	void SetAutoNumAdjustPrefix( const char* sz )		{ m_AutoNumAdjustStr.SetValue( sz ); }
	void SetAutoNumReturnPrefix( const char* sz )		{ m_AutoNumReturnStr.SetValue( sz ); }
	void SetAutoNumXferPrefix( const char* sz )			{ m_AutoNumXferStr.SetValue( sz ); }

	//STOCK2 TAB (IMPORTS)
	void SetLegacyImportTypeClosing( int nType )		{ m_LegacyImportTypeClosing.SetValue( nType ); }
	void SetLegacyImportTypeOpening( int nType )		{ m_LegacyImportTypeOpening.SetValue( nType ); }
	void SetLegacyImportTypeAdhoc( int nType )			{ m_LegacyImportTypeAdhoc.SetValue( nType ); }
	void SetLegacyImportTypeReturn( int nType )			{ m_LegacyImportTypeReturn.SetValue( nType ); }
	void SetLegacyImportTypeXfer( int nType )			{ m_LegacyImportTypeXfer.SetValue( nType ); }
	void SetLegacyImportTypeSales( int nType )			{ m_LegacyImportTypeSales.SetValue( nType ); }
	void SetLegacyImportTypeReorder( int nType )		{ m_LegacyImportTypeReorder.SetValue( nType ); }

	void SetImportClosingPluFlag( bool b )				{ m_ImportClosingPlu.SetValue( b ? 1 : 0 ); }
	void SetImportClosingWrapperFlag( bool b )			{ m_ImportClosingWrapper.SetValue( b ? 1 : 0 ); }
	void SetImportClosingSheetFlag( bool b )			{ m_ImportClosingSheet.SetValue( b ? 1 : 0 ); }
	void SetImportOpeningPluFlag( bool b )				{ m_ImportOpeningPlu.SetValue( b ? 1 : 0 ); }
	void SetImportOpeningWrapperFlag( bool b )			{ m_ImportOpeningWrapper.SetValue( b ? 1 : 0 ); }
	void SetImportOpeningSheetFlag( bool b )			{ m_ImportOpeningSheet.SetValue( b ? 1 : 0 ); }
	void SetImportAdhocPluFlag( bool b )				{ m_ImportAdhocPlu.SetValue( b ? 1 : 0 ); }
	void SetImportAdhocWrapperFlag( bool b )			{ m_ImportAdhocWrapper.SetValue( b ? 1 : 0 ); }
	void SetImportReturnPluFlag( bool b )				{ m_ImportReturnPlu.SetValue( b ? 1 : 0 ); }
	void SetImportReturnWrapperFlag( bool b )			{ m_ImportReturnWrapper.SetValue( b ? 1 : 0 ); }
	void SetImportXferPluFlag( bool b )					{ m_ImportXferPlu.SetValue( b ? 1 : 0 ); }
	void SetImportXferWrapperFlag( bool b )				{ m_ImportXferWrapper.SetValue( b ? 1 : 0 ); }
	void SetImportSalesPluFlag( bool b )				{ m_ImportSalesPlu.SetValue( b ? 1 : 0 ); }
	void SetImportSalesWrapperFlag( bool b )			{ m_ImportSalesWrapper.SetValue( b ? 1 : 0 ); }
	void SetImportReorderPluFlag( bool b )				{ m_ImportReorderPlu.SetValue( b ? 1 : 0 ); }
	void SetImportReorderWrapperFlag( bool b )			{ m_ImportReorderWrapper.SetValue( b ? 1 : 0 ); }
	void SetImportAdjustPluFlag( bool b )				{ m_ImportAdjustPlu.SetValue( b ); }
	
	void SetImportAdhocPluName( const char* sz )		{ m_ImportAdhocPluName.SetValue( sz ); }
	void SetImportReturnPluName( const char* sz )		{ m_ImportReturnPluName.SetValue( sz ); }
	void SetImportAdjustPluName( const char* sz )		{ m_ImportAdjustPluName.SetValue( sz ); }
	void SetImportXferPluName( const char* sz )			{ m_ImportXferPluName.SetValue( sz ); }
	void SetImportOpeningPluName( const char* sz )		{ m_ImportOpeningPluName.SetValue( sz ); }
	void SetImportClosingPluName( const char* sz )		{ m_ImportClosingPluName.SetValue( sz ); }
	void SetImportSalesPluName( const char* sz )		{ m_ImportSalesPluName.SetValue( sz ); }
	void SetImportReorderPluName( const char* sz )		{ m_ImportReorderPluName.SetValue( sz ); }
	
	void SetImportAdhocWrpName( const char* sz )		{ m_ImportAdhocWrpName.SetValue( sz ); }
	void SetImportReturnWrpName( const char* sz )		{ m_ImportReturnWrpName.SetValue( sz ); }
	void SetImportXferWrpName( const char* sz )			{ m_ImportXferWrpName.SetValue( sz ); }
	void SetImportOpeningWrpName( const char* sz )		{ m_ImportOpeningWrpName.SetValue( sz ); }
	void SetImportClosingWrpName( const char* sz )		{ m_ImportClosingWrpName.SetValue( sz ); }
	void SetImportSalesWrpName( const char* sz )		{ m_ImportSalesWrpName.SetValue( sz ); }
	void SetImportReorderWrpName( const char* sz )		{ m_ImportReorderWrpName.SetValue( sz ); }
	
	//GLOBAL TAB
	void SetGlobalAllowanceFlag( bool bFlag )			{ m_GlobalAllow.SetValue( bFlag ); }
	void SetGlobalCategoryFlag( bool bFlag )			{ m_GlobalCat.SetValue( bFlag ); }
	void SetGlobalCustomFieldFlag( bool bFlag )			{ m_GlobalCustom.SetValue( bFlag ); }
	
	//REPORTS1 TAB
	void SetReportsSiteFlag( bool bFlag )				{ m_ReportSite.SetValue( bFlag ); }
	void SetReportsTrimStockZerosFlag( bool bFlag )		{ m_ReportTrimZero.SetValue( bFlag ); }
	
	//AUTO EXPIRE TAB
	void SetAutoExpireTimeSalesHistory( int nNewValue )	{ m_ExpireSales.SetValue( nNewValue ); }
	void SetAutoExpireTimeStockAudit( int nNewValue )	{ m_ExpireAudit.SetValue( nNewValue ); }
	void SetAutoExpireTimeSalesRetry( int nNewValue )	{ m_ExpireRetry.SetValue( nNewValue ); }

	//LEGACY (AUTONUM)
	void SetLegacyAutoNumAdhoc( int nNewValue )			{ m_LegacyNumAdhoc.SetValue( nNewValue ); }
	void SetLegacyAutoNumAdjustments( int nNewValue )	{ m_LegacyNumAdjust.SetValue( nNewValue ); }
	void SetLegacyAutoNumReturns( int nNewValue )		{ m_LegacyNumReturn.SetValue( nNewValue ); }
	void SetLegacyAutoNumTransfers( int nNewValue )		{ m_LegacyNumXfer.SetValue( nNewValue ); }

public:
	void CheckImportOptions( CIniFileInt& Legacy, CIniFileInt& Plu, CIniFileInt& Wrapper, CIniFileInt& Sheet );

public:
	int GetAutoNumAdhoc();
	int GetAutoNumAdjust();
	int GetAutoNumReturn();
	int GetAutoNumXfer();

	bool SetAutoNumAdhoc( int n );
	bool SetAutoNumAdjust( int n );
	bool SetAutoNumReturn( int n );
	bool SetAutoNumXfer( int n );
		
private:
	//PLU TAB
	CIniFileBool m_PluManualImport;
	CIniFileBool m_PluManualExport;
	CIniFileBool m_PluSimpleEdit;
	CIniFileBool m_PluDelUnlinked;
	CIniFileBool m_PluBrowseStock;
	CIniFileBool m_PluRedirect;

	//STOCK1 TAB
	CIniFileInt m_StockSuppliers;
	CIniFileBool m_StockSubUnits;
	CIniFileBool m_StockCreate;
	CIniFileBool m_StockAddZeros;
	CIniFileBool m_StockFinalise;
	CIniFileInt m_StockSales;
	CIniFileInt m_StockAdjust;
	CIniFileBool m_StockBelowMinFlag;
	CIniFileInt m_StockBelowMinColour;
	CIniFileBool m_StockAboveMaxFlag;
	CIniFileInt m_StockAboveMaxColour;
	CIniFileBool m_StockmateLink;
	
	//STOCK1 TAB (ORDERS)
	CIniFileInt m_OrderGenerate;
	CIniFileBool m_OrderTemplate;
	CIniFileInt m_OrderSeparate;
	CIniFileBool m_OrderRestrict;
	CIniFileBool m_OrderPrint;
	CIniFileBool m_OrderPrintPurchase;
	CIniFileBool m_OrderEmailPurchase;
	CIniFileInt m_OrderSuggest;

	//STOCK2 TAB
	CIniFileInt m_StockPriceChange;
	CIniFileInt m_StockSuppChange;

	//STOCK2 TAB (IMPORTS)
	CIniFileBool m_ImportAdhocVal;
	CIniFileBool m_ImportAdhocCost;
	CIniFileBool m_ImportAdjustVal;
	CIniFileBool m_ImportReturnVal;
	CIniFileBool m_ImportException;

	//STOCK3 TAB (AUTONUM)
	CIniFileBool m_AutoNumAdhoc;
	CIniFileBool m_AutoNumAdjust;
	CIniFileBool m_AutoNumReturn;
	CIniFileBool m_AutoNumXfer;
	CIniFileString m_AutoNumAdhocStr;
	CIniFileString m_AutoNumAdjustStr;
	CIniFileString m_AutoNumReturnStr;
	CIniFileString m_AutoNumXferStr;

	//STOCK2 TAB (IMPORTS)
	CIniFileInt m_LegacyImportTypeClosing;
	CIniFileInt m_LegacyImportTypeOpening;
	CIniFileInt m_LegacyImportTypeAdhoc;
	CIniFileInt m_LegacyImportTypeReturn;
	CIniFileInt m_LegacyImportTypeXfer;
	CIniFileInt m_LegacyImportTypeSales;
	CIniFileInt m_LegacyImportTypeReorder;

	CIniFileInt m_ImportClosingPlu;
	CIniFileInt m_ImportClosingWrapper;
	CIniFileInt m_ImportClosingSheet;
	CIniFileInt m_ImportOpeningPlu;
	CIniFileInt m_ImportOpeningWrapper;
	CIniFileInt m_ImportOpeningSheet;
	CIniFileInt m_ImportAdhocPlu;
	CIniFileInt m_ImportAdhocWrapper;
	CIniFileInt m_ImportReturnPlu;
	CIniFileInt m_ImportReturnWrapper;
	CIniFileInt m_ImportXferPlu;
	CIniFileInt m_ImportXferWrapper;
	CIniFileInt m_ImportSalesPlu;
	CIniFileInt m_ImportSalesWrapper;
	CIniFileInt m_ImportReorderPlu;
	CIniFileInt m_ImportReorderWrapper;
	CIniFileBool m_ImportAdjustPlu;
	
	CIniFileString m_ImportAdhocPluName;
	CIniFileString m_ImportReturnPluName;
	CIniFileString m_ImportAdjustPluName;
	CIniFileString m_ImportXferPluName;
	CIniFileString m_ImportOpeningPluName;
	CIniFileString m_ImportClosingPluName;
	CIniFileString m_ImportSalesPluName;
	CIniFileString m_ImportReorderPluName;

	CIniFileString m_ImportAdhocWrpName;
	CIniFileString m_ImportReturnWrpName;
	CIniFileString m_ImportXferWrpName;
	CIniFileString m_ImportOpeningWrpName;
	CIniFileString m_ImportClosingWrpName;
	CIniFileString m_ImportSalesWrpName;
	CIniFileString m_ImportReorderWrpName;

	//GLOBAL TAB
	CIniFileBool m_GlobalAllow;
	CIniFileBool m_GlobalCat;
	CIniFileBool m_GlobalCustom;

	//REPORTS1 TAB
	CIniFileBool m_ReportSite;
	CIniFileBool m_ReportTrimZero;
	
	
	//AUTO EXPIRE TAB
	CIniFileInt m_ExpireSales;
	CIniFileInt m_ExpireAudit;
	CIniFileInt m_ExpireRetry;

	//LEGACY (AUTONUM)
	CIniFileInt m_LegacyNumAdhoc;
	CIniFileInt m_LegacyNumAdjust;
	CIniFileInt m_LegacyNumReturn;
	CIniFileInt m_LegacyNumXfer;
	
private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CStockOptions StockOptions;		/* global */
/**********************************************************************/
#endif
/**********************************************************************/
