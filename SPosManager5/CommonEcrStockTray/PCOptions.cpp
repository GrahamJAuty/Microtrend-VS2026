/**********************************************************************/
#include "PCOptions.h"
/**********************************************************************/

CPCOptionsHost::CPCOptionsHost()
{
	m_BackupAccessType.Create("BackupAccessType", 0, 3, 0);
	m_DefaultReportPrinter.Create("DefaultReportPrinter", 150, "");

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CPCOptionsHost::CopyFrom(CPCOptionsHost& source)
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile(iniFile);
	ReloadFromIniFile(iniFile);
}

/**********************************************************************/

void CPCOptionsHost::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadInt(m_BackupAccessType);
	iniFile.ReadString(m_DefaultReportPrinter);
}

/**********************************************************************/

void CPCOptionsHost::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteInt(m_BackupAccessType);
	iniFile.WriteString(m_DefaultReportPrinter);
}

/**********************************************************************/

void CPCOptionsHost::Reset()
{
	m_BackupAccessType.Reset();
	m_DefaultReportPrinter.Reset();
}

/**********************************************************************/

void CPCOptionsHost::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CPCOptionsHost::Read()
{
	bool bResult = ReadInternal();
	return bResult;
}

/**********************************************************************/

bool CPCOptionsHost::ReadInternal()
{
	CFilenameUpdater FnUp(SysFiles::PCOptionsHost);
	CString strFilename = FnUp.GetFilenameToUse();

	if (::FileExists(strFilename) == TRUE)
	{
		CEnhancedIniFile iniFile;
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

bool CPCOptionsHost::Write()
{
	bool bResult = FALSE;

	if (DataManager.LockAllUsers() == TRUE)
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile(iniFile);

		CFilenameUpdater FnUp(SysFiles::PCOptionsHost);
		bResult = iniFile.Write(FnUp.GetFilenameToUse());

		DataManager.UnlockAllUsers();
	}

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPCOptionsClient::CPCOptionsClient()
{
	m_DefaultLabelPrinter.Create("DefaultLabelPrinter", 150, "");
	m_DefaultReportPrinter.Create("DefaultReportPrinter", 150, "");
	
	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CPCOptionsClient::CopyFrom(CPCOptionsClient& source)
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile(iniFile);
	ReloadFromIniFile(iniFile);
}

/**********************************************************************/

void CPCOptionsClient::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadString(m_DefaultLabelPrinter);
	iniFile.ReadString(m_DefaultReportPrinter);
}

/**********************************************************************/

void CPCOptionsClient::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteString(m_DefaultLabelPrinter);
	iniFile.WriteString(m_DefaultReportPrinter);
}

/**********************************************************************/

void CPCOptionsClient::Reset()
{
	m_DefaultLabelPrinter.Reset();
	m_DefaultReportPrinter.Reset();
}

/**********************************************************************/

void CPCOptionsClient::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CPCOptionsClient::Read()
{
	bool bResult = ReadInternal();
	return bResult;
}

/**********************************************************************/

bool CPCOptionsClient::ReadInternal()
{
	CFilenameUpdater FnUp(SysFiles::PCOptionsClient);
	CString strFilename = FnUp.GetFilenameToUse();

	if (::FileExists(strFilename) == TRUE)
	{
		CEnhancedIniFile iniFile;
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

bool CPCOptionsClient::Write()
{
	bool bResult = FALSE;

	if (DataManager.LockAllUsers() == TRUE)
	{
		CEnhancedIniFile iniFile;
		PrepareIniFile(iniFile);

		CFilenameUpdater FnUp(SysFiles::PCOptionsClient);
		bResult = iniFile.Write(FnUp.GetFilenameToUse());

		DataManager.UnlockAllUsers();
	}

	return bResult;
}

/**********************************************************************/
