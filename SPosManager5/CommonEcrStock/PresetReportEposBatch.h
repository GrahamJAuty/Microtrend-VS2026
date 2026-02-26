#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#define PRESET_EPOS_BATCH_VERSION 3
/**********************************************************************/
#include "Consolidation.h"
#include "..\CommonEcrStock\PresetReportBase.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPresetReportInfoEposBatch
{
public:
	CPresetReportInfoEposBatch();
	void Reset();

public:
	void ToString( CString& str );
	void FromString( CString& str );

public:
	void ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase );
	void V1To3ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase, int nVer );
	void ConvertToCSV ( CCSV& csv, CPresetReportInfo& infoBase );

public:
	bool GetReportByTerminalFlag() { return m_bReportByTerminal; }
	bool GetReportByLocationFlag() { return m_bReportByLocation; }
	bool GetReportAllLocationsFlag() { return m_bReportAllLocations; }
	int GetActionType(){ return m_nActionType; }
	bool GetReportFlag( int nReportType );

public:
	void SetReportByTerminalFlag( bool bFlag ) { m_bReportByTerminal = bFlag; }
	void SetReportByLocationFlag( bool bFlag ) { m_bReportByLocation = bFlag; }
	void SetReportAllLocationsFlag( bool bFlag ) { m_bReportAllLocations = bFlag; }
	void SetActionType( int n );
	void SetReportFlag( int nReportType, bool bFlag );
		
private:
	bool m_bReportByTerminal; 
	bool m_bReportByLocation; 
	bool m_bReportAllLocations;  
	int m_nActionType;

private:
	CReportConsolidationArray<CSortedIntItem> m_arrayReports;
};

/**********************************************************************/

class CPresetReportArrayEposBatch : public CPresetReportArray
{
public:
	CPresetReportArrayEposBatch(void);
	~CPresetReportArrayEposBatch(void);

public:
	const char* GetFilename();

private:
	void PrepareForEditingInternal();
	void PrepareForWrite();

private:
	void CreateDefaultBatch( const char* szBatchName, CArray<int,int>& arrayReports );
};

/**********************************************************************/
#endif
/**********************************************************************/
