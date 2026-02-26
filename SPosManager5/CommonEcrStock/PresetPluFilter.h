#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/

class CPresetPluFilter
{
public:
	CPresetPluFilter();
	void SetFilename(int nDbIdx, int nPresetNo);
	void Reset();

	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom(CPresetPluFilter& source);
	void SetDefaults();

private:
	void CreateStockFields();
	void ReadStockFields(CEnhancedIniFile& iniFile);
	void WriteStockFields(CEnhancedIniFile& iniFile);

private:
	bool ReadInternal();
	void ReloadFromIniFile(CEnhancedIniFile& iniFile);
	void PrepareIniFile(CEnhancedIniFile& iniFile);

public:
	bool GetPluRangeFlag() { return m_PluRangeFlag.GetValue(); }
	__int64 GetPluRangeStart() { return m_PluRangeStart.GetValue(); }
	__int64 GetPluRangeEnd() { return m_PluRangeEnd.GetValue(); }
	CString GetDescription() { return m_Description.GetValue(); }
	int GetSearchType() { return m_SearchType.GetValue(); }
	bool GetMatchCaseFlag() { return m_MatchCase.GetValue(); }
	bool GetLocalPluLocationFlag() { return m_LocalPluLocation.GetValue(); }
	bool GetLocalPluMasterFlag() { return m_LocalPluMaster.GetValue(); }
	bool GetLocalPluExcludeFlag() { return m_LocalPluExclude.GetValue(); }
	bool GetEANOnlyFlag() { return m_EANOnly.GetValue(); }
	bool GetLiteralOnlyFlag() { return m_LiteralOnly.GetValue(); }
	int GetDepartmentFilter() { return m_DepartmentFilter.GetValue(); }
	bool GetShowActiveItemsFlag() { return m_ShowActiveItems.GetValue(); }
	bool GetShowInactiveItemsFlag() { return m_ShowInactiveItems.GetValue(); }
	CString GetStockmanStockCode() { return m_StockmanStockCode.GetValue(); }
	bool GetStockmanShowUnlinkedFlag() { return m_StockmanShowUnlinked.GetValue(); }
	bool GetStockmanShowLinkedFlag() { return m_StockmanShowLinked.GetValue(); }
	bool GetStockmanShowRecipeFlag() { return m_StockmanShowRecipe.GetValue(); }
	int GetEcrmanSupplierFilter() { return m_EcrmanSupplierFilter.GetValue(); }
	bool GetEcrmanShowNonStockFlag() { return m_EcrmanShowNonStock.GetValue(); }
	CString GetEcrmanSuppRef() { return m_EcrmanSuppRef.GetValue(); }

public:
	void SetPluRangeFlag(bool b) { m_PluRangeFlag.SetValue(b); }
	void SetPluRangeStart(__int64 n) { m_PluRangeStart.SetValue(n); }
	void SetPluRangeEnd(__int64 n) { m_PluRangeEnd.SetValue(n); }
	void SetDescription( CString str ) { m_Description.SetValue(str); }
	void SetSearchType( int n ) { m_SearchType.SetValue(n); }
	void SetMatchCaseFlag( bool b ) { m_MatchCase.SetValue(b); }
	void SetLocalPluLocationFlag( bool b ) { m_LocalPluLocation.SetValue(b); }
	void SetLocalPluMasterFlag( bool b ) { m_LocalPluMaster.SetValue(b); }
	void SetLocalPluExcludeFlag( bool b ) { m_LocalPluExclude.SetValue(b); }
	void SetEANOnlyFlag( bool b) { m_EANOnly.SetValue(b); }
	void SetLiteralOnlyFlag( bool b ) { m_LiteralOnly.SetValue(b); }
	void SetDepartmentFilter( int n ) { m_DepartmentFilter.SetValue(n); }
	void SetShowActiveItemsFlag( bool b ) { m_ShowActiveItems.SetValue(b); }
	void SetShowInactiveItemsFlag( bool b ) { m_ShowInactiveItems.SetValue(b); }
	void SetStockmanStockCode( CString str ) { m_StockmanStockCode.SetValue(str); }
	void SetStockmanShowUnlinkedFlag( bool b ) { m_StockmanShowUnlinked.SetValue(b); }
	void SetStockmanShowLinkedFlag( bool b ) { m_StockmanShowLinked.SetValue(b); }
	void SetStockmanShowRecipeFlag( bool b ) { m_StockmanShowRecipe.SetValue(b); }
	void SetEcrmanSupplierFilter( int n ) { m_EcrmanSupplierFilter.SetValue(n); }
	void SetEcrmanShowNonStockFlag( bool b ) { m_EcrmanShowNonStock.SetValue(b); }
	void SetEcrmanSuppRef( CString str ) { m_EcrmanSuppRef.SetValue(str); }

private:
	CIniFileBool m_PluRangeFlag;
	CIniFileInt64 m_PluRangeStart;
	CIniFileInt64 m_PluRangeEnd;
	CIniFileString m_Description;
	CIniFileInt m_SearchType;
	CIniFileBool m_MatchCase;
	CIniFileBool m_LocalPluLocation;
	CIniFileBool m_LocalPluMaster;
	CIniFileBool m_LocalPluExclude;
	CIniFileBool m_EANOnly;
	CIniFileBool m_LiteralOnly;
	CIniFileInt m_DepartmentFilter;
	CIniFileBool m_ShowActiveItems;
	CIniFileBool m_ShowInactiveItems;
	CIniFileString m_StockmanStockCode;
	CIniFileBool m_StockmanShowUnlinked;
	CIniFileBool m_StockmanShowLinked;
	CIniFileBool m_StockmanShowRecipe;
	CIniFileInt m_EcrmanSupplierFilter;
	CIniFileBool m_EcrmanShowNonStock;
	CIniFileString m_EcrmanSuppRef;

private:
	CString m_strDataFilename;
	bool m_bFatalReadError;
};

/**********************************************************************/
#endif
/**********************************************************************/
