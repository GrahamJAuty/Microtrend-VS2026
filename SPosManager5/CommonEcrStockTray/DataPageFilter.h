#pragma once
/**********************************************************************/
#define DATAPAGE_TYPE_ECRDATA 0
#define DATAPAGE_TYPE_STOCKDATA 1
#define DATAPAGE_TYPE_ECRTEXT 2
#define DATAPAGE_TYPE_STOCKTEXT 3
#define DATAPAGE_TYPE_REPORTSET 4
#define DATAPAGE_TYPE_KEYBOARD 5
#define DATAPAGE_TYPE_SERVER 6
#define DATAPAGE_TYPE_MIXMATCH 7
#define DATAPAGE_TYPE_CUSTOMER 8
#define DATAPAGE_TYPE_PRICESET 9
#define DATAPAGE_TYPE_BILLTEXT 10
#define DATAPAGE_TYPE_FUNCTION 11
#define DATAPAGE_TYPE_OPGROUP 12
#define DATAPAGE_TYPE_SVRPRESET 13
#define DATAPAGE_TYPE_TABLES 14
#define DATAPAGE_TYPE_MACRO 15
#define DATAPAGE_TYPE_REASON 16
/**********************************************************************/
#define SCOPE_GLOBAL 0
#define SCOPE_DBASE 1
#define SCOPE_STKPOINT 2
#define SCOPE_HIDE 3
/**********************************************************************/

struct CDataPageFilter
{
public:
	CDataPageFilter();
	void SetAllToHide();

public:
	void PrepareEcrDataFilter();
	void PreparePriceSetFilter();
	void PrepareMixMatchFilter();
	void PrepareEcrTextFilter();
	void PrepareReportSetFilter();

#ifdef STOCKMAN_SYSTEM
	void PrepareStockDataFilter();
	void PrepareStockTextFilter();
#endif

public:
	bool GotLocalData( int nDataListType );
	bool GotGlobalData( int nDataListType );
	bool GotSystemFilterData( int nDataListType );
	bool GotTimeSettingData( int nDataListType );
	bool GotReportListData( int nDataListType );
	bool GotStockpointData( int nDataListType );
	int GetDbTransferTargets( int nDbIdx );

public:
	int GetAdjustmentsFlag(){ return m_nShowAdjustments; }
	int GetAdjustTextFlag(){ return m_nShowAdjustText; }
	int GetAllergenFlag(){ return m_nShowAllergen; }
	int GetAllowanceFlag(){ return m_nShowAllowance; }
	int GetBarcodeFlag(){ return m_nShowBarcode; }
	int GetBatchEposFlag(){ return m_nShowBatchEpos; }
	int GetCategoryFlag(){ return m_nShowCategory; }
	int GetCategorySetFlag(){ return m_nShowCategorySet; }
	int GetConsolGroupFlag(){ return m_nShowConsolGroup; }
	int GetCustomFieldFlag(){ return m_nShowCustomField; }
	int GetCustomFieldSetFlag(){ return m_nShowCustomFieldSet; }
	int GetDeliveriesFlag(){ return m_nShowDeliveries; }
	int GetDelUnitFlag(){ return m_nShowDelUnit; }
	int GetDeptFlag(){ return m_nShowDept; }
	int GetDeptSetFlag(){ return m_nShowDeptSet; }
	int GetEcrSupplierFlag(){ return m_nShowEcrSupplier; }
	int GetEposGroupFlag(){ return m_nShowEposGroup; }
	int GetEposReportsFlag(){ return m_nShowEposReports; }
	int GetManualSalesFlag(){ return m_nShowManualSales; }
	int GetMixMatchFlag(){ return m_nShowMixMatch; }
	int GetWalletStampRuleFlag() { return m_nShowWalletStampRule; }
	int GetLoyaltySchemeFlag() { return m_nShowLoyaltyScheme; }
	int GetModifierFlag(){ return m_nShowModifier; }
	int GetOfferGroupFlag(){ return m_nShowOfferGroup; }
	int GetOpeningFlag(){ return m_nShowOpening; }
	int GetPaidInOutTextFlag(){ return m_nShowPaidInOutText; }
	int GetPaymentFlag(){ return m_nShowPayment; }
	int GetPluFlag(){ return m_nShowPlu; }
	int GetAnalysisCategoryFlag(){ return m_nShowAnalysisCategory; }
	int GetPluUnitFlag(){ return m_nShowPluUnit; }
	int GetPluFilterFlag(){ return m_nShowPluFilter; }
	int GetPriceTextFlag(){ return m_nShowPriceText; }
	int GetPromotionFlag(){ return m_nShowPromotion; }
	int GetReorderFlag(){ return m_nShowReorder; }
	int GetActiveStockFlag(){ return m_nShowActiveStock; }
	int GetReportGroupFlag(){ return m_nShowReportGroup; }
	int GetReturnsFlag(){ return m_nShowReturns; }
	int GetSalesHistoryPluFlag(){ return m_nShowSalesHistoryPlu; }
	int GetSalesHistoryStockFlag(){ return m_nShowSalesHistoryStock; }
	int GetSessionFlag(){ return m_nShowSession; }
	int GetStockFlag(){ return m_nShowStock; }
	int GetStockLevelReportsFlag(){ return m_nShowStockLevelReports; }
	int GetStockUnitFlag(){ return m_nShowStockUnit; }
	int GetSubUnitFlag(){ return m_nShowSubUnit; }
	int GetSupplierFlag(){ return m_nShowSupplier; }
	int GetSupplierSetFlag(){ return m_nShowSupplierSet; }
	int GetTaxFlag(){ return m_nShowTax; }
	int GetTimePlanFlag(){ return m_nShowTimePlan; }
	int GetTimeSliceMapsFlag(){ return m_nShowTimeSliceMaps; }
	int GetTransfersFlag(){ return m_nShowTransfers; }
	int GetUnitechFlag(){ return m_nShowUnitech; }

public:
	void SetPluFlag( int n ){ m_nShowPlu = n; }

private:
	int m_nShowAdjustments;
	int m_nShowAdjustText;
	int m_nShowAllergen;
	int m_nShowAllowance;
	int m_nShowBarcode;
	int m_nShowBatchEpos;
	int m_nShowCategory;
	int m_nShowCategorySet;
	int m_nShowConsolGroup;
	int m_nShowCustomField;
	int m_nShowCustomFieldSet;
	int m_nShowDeliveries;
	int m_nShowDelUnit;
	int m_nShowDept;
	int m_nShowDeptSet;
	int m_nShowEcrSupplier;
	int m_nShowEposGroup;
	int m_nShowEposReports;
	int m_nShowManualSales;
	int m_nShowMixMatch;
	int m_nShowModifier;
	int m_nShowOfferGroup;
	int m_nShowOpening;
	int m_nShowPaidInOutText;
	int m_nShowPayment;
	int m_nShowPlu;
	int m_nShowAnalysisCategory;
	int m_nShowPluFilter;
	int m_nShowPluUnit;
	int m_nShowPriceText;
	int m_nShowPromotion;
	int m_nShowWalletStampRule;
	int m_nShowLoyaltyScheme;
	int m_nShowReorder;
	int m_nShowActiveStock;
	int m_nShowReportGroup;
	int m_nShowReturns;
	int m_nShowSalesHistoryPlu;
	int m_nShowSalesHistoryStock;
	int m_nShowSession;
	int m_nShowStock;
	int m_nShowStockLevelReports;
	int m_nShowStockUnit;
	int m_nShowSubUnit;
	int m_nShowSupplier;
	int m_nShowSupplierSet;
	int m_nShowTax;
	int m_nShowTimePlan;
	int m_nShowTimeSliceMaps;
	int m_nShowTransfers;
	int m_nShowUnitech;

private:
	CWordArray m_arrayDbTransferTargets;
};

/**********************************************************************/

