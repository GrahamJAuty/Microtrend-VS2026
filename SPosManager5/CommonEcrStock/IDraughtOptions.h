#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/

struct CIDraughtDeptFilter
{
	CString m_strFilter;
	bool m_bChecked;
	bool m_bInvalid;
	int m_nErrorInfo;
};

/**********************************************************************/

class CIDraughtOptions 
{
public:
	CIDraughtOptions();
	void Reset();

	bool Read();
	bool Write();
	
	void CopyFrom( CIDraughtOptions& source );

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

private:
	void ReloadFromIniFile( CEnhancedIniFile& file );
	void PrepareIniFile( CEnhancedIniFile& file );

public:
	int GetExportDelayTime()				{ return m_ExportDelayTime.GetValue(); }
	int GetConnectRetryTime()				{ return m_ConnectRetryTime.GetValue(); }
	const char* GetFTPServer()				{ return m_FTPServer.GetValue(); }
	const char* GetFTPUserName()			{ return m_FTPUserName.GetValue(); }
	const char* GetFTPPassword()			{ return m_FTPPassword.GetValue(); }

	void SetExportDelayTime( int n )		{ m_ExportDelayTime.SetValue(n); } 
	void SetConnectRetryTime( int n )		{ m_ConnectRetryTime.SetValue(n); } 
	void SetFTPServer( const char* sz )		{ m_FTPServer.SetValue(sz); }
	void SetFTPUserName( const char* sz )	{ m_FTPUserName.SetValue(sz); }
	void SetFTPPassword( const char* sz )	{ m_FTPPassword.SetValue(sz); }

	void CreateIDraughtFolders();

	void ResetDepartmentFilters();
	void GetDatabaseDepartmentFilter( int nDbIdx, CIDraughtDeptFilter& info );
	bool GetDatabaseDepartmentFlag( int nDbIdx, int nDeptNo );
	void SetDatabaseDepartmentFlag( int nDbIdx, int nDeptNo, bool bFlag );

	void SetSiteIDBuffer( const char* sz ){ m_strSiteIDBuffer = sz; }
	const char* GetSiteIDBuffer(){ return m_strSiteIDBuffer; }

	int GetFiddleLevel();
		
private:
	CIniFileInt m_ConnectRetryTime;
	CIniFileInt m_ExportDelayTime;
	CIniFileString m_FTPServer;
	CIniFileString m_FTPUserName;
	CIniFileString m_FTPPassword;

private:
	CString m_strSiteIDBuffer;
		
public:
	bool m_bFatalReadError;

private:
	CArray<CIDraughtDeptFilter,CIDraughtDeptFilter> m_arrayDeptFilter;
};

/**********************************************************************/
extern CIDraughtOptions IDraughtOptions; /* global */
/**********************************************************************/

