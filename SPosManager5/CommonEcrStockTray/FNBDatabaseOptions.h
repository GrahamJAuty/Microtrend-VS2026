#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/

class CFNBDatabaseOptions 
{
public:
	CFNBDatabaseOptions();
	void Reset();

	bool Read( int nDbIdx, bool bTemp );
	bool Write( int nDbIdx, bool bTemp );
	
	void CopyFrom( CFNBDatabaseOptions& source );

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

private:
	void ReloadFromIniFile( CEnhancedIniFile& file );
	void PrepareIniFile( CEnhancedIniFile& file );

public:
	bool GetEnable()						{ return m_Enable.GetValue(); }
	int GetLocSelType()						{ return m_LocSelType.GetValue(); }
	int GetLocSelNwkNo()					{ return m_LocSelNwkNo.GetValue(); }
	int GetLocSelEntityNo()					{ return m_LocSelEntityNo.GetValue(); }
	const char* GetFTPServer()				{ return m_FTPServer.GetValue(); }
	const char* GetFTPUserName()			{ return m_FTPUserName.GetValue(); }
	const char* GetFTPPassword()			{ return m_FTPPassword.GetValue(); }
	const char* GetFTPFolder()				{ return m_FTPFolder.GetValue(); }

	void SetEnable( bool b )				{ m_Enable.SetValue(b); }
	void SetLocSelType( int n )				{ m_LocSelType.SetValue(n); }
	void SetLocSelNwkNo( int n )			{ m_LocSelNwkNo.SetValue(n); }
	void SetLocSelEntityNo( int n )			{ m_LocSelEntityNo.SetValue(n); }
	void SetFTPServer( const char* sz )		{ m_FTPServer.SetValue(sz); }
	void SetFTPUserName( const char* sz )	{ m_FTPUserName.SetValue(sz); }
	void SetFTPPassword( const char* sz )	{ m_FTPPassword.SetValue(sz); }
	void SetFTPFolder( const char* sz )		{ m_FTPFolder.SetValue(sz); }

public:
	int ValidateSettings() { return 0; }
	const char* GetFilename( int nDbIdx, bool bTemp );

public:
	const char* GetLastExportDate( int nDbIdx, bool bTemp );
	void SetLastExportDate( int nDbIdx, COleDateTime& date, bool bTemp );
	void SetLastExportDate( int nDbIdx, const char* sz, bool bTemp );
	bool GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate );

private:
	const char* GetDateFilename( int nDbIdx, bool bTemp );
	
private:
	CIniFileBool m_Enable;
	CIniFileInt m_LocSelType;
	CIniFileInt m_LocSelNwkNo;
	CIniFileInt m_LocSelEntityNo;
	CIniFileString m_FTPServer;
	CIniFileString m_FTPUserName;
	CIniFileString m_FTPPassword;
	CIniFileString m_FTPFolder;

private:
	CString m_strLastExportDate;
	CString m_strFilename;
	CString m_strDateFilename;
		
public:
	bool m_bFatalReadError;
};

/**********************************************************************/

