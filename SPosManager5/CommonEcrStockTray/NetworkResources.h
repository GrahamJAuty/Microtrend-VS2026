#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/
#define MAX_NETWORK_RESOURCES 15
/**********************************************************************/
#define MAX_LENGTH_NETWORKRESOURCE_NAME 150
#define MAX_LENGTH_NETWORKRESOURCE_USER 50
#define MAX_LENGTH_NETWORKRESOURCE_PASSWORD 50
/**********************************************************************/

class CNetworkResources 
{
public:
	CNetworkResources();
	void Reset();
	
	bool Read();
	bool Write();

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

	void CopyFrom( CNetworkResources& source );	
	void SetDefaults();

private:
	bool ReadInternal();
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );

public:
	void RunBatchFile();
	
public:
	const char* GetResourceName( int n )		{ return m_ResourceNames.GetValue(n); }
	const char* GetUserName( int n )			{ return m_UserNames.GetValue(n); }
	const char* GetPassword( int n )			{ return m_Passwords.GetValue(n); }

public:
	void SetResourceName( int n, const char* sz )	{ m_ResourceNames.SetValue(n, sz); }
	void SetUserName( int n, const char* sz )		{ m_UserNames.SetValue(n, sz); }
	void SetPassword( int n, const char* sz )		{ m_Passwords.SetValue(n ,sz); }

private:
	CIniFileStringArray m_ResourceNames;
	CIniFileStringArray m_UserNames;
	CIniFileStringArray m_Passwords;
	
private:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CNetworkResources NetworkResources;		/* global */
/**********************************************************************/
