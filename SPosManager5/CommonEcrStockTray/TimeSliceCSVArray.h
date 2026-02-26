#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define TIMESLICE_VERSION 1
/**********************************************************************/

namespace TimeSlice
{
const recString		Name		= { "Name", 0, 32 };
const recInt		Start		= { "Start", 0, 1439 };
const recInt		End			= { "End", 0, 1439 };
const recInt		SortOrder	= { "SortOrder", 0, 100 };
};

/**********************************************************************/

class CTimeSliceCSVRecord : public CCSVRecord
{
public:
	CTimeSliceCSVRecord(); 
	virtual ~CTimeSliceCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	void Reset() { ClearRecord(); }
	
	const char* GetName(){ return m_strName; }
	int GetStart(){ return m_nStart; }
	int GetEnd(){ return m_nEnd; }
	int GetSortOrder(){ return m_nSortOrder; }
	
	void SetName ( const char* szName );
	void SetStart( int n );
	void SetEnd( int n );
	void SetSortOrder( int n );

private:
	void ClearRecord();
		
protected:
	CString m_strName;
	int m_nStart;
	int m_nEnd;
	int m_nSortOrder;
};

/**********************************************************************/

class CTimeSliceCSVArray : public CSharedCSVArray
{
public:
	CTimeSliceCSVArray();
	int Open( int nFileNo, const char* szFilename, int nMode );

	const char* GetName ( int nIndex );
	int GetStart( int nIndex );
	int GetEnd( int nIndex );
	int GetSortOrder( int nIndex );

	void CreateHourlyMap();
	void CreateHalfHourlyMap();
	void CreateQuarterHourlyMap();
	void CreateReportSessionMap();

	void SortTimeSlices();

protected:	
	CString m_strName;
};

/**********************************************************************/
