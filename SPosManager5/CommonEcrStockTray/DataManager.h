#pragma once
/**********************************************************************/
#include "..\CommonStockTray\CustomField.h"
/**********************************************************************/
#include "..\CommonEcrStockTray\EcrmanSupplierCSVArray.h"
/**********************************************************************/
#include "..\CommonEcrStock\LabelListCSVArray.h"
#include "..\CommonEcrStock\ListKeyImageWrapperDept.h"
#include "..\CommonEcrStock\ListKeyImageWrapperPlu.h"
#include "..\CommonEcrStock\LoyaltySchemeCSVArray.h"
#include "..\CommonEcrStock\PluInfoResourceWrapper.h"
/**********************************************************************/
#include "..\SPosStockManager5\CategorySetCSVArray.h"
#include "..\SPosStockManager5\CustomFieldSetCSVArray.h"
#include "..\SPosStockManager5\CustomFieldManager.h"
#include "..\SPosStockManager5\DefTextAllowance.h"
#include "..\SPosStockManager5\DefTextCategory.h"
#include "..\SPosStockManager5\DefTextDelUnit.h"
#include "..\SPosStockManager5\OrderDatabase.h"
#include "..\SPosStockManager5\PluMonitorStockman.h"
#include "..\SPosStockManager5\StockMonitor.h"
#include "..\SPosStockManager5\StockMonitorPosTray.h"
#include "..\SPosStockManager5\StockPluArray.h"
#include "..\SPosStockManager5\SupplierSetCSVArray.h"
/**********************************************************************/
#include "..\SPosEcrManager5\PluMonitorEcrman.h"
/**********************************************************************/
#include "..\SPosTray5\PluMonitorPosTray.h"
/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
#include "BarcodeCSVArray.h"
#include "DataManagerBase.h"
#include "DepartmentCSVArray.h"
#include "DepartmentSetCSVArray.h"
#include "GroupCSVArrayConsol.h"
#include "GroupCSVArrayEpos.h"
#include "GroupCSVArrayPayment.h"
#include "GroupCSVArrayReport.h"
#include "HistoricalTaxRates.h"
#include "MixMatchCSVArray.h"
#include "ModifierCSVArray.h"
#include "OfferGroupsCSVArray.h"
#include "OfferGroupEntriesCSVArray.h"
#include "PaidInOutTextCSVArray.h"
#include "PaymentCSVArray.h"
#include "PriceTextCSVArray.h"
#include "PromotionsCSVArray.h"
#include "ServerCSVArray.h"
#include "SmartEntertainmentPaymentMapArray.h"
#include "SportsBookerPaymentMapArray.h"
#include "SportsBookerPriceBandCSVArray.h"
#include "TaxArray.h"
#include "TimePlansCSVArray.h"
#include "TimePlanEntriesCSVArray.h"
#include "WalletStampRuleCSVArray.h"
/**********************************************************************/

class CDataManager : public CDataManagerBase
{
public:
	CDataManager();
	~CDataManager();

public:
	void OpenDatabaseQuickNoFiles( int nDbIdx, bool bKeepIfAlreadyOpen );
	bool OpenDatabaseReadOnly( int nDbIdx, CDataManagerInfo& info, bool bDummyStockLevels = TRUE );
	bool OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CDataManagerInfo& info, bool bDummyStockLevels = TRUE );
	bool OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, CDataManagerInfo& info, bool bDummyStockLevels = TRUE );
	void CloseDatabase();

	int GetActiveDbIdx() { return m_nActiveDbIdx; }

	void SetPluPriceSetModeFlag( bool b ){ m_bPluPriceSetMode = b; }
	
private:
	bool OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, int nDefaultType, CDataManagerInfo& info );
	int GetPromoDbIdx( int nDbIdx );
	
protected:
	virtual bool OpenSpecialReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, int nDefaultType, CDataManagerInfo& info ){ return TRUE; }
	virtual bool OpenSpecialReadOnly( bool bSameDb, CDataManagerInfo& info ){ return TRUE; }
	virtual void CloseSpecial(){}

public:
	bool OpenBarcode( int nType, CDataManagerInfo& info );
	bool OpenDepartment( int nType, CDataManagerInfo& info );
	bool OpenDepartmentSet( int nType, CDataManagerInfo& info );
	bool OpenEposGroup( int nType, CDataManagerInfo& info );
	bool OpenReportGroup( int nType, CDataManagerInfo& info );
	bool OpenConsolGroup( int nType, CDataManagerInfo& info );
	bool OpenHistoricalTax( int nType, CDataManagerInfo& info );
	bool OpenMixMatch( int nType, CDataManagerInfo& info );
	bool OpenModifier( int nType, CDataManagerInfo& info );
	bool OpenPaidInText( int nType, CDataManagerInfo& info );
	bool OpenPaidOutText( int nType, CDataManagerInfo& info );
	bool OpenPayment( int nType, CDataManagerInfo& info );
	bool OpenPaymentGroup( int nType, CDataManagerInfo& info );
	bool OpenSBPaymentMap( int nType, CDataManagerInfo& info );
	bool OpenSEPaymentMap( int nType, CDataManagerInfo& info );
	bool OpenAnalysisCategory( int nType, CDataManagerInfo& info );
	bool OpenDbPluFilterNames( int nType, CDataManagerInfo& info );
	bool OpenDbPluFilterItems( int nType, CDataManagerInfo& info );
	bool OpenPriceText( int nType, CDataManagerInfo& info );
	bool OpenPromotion( int nType, CDataManagerInfo& info );
	bool OpenSBPriceText( int nType, CDataManagerInfo& info );
	bool OpenTaxRates( int nType, CDataManagerInfo& info );
	bool OpenTimePlan( int nType, CDataManagerInfo& info );
	bool OpenTimePlanEntry( int nType, CDataManagerInfo& info );
	bool OpenTimePlanTemplateEntry(int nType, CDataManagerInfo& info);
	bool OpenWalletStampRule(int nType, CDataManagerInfo& info);
	
	virtual bool OpenPlu( int nType, CDataManagerInfo& info ) = 0;

public:
	bool ReopenBarcode( CDataManagerInfo& info );
	bool ReopenDepartment( CDataManagerInfo& info );
	bool ReopenEposGroup( CDataManagerInfo& info );
	bool ReopenReportGroup( CDataManagerInfo& info );
	bool ReopenConsolGroup( CDataManagerInfo& info );
	bool ReopenMixMatch( CDataManagerInfo& info );
	bool ReopenModifier( CDataManagerInfo& info );
	bool ReopenPaidInText( CDataManagerInfo& info );
	bool ReopenPaidOutText( CDataManagerInfo& info );
	bool ReopenPayment( CDataManagerInfo& info );
	bool ReopenPaymentGroup( CDataManagerInfo& info );
	bool ReopenAnalysisCategory( CDataManagerInfo& info );
	bool ReopenDbPluFilterNames( CDataManagerInfo& info );
	bool ReopenDbPluFilterItems( CDataManagerInfo& info );
	bool ReopenPromotion( CDataManagerInfo& info );
	bool ReopenTaxRates( CDataManagerInfo& info );
	bool ReopenTimePlan( CDataManagerInfo& info );
	bool ReopenTimePlanEntry( CDataManagerInfo& info );
	bool ReopenTimePlanTemplateEntry(CDataManagerInfo& info);
	bool ReopenWalletStampRule(CDataManagerInfo& info);
	
public:
	bool CloseBarcode( CDataManagerInfo& info );
	bool CloseDepartment( CDataManagerInfo& info );
	bool CloseDepartmentSet( CDataManagerInfo& info );
	bool CloseEposGroup( CDataManagerInfo& info );
	bool CloseReportGroup( CDataManagerInfo& info );
	bool CloseConsolGroup( CDataManagerInfo& info );
	bool CloseHistoricalTax( CDataManagerInfo& info );
	bool CloseMixMatch( CDataManagerInfo& info );
	bool CloseModifier( CDataManagerInfo& info );
	bool ClosePaidInText( CDataManagerInfo& info );
	bool ClosePaidOutText( CDataManagerInfo& info );
	bool ClosePayment( CDataManagerInfo& info );
	bool ClosePaymentGroup( CDataManagerInfo& info );
	bool CloseSBPaymentMap( CDataManagerInfo& info );
	bool CloseSEPaymentMap( CDataManagerInfo& info );
	bool CloseAnalysisCategory( CDataManagerInfo& info );
	bool CloseDbPluFilterNames( CDataManagerInfo& info );
	bool CloseDbPluFilterItems( CDataManagerInfo& info );
	bool ClosePriceText( CDataManagerInfo& info );
	bool ClosePromotion( CDataManagerInfo& info );
	bool CloseSBPriceText( CDataManagerInfo& info );
	bool CloseTaxRates( CDataManagerInfo& info );
	bool CloseTimePlan( CDataManagerInfo& info );
	bool CloseTimePlanEntry( CDataManagerInfo& info );
	bool CloseTimePlanTemplateEntry(CDataManagerInfo& info);
	bool CloseWalletStampRule(CDataManagerInfo& info);
	virtual bool ClosePlu( CDataManagerInfo& info ) = 0;
			
public:
	bool WriteBarcode( CDataManagerInfo& info );
	bool WriteDepartment( CDataManagerInfo& info );
	bool WriteDepartmentSet( CDataManagerInfo& info );
	bool WriteEposGroup( CDataManagerInfo& info );
	bool WriteReportGroup( CDataManagerInfo& info );
	bool WriteConsolGroup( CDataManagerInfo& info );
	bool WriteHistoricalTax( CDataManagerInfo& info );
	bool WriteMixMatch( CDataManagerInfo& info );
	bool WriteModifier( CDataManagerInfo& info );
	bool WritePaidInText( CDataManagerInfo& info );
	bool WritePaidOutText( CDataManagerInfo& info );
	bool WritePayment( CDataManagerInfo& info );
	bool WritePaymentGroup( CDataManagerInfo& info );
	bool WriteSBPaymentMap( CDataManagerInfo& info );
	bool WriteSEPaymentMap( CDataManagerInfo& info );
	bool WriteAnalysisCategory( CDataManagerInfo& info );
	bool WriteDbPluFilterNames( CDataManagerInfo& info );
	bool WriteDbPluFilterItems( CDataManagerInfo& info );
	bool WritePriceText( CDataManagerInfo& info );
	bool WritePromotion( CDataManagerInfo& info );
	bool WriteSBPriceText( CDataManagerInfo& info );
	bool WriteTaxRates( CDataManagerInfo& info );
	bool WriteTimePlan( CDataManagerInfo& info );
	bool WriteTimePlanEntry( CDataManagerInfo& info );
	bool WriteTimePlanTemplateEntry(CDataManagerInfo& info);
	bool WriteWalletStampRule(CDataManagerInfo& info);
	
#ifndef POSTRAY_UTILITY

	bool OpenOfferGroup( int nType, CDataManagerInfo& info );
	bool OpenOfferGroupEntry( int nType, CDataManagerInfo& info );
	bool OpenShelfLabels( int nType, CDataManagerInfo& info );
	bool OpenShelfLabels( int nType, int nLocIdx, int nPriceSet, CDataManagerInfo& info );
	bool OpenLoyaltyScheme(int nType, CDataManagerInfo& info);

	bool ReopenOfferGroup( CDataManagerInfo& info );
	bool ReopenOfferGroupEntry( CDataManagerInfo& info );
	bool ReopenLoyaltyScheme(CDataManagerInfo& info);

	bool CloseOfferGroup( CDataManagerInfo& info );
	bool CloseOfferGroupEntry( CDataManagerInfo& info );
	bool CloseShelfLabels( CDataManagerInfo& info );
	bool CloseLoyaltyScheme(CDataManagerInfo& info);

	bool WriteOfferGroup( CDataManagerInfo& info );
	bool WriteOfferGroupEntry( CDataManagerInfo& info );
	bool WriteShelfLabels( CDataManagerInfo& info );
	bool WriteLoyaltyScheme(CDataManagerInfo& info);
	
#endif

	virtual bool WritePlu( CDataManagerInfo& info ) = 0;
	
public:
	bool LockAllUsers();
	void UnlockAllUsers();
	
protected:
	bool CloseStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState );
	bool WriteStringArray( CSharedStringArray& array, CDataManagerInfo& info, int& nCurrentState );
	
	bool CloseCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState );
	bool WriteCSVArray( CSharedCSVArray& array, CDataManagerInfo& info, int& nCurrentState );

public:
	const char* GetFilePathBarcode( int nDbIdx );
	const char* GetFilePathDepartment( int nDbIdx );
	const char* GetFilePathDepartmentSet( int nDbIdx );
	const char* GetFilePathEposGroup( int nDbIdx );
	const char* GetFilePathReportGroup( int nDbIdx );
	const char* GetFilePathConsolGroup( int nDbIdx );
	const char* GetFilePathHistoricalTax( int nDbIdx );
	const char* GetFilePathMixMatch( int nDbIdx );
	const char* GetFilePathModifier( int nDbIdx );
	const char* GetFilePathOfferGroup( int nDbIdx );
	const char* GetFilePathOfferGroupEntry( int nDbIdx );
	const char* GetFilePathPaidInText( int nDbIdx );
	const char* GetFilePathPaidOutText( int nDbIdx );
	const char* GetFilePathPayment( int nDbIdx );
	const char* GetFilePathPaymentGroup( int nDbIdx );
	const char* GetFilePathSptBookPaymentMap( int nDbIdx );
	const char* GetFilePathSmartEntPaymentMap( int nDbIdx );
	const char* GetFilePathAnalysisCategory( int nDbIdx );
	const char* GetFilePathDbPluFilterNames( int nDbIdx );
	const char* GetFilePathDbPluFilterItems( int nDbIdx );
	const char* GetFilePathPlu( int nDbIdx );
	const char* GetFilePathPriceText( int nDbIdx );
	const char* GetFilePathPromotion( int nDbIdx );
	const char* GetFilePathSBPriceText( int nDbIdx );
	const char* GetFilePathTaxRates( int nDbIdx );
	const char* GetFilePathTimePlan( int nDbIdx );
	const char* GetFilePathTimePlanEntry( int nDbIdx );
	const char* GetFilePathTimePlanTemplateEntry(int nDbIdx);
	const char* GetFilePathWalletStampRule(int nDbIdx);
	const char* GetFilePathLoyaltyScheme(int nDbIdx);
	
	const char* GetFilePathShelfLabels( int nDbIdx );
	const char* GetFilePathShelfLabels( int nLocIdx, int nPriceSet );
	
public:
	CBarcodeCSVArray Barcode;
	CDepartmentCSVArray Department;
	CDepartmentSetCSVArray DepartmentSet;
	CGroupCSVArrayEpos EposGroup;
	CGroupCSVArrayReport ReportGroup;
	CGroupCSVArrayConsol ConsolGroup;
	CHistoricalTaxRates HistoricalTaxRates;
	CMixMatchCSVArray MixMatch;
	CModifierCSVArray Modifier;
	CPaidInOutTextCSVArray PaidInText;
	CPaidInOutTextCSVArray PaidOutText;
	CPaymentCSVArray Payment;
	CGroupCSVArrayPayment PaymentGroup;
	CSmartEntertainmentPaymentMapCSVArray SEPaymentMap;
	CSportsBookerPaymentMapCSVArray SBPaymentMap;
	CAnalysisCategoryCSVArray AnalysisCategory;
	COfferGroupsCSVArray DbPluFilterNames;
	COfferGroupEntriesCSVArray DbPluFilterItems;
	CPriceTextCSVArray PriceText;
	CPromotionsCSVArray Promotion;
	CSportsBookerPriceBandCSVArray SBPriceText;
	CTaxArray TaxRates;
	CTimePlansCSVArray TimePlan;
	CTimePlanEntriesCSVArray TimePlanEntry;
	CTimePlanEntriesCSVArray TimePlanTemplateEntry;
	CWalletStampRuleCSVArray WalletStampRule;

#ifndef POSTRAY_UTILITY
	CListKeyImageWrapperDept ListKeyImageWrapperDept;
	CListKeyImageWrapperPlu ListKeyImageWrapperPlu;
	CPluInfoResourceWrapper ResourceWrapperPlu;
	COfferGroupsCSVArray OfferGroup;
	COfferGroupEntriesCSVArray OfferGroupEntry;
	CLabelListCSVArray ShelfLabels;
	CLoyaltySchemeCSVArray LoyaltyScheme;
#endif

protected:
	int m_nStatusBarcode;
	int m_nStatusDepartment;
	int m_nStatusDepartmentSet;
	int m_nStatusEposGroup;
	int m_nStatusReportGroup;
	int m_nStatusConsolGroup;
	int m_nStatusHistoricalTax;
	int m_nStatusMixMatch;
	int m_nStatusModifier;
	int m_nStatusOfferGroup;
	int m_nStatusOfferGroupEntry;
	int m_nStatusPaidInText;
	int m_nStatusPaidOutText;
	int m_nStatusPayment;
	int m_nStatusPaymentGroup;
	int m_nStatusSBPaymentMap;
	int m_nStatusSEPaymentMap;
	int m_nStatusAnalysisCategory;
	int m_nStatusDbPluFilterNames;
	int m_nStatusDbPluFilterItems;
	int m_nStatusPlu;
	int m_nStatusPriceText;
	int m_nStatusPromotion;
	int m_nStatusSBPriceText;
	int m_nStatusTaxRates;
	int m_nStatusTimePlan;
	int m_nStatusTimePlanEntry;
	int m_nStatusTimePlanTemplateEntry;
	int m_nStatusWalletStampRule;
	int m_nStatusLoyaltyScheme;
	
	int m_nStatusShelfLabels;
	
protected:
	int m_nActiveDbIdx;
	int m_nUserLockCount;
	bool m_bPluPriceSetMode;
	
protected:
	CString m_strFilePathData;
};

/**********************************************************************/
#if defined (STOCKMAN_SYSTEM)
/**********************************************************************/

class CDataManagerStockman : public CDataManager
{
public:
	CDataManagerStockman();
	
	void OpenDatabaseQuickNoFiles( int nDbIdx, bool bKeepIfAlreadyOpen );
	bool OpenDatabaseReadOnly( int nDbIdx, CDataManagerInfo& info, bool bOpenStockLevels = TRUE, bool bOpenTransferDetail = FALSE );
	bool OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CDataManagerInfo& info, bool bOpenStockLevels = TRUE, bool bOpenTransferDetail = FALSE );
	bool OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, CDataManagerInfo& info, bool bOpenStockLevels = TRUE, bool bOpenTransferDetail = FALSE );
	
protected:
	bool OpenSpecialReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, int nDefaultType, CDataManagerInfo& info );
	bool OpenSpecialReadOnly( bool bSameDb, CDataManagerInfo& info );
	void CloseSpecial();

public:
	bool OpenPlu( int nType, CDataManagerInfo& info );
	bool OpenAllowance( int nType, CDataManagerInfo& info );
	bool OpenCategory( int nType, CDataManagerInfo& info );
	bool OpenCategorySet( int nType, CDataManagerInfo& info );
	bool OpenCustomFieldValue( int nFieldNo, int nType, CDataManagerInfo& info );
	bool OpenCustomFieldSet( int nFieldNo, int nType, CDataManagerInfo& info );
	bool OpenDelUnit( int nType, CDataManagerInfo& info );
	bool OpenOrder( int nType, CDataManagerInfo& info );
	bool OpenOrderSp( int nSpIdx, int nType, CDataManagerInfo& info );
	bool OpenProductLabels( int nType, CDataManagerInfo& info );
	bool OpenStockOnly( int nType, CDataManagerInfo& info );
	bool OpenStock( int nType, CDataManagerInfo& info );
	bool OpenSupplier( int nType, CDataManagerInfo& info );
	bool OpenSupplierSet( int nType, CDataManagerInfo& info );
	bool OpenStockLevels( int nSpIdx, int nType, CDataManagerInfo& info );
	bool OpenTransferDetail( int nSpIdx, int nType, CDataManagerInfo& info );

public:
	bool OpenCategoryBuffer( CDataManagerInfo& info );
	bool OpenCustomFieldValueBuffer( int nFieldNo, CDataManagerInfo& info );
	bool OpenCustomFieldSetBuffer( int nFieldNo, CDataManagerInfo& info );
	bool OpenDelUnitBuffer( CDataManagerInfo& info );
	bool OpenSupplierBuffer( CDataManagerInfo& info );

public:
	bool ReopenPlu( CDataManagerInfo& info );
	
public:
	bool ClosePlu( CDataManagerInfo& info );
	bool CloseAllowance( CDataManagerInfo& info );
	bool CloseCategory( CDataManagerInfo& info );
	bool CloseCategorySet( CDataManagerInfo& info );
	bool CloseCustomFieldValue( int nFieldNo, CDataManagerInfo& info );
	bool CloseCustomFieldSet( int nFieldNo, CDataManagerInfo& info );
	bool CloseDelUnit( CDataManagerInfo& info );
	bool CloseOrder( CDataManagerInfo& info );
	bool CloseProductLabels( CDataManagerInfo& info );
	bool CloseStock( CDataManagerInfo& info );
	bool CloseStockOnly( CDataManagerInfo& info );
	bool CloseSupplier( CDataManagerInfo& info );
	bool CloseSupplierSet( CDataManagerInfo& info );
	bool CloseStockLevels( int nSpIdx, CDataManagerInfo& info );
	bool CloseTransferDetail( int nSpIdx, CDataManagerInfo& info );

	bool ForceCloseSupplier( CDataManagerInfo& info );
	bool ForceCloseSupplierSet( CDataManagerInfo& info );
	
public:
	bool CloseCategoryBuffer( CDataManagerInfo& info );
	bool CloseCustomFieldValueBuffer( int nFieldNo, CDataManagerInfo& info );
	bool CloseCustomFieldSetBuffer( int nFieldNo, CDataManagerInfo& info );
	bool CloseDelUnitBuffer( CDataManagerInfo& info );
	bool CloseSupplierBuffer( CDataManagerInfo& info );
	
public:
	bool WritePlu( CDataManagerInfo& info);
	bool WriteAllowance( CDataManagerInfo& info);
	bool WriteCategory( CDataManagerInfo& info);
	bool WriteCategorySet( CDataManagerInfo& info);
	bool WriteCustomFieldValue( int nFieldNo, CDataManagerInfo& info );
	bool WriteCustomFieldSet( int nFieldNo, CDataManagerInfo& info );
	bool WriteDelUnit( CDataManagerInfo& info);
	bool WriteOrder( CDataManagerInfo& info);
	bool WriteProductLabels( CDataManagerInfo& info);
	bool WriteSupplier( CDataManagerInfo& info);
	bool WriteSupplierSet( CDataManagerInfo& info);
	bool WriteStock( CDataManagerInfo& info, bool bItems, bool bLevels );
	bool WriteStockLevels( int nSpIdx, CDataManagerInfo& info );
	bool WriteTransferDetail( int nSpIdx, CDataManagerInfo& info );

	bool ForceWriteSupplier( CDataManagerInfo& info );
	bool ForceWriteSupplierSet( CDataManagerInfo& info );

	void WriteExportLiveApparentStockToken();

public:
	bool WriteCategoryBuffer( CDataManagerInfo& info);
	bool WriteCustomFieldValueBuffer( int nFieldNo, CDataManagerInfo& info );
	bool WriteCustomFieldSetBuffer( int nFieldNo, CDataManagerInfo& info );
	bool WriteDelUnitBuffer( CDataManagerInfo& info);
	bool WriteSupplierBuffer( CDataManagerInfo& info);
	
public:
	void LockSuppliers()	{ m_bLockSuppliers = TRUE; }
	void LockSupplierSets()	{ m_bLockSupplierSets = TRUE; }
	
public:
	void UnlockSuppliers()	{ m_bLockSuppliers = FALSE; }
	void UnlockSupplierSets(){ m_bLockSupplierSets = FALSE; }
	
public:
	int GetActiveCustomFieldNo(){ return m_nActiveCustomFieldNo; }
	void SetActiveCustomFieldNo( int n );
	CCustomFieldValueCSVArray* GetActiveCustomField();
	CCustomFieldSetCSVArray* GetActiveCustomFieldSet();
	void GetCustomFieldPtrArray( CPtrArray& CustomFieldPtrArray );

public:
	const char* GetFilePathAllowance( int nDbIdx );
	const char* GetFilePathCategory( int nDbIdx );
	const char* GetFilePathCategorySet( int nDbIdx );
	const char* GetFilePathCustomFieldValue( int nDbIdx, int nFieldNo );
	const char* GetFilePathCustomFieldSet( int nDbIdx, int nFieldNo );
	const char* GetFilePathOrder( int nDbIdx );
	const char* GetFilePathProductLabels( int nDbIdx );
	const char* GetFilePathRecipe( int nDbIdx );
	const char* GetFilePathStock( int nDbIdx );
	const char* GetFilePathSupplier();

public:
	void GetSortedCategoryList( CWordArray& arrayCatNo );

public:
	CPluMonitorStockman Plu;
	CDefTextAllowance Allowance;
	CDefTextCategory Category;
	CCategorySetCSVArray CategorySet;
	CCustomFieldValueCSVArray CustomFieldValue[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	CCustomFieldSetCSVArray CustomFieldSet[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	CDefTextDelUnit DelUnit;
	CLabelListCSVArray ProductLabels;

	COrderCSVArray Order;
	CStockMonitor Stock;
	CStockPluArray StockPlu;
	CSupplierCSVArray Supplier;
	CSupplierSetCSVArray SupplierSet;

public:
	CCustomFieldManager CustomFieldManager;

private:
	CDefTextCategory CategoryBuffer;
	CCustomFieldValueCSVArray CustomFieldValueBuffer[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	CCustomFieldSetCSVArray CustomFieldSetBuffer[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	CDefTextDelUnit DelUnitBuffer;
	CSupplierCSVArray SupplierBuffer;
	
private:
	int m_nStatusRecipe;
	int m_nStatusAllowance;
	int m_nStatusCategory;
	int m_nStatusCategorySet;
	int m_nStatusCustomFieldValue[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	int m_nStatusCustomFieldSet[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	int m_nStatusDelUnit;
	int m_nStatusOrder;
	int m_nStatusProductLabels;
	int m_nStatusStock;
	int m_nStatusSupplier;
	int m_nStatusSupplierSet;

private:
	int m_nStatusCategoryBuffer;
	int m_nStatusCustomFieldValueBuffer[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	int m_nStatusCustomFieldSetBuffer[ MAX_ITEM_CUSTOM_FIELDS + 1 ];
	int m_nStatusDelUnitBuffer;
	int m_nStatusSupplierBuffer;

private:
	bool m_bLockSuppliers;
	bool m_bLockSupplierSets;
	
	bool m_bOpenStockLevels;
	bool m_bOpenTransferDetail;
	friend class CStatusProgress;

	int m_nActiveCustomFieldNo;
};

/**********************************************************************/
extern CDataManagerStockman DataManager;
/**********************************************************************/

/**********************************************************************/
#elif defined (POSTRAY_UTILITY)
/**********************************************************************/

class CDataManagerPosTray : public CDataManager
{
public:
	CDataManagerPosTray();
	
	void OpenDatabaseQuickNoFiles( int nDbIdx, bool bKeepIfAlreadyOpen );
	bool OpenDatabaseReadOnly( int nDbIdx, CDataManagerInfo& info, bool bOpenStockLevels = TRUE, bool bOpenTransferDetail = FALSE );
	bool OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CDataManagerInfo& info, bool bOpenStockLevels = TRUE, bool bOpenTransferDetail = FALSE );
	bool OpenDatabaseReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, CDataManagerInfo& info, bool bOpenStockLevels = TRUE, bool bOpenTransferDetail = FALSE );
	
protected:
	bool OpenSpecialReadWrite( int nDbIdx, CWordArray& arrayReadWrite, CWordArray& arrayReadOnly, int nDefaultType, CDataManagerInfo& info );
	bool OpenSpecialReadOnly( bool bSameDb, CDataManagerInfo& info );
	void CloseSpecial();

public:
	bool OpenPlu( int nType, CDataManagerInfo& info );
	bool OpenStock( int nType, CDataManagerInfo& info );
	
public:
	bool ReopenPlu( CDataManagerInfo& info );
	
public:
	bool ClosePlu( CDataManagerInfo& info );
	bool CloseStock( CDataManagerInfo& info );

public:
	bool WritePlu( CDataManagerInfo& info ){ return FALSE; }
			
public:
	const char* GetFilePathRecipe( int nDbIdx );
	const char* GetFilePathStock( int nDbIdx );
	
public:
	CPluMonitorPosTray Plu;
	CStockMonitorPosTray Stock;
		
private:
	int m_nStatusRecipe;
	int m_nStatusStock;
};

/**********************************************************************/
extern CDataManagerPosTray DataManager;
/**********************************************************************/

/**********************************************************************/
#else
/**********************************************************************/

class CDataManagerEcrman : public CDataManager
{
public:
	CDataManagerEcrman();

public:
	bool OpenPlu( int nType, CDataManagerInfo& info );
	bool ReopenPlu( CDataManagerInfo& info );
	bool ClosePlu( CDataManagerInfo& info );
	bool WritePlu( CDataManagerInfo& info );

public:
	CPluMonitorEcrman Plu;

	friend class CStatusProgress;
};

/**********************************************************************/
extern CDataManagerEcrman DataManager;
/**********************************************************************/

/**********************************************************************/
#endif
/**********************************************************************/
