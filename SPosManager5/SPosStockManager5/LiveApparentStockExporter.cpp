/**********************************************************************/
#include "..\CommonEcrStock\CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "FileRemover.h"
/**********************************************************************/
#include "LiveApparentStockExporter.h"
/**********************************************************************/

CLiveApparentStockExporter::CLiveApparentStockExporter(void)
{
}

/**********************************************************************/

void CLiveApparentStockExporter::CheckAllDatabases()
{
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		CheckDatabase( nDbIdx );
}

/**********************************************************************/

void CLiveApparentStockExporter::CheckDatabase( int nDbIdx )
{
	CFilenameUpdater FnUp1( SysFiles::ExportLiveApptStockToken, nDbIdx );
	CFilenameUpdater FnUp2( SysFiles::ExportLiveApptStockLock, nDbIdx );
	
	CString strFilename1 = FnUp1.GetFilenameToUse();
	if ( ::FileExists( strFilename1 ) == FALSE )
		return;
	
	CSSFile fileLock;
	if ( fileLock.Open( FnUp2.GetFilenameToUse(), "wb" ) == TRUE )
	{
		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadOnly( nDbIdx, info, TRUE ) == TRUE )
		{
			CCustomExportManagerPlu( TRUE, TRUE );
			CFileRemover FileRemover( strFilename1 );
		}
	}
}

/**********************************************************************/
