#pragma once
/**********************************************************************/

struct CEposSelectArrayFeederLocInfo
{
public:
	CString m_strSelected;
	CString m_strOriginal;
};

/**********************************************************************/

struct CEposSelectArrayFeederDbInfo
{
public:
	bool m_bImport;
	bool m_bManual;
};

/**********************************************************************/

class CEposSelectArrayFeeder
{
public:
	CEposSelectArrayFeeder();
	void RemoveAll();

public:
	void SelectTerminal( int nLocIdx, int nTermIdx );
	void SelectLocation( int nLocIdx );
	void SelectDatabase( int nDbIdx );
	void SelectSystem();

public:
	const char* GetLocationString( int nLocIdx );
	bool SetLocationString( int nLocIdx, const char* sz );

public:
	void SelectDatabaseForNonEposSales( int nDbIdx, bool bManual, bool bImport );

public:
	void TagOriginalSelectionStatus( bool bSelectAllTerminals );
	bool GetOriginalSelectionStatus( int nLocIdx, int nTermIdx );
	bool GetSelectionStatus( int nLocIdx, int nTermIdx );

public:
	bool IsEmpty(){ return m_bIsEmpty; }
	int GetLocCount(){ return m_arrayLocations.GetSize(); }
	void GetTermIdxArray( int nLocIdx, CArray<int,int>& arrayTermIdx );
	bool GotTerminals( int nLocIdx );
	bool GetDbImportFlag( int nDbIdx );
	bool GetDbManualFlag( int nDbIdx );

private:
	bool ValidateLocIdx( int nLocIdx );
	bool ValidateDbIdx( int nDbIdx );

private:
	bool m_bIsEmpty;
	CArray<CEposSelectArrayFeederLocInfo,CEposSelectArrayFeederLocInfo> m_arrayLocations;
	CArray<CEposSelectArrayFeederDbInfo,CEposSelectArrayFeederDbInfo> m_arrayDatabases;

private:
	CString m_strLocation;
};

/**********************************************************************/
