#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "ServerCSVArray.h"
/**********************************************************************/
#define SERVERRESTRICTION_VERSION 1
/**********************************************************************/

namespace ServerRestriction
{
	const recString		Restrictions =		{ "Restrictions", 0, 500 };
};

/**********************************************************************/

class CServerRestrictionCSVRecord : public CCSVRecord
{
public:
	CServerRestrictionCSVRecord();
	virtual ~CServerRestrictionCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	void ClearRecord();
	void SetServerNo ( int n );
	void SetTimeStamp ( const char* sz );
	void SetRestrictions( const char* sz );
	
public:
	int GetServerNo() { return m_nServerNo; }
	const char* GetTimeStamp() { return m_strTimeStamp; }
	const char* GetRestrictions(){ return m_strRestrictions; }
	
private:
	void V1ConvertFromCSV ( CCSV& csv );

private:
	int m_nServerNo;
	CString m_strTimeStamp;
	CString m_strRestrictions;
};

/**********************************************************************/

class CServerRestrictionCSVArray : public CSharedCSVArray
{
public:
	CServerRestrictionCSVArray();
	void RemoveAll();
	
public:
	int Open( const char* szFilename, int nMode );
	int GetCurrentLocIdx(){ return m_nCurrentLocIdx; }
	void SetCurrentLocIdx( int n ){ m_nCurrentLocIdx = n; }

public:
	void GetRestrictions( CServerCSVRecord& ServerRecord, CServerRestrictionCSVRecord& TerminalRecord );
	void SetRestrictions( CServerRestrictionCSVRecord& TerminalRecord );
	void PurgeDeletedServers();

private:
	bool FindServerByNumber( int nServerNo, int& nIndex );
	int GetServerNo( int nIndex );
	const char* GetTimeStamp( int nIndex );
		
private:
	int m_nCurrentLocIdx;
	CString m_strFilename;
	CString m_strTimeStamp;
};

/**********************************************************************/
