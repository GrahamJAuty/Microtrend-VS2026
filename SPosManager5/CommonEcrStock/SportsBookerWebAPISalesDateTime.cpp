/**********************************************************************/
//#include "globalfunctions.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "SportsBookerWebAPISalesDateTime.h"
/**********************************************************************/

CSportsBookerWebAPISalesDateTime::CSportsBookerWebAPISalesDateTime(void)
{
	m_nNetworkIdx = -1;
	ResetDateTime();
}

/**********************************************************************/

void CSportsBookerWebAPISalesDateTime::ResetDateTime()
{
	m_nDateTimeStatus = SB_WEBDATE_NONE;
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_oleDateTimeNextRead = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );
}

/**********************************************************************/

bool CSportsBookerWebAPISalesDateTime::CreateFile( int nNetworkIdx )
{
	bool bResult = FALSE;

	if ( ( nNetworkIdx >= 0 ) && ( nNetworkIdx < dbNetwork.GetSize() ) )
	{
		m_nNetworkIdx = nNetworkIdx;
		ResetDateTime();
		bResult = Write();
	}

	return bResult;
}

/**********************************************************************/

int CSportsBookerWebAPISalesDateTime::Read( int nNetworkIdx )
{
	int nResult = SB_READDATES_ERROR;

	if ( ( nNetworkIdx >= 0 ) && ( nNetworkIdx < dbNetwork.GetSize() ) )
	{
		CFilenameUpdater FnUp( SysFiles::SportsBookerDates, nNetworkIdx );
		CString strFilename = FnUp.GetFilenameToUse();

		if ( ::FileExists( strFilename ) == TRUE )
		{
			CString strBuffer = "";
			
			{
				CSSFile fileDates;
				if ( fileDates.Open( strFilename, "rb" ) == TRUE )
				{
					fileDates.ReadString( strBuffer );
					fileDates.Close();
				}
			}

			if ( strBuffer != "" )
			{
				CCSV csv( strBuffer );

				bool bResult = FALSE;
				switch( csv.GetInt(0) )
				{
				case 1:	bResult = ReadV1Line( csv );	break;
				}

				if ( TRUE == bResult )
				{
					m_nNetworkIdx = nNetworkIdx;
					nResult = SB_READDATES_OK;
				}
			}
		}
		else
		{
			m_nNetworkIdx = nNetworkIdx;
			nResult = SB_READDATES_NOFILE;
			ResetDateTime();
		}
	}

	return nResult;
}

/**********************************************************************/

bool CSportsBookerWebAPISalesDateTime::Write()
{
	bool bResult = FALSE;

	if ( m_nNetworkIdx != -1 )
	{
		CFilenameUpdater FnUp( SysFiles::SportsBookerDates, m_nNetworkIdx );
		CString strFilename = FnUp.GetFilenameToUse();

		CCSV csv;
		csv.Add( 1 );
		csv.Add( m_nDateTimeStatus );

		switch( m_nDateTimeStatus )
		{
		case SB_WEBDATE_FIRST:
		case SB_WEBDATE_NEXT:
			{
				CString strDate;
				OleDateTimeToString( m_oleDateTimeNextRead, strDate );
				csv.Add( strDate );
			}
			break;
		}
		
		CSSFile fileDates;
		if ( fileDates.Open( strFilename, "wb" ) == TRUE )
		{
			fileDates.WriteLine( csv.GetLine() );
			bResult = TRUE;
		}
	}

	return bResult;
}

/**********************************************************************/

bool CSportsBookerWebAPISalesDateTime::ReadV1Line( CCSV& csv )
{
	bool bResult = FALSE;

	int nStatus = csv.GetInt(1);
	
	switch( nStatus )
	{
	case SB_WEBDATE_FIRST:
	case SB_WEBDATE_NEXT:
		
		m_nDateTimeStatus = nStatus;

		if ( StringToOleDateTime( csv.GetString(2), m_oleDateTimeNextRead ) == TRUE )
			bResult = TRUE;

		break;

	case SB_WEBDATE_NONE:

		m_nDateTimeStatus = nStatus;
		bResult = TRUE;

		break;

	default:
		m_nDateTimeStatus = SB_WEBDATE_NONE;
		break;
	}
	
	return bResult;
}

/**********************************************************************/

void CSportsBookerWebAPISalesDateTime::GetNextReadDateTime( COleDateTime& time )
{
	time = m_oleDateTimeNextRead;
}

/**********************************************************************/

void CSportsBookerWebAPISalesDateTime::SetFirstReadDateTime( COleDateTime& time )
{
	if ( SB_WEBDATE_NEXT != m_nDateTimeStatus )
	{
		if ( time.m_status == COleDateTime::valid )
		{
			m_oleDateTimeNextRead = time;
			m_nDateTimeStatus = SB_WEBDATE_FIRST;
		}
	}
}

/**********************************************************************/

bool CSportsBookerWebAPISalesDateTime::StringToOleDateTime( const char* sz, COleDateTime& datetime )
{
	CString str = sz;

	if ( str.GetLength() != 14 )
		return FALSE;

	if ( ::TestNumeric( str ) == FALSE )
		return FALSE;

	datetime = COleDateTime(
				atoi( str.Left(4) ),
				atoi( str.Mid(4,2) ),
				atoi( str.Mid(6,2) ),
				atoi( str.Mid(8,2) ),
				atoi( str.Mid(10,2) ),
				atoi( str.Right(2) ) );

	return ( datetime.m_status == COleDateTime::valid );
}

/**********************************************************************/

void CSportsBookerWebAPISalesDateTime::OleDateTimeToString( COleDateTime& datetime, CString& str )
{
	str = "";
	if ( datetime.m_status == COleDateTime::valid )
	{
		str.Format( "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
			datetime.GetYear(),
			datetime.GetMonth(),
			datetime.GetDay(),
			datetime.GetHour(),
			datetime.GetMinute(),
			datetime.GetSecond() );
	}
}

/**********************************************************************/

const char* CSportsBookerWebAPISalesDateTime::GetWebAPIDateTimeString()
{
	m_strWebAPIDateTimeString = "";

	switch( m_nDateTimeStatus )
	{
	case SB_WEBDATE_FIRST:
	case SB_WEBDATE_NEXT:
		{
			COleDateTime datetimeRead = m_oleDateTimeNextRead;
			
			if ( SB_WEBDATE_NEXT == m_nDateTimeStatus )
				datetimeRead += COleDateTimeSpan( 0, 0, 0, 1 );

			m_strWebAPIDateTimeString.Format( "%4.4d-%2.2d-%2.2d+%2.2d:%2.2d:%2.2d",
				datetimeRead.GetYear(),
				datetimeRead.GetMonth(),
				datetimeRead.GetDay(),
				datetimeRead.GetHour(),
				datetimeRead.GetMinute(),
				datetimeRead.GetSecond() );
		}
		break;
	}
	
	return m_strWebAPIDateTimeString;
}

/**********************************************************************/

void CSportsBookerWebAPISalesDateTime::ProcessXMLDateTime( const char* szDate )
{
	CString strWebDate = szDate;
	if ( strWebDate.GetLength() == 19 )
	{
		CString strOleDate = strWebDate.Left(4) + strWebDate.Mid(5,2) + strWebDate.Mid(8,2) + strWebDate.Mid(11,2) + strWebDate.Mid(14,2) + strWebDate.Right(2);

		COleDateTime oleDate;
		if ( StringToOleDateTime( strOleDate, oleDate ) == TRUE )
		{
			switch( m_nDateTimeStatus )
			{
			case SB_WEBDATE_NEXT:
				if ( oleDate > m_oleDateTimeNextRead )
					m_oleDateTimeNextRead = oleDate;
				break;

			case SB_WEBDATE_FIRST:
				m_oleDateTimeNextRead = oleDate;
				m_nDateTimeStatus = SB_WEBDATE_NEXT;
				break;
			}
		}
	}
}

/**********************************************************************/

const char* CSportsBookerWebAPISalesDateTime::GetDisplayDateTimeNextRead()
{
	m_strDisplayDateTimeNextRead = "";
	
	switch( m_nDateTimeStatus )
	{
	case SB_WEBDATE_FIRST:
	case SB_WEBDATE_NEXT:
		if ( m_oleDateTimeNextRead.m_status == COleDateTime::valid )
		{
			COleDateTime datetimeRead = m_oleDateTimeNextRead;
			
			if ( SB_WEBDATE_NEXT == m_nDateTimeStatus )
				datetimeRead += COleDateTimeSpan( 0, 0, 0, 1 );

			m_strDisplayDateTimeNextRead.Format( "%2.2d/%2.2d/%4.4d  at  %2.2d:%2.2d",
				datetimeRead.GetDay(),
				datetimeRead.GetMonth(),
				datetimeRead.GetYear(),
				datetimeRead.GetHour(),
				datetimeRead.GetMinute() );
		}
		break;

	case SB_WEBDATE_NONE:
	default:
		m_strDisplayDateTimeNextRead = "<Not Set>";
		break;
	}

	return m_strDisplayDateTimeNextRead;
}

/**********************************************************************/
