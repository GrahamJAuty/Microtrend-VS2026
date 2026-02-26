#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define SESSION_VERSION 5
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CReportSessionInfo
{
public:
	CReportSessionInfo();
	void Reset();

public:
	bool SpansMidnight();
	bool IsPreMidnight( COleDateTime& time );
	bool IsPostMidnight( COleDateTime& time );
	bool IsInSession( COleDateTime& time );
	bool StartsBeforeSOD();
	
public:
	bool m_bEOD;
	bool m_bContinuous;
	bool m_bSpansMidnight;
	int m_nHistoryDateType;
	int m_nSessionStartType;
	int m_nSessionStartHour;
	int m_nSessionStartMinute;
	int m_nSessionEndHour;
	int m_nSessionEndMinute;
	CSortedDateTimeItem m_SessionDateFrom;
	CSortedDateTimeItem m_SessionDateTo;
};

/**********************************************************************/

class CSessionCSVRecord : public CCSVRecord
{
public:
	CSessionCSVRecord();
	virtual ~CSessionCSVRecord() {}
	void ClearRecord();
	void CopySession(CSessionCSVRecord& source);

public:
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 

	void GetSessionInfo( CReportSessionInfo& info );

	bool GetPreV3RecordFlag(){ return m_bPreV3Record; }
	
	void SetName ( const char* szName ) { m_strName = szName; }
	const char* GetName();
	
	bool GetCombinedSessionFlag(){ return m_bCombinedSession; }
	void GetCombinedSessionIDs( CStringArray& arrayIDs );
	
	int GetSessionStartHour() { return m_SessionInfo.m_nSessionStartHour; }
	int GetSessionStartMinute() { return m_SessionInfo.m_nSessionStartMinute; }
	int GetSessionEndHour() { return m_SessionInfo.m_nSessionEndHour; }
	int GetSessionEndMinute() { return m_SessionInfo.m_nSessionEndMinute; }
	int GetSessionStartType() { return m_SessionInfo.m_nSessionStartType; }
	int GetHistoryDateType();
	const char* GetEntityID(){ return m_strEntityID; }
	bool GetEODFlag(){ return m_SessionInfo.m_bEOD; }
	bool GetContinuousFlag(){ return m_SessionInfo.m_bContinuous; }
	
	void SetCombinedSessionFlag( bool b){ m_bCombinedSession = b; }
	void SetCombinedSessionIDs( CStringArray& arrayIDs );
	
	void SetSessionStartHour( int nHour );
	void SetSessionStartMinute( int nMinute );
	void SetSessionEndHour( int nHour );
	void SetSessionEndMinute( int nMinute );
	void SetSessionStartType( int nType );
	void SetHistoryDateType( int nType );
	void SetEntityID( const char* sz ){ m_strEntityID = sz; }
	void SetEODFlag( bool bEOD ){ m_SessionInfo.m_bEOD = bEOD; }
	void SetContinuousFlag( bool b ){ m_SessionInfo.m_bContinuous = b; }

public:
	bool SpansMidnight();
	bool StartsBeforeSOD();
	bool IsPreMidnight( COleDateTime& time );
	bool IsPostMidnight( COleDateTime& time );
	bool IsInSession( COleDateTime& time );
	static bool IsPreSOD( COleDateTime& time );

private:
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3To4ConvertFromCSV( CCSV& csv, int nVer );
	void V5ConvertFromCSV( CCSV& csv );

private:
	CString m_strName; 
	bool m_bCombinedSession;
	CStringArray m_arrayCombinedSessionIDs;
	CReportSessionInfo m_SessionInfo;
	CString m_strEntityID;
	
private:
	bool m_bPreV3Record;
	CString m_strReportName;
};

/**********************************************************************/

class CSessionCSVArray : public CSharedCSVArray
{
public:
	CSessionCSVArray();
	const char* GetName ( int nIndex );
	const char* GetEntityID( int nIndex );

public:
	int Open( const char* szFilename, int nMode );

private:
	CString m_strName;
	CString m_strEntityID;
};

/**********************************************************************/
