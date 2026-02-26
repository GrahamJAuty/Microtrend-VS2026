/**********************************************************************/
#include "CommsErrors.h"
#include "Consolidation.h"
#include "FileRemover.h"
#include "FTPConnect.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "NetworkCSVArray.h"
#include "RepCSV.h"
#include "ReportConsolidationArray.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "EODFileMover.h"
/**********************************************************************/

CEODFileInfo::CEODFileInfo()
{
	Reset();
}

/**********************************************************************/

void CEODFileInfo::Reset()
{
	m_strFirstLine = "";
	m_strFilename = "";
	m_strFilenameToDelete = "";
	m_nFileLength = 0;
}

/**********************************************************************/

void CEODFileInfo::Add(CEODFileInfo& source)
{
	if (source.m_nFileLength > m_nFileLength)
	{
		source.m_strFilenameToDelete = m_strFilename;
		m_strFilename = source.m_strFilename;
		m_nFileLength = source.m_nFileLength;
	}
	else
	{
		source.m_strFilenameToDelete = source.m_strFilename;
	}
}

/**********************************************************************/

int CEODFileInfo::Compare( CEODFileInfo& source, int nHint )
{
	if (m_strFirstLine != source.m_strFirstLine)
	{
		return (m_strFirstLine > source.m_strFirstLine ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEODFileMover::CEODFileMover(void)
{
	m_nFileCount = 0;
}

/**********************************************************************/

void CEODFileMover::MoveReasonFilesNetwork( int nNetworkIdx, int nParam1 )
{
	CString strSlash = "\\";

	m_nFileCount = 0;
	
	CString strSourcePath;
	dbNetwork.GetSafeFileNetworkPath( nNetworkIdx, strSourcePath );
	
	CString strDestPath1 = dbNetwork.GetFolderPathDataHistory( nNetworkIdx );
	::CreateSubdirectory( strDestPath1 );
	
	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strSourcePath + "EOD*" );

	while ( TRUE == bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == FALSE )
			continue;

		CString strEODFolder = finder.GetFileName();
		
		CString strDestPath2 = dbNetwork.GetFolderPathDataEODFiles( nNetworkIdx, strEODFolder );
		::CreateSubdirectory( strDestPath2 );
		
		if ( ( nParam1 >=0 ) && ( nParam1 <= 7 ) )
		{
			COleDateTime date = COleDateTime::GetCurrentTime();

			for ( int x = 0; x <= nParam1; x++ )
			{
				CString strDateFolder;
				strDateFolder.Format( "%4.4d%2.2d%2.2d",
					date.GetYear(),
					date.GetMonth(),
					date.GetDay() );

				CString strSourcePathDate = "";
				strSourcePathDate += strSourcePath;
				strSourcePathDate += strEODFolder;
				strSourcePathDate += strSlash;
				strSourcePathDate += strDateFolder;
				
				if ( ::FileExists( strSourcePathDate + strSlash + "REASONS.CSV" ) == TRUE )
				{
					CString strDestFile = "";
					strDestFile += "REASONS_";
					strDestFile += strDateFolder;
					strDestFile += ".CSV";

					CopyFile( strSourcePathDate + strSlash + "REASONS.CSV", strDestPath2 + strSlash + strDestFile, FALSE );
					
					dbNetwork.WriteReasonTokenFile( nNetworkIdx );
					m_nFileCount++;
				}
				else if ( ::FileExists( strSourcePathDate + strSlash + "REASONS.CON" ) == TRUE )
				{
					CString strDestFile = "";
					strDestFile += "REASONS_";
					strDestFile += strDateFolder;
					strDestFile += ".CON";

					CopyFile( strSourcePathDate + strSlash + "REASONS.CON", strDestPath2 + strSlash + strDestFile, FALSE );
					
					dbNetwork.WriteReasonTokenFile( nNetworkIdx );
					m_nFileCount++;
				}
			
				date -= COleDateTimeSpan( 1, 0, 0, 0 );
			}
		}
		else
		{
			CFileFind finderDate;
			BOOL bWorkingDate = finderDate.FindFile( strSourcePath + strEODFolder + "\\*.*" );

			while( TRUE == bWorkingDate )
			{
				bWorkingDate = finderDate.FindNextFile();

				if ( finderDate.IsDots() == TRUE )
					continue;

				if ( finderDate.IsDirectory() == FALSE )
					continue;

				CString strDateFolder = finderDate.GetFileName();
				
				if ( strDateFolder.GetLength() != 8 )
					continue;

				if ( ::TestNumeric( strDateFolder) == FALSE )
					continue;

				CString strSourcePathDate = "";
				strSourcePathDate += strSourcePath;
				strSourcePathDate += strEODFolder;
				strSourcePathDate += strSlash;
				strSourcePathDate += strDateFolder;
				
				if ( ::FileExists( strSourcePathDate + strSlash + "REASONS.CSV" ) == TRUE )
				{
					CString strDestFile = "";
					strDestFile += "REASONS_";
					strDestFile += strDateFolder;
					strDestFile += ".CSV";
		
					CopyFile( strSourcePathDate + strSlash + "REASONS.CSV", strDestPath2 + strSlash + strDestFile, FALSE );
					
					dbNetwork.WriteReasonTokenFile( nNetworkIdx );
					m_nFileCount++;
				}
				else if ( ::FileExists( strSourcePathDate + strSlash + "REASONS.CON" ) == TRUE )
				{
					CString strDestFile = "";
					strDestFile += "REASONS_";
					strDestFile += strDateFolder;
					strDestFile += ".CON";
		
					CopyFile( strSourcePathDate + strSlash + "REASONS.CON", strDestPath2 + strSlash + strDestFile, FALSE );
					
					dbNetwork.WriteReasonTokenFile( nNetworkIdx );
					m_nFileCount++;
				}
			}
		}
	}
}

/**********************************************************************/

void CEODFileMover::TidyReasonFiles( int nNetworkIdx )
{
	CString strPath = dbNetwork.GetFolderPathDataHistory( nNetworkIdx );
	strPath += "\\";

	CFileFind finder;
	BOOL bWorking = finder.FindFile( strPath + "EOD*" );

	int nDummy;
	CFileRemover FileRemover;
	CString strDestFolder = dbNetwork.GetFolderPathDataEODFiles( nNetworkIdx, "Reasons" );
	FileRemover.RemoveFolderContents( strDestFolder, nDummy );
	::CreateSubdirectory( strDestFolder );
	strDestFolder += "\\";

	while( TRUE == bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == FALSE )
			continue;

		CReportConsolidationArray<CEODFileInfo> arrayFiles;
		CReportConsolidationArray<CSortedStringItem> arrayFilesToDelete;

		CString strPath2 = "";
		strPath2 += strPath;
		strPath2 += finder.GetFileName();
		strPath2 += "\\";
		
		CFileFind finder2;
		BOOL bWorking2 = finder2.FindFile( strPath2 + "reasons_*" );

		while( TRUE == bWorking2 )
		{
			bWorking2 = finder2.FindNextFile();

			if ( finder2.IsDots() == TRUE )
				continue;

			if ( finder2.IsDirectory() == TRUE )
				continue;

			CString strFilename = finder2.GetFileName();
			if ( strFilename.GetLength() != 20 )
				continue;

			strFilename.MakeLower();
			if ( ( strFilename.Right(4) != ".con" ) && ( strFilename.Right(4) != ".csv" ) )
				continue;

			CString strDate = strFilename.Mid(8,8);
			if ( ::TestNumeric( strDate ) == FALSE )
				continue;

			CSSFile fileReasons;
			if ( fileReasons.Open( strPath2 + strFilename, "rb" ) == FALSE )
				continue;

			CString strFirstLine = "";
			if ( fileReasons.ReadString( strFirstLine ) == TRUE )
			{
				CEODFileInfo info;
				info.m_strFirstLine = strFirstLine;
				info.m_strFilename = strFilename;
				info.m_strFilenameToDelete = "";
				info.m_nFileLength = fileReasons.GetLength();

				arrayFiles.Consolidate( info );

				if ( info.m_strFilenameToDelete != "" )
				{
					CSortedStringItem item;
					item.m_strItem = info.m_strFilenameToDelete;
					arrayFilesToDelete.Consolidate( item );
				}

				fileReasons.Close();
			}
		}

		for ( int n = 0; n < arrayFilesToDelete.GetSize(); n++ )
		{
			CSortedStringItem item;
			arrayFilesToDelete.GetAt( n, item );
			CFileRemover FileRemover( strPath2 + item.m_strItem );
		}

		//int nDummy;
		//CFileRemover FileRemover;
		//CString strDestFolder = dbNetwork.GetFolderPathDataEODFiles( nNetworkIdx, "Reasons" );
		//FileRemover.RemoveFolderContents( strDestFolder, nDummy );
		//::CreateSubdirectory( strDestFolder );
		//strDestFolder += "\\";

		__int64 nTotalFileLength = 0;
		for ( int n = 0; n < arrayFiles.GetSize(); n++ )
		{
			CEODFileInfo info;
			arrayFiles.GetAt( n, info );
			nTotalFileLength += info.m_nFileLength;
		}

		__int64 nProgress = 0;
		StatusProgress.Lock( TRUE, "Rebuilding Reason File History" );

		CReportConsolidationArray<CSortedIntByString> arrayFilenames;
		for ( int n = 0; n < arrayFiles.GetSize(); n++ )
		{
			CEODFileInfo info;
			arrayFiles.GetAt( n, info );

			CSortedIntByString item;
			item.m_strItem = info.m_strFilename;
			item.m_nItem = info.m_nFileLength;
			arrayFilenames.Consolidate( item );
		}
		
		for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
		{
			bool bDeleteAndSkip = FALSE;
			
			CSortedIntByString item;
			arrayFilenames.GetAt( n, item );
			CString strFilename = item.m_strItem;
			int nFileLength = item.m_nItem;
			
			if ( strFilename.Right(3) == "con" )
			{
				if ( n < arrayFilenames.GetSize() - 1 )
				{
					CSortedIntByString item2;
					arrayFilenames.GetAt( n + 1, item2 );
					CString strNextFilename = item2.m_strItem;

					if ( strNextFilename.Right(3) == "csv" )
						if ( strFilename.Left(17) == strNextFilename.Left(17) )
							bDeleteAndSkip = TRUE;
				}
			}

			if ( TRUE == bDeleteAndSkip )
				CFileRemover FileRemover( strPath2 + strFilename );
			else
			{
				CSSFile fileSource;
				if ( fileSource.Open( strPath2 + strFilename, "rb" ) == TRUE )
				{	 
					CString strBuffer;
					while( fileSource.ReadString( strBuffer ) == TRUE )
					{
						StatusProgress.SetPos( nProgress + fileSource.Tell(), nTotalFileLength );
						
						CString strType = strFilename.Right(3);
						int nTNo = 0;
						CString strDate = "";

						if ( strType == "con" )
						{
							CCSVReasonDataCON csv ( strBuffer );
							nTNo = csv.GetTNo();
							strDate = csv.GetDate();
						}
						else
						{
							CCSVReasonDataCSV csv ( strBuffer );
							nTNo = csv.GetTNo();
							strDate = csv.GetDate();
						}

						if ( strDate.GetLength() != 8 )
							continue;
				
						CString strDestFile;
						strDestFile.Format( "20%s%s%s_%s.%3.3d",
							(const char*) strDate.Right(2),
							(const char*) strDate.Mid(3,2),
							(const char*) strDate.Left(2),
							(const char*) strType,
							nTNo );

						CSSFile fileDest;
						if (fileDest.Open(strDestFolder + strDestFile, "ab") == TRUE)
						{
							fileDest.WriteLine(strBuffer);
						}
					}
				}
			}

			//LENGTH OF FILE
			nProgress += nFileLength;
		}

		StatusProgress.Unlock();
	}
}

/**********************************************************************/

void CEODFileMover::MoveEODPluFileLocation( int nNwkIdx, int nLocIdx )
{
	m_nFileCount = 0;

	CString strPath1, strPath2;
	dbNetwork.GetSafeFileNetworkPath( nNwkIdx, strPath1 );
	dbLocation.GetSafeTerminalExportPath( nLocIdx, strPath2 );
	
	CString strSourcePath = "";
	strSourcePath += strPath1;
	strSourcePath += strPath2;

	if ( dbLocation.GetProdCSVImportFolderType( nLocIdx ) == EODIMPORT_FOLDERTYPE_INTRADING )
		strSourcePath += "INTRADING\\";
	
	CString strDestPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( nLocIdx );
	::CreateSubdirectory( strDestPath );
	strDestPath += "\\";

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strSourcePath + "PROD*.CSV" );

	while ( TRUE == bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();

		if ( CopyFile( strSourcePath + strFilename, strDestPath + strFilename, FALSE ) == TRUE )
		{
			m_nFileCount++;
			CFileRemover FileRemover( strSourcePath + strFilename );
		}
	}
}

/**********************************************************************/

void CEODFileMover::MoveEODCustomerFileLocation( int nNwkIdx, int nLocIdx )
{
	m_nFileCount = 0;

	CString strPath1, strPath2;
	dbNetwork.GetSafeFileNetworkPath( nNwkIdx, strPath1 );
	dbLocation.GetSafeTerminalExportPath( nLocIdx, strPath2 );
	
	CString strSourcePath = "";
	strSourcePath += strPath1;
	strSourcePath += strPath2;

	if ( dbLocation.GetCustomerImportFolderType( nLocIdx ) == EODIMPORT_FOLDERTYPE_INTRADING )
		strSourcePath += "INTRADING\\";
	
	CString strDestPath = dbLocation.GetFolderPathSyssetPendingCustomerLocal( nLocIdx );
	::CreateSubdirectory( strDestPath );
	strDestPath += "\\";

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strSourcePath + "CUSTOMER*.SQL" );

	while ( TRUE == bWorking )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();

		if ( CopyFile( strSourcePath + strFilename, strDestPath + strFilename, FALSE ) == TRUE )
		{
			m_nFileCount++;
			CFileRemover FileRemover( strSourcePath + strFilename );
		}
	}
}

/**********************************************************************/
