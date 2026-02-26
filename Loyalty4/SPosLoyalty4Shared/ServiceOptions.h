#pragma once
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/
#define MAX_LENGTH_WINDOWS_USERNAME 50
#define MAX_LENGTH_WINDOWS_PASSWORD 50
/**********************************************************************/

class CServiceOptions
{
public:
	CServiceOptions();
	void Reset();

	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom(CServiceOptions& source);
	void SetDefaults();

private:
	void ReloadFromIniFile(CEnhancedIniFile& iniFile);
	void PrepareIniFile(CEnhancedIniFile& iniFile);

public:
	__int64 GetPCId() { return m_PCId.GetValue(); }
	bool GetWindowsLogonFlag() { return m_WindowsLogon.GetValue(); }
	const char* GetWindowsUsername() { return m_WindowsUsername.GetValue(); }
	const char* GetWindowsPassword() { return m_WindowsPassword.GetValue(); }
	int GetServerIconType() { return m_ServerIconType.GetValue(); }
	bool GetAllowDesktopAppFlag(){ return m_AllowDesktopApp.GetValue(); }

public:
	void SetPCId(__int64 n) { m_PCId.SetValue(n); }
	void SetWindowsLogonFlag(bool b) { m_WindowsLogon.SetValue(b); }
	void SetWindowsUsername(const char* sz) { m_WindowsUsername.SetValue(sz); }
	void SetWindowsPassword(const char* sz) { m_WindowsPassword.SetValue(sz); }
	void SetServerIconType(int n) { m_ServerIconType.SetValue(n); }
	void SetAllowDesktopAppFlag(bool b) { m_AllowDesktopApp.SetValue(b); }

private:
	CIniFileInt64	m_PCId;
	CIniFileBool	m_WindowsLogon;
	CIniFileString	m_WindowsUsername;
	CIniFileString	m_WindowsPassword;
	CIniFileInt		m_ServerIconType;
	CIniFileBool	m_AllowDesktopApp;

private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CServiceOptions ServerServiceOptions;		/* global */
/**********************************************************************/
