#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "JobNumbers.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define TASK_TYPE_PRESET 0
#define TASK_TYPE_ADHOC 1		
/**********************************************************************/
#define IMPORT_IMMEDIATE 0
#define IMPORT_STARTOFDAY 1
#define IMPORT_SPECIFIED 2
/**********************************************************************/

struct CEposTaskHeader
{
public:
	CEposTaskHeader();
	void Reset();

public:
	int Compare ( CEposTaskHeader& source, int nHint = 0 );
	void Add	( CEposTaskHeader& source );
	
public:
	int GetTaskNo(){ return m_nTaskNo; }
	const char* GetName(){ return m_strName; }
	int GetHours(){ return m_nHours; }
	int GetMinutes(){ return m_nMinutes; }
	int GetImportType(){ return m_nImportType; }
	bool GetSelectLocationsWhenRunFlag(){ return m_bSelectLocationsWhenRun; } 

public:
	void SetTaskNo( int n );
	void SetName( const char* sz );
	void SetHours( int n );
	void SetMinutes( int n );
	void SetImportType( int n );
	void SetSelectLocationsWhenRunFlag( bool b ){ m_bSelectLocationsWhenRun = b; }

private:
	int m_nTaskNo;
	CString m_strName;
	int m_nHours;
	int m_nMinutes;
	int m_nImportType;
	bool m_bSelectLocationsWhenRun;
};

/**********************************************************************/

struct CEposJobLocation
{
public:
	CEposJobLocation();
	void Reset();

public:
	int Compare ( CEposJobLocation& source, int nHint = 0 );
	void Add	( CEposJobLocation& source );
	
public:
	int m_nNetWkLocNo;
	int m_nTerminalNo;
};

/**********************************************************************/

struct CEposTaskJob
{
public:
	CEposTaskJob();
	void Reset();

public:
	int Compare ( CEposTaskJob& source, int nHint = 0 );
	void Add	( CEposTaskJob& source );
	
public:
	int GetTaskNo(){ return m_nTaskNo; }
	int GetLineNo(){ return m_nLineNo; }
	int GetJobNo(){ return m_nJobNo; }
	int GetParam1BaseJob(){ return m_nParam1BaseJob; }
	int GetParam1ExtraJob(){ return m_nParam1ExtraJob; }
	int GetAllFlag(){ return m_bAll; }
	const char* GetLocations(){ return m_strLocations; }

public:
	void SetTaskNo( int n );
	void SetLineNo( int n );
	void SetJobNo( int n );
	void SetParam1BaseJob( int n );
	void SetParam1ExtraJob( int n );
	void SetAllFlag( bool b );
	void SetLocations( const char* szLocations );

public:
	bool IsRunAtNetwork( int nNetWkIdx );
	bool IsRunAtLocation( int nLocIdx, int nTerminalNo );
	void GetLocationArray( CReportConsolidationArray<CEposJobLocation>& arrayLocations );
	void SetLocationArray( CReportConsolidationArray<CEposJobLocation>& arrayLocations );
	const char* GetLocText();

private:
	int m_nTaskNo;
	int m_nLineNo;
	int m_nJobNo;
	int m_nParam1BaseJob;
	int m_nParam1ExtraJob;
	bool m_bAll;
	CString m_strLocations;

private:
	CString m_strLocText;
};

/**********************************************************************/

class CEposTaskDatabase
{
public:
	CEposTaskDatabase(void);

public:
	bool Read();
	bool Write();
	bool ReadPluChangesTask();
	bool WritePluChangesTask();
	void CleanUpTaskFiles();
	bool GetPluChangesMode(){ return m_bPluChangesMode; }

private:
	bool ReadInternal();
	bool WriteInternal();
	void SetPluChangesMode( bool b ){ m_bPluChangesMode = b; }
	const char* GetFilename();
	int GetMinTaskNo();
	int GetMaxTaskNo();
	int GetMaxTasks();

public:
	static int GetJobSortOrder( int nJobNo );
	void LoadEditableTask( CEposTaskDatabase& EditableTask, int nSourceTaskNo );
	void SaveEditableTask( CEposTaskDatabase& EditableTask );
	void GetEditableTaskHeader( CEposTaskHeader& Header );
	void SetEditableTaskHeader( CEposTaskHeader& Header );
	int GetEditableTaskJobCount();
	void GetEditableTaskJob( int nJobIdx, CEposTaskJob& Job );
	void SetEditableTaskJob( int nJobIdx, CEposTaskJob& Job );
	void AddEditableTaskJob( int nJobNo, int nParam1BaseJob = 0, bool bFromStart = FALSE, int nParam1ExtraJob = 0 );
	bool HasEditableTaskDownloadJob();
	void RemoveEditableTaskJob( int nJobIdx );
	void TidyEditableTaskJobs();
	
public:
	void CreateSimpleTask( int nJobNo, int nLocIdx, int nParam1 = 0 );
	void CreateSimpleTask( int nJobNo, CArray<int,int>& arrayLocIdx, int nParam1 = 0);
	void CreateSimpleTask( CWordArray& arrayJobNo, CArray<int,int>& arrayLocIdx, int nParam1 = 0);
	void CreateAdhocTask();

public:
	void GetLibraryTaskHeader( int nTaskNo, CEposTaskHeader& Header );
	int GetLibraryTaskJobCount( int nTaskNo );
	const char* GetLibraryTaskName( int nTaskNo );

private:
	void GetLibraryTaskJobIdxRange( int nTaskNo, int& nStartJobIdx, int& nEndJobIdx );
	
private:
	void ImportLegacyTaskLine( CCSV& csv, int nVer, int& nTaskCount );
	void ImportSPOSTaskLine( CCSV& csv, int nVer );
			
private:
	CReportConsolidationArray<CEposTaskHeader> m_arrayHeaders;
	CReportConsolidationArray<CEposTaskJob> m_arrayJobs;

private:
	CString m_strFilename;
	int m_nCurrentImportTaskNo;
	int m_nCurrentImportJobIdx;
	CString m_strTaskName;
	bool m_bPluChangesMode;
};

/**********************************************************************/
#endif
/**********************************************************************/
