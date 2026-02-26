#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "EposTaskDatabase.h"
#include "ReportConsolidationArray.h"
#include "SelectMultipleEntityInfo.h"
/**********************************************************************/
#define JOBLIST_LINETYPE_NETWORK 0
#define JOBLIST_LINETYPE_LOCATION 1
#define JOBLIST_LINETYPE_JOB 2
#define JOBLIST_LINETYPE_BLANK 3
/**********************************************************************/
#define JOBLIST_STATUS_NOTSTARTED 0
#define JOBLIST_STATUS_NOTFINISHED 1
#define JOBLIST_STATUS_FINISHED 2
#define JOBLIST_STATUS_FAILED 3
#define JOBLIST_STATUS_NODATA 4
/**********************************************************************/
#define JOBLIST_SETUP_ACCEPTED 0
#define JOBLIST_SETUP_NEED_COMMS 1
#define JOBLIST_SETUP_NEED_REALTIME 2
#define JOBLIST_SETUP_NEED_BATCH 3
#define JOBLIST_SETUP_NEED_DBLINK 4
#define JOBLIST_SETUP_UNKNOWNJOB 5
//#define JOBLIST_SETUP_NEEDVENTAPRO_V2 6
//#define JOBLIST_SETUP_NOTONVENTAPRO_V1 7
//#define JOBLIST_SETUP_NEEDVENTAPRO_V3 8
#define JOBLIST_SETUP_NEED_DBPRICESET 9
#define JOBLIST_SETUP_NEED_KBDLINK 10
#define JOBLIST_SETUP_NEED_IMMEDIATE 11
#define JOBLIST_SETUP_NEED_V4_992 12
#define JOBLIST_SETUP_LOC_DISABLE 14
#define JOBLIST_SETUP_REJECT_SPTBOOK 15
#define JOBLIST_SETUP_NEED_SPTBOOK 16
#define JOBLIST_SETUP_REJECT_SMARTENT 17
#define JOBLIST_SETUP_NEED_SMARTENT 18
#define JOBLIST_SETUP_NEED_V4_2606 19
#define JOBLIST_SETUP_NEED_FIRSTDB 20
#define JOBLIST_SETUP_NEED_V4_2_1530 21
#define JOBLIST_SETUP_NEED_V4_2_1579 22
/**********************************************************************/

struct CJobSetupInfo
{
	int m_nJobType;				//JOB TYPE AS SPECIFIED IN TASK
	int m_nExecuteJobType;		//ACTUAL JOB TYPE TO EXECUTE
	int m_nValidateJobType;		//JOB TYPE FOR VALIDATING TASK
	int m_nExtraJobType;		//KEYBOARD JOB FOR PRICE SET
	int m_nExtraParam1;			//RESET MACRO FLAG FOR PRICE SET KBD JOB
	int m_nParam1;
	int m_nNwkIdx;
	int m_nLocIdx;
	int m_nStatus;
};

/**********************************************************************/

class CJobListCSVRecord : public CCSVRecord
{
public:
	CJobListCSVRecord();
	virtual ~CJobListCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	void SetLineType( int nType ) { m_nLineType = nType; }
	void SetNetworkIdx( int nIdx ) { m_nNetworkIdx = nIdx; }
	void SetLocIdx( int nIdx ) { m_nLocIdx = nIdx; }
	void SetTerminalNo( int nNo ) { m_nTerminalNo = nNo; }
	void SetIsImmediateFlag( bool b ) { m_bIsImmediate = b; }
	void SetJobType( int nType ) { m_nJobType = nType; }
	void SetParam1( int nParam ) { m_nParam1 = nParam; }
	void SetStatus( int nStatus ) { m_nStatus = nStatus; }
	void SetAttempts( int nAttempts ) { m_nAttempts = nAttempts; }
	void IncAttempts() { m_nAttempts++; }
	void SetLocDownloadFlag( bool b ){ m_bLocDownload = b; }
	void SetRejectedRecordCount( int n ){ m_nRejectedRecords = n; }

public:
	int GetLineType() { return m_nLineType; }
	int GetNetworkIdx() { return m_nNetworkIdx; }
	int GetLocIdx() { return m_nLocIdx; }
	int GetTerminalNo() { return m_nTerminalNo; }
	bool GetIsImmediateFlag() { return m_bIsImmediate; }
	int GetJobType() { return m_nJobType; }
	int GetParam1() { return m_nParam1; }
	int GetStatus() { return m_nStatus; }
	bool CanRetryJob(){ return ( JOBLIST_STATUS_FINISHED != m_nStatus ) && ( JOBLIST_STATUS_NODATA != m_nStatus ); }
	int GetAttempts() { return m_nAttempts; }
	bool GetLocDownloadFlag(){ return m_bLocDownload; }
	int GetRejectedRecordCount(){ return m_nRejectedRecords; }

	int GetPriceSet();
	const char* GetJobName( int nJobType, int nParam1, int nExtraJobParam1 );
	const char* GetLogLine();
	const char* GetDownloadExclusionLogLine();
	
private:
	bool m_bIsImmediate;
	int m_nLineType;
	int m_nLocIdx;
	int m_nTerminalNo;
	int m_nNetworkIdx;
	int m_nJobType;
	int m_nParam1;
	int m_nStatus;
	int m_nAttempts;
	bool m_bLocDownload;
	int m_nRejectedRecords;

private:
	CString m_strJobName;
	CString m_strLogLine;
};

/**********************************************************************/

class CJobListCSVArray : public CCSVArray
{
public:
	CJobListCSVArray();

public:
	const char* GetDateFolder( CJobListCSVRecord& JobInfo );	
	void SetDate( const char* szDate ) { m_strDate = szDate; }

public:
	int BuildFromTaskRecord( CEposTaskDatabase& TaskDatabase, CString& strDate, CArray<CJobSetupInfo,CJobSetupInfo>& JobSetupInfo, int nDbIdx, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>* pDownloadMasters );
	void GetDownloadMasterList( CEposTaskDatabase& TaskDatabase, CReportConsolidationArray<CSortedIntAndInt>& arrayDownloadMasters );

public:
	void AddBlankLine();
	void AddNetworkLine( int nNetworkIdx );
	void AddLocationLine( int nNetworkIdx, int nLocIdx, int nTerminalNo, bool bIsImmediate );
	void AddJobLine( int nNetworkIdx, int nLocIdx, int nTerminalNo, int nJobType, int nParam1, bool bIsImmediate );
	const char* GetDisplayLine( int nIndex );

public:
	void SetLocDownloadFlag( int nIdx );

private:
	bool JobNeedsDbLink( int nJobType );
	bool IsLocationJob( int nJobType );

private:
	CString m_strDisplayLine;
	CString m_strDate;
};

/**********************************************************************/
#endif
/**********************************************************************/
