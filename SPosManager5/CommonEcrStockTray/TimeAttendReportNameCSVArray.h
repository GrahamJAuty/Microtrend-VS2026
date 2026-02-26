#pragma once
/**********************************************************************/
#define TIMEATTEND_REPORTNAME_VERSION 1
/**********************************************************************/

namespace TimeAttendReportName
{
	const recInt		ReportNo =				{ "ReportNo", 1, 5 };
	const recString		ReportName =			{ "ReportName", 0, 40 };
};

/**********************************************************************/

class CTimeAttendReportNameCSVRecord : public CCSVRecord
{
public:
	CTimeAttendReportNameCSVRecord();
	virtual ~CTimeAttendReportNameCSVRecord(){}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	int GetReportNo()					{ return m_nReportNo; }
	const char* GetReportFilename()		{ return m_strReportFilename; }
	
	void SetReportNo( int n );
	void SetReportFilename( const char* sz );
	
private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nReportNo;
	CString m_strReportFilename;
};

/**********************************************************************/

class CTimeAttendReportNameCSVArray : public CSharedCSVArray
{
public:
	CTimeAttendReportNameCSVArray();
	~CTimeAttendReportNameCSVArray();
	int Open( const char* szFilename, int nMode );
	
public:
	bool FindReportByNumber( int nReportNo, int& nReportIdx );
};

/**********************************************************************/
