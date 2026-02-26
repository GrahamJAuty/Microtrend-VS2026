#pragma once
/**********************************************************************/
#include "EnhancedIniFile.h"
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
	const char* GetDefaultReportPrinter() { return m_DefaultReportPrinter.GetValue(); }

public:
	void SetBackupAccessType(int n) { m_BackupAccessType.SetValue(n); }
	void SetDefaultReportPrinter(const char* sz) { m_DefaultReportPrinter.SetValue(sz); }

private:
	CIniFileInt m_BackupAccessType;
	CIniFileString m_DefaultReportPrinter;

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
	const char* GetDefaultLabelPrinter() { return m_DefaultLabelPrinter.GetValue(); }
	const char* GetDefaultReportPrinter() { return m_DefaultReportPrinter.GetValue(); }
	
public:
	void SetDefaultLabelPrinter(const char* sz) { m_DefaultLabelPrinter.SetValue(sz); }
	void SetDefaultReportPrinter(const char* sz) { m_DefaultReportPrinter.SetValue(sz); }

private:
	CIniFileString m_DefaultLabelPrinter;
	CIniFileString m_DefaultReportPrinter;

private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CPCOptionsHost PCOptionsHost;			/* global */
extern CPCOptionsClient PCOptionsClient;		/* global */
/**********************************************************************/
