#pragma once
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "EnhancedIniFile.h"
/**********************************************************************/
#define MAX_KEYBOARDS_PER_LOCATION 14
#define MAX_KEYBOARD_SETS_PER_DATABASE 10
/**********************************************************************/
#define SERVER_NONE 0
#define SERVER_SYS 1
#define SERVER_DB 2
#define SERVER_LOC 3
/**********************************************************************/
#define REPORTCASH_METHOD1_ONLY 0
#define REPORTCASH_ALL 1
#define REPORTCASH_METHOD1_CONSOLIDATE 2
#define REPORTCASH_METHOD1_CHANGE 3
/**********************************************************************/
#define MAX_STARTUPMSG_LINES 10
#define MAX_STARTUPMSG_LINELENGTH 200
/**********************************************************************/

class CEcrmanOptions 
{
public:
	CEcrmanOptions();
	void Reset();
	
	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CEcrmanOptions& source );	
	void SetDefaults();

private:
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void LegacyReloadFromString( CString& strBuffer );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	void MakeIntSettings();
	
public:
	//PLU TAB
	bool GetPluTabBarcodeFlag()				{ return m_PluTabBarcode.GetValue(); }
	bool GetPluTabLinkFlag()				{ return m_PluTabLink.GetValue(); }
	bool GetPluTabInfoFlag()				{ return m_PluTabInfo.GetValue(); }
	bool GetPluPriceSetsFlag()				{ return m_PluPriceSet.GetValue(); }
	bool GetPluDownloadByDateFlag()			{ return m_PluDownloadDate.GetValue(); }
	bool GetPluInactiveNoDownloadFlag()		{ return m_PluInactiveNoDownload.GetValue(); }
	bool GetPluInactiveAutoHideFlag()		{ return m_PluInactiveAutoHide.GetValue(); }
	bool GetPluTaskAfterChangesFlag()		{ return m_PluTask.GetValue(); }
	int GetPluDefaultTextType()				{ return m_PluDefText.GetValue(); }
	bool GetPluDiscardUnusedModifierFlag()	{ return m_PluDiscardMod.GetValue(); }
	bool GetPluCSVAutoImportFlag()			{ return m_PluAutoImport.GetValue(); }
	bool GetPluCSVManualImportFlag()		{ return m_PluManImport.GetValue(); }
	bool GetPluCSVManualExportFlag()		{ return m_PluManExport.GetValue(); }
	bool GetPluExportPurchaseHistoryFlag()	{ return m_PluPurchase.GetValue(); }
	int GetPluProdCSVImportType()			{ return m_PluProdCSV.GetValue(); }
	int GetPluAutoDeleteBarcodeType()		{ return m_PluDelBarcode.GetValue(); }
	bool GetPluBarcodeModifierFlag()		{ return m_PluBarcodeMod.GetValue(); }
	int GetPluAutoModEposTextType()			{ return m_PluAutoModEposText.GetValue(); }
	int GetPluAutoModKeyTextType()			{ return m_PluAutoModKeyText.GetValue(); }

	//GLOBAL TAB
	int GetGlobalDepartmentType()			{ return m_GlobalDept.GetValue(); }
	bool GetGlobalDepartmentFlag();			
	bool GetGlobalDeptGroupReportFlag();
	bool GetGlobalDeptGroupConsolFlag();
	bool GetGlobalDeptGroupEposFlag()		{ return m_GlobalDeptGroupEpos.GetValue(); }
	bool GetGlobalTaxFlag()					{ return m_GlobalTax.GetValue(); }
	bool GetGlobalModifierFlag()			{ return m_GlobalMod.GetValue(); }
	bool GetGlobalAnalysisCategoryFlag()	{ return ( m_GlobalACat.GetValue() == 1 ); }
	bool GetGlobalPaidInOutFlag()			{ return m_GlobalPaidIO.GetValue(); }
	int GetGlobalPaymentType()				{ return m_GlobalPay.GetValue(); }
	bool GetGlobalPaymentFlag();
	bool GetGlobalPaymentGroupFlag();
	
	//FEATURES TAB
	bool GetFeaturesPluStockFlag()			{ return m_FeaturesPluStock.GetValue(); }
	bool GetFeaturesPluAuditFlag()			{ return m_FeaturesPluAudit.GetValue(); }
	int GetFeaturesLegacyEposDeliveryType()	{ return m_FeaturesLegacyEposDelivery.GetValue(); }
	int GetFeaturesEposDeliveryType()		{ return m_FeaturesEposDelivery.GetValue(); }
	bool GetFeaturesMixMatchFlag()			{ return m_FeaturesMixMatch.GetValue(); }
	bool GetFeaturesPromotionFlag()			{ return m_FeaturesPromotion.GetValue(); }
	bool GetFeaturesTimePlanFlag()			{ return m_FeaturesTimePlan.GetValue(); }
	bool GetFeaturesTimePlanDownloadFlag()	{ return m_FeaturesTimePlanDL.GetValue(); }
	bool GetFeaturesTimePlanTemplateFlag()	{ return m_FeaturesTimePlanTemplate.GetValue(); }
	bool GetFeaturesWalletStampRuleFlag();
	bool GetFeaturesGlobalWalletStampRuleFlag();
	bool GetFeaturesKeyboardFlag()			{ return m_FeaturesKeyboard.GetValue(); }
	int GetFeaturesKeyboardsPerLocation()	{ return m_FeaturesLocKeyboards.GetValue(); }
	bool GetFeaturesKeyboardPriceSetFlag()	{ return m_FeaturesKBPriceSet.GetValue(); }
	bool GetFeaturesKeyboardShareFlag()		{ return m_FeaturesKBShare.GetValue(); }
	int GetFeaturesKeyboardTextOrder()		{ return m_FeaturesKBTextOrder.GetValue(); }
	int GetFeaturesServerSettingsType()		{ return m_FeaturesServerType.GetValue(); }
	bool GetFeaturesServerRestrictionFlag()	{ return m_FeaturesServerRestrict.GetValue(); }
	bool GetFeaturesServerPresetFlag()		{ return m_FeaturesServerPreset.GetValue(); }
	bool GetFeaturesServerLogonFlag()		{ return m_FeaturesServerLogon.GetValue(); }	
	bool GetFeaturesCustomerBalanceFlag()	{ return m_FeaturesCustomerBal.GetValue(); }
	int GetFeaturesCustomerNameImportType()	{ return m_FeaturesCustomerName.GetValue(); }
	int GetFeaturesCustomerAccountDelDay()	{ return m_FeaturesCustomerAccountDelDay.GetValue(); }
	int GetFeaturesCustomerOneOffDelDay()	{ return m_FeaturesCustomerOneOffDelDay.GetValue(); }
	int GetFeaturesDatabaseEditingType()	{ return m_FeaturesDBaseEdit.GetValue(); }
	int GetFeaturesBackgroundColour()		{ return m_FeaturesBackCol.GetValue(); }
	bool GetFeaturesContextForceNowFlag()	{ return m_FeaturesContextNow.GetValue(); }
	bool GetFeaturesOPNameFirstFlag()		{ return m_FeaturesOPNameFirst.GetValue(); }
	bool GetFeaturesAllergenFlag()			{ return ( m_FeaturesAllergen.GetValue() ) && ( DealerFlags.Get_LIPA_LJMU_ExportType() == 0 ); }

	//REPORTS1 TAB
	const char* GetReportsFinancialYearStart()	{ return m_ReportsFYearStart.GetValue(); }
	int GetReportsTaxQuarterStart()				{ return m_ReportsTaxQStart.GetValue(); }
	int GetReportsWeekStartDay()				{ return m_ReportsWeekStart.GetValue(); }
	int GetReportsWeekNumberingType()			{ return m_ReportsWeekNumber.GetValue(); }
	int GetReportsDayStartHour()				{ return m_ReportsDayHour.GetValue(); }
	int GetReportsDayStartMinute()				{ return m_ReportsDayMinute.GetValue(); }
	int GetReportsTimeAttendSignOutHour()		{ return m_ReportsTimeAttendHour.GetValue(); }
	int GetReportsTimeAttendSignOutMinute()		{ return m_ReportsTimeAttendMinute.GetValue(); }
	int GetReportsLocationSortType()			{ return m_ReportsLocSort.GetValue(); }
	int GetReportsSalesTaxType()				{ return m_ReportsSalesTaxType.GetValue(); }
	bool GetReportsReportpointFlag()			{ return m_ReportsRepPoint.GetValue(); }
	bool GetReportsLocationSetFlag()			{ return m_ReportsLocSet.GetValue(); }
	bool GetReportsTrimPluZerosFlag()			{ return m_ReportsTrimZero.GetValue(); } 
	bool GetReportsRegionDateRangeFlag()		{ return m_ReportsRegion.GetValue(); }
	bool GetReportsCustomerTransactionFlag()	{ return m_ReportsCustTran.GetValue(); }
	bool GetReportsCustomerStatementFlag()		{ return m_ReportsCustState.GetValue(); }
	bool GetReportsCustomerContactFlag()		{ return m_ReportsCustContact.GetValue(); }
	int GetReportsContactBeforeTime()			{ return m_ReportsContactBeforeTime.GetValue(); }
	int GetReportsContactAfterTime()			{ return m_ReportsContactAfterTime.GetValue(); }
	int GetReportsContactVisitTime()			{ return m_ReportsContactVisitTime.GetValue(); }
	const char* GetReportsDefaultDateRange()	{ return m_ReportsDateRange.GetValue(); }
	int GetReportsSingleDayRanges()				{ return m_ReportsSingleDayRanges.GetValue(); }
	int GetReportsPreviousDayRanges()			{ return m_ReportsPreviousDayRanges.GetValue(); }
	bool GetReportsEODSessionFlag()				{ return m_ReportsEODSession.GetValue(); }
	int GetReportsEcrLiteLogMode()				{ return m_ReportsEcrLiteLogMode.GetValue(); }
	
	//REPORTS2 TAB
	int GetReportsVIPType()						{ return m_ReportsVIPType.GetValue(); }
	int GetReportsVIPLevel()					{ return m_ReportsVIPLevel.GetValue(); }
	bool GetReportsVIPRetailFlag()				{ return m_ReportsVIPRetail.GetValue(); }
	int GetReportsCustomerSortType()			{ return m_ReportsCustSortType.GetValue(); }
	int GetReportsSalesTimeType()				{ return m_ReportsSalesTimeType.GetValue(); }
	bool GetReportsDiscountDetailSalesFlag()	{ return m_ReportsDiscountDetailSales.GetValue(); }
	bool GetReportsPremiumDetailSalesFlag()		{ return m_ReportsPremiumDetailSales.GetValue(); }
	bool GetReportsNetOfTaxSalesFlag()			{ return m_ReportsNetOfTaxSales.GetValue(); }
	bool GetReportsDiscountDetailCoversFlag();
	bool GetReportsPremiumDetailCoversFlag()	{ return m_ReportsPremiumDetailCovers.GetValue(); }
	bool GetReportsNetOfTaxCoversFlag()			{ return m_ReportsNetOfTaxCovers.GetValue(); }
	bool GetReportsCashRSPItemTextFlag()		{ return m_ReportsCashRSPItemText.GetValue(); }
	bool GetReportsAuthServerFlag()				{ return m_ReportsAuthServer.GetValue(); }
	bool GetReportsGroupReportpointFlag()		{ return m_ReportsGroupReportpoint.GetValue(); }
	bool GetReportsEODReasonsFlag()				{ return m_ReportsEODReasons.GetValue(); }
	bool GetReportsGeneralPageFlag()			{ return m_ReportsGeneralPage.GetValue(); }
	bool GetReportsShowCashChangeFlag()			{ return m_ReportsChange.GetValue(); }
	int GetReportsMultiCashMethod()				{ return m_ReportsMultiCash.GetValue(); }
	bool GetReportsShowPaymentQuantityFlag()	{ return m_ReportsPayQty.GetValue(); }
	bool GetReportsShowRAPaymentsFlag()			{ return m_ReportsRAPay.GetValue(); }
	bool GetReportsSeparateRAPaymentsFlag()		{ return m_ReportsSplitRAPay.GetValue(); }
	bool GetReportsShowRABalanceFlag()			{ return m_ReportsRABal.GetValue(); }
	bool GetReportsSeparateRABalanceFlag()		{ return m_ReportsSplitRABal.GetValue(); }
	bool GetReportsCashRSPDiscountPennyFlag()	{ return m_ReportsCashRSPDiscountPenny.GetValue(); }

	bool AllowPremiumDetailSales()				{ return GetReportsDiscountDetailSalesFlag() && GetReportsPremiumDetailSalesFlag(); }
	bool AllowPremiumDetailCovers()				{ return GetReportsDiscountDetailCoversFlag() && GetReportsPremiumDetailCoversFlag(); }

	//AUTO EXPIRE TAB
	int GetAutoExpireTimePluReports()			{ return m_ExpireSales.GetValue(); }
	int GetAutoExpireTimeSummaryReports()		{ return m_ExpireSummary.GetValue(); }
	int GetAutoExpireTimeDetailReports()		{ return m_ExpireDetail.GetValue(); }
	//int GetAutoExpireTimeAttendReports()		{ return m_ExpireTimeAttend.GetValue(); }
	int GetAutoExpireTimePluDeliveryAudit()		{ return m_ExpireAudit.GetValue(); }
	
	//LABELS1 TAB
	bool GetLabelsShelfShowPluFlag()			{ return m_LabelsShelfPlu.GetValue(); }
	bool GetLabelsShelfShowStockCodeFlag()		{ return m_LabelsShelfStock.GetValue(); }
	bool GetLabelsShelfShowSuppRefFlag()		{ return m_LabelsShelfSupp.GetValue(); }
	bool GetLabelsShelfTrimPluZerosFlag()		{ return m_LabelsShelfTrimZero.GetValue(); }
	int GetLabelsShelfPriceLevel1()				{ return m_LabelsShelfPrice1.GetValue(); }
	int GetLabelsShelfPriceLevel2()				{ return m_LabelsShelfPrice2.GetValue(); }
	int GetLabelsShelfTaxType1()				{ return m_LabelsShelfTax1.GetValue(); }
	int GetLabelsShelfTaxType2()				{ return m_LabelsShelfTax2.GetValue(); }
	const char* GetLabelsShelfPriceSymbol1()	{ return m_LabelsShelfSymbol1.GetValue(); }
	const char* GetLabelsShelfPriceSymbol2()	{ return m_LabelsShelfSymbol2.GetValue(); }
	bool GetLabelsProductShowPluFlag()			{ return m_LabelsProdPlu.GetValue(); }
	bool GetLabelsProductShowStockCodeFlag()	{ return m_LabelsProdStock.GetValue(); }
	bool GetLabelsProductShowSuppRefFlag()		{ return m_LabelsProdSupp.GetValue(); }
	bool GetLabelsProductTrimPluZerosFlag()		{ return m_LabelsProdTrimZero.GetValue(); }
	int GetLabelsProductPriceLevel1()			{ return m_LabelsProdPrice1.GetValue(); }
	int GetLabelsProductPriceLevel2()			{ return m_LabelsProdPrice2.GetValue(); }
	int GetLabelsProductTaxType1()				{ return m_LabelsProdTax1.GetValue(); }
	int GetLabelsProductTaxType2()				{ return m_LabelsProdTax2.GetValue(); }
	const char* GetLabelsProductPriceSymbol1()	{ return m_LabelsProdSymbol1.GetValue(); }
	const char* GetLabelsProductPriceSymbol2()	{ return m_LabelsProdSymbol2.GetValue(); }
	
	//LABELS2 TAB
	const char* GetLabelsShelfNameSingle()		{ return m_LabelsShelfNameSingle.GetValue(); }
	const char* GetLabelsShelfNameBatch()		{ return m_LabelsShelfNameBatch.GetValue(); }
	bool GetLabelsShelfAutoBatchFlag()			{ return m_LabelsShelfAutoBatch.GetValue(); }
	bool GetLabelsShelfPromptBatchFlag()		{ return m_LabelsShelfPromptBatch.GetValue(); }
	bool GetLabelsShelfPrintSingleFlag()		{ return m_LabelsShelfPrintSingle.GetValue(); }
	bool GetLabelsShelfPromptSingleFlag()		{ return m_LabelsShelfPromptSingle.GetValue(); }
	const char* GetLabelsProductNameSingle()	{ return m_LabelsProdNameSingle.GetValue(); }
	const char* GetLabelsProductNameBatch()		{ return m_LabelsProdNameBatch.GetValue(); }
	bool GetLabelsProductAutoBatchFlag()		{ return m_LabelsProdAutoBatch.GetValue(); }
	bool GetLabelsProductPrintSingleFlag()		{ return m_LabelsProdPrintSingle.GetValue(); }
	bool GetLabelsProductPromptSingleFlag()		{ return m_LabelsProdPromptSingle.GetValue(); }

	//PASSWORDS
	bool GetPasswordsLogFlag()					{ return m_PasswordsLog.GetValue(); }

	//CUSTOM MESSAGE BOX
	bool GetCustomMessageBoxFlag()				{ return m_CustomMessageBox.GetValue(); }
	int GetCustomMessageBoxFont()				{ return m_CustomMessageBoxFont.GetValue(); }
	int GetCustomMessageBoxLineGap()			{ return m_CustomMessageBoxLineGap.GetValue(); }
	const char* GetCustomMeesageBoxColourBack()	{ return m_CustomMessageBoxColourBack.GetValue(); }
	const char* GetCustomMeesageBoxColourText()	{ return m_CustomMessageBoxColourText.GetValue(); }

	//COLOURS
	int GetCustomMessageBoxColourBackInt() { return m_nCustomMessageBoxColourBack; }
	int GetCustomMessageBoxColourTextInt() { return m_nCustomMessageBoxColourText; }

	//STARTUP MESSAGE BOX
	bool GetStartupMessageBoxFlag() { return m_StartupMessageBox.GetValue(); }
	int GetStartupMessageBoxFont() { return m_StartupMessageBoxFont.GetValue(); }
	int GetStartupMessageBoxLineGap() { return m_StartupMessageBoxLineGap.GetValue(); }
	const char* GetStartupMeesageBoxColourBack() { return m_StartupMessageBoxColourBack.GetValue(); }
	const char* GetStartupMeesageBoxColourText() { return m_StartupMessageBoxColourText.GetValue(); }

	//COLOURS
	int GetStartupMessageBoxColourBackInt()		{ return m_nStartupMessageBoxColourBack; }
	int GetStartupMessageBoxColourTextInt()		{ return m_nStartupMessageBoxColourText; }

private:
	int MakeColourInt( CIniFileString& iniString );
	bool MakeColourString( int nColour, CString& strColour );

public:
	//PLU TAB
	void SetPluTabBarcodeFlag( bool b )				{ m_PluTabBarcode.SetValue(b); }
	void SetPluTabLinkFlag( bool b )				{ m_PluTabLink.SetValue(b); }
	void SetPluTabInfoFlag( bool b )				{ m_PluTabInfo.SetValue(b); }
	void SetPluPriceSetsFlag( bool b )				{ m_PluPriceSet.SetValue(b); }
	void SetPluDownloadByDateFlag( bool b )			{ m_PluDownloadDate.SetValue(b); }
	void SetPluInactiveNoDownloadFlag( bool b )		{ m_PluInactiveNoDownload.SetValue(b); }
	void SetPluInactiveAutoHideFlag( bool b)		{ m_PluInactiveAutoHide.SetValue(b); }
	void SetPluTaskAfterChangesFlag( bool b )		{ m_PluTask.SetValue(b); }
	void SetPluDefaultTextType( int n )				{ m_PluDefText.SetValue(n); }
	void SetPluDiscardUnusedModifierFlag( bool b )	{ m_PluDiscardMod.SetValue(b); }
	void SetPluCSVAutoImportFlag( bool b )			{ m_PluAutoImport.SetValue(b); }
	void SetPluCSVManualImportFlag ( bool b )		{ m_PluManImport.SetValue(b); }
	void SetPluCSVManualExportFlag ( bool b )		{ m_PluManExport.SetValue(b); }
	void SetPluExportPurchaseHistoryFlag ( bool b )	{ m_PluPurchase.SetValue(b); }
	void SetPluProdCSVImportType( int n )			{ m_PluProdCSV.SetValue(n); }
	void SetPluAutoDeleteBarcodeType( int n )		{ m_PluDelBarcode.SetValue(n); }
	void SetPluBarcodeModifierFlag( bool b )		{ m_PluBarcodeMod.SetValue(b); }
	void SetPluAutoModEposTextType( int n )			{ m_PluAutoModEposText.SetValue(n); }
	void SetPluAutoModKeyTextType( int n )			{ m_PluAutoModKeyText.SetValue(n); }

	//GLOBAL TAB
	void SetGlobalDepartmentType( int n )			{ m_GlobalDept.SetValue(n); }
	void SetGlobalDeptGroupEposFlag( bool b )		{ m_GlobalDeptGroupEpos.SetValue(b); }
	void SetGlobalTaxFlag( bool b )					{ m_GlobalTax.SetValue(b); }
	void SetGlobalModifierFlag( bool b )			{ m_GlobalMod.SetValue(b); }
	void SetGlobalAnalysisCategoryFlag( bool b )	{ m_GlobalACat.SetValue(b ? 1 : 0); }
	void SetGlobalPaidInOutFlag(  bool b )			{ m_GlobalPaidIO.SetValue(b); }
	void SetGlobalPaymentType( int n )				{ m_GlobalPay.SetValue(n); }

	//FEATURES TAB
	void SetFeaturesPluStockFlag( bool b )			{ m_FeaturesPluStock.SetValue(b); }
	void SetFeaturesPluAuditFlag( bool b )			{ m_FeaturesPluAudit.SetValue(b); }
	void SetFeaturesLegacyEposDeliveryType( int n )	{ m_FeaturesLegacyEposDelivery.SetValue(n); }
	void SetFeaturesEposDeliveryType( int n )		{ m_FeaturesEposDelivery.SetValue(n); }
	void SetFeaturesMixMatchFlag( bool b )			{ m_FeaturesMixMatch.SetValue(b); }
	void SetFeaturesPromotionFlag( bool b )			{ m_FeaturesPromotion.SetValue(b); }
	void SetFeaturesTimePlanFlag( bool b )			{ m_FeaturesTimePlan.SetValue(b); }
	void SetFeaturesTimePlanDownloadFlag( bool b )	{ m_FeaturesTimePlanDL.SetValue(b); }
	void SetFeaturesTimePlanTemplateFlag(bool b)	{ m_FeaturesTimePlanTemplate.SetValue(b); }
	void SetFeaturesWalletStampFlag(bool b)	{ m_FeaturesWalletStampRule.SetValue(b); }
	void SetFeaturesGlobalWalletStampFlag(bool b) { m_FeaturesGlobalWalletStampRule.SetValue(b); }
	void SetFeaturesKeyboardFlag( bool b )			{ m_FeaturesKeyboard.SetValue(b); }
	void SetFeaturesKeyboardsPerLocation( int n )	{ m_FeaturesLocKeyboards.SetValue(n); }
	void SetFeaturesKeyboardPriceSetFlag( bool b )	{ m_FeaturesKBPriceSet.SetValue(b); }
	void SetFeaturesKeyboardShareFlag( bool b )		{ m_FeaturesKBShare.SetValue(b); }
	void SetFeaturesKeyboardTextOrder( int n )		{ m_FeaturesKBTextOrder.SetValue(n); }
	void SetFeaturesServerSettingsType( int n )		{ m_FeaturesServerType.SetValue(n); }
	void SetFeaturesServerRestrictionFlag( bool b )	{ m_FeaturesServerRestrict.SetValue(b); }
	void SetFeaturesServerPresetFlag( bool b )		{ m_FeaturesServerPreset.SetValue(b); }
	void SetFeaturesServerLogonFlag(bool b)			{ m_FeaturesServerLogon.SetValue(b); }
	void SetFeaturesCustomerBalanceFlag( bool b )	{ m_FeaturesCustomerBal.SetValue(b); }
	void SetFeaturesCustomerNameImportType( int n )	{ m_FeaturesCustomerName.SetValue(n); }
	void SetFeaturesCustomerAccountDelDay( int n )	{ m_FeaturesCustomerAccountDelDay.SetValue(n); }
	void SetFeaturesCustomerOneOffDelDay( int n )	{ m_FeaturesCustomerOneOffDelDay.SetValue(n); }
	void SetFeaturesDBaseEditType( int n );
	void SetFeaturesBackgroundColour( int n )		{ m_FeaturesBackCol.SetValue(n); }
	void SetFeaturesContextForceNowFlag( bool b )	{ m_FeaturesContextNow.SetValue(b); }
	void SetFeaturesOPNameFirstFlag(bool b)			{ m_FeaturesOPNameFirst.SetValue(b); }
	void SetFeaturesAllergenFlag( bool b )			{ m_FeaturesAllergen.SetValue(b); }

	//REPORTS1 TAB
	void SetReportsFinancialYearStart( const char* sz )	{ m_ReportsFYearStart.SetValue(sz); }
	void SetReportsTaxQuarterStart( int n )				{ m_ReportsTaxQStart.SetValue(n); }
	void SetReportsWeekStartDay( int n )				{ m_ReportsWeekStart.SetValue(n); }
	void SetReportsWeekNumberingType( int n )			{ m_ReportsWeekNumber.SetValue(n); }
	void SetReportsDayStartHour( int n )				{ m_ReportsDayHour.SetValue(n); }
	void SetReportsDayStartMinute( int n )				{ m_ReportsDayMinute.SetValue(n); }
	void SetReportsTimeAttendSignOutHour( int n )		{ m_ReportsTimeAttendHour.SetValue(n); }
	void SetReportsTimeAttendSignOutMinute( int n )		{ m_ReportsTimeAttendMinute.SetValue(n); }
	void SetReportsLocationSortType( int n )			{ m_ReportsLocSort.SetValue(n); }
	void SetReportsSalesTaxType( int n )				{ m_ReportsSalesTaxType.SetValue(n); }
	void SetReportsReportpointFlag( bool b )			{ m_ReportsRepPoint.SetValue(b); }
	void SetReportsLocationSetFlag( bool b )			{ m_ReportsLocSet.SetValue(b); }
	void SetReportsTrimPluZerosFlag( bool b )			{ m_ReportsTrimZero.SetValue(b); }
	void SetReportsRegionDateRangeFlag( bool b )		{ m_ReportsRegion.SetValue(b); }
	void SetReportsCustomerTransactionFlag( bool b )	{ m_ReportsCustTran.SetValue(b); }
	void SetReportsCustomerStatementFlag( bool b )		{ m_ReportsCustState.SetValue(b); }
	void SetReportsCustomerContactFlag( bool b )		{ m_ReportsCustContact.SetValue(b); }
	void SetReportsContactBeforeTime( int n )			{ m_ReportsContactBeforeTime.SetValue(n); }
	void SetReportsContactAfterTime( int n )			{ m_ReportsContactAfterTime.SetValue(n); }
	void SetReportsContactVisitTime( int n )			{ m_ReportsContactVisitTime.SetValue(n); }
	void SetReportsDefaultDateRange( const char* sz )	{ m_ReportsDateRange.SetValue(sz); }
	void SetReportsSingleDayRanges( int n )				{ m_ReportsSingleDayRanges.SetValue(n); }
	void SetReportsPreviousDayRanges( int n )			{ m_ReportsPreviousDayRanges.SetValue(n); }
	void SetReportsEODSessionFlag( bool b )				{ m_ReportsEODSession.SetValue(b); }
	void SetReportsEcrLiteLogMode( int n )				{ m_ReportsEcrLiteLogMode.SetValue(n); }

	//REPORTS2 TAB
	void SetReportsVIPType( int n )						{ m_ReportsVIPType.SetValue(n); }
	void SetReportsVIPLevel( int n )					{ m_ReportsVIPLevel.SetValue(n); }
	void SetReportsVIPRetailFlag( bool b )				{ m_ReportsVIPRetail.SetValue(b); }
	void SetReportsCustomerSortType( int n )			{ m_ReportsCustSortType.SetValue(n); }
	void SetReportsSalesTimeType( int n )				{ m_ReportsSalesTimeType.SetValue(n); }
	void SetReportsDiscountDetailSalesFlag( bool b )	{ m_ReportsDiscountDetailSales.SetValue(b); }
	void SetReportsPremiumDetailSalesFlag(bool b)		{ m_ReportsPremiumDetailSales.SetValue(b); }
	void SetReportsNetOfTaxSalesFlag( bool b )			{ m_ReportsNetOfTaxSales.SetValue(b); }
	void SetReportsDiscountDetailCoversFlag( bool b )	{ m_ReportsDiscountDetailCovers.SetValue(b ? 2 : 1); }
	void SetReportsPremiumDetailCoversFlag(bool b)		{ m_ReportsPremiumDetailCovers.SetValue(b); }
	void SetReportsNetOfTaxCoversFlag( bool b )			{ m_ReportsNetOfTaxCovers.SetValue(b); }
	void SetReportsCashRSPItemTextFlag( bool b )		{ m_ReportsCashRSPItemText.SetValue(b); }
	void SetReportsAuthServerFlag( bool b )				{ m_ReportsAuthServer.SetValue(b); }
	void SetReportsGroupReportpointFlag( bool b )		{ m_ReportsGroupReportpoint.SetValue(b); }
	void SetReportsEODReasonsFlag( bool b )				{ m_ReportsEODReasons.SetValue(b); }
	void SetReportsGeneralPageFlag( bool b )			{ m_ReportsGeneralPage.SetValue(b); }
	void SetReportsShowCashChangeFlag( bool b )			{ m_ReportsChange.SetValue(b); }
	void SetReportsMultiCashMethod( int n )				{ m_ReportsMultiCash.SetValue(n); }
	void SetReportsShowPaymentQuantityFlag( bool b )	{ m_ReportsPayQty.SetValue(b); }
	void SetReportsShowRAPaymentsFlag( bool b )			{ m_ReportsRAPay.SetValue(b); }
	void SetReportsSeparateRAPaymentsFlag( bool b )		{ m_ReportsSplitRAPay.SetValue(b); }
	void SetReportsShowRABalanceFlag( bool b )			{ m_ReportsRABal.SetValue(b); }
	void SetReportsSeparateRABalanceFlag( bool b )		{ m_ReportsSplitRABal.SetValue(b); }
	void SetReportsCashRSPDiscountPennyFlag( bool b)	{ m_ReportsCashRSPDiscountPenny.SetValue(b); }

	//AUTO EXPIRE TAB
	void SetAutoExpireTimePluReports( int n )			{ m_ExpireSales.SetValue(n); }
	void SetAutoExpireTimeSummaryReports( int n )		{ m_ExpireSummary.SetValue(n); }
	void SetAutoExpireTimeDetailReports( int n )		{ m_ExpireDetail.SetValue(n); }
	//void SetAutoExpireTimeAttendReports(int n)			{ m_ExpireTimeAttend.SetValue(n); }
	void SetAutoExpireTimePluDeliveryAudit( int n )		{ m_ExpireAudit.SetValue(n); }

	//LABELS1 TAB
	void SetLabelsShelfShowPluFlag( bool b )			{ m_LabelsShelfPlu.SetValue(b); }
	void SetLabelsShelfShowStockCodeFlag( bool b )		{ m_LabelsShelfStock.SetValue(b); }
	void SetLabelsShelfShowSuppRefFlag( bool b )		{ m_LabelsShelfSupp.SetValue(b); }
	void SetLabelsShelfTrimPluZerosFlag( bool b )		{ m_LabelsShelfTrimZero.SetValue(b); }
	void SetLabelsShelfPriceLevel1( int n )				{ m_LabelsShelfPrice1.SetValue(n); }
	void SetLabelsShelfPriceLevel2( int n )				{ m_LabelsShelfPrice2.SetValue(n); }
	void SetLabelsShelfTaxType1( int n )				{ m_LabelsShelfTax1.SetValue(n); }
	void SetLabelsShelfTaxType2( int n )				{ m_LabelsShelfTax2.SetValue(n); }
	void SetLabelsShelfPriceSymbol1( const char* sz )	{ m_LabelsShelfSymbol1.SetValue(sz); }
	void SetLabelsShelfPriceSymbol2( const char* sz )	{ m_LabelsShelfSymbol2.SetValue(sz); }
	void SetLabelsProductShowPluFlag( bool b )			{ m_LabelsProdPlu.SetValue(b); }
	void SetLabelsProductShowStockCodeFlag( bool b )	{ m_LabelsProdStock.SetValue(b); }
	void SetLabelsProductShowSuppRefFlag( bool b )		{ m_LabelsProdSupp.SetValue(b); }
	void SetLabelsProductTrimPluZerosFlag( bool b )		{ m_LabelsProdTrimZero.SetValue(b); }
	void SetLabelsProductPriceLevel1( int n )			{ m_LabelsProdPrice1.SetValue(n); }
	void SetLabelsProductPriceLevel2( int n )			{ m_LabelsProdPrice2.SetValue(n); }
	void SetLabelsProductTaxType1( int n )				{ m_LabelsProdTax1.SetValue(n); }
	void SetLabelsProductTaxType2( int n )				{ m_LabelsProdTax2.SetValue(n); }
	void SetLabelsProductPriceSymbol1( const char* sz )	{ m_LabelsProdSymbol1.SetValue(sz); }
	void SetLabelsProductPriceSymbol2( const char* sz )	{ m_LabelsProdSymbol2.SetValue(sz); }
	
	//LABELS2 TAB
	void SetLabelsShelfNameSingle( const char* sz )		{ m_LabelsShelfNameSingle.SetValue(sz); }
	void SetLabelsShelfNameBatch( const char* sz )		{ m_LabelsShelfNameBatch.SetValue(sz); }
	void SetLabelsShelfAutoBatchFlag( bool b )			{ m_LabelsShelfAutoBatch.SetValue(b); }
	void SetLabelsShelfPromptBatchFlag( bool b )		{ m_LabelsShelfPromptBatch.SetValue(b); }
	void SetLabelsShelfPrintSingleFlag( bool b )		{ m_LabelsShelfPrintSingle.SetValue(b); }
	void SetLabelsShelfPromptSingleFlag( bool b )		{ m_LabelsShelfPromptSingle.SetValue(b); }
	void SetLabelsProductNameSingle( const char* sz )	{ m_LabelsProdNameSingle.SetValue(sz); }
	void SetLabelsProductNameBatch( const char* sz )	{ m_LabelsProdNameBatch.SetValue(sz); }
	void SetLabelsProductAutoBatchFlag( bool b )		{ m_LabelsProdAutoBatch.SetValue(b); }
	void SetLabelsProductPrintSingleFlag( bool b )		{ m_LabelsProdPrintSingle.SetValue(b); }
	void SetLabelsProductPromptSingleFlag( bool b )		{ m_LabelsProdPromptSingle.SetValue(b); }

	//PASSWORDS
	void SetPasswordsLogFlag( bool b )					{ m_PasswordsLog.SetValue(b); }

	//CUSTOM MESSAGE BOX
	void SetCustomMessageBoxFlag( bool b )				{ m_CustomMessageBox.SetValue(b); }
	void SetCustomMessageBoxFont( int n );
	void SetCustomMessageBoxLineGap( int n )			{ m_CustomMessageBoxLineGap.SetValue(n); }
	void SetCustomMessageBoxColourBack( const char* sz ){ m_CustomMessageBoxColourBack.SetValue(sz); }
	void SetCustomMessageBoxColourText( const char* sz ){ m_CustomMessageBoxColourText.SetValue(sz); }

	//COLOURS
	void SetCustomMessageBoxColourBackInt( int n );
	void SetCustomMessageBoxColourTextInt( int n );

	//STARTUP MESSAGE BOX
	void SetStartupMessageBoxFlag(bool b) { m_StartupMessageBox.SetValue(b); }
	void SetStartupMessageBoxFont(int n);
	void SetStartupMessageBoxLineGap(int n) { m_StartupMessageBoxLineGap.SetValue(n); }
	void SetStartupMessageBoxColourBack(const char* sz) { m_StartupMessageBoxColourBack.SetValue(sz); }
	void SetStartupMessageBoxColourText(const char* sz) { m_StartupMessageBoxColourText.SetValue(sz); }

	//COLOURS
	void SetStartupMessageBoxColourBackInt(int n);
	void SetStartupMessageBoxColourTextInt(int n);
	
	//AUTO CUSTOMER DELETE FLAG
	bool GetFeaturesCustomerAccountAutoDeleteFlag();
	bool GetFeaturesCustomerOneOffAutoDeleteFlag();
	
public:
	bool AllowBarcodeModifierLinks();
	bool AllowPluDeliveryAudit() { return ( m_FeaturesPluStock.GetValue() && m_FeaturesPluAudit.GetValue() ); }
	void GetFYearStart ( int& nDay, int& nMonth );
	bool HasIdleTimeCustomerTask();

public:
	const char* GetSalesTaxName();
	const char* GetEstTaxString();
	const char* GetExTaxString();
	const char* GetTaxContentString();
	const char* GetTaxBandString();
	const char* GetTaxString( const char* szSource );
	
private:
	//PLU TAB
	CIniFileBool m_PluTabBarcode;
	CIniFileBool m_PluTabLink;
	CIniFileBool m_PluTabInfo;
	CIniFileBool m_PluPriceSet;
	CIniFileBool m_PluDownloadDate;
	CIniFileBool m_PluInactiveNoDownload;
	CIniFileBool m_PluInactiveAutoHide;
	CIniFileBool m_PluTask;
	CIniFileInt m_PluDefText;
	CIniFileBool m_PluDiscardMod;
	CIniFileBool m_PluAutoImport;
	CIniFileBool m_PluManImport;
	CIniFileBool m_PluManExport;
	CIniFileBool m_PluPurchase;
	CIniFileInt m_PluProdCSV;
	CIniFileInt m_PluDelBarcode;
	CIniFileBool m_PluBarcodeMod;
	CIniFileInt m_PluAutoModEposText;
	CIniFileInt m_PluAutoModKeyText;

	//GLOBAL TAB
	CIniFileInt m_GlobalDept;
	CIniFileBool m_GlobalDeptGroupEpos;
	CIniFileBool m_GlobalTax;
	CIniFileBool m_GlobalMod;
	CIniFileBool m_GlobalACatLegacy;
	CIniFileInt m_GlobalACat;
	CIniFileBool m_GlobalPaidIO;
	CIniFileInt m_GlobalPay;
	CIniFileBool m_GlobalPayGroupReport;

	//FEATURES TAB
	CIniFileBool m_FeaturesPluStock;
	CIniFileBool m_FeaturesPluAudit;
	CIniFileInt m_FeaturesLegacyEposDelivery;
	CIniFileInt m_FeaturesEposDelivery;
	CIniFileBool m_FeaturesMixMatch;
	CIniFileBool m_FeaturesPromotion;
	CIniFileBool m_FeaturesTimePlan;
	CIniFileBool m_FeaturesTimePlanDL;
	CIniFileBool m_FeaturesTimePlanTemplate;
	CIniFileBool m_FeaturesWalletStampRule;
	CIniFileBool m_FeaturesGlobalWalletStampRule;
	CIniFileBool m_FeaturesKeyboard;
	CIniFileInt m_FeaturesLocKeyboards;
	CIniFileBool m_FeaturesKBPriceSet;
	CIniFileBool m_FeaturesKBShare;
	CIniFileInt m_FeaturesKBTextOrder;
	CIniFileInt m_FeaturesServerType;
	CIniFileBool m_FeaturesServerRestrict;
	CIniFileBool m_FeaturesServerPreset;
	CIniFileBool m_FeaturesServerLogon;
	CIniFileBool m_FeaturesCustomerBal;
	CIniFileInt m_FeaturesCustomerName;
	CIniFileInt m_FeaturesCustomerAccountDelDay;
	CIniFileInt m_FeaturesCustomerOneOffDelDay;
	CIniFileInt m_FeaturesDBaseEdit;
	CIniFileInt m_FeaturesBackCol;
	CIniFileBool m_FeaturesContextNow;
	CIniFileBool m_FeaturesOPNameFirst;
	CIniFileBool m_FeaturesAllergen;

	//REPORTS1 TAB
	CIniFileString m_ReportsFYearStart;
	CIniFileInt m_ReportsTaxQStart;
	CIniFileInt m_ReportsWeekStart;
	CIniFileInt	m_ReportsWeekNumber;
	CIniFileInt	m_ReportsDayHour;
	CIniFileInt	m_ReportsDayMinute;
	CIniFileInt	m_ReportsTimeAttendHour;
	CIniFileInt	m_ReportsTimeAttendMinute;
	CIniFileInt	m_ReportsLocSort;
	CIniFileInt m_ReportsSalesTaxType;
	CIniFileBool m_ReportsRepPoint;
	CIniFileBool m_ReportsLocSet;
	CIniFileBool m_ReportsTrimZero;
	CIniFileBool m_ReportsRegion;
	CIniFileBool m_ReportsCustTran;
	CIniFileBool m_ReportsCustState;
	CIniFileBool m_ReportsCustContact;
	CIniFileInt m_ReportsContactBeforeTime;
	CIniFileInt m_ReportsContactAfterTime;
	CIniFileInt m_ReportsContactVisitTime;
	CIniFileString m_ReportsDateRange;
	CIniFileInt m_ReportsSingleDayRanges;
	CIniFileInt m_ReportsPreviousDayRanges;
	CIniFileBool m_ReportsEODSession;
	CIniFileInt m_ReportsEcrLiteLogMode;

	//REPORTS2 TAB
	CIniFileInt m_ReportsVIPType;
	CIniFileInt m_ReportsVIPLevel;
	CIniFileBool m_ReportsVIPRetail;
	CIniFileInt m_ReportsCustSortType;
	CIniFileInt m_ReportsSalesTimeType;
	CIniFileBool m_ReportsDiscountDetailSales;
	CIniFileBool m_ReportsPremiumDetailSales;
	CIniFileBool m_ReportsNetOfTaxSales;
	CIniFileInt m_ReportsDiscountDetailCovers;
	CIniFileBool m_ReportsPremiumDetailCovers;
	CIniFileBool m_ReportsNetOfTaxCovers;
	CIniFileBool m_ReportsCashRSPItemText;
	CIniFileBool m_ReportsAuthServer;
	CIniFileBool m_ReportsGroupReportpoint;
	CIniFileBool m_ReportsEODReasons;
	CIniFileBool m_ReportsGeneralPage;
	CIniFileBool m_ReportsChange;
	CIniFileInt m_ReportsMultiCash;
	CIniFileBool m_ReportsPayQty;
	CIniFileBool m_ReportsRAPay;
	CIniFileBool m_ReportsSplitRAPay;
	CIniFileBool m_ReportsRABal;
	CIniFileBool m_ReportsSplitRABal;
	CIniFileBool m_ReportsCashRSPDiscountPenny;

	//AUTO EXPIRE TAB
	CIniFileInt m_ExpireSales;
	CIniFileInt m_ExpireAudit;
	CIniFileInt m_ExpireSummary;
	CIniFileInt m_ExpireDetail;
	//CIniFileInt m_ExpireTimeAttend;

	//LABELS1 TAB
	CIniFileBool m_LabelsShelfPlu;
	CIniFileBool m_LabelsShelfStock;
	CIniFileBool m_LabelsShelfSupp;
	CIniFileBool m_LabelsShelfTrimZero;
	CIniFileInt m_LabelsShelfPrice1;
	CIniFileInt m_LabelsShelfPrice2;
	CIniFileInt m_LabelsShelfTax1;
	CIniFileInt m_LabelsShelfTax2;
	CIniFileString m_LabelsShelfSymbol1;
	CIniFileString m_LabelsShelfSymbol2;
	CIniFileBool m_LabelsProdPlu;
	CIniFileBool m_LabelsProdStock;
	CIniFileBool m_LabelsProdSupp;
	CIniFileBool m_LabelsProdTrimZero;
	CIniFileInt m_LabelsProdPrice1;
	CIniFileInt m_LabelsProdPrice2;
	CIniFileInt m_LabelsProdTax1;
	CIniFileInt m_LabelsProdTax2;
	CIniFileString m_LabelsProdSymbol1;
	CIniFileString m_LabelsProdSymbol2;
	
	//LABELS2 TAB
	CIniFileString m_LabelsShelfNameSingle;
	CIniFileString m_LabelsShelfNameBatch;
	CIniFileBool m_LabelsShelfAutoBatch;
	CIniFileBool m_LabelsShelfPromptBatch;
	CIniFileBool m_LabelsShelfPrintSingle;
	CIniFileBool m_LabelsShelfPromptSingle;
	CIniFileString m_LabelsProdNameSingle;
	CIniFileString m_LabelsProdNameBatch;
	CIniFileBool m_LabelsProdAutoBatch;
	CIniFileBool m_LabelsProdPrintSingle;
	CIniFileBool m_LabelsProdPromptSingle;

	//PASSWORDS
	CIniFileBool m_PasswordsLog;

	//CUSTOM MESSAGE BOX
	CIniFileBool m_CustomMessageBox;
	CIniFileInt m_CustomMessageBoxFont;
	CIniFileInt m_CustomMessageBoxLineGap;
	CIniFileString m_CustomMessageBoxColourBack;
	CIniFileString m_CustomMessageBoxColourText;

	//COLOURS
	int m_nCustomMessageBoxColourBack;
	int m_nCustomMessageBoxColourText;

	//STARTUP MESSAGE BOX
	CIniFileBool m_StartupMessageBox;
	CIniFileInt m_StartupMessageBoxFont;
	CIniFileInt m_StartupMessageBoxLineGap;
	CIniFileString m_StartupMessageBoxColourBack;
	CIniFileString m_StartupMessageBoxColourText;

	//COLOURS
	int m_nStartupMessageBoxColourBack;
	int m_nStartupMessageBoxColourText;

private:
	//SALES TAX
	CString m_strSalesTaxName;
	CString m_strSalesTaxLabel;
	
private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CEcrmanOptions EcrmanOptions;		/* global */
/**********************************************************************/
