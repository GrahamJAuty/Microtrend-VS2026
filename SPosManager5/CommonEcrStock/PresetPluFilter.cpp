/**********************************************************************/
#include "PresetPluFilter.h"
/**********************************************************************/

CPresetPluFilter::CPresetPluFilter()
{
	m_strDataFilename = "";

	m_PluRangeFlag.Create("PluRangeFlag", FALSE);
	m_PluRangeStart.Create("PluRangeStart", 1, Plu::PluNo.Max, 1);
	m_PluRangeEnd.Create("PluRangeEnd", 1, Plu::PluNo.Max, Plu::PluNo.Max);
	m_Description.Create("Description", Plu::RepText.Max, "");
	m_SearchType.Create("SearchType", 0, 2, 0);
	m_MatchCase.Create("MatchCase", FALSE);
	m_LocalPluLocation.Create("LocalPluLocation", TRUE);
	m_LocalPluMaster.Create("LocalPluMaster", TRUE);
	m_LocalPluExclude.Create("LocalPluExclude", TRUE);
	m_EANOnly.Create("EANOnly", FALSE);
	m_LiteralOnly.Create("LiteralOnly", FALSE);
	m_DepartmentFilter.Create("DepartmentFilter", 0, 20000, DEPARTMENTSET_ALL);
	m_ShowActiveItems.Create("ShowActiveItems", TRUE);
	m_ShowInactiveItems.Create("ShowInactiveItems", TRUE);
	CreateStockFields();

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CPresetPluFilter::SetFilename(int nDbIdx, int nPreset)
{
	CFilenameUpdater FnUp(SysFiles::PresetPluFilters, nDbIdx, nPreset);
	m_strDataFilename = FnUp.GetFilenameToUse();
}

/**********************************************************************/

void CPresetPluFilter::CopyFrom(CPresetPluFilter& source )
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile( iniFile );
	ReloadFromIniFile( iniFile );
}

/**********************************************************************/

void CPresetPluFilter::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadBool(m_PluRangeFlag);
	iniFile.ReadInt64(m_PluRangeStart);
	iniFile.ReadInt64(m_PluRangeEnd);
	iniFile.ReadString(m_Description);
	iniFile.ReadInt(m_SearchType);
	iniFile.ReadBool(m_MatchCase);
	iniFile.ReadBool(m_LocalPluLocation);
	iniFile.ReadBool(m_LocalPluMaster);
	iniFile.ReadBool(m_LocalPluExclude);
	iniFile.ReadBool(m_EANOnly);
	iniFile.ReadBool(m_LiteralOnly);
	iniFile.ReadInt(m_DepartmentFilter);
	iniFile.ReadBool(m_ShowActiveItems);
	iniFile.ReadBool(m_ShowInactiveItems);
	ReadStockFields(iniFile);
}

/**********************************************************************/

void CPresetPluFilter::PrepareIniFile( CEnhancedIniFile& iniFile )
{
	iniFile.WriteBool(m_PluRangeFlag);
	iniFile.WriteInt64(m_PluRangeStart);
	iniFile.WriteInt64(m_PluRangeEnd);
	iniFile.WriteString(m_Description);
	iniFile.WriteInt(m_SearchType);
	iniFile.WriteBool(m_MatchCase);
	iniFile.WriteBool(m_LocalPluLocation);
	iniFile.WriteBool(m_LocalPluMaster);
	iniFile.WriteBool(m_LocalPluExclude);
	iniFile.WriteBool(m_EANOnly);
	iniFile.WriteBool(m_LiteralOnly);
	iniFile.WriteInt(m_DepartmentFilter);
	iniFile.WriteBool(m_ShowActiveItems);
	iniFile.WriteBool(m_ShowInactiveItems);
	WriteStockFields(iniFile);
}

/**********************************************************************/

void CPresetPluFilter::Reset()
{
	m_PluRangeFlag.Reset();
	m_PluRangeStart.Reset();
	m_PluRangeEnd.Reset();
}

/**********************************************************************/

void CPresetPluFilter::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CPresetPluFilter::Read()
{
	bool bResult = ReadInternal();
	return bResult;
}

/**********************************************************************/

bool CPresetPluFilter::ReadInternal()
{
	if ( ::FileExists( m_strDataFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile( SS_INI_NORMAL );
		if ( iniFile.Read( m_strDataFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile( iniFile );
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CPresetPluFilter::Write()
{
	bool bResult = FALSE;

	CEnhancedIniFile iniFile( SS_INI_NORMAL );
	PrepareIniFile( iniFile );
		
	bResult = iniFile.Write( m_strDataFilename );

	return bResult;
}

/**********************************************************************/
