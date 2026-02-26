#pragma once
/**********************************************************************/
#define SERVERLOGON_VERSION 1
/**********************************************************************/
#include "Consolidation.h"
#include "MinimumSPOSVersion.h"
#include "NamespaceServer.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#define MAX_SERVERNO_LEN 4
/**********************************************************************/

class CServerLogonCSVRecord : public CCSVRecord
{
public:
	CServerLogonCSVRecord();
	virtual ~CServerLogonCSVRecord() {}
	virtual void ConvertToCSV(CCSV& csv);
	virtual void ConvertFromCSV(CCSV& csv);

public:
	__int64 GetLogonNo() { return m_nLogonNo; }
	int GetServerNo() { return m_nServerNo; }
	
	void SetLogonNo(__int64 n);
	void SetServerNo(int n);
	
public:
	void CheckMinimumSPOSVersion(CMinimumSPOSVersion& version);

private:
	void ClearRecord();
	void V1ConvertFromCSV(CCSV& csv);
	
private:
	__int64 m_nLogonNo;
	int m_nServerNo;
};

/**********************************************************************/

class CServerLogonCSVArray : public CSharedCSVArray
{
public:
	CServerLogonCSVArray();
	~CServerLogonCSVArray();

public:
	int Open( const char* szFilename, int nMode );

public:
	__int64 GetLogonNo( int nIndex );

public:
	bool FindLogonByNumber( __int64 nLogonNo, int& nLogonIdx );
	
public:
	void GetLogonListForServer(int nServerNo, CArray<__int64,__int64>& arrayLogon);
	void SetLogonListForServer(int nServerNo, CArray<__int64,__int64>& arrayLogon);
};

/**********************************************************************/
