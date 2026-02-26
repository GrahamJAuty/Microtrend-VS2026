/**********************************************************************/
#include "Password.h"
/**********************************************************************/
#include "FileWriteLog.h"
/**********************************************************************/

CFileWriteLog::CFileWriteLog()
{
	if ( m_ReportFile.Open ( Super.ReportFilename() ) == TRUE )
	{
		m_ReportFile.SetStyle1 ( "File Changes Log" );
		m_ReportFile.AddColumn ( "User", TA_LEFT, 300 );
		m_ReportFile.AddColumn ( "Date", TA_LEFT, 300 );
		m_ReportFile.AddColumn ( "Time", TA_LEFT, 300 );
		m_ReportFile.AddColumn ( "Filename", TA_LEFT, 1000 );
		m_ReportFile.AddColumn ( "Result", TA_LEFT, 300 );
		
		CStringArray arrayFiles;
		arrayFiles.Add( Super.FileWriteLogOld() );
		arrayFiles.Add( Super.FileWriteLogNew() );

		for ( int n = 0; n < arrayFiles.GetSize(); n++ )
		{
			CSSFile fileLog;
			if ( fileLog.Open( arrayFiles.GetAt(n), "rb" ) == TRUE )
			{
				CString strBuffer;
				while( fileLog.ReadString( strBuffer ) == TRUE )
				{
					CCSV csvBuffer( strBuffer );

					if ( csvBuffer.GetInt(0) != 1 )
						continue;

					CString strReportUser = "";
					{
						int nPasswordIdx = csvBuffer.GetInt(1);

						strReportUser.Format( "%2.2d  ", nPasswordIdx );

						if ( 0 == nPasswordIdx )
							strReportUser += "Master";
						else if ( 99 == nPasswordIdx )
							strReportUser += "PosTray";
						else if ( ( nPasswordIdx >= 1 ) && ( nPasswordIdx <= MAX_PASSWORDS ) )
							strReportUser += PasswordArray.GetUserName( nPasswordIdx );
						else
							strReportUser += "??";
					}

					CString strReportDate = "";
					{
						CString strDate = csvBuffer.GetString(2);
						if ( strDate.GetLength() != 8 )
							strReportDate = strDate;
						else
						{
							strReportDate.Format( "%s/%s/%s",
								(const char*) strDate.Right(2),
								(const char*) strDate.Mid(4,2),
								(const char*) strDate.Left(4) );
						}
					}

					CString strReportTime = "";
					{
						CString strTime = csvBuffer.GetString(3);
						if ( strTime.GetLength() != 6 )
							strReportTime = strTime;
						else
						{
							strReportTime.Format( "%s:%s:%s",
								(const char*) strTime.Left(2),
								(const char*) strTime.Mid(2,2),
								(const char*) strTime.Right(2) );
						}
					}

					CString strReportResult = "";
					{
						int nResult = csvBuffer.GetInt(4);
						if ( 0 == nResult )
							strReportResult = "OK";
						else if ( 1005 == nResult )
							strReportResult.Format( "Error 1005 (%d)", csvBuffer.GetInt(5) );
						else
							strReportResult.Format( "Error %d", nResult );
					}
					
					CCSV csvReport( '\t' );
					csvReport.Add( strReportUser );
					csvReport.Add( strReportDate );
					csvReport.Add( strReportTime );
					csvReport.Add( csvBuffer.GetString(6) );
					csvReport.Add( strReportResult );
					m_ReportFile.WriteLine( csvReport.GetLine() );
				}
			}
		}

		m_ReportFile.WriteLine( "" );
		m_ReportFile.Close();
	}
}

/**********************************************************************/
