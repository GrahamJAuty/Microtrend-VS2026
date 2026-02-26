 /**********************************************************************/
#include "Consolidation.h"
#include "KeyboardJobInfo.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EposTaskDatabase.h"
/**********************************************************************/

CEposTaskHeader::CEposTaskHeader()
{
	Reset();
}

/**********************************************************************/

void CEposTaskHeader::Reset()
{
	m_nTaskNo = 0;
	m_strName = "";
	m_nHours = 24;
	m_nMinutes = 0;
	m_nImportType = IMPORT_IMMEDIATE;
	m_bSelectLocationsWhenRun = FALSE;
}

/**********************************************************************/

void CEposTaskHeader::SetTaskNo( int n )
{
	m_nTaskNo = n;
}

/**********************************************************************/

void CEposTaskHeader::SetName( const char* sz )
{
	m_strName = sz;
}

/**********************************************************************/

void CEposTaskHeader::SetHours( int n )
{
	if ( n >= 0 && n <= 24 ) m_nHours = n;
}

/**********************************************************************/

void CEposTaskHeader::SetMinutes( int n )
{
	if ( n >= 0 && n <= 59 ) m_nMinutes = n;
}

/**********************************************************************/

void CEposTaskHeader::SetImportType( int n )
{
	switch( n )
	{
	case IMPORT_IMMEDIATE:
	case IMPORT_STARTOFDAY:
	case IMPORT_SPECIFIED:
		m_nImportType = n;
		break;
	}
}

/**********************************************************************/

void CEposTaskHeader::Add ( CEposTaskHeader& source )
{
}

/**********************************************************************/

int CEposTaskHeader::Compare( CEposTaskHeader& source, int nHint )
{
	if ( m_nTaskNo != source.m_nTaskNo )
		return ( ( m_nTaskNo > source.m_nTaskNo ) ? 1 : -1 );

	return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposJobLocation::CEposJobLocation()
{
	Reset();
}

/**********************************************************************/

void CEposJobLocation::Reset()
{
	m_nNetWkLocNo = 0;
	m_nTerminalNo = 0;
}

/**********************************************************************/

void CEposJobLocation::Add ( CEposJobLocation& source )
{
}

/**********************************************************************/

int CEposJobLocation::Compare( CEposJobLocation& source, int nHint )
{
	if ( m_nNetWkLocNo != source.m_nNetWkLocNo )
		return ( ( m_nNetWkLocNo > source.m_nNetWkLocNo ) ? 1 : -1 );

	if ( m_nTerminalNo != source.m_nTerminalNo )
		return ( ( m_nTerminalNo > source.m_nTerminalNo ) ? 1 : -1 );

	return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposTaskJob::CEposTaskJob()
{
	Reset();
}

/**********************************************************************/

void CEposTaskJob::Reset()
{
	m_nTaskNo = 0;
	m_nLineNo = 0;
	m_nJobNo = 0;
	m_nParam1BaseJob = 0;
	m_nParam1ExtraJob = 0;
	m_bAll = TRUE;
	m_strLocations = "";
}

/**********************************************************************/

void CEposTaskJob::SetTaskNo( int n )
{
	m_nTaskNo = n;
}

/**********************************************************************/

void CEposTaskJob::SetLineNo( int n )
{
	m_nLineNo = n;
}

/**********************************************************************/

void CEposTaskJob::SetJobNo( int n )
{
	m_nJobNo = n;
}

/**********************************************************************/

void CEposTaskJob::SetParam1BaseJob( int n )
{
	m_nParam1BaseJob = n;
}

/**********************************************************************/

void CEposTaskJob::SetParam1ExtraJob( int n )
{
	m_nParam1ExtraJob = n;
}

/**********************************************************************/

void CEposTaskJob::SetAllFlag( bool b )
{
	m_bAll = b;
}

/**********************************************************************/

void CEposTaskJob::SetLocations( const char* sz )
{
	m_strLocations = sz;
}

/**********************************************************************/

void CEposTaskJob::Add ( CEposTaskJob& source )
{
}

/**********************************************************************/

int CEposTaskJob::Compare( CEposTaskJob& source, int nHint )
{
	if ( m_nTaskNo != source.m_nTaskNo )
		return ( ( m_nTaskNo > source.m_nTaskNo ) ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( ( m_nLineNo > source.m_nLineNo ) ? 1 : -1 );

	return 0;
}

/**********************************************************************/

const char* CEposTaskJob::GetLocText()
{
	if ( TRUE == m_bAll )
		m_strLocText = "All";
	else
	{
		CCSV csvLocation( m_strLocations );
		m_strLocText.Format( "Selected (%d)", csvLocation.GetSize() / 2 );
	}

	return m_strLocText;
}

/**********************************************************************/

bool CEposTaskJob::IsRunAtNetwork( int nNetWkIdx )
{
	bool bResult = FALSE;
	
	if ( ( nNetWkIdx >= 0 ) && ( nNetWkIdx < dbNetwork.GetSize() ) )
	{
		if ( TRUE == m_bAll )
			bResult = TRUE;
		else
		{
			int nNetWkNo = dbNetwork.GetNetworkNo( nNetWkIdx );
			CCSV csv( m_strLocations );

			for ( int n = 0; n < csv.GetSize(); n+= 2 )
			{
				if ( csv.GetInt(n) == nNetWkNo )
				{
					bResult = TRUE;
					break;
				}
			}
		}
	}
	
	return bResult;
}

/**********************************************************************/

bool CEposTaskJob::IsRunAtLocation( int nLocIdx, int nTerminalNo )
{
	bool bResult = FALSE;
	
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < dbLocation.GetSize() ) )
	{
		if ( TRUE == m_bAll )
			bResult = TRUE;
		else
		{
			int nNetWkLocNo = dbLocation.GetNetworkLocNo( nLocIdx );
			CCSV csv( m_strLocations );

			for ( int n = 0; n < csv.GetSize(); n+= 2 )
			{
				if ( ( csv.GetInt(n) == nNetWkLocNo ) && ( csv.GetInt( n + 1 ) == nTerminalNo ) )
				{
					bResult = TRUE;
					break;
				}
			}
		}
	}
	
	return bResult;
}

/**********************************************************************/

void CEposTaskJob::GetLocationArray( CReportConsolidationArray<CEposJobLocation>& arrayLocations )
{
	arrayLocations.RemoveAll();

	CCSV csvLocations( m_strLocations );
	for ( int n = 0; n < csvLocations.GetSize() - 1; n += 2 )
	{
		CEposJobLocation Location;
		Location.m_nNetWkLocNo = csvLocations.GetInt(n);
		Location.m_nTerminalNo = csvLocations.GetInt(n+1);
		arrayLocations.Consolidate( Location );
	}
}

/**********************************************************************/

void CEposTaskJob::SetLocationArray( CReportConsolidationArray<CEposJobLocation>& arrayLocations )
{
	CCSV csvLocations;
	for ( int n = 0; n < arrayLocations.GetSize(); n++ )
	{
		CEposJobLocation Location;
		arrayLocations.GetAt(n, Location);
		csvLocations.Add( Location.m_nNetWkLocNo );
		csvLocations.Add( Location.m_nTerminalNo );
	}
	m_strLocations = csvLocations.GetLine();
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposTaskDatabase::CEposTaskDatabase(void)
{
	m_bPluChangesMode = FALSE;
}

/**********************************************************************/

const char* CEposTaskDatabase::GetFilename()
{
	if ( FALSE == m_bPluChangesMode )
	{
		CFilenameUpdater FnUp( SysFiles::Tasks );
		m_strFilename = FnUp.GetFilenameToUse();
	}
	else
	{
		CFilenameUpdater FnUp( SysFiles::PluChangesTask );
		m_strFilename = FnUp.GetFilenameToUse();
	}

	return m_strFilename;
}

/**********************************************************************/

int CEposTaskDatabase::GetMinTaskNo()
{
	if ( FALSE == m_bPluChangesMode )
		return 0x81;
	else
		return 255;
}

/**********************************************************************/

int CEposTaskDatabase::GetMaxTaskNo()
{
	if ( FALSE == m_bPluChangesMode )
		return 0x80 + SysInfo.GetMaxPresetTasks();
	else
		return 255;
}

/**********************************************************************/

int CEposTaskDatabase::GetMaxTasks()
{
	if ( FALSE == m_bPluChangesMode )
		return SysInfo.GetMaxPresetTasks();
	else
		return 1;
}

/**********************************************************************/

bool CEposTaskDatabase::Read()
{
	SetPluChangesMode( FALSE );
	return ReadInternal();
}

/**********************************************************************/

bool CEposTaskDatabase::ReadPluChangesTask()
{
	m_arrayHeaders.RemoveAll();
	m_arrayJobs.RemoveAll();

	SetPluChangesMode( TRUE );
	bool bResult = ReadInternal();

	if ( TRUE == bResult )
	{
		if ( m_arrayJobs.GetSize() == 0 )
		{
			CEposTaskJob TaskJob;
			TaskJob.SetTaskNo( 255 );
			TaskJob.SetLineNo( 1 );
			TaskJob.SetJobNo( DN_PLU );
			TaskJob.SetParam1BaseJob( 0 );
			TaskJob.SetParam1ExtraJob( 0 );
			TaskJob.SetAllFlag( TRUE );
			TaskJob.SetLocations( "" );
			m_arrayJobs.DirectAdd( TaskJob );
		}
	}

	return bResult;
}

/**********************************************************************/

bool CEposTaskDatabase::ReadInternal()
{
	bool bResult = TRUE;

	m_arrayHeaders.RemoveAll();
	m_arrayJobs.RemoveAll();
	m_nCurrentImportTaskNo = 0;
	m_nCurrentImportJobIdx = 0;

	if ( FALSE == m_bPluChangesMode )
	{
		for ( int nTaskNo = 1; nTaskNo <= SysInfo.GetMaxPresetTasks(); nTaskNo++ )
		{
			CString strName;
			strName.Format( "Preset %d", nTaskNo );

			CEposTaskHeader TaskHeader;
			TaskHeader.SetTaskNo( 0x80 + nTaskNo );
			TaskHeader.SetName( strName );
			m_arrayHeaders.DirectAdd( TaskHeader );
		}
	}
	else
	{
		CEposTaskHeader TaskHeader;
		TaskHeader.SetTaskNo( 255 );
		TaskHeader.SetName( "Plu Changes Task" );
		m_arrayHeaders.DirectAdd( TaskHeader );
	}

	CSSFile fileTasks;
	CString strFilename = GetFilename();
	if ( ::FileExists( strFilename ) == TRUE )
	{
		if ( fileTasks.Open( GetFilename(), "rb" ) == FALSE )
		{
			bResult = FALSE;
		}
		else
		{
			int nVersionType = 0;
			int nTaskCount = 0;

			CString strBuffer;
			while( fileTasks.ReadString( strBuffer ) == TRUE )
			{
				CCSV csv( strBuffer );

				int nThisVer = csv.GetInt(1);

				switch( nThisVer )
				{
				case 1:
				case 2:
					if ( nVersionType != 2 )
					{
						ImportLegacyTaskLine( csv, nThisVer, nTaskCount );
						nVersionType = 1;
					}
					break;

				case 3:
				case 4:
					if ( nVersionType != 1 )
					{
						ImportSPOSTaskLine( csv, nThisVer );
						nVersionType = 2;
					}
					break;
				}
			}
		}
	}
			
	return bResult;
}

/**********************************************************************/

void CEposTaskDatabase::ImportLegacyTaskLine( CCSV& csv, int nVer, int& nTaskCount )
{
	if ( nTaskCount < GetMaxTasks() )
	{
		CEposTaskHeader TaskHeader;
		m_arrayHeaders.GetAt( nTaskCount, TaskHeader );
		TaskHeader.SetName( csv.GetString(2) );
		TaskHeader.SetHours( csv.GetInt(3) );
		TaskHeader.SetMinutes( csv.GetInt(4) );
		TaskHeader.SetImportType( csv.GetInt(5) );
		m_arrayHeaders.SetAt( nTaskCount, TaskHeader );

		CEposTaskJob Job;

		if ( FALSE == m_bPluChangesMode )
			Job.SetTaskNo( 0x81 + nTaskCount );
		else
			Job.SetTaskNo( 255 );
	
		int nJobCount = csv.GetInt(6);
		int nOffset = 7;
		
		for ( int n = 1; n <= nJobCount; n++ )
		{
			Job.SetJobNo( csv.GetInt( nOffset++ ) );
			Job.SetLineNo( 1 );

			if ( 2 == nVer )
			{
				Job.SetParam1BaseJob( csv.GetInt( nOffset++ ) );
			}
			else
			{
				Job.SetParam1BaseJob( 0 );
			}

			Job.SetParam1ExtraJob(0);

			Job.SetAllFlag( csv.GetBool( nOffset++ ) );

			CCSV csvLocations;
			int nLocCount = csv.GetInt( nOffset++ );
			for ( int n = 0; n < nLocCount; n++ )
			{
				csvLocations.Add( csv.GetInt(nOffset++) );
				csvLocations.Add( 0 );
			}

			Job.SetLocations( csvLocations.GetLine() );
			
			m_arrayJobs.DirectAdd( Job );
		}

		nTaskCount++;
	}
}

/**********************************************************************/

void CEposTaskDatabase::ImportSPOSTaskLine( CCSV& csv, int nVer )
{
	int nTaskNo = csv.GetInt(0);
	
	if ( ( nTaskNo >= GetMinTaskNo() ) && ( nTaskNo <= GetMaxTaskNo() ) )
	{
		switch( csv.GetInt(2) )
		{
		case 0:	//HEADER
			{
				CEposTaskHeader Header;

				if ( FALSE == m_bPluChangesMode )
				{
					m_arrayHeaders.GetAt( nTaskNo - 0x81, Header );
				}
				else
				{
					m_arrayHeaders.GetAt( 0, Header );
				}

				Header.SetName( csv.GetString(3) );
				Header.SetHours( csv.GetInt(4) );
				Header.SetMinutes( csv.GetInt(5) );
				Header.SetImportType( csv.GetInt(6) );
				Header.SetSelectLocationsWhenRunFlag( csv.GetBool(7) );
				
				if ( FALSE == m_bPluChangesMode )
				{
					m_arrayHeaders.SetAt( nTaskNo - 0x81, Header );
				}
				else
				{
					m_arrayHeaders.SetAt( 0, Header );
				}
			}
			break;

		case 1:	//JOB LINE
			{
				CEposTaskJob Job;
				Job.SetTaskNo( nTaskNo );

				int nOffset = 3;

				Job.SetJobNo( csv.GetInt(nOffset++) );
				Job.SetParam1BaseJob( csv.GetInt(nOffset++) );

				if ( nVer >= 4 )
				{
					Job.SetParam1ExtraJob( csv.GetInt(nOffset++) );
				}

				Job.SetAllFlag( csv.GetBool(nOffset++) );
				Job.SetLocations( csv.GetString(nOffset++) );
				
				if ( nTaskNo != m_nCurrentImportTaskNo )
				{
					Job.SetLineNo(0);
					Job.SetTaskNo( nTaskNo + 1 );
					m_arrayJobs.Find( Job, m_nCurrentImportJobIdx );
					Job.SetTaskNo( nTaskNo );
				}

				Job.SetLineNo(1);
				m_arrayJobs.InsertAt( m_nCurrentImportJobIdx++, Job );
				m_nCurrentImportTaskNo = Job.GetTaskNo();
			}
			break;
		}
	}
}

/**********************************************************************/

bool CEposTaskDatabase::Write()
{
	SetPluChangesMode( FALSE );
	return WriteInternal();
}

/**********************************************************************/

bool CEposTaskDatabase::WritePluChangesTask()
{
	SetPluChangesMode( TRUE );
	return WriteInternal();
}

/**********************************************************************/

bool CEposTaskDatabase::WriteInternal()
{
	bool bResult = FALSE;

	CSSFile fileOut;
	if ( fileOut.Open( GetFilename(), "wb" ) == TRUE )
	{
		for ( int n = 0; n < m_arrayHeaders.GetSize(); n++ )
		{
			CEposTaskHeader Header;
			m_arrayHeaders.GetAt( n, Header );

			CCSV csv;
			csv.Add( Header.GetTaskNo() );
			csv.Add( 4 );
			csv.Add( 0 );
			csv.Add( Header.GetName() );
			csv.Add( Header.GetHours() );
			csv.Add( Header.GetMinutes() );
			csv.Add( Header.GetImportType() );
			csv.Add( Header.GetSelectLocationsWhenRunFlag() );
			fileOut.WriteLine( csv.GetLine() );
		}

		for ( int n = 0; n < m_arrayJobs.GetSize(); n++ )
		{
			CEposTaskJob Job;
			m_arrayJobs.GetAt( n, Job );

			CCSV csv;
			csv.Add( Job.GetTaskNo() );
			csv.Add( 4 );
			csv.Add( 1 );
			csv.Add( Job.GetJobNo() );
			csv.Add( Job.GetParam1BaseJob() );
			csv.Add( Job.GetParam1ExtraJob() );
			csv.Add( Job.GetAllFlag() );
			csv.Add( Job.GetLocations() );
			fileOut.WriteLine( csv.GetLine() );
		}

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CEposTaskDatabase::CleanUpTaskFiles()
{
	Read();

	int nProgress = 0;
	StatusProgress.Lock( TRUE, "Updating task files" );
	
	int nJobCount = m_arrayJobs.GetSize();
	for ( int nJobIdx = 0; nJobIdx < nJobCount; nJobIdx++ )
	{
		StatusProgress.SetPos ( nJobIdx, nJobCount );
		
		CEposTaskJob Job;
		m_arrayJobs.GetAt( nJobIdx, Job );

		if ( Job.GetAllFlag() == TRUE )
		{
			Job.SetLocations( "" );
		}
		else if ( Job.GetJobNo() == RX_SALES )
		{
			CReportConsolidationArray<CEposJobLocation> arrayNetworks;
			Job.GetLocationArray( arrayNetworks );

			bool bChanged = FALSE;
			for ( int n = arrayNetworks.GetSize() - 1; n >= 0; n-- )
			{
				CEposJobLocation JobLoc;
				arrayNetworks.GetAt( n, JobLoc );

				int nNwkIdx;
				if ( dbNetwork.FindNetworkByNumber ( JobLoc.m_nNetWkLocNo, nNwkIdx ) == FALSE )	
				{
					arrayNetworks.RemoveAt(n);
					bChanged = TRUE;
				}
			}

			if ( TRUE == bChanged )
				Job.SetLocationArray( arrayNetworks );
		}
		else
		{
			CReportConsolidationArray<CEposJobLocation> arrayLocations;
			Job.GetLocationArray( arrayLocations );

			bool bChanged = FALSE;
			for ( int n = arrayLocations.GetSize() - 1; n >= 0; n-- )
			{
				CEposJobLocation JobLoc;
				arrayLocations.GetAt( n, JobLoc );

				int nNetWkNo = JobLoc.m_nNetWkLocNo / 1000;
				int nLocNo = JobLoc.m_nNetWkLocNo % 1000;

				int nLocIdx;
				if ( dbLocation.FindLocationByNumber ( nNetWkNo, nLocNo, nLocIdx ) == FALSE )	
				{
					arrayLocations.RemoveAt(n);
					bChanged = TRUE;
				}
			}

			if ( TRUE == bChanged )
				Job.SetLocationArray( arrayLocations );
		}

		m_arrayJobs.SetAt( nJobIdx, Job );
	}
	
	StatusProgress.Unlock();
	Write();
}

/**********************************************************************/

void CEposTaskDatabase::GetLibraryTaskHeader( int nTaskNo, CEposTaskHeader& Header )
{
	Header.Reset();
	Header.SetTaskNo( nTaskNo );

	int nPos;
	if ( m_arrayHeaders.Find( Header, nPos ) == TRUE )
		m_arrayHeaders.GetAt( nPos, Header );
}

/**********************************************************************/

void CEposTaskDatabase::GetLibraryTaskJobIdxRange( int nTaskNo, int& nStartJobIdx, int& nEndJobIdx )
{
	CEposTaskJob TaskJob;
	TaskJob.SetTaskNo( nTaskNo );
	TaskJob.SetLineNo( -1 );
	m_arrayJobs.Find( TaskJob, nStartJobIdx );

	TaskJob.SetTaskNo( nTaskNo + 1 );
	m_arrayJobs.Find( TaskJob, nEndJobIdx );
	nEndJobIdx--;
}

/**********************************************************************/

int CEposTaskDatabase::GetLibraryTaskJobCount( int nTaskNo )
{
	int nStartIdx, nEndIdx;
	GetLibraryTaskJobIdxRange( nTaskNo, nStartIdx, nEndIdx );
	return ( nEndIdx - nStartIdx ) + 1;
}

/**********************************************************************/

const char* CEposTaskDatabase::GetLibraryTaskName( int nTaskNo )
{
	CEposTaskHeader TaskHeader;
	GetLibraryTaskHeader( nTaskNo, TaskHeader );
	m_strTaskName = TaskHeader.GetName();
	return m_strTaskName;
}

/**********************************************************************/

void CEposTaskDatabase::CreateSimpleTask( int nJobNo, int nLocIdx, int nParam1 )
{
	CWordArray arrayJobNo;
	arrayJobNo.Add( nJobNo );

	CArray<int,int> arrayLocIdx;
	arrayLocIdx.Add( nLocIdx );
	
	CreateSimpleTask( arrayJobNo, arrayLocIdx, nParam1 );
}

/**********************************************************************/

void CEposTaskDatabase::CreateSimpleTask( int nJobNo, CArray<int,int>& arrayLocIdx, int nParam1 )
{
	CWordArray arrayJobNo;
	arrayJobNo.Add( nJobNo );
	CreateSimpleTask( arrayJobNo, arrayLocIdx, nParam1 );
}

/**********************************************************************/

void CEposTaskDatabase::CreateSimpleTask( CWordArray& arrayJobNo, CArray<int,int>& arrayLocIdx, int nParam1 )
{
	m_arrayHeaders.RemoveAll();
	m_arrayJobs.RemoveAll();

	CEposTaskHeader TaskHeader;
	TaskHeader.SetTaskNo(0);
	TaskHeader.SetImportType( IMPORT_IMMEDIATE );
	m_arrayHeaders.DirectAdd( TaskHeader );
	
	CEposTaskJob TaskJob;
	TaskJob.SetTaskNo(0);
	TaskJob.SetLineNo(1);
	TaskJob.SetAllFlag(FALSE);
	TaskJob.SetParam1BaseJob(nParam1);
	TaskJob.SetParam1ExtraJob(0);
	
	for ( int j = 0; j < arrayJobNo.GetSize(); j++ )
	{
		TaskJob.SetJobNo( arrayJobNo.GetAt(j) );

		CCSV csvLocations;

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);

			CArray<CLocationMasterInfo,CLocationMasterInfo> arrayMasters;
			LocationMasterMap.GetLocationMasterList( nLocIdx, arrayMasters, TRUE, FALSE );
			
			for ( int nIdx = 0; nIdx < arrayMasters.GetSize(); nIdx++ )
			{
				CLocationMasterInfo infoMaster = arrayMasters.GetAt( nIdx );
				csvLocations.Add( dbLocation.GetNetworkLocNo( nLocIdx ) );
				csvLocations.Add( infoMaster.GetTerminalNo() );
			}
		}

		TaskJob.SetLocations( csvLocations.GetLine() );
		m_arrayJobs.DirectAdd( TaskJob );
	}
}

/**********************************************************************/

void CEposTaskDatabase::CreateAdhocTask()
{
	m_arrayHeaders.RemoveAll();
	m_arrayJobs.RemoveAll();

	CEposTaskHeader TaskHeader;
	TaskHeader.SetTaskNo(0);
	TaskHeader.SetName( "Adhoc Task" );
	TaskHeader.SetImportType( IMPORT_IMMEDIATE );
	m_arrayHeaders.DirectAdd( TaskHeader );
}

/**********************************************************************/

void CEposTaskDatabase::LoadEditableTask( CEposTaskDatabase& EditableTask, int nSourceTaskNo )
{
	EditableTask.m_arrayHeaders.RemoveAll();
	EditableTask.m_arrayJobs.RemoveAll();

	{
		CEposTaskHeader TaskHeader;
		GetLibraryTaskHeader( nSourceTaskNo, TaskHeader );
		EditableTask.m_arrayHeaders.DirectAdd( TaskHeader );
	}
		
	int nStartIdx, nEndIdx;
	GetLibraryTaskJobIdxRange( nSourceTaskNo, nStartIdx, nEndIdx );

	for ( int nJobIdx = nStartIdx; nJobIdx <= nEndIdx; nJobIdx++ )
	{
		CEposTaskJob TaskJob;
		m_arrayJobs.GetAt( nJobIdx, TaskJob );
		EditableTask.m_arrayJobs.DirectAdd( TaskJob );
	}

	EditableTask.SetPluChangesMode( GetPluChangesMode() );
}

/**********************************************************************/

void CEposTaskDatabase::SaveEditableTask( CEposTaskDatabase& EditableTask )
{
	CEposTaskHeader TaskHeader;
	EditableTask.GetEditableTaskHeader( TaskHeader );

	int nPos;
	if ( m_arrayHeaders.Find( TaskHeader, nPos ) == TRUE )
		m_arrayHeaders.SetAt( nPos, TaskHeader );
	else
		m_arrayHeaders.InsertAt( nPos, TaskHeader );
	
	int nStartIdx, nEndIdx;
	GetLibraryTaskJobIdxRange( TaskHeader.GetTaskNo(), nStartIdx, nEndIdx );
	for ( int nJobIdx = nEndIdx; nJobIdx >= nStartIdx; nJobIdx-- )
		m_arrayJobs.RemoveAt( nJobIdx );

	for ( int nJobIdx = 0; nJobIdx < EditableTask.m_arrayJobs.GetSize(); nJobIdx++ )
	{
		CEposTaskJob TaskJob;
		EditableTask.m_arrayJobs.GetAt( nJobIdx, TaskJob );
		m_arrayJobs.InsertAt( nStartIdx++, TaskJob );
	}
}

/**********************************************************************/

void CEposTaskDatabase::GetEditableTaskHeader( CEposTaskHeader& Header )
{
	if ( m_arrayHeaders.GetSize() == 1 ) m_arrayHeaders.GetAt( 0, Header );
}

/**********************************************************************/

void CEposTaskDatabase::SetEditableTaskHeader( CEposTaskHeader& Header )
{
	if ( m_arrayHeaders.GetSize() == 1 ) m_arrayHeaders.SetAt( 0, Header );
}

/**********************************************************************/

int CEposTaskDatabase::GetEditableTaskJobCount()
{
	if ( m_arrayHeaders.GetSize() == 1 ) 
		return m_arrayJobs.GetSize();
	else
		return 0;
}

/**********************************************************************/

void CEposTaskDatabase::GetEditableTaskJob( int nJobIdx, CEposTaskJob& Job )
{
	if ( m_arrayHeaders.GetSize() == 1 ) 
		if ( ( nJobIdx >= 0 ) && ( nJobIdx < m_arrayJobs.GetSize() ) )
			m_arrayJobs.GetAt( nJobIdx, Job );
}

/**********************************************************************/

void CEposTaskDatabase::SetEditableTaskJob( int nJobIdx, CEposTaskJob& Job )
{
	if ( m_arrayHeaders.GetSize() == 1 ) 
		if ( ( nJobIdx >= 0 ) && ( nJobIdx < m_arrayJobs.GetSize() ) )
			m_arrayJobs.SetAt( nJobIdx, Job );
}

/**********************************************************************/

void CEposTaskDatabase::RemoveEditableTaskJob( int nJobIdx )
{
	if ( m_arrayHeaders.GetSize() == 1 ) 
		if ( ( nJobIdx >= 0 ) && ( nJobIdx < m_arrayJobs.GetSize() ) )
			m_arrayJobs.RemoveAt( nJobIdx );
}

/**********************************************************************/

bool CEposTaskDatabase::HasEditableTaskDownloadJob()
{
	bool bResult = FALSE;
	
	for ( int nJobIdx = 0; nJobIdx <= m_arrayJobs.GetSize(); nJobIdx++ )
	{
		CEposTaskJob TaskJob;
		m_arrayJobs.GetAt( nJobIdx, TaskJob );
		
		if ( ( TaskJob.GetJobNo() & 0xFF00 ) == DN_JOB )
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

/**********************************************************************/

void CEposTaskDatabase::AddEditableTaskJob( int nJobNo, int nParam1BaseJob, bool bFromStart, int nParam1ExtraJob )
{
	if ( m_arrayHeaders.GetSize() == 1 ) 
	{
		CEposTaskHeader TaskHeader;
		GetEditableTaskHeader( TaskHeader );

		CEposTaskJob TaskJob;
		TaskJob.SetTaskNo( TaskHeader.GetTaskNo() );
		TaskJob.SetLineNo(1);
		TaskJob.SetParam1BaseJob(nParam1BaseJob);
		TaskJob.SetParam1ExtraJob(nParam1ExtraJob);
		TaskJob.SetJobNo(nJobNo);
		TaskJob.SetAllFlag(TRUE);
		TaskJob.SetLocations("");

		if ( FALSE == bFromStart )
			m_arrayJobs.DirectAdd( TaskJob );
		else
			m_arrayJobs.InsertAt( 0, TaskJob );
	}
}	

/**********************************************************************/

int CEposTaskDatabase::GetJobSortOrder( int nJobNo )
{
	CKeyboardJobInfo infoKb;
	int nKbNo = infoKb.GetKbNoFromJobNo( nJobNo );

	if (nKbNo != 0)
	{
		return nKbNo + 100;
	}
	else
	{
		switch( nJobNo )
		{
		case RX_SALES:				return 1;
		case RX_REASON:				return 2;
		case RX_EODPLU:				return 3;
		case RX_EODCUSTOMER:		return 4;
		case DN_PLU:				return 5;
		case DN_PLU_KBD:			return 6;
		case DN_PLU_SET1:			return 7;
		case DN_PLU_SET1_KBD:		return 8;
		case DN_PLU_SET2:			return 9;
		case DN_PLU_SET2_KBD:		return 10;
		case DN_PLU_SET3:			return 11;
		case DN_PLU_SET3_KBD:		return 12;
		case DN_PLU_INFO:			return 13;
		case DN_BAR:				return 14;
		case DN_ACAT:				return 15;
		case DN_PAIDIO:				return 16;
		case DN_DEPT:				return 17;
		case DN_GROUP:				return 18;
		case DN_ALLERGEN:			return 18;
		case DN_MIX:				return 19;
		case DN_PROMO:				return 20;
		case DN_WALLETSTAMPRULE:			return 21;
		case DN_ALLOFFER:			return 22;
		case DN_TPLAN:				return 23;
		case DN_SERVER_NAME:		return 24;
		case DN_SERVER_PRESET:		return 25;
		case DN_TAX:				return 26;
		case DN_PAY:				return 27;
		case DN_MOD:				return 28;
		case DN_RECEIPT:			return 29;
		case DN_FUNCTION:			return 30;
		case DN_IMAGES:				return 31;
		case DN_LOYALTY_SCHEME:		return 32;
		case DN_TABLE_AREA:			return 33;
		case DN_TABLE_NAME:			return 34;
		case DN_MACRO:				return 35;
		case DN_REASON:				return 36;
		default:					return 0;
		}
	}
}

/**********************************************************************/

void CEposTaskDatabase::TidyEditableTaskJobs()
{
	CReportConsolidationArray<CEposTaskJob> arrayTemp;

	bool bGotPromotion = FALSE;
	bool bGotMixMatch = FALSE;
	bool bGotWalletStampRule = FALSE;
	bool bGotAllOffer = FALSE;

	for ( int n = 0; n < m_arrayJobs.GetSize(); n++ )
	{
		CEposTaskJob TaskJob;
		m_arrayJobs.GetAt( n, TaskJob );

		switch (TaskJob.GetJobNo())
		{
		case DN_PROMO:
			bGotPromotion = TRUE;
			break;

		case DN_MIX:
			bGotMixMatch = TRUE;
			break;

		case DN_WALLETSTAMPRULE:
			bGotWalletStampRule = TRUE;
			break;

		case DN_ALLOFFER:
			bGotAllOffer = TRUE;
			break;
		}
		
		TaskJob.SetLineNo( GetJobSortOrder( TaskJob.GetJobNo() ) );

		int nPos;
		if (arrayTemp.Find(TaskJob, nPos) == FALSE)
		{
			arrayTemp.InsertAt(nPos, TaskJob);
		}
	}

	m_arrayJobs.RemoveAll();
	
	int nPromoIdx = -1;
	int nMixMatchIdx = -1;

	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CEposTaskJob TaskJob;
		arrayTemp.GetAt( n, TaskJob );
		TaskJob.SetLineNo(1);

		bool bAddJob = TRUE;

		switch (TaskJob.GetJobNo())
		{
		case DN_PROMO:
			if ((TRUE == bGotMixMatch) || (TRUE == bGotAllOffer))
			{
				bAddJob = FALSE;
			}
			break;

		case DN_MIX:
		case DN_WALLETSTAMPRULE:
			if (TRUE == bGotAllOffer)
			{
				bAddJob = FALSE;
			}
			break;
		}

		if (TRUE == bAddJob)
		{
			m_arrayJobs.DirectAdd(TaskJob);
		}
	}
}

/**********************************************************************/
