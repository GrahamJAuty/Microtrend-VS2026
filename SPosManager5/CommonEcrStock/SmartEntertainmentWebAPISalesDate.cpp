/**********************************************************************/
//#include "globalfunctions.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "SmartEntertainmentWebAPISalesDate.h"
/**********************************************************************/

CSmartEntertainmentWebAPISalesDate::CSmartEntertainmentWebAPISalesDate(void)
{
	m_nNetworkIdx = -1;
	ResetDate();
}

/**********************************************************************/

void CSmartEntertainmentWebAPISalesDate::ResetDate()
{
	m_nDateStatus = SE_WEBDATE_NONE;
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	m_oleDateTimeNextRead = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );
}

/**********************************************************************/

bool CSmartEntertainmentWebAPISalesDate::CreateFile( int nNetworkIdx )
{
	bool bResult = FALSE;

	if ( ( nNetworkIdx >= 0 ) && ( nNetworkIdx < dbNetwork.GetSize() ) )
	{
		m_nNetworkIdx = nNetworkIdx;
		ResetDate();
		bResult = Write();
	}

	return bResult;
}

/**********************************************************************/

int CSmartEntertainmentWebAPISalesDate::Read( int nNetworkIdx )
{
	int nResult = SE_READDATES_ERROR;

	if ( ( nNetworkIdx >= 0 ) && ( nNetworkIdx < dbNetwork.GetSize() ) )
	{
		m_nNetworkIdx = nNetworkIdx;
		
		CFilenameUpdater FnUp( SysFiles::SmartEntertainmentDates, nNetworkIdx );
		CString strFilename = FnUp.GetFilenameToUse();

		if ( ::FileExists( strFilename ) == FALSE )
		{
			nResult = SE_READDATES_NOFILE;
		}
		else
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
					nResult = SE_READDATES_OK;
			}
		}
	}

	if ( nResult != SE_READDATES_OK )
		ResetDate();

	return nResult;
}

/**********************************************************************/

bool CSmartEntertainmentWebAPISalesDate::Write()
{
	bool bResult = FALSE;

	if ( m_nNetworkIdx != -1 )
	{
		CFilenameUpdater FnUp( SysFiles::SmartEntertainmentDates, m_nNetworkIdx );
		CString strFilename = FnUp.GetFilenameToUse();

		CCSV csv;
		csv.Add( 1 );
		csv.Add( m_nDateStatus );

		switch( m_nDateStatus )
		{
		case SE_WEBDATE_NEXT:
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

bool CSmartEntertainmentWebAPISalesDate::ReadV1Line( CCSV& csv )
{
	bool bResult = FALSE;

	int nStatus = csv.GetInt(1);
	
	switch( nStatus )
	{
	case SE_WEBDATE_NEXT:
		
		m_nDateStatus = nStatus;

		if ( StringToOleDateTime( csv.GetString(2), m_oleDateTimeNextRead ) == TRUE )
			bResult = TRUE;

		break;

	case SE_WEBDATE_NONE:
	default:
		m_nDateStatus = SE_WEBDATE_NONE;
		bResult = TRUE;
		break;
	}
	
	return bResult;
}

/**********************************************************************/

void CSmartEntertainmentWebAPISalesDate::GetNextReadDate( COleDateTime& time )
{
	time = m_oleDateTimeNextRead;
}

/**********************************************************************/

bool CSmartEntertainmentWebAPISalesDate::StringToOleDateTime( const char* sz, COleDateTime& datetime )
{
	CString str = sz;

	if ( str.GetLength() != 8 )
		return FALSE;

	if ( ::TestNumeric( str ) == FALSE )
		return FALSE;

	datetime = COleDateTime(
				atoi( str.Left(4) ),
				atoi( str.Mid(4,2) ),
				atoi( str.Right(2) ),
				0, 0, 0 );

	return ( datetime.m_status == COleDateTime::valid );
}

/**********************************************************************/

void CSmartEntertainmentWebAPISalesDate::OleDateTimeToString( COleDateTime& datetime, CString& str )
{
	str = "";
	if ( datetime.m_status == COleDateTime::valid )
	{
		str.Format( "%4.4d%2.2d%2.2d",
			datetime.GetYear(),
			datetime.GetMonth(),
			datetime.GetDay() );
	}
}

/**********************************************************************/

const char* CSmartEntertainmentWebAPISalesDate::GetWebAPIDateString()
{
	m_strWebAPIDateString = "";

	switch( m_nDateStatus )
	{
	case SE_WEBDATE_NEXT:
		{
			COleDateTime datetimeRead = m_oleDateTimeNextRead;
			
			m_strWebAPIDateString.Format( "%4.4d-%2.2d-%2.2d",
				datetimeRead.GetYear(),
				datetimeRead.GetMonth(),
				datetimeRead.GetDay() );
		}
		break;
	}
	
	return m_strWebAPIDateString;
}

/**********************************************************************/

void CSmartEntertainmentWebAPISalesDate::SetNextReadDate( COleDateTime& date )
{
	if ( date.m_status == COleDateTime::valid )
	{
		m_oleDateTimeNextRead = COleDateTime( date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0 );
		m_nDateStatus = SE_WEBDATE_NEXT;
	}
}

/**********************************************************************/

const char* CSmartEntertainmentWebAPISalesDate::GetDisplayDateNextRead()
{
	m_strDisplayDateNextRead = "";
	
	switch( m_nDateStatus )
	{
	case SE_WEBDATE_NEXT:
		if ( m_oleDateTimeNextRead.m_status == COleDateTime::valid )
		{
			COleDateTime datetimeRead = m_oleDateTimeNextRead;
			
			if ( SE_WEBDATE_NEXT == m_nDateStatus )
				datetimeRead += COleDateTimeSpan( 0, 0, 0, 1 );

			m_strDisplayDateNextRead.Format( "%2.2d/%2.2d/%4.4d",
				datetimeRead.GetDay(),
				datetimeRead.GetMonth(),
				datetimeRead.GetYear() );
		}
		break;

	case SE_WEBDATE_NONE:
	default:
		m_strDisplayDateNextRead = "<Not Set>";
		break;
	}

	return m_strDisplayDateNextRead;
}

/**********************************************************************/
