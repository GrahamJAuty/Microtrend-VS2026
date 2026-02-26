#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/

class CSageOptions 
{
public:
	CSageOptions();
	void Reset();

	bool Read();
	bool Write();

	void CopyFrom( CSageOptions& source );
	
	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

private:
	void ReloadFromIniFile( CEnhancedIniFile& m_iniFile );	
	void PrepareIniFile( CEnhancedIniFile& m_iniFile );

public:
	bool GetExportEnableFlag()			{ return m_ExportEnable.GetValue(); }
	int GetExportDelayTime()			{ return m_ExportDelayTime.GetValue(); }
	const char* GetAccountReference()	{ return m_AccountRef.GetValue(); }
	int GetNominalCodeLength()			{ return m_CodeLength.GetValue(); }
	
	void SetExportEnableFlag( bool b )			{ m_ExportEnable.SetValue(b); }
	void SetExportDelayTime( int n )			{ m_ExportDelayTime.SetValue(n); }
	void SetAccountReference( const char* sz )	{ m_AccountRef.SetValue(sz); }
	void SetNominalCodeLength( int n)			{ m_CodeLength.SetValue(n); }
	
	void CreateSageFolders();
	
private:
	CIniFileBool m_ExportEnable;
	CIniFileInt m_ExportDelayTime;
	CIniFileString m_AccountRef;
	CIniFileInt m_CodeLength;
	
public:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CSageOptions SageOptions; /* global */
/**********************************************************************/

