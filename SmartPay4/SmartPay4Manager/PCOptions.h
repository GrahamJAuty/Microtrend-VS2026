#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\EnhancedIniFile.h"
/**********************************************************************/
#define BACKUP_ACCESSTYPE_ALL 0
#define BACKUP_ACCESSTYPE_NOAUTO 1
#define BACKUP_ACCESSTYPE_RESTORE 2
#define BACKUP_ACCESSTYPE_NONE 3
/**********************************************************************/

class CPCOptionsHost 
{
public:
	CPCOptionsHost();
	void Reset();
	
	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CPCOptionsHost& source );	
	void SetDefaults();

private:
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	
public:
	int GetBackupAccessType() { return m_BackupAccessType.GetValue(); }

public:
	void SetBackupAccessType(int n) { m_BackupAccessType.SetValue(n); }
	
private:
	CIniFileInt m_BackupAccessType;

private:
	bool m_bFatalReadError;
};

/**********************************************************************/

class CPCOptionsClient
{
public:
	CPCOptionsClient();
	void Reset();

	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom(CPCOptionsClient& source);
	void SetDefaults();

private:
	bool ReadInternal();
	void ReloadFromIniFile(CEnhancedIniFile& iniFile);
	void PrepareIniFile(CEnhancedIniFile& iniFile);

public:
	const char* GetDefaultReportPrinter() { return m_DefaultReportPrinter.GetValue(); }

public:
	void SetDefaultReportPrinter(const char* sz) { m_DefaultReportPrinter.SetValue(sz); }

private:
	CIniFileString m_DefaultReportPrinter;

private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CPCOptionsClient PCOptionsClient;	/* global */
extern CPCOptionsHost PCOptionsHost;		/* global */
/**********************************************************************/
