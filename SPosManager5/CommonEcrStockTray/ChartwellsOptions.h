#pragma once
/**********************************************************************/
 #include "EnhancedIniFile.h"
/**********************************************************************/

class CChartwellsOptions 
{
public:
	CChartwellsOptions();
	void Reset();

	bool Read( bool bTemp );
	bool Write( bool bTemp );
	
	void CopyFrom( CChartwellsOptions& source );

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

private:
	void ReloadFromIniFile( CEnhancedIniFile& file );
	void PrepareIniFile( CEnhancedIniFile& file );

public:
	bool GetEnable()					{ return TRUE; } 
	CString GetFTPServer()				{ return m_FTPServer.GetValue(); }
	CString GetFTPUserName()			{ return m_FTPUserName.GetValue(); }
	CString GetFTPPassword()			{ return m_FTPPassword.GetValue(); }
	CString GetChartwellsSiteNo()		{ return m_ChartwellsSiteNo.GetValue(); }
	int GetSmartPaySerialNo()			{ return m_SmartPaySerialNo.GetValue(); }
	
	void SetFTPServer( CString str )		{ m_FTPServer.SetValue(str); }
	void SetFTPUserName( CString str)		{ m_FTPUserName.SetValue(str); }
	void SetFTPPassword(CString str)		{ m_FTPPassword.SetValue(str); }
	void SetChartwellsSiteNo(CString str)	{ m_ChartwellsSiteNo.SetValue(str); }
	void SetSmartPaySerialNo(int n)			{ m_SmartPaySerialNo.SetValue(n); }		
	
public:
	int ValidateSettings() { return 0; }
	const char* GetFilename( bool bTemp );

public:
	int GetMinSerialNo() { return 9000; }
	int GetMaxSerialNo() { return 999999; }
	int GetDefaultSerialNo() { return 9062; }	

public:
	const char* GetLastExportDate( bool bTemp );
	void SetLastExportDate( COleDateTime& date, bool bTemp );
	void SetLastExportDate( const char* sz, bool bTemp );
	bool GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate );

private:
	const char* GetDateFilename( bool bTemp );
	
private:
	CIniFileString m_FTPServer;
	CIniFileString m_FTPUserName;
	CIniFileString m_FTPPassword;
	CIniFileString m_ChartwellsSiteNo;
	CIniFileInt m_SmartPaySerialNo;

private:
	CString m_strLastExportDate;
	CString m_strFilename;
	CString m_strDateFilename;
		
public:
	bool m_bFatalReadError;
};

/**********************************************************************/
extern CChartwellsOptions ChartwellsOptions;
/**********************************************************************/
