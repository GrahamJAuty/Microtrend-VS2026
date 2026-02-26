/**********************************************************************/
#include "TimeAttendReportNameCSVArray.h"
/**********************************************************************/

CTimeAttendReportNameCSVRecord::CTimeAttendReportNameCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CTimeAttendReportNameCSVRecord::ClearRecord()
{
	m_nReportNo = 1;
	m_strReportFilename = "";
}

/**********************************************************************/

void CTimeAttendReportNameCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetReportNo( csv.GetInt(0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:		V1ConvertFromCSV ( csv );			break;
	}
}

/**********************************************************************/

void CTimeAttendReportNameCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetReportFilename		( csv.GetString(2) );
}

/**********************************************************************/
	
void CTimeAttendReportNameCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nReportNo );
	csv.Add ( TIMEATTEND_REPORTNAME_VERSION );
	csv.Add ( m_strReportFilename );
}

/**********************************************************************/

void CTimeAttendReportNameCSVRecord::SetReportNo( int n )
{
	Set ( m_nReportNo, n, TimeAttendReportName::ReportNo );
}

/**********************************************************************/

void CTimeAttendReportNameCSVRecord::SetReportFilename ( const char* sz )
{
	CString strName = sz;
	::TrimSpaces ( strName, FALSE );
	Set( m_strReportFilename, strName, TimeAttendReportName::ReportName );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTimeAttendReportNameCSVArray::CTimeAttendReportNameCSVArray()
{
}

/**********************************************************************/

CTimeAttendReportNameCSVArray::~CTimeAttendReportNameCSVArray()
{
}

/**********************************************************************/

int CTimeAttendReportNameCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int n = 0;
	for ( ; ( n < TimeAttendReportName::ReportNo.Max ) && ( n < GetSize() ); n++)
	{
		CTimeAttendReportNameCSVRecord record;
		GetAt(n, record);

		if (record.GetReportNo() != n + 1)
		{
			break;
		}
	}

	if (n < GetSize())
	{
		RemoveAt(n, GetSize() - n);
	}

	for (; n < TimeAttendReportName::ReportNo.Max; n++)
	{
		CTimeAttendReportNameCSVRecord record;
		record.SetReportNo(n + 1);

		CString strFilename = "";
		strFilename.Format("TA%3.3d", n + 1);
		record.SetReportFilename(strFilename);
		Add(record);
	}

	return nReply;
}

/**********************************************************************/

bool CTimeAttendReportNameCSVArray::FindReportByNumber( int nReportNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		
		CTimeAttendReportNameCSVRecord record;
		GetAt ( nIndex, record );
		int nArrayNo = record.GetReportNo();
			
		if ( nReportNo == nArrayNo )
			return TRUE;

		if ( nReportNo < nArrayNo )
			nEnd = nIndex - 1;
		else
			nStart = nIndex + 1;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

