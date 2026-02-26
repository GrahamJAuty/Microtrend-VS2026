#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "EnhancedIniFile.h"
/**********************************************************************/
#define MAXLENGTH_S4LABOUR_LOCID 30
/**********************************************************************/
#define S4LABOUR_VALIDATE_OK 0
#define S4LABOUR_VALIDATE_NOID 1
#define S4LABOUR_VALIDATE_NODB 2
#define S4LABOUR_VALIDATE_DUPLICATEID 3
/**********************************************************************/

class CS4LabourLocationOptions 
{
public:
	CS4LabourLocationOptions();
	void Reset();

	bool Read( int nLocIdx, bool bTemp );
	bool Write( int nLocIdx, bool bTemp );
	
	void CopyFrom( CS4LabourLocationOptions& source );

	bool GetFatalReadErrorFlag() { return m_bFatalReadError; }
	void ClearFatalReadErrorFlag() { m_bFatalReadError = FALSE; }

public:
	void GetLocIdxList( int nDbIdx, CWordArray& arrayLocIdx );

private:
	void ReloadFromIniFile( CEnhancedIniFile& file );
	void PrepareIniFile( CEnhancedIniFile& file );

public:
	bool GetEnable()						{ return m_Enable.GetValue(); }
	const char* GetLocID()					{ return m_LocID.GetValue(); }
	bool GetUseTermIDFlag()					{ return m_UseTermID.GetValue(); }

public:
	void SetEnable( bool b )				{ m_Enable.SetValue(b); }
	void SetLocID( const char* sz )			{ m_LocID.SetValue(sz); }
	void SetUseTermIDFlag( bool b )			{ m_UseTermID.SetValue(b); }

public:
	int ValidateSettings( int nLocIdx );
	const char* GetFilename( int nLocIdx, bool bTemp );

public:
	const char* GetLastExportDate( int nLocIdx, bool bTemp );
	void SetLastExportDate( int nLocIdx, COleDateTime& date, bool bTemp );
	void SetLastExportDate( int nLocIdx, const char* sz, bool bTemp );
	bool GetNextExportTime( COleDateTime& oleNextDayToExport, COleDateTime& oleNextExportTime, CString& strLastExportDate );
	COleDateTime GetLatestExportableDate();

private:
	const char* GetDateFilename( int nLocIdx, bool bTemp );
	
private:
	CIniFileBool m_Enable;
	CIniFileString m_LocID;
	CIniFileBool m_UseTermID;

private:
	CString m_strLastExportDate;
	CString m_strFilename;
	CString m_strDateFilename;
		
public:
	bool m_bFatalReadError;
};

/**********************************************************************/

