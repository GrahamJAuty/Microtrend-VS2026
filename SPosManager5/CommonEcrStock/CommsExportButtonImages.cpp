/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "SPOSVersions.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "CommsExportButtonImages.h"
/**********************************************************************/

CCommsExportButtonImages::CCommsExportButtonImages( CJobListCSVRecord& JobInfo ) 
{
	m_strFilename = "";
	m_strSourcePath = "";
	m_strDestPath = "";
	m_nNetworkIdx = JobInfo.GetNetworkIdx();
	m_nLocIdx = JobInfo.GetLocIdx();
	m_nTerminalNo = JobInfo.GetTerminalNo();
	m_bImmediate = JobInfo.GetIsImmediateFlag();
}

/**********************************************************************/

int CCommsExportButtonImages::PrepareFile()
{
	if ( m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize() )
		return COMMS_ERROR_BADNETIDX;

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		return COMMS_ERROR_BADLOCIDX;

	m_strSourcePath = "Images\\";
	::GetDataProgramPath( m_strSourcePath );

	m_strDestPath = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );
	
	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo ( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx ) == FALSE )
		return COMMS_ERROR_BADDBLINK;

	if ( CSPOSVersions::CheckSPOSVersion( dbNetwork.GetSPOSVersion( m_nNetworkIdx ), SPOS_V4 ) == FALSE )
		return COMMS_ERROR_CREATEFILE;
	
	PrepareFileList();
	if ( m_arrayFilenames.GetSize() == 0 )
		return COMMS_ERROR_NODATA;

	return COMMS_ERROR_NONE;
}

/**********************************************************************/

void CCommsExportButtonImages::PrepareFileList()
{
	m_arrayFilenames.RemoveAll();

	CFileFind FileFinder;
	bool bWorking = ( FileFinder.FindFile ( m_strSourcePath + "*.*" ) != 0 );
	
	int nPos = 0;
	while (bWorking)   		
	{
		( bWorking = FileFinder.FindNextFile() != 0 );

		//IGNORE CURRENT AND PARENT DIRECTORY MARKERS
		if ( FileFinder.IsDots() == TRUE )
			continue;

		//WE ONLY NEED TO CHECK FILES
		if ( FileFinder.IsDirectory() == TRUE )
			continue;

		CString strFilename = FileFinder.GetFileName();
		if ( strFilename.GetLength() < 5 )
			continue;

		strFilename.MakeUpper();
		
		bool bAccept = FALSE;
		if ( strFilename.Right(4) == ".BMP" )		bAccept = TRUE;
		else if ( strFilename.Right(4) == ".GIF" )	bAccept = TRUE;
		else if ( strFilename.Right(4) == ".JPG" )	bAccept = TRUE;

		if ( bAccept == FALSE )
			continue;

		m_arrayFilenames.Add( FileFinder.GetFileName() );
	}
}

/**********************************************************************/

const char* CCommsExportButtonImages::GetFilename( int n )
{
	if ( n >= 0 && n < m_arrayFilenames.GetSize() )
		m_strFilename = m_arrayFilenames.GetAt(n);
	else
		m_strFilename = "";

	return m_strFilename;
}

/**********************************************************************/
