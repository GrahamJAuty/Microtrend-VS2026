 /**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "ReportTypes.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EposReportEODStatus.h"
/**********************************************************************/

CEposReportEODStatus::CEposReportEODStatus( CEposSelectArray& SelectArray ) : CEposReport( SelectArray )
{
	m_nReportType = REPORT_TYPE_EODSTATUS;
	SetPMSAllowMode( PMS_ALLOWMODE_NONE );
	m_bGuestAccountWarn = FALSE;
}

/**********************************************************************/

void CEposReportEODStatus::Consolidate()
{
	m_SelectArray.MakeList( TRUE, TRUE );

	__int64 nTarget = 0;
	__int64 nProgress = 0;

	for ( int n = 0; n < m_SelectArray.GetListSize(); n++ )
	{
		CEposSelectArrayListItem item;
		m_SelectArray.GetListItem( n, item );

		CFilenameUpdater FnUp( SysFiles::EODDates, item.m_nLocIdx );

		if ( dbLocation.IsPMSPseudoLocation( item.m_nLocIdx ) )
			m_bGuestAccountWarn = TRUE;
		else
		{
			CSSFile fileEOD;
			fileEOD.Open( FnUp.GetFilenameToUse(), "rb" );
			nTarget += fileEOD.GetLength();
		}
	}

	StatusProgress.Lock( TRUE, "Checking EOD Dates" );

	for ( int n = 0; n < m_SelectArray.GetListSize(); n++ )
	{
		CEposSelectArrayListItem item;
		m_SelectArray.GetListItem( n, item );

		if ( dbLocation.IsPMSPseudoLocation( item.m_nLocIdx ) )
			continue;

		CFilenameUpdater FnUp( SysFiles::EODDates, item.m_nLocIdx );

		CSSFile fileEOD;
		if ( fileEOD.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
		{
			CString strBuffer;
			while( fileEOD.ReadString( strBuffer ) == TRUE )
			{
				StatusProgress.SetPos( nProgress + fileEOD.Tell(), nTarget );

				switch( strBuffer.GetLength() )
				{
				case 15:
				case 24:
				case 35:
					break;

				default:
					continue;
				}

				COleDateTime timeTest = COleDateTime(
					atoi( strBuffer.Left(4) ),
					atoi( strBuffer.Mid(4,2) ),
					atoi( strBuffer.Mid(6,2) ),
					0, 0, 0 );

				if ( timeTest.m_status != COleDateTime::valid )
					continue;

				if ( DataManagerNonDb.SessionDateTimeFilter.CheckTime( strBuffer.Left(8), strBuffer.Mid(9,6)  ) == 0 )
					continue;

				CEposReportConsolEODStatus info;
				info.m_nLocIdx = item.m_nLocIdx;
				info.m_nEODCount = 1;
				info.m_strLatestEOD = strBuffer.Left(15);
				m_arrayEODStatus.Consolidate( info );
			}

			nProgress += fileEOD.GetLength();
			fileEOD.Close();
		}
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

bool CEposReportEODStatus::CreateReport()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == FALSE )
		return FALSE;

	bool bWrittenLine = FALSE;

	m_ReportFile.SetStyle1 ( GetReportTitle(FALSE) );
	m_ReportFile.AddColumn ( "Location", TA_LEFT, 600 );
	m_ReportFile.AddColumn ( "EOD Count", TA_LEFT, 300 );
	m_ReportFile.AddColumn ( "Latest EOD", TA_LEFT, 600 );

	for ( int n = 0; n < m_SelectArray.GetListSize(); n++ )
	{
		CEposSelectArrayListItem item;
		m_SelectArray.GetListItem( n, item );

		if (dbLocation.IsPMSPseudoLocation(item.m_nLocIdx))
		{
			continue;
		}

		CCSV csv( '\t' );
		csv.Add( dbLocation.GetName( item.m_nLocIdx ) );

		int nPos;
		CEposReportConsolEODStatus info;
		info.m_nLocIdx = item.m_nLocIdx;
		if ( m_arrayEODStatus.Find( info, nPos ) == TRUE )
		{
			m_arrayEODStatus.GetAt( nPos, info );
			csv.Add( info.m_nEODCount );

			CString strDateTime;
			strDateTime.Format( "%s/%s/%s at %s:%s",
				(const char*) info.m_strLatestEOD.Mid(6,2),
				(const char*) info.m_strLatestEOD.Mid(4,2),
				(const char*) info.m_strLatestEOD.Left(4),
				(const char*) info.m_strLatestEOD.Mid(9,2),
				(const char*) info.m_strLatestEOD.Mid(11,2) );

			csv.Add( strDateTime );
		}

		m_ReportFile.WriteReportDataLine( csv.GetLine() );
		m_ReportFile.WriteReportDataLine( "<LI>" );
		bWrittenLine = TRUE;
	}

	if ( TRUE == m_bGuestAccountWarn )
	{
		m_ReportFile.WriteReportMiscLine( "" );
		m_ReportFile.WriteReportMiscLine( "<..>(This report is not available for guest accounts)" );
		bWrittenLine = TRUE;
	}

	if (FALSE == bWrittenLine)
	{
		m_ReportFile.WriteReportMiscLine("");
	}
	
	m_ReportFile.Close();
	return TRUE;
}

/**********************************************************************/