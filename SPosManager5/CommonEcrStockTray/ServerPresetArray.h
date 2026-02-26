#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define SVRPRESETFILE_SYSTEM 0
#define SVRPRESETFILE_DATABASE 1
#define SVRPRESETFILE_LOCATION 2
#define SVRPRESETFILE_TERMINAL 3
/**********************************************************************/
#define MAX_SVRPRESETNO 50
/**********************************************************************/

struct CServerPresetMiniInfo
{
public:
	int m_nType;
	int m_nPresetNo;
	int m_nServerNo;
};

/**********************************************************************/

struct CServerPresetInfo
{
public:
	CServerPresetInfo();
	void Reset();

public:
	int GetPresetNo(){ return m_nPresetNo; }
	//SYSTEM
	int GetServerNoSystem(){ return m_nServerNoSystem; }
	//DATABASE
	int GetServerNoDatabase(){ return m_nServerNoDatabase; }
	//LOCATION
	int GetServerNoLocation(){ return m_nServerNoLocation; }
	//TERMINAL
	int GetServerNoTerminal(){ return m_nServerNoTerminal; }

public:
	void SetPresetNo( int n );
	//SYSTEM
	void SetServerNoSystem( int n );
	//DATABASE
	void SetServerNoDatabase( int n );
	//LOCATION
	void SetServerNoLocation( int n );
	//TERMINAL
	void SetServerNoTerminal( int n );
	
public:
	int Compare ( CServerPresetInfo& source, int nHint = 0 );
	void Add	( CServerPresetInfo& source );

private:
	int m_nPresetNo;
	//SYSTEM
	int m_nServerNoSystem;
	//DATABASE
	int m_nServerNoDatabase;
	//LOCATION
	int m_nServerNoLocation;
	//TERMINAL
	int m_nServerNoTerminal;
};

/**********************************************************************/

class CServerPresetTable
{
public:
	CServerPresetTable(void);
	void ResetPresetList( int nPresetMode );

public:
	int GetSize(){ return m_arrayPresets.GetSize(); }
	bool GetPreset( int nPresetIdx, CServerPresetMiniInfo& infoMini );
	bool GetPreset( CServerPresetMiniInfo& infoMini );

public:
	void UpdatePreset( CServerPresetMiniInfo& infoMini );
	void UpdatePreset( int nPresetIdx, CServerPresetMiniInfo& infoMini );

private:
	void AddPreset( int nPresetIdx );

private:
	CReportConsolidationArray<CServerPresetInfo> m_arrayPresets;
};

/**********************************************************************/

class CServerPresetArray : public CSharedStringArray
{
public:
	CServerPresetArray( int nPresetMode, CServerPresetTable& PresetTable );

public:
	int Open( const char* szFilename, int nMode );
	void PrepareForWrite();

public:
	int GetPresetCount(){ return m_PresetTable.GetSize(); }
	void GetPreset( int nPresetIdx, CServerPresetMiniInfo& infoMini );
	void GetPreset( CServerPresetMiniInfo& infoMini );
	void UpdatePreset( int nPresetIdx, CServerPresetMiniInfo& infoMini );
	void UpdatePreset( CServerPresetMiniInfo& infoMini );
	void ResetPresetList();

private:
	int m_nPresetMode;
	CServerPresetTable& m_PresetTable;
};

/**********************************************************************/
