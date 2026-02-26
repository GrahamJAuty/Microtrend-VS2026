#pragma once
/**********************************************************************/
#include "..\SPosLoyalty4Shared\EnhancedIniFile.h"
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

	void CopyFrom( CPCOptionsClient& source );	
	void SetDefaults();

private:
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	
public:
	const char* GetDefaultReportPrinter()		{ return m_DefaultReportPrinter.GetValue(); }
	const char* GetDefaultBarcodePrinter()		{ return m_DefaultBarcodePrinter.GetValue(); }

public:
	void SetDefaultReportPrinter( const char* sz )		{ m_DefaultReportPrinter.SetValue(sz); }
	void SetDefaultBarcodePrinter(const char* sz)		{ m_DefaultBarcodePrinter.SetValue(sz); }

private:
	CIniFileString m_DefaultReportPrinter;
	CIniFileString m_DefaultBarcodePrinter;

private:
	bool m_bFatalReadError;
};

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

	void CopyFrom(CPCOptionsHost& source);
	void SetDefaults();

private:
	void ReloadFromIniFile(CEnhancedIniFile& iniFile);
	void PrepareIniFile(CEnhancedIniFile& iniFile);

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
extern CPCOptionsHost PCOptionsHost;			/* global */
extern CPCOptionsClient PCOptionsClient;		/* global */
/**********************************************************************/
