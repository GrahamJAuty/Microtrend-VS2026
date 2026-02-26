#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CascadingReasonCSVArray.h"
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CSQLImporter
{
public:
	CSQLImporter(const char* szFilename);
	void ImportFile(bool bChangeDatabase, int nDbIdx = 0, int nLocIdx = 0);
	void ImportCustomersOnly(bool bAuto);

	bool GotFile() { return m_bGotFile; }
	bool GotData();
	int GetFileTypeCount();

	bool LegacyWarning();

public:
	void SetServerPresetLocIdx(int n) { m_nServerPresetLocIdx = n; }
	void SetWantBarcodeFlag(bool b) { m_bWantBarcode = b; }
	void SetWantDeptFlag(bool b) { m_bWantDept = b; }
	void SetWantCategoryFlag(bool b) { m_bWantCategory = b; }
	void SetWantGroupFlag(bool b) { m_bWantGroup = b; }
	void SetWantAllergenFlag(bool b) { m_bWantAllergen = b; }
	void SetWantMixMatchFlag(bool b) { m_bWantMixMatch = b; }
	void SetWantWalletStampRuleFlag(bool b) { m_bWantWalletStampRule = b; }
	void SetWantTimePlanFlag(bool b) { m_bWantTimePlan = b; }
	void SetWantModifierFlag(bool b) { m_bWantModifier = b; }
	void SetWantPaidInOutTextFlag(bool b) { m_bWantPaidInOutText = b; }
	void SetWantPayMethFlag(bool b) { m_bWantPayMeth = b; }
	void SetWantServerFlag(bool b) { m_bWantServer = b; }
	void SetWantServerLogonFlag(bool b) { m_bWantServerLogon = b; }
	void SetWantServerRestrictFlag(bool b) { m_bWantServerRestrict = b; }
	void SetWantServerPresetFlag(bool b) { m_bWantServerPreset = b; }
	void SetWantPluFlag(bool b) { m_bWantPlu = b; }
	void SetWantTaxRateFlag(bool b) { m_bWantTaxRate = b; }
	void SetWantLoyaltySchemeFlag(bool b) { m_bWantLoyaltyScheme = b; }
	void SetWantTableAreaFlag(bool b) { m_bWantTableArea = b; }
	void SetWantTableNameFlag(bool b) { m_bWantTableName = b; }
	void SetWantFunctionSystemFlag(bool b) { m_bWantFunctionSystem = b; }
	void SetWantFunctionDatabaseType(int n) { m_nWantFunctionDatabaseType = n; }
	void SetWantFunctionLocationType(int n) { m_nWantFunctionLocationType = n; }
	void SetWantCCMacroSystemFlag(bool b) { m_bWantCCMacroSystem = b; }
	void SetWantCCMacroDatabaseFlag(bool b) { m_bWantCCMacroDatabase = b; }
	void SetWantCCMacroLocationFlag(bool b) { m_bWantCCMacroLocation = b; }
	void SetWantCCReasonSystemFlag(bool b) { m_bWantCCReasonSystem = b; }
	void SetWantCCReasonDatabaseFlag(bool b) { m_bWantCCReasonDatabase = b; }
	void SetWantCCReasonLocationFlag(bool b) { m_bWantCCReasonLocation = b; }
	void SetCreateStockFlag(bool b) { m_bCreateStock = b; }
	void SetClearReasonSystemFlag(bool b) { m_bClearReasonSystem = b; }
	void SetClearReasonDatabaseFlag(bool b) { m_bClearReasonDatabase = b; }
	void SetClearReasonLocationFlag(bool b) { m_bClearReasonLocation = b; }

public:
	int GetBarcodeCount() { return m_nTotalBarcode; }
	int GetCustomerCount() { return m_nTotalCustomer; }
	int GetDeptCount() { return m_nTotalDept; }
	int GetCategoryCount() { return m_nTotalCategory; }
	int GetGroupCount() { return m_nTotalGroup; }
	int GetAllergenCount() { return m_nTotalAllergen; }
	int GetMixMatchCount() { return m_nTotalMixMatch; }
	int GetWalletStampRuleCount() { return m_nTotalWalletStampRule; }
	int GetOfferGroupCount() { return m_nTotalOfferGroup; }
	int GetOfferEntryCount() { return m_nTotalOfferEntry; }
	int GetPaidInOutTextCount() { return m_nTotalPaidInOutText; }
	int GetTimePlanCount() { return m_nTotalTimePlan; }
	int GetTimePlanEntryCount() { return m_nTotalTimePlanEntry; }
	int GetPromotionCount() { return m_nTotalPromotion; }
	int GetModifierCount() { return m_nTotalModifier; }
	int GetPayMethCount() { return m_nTotalPayMeth; }
	int GetServerCount() { return m_nTotalServer; }
	int GetServerLogonCount() { return m_nTotalServerLogon; }
	int GetServerRestrictCount() { return m_nTotalServerRestrict; }
	int GetServerPresetCount() { return m_nTotalServerPreset; }
	int GetBasePluCount() { return m_nTotalBasePlu; }
	int GetModifierPluCount() { return m_nTotalModifierPlu; }
	int GetTotalPluCount() { return m_nTotalBasePlu + m_nTotalModifierPlu; }
	int GetTaxRateCount() { return m_nTotalTaxRate; }
	int GetLoyaltySchemeCount() { return m_nTotalLoyaltyScheme; }
	int GetTableAreaCount() { return m_nTotalTableArea; }
	int GetTableNameCount() { return m_nTotalTableName; }
	int GetFunctionCount() { return m_nTotalFunction; }
	int GetCCMacroCount() { return m_nTotalCCMacro; }
	int GetCCReasonCount() { return m_nTotalCCReason; }

	int GetCustomersAdded() { return m_nCustomersAdded; }
	int GetCustomersModified() { return m_nCustomersModified; }

	bool GetLegacyPaymentsFlag() { return m_bGotLegacyPayments; }
	bool GetLegacyServersFlag() { return m_bGotLegacyServers; }

	void GetOfferGroupList(CWordArray& array);

public:
	void Reset();

private:
	void ImportProducts(CSSFile& fileSQL, int nPassNo);
	void ImportDepartments(CSSFile& fileSQL, int nPassNo);
	void ImportCategories(CSSFile& fileSQL, int nPassNo);
	void ImportGroups(CSSFile& fileSQL, int nPassNo);
	void ImportAllergens(CSSFile& fileSQL, int nPassNo);
	void ImportMixMatch(CSSFile& fileSQL, int nPassNo);
	void ImportWalletStampRules(CSSFile& fileSQL, int nPassNo);
	void ImportOfferGroup(CSSFile& fileSQL, int nPassNo);
	void ImportOfferEntry(CSSFile& fileSQL, int nPassNo);
	void ImportPaidInOutTexts(CSSFile& fileSQL, int nPassNo);
	void ImportTimePlan(CSSFile& fileSQL, int nPassNo);
	void ImportTimePlanEntry(CSSFile& fileSQL, int nPassNo);
	void ImportPromotion(CSSFile& fileSQL, int nPassNo);
	void ImportModifiers(CSSFile& fileSQL, int nPassNo);
	void ImportPaymentMethods(CSSFile& fileSQL, int nPassNo);
	void ImportServers(CSSFile& fileSQL, int nPassNo);
	void ImportServerLogons(CSSFile& fileSQL, int nPassNo);
	void ImportServerRestricts(CSSFile& fileSQL, int nPassNo);
	void ImportServerPresets(CSSFile& fileSQL, int nPassNo);
	void ImportBarcodes(CSSFile& fileSQL, int nPassNo);
	void ImportTaxRates(CSSFile& fileSQL, int nPassNo);
	void ImportLoyaltySchemes(CSSFile& fileSQL, int nPassNo);
	void ImportTableAreas(CSSFile& fileSQL, int nPassNo);
	void ImportTableNames(CSSFile& fileSQL, int nPassNo);
	void ImportFunctions(CSSFile& fileSQL, int nPassNo);
	void ImportCCMacros(CSSFile& fileSQL, int nPassNo);
	void ImportCCReasons(CSSFile& fileSQL, int nPassNo, int nDbIdx, int nLocIdx );
	void ImportCustomers(CSSFile& fileSQL, bool bAuto, const char* szTableName);

private:
	void ImportProduct(CString& strBuffer);
	void ImportDepartment(CString& strBuffer);
	void ImportCategory(CString& strBuffer);
	void ImportGroup(CString& strBuffer);
	void ImportAllergen(CString& strBuffer);
	void ImportMixMatch(CString& strBuffer);
	void ImportWalletStampRule(CString& strBuffer);
	void ImportOfferGroup(CString& strBuffer);
	void ImportOfferEntry(CString& strBuffer);
	void ImportPaidInOutText(CString& strBuffer);
	void ImportTimePlan(CString& strBuffer);
	void ImportTimePlanEntry(CString& strBuffer);
	void ImportModifier(CString& strBuffer);
	void ImportPaymentMethod(CString& strBuffer);
	void ImportPromotion(CString& strBuffer);
	void ImportServer(CString& strBuffer);
	void ImportServerLogon(CString& strBuffer);
	void ImportServerRestrict(CString& strBuffer);
	void ImportServerPreset(int& nCurrentTNo, CString& strBuffer);
	void ImportBarcode(CString& strBuffer);
	void ImportTaxRates(CString& strBuffer);
	void ImportLoyaltyScheme(CString& strBuffer);
	void ImportTableArea(CString& strBuffer);
	void ImportTableName(CString& strBuffer);
	void ImportFunction(CString& strBuffer);
	void ImportCCMacro(CString& strBuffer);
	void ImportCCReason(CString& strBuffer, int nDbIdx, int nLocIdx );
	void ImportCustomer(CString& strBuffer, bool bAuto);

private:
	void ProcessMacro(CCascadingMacroCSVArray& MacroArray, CCSV& csvIn);
	void ProcessReason(CCascadingReasonCSVArray& ReasonArray, CCSV& csvIn, bool bCanClear, bool bAlreadyCleared);

private:
	void AddToFileTypeCount( int& nCount, bool bCheck, int nAdd = 1 );

private:
	bool m_bGotFile;
	int m_nTotalBarcode;
	int m_nTotalCustomer;
	int m_nTotalDept;
	int m_nTotalCategory;
	int m_nTotalGroup;
	int m_nTotalAllergen;
	int m_nTotalMixMatch;
	int m_nTotalWalletStampRule;
	int m_nTotalOfferGroup;
	int m_nTotalOfferEntry;
	int m_nTotalPaidInOutText;
	int m_nTotalTimePlan;
	int m_nTotalTimePlanEntry;
	int m_nTotalModifier;
	int m_nTotalPayMeth;
	int m_nTotalPromotion;
	int m_nTotalServer;
	int m_nTotalServerLogon;
	int m_nTotalServerRestrict;
	int m_nTotalServerPreset;
	int m_nTotalBasePlu;
	int m_nTotalModifierPlu;
	int m_nTotalTaxRate;
	int m_nTotalLoyaltyScheme;
	int m_nTotalTableArea;
	int m_nTotalTableName;
	int m_nTotalFunction;
	int m_nTotalCCMacro;
	int m_nTotalCCReason;

private:
	int m_nCustomersAdded;
	int m_nCustomersModified;

private:
	int m_nServerPresetLocIdx;
	bool m_bChangeDatabase;
	bool m_bWantBarcode;
	bool m_bWantDept;
	bool m_bWantCategory;
	bool m_bWantGroup;
	bool m_bWantAllergen;
	bool m_bWantMixMatch;
	bool m_bWantWalletStampRule;
	bool m_bWantTimePlan;
	bool m_bWantModifier;
	bool m_bWantPaidInOutText;
	bool m_bWantPayMeth;
	bool m_bWantServer;
	bool m_bWantServerLogon;
	bool m_bWantServerRestrict;
	bool m_bWantServerPreset;
	bool m_bWantPlu;
	bool m_bCreateStock;
	bool m_bWantTaxRate;
	bool m_bWantLoyaltyScheme;
	bool m_bWantTableArea;
	bool m_bWantTableName;
	bool m_bWantFunctionSystem;
	int m_nWantFunctionDatabaseType;
	int m_nWantFunctionLocationType;
	bool m_bWantCCMacroSystem;
	bool m_bWantCCMacroDatabase;
	bool m_bWantCCMacroLocation;
	bool m_bWantCCReasonSystem;
	bool m_bWantCCReasonDatabase;
	bool m_bWantCCReasonLocation;

	bool m_bGotLegacyPayments;
	bool m_bGotLegacyServers;

	bool m_bClearReasonSystem;
	bool m_bClearReasonDatabase;
	bool m_bClearReasonLocation;
	bool m_bHaveClearedReasonSystem[REASONTEXT_TYPES];
	bool m_bHaveClearedReasonDatabase[REASONTEXT_TYPES];
	bool m_bHaveClearedReasonLocation[REASONTEXT_TYPES];

private:
	CString m_strFilename;

	CReportConsolidationArray<CSortedStringItem> m_arrayOfferGroups;
	CReportConsolidationArray<CSortedStringItem> m_arrayTimePlans;
};

/**********************************************************************/
#endif
/**********************************************************************/
