/**********************************************************************/
#include "TaxImportMap.h"
/**********************************************************************/

CTaxImportMap::CTaxImportMap()
{
	m_TaxMapT0.Create("T0", 1, "L");
	m_TaxMapT1.Create("T1", 1, "A");
	m_TaxMapT2.Create("T2", 1, "B");
	m_TaxMapT3.Create("T3", 1, "C");
	m_TaxMapT4.Create("T4", 1, "D");
	m_TaxMapT5.Create("T5", 1, "E");
	m_TaxMapT6.Create("T6", 1, "F");
	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CTaxImportMap::CopyFrom(CTaxImportMap& source)
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile(iniFile);
	ReloadFromIniFile(iniFile);
}

/**********************************************************************/

void CTaxImportMap::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadString(m_TaxMapT0);
	iniFile.ReadString(m_TaxMapT1);
	iniFile.ReadString(m_TaxMapT2);
	iniFile.ReadString(m_TaxMapT3);
	iniFile.ReadString(m_TaxMapT4);
	iniFile.ReadString(m_TaxMapT5);
	iniFile.ReadString(m_TaxMapT6);
}

/**********************************************************************/

void CTaxImportMap::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteString(m_TaxMapT0);
	iniFile.WriteString(m_TaxMapT1);
	iniFile.WriteString(m_TaxMapT2);
	iniFile.WriteString(m_TaxMapT3);
	iniFile.WriteString(m_TaxMapT4);
	iniFile.WriteString(m_TaxMapT5);
	iniFile.WriteString(m_TaxMapT6);
}

/**********************************************************************/

void CTaxImportMap::Reset()
{
	m_TaxMapT0.Reset();
	m_TaxMapT1.Reset();
	m_TaxMapT2.Reset();
	m_TaxMapT3.Reset();
	m_TaxMapT4.Reset();
	m_TaxMapT5.Reset();
	m_TaxMapT6.Reset();
}

/**********************************************************************/

bool CTaxImportMap::Read()
{
	bool bResult = ReadInternal();
	return bResult;
}

/**********************************************************************/

bool CTaxImportMap::ReadInternal()
{
	CFilenameUpdater FnUp(SysFiles::TaxImportMap);
	CString strFilename = FnUp.GetFilenameToUse();

	if (::FileExists(strFilename) == TRUE)
	{
		CEnhancedIniFile iniFile(SS_INI_NORMAL);
		if (iniFile.Read(strFilename) == FALSE)
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}

		ReloadFromIniFile(iniFile);
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

bool CTaxImportMap::Write()
{
	bool bResult = FALSE;

	if (DataManager.LockAllUsers() == TRUE)
	{
		CEnhancedIniFile iniFile(SS_INI_NORMAL);
		PrepareIniFile(iniFile);

		CFilenameUpdater FnUp(SysFiles::TaxImportMap);
		bResult = iniFile.Write(FnUp.GetFilenameToUse());

		DataManager.UnlockAllUsers();
	}

	return bResult;
}

/**********************************************************************/
