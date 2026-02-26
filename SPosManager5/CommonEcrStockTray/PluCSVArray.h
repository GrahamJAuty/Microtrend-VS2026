#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "MinimumSPOSVersion.h"
#include "NamespacePlu.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
/*	PLU DATABASE											*/
/**********************************************************************/
#define	PLUDB_VERSION_NO 43		// plu database version number
/**********************************************************************/
#define	PLU_GPTYPE_VARIABLE 0
#define	PLU_GPTYPE_FIXED 1
#define	PLU_GPTYPE_PERCENT 2
/**********************************************************************/
#define	MAX_PLU_PRICES 10
#define MAX_PLU_ALLERGENS 6
#define MAX_LINK_PLU_ITEMS 30
#define MAX_RECIPE_PLU_ITEMS 20
/**********************************************************************/
#define	PLU_MODIFIERS 9
/**********************************************************************/
#define	PLU_ORDER_PRINTERS_PRE_V39 9
#define	PLU_ORDER_PRINTERS_NOW 16
/**********************************************************************/
#define NO_CHANGE 0
#define EPOS_CHANGE 1
#define SUPER_CHANGE 2
/**********************************************************************/

struct CEcrmanStockInfo
{
public:
	__int64 m_nPluNo;
	double m_dStockQty;
};

/**********************************************************************/

struct CEcrmanRecipeItem
{
public:
	CEcrmanRecipeItem();
	void Reset();
	
public:
	int Compare ( CEcrmanRecipeItem& source, int nHint = 0 );
	void Add	( CEcrmanRecipeItem& source );
	
public:
	__int64 m_nPluNo;
	double m_dStockQty;
};

/**********************************************************************/

class CPluCSVRecord : public CCSVRecord
{
public:
	CPluCSVRecord();
	virtual ~CPluCSVRecord() {}
	virtual void ConvertToCSV(CCSV& csv);
	virtual void ConvertFromCSV(CCSV& csv);

	void CreateExportLine(CCSV& csv);

	void ImportLine(CSSImportFile& import, CCSV& csv, bool bWithStock = FALSE );
	int GetChange(CPluCSVRecord& plu);		// 0=no change, 1=epos change, 2=non epos change

	//DESCRIPTION AND TYPE
	void SetPluNo(__int64 nPluNo);
	void SetEposText(const char* szText);
	void SetKeyText(const char* szText);
	void SetRepText(const char* szRepText);
	void SetDefaultText();
	void SetSortCode(const char* szSortCode);
	void SetAgressoGLCode(int n);
	void SetAgressoCostCentre(const char* sz);
	void SetAgressoProject(const char* sz);
	void SetBaseDeptNo(int nDeptNo);
	void SetBaseAnalysisCategory(int nCategory);
	void SetMicrotrendPluType(int nType);
	void SetExternalProcess(int nProcess);
	void SetBaseTaxBandString(const char* szTaxBand);
	void SetBaseTaxBandInt(int nTaxBand);
	//PRICES AND POINTS
	void SetPrice(int n, double dPrice);
	void SetPoints(int nPoints);
	void SetStampWeight(int nSW);
	//COST AND PROFIT
	void SetPurchaseCost(double dCost);
	void SetReportCostEnable(bool b);
	void SetReportCost(double dCost);
	void SetGPType(int nGPType);							// 0=variable, 1=fixed, 2=percent
	void SetProfitMargin(double dMargin);
	//ALLERGENS
	void SetAllergenNo(int n, int nAllergenNo);
	//KEYBOARD LISTS
	void SetFirstMenu(int nMenu);
	void SetFirstQualifier(int nQualifier);
	void SetFirstExtra(int nExtra);
	//ORDER PRINTERS
	void SetOrderPrinterGroup(int n);
	void SetOrderPrinterFlag(int nNo, bool bFlag);
	void SetPriority(int nPriority);
	//MODIFIERS
	void SetAllowModifierFlag(bool b);
	void SetModifierEnable(int nMod, bool bFlag);
	void SetModifierEposText(int nMod, bool bFlag, const char* szText);
	void SetModifierKeyText(int nMod, bool bFlag, const char* szText);
	void SetModifierDeptNoRaw(int nMod, int nDeptNo);
	void SetModifierAnalysisCategoryRaw(int nMod, int nCat);
	void SetModifierMicrotrendPluType(int nMod, int nType);
	void SetModifierTaxBandStringRaw(int nMod, const char* szTaxBand);
	void SetModifierTaxBandIntRaw(int nMod, int nTaxBand);
	void SetModifierPrice(int nMod, int nLevel, double dPrice);
	void SetModifierFirstMenu(int nMod, int nMenu);
	void SetModifierFirstQualifier(int nMod, int nQualifier);
	void SetModifierFirstExtra(int nMod, int nExtra);
	void SetModifierOrderPrinterGroup(int nMod, int n);
	//MISC PROGRAMMING
	void SetPriceType(int nType);
	void SetAgeVerificationType(int n);
	void SetKPNotAloneFlag(bool b);
	void SetKPConsolidateTopItemFlag(bool b);
	void SetKPImagePrintFlag(bool b);
	void SetKPImageModifierFlag(bool b);
	void SetAutoCoversQtyFlag(bool b);
	void SetEANOnReceiptFlag(bool b);
	void SetSalesTrackingFlag(bool b);
	void SetSoldOutFlag(bool b);
	void SetDiscountableFlag(bool b);
	void SetStaffDiscountFlag(bool b);
	void SetDiscountFlagsFromType(int nType);
	void SetFreeTextFlag(bool bFreeText);
	void SetFreeTextDescriptionFlag(bool b);
	void SetAccountRequiredFlag(bool b);
	//LABELS
	void SetPrintShelfFlag(bool b);
	void SetPrintProductFlag(bool b);
	void SetUnitPriceText(const char* szText);
	void SetUnitPriceSize(double dSize);
	//ECRMAN STOCK
	void SetApparentLevel(double dLevel);
	void SetMinimumLevel(double dLevel);
	void SetSupplierRef(const char* szRef);
	void SetSupplierNo(int nSuppNo);
	void SetNonStockInternalFlag(bool b);
	//MISC
	void SetPluInfoModifierFlag(bool b);
	void SetLinkType(int n);
	void SetLinkSellTiedItemFlag(bool b);
	void SetLinkTableString(const char* sz);
	void SetEcrmanRecipeTableString(const char* sz);
	void SetImageFilename(const char* sz);
	void SetActiveItemFlag(bool b);
	//INTERNAL
	void SetModifiedDateTimePlu();
	void SetModifiedDateTimePlu(const char* szDateTime);	// yyyymmddhhmmss
	void SetModifiedDateTimeInfo();
	void SetModifiedDateTimeInfo(const char* szDateTime);	// yyyymmddhhmmss
	void SetDownloadDatePlu();
	void SetDownloadDatePlu(const char* szDate);	// yyyymmdd
	void SetExport(bool b);

	//DESCRIPTION AND TYPE
	__int64 GetPluNoInt() { return m_nPluNo; }
	const char* GetPluNoString();
	const char* GetEposText() { return m_strEposText; }
	const char* GetKeyText() { return m_strKeyText; }
	const char* GetRepText() { return m_strRepText; }
	const char* GetReportText();
	const char* GetSortCode() { return m_strSortCode; }
	int GetAgressoGLCode() { return m_nAgressoGLCode; }
	const char* GetAgressoCostCentre() { return m_strAgressoCostCentre; }
	const char* GetAgressoProject() { return m_strAgressoProject; }
	int GetBaseDeptNo() { return m_nBaseDeptNo; }
	int GetBaseAnalysisCategory() { return m_nBaseAnalysisCategory; }
	int GetMicrotrendPluType() { return m_nMicrotrendPluType; }
	int GetExternalProcess() { return m_nExternalProcess; }
	const char* GetBaseTaxBandString() { return m_strBaseTaxBand; }
	int GetBaseTaxBandInt();
	//PRICES AND POINTS
	double GetPrice(int n);
	int GetPoints() { return m_nPoints; }
	int GetStampWeight(){ return m_nStampWeight; }
	//COST AND PROFIT
	double GetPurchaseCost() { return m_dPurchaseCost; }
	bool GetReportCostEnable() { return m_bReportCostEnable; }
	double GetReportCost() { return m_dReportCost; }
	int GetGPType() { return m_nGPType; }
	double GetProfitMargin() { return m_dProfitMargin; }
	//ALLERGENS
	int GetAllergenNo(int n);
	//KEYBOARD LISTS
	int GetFirstMenu() { return m_nFirstMenu; }
	int GetFirstQualifier() { return m_nFirstQualifier; }
	int GetFirstExtra() { return m_nFirstExtra; }
	//ORDER PRINTERS
	int GetOrderPrinterGroup() { return m_nOrderPrinterGroup; }
	bool GetOrderPrinterFlag(int nNo);
	int GetPriority() { return m_nPriority; }
	//MODIFIERS
	bool GetModifierEnable(int nMod);
	bool GetModifierEposTextFlag(int nMod);
	const char* GetModifierEposText(int nMod);
	bool GetModifierKeyTextFlag(int nMod);
	const char* GetModifierKeyText(int nMod);
	const char* GetModifierReportText(int nMod);
	int GetModifierDeptNoRaw(int nMod);
	int GetModifierDeptNoForReport(int nMod);
	int GetModifierAnalysisCategoryRaw(int nMod);
	int GetModifierAnalysisCategoryForReport(int nMod);
	int GetModifierMicrotrendPluType(int nMod);
	const char* GetModifierTaxBandStringRaw(int nMod);
	int GetModifierTaxBandIntRaw(int nMod);
	int GetModifierTaxBandIntForReport(int nMod);
	CString GetModifierTaxBandStringForReport(int nMod);
	double GetModifierPrice(int nMod, int nLevel);
	int GetModifierFirstMenu(int nMod);
	int GetModifierFirstQualifier(int nMod);
	int GetModifierFirstExtra(int nMod);
	int GetModifierOrderPrinterGroup(int nMod);
	//MISC PROGRAMMING	
	int GetPriceType() { return m_nPriceType; }
	int GetAgeVerificationType() { return m_nAgeVerificationType; }
	bool GetKPNotAloneFlag() { return m_bKPNotAlone; }
	bool GetKPConsolidateTopItemFlag() { return m_bKPConsolidateTopItem; }
	bool GetKPImagePrintFlag() { return m_bKPImagePrint; }
	bool GetKPImageModifierFlag() { return m_bKPImageModifier; }
	bool GetAutoCoversQtyFlag() { return m_bAutoCoversQty; }
	bool GetEANOnReceiptFlag() { return m_bEANOnReceipt; }
	bool GetSalesTrackingFlag() { return m_bSalesTracking; }
	bool GetSoldOutFlag() { return m_bSoldOut; }
	bool GetDiscountableFlag() { return m_bDiscountable; }
	bool GetStaffDiscountFlag() { return m_bStaffDiscount; }
	bool GetFreeTextFlag() { return m_bFreeText; }
	bool GetFreeTextDescriptionFlag() { return m_bFreeTextDescription; }
	bool GetAllowModifierFlag() { return m_bAllowModifier; }
	bool GetAccountRequiredFlag() { return m_bAccountRequired; }
	//LABELS
	bool GetPrintShelfFlag() { return m_bPrintShelf; }
	bool GetPrintProductFlag() { return m_bPrintProduct; }
	const char* GetUnitPriceText() { return m_strUnitPriceText; }
	double GetUnitPriceSize() { return m_dUnitPriceSize; }
	//ECRMAN STOCK
	double GetApparentLevel() { return m_dApparentLevel; }
	double GetMinimumLevel() { return m_dMinimumLevel; }
	const char* GetSupplierRef() { return m_strSupplierRef; }
	int GetSupplierNo() { return m_nSupplierNo; }
	bool GetNonStockInternalFlag() { return m_bNonStock; }
	bool GetNonStockExternalFlag();
	//MISC
	bool GetPluInfoModifierFlag() { return m_bPluInfoModifier; }
	int GetLinkType() { return m_nLinkType; }
	bool GetLinkSellTiedItemFlag() { return m_bLinkSellTiedItem; }
	const char* GetLinkTableString() { return m_strLinkTable; }
	const char* GetEcrmanRecipeTableString() { return m_strEcrmanRecipeTable; }
	const char* GetImageFilename() { return m_strImageFilename; }
	bool GetActiveItemFlag() { return m_bActive; }
	//INTERNAL FIELDS	
	const char* GetModifiedDateTimePlu() { return m_strModifiedDateTimePlu; }	// yyyymmddhhmmss
	const char* GetModifiedDateTimeInfo() { return m_strModifiedDateTimeInfo; }	// yyyymmddhhmmss
	const char* GetDownloadDatePlu() { return m_strDownloadDatePlu; }	// yyyymmdd
	bool GetExport() { return m_bExport; }

	//HELPER FUNCTIONS
	const char* GetPluTypeText(int nMod = 0);
	const char* GetPriceTypeText();
	const char* GetAgeVerificationTypeText();
	const char* GetCategoryText(int nMod = 0);
	const char* GetExternalProcessText();

	void GetLinkTableArray(CArray<__int64, __int64>& arrayPluNo);
	void SetLinkTableArray(CArray<__int64, __int64>& arrayPluNo);

	bool GotEcrmanRecipeTableArray();
	void GetEcrmanRecipeTableArray(CReportConsolidationArray<CEcrmanRecipeItem>& arrayPluNo);
	void SetEcrmanRecipeTableArray(CReportConsolidationArray<CEcrmanRecipeItem>& arrayPluNo);

#ifndef POSTRAY_UTILITY

public:
	void CheckMinimumSPOSVersion(CMinimumSPOSVersion& version, __int64& nSharpPluNo, bool& bModifiable);
	const char* GetAllergenText(int n);

#endif

private:
	void ClearRecord();
	void V1ConvertFromCSV(CCSV& csv);
	void V2ConvertFromCSV(CCSV& csv);
	void V3ConvertFromCSV(CCSV& csv);
	void V4To10ConvertFromCSV(CCSV& csv, int nVer);
	void V11To13ConvertFromCSV(CCSV& csv, int nVer);
	void V14To26ConvertFromCSV(CCSV& csv, int nVer);
	void X500V27To32ConvertFromCSV(CCSV& csv, int nVer);
	void SPOSV30To36ConvertFromCSV(CCSV& csv, int nVer);
	void SPOSV37To38ConvertFromCSV(CCSV& csv, int nVer);
	void SPOSV39To43ConvertFromCSV(CCSV& csv, int nVer);

private:
	void ImportString(CCSV& csv, CSSImportFile& import, int& n, CString& str, const recString& rec, bool bFilter = FALSE);
	void ImportDouble(CCSV& csv, CSSImportFile& import, int& n, double& d, const recDouble& rec);
	void ImportInt(CCSV& csv, CSSImportFile& import, int& n, int& i, const recInt& rec);
	void ImportInt64(CCSV& csv, CSSImportFile& import, int& n, __int64& i, const recInt64& rec);
	void ImportBool(CCSV& csv, CSSImportFile& import, int& n, bool& b);

private:
	void CreateCSVLine(CCSV& csv, bool bForceFullLine);

public:
	static void GetPriceLabel(int nPrice, CString& strLabel);
	static void GetAllergenLabel(int nNo, CString& strLabel);
	static void GetOrderPrinterLabel(int nNo, CString& strLabel);
	static void GetModifierLabelEnable(int nMod, bool bLegacy, CString& strLabel);
	static void GetModifierLabelEposTextFlag(int nMod, bool bLegacy, CString& strLabel);
	static void GetModifierLabelEposText(int nMod, bool bLegacy, CString& strLabel);
	static void GetModifierLabelKeyTextFlag(int nMod, bool bLegacy, CString& strLabel);
	static void GetModifierLabelKeyText(int nMod, bool bLegacy, CString& strLabel);
	static void GetModifierLabelDeptNo(int nMod, CString& strLabel);
	static void GetModifierLabelAnalysisCategory(int nMod, CString& strLabel);
	static void GetModifierLabelMicrotrendPluType(int nMod, CString& strLabel);
	static void GetModifierLabelTaxBand(int nMod, bool bLegacy, CString& strLabel);
	static void GetModifierLabelPrice(int nMod, int nPrice, bool bLegacy, CString& strLabel);
	static void GetModifierLabelFirstMenu(int nMod, CString& strLabel);
	static void GetModifierLabelFirstQualifier(int nMod, CString& strLabel);
	static void GetModifierLabelFirstExtra(int nMod, CString& strLabel);
	static void GetModifierLabelOrderPrinterGroup(int nMod, CString& strLabel);
	static void GetStockpointPseudoLabel(CString& strLabel) { strLabel = "Stockpoint"; }

private:
	//DESCRIPTION AND TYPE
	__int64 m_nPluNo;
	CString m_strPluNoString;
	CString m_strEposText;
	CString m_strKeyText;
	CString m_strRepText;
	CString m_strReportText;
	CString m_strSortCode;
	int m_nAgressoGLCode;
	CString m_strAgressoCostCentre;
	CString m_strAgressoProject;
	int m_nBaseDeptNo;
	int m_nBaseAnalysisCategory;
	int m_nMicrotrendPluType;
	int m_nExternalProcess;
	CString m_strBaseTaxBand;
	//PRICES AND POINTS
	double m_dPrice[MAX_PLU_PRICES];
	int m_nPoints;
	int m_nStampWeight;
	//COST AND PROFIT
	double m_dPurchaseCost;
	bool m_bReportCostEnable;
	double m_dReportCost;
	int m_nGPType;
	double m_dProfitMargin;
	//ALLERGENS
	int m_nAllergenNo[MAX_PLU_ALLERGENS];
	//KEYBOARD LISTS
	int m_nFirstMenu;
	int m_nFirstQualifier;
	int m_nFirstExtra;
	//ORDER PRINTERS
	int m_nOrderPrinterGroup;
	bool m_bOrderPrinterFlag[PLU_ORDER_PRINTERS_NOW];
	int m_nPriority;
	//MODIFIERS
	bool m_bAllowModifier;
	bool m_bModifierEnable[PLU_MODIFIERS];
	bool m_bModifierEposText[PLU_MODIFIERS];
	CString m_strModifierEposText[PLU_MODIFIERS];
	bool m_bModifierKeyText[PLU_MODIFIERS];
	CString m_strModifierKeyText[PLU_MODIFIERS];
	int m_nModifierDeptNo[PLU_MODIFIERS];
	int m_nModifierAnalysisCategory[PLU_MODIFIERS];
	int m_nModifierMicrotrendPluType[PLU_MODIFIERS];
	CString m_strModifierTaxBand[PLU_MODIFIERS];
	double m_dModifierPrice[PLU_MODIFIERS][MAX_PLU_PRICES];
	int m_nModifierFirstMenu[PLU_MODIFIERS];
	int m_nModifierFirstQualifier[PLU_MODIFIERS];
	int m_nModifierFirstExtra[PLU_MODIFIERS];
	int m_nModifierOrderPrinterGroup[PLU_MODIFIERS];
	//MISC PROGRAMMING
	int m_nPriceType;
	int m_nAgeVerificationType;
	bool m_bKPNotAlone;
	bool m_bKPConsolidateTopItem;
	bool m_bKPImagePrint;
	bool m_bKPImageModifier;
	bool m_bAutoCoversQty;
	bool m_bEANOnReceipt;
	bool m_bSalesTracking;
	bool m_bSoldOut;
	bool m_bDiscountable;
	bool m_bStaffDiscount;
	bool m_bFreeText;
	bool m_bFreeTextDescription;
	bool m_bAccountRequired;
	//LABELS
	bool m_bPrintShelf;
	bool m_bPrintProduct;
	CString m_strUnitPriceText;
	double m_dUnitPriceSize;
	//ECRMAN STOCK
	double m_dApparentLevel;
	double m_dMinimumLevel;
	CString m_strSupplierRef;
	int m_nSupplierNo;
	bool m_bNonStock;
	//MISC
	bool m_bPluInfoModifier;
	int m_nLinkType;
	bool m_bLinkSellTiedItem;
	CString m_strLinkTable;
	CString m_strEcrmanRecipeTable;
	CString m_strImageFilename;
	bool m_bActive;
	//INTERNAL FIELDS	
	CString m_strModifiedDateTimePlu;
	CString m_strModifiedDateTimeInfo;
	CString m_strDownloadDatePlu;
	bool m_bExport;

private:
	CString m_strPluTypeText;
	CString m_strPriceTypeText;
	CString m_strCategoryText;
	CString m_strDiscountTypeText;
	CString m_strAgeVerificationTypeText;
	CString m_strExternalProcessText;
	CString m_strAllergenText;
};

/**********************************************************************/

struct CEODPluImportInfo
{
public:
	CEODPluImportInfo();

public:
	CString m_strImportLine;
	bool m_bImportIfExists;
	int m_nExistingMask;
	int m_nFieldMask;
	bool m_bImportIfNew;
	int m_nImportModifierMode;
	int m_nResult;
	__int64 m_nLastBasePluNo;
	int m_nNewPluDeptNo;
	CString m_strNewPluText;
};

/**********************************************************************/

class CPluCSVArray : public CSharedCSVArray
{
public:
	CPluCSVArray();
	
	bool FindPluByNumber ( __int64 nPluNo, int& nIndex ); 
	
	__int64 GetPluNoInt( int nIndex );
	const char* GetPluNoString ( int nIndex );

	int InsertRecord ( CPluCSVRecord& NewRecord );
	int ImportLine ( int nImportMethod, int& nIndex, CSSImportFile& import, const char* szImportLine, bool bWithStock = FALSE );
	int ImportLinePriceSet ( int& nIndex, CSSImportFile& import, const char* szImportLine, CString& strOldRecord );
	void ImportEODPluLine( CEODPluImportInfo& ImportInfo );

	void SetModifiedFlag ( bool b ) { CSharedCSVArray::SetModifiedFlag(b); }

	int SaveRecord ( int nPluIdx, CPluCSVRecord& PluRecord );

	bool CopyDefaultPlu( int nDeptNo, CPluCSVRecord& Target );

private:
	CString m_strPluNo;
};

/**********************************************************************/
