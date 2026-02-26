/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "SessionCSVArray.h"
/**********************************************************************/

CReportSessionInfo::CReportSessionInfo()
{
	Reset();
}

/**********************************************************************/

void CReportSessionInfo::Reset()
{
	m_bEOD = FALSE;
	m_bContinuous = FALSE;
	m_bSpansMidnight = FALSE;
	m_nHistoryDateType = 0;
	m_nSessionStartType = 0;
	m_nSessionStartHour = 0;
	m_nSessionStartMinute = 0;
	m_nSessionEndHour = 23;
	m_nSessionEndMinute = 59;
}

/**********************************************************************/

bool CReportSessionInfo::SpansMidnight()
{
	int nStart = ( m_nSessionStartHour * 60 ) + m_nSessionStartMinute;
	int nEnd = ( m_nSessionEndHour * 60 ) + m_nSessionEndMinute;
	return ( nEnd < nStart );
}

/**********************************************************************/

bool CReportSessionInfo::IsPreMidnight( COleDateTime& time )
{
	if ( time.GetHour() > m_nSessionStartHour )
		return TRUE;

	if ( time.GetHour() < m_nSessionStartHour )
		return FALSE;

	return ( time.GetMinute() >= m_nSessionStartMinute );
}

/**********************************************************************/

bool CReportSessionInfo::IsInSession( COleDateTime& time )
{
	int nStart = ( m_nSessionStartHour * 60 ) + m_nSessionStartMinute;
	int nEnd = ( m_nSessionEndHour * 60 ) + m_nSessionEndMinute;
	int nTime = ( time.GetHour() * 60 ) + time.GetMinute();
	return ( ( nTime >= nStart ) && ( nTime <= nEnd ) );
}

/**********************************************************************/

bool CReportSessionInfo::IsPostMidnight( COleDateTime& time )
{
	if ( time.GetHour() < m_nSessionEndHour )
		return TRUE;

	if ( time.GetHour() > m_nSessionEndHour )
		return FALSE;

	return ( time.GetMinute() <= m_nSessionEndMinute );
}

/**********************************************************************/

bool CReportSessionInfo::StartsBeforeSOD()
{
	int nStart = ( m_nSessionStartHour * 60 ) + m_nSessionStartMinute;
	int nSOD = ( EcrmanOptions.GetReportsDayStartHour() * 60 ) + EcrmanOptions.GetReportsDayStartMinute();
	return ( nStart < nSOD );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSessionCSVRecord::CSessionCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CSessionCSVRecord::ClearRecord()
{
	m_bCombinedSession = FALSE;
	m_arrayCombinedSessionIDs.RemoveAll();
	m_strName = "";
	m_strEntityID = "";
	m_bPreV3Record = FALSE;
	m_SessionInfo.Reset();
}

/**********************************************************************/

void CSessionCSVRecord::CopySession(CSessionCSVRecord& source)
{
	m_strName = source.m_strName;
	m_bCombinedSession = source.m_bCombinedSession;
	m_strEntityID = source.m_strEntityID;
	m_bPreV3Record = source.m_bPreV3Record;
	m_strReportName = source.m_strReportName;

	m_arrayCombinedSessionIDs.RemoveAll();
	for (int n = 0; n < source.m_arrayCombinedSessionIDs.GetSize(); n++)
	{
		m_arrayCombinedSessionIDs.Add(source.m_arrayCombinedSessionIDs.GetAt(n));
	}

	m_SessionInfo = source.m_SessionInfo;
}

/**********************************************************************/

void CSessionCSVRecord::GetSessionInfo( CReportSessionInfo& info )
{
	info = m_SessionInfo;
}

/**********************************************************************/

void CSessionCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();
	m_strName = csv.GetString (0);
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	case 2:	V2ConvertFromCSV ( csv );		break;
	case 3:	V3To4ConvertFromCSV ( csv, 3 );	break;
	case 4:	V3To4ConvertFromCSV ( csv, 4 );	break;
	case 5:	V5ConvertFromCSV ( csv );		break;
	}
}

/**********************************************************************/

void CSessionCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	m_bPreV3Record = TRUE;
	SetSessionStartHour( csv.GetInt(2) );
	SetSessionStartMinute( csv.GetInt(3) );
	SetSessionEndHour( csv.GetInt(4) );
	SetSessionEndMinute( csv.GetInt(5) );
}

/**********************************************************************/

void CSessionCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	m_bPreV3Record = TRUE;
	SetSessionStartHour( csv.GetInt(2) );
	SetSessionStartMinute( csv.GetInt(3) );
	SetSessionEndHour( csv.GetInt(4) );
	SetSessionEndMinute( csv.GetInt(5) );
	SetSessionStartType( csv.GetInt(6) );
	SetHistoryDateType( csv.GetInt(7) );

	if ( DealerFlags.GetSalesHistoryDateControlFlag() == FALSE )
		m_SessionInfo.m_nHistoryDateType = m_SessionInfo.m_nSessionStartType;
}

/**********************************************************************/

void CSessionCSVRecord::V3To4ConvertFromCSV ( CCSV& csv, int nVer )
{
	m_bPreV3Record = FALSE;
	SetEntityID( csv.GetString(2) );
	SetSessionStartHour( csv.GetInt(3) );
	SetSessionStartMinute( csv.GetInt(4) );
	SetSessionEndHour( csv.GetInt(5) );
	SetSessionEndMinute( csv.GetInt(6) );
	SetSessionStartType( csv.GetInt(7) );
	SetHistoryDateType( csv.GetInt(8) );

	if ( DealerFlags.GetSalesHistoryDateControlFlag() == FALSE )
		m_SessionInfo.m_nHistoryDateType = m_SessionInfo.m_nSessionStartType;

	if ( nVer >= 4 )
		SetContinuousFlag( csv.GetBool(9) );
}

/**********************************************************************/

void CSessionCSVRecord::V5ConvertFromCSV ( CCSV& csv )
{
	m_bPreV3Record = FALSE;
	SetEntityID( csv.GetString(2) );

	m_bCombinedSession = csv.GetBool(3);

	if ( TRUE == m_bCombinedSession )
	{
		SetSessionStartType( csv.GetInt(4) );
		
		int nCount = csv.GetSize() - 5;
		if ( nCount > 5 ) nCount = 5;

		for ( int n = 0; n < nCount; n++ )
			m_arrayCombinedSessionIDs.Add( csv.GetString( 5 + n ) );

		m_SessionInfo.m_nHistoryDateType = m_SessionInfo.m_nSessionStartType;
	}
	else
	{
		SetSessionStartHour( csv.GetInt(4) );
		SetSessionStartMinute( csv.GetInt(5) );
		SetSessionEndHour( csv.GetInt(6) );
		SetSessionEndMinute( csv.GetInt(7) );
		SetSessionStartType( csv.GetInt(8) );
		SetHistoryDateType( csv.GetInt(9) );
		SetContinuousFlag( csv.GetBool(10) );

		if ( DealerFlags.GetSalesHistoryDateControlFlag() == FALSE )
			m_SessionInfo.m_nHistoryDateType = m_SessionInfo.m_nSessionStartType;
	}
}

/**********************************************************************/

void CSessionCSVRecord::ConvertToCSV ( CCSV& csv )
{
	if ( DealerFlags.GetSalesHistoryDateControlFlag() == FALSE )
		m_SessionInfo.m_nHistoryDateType = m_SessionInfo.m_nSessionStartType;

	csv.Add ( m_strName );
	csv.Add ( SESSION_VERSION );
	csv.Add ( m_strEntityID );
	csv.Add ( m_bCombinedSession );

	if ( FALSE == m_bCombinedSession )
	{
		csv.Add ( m_SessionInfo.m_nSessionStartHour );
		csv.Add ( m_SessionInfo.m_nSessionStartMinute );
		csv.Add ( m_SessionInfo.m_nSessionEndHour );
		csv.Add ( m_SessionInfo.m_nSessionEndMinute );
		csv.Add ( m_SessionInfo.m_nSessionStartType );
		csv.Add ( m_SessionInfo.m_nHistoryDateType );
		csv.Add ( m_SessionInfo.m_bContinuous );
	}
	else
	{
		csv.Add ( m_SessionInfo.m_nSessionStartType );

		for ( int n = 0; ( n < m_arrayCombinedSessionIDs.GetSize() ) && ( n < 5 ); n++ )
			csv.Add( m_arrayCombinedSessionIDs.GetAt(n) );
	}
}

/**********************************************************************/
			
void CSessionCSVRecord::SetSessionStartHour( int nHour )
{
	if ( nHour >= 0 && nHour <= 23 ) m_SessionInfo.m_nSessionStartHour = nHour;
}

/**********************************************************************/
			
void CSessionCSVRecord::SetSessionStartMinute( int nMinute )
{
	if ( nMinute >= 0 && nMinute <= 59 ) m_SessionInfo.m_nSessionStartMinute = nMinute;
}

/**********************************************************************/
			
void CSessionCSVRecord::SetSessionEndHour( int nHour )
{
	if ( nHour >= 0 && nHour <= 23 ) m_SessionInfo.m_nSessionEndHour = nHour;
}

/**********************************************************************/
			
void CSessionCSVRecord::SetSessionEndMinute( int nMinute )
{
	if ( nMinute >= 0 && nMinute <= 59 ) m_SessionInfo.m_nSessionEndMinute = nMinute;
}

/**********************************************************************/

void CSessionCSVRecord::SetSessionStartType( int nType )
{
	if ( nType >= 0 && nType <= 1 ) m_SessionInfo.m_nSessionStartType = nType;
}

/**********************************************************************/

int CSessionCSVRecord::GetHistoryDateType()
{
	if ( DealerFlags.GetSalesHistoryDateControlFlag() == FALSE )
		return m_SessionInfo.m_nSessionStartType;
	else
		return m_SessionInfo.m_nHistoryDateType;
}

/**********************************************************************/

void CSessionCSVRecord::SetHistoryDateType( int nType )
{
	if ( DealerFlags.GetSalesHistoryDateControlFlag() == TRUE )
		if ( nType >= 0 && nType <= 3 ) 
			m_SessionInfo.m_nHistoryDateType = nType;
}

/**********************************************************************/

bool CSessionCSVRecord::SpansMidnight()
{
	return m_SessionInfo.SpansMidnight();
}

/**********************************************************************/

bool CSessionCSVRecord::IsPreMidnight( COleDateTime& time )
{
	return m_SessionInfo.IsPreMidnight( time );
}

/**********************************************************************/

bool CSessionCSVRecord::IsInSession( COleDateTime& time )
{
	return m_SessionInfo.IsInSession( time );
}

/**********************************************************************/

bool CSessionCSVRecord::IsPostMidnight( COleDateTime& time )
{
	return m_SessionInfo.IsPostMidnight( time );
}

/**********************************************************************/

bool CSessionCSVRecord::IsPreSOD( COleDateTime& time )
{
	if ( time.GetHour() < EcrmanOptions.GetReportsDayStartHour() )
		return TRUE;

	if ( time.GetHour() > EcrmanOptions.GetReportsDayStartHour() )
		return FALSE;

	return ( time.GetMinute() < EcrmanOptions.GetReportsDayStartMinute() );
}

/**********************************************************************/

bool CSessionCSVRecord::StartsBeforeSOD()
{
	return m_SessionInfo.StartsBeforeSOD();
}

/**********************************************************************/

const char* CSessionCSVRecord::GetName()
{
	if ( FALSE == m_SessionInfo.m_bEOD )
		m_strReportName = m_strName;
	else
		m_strReportName = "EOD Business Day";

	return m_strReportName;
}

/**********************************************************************/

void CSessionCSVRecord::GetCombinedSessionIDs( CStringArray& arrayIDs )
{
	arrayIDs.RemoveAll();
	for ( int n = 0; n < m_arrayCombinedSessionIDs.GetSize(); n++ )
		arrayIDs.Add( m_arrayCombinedSessionIDs.GetAt(n) );
}

/**********************************************************************/

void CSessionCSVRecord::SetCombinedSessionIDs( CStringArray& arrayIDs )
{
	m_arrayCombinedSessionIDs.RemoveAll();
	for ( int n = 0; n < arrayIDs.GetSize(); n++ )
		m_arrayCombinedSessionIDs.Add( arrayIDs.GetAt(n) );
}

/**********************************************************************/

void SetCombinedSessionIDs( CReportConsolidationArray<CSortedStringItem>& arrayIDs )
{
}

/**********************************************************************/

CSessionCSVArray::CSessionCSVArray() : CSharedCSVArray()
{
}

/**********************************************************************/

const char* CSessionCSVArray::GetName ( int nIndex )
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

const char* CSessionCSVArray::GetEntityID( int nIndex )
{
	m_strEntityID = "";

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CSessionCSVRecord Record;
		GetAt( nIndex, Record );
		m_strEntityID = Record.GetEntityID();
	}

	return m_strEntityID;
}

/**********************************************************************/

int CSessionCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );

	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	if ( GetSize() != 0 )
	{
		CSessionCSVRecord RecordZero;
		GetAt( 0, RecordZero );

		if ( RecordZero.GetPreV3RecordFlag() == TRUE )
		{
			for ( int n = 0; n < GetSize(); n++ )
			{
				CSessionCSVRecord Record;
				GetAt( n, Record );

				CString str;
				str.Format ( "%5.5X", 0x10000 + n );
				Record.SetEntityID( str );
				
				SetAt( n, Record );
			}
		}
	}

	if ( SysInfo.IsEcrLiteSystem() == TRUE )
		if ( GetSize() > 2 )
			RemoveAt( 2, GetSize() - 2 );

	return nReply;
}

/**********************************************************************/
