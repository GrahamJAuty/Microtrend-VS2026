/**********************************************************************/
#include "SpecialKeys.h"
/**********************************************************************/

CSpecialKeys::CSpecialKeys()
{
	m_HiddenOptionsKey.Create( "SpecialKeyHiddenOptions", 1, "H" );
	m_PluNumberModeKey.Create( "SpecialKeyPluNumberMode", 1, "P" );
	m_RebuildSalesHistoryKey.Create( "SpecialKeyRebuildSalesHistory", 1, "R" );
	m_CustomerAutoDeleteKey.Create( "SpecialKeyCustomerAutoDelete", 1, "C" );
	m_ButtonOptionsKey.Create( "SpecialKeyButtonOptions", 1, "B" );
	m_ButtonActionsKey.Create( "SpecialKeyButtonActions", 1, "L" );
	m_PMSRebuildSalesKey.Create( "SpecialKeyPMSRebuildSales", 1, "R" );
	m_PMSRebuildTaxKey.Create( "SpecialKeyPMSRebuildTax", 1, "V" );
	
#ifdef STOCKMAN_SYSTEM
	m_StockUpgradeKey.Create( "SpecialKeyStockUpgrade", 1, "E" );
#endif

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CSpecialKeys::CopyFrom( CSpecialKeys& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CSpecialKeys::ReloadFromIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.ReadString( m_HiddenOptionsKey );
	iniFile.ReadString( m_PluNumberModeKey );
	iniFile.ReadString( m_RebuildSalesHistoryKey );
	iniFile.ReadString( m_CustomerAutoDeleteKey );
	iniFile.ReadString( m_ButtonOptionsKey );
	iniFile.ReadString( m_ButtonActionsKey );
	iniFile.ReadString( m_PMSRebuildSalesKey );
	iniFile.ReadString( m_PMSRebuildTaxKey );

#ifdef STOCKMAN_SYSTEM
	iniFile.ReadString( m_StockUpgradeKey );
#endif
}

/**********************************************************************/

void CSpecialKeys::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteString( m_HiddenOptionsKey );
	iniFile.WriteString( m_PluNumberModeKey );
	iniFile.WriteString( m_RebuildSalesHistoryKey );
	iniFile.WriteString( m_CustomerAutoDeleteKey );
	iniFile.WriteString( m_ButtonOptionsKey );
	iniFile.WriteString( m_ButtonActionsKey );
	iniFile.WriteString( m_PMSRebuildSalesKey );
	iniFile.WriteString( m_PMSRebuildTaxKey );

#ifdef STOCKMAN_SYSTEM
	iniFile.WriteString( m_StockUpgradeKey );
#endif
}

/**********************************************************************/

void CSpecialKeys::Reset()
{
	m_HiddenOptionsKey.Reset();
	m_PluNumberModeKey.Reset();
	m_RebuildSalesHistoryKey.Reset();
	m_CustomerAutoDeleteKey.Reset();
	m_ButtonOptionsKey.Reset();
	m_ButtonActionsKey.Reset();
	m_PMSRebuildSalesKey.Reset();
	m_PMSRebuildTaxKey.Reset();

#ifdef STOCKMAN_SYSTEM
	m_StockUpgradeKey.Reset();
#endif
}

/**********************************************************************/

bool CSpecialKeys::Read()
{
	CFilenameUpdater FnUp( SysFiles::SpecialKeys );
	CString strFilename = FnUp.GetFilenameToUse();
	
	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile;
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return TRUE;
}

/**********************************************************************/

bool CSpecialKeys::Write()
{
	bool bResult = FALSE;

	if ( DataManager.LockAllUsers() == TRUE )
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile( iniFile );
		
		CFilenameUpdater FnUp( SysFiles::SpecialKeys );
		bResult = iniFile.Write( FnUp.GetFilenameToUse() );

		DataManager.UnlockAllUsers();
	}
		
	return bResult;
}

/**********************************************************************/
