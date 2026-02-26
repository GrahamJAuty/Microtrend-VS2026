#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/

class CSpecialKeys 
{
public:
	CSpecialKeys();
	void Reset();

	bool Read();
	bool Write();
	
	bool GetFatalReadErrorFlag()	{ return m_bFatalReadError; }
	void ClearFatalReadErrorFlag()	{ m_bFatalReadError = FALSE; }

	void CopyFrom( CSpecialKeys& source );

private:
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );

public:
	const char* GetHiddenOptionsKey()					{ return m_HiddenOptionsKey.GetValue(); }
	const char* GetPluNumberModeKey()					{ return m_PluNumberModeKey.GetValue(); }
	const char* GetRebuildSalesHistoryKey()				{ return m_RebuildSalesHistoryKey.GetValue(); }
	const char* GetCustomerAutoDeleteKey()				{ return m_CustomerAutoDeleteKey.GetValue(); }
	const char* GetButtonOptionsKey()					{ return m_ButtonOptionsKey.GetValue(); }
	const char* GetButtonActionsKey()					{ return m_ButtonActionsKey.GetValue(); }
	const char* GetPMSRebuildTaxKey()					{ return m_PMSRebuildTaxKey.GetValue(); }
	const char* GetPMSRebuildSalesKey()					{ return m_PMSRebuildSalesKey.GetValue(); }

	void SetHiddenOptionsKey( const char* sz )			{ m_HiddenOptionsKey.SetValue(sz); }
	void SetPluNumberModeKey( const char* sz )			{ m_PluNumberModeKey.SetValue(sz); }
	void SetRebuildSalesHistoryKey( const char* sz )	{ m_RebuildSalesHistoryKey.SetValue(sz); }
	void SetCustomerAutoDeleteKey( const char* sz )		{ m_CustomerAutoDeleteKey.SetValue(sz); }
	void SetButtonOptionsKey( const char* sz )			{ m_ButtonOptionsKey.SetValue(sz); }
	void SetButtonActionsKey( const char* sz )			{ m_ButtonActionsKey.SetValue(sz); }
	void SetPMSRebuildTaxKey( const char* sz )			{ m_PMSRebuildTaxKey.SetValue(sz); }
	void SetPMSRebuildSalesKey( const char* sz )		{ m_PMSRebuildSalesKey.SetValue(sz); }
	
#ifdef STOCKMAN_SYSTEM
	const char* GetStockUpgradeKey()					{ return m_StockUpgradeKey.GetValue(); }
	void SetStockUpgradeKey( const char* sz )			{ m_StockUpgradeKey.SetValue(sz); }
#endif

private:
	CIniFileString m_HiddenOptionsKey;
	CIniFileString m_PluNumberModeKey;
	CIniFileString m_RebuildSalesHistoryKey;
	CIniFileString m_CustomerAutoDeleteKey;
	CIniFileString m_ButtonOptionsKey;
	CIniFileString m_ButtonActionsKey;
	CIniFileString m_PMSRebuildTaxKey;
	CIniFileString m_PMSRebuildSalesKey;

#ifdef STOCKMAN_SYSTEM
	CIniFileString m_StockUpgradeKey;
#endif

public:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CSpecialKeys SpecialKeys; /* global */
/**********************************************************************/
#endif
/**********************************************************************/