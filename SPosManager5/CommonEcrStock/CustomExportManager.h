#pragma once
/**********************************************************************/
#include "PriceSetIncludes.h"
#include "PriceSetItemManager.h"
/**********************************************************************/

class CCustomExportManagerBase  
{
public:
	CCustomExportManagerBase();
	
protected:
	void ProcessExports( CPriceSetExportInfo& info );
	
	void ProcessPluExport( CString& strPath, CString& strExportHeader );
	void ProcessPluApparentExport( CString& strPath, CString& strExportHeader );
	
	void ProcessBarcodeExport( CString& strPath, CString& strExportHeader );
	void ProcessBarcodeApparentExport( CString& strPath, CString& strExportHeader );
	
	void ProcessSupplierExport( CString& strPath, CString& strExportHeader );

protected:
	bool WantAllPluFields(const CString& strHeader) const 
	{
		return strHeader.Left(1) == "*";
	}

	void GetPluExportHeaders(const bool bAllPluFields, const CString strRequestedHeader, CString& strDatabaseHeader, CString& strExportHeader) const;
	void RemoveHeaderBlanks(CString& strHeader, const bool bRemoveEcrmanStock = FALSE, const bool bForBarcodeExport = FALSE) const;
	bool CompareLabel(const CString strFileLabel, CString strDatabaseLabel) const;
	CString GetBarcodeDatabaseHeader() const;
	CString GetBarcodeDatabaseLine(CBarcodeCSVRecord& BarcodeRecord) const;

#ifdef STOCKMAN_SYSTEM
	void CheckOptionalStockFields(CString& strHeader, bool& bWantDbApparentLevel, bool& bWantStockQty, bool& bWantStockLink) const;
	void ReattachStockHeaders(const bool bWantApparentDbLevel, const bool bWantStockQty, const bool bWantPluStockLink, const CString& strStockDatabaseHeader, CString& strExportHeaderToUse) const;
	void FixPluPurchaseCost(const int nPluIdx, CPluCSVRecord& PluRecord, CRecipeCSVRecord& RecipeRecord) const;
#endif

public:
	void WritePluPointsFile();
	void WriteWalletStampRuleFile();
	void WriteDepartmentFile();
	void WriteLoyaltySchemeFile();
	void WriteOperatorFile();
	void WriteEposTerminalFile();
	void WriteDbInfoFile();

public:
	static bool WantStockLevels();

protected:
	bool m_bDoPlu;
	bool m_bApparentStock;
	bool m_bAlternativeExports;
};

/**********************************************************************/

class CCustomExportManagerPlu : public CCustomExportManagerBase
{
public:
	CCustomExportManagerPlu( bool bApparentStock = FALSE, bool bAlternativeExports = FALSE );
};

/**********************************************************************/

class CCustomExportManagerPriceSet : public CCustomExportManagerBase
{
public:
	CCustomExportManagerPriceSet( CPriceSetExportInfo& info );
};

/**********************************************************************/

class CCustomExportManagerSCard : public CCustomExportManagerBase
{
public:
	CCustomExportManagerSCard();
};

/**********************************************************************/
