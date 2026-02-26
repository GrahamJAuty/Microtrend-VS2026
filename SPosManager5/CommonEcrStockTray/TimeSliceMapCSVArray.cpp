/**********************************************************************/
//#include "DataManagerNonDb.h"
/**********************************************************************/
#include "TimeSliceMapCSVArray.h"
/**********************************************************************/

CTimeSliceMapCSVRecord::CTimeSliceMapCSVRecord()
{
	ClearRecord();	
}

/**********************************************************************/

void CTimeSliceMapCSVRecord::ClearRecord()
{
	m_strName = "";
	m_nReportType = SH_TIMESLICE_SECOND;
	m_nTimeSliceFileNo = 0;
	m_strEntityID = "";
	m_bPreV2Record = FALSE;
}

/**********************************************************************/

void CTimeSliceMapCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	SetName ( csv.GetString (0) );
	SetReportType ( csv.GetInt(1) ); 
	SetTimeSliceFileNo( csv.GetInt(2) );

	switch( csv.GetInt(3) )
	{
	case 1:		V1ConvertFromCSV( csv );	break;
	case 2:		V2ConvertFromCSV( csv );	break;
	default:	m_bPreV2Record = TRUE;		break;
	}
}

/**********************************************************************/

void CTimeSliceMapCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	m_bPreV2Record = TRUE;
}

/**********************************************************************/

void CTimeSliceMapCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	m_bPreV2Record = FALSE;
	SetEntityID( csv.GetString(4) );
}

/**********************************************************************/

void CTimeSliceMapCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strName );
	csv.Add ( m_nReportType );
	csv.Add ( m_nTimeSliceFileNo );
	csv.Add ( TIMESLICEMAP_VERSION );
	csv.Add ( m_strEntityID );
}

/**********************************************************************/

void CTimeSliceMapCSVRecord::SetName( const char* sz )
{
	Set( m_strName, sz, TimeSliceMap::Name );
}

/**********************************************************************/

void CTimeSliceMapCSVRecord::SetTimeSliceFileNo( int n )
{
	Set( m_nTimeSliceFileNo, n, TimeSliceMap::FileNo );
}

/**********************************************************************/

const char* CTimeSliceMapCSVRecord::GetListName()
{
	switch( m_nReportType )
	{
	case SH_CUSTOM:				
		m_strListName = m_strName;			
		break;
	
	case SH_TIMESLICE_SECOND:	
		m_strListName = "Consolidate by second";			
		break;

	case SH_TIMESLICE_MINUTE:	
		m_strListName = "Consolidate by minute";			
		break;

	case SH_TIMESLICE_5MIN:	
		m_strListName = "Consolidate by 5 minute period";			
		break;

	case SH_TIMESLICE_10MIN:	
		m_strListName = "Consolidate by 10 minute period";			
		break;

	case SH_TIMESLICE_15MIN:	
		m_strListName = "Consolidate by quarter hour";			
		break;

	case SH_TIMESLICE_30MIN:	
		m_strListName = "Consolidate by half hour";			
		break;

	case SH_TIMESLICE_60MIN:	
		m_strListName = "Consolidate by hour";			
		break;

	default:					
		m_strListName = "Unknown";			
		break;
	}
	return m_strListName;
}

/**********************************************************************/

bool CTimeSliceMapCSVRecord::CanBeCopied()
{
	bool bResult = FALSE;

	switch ( GetReportType() )
	{
	case SH_CUSTOM:
		bResult = TRUE;
		break;
	}

	return bResult;
}

/**********************************************************************/

int CTimeSliceMapCSVRecord::CreateCopy( CTimeSliceMapCSVRecord& copy )
{
	if ( CanBeCopied() == FALSE )
		return 1;

	int nNewFileNo = DataManagerNonDb.TimeSliceList.GetNextTimeSliceFileNo();
	if ( nNewFileNo == 0 )
		return 2;

	CFilenameUpdater FnUp1(SysFiles::TimeSliceDetail, -1, m_nTimeSliceFileNo );
	CString strFilename1 = FnUp1.GetFilenameToUse();

	CFilenameUpdater FnUp2(SysFiles::TimeSliceDetail, -1, nNewFileNo );
	CString strFilename2 = FnUp2.GetFilenameToUse();

	if ( ::CopyFile( strFilename1, strFilename2, FALSE ) == FALSE )
		return 3;

	copy.ClearRecord();
	copy.SetReportType( SH_CUSTOM );
	copy.SetName( GetListName() );
	copy.SetTimeSliceFileNo( nNewFileNo );

	return 0;
}

/**********************************************************************/

const char* CTimeSliceMapCSVRecord::GetEntityID()
{
	if ( SH_CUSTOM != m_nReportType )
		m_strEntityID.Format( "%4.4X", m_nReportType );

	return m_strEntityID;
}

/**********************************************************************/

CTimeSliceMapCSVArray::CTimeSliceMapCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CTimeSliceMapCSVArray::GetName ( int nIndex )
{
	m_strName = "";
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strName = csv.GetString ( 0 );
	}

	return m_strName;
}

/**********************************************************************/

int CTimeSliceMapCSVArray::GetReportType ( int nIndex )
{
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		return csv.GetInt ( 1 );
	}

	return 0;
}

/**********************************************************************/

int CTimeSliceMapCSVArray::GetTimeSliceFileNo ( int nIndex )
{
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 3 );
		return csv.GetInt ( 2 );
	}

	return 0;
}

/**********************************************************************/

const char* CTimeSliceMapCSVArray::GetEntityID( int nIndex )
{
	m_strEntityID = "";

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CTimeSliceMapCSVRecord Record;
		GetAt( nIndex, Record );
		m_strEntityID = Record.GetEntityID();
	}

	return m_strEntityID;
}

/**********************************************************************/

int CTimeSliceMapCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	if ( GetSize() == 0 ) 
	{
		CWordArray ReportList;
		GetSystemReportList( ReportList );

		for ( int n = 0; n < ReportList.GetSize(); n++ )
		{
			CTimeSliceMapCSVRecord Record;
			Record.SetReportType( ReportList.GetAt(n) );
			Add( Record );
		}
	}

	for ( int n = GetSize() - 1; n >= 0; n-- )
		if ( AllowReport(GetReportType(n)) == FALSE )
			RemoveAt(n);

	if ( GetSize() != 0 )
	{
		CTimeSliceMapCSVRecord RecordZero;
		GetAt( 0, RecordZero );

		if ( RecordZero.GetPreV2RecordFlag() == TRUE )
		{
			for ( int n = 0; n < GetSize(); n++ )
			{
				CTimeSliceMapCSVRecord Record;
				GetAt( n, Record );

				if ( Record.GetReportType() == SH_CUSTOM )
				{
					CString str;
					str.Format ( "%5.5X", 0x10000 + n );
					Record.SetEntityID( str );
				}

				SetAt( n, Record );
			}
		}
	}

	return nReply;
}

/**********************************************************************/

bool CTimeSliceMapCSVArray::AllowReport( int nReportType )
{
	switch ( nReportType )
	{
	case SH_CUSTOM:
	case SH_TIMESLICE_SECOND:
	case SH_TIMESLICE_MINUTE:
	case SH_TIMESLICE_5MIN:
	case SH_TIMESLICE_10MIN:
	case SH_TIMESLICE_15MIN:
	case SH_TIMESLICE_30MIN:
	case SH_TIMESLICE_60MIN:
		return TRUE;

	default:
		return FALSE;
	}
}

/**********************************************************************/

void CTimeSliceMapCSVArray::GetSystemReportList( CWordArray& array )
{
	array.RemoveAll();
	array.Add ( SH_TIMESLICE_SECOND );
	array.Add ( SH_TIMESLICE_MINUTE );
	array.Add ( SH_TIMESLICE_5MIN );
	array.Add ( SH_TIMESLICE_10MIN );
	array.Add ( SH_TIMESLICE_15MIN );
	array.Add ( SH_TIMESLICE_30MIN );
	array.Add ( SH_TIMESLICE_60MIN );
}

/**********************************************************************/

int CTimeSliceMapCSVArray::GetNextTimeSliceFileNo()
{
	CArray<bool,bool> arrayAvailable;
	
	for ( int n = 0; n <= TimeSliceMap::FileNo.Max; n++ )
		arrayAvailable.Add( TRUE );

	for ( int n = 0; n < GetSize(); n++ )
	{
		CTimeSliceMapCSVRecord TimeSlice;
		GetAt( n, TimeSlice );

		if ( TimeSlice.GetReportType() == SH_CUSTOM )
		{
			int nFileNo = TimeSlice.GetTimeSliceFileNo();

			if ( ( nFileNo >= 1 ) && ( nFileNo <= TimeSliceMap::FileNo.Max ) )
				arrayAvailable[nFileNo] = FALSE;
		}
	}

	for ( int n = 1; n <= TimeSliceMap::FileNo.Max; n++ )
		if ( arrayAvailable[n] == TRUE )
			return n;

	return 0;
}

/**********************************************************************/
