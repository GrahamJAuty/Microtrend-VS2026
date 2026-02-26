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
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );
	
public:
	const char* GetDefaultReportPrinter()		{ return m_DefaultReportPrinter.GetValue(); }

public:
	void SetDefaultReportPrinter( const char* sz )	{ m_DefaultReportPrinter.SetValue(sz); }
	
private:
	CIniFileString m_DefaultReportPrinter;

private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CPCOptionsClient PCOptionsClient;		/* global */
/**********************************************************************/
