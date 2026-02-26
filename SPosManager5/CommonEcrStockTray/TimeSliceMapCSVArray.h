#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SalesHistoryFields.h"
/**********************************************************************/
#define TIMESLICEMAP_VERSION 2
/**********************************************************************/

namespace TimeSliceMap
{
const recString		Name		= { "Name", 0, 32 };
const recInt		FileNo		= { "FileNo", 0, 10 };
};

/**********************************************************************/

class CTimeSliceMapCSVRecord : public CCSVRecord
{
public:
	CTimeSliceMapCSVRecord(); 
	virtual ~CTimeSliceMapCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void Reset() { ClearRecord(); }

	void V1ConvertFromCSV ( CCSV& csv ); 
	void V2ConvertFromCSV ( CCSV& csv );

	bool GetPreV2RecordFlag(){ return m_bPreV2Record; }
	
	const char* GetActualName(){ return m_strName; }
	int GetReportType() { return m_nReportType; }
	int GetTimeSliceFileNo() { return m_nTimeSliceFileNo; }
	const char* GetEntityID();
	
	void SetName ( const char* szName ); 
	void SetReportType ( int nType ) { m_nReportType = nType; }
	void SetTimeSliceFileNo( int n );
	void SetEntityID( const char* sz ){ m_strEntityID = sz; }
	
	int CreateCopy ( CTimeSliceMapCSVRecord& copy );
	
private:
	void ClearRecord();
	
public:
	const char* GetListName(); 
	bool CanBeCopied();
	
protected:
	CString m_strName;
	int m_nReportType;
	int m_nTimeSliceFileNo;
	CString m_strEntityID;
		
protected:
	CString m_strListName;
	CString m_strReportName;
	bool m_bPreV2Record;
};

/**********************************************************************/

class CTimeSliceMapCSVArray : public CSharedCSVArray
{
public:
	CTimeSliceMapCSVArray();
	int Open( const char* szFilename, int nMode );

	const char* GetName ( int nIndex );
	int GetReportType( int nIndex );
	int GetTimeSliceFileNo( int nIndex );
	const char* GetEntityID( int nIndex );
	int GetNextTimeSliceFileNo();

	bool AllowReport( int nReportType );
	void GetSystemReportList( CWordArray& array );

protected:	
	CString m_strName;
	CString m_strEntityID;
};

/**********************************************************************/
