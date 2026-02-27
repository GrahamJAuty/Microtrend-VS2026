/**********************************************************************/
#include "ServiceOptions.h"
/**********************************************************************/

CServiceOptions::CServiceOptions(bool bBackground)
{
	m_bBackground = bBackground;

	m_PCId.Create("PCId", 0, 9999999999, 0);
	m_WindowsLogon.Create("WindowsLogon", FALSE);
	m_WindowsUsername.Create("WindowsUsername", MAX_LENGTH_WINDOWS_USERNAME, "");
	m_WindowsPassword.Create("WindowsPassword", MAX_LENGTH_WINDOWS_PASSWORD, "");
	m_ServerIconType.Create("ServerIconType", 0, 2, 0);
	m_AllowDesktopApp.Create("AllowDesktopApp", FALSE);

	Reset();
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

void CServiceOptions::CopyFrom(CServiceOptions& source)
{
	CEnhancedIniFile iniFile;
	source.PrepareIniFile(iniFile);
	ReloadFromIniFile(iniFile);
}

/**********************************************************************/

void CServiceOptions::ReloadFromIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.ReadInt64(m_PCId);
	iniFile.ReadBool(m_WindowsLogon);
	iniFile.ReadString(m_WindowsUsername);
	iniFile.ReadString(m_WindowsPassword);
	iniFile.ReadInt(m_ServerIconType);
	iniFile.ReadBool(m_AllowDesktopApp);
}

/**********************************************************************/

void CServiceOptions::PrepareIniFile(CEnhancedIniFile& iniFile)
{
	iniFile.WriteInt64(m_PCId);
	iniFile.WriteBool(m_WindowsLogon);
	iniFile.WriteString(m_WindowsUsername);
	iniFile.WriteString(m_WindowsPassword);
	iniFile.WriteInt(m_ServerIconType);
	iniFile.WriteBool(m_AllowDesktopApp);
}

/**********************************************************************/

void CServiceOptions::Reset()
{
	m_PCId.Reset();
	m_WindowsLogon.Reset();
	m_WindowsUsername.Reset();
	m_WindowsPassword.Reset();
	m_ServerIconType.Reset();
	m_AllowDesktopApp.Reset();
}

/**********************************************************************/

void CServiceOptions::SetDefaults()
{
	Reset();
}

/**********************************************************************/

bool CServiceOptions::Read()
{
	bool bResult = FALSE;

	CString strFilename = m_bBackground ? Filenames.GetBgndServiceOptionsFilename() : Filenames.GetServerServiceOptionsFilename();

	if ( ::FileExists( strFilename ) == TRUE )
	{
		CEnhancedIniFile iniFile( SS_INI_AES );;
		if ( iniFile.Read( strFilename ) == FALSE )
		{
			m_bFatalReadError = TRUE;
		}
		else
		{
			ReloadFromIniFile( iniFile );
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CServiceOptions::Write()
{
	CString strFilename = m_bBackground ? Filenames.GetBgndServiceOptionsFilename() : Filenames.GetServerServiceOptionsFilename();

	CEnhancedIniFile iniFile( SS_INI_AES );;
	PrepareIniFile( iniFile );
	return iniFile.Write( strFilename );
}

/**********************************************************************/

