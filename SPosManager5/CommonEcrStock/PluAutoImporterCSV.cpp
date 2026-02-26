/**********************************************************************/
#include "CustomExportManager.h"
#include "DatabaseCSVArray.h"
#include "..\SPosEcrManager5\DatabaseImportHandlerEcrman.h"
#include "..\SPosStockManager5\DatabaseImportHandlerStockman.h"
#include "FileRemover.h"
#include "ImportDefines.h"
#include "KeyboardHelpers.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "PluAutoImporterCSV.h"
/**********************************************************************/

CPluAutoImporterCSV::CPluAutoImporterCSV()
{
	m_nDbIdx = 0;

	m_bReadyToGo = FALSE;
}

/**********************************************************************/

void CPluAutoImporterCSV::CheckDbIdx()
{
	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		m_nDbIdx = 0;
}

/**********************************************************************/

void CPluAutoImporterCSV::GotoNextDb()
{
	m_nDbIdx++;
	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		m_nDbIdx = 0;
}

/**********************************************************************/

void CPluAutoImporterCSV::ImportPluIdleTime()
{
	if ( m_bReadyToGo == FALSE )
		return;

	if ( EcrmanOptions.GetPluCSVAutoImportFlag() == FALSE )
		return;

	CheckDbIdx();

	if ( dbDatabase.GetSize() == 0 )
		return;

	CReportConsolidationArray<CSortedStringItem> arrayFilenames;
	GetImportFileList( m_nDbIdx, arrayFilenames );

	if ( arrayFilenames.GetSize() == 0 )
		return;
	
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );

#ifdef STOCKMAN_SYSTEM
	arrayReadWrite.Add( NODE_STOCK );
#endif
	
	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_BARCODE );
	arrayReadOnly.Add( NODE_MODIFIER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		return;
		
	for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
	{
		CSortedStringItem item;
		arrayFilenames.GetAt( n, item );
		CString strFilename = item.m_strItem + ".CSV";
		ImportPluInternal( m_nDbIdx, strFilename );
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/

void CPluAutoImporterCSV::ImportPluExternal( int nDbIdx )
{
	if ( EcrmanOptions.GetPluCSVAutoImportFlag() == FALSE )
		return;

	CReportConsolidationArray<CSortedStringItem> arrayFilenames;
	GetImportFileList( nDbIdx, arrayFilenames );

	if ( arrayFilenames.GetSize() == 0 )
		return;
	
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );

#ifdef STOCKMAN_SYSTEM
	arrayReadWrite.Add( NODE_STOCK );
#endif
	
	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_BARCODE );
	arrayReadOnly.Add( NODE_MODIFIER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		return;
	
	for ( int n = 0; n < arrayFilenames.GetSize(); n++ )
	{
		CSortedStringItem item;
		arrayFilenames.GetAt( n, item );
		CString strFilename = item.m_strItem + ".CSV";
		ImportPluInternal( nDbIdx, strFilename );
	}

	DataManager.CloseDatabase();
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CPluAutoImporterCSV::ImportPluInternal( int nDbIdx, const char* szFilename )
{
	CDatabaseImportHandlerStockman ImportHandler;
	ImportHandler.SetPluImportMethod ( IMPORT_METHOD_ALL );
	ImportHandler.SetCreateStockFlag ( ( StockOptions.GetStockAutoCreateFlag() ) );	

	CString strImportFile = dbDatabase.GetFolderPathData( nDbIdx );
	strImportFile += "\\";
	strImportFile += szFilename;
	
	if ( ::FileExists( strImportFile ) == TRUE )
	{
		ImportHandler.ImportPluFile ( strImportFile );
			
		CFileRemover FileRemover ( strImportFile );
		
		if ( DataManager.Plu.IsModified() == TRUE || DataManager.Stock.IsModified() == TRUE )
		{
			CWaitCursor wait;

			CDataManagerInfo info;
			DataManager.WritePlu( info );
			DataManager.WriteStock( info, TRUE, TRUE );

			CCustomExportManagerPlu CustomExport;
		}

		CMessageLoggerInfo info;
		info.m_strMessage.Format ( "Auto Plu Import Db%3.3d (%s) (1:%d:%d:%d:%d:%d:%d)",
			dbDatabase.GetDbNo( nDbIdx ),
			dbDatabase.GetName( nDbIdx ),
			ImportHandler.GetImportTotalLines(),
			ImportHandler.GetImportIgnoredLines(),
			ImportHandler.GetImportPluAdded(),
			ImportHandler.GetImportPluModified(),
			ImportHandler.GetImportStockAdded(),
			ImportHandler.GetImportStockModified() );
		info.m_nHideLevel = 2;

		if ( ( ImportHandler.GetImportPluAdded() != 0 ) || ( ImportHandler.GetImportPluModified() != 0 ) )
			CKeyboardHelpers::RemovePluFile( nDbIdx );

		MessageLogger.LogSchedulerMessage ( info, FALSE, TRUE, FALSE );
	}
}

/**********************************************************************/
#else
/**********************************************************************/

void CPluAutoImporterCSV::ImportPluInternal( int nDbIdx, const char* szFilename )
{
	CDatabaseImportHandlerEcrman ImportHandler;
	ImportHandler.SetPluImportMethod ( IMPORT_METHOD_ALL );	

	CString strImportFile = dbDatabase.GetFolderPathData( nDbIdx );
	strImportFile += "\\";
	strImportFile += szFilename;
	
	ImportHandler.ImportPluFile ( strImportFile );		
	CFileRemover FileRemover ( strImportFile );
	
	if ( DataManager.Plu.IsModified() == TRUE )	
	{
		CWaitCursor wait;

		CDataManagerInfo info;
		DataManager.WritePlu( info );

		CCustomExportManagerPlu CustomExport;	
	}

	CMessageLoggerInfo info;
	info.m_strMessage.Format( "Auto Plu Import Db%3.3d (%s) (1:%d:%d:%d:%d)",
		dbDatabase.GetDbNo( nDbIdx ),
		dbDatabase.GetName( nDbIdx ),
		ImportHandler.GetImportTotalLines(),
		ImportHandler.GetImportIgnoredLines(),
		ImportHandler.GetImportPluAdded(),
		ImportHandler.GetImportPluModified() );
	info.m_nHideLevel = 2;

	if ( ( ImportHandler.GetImportPluAdded() != 0 ) || ( ImportHandler.GetImportPluModified() != 0 ) )
		CKeyboardHelpers::RemovePluFile( nDbIdx );

	MessageLogger.LogSchedulerMessage ( info, FALSE, TRUE, FALSE );
}

/**********************************************************************/
#endif
/**********************************************************************/

void CPluAutoImporterCSV::GetImportFileList( int nDbIdx, CReportConsolidationArray<CSortedStringItem>& arrayFilenames )
{
	arrayFilenames.RemoveAll();
	
	CString strPath = dbDatabase.GetFolderPathData( m_nDbIdx );

	CFileFind finder;
	BOOL bWorking = finder.FindFile ( strPath + "\\pluxfer*.csv" );

	while ( bWorking == TRUE )
	{
		bWorking = finder.FindNextFile();

		if ( finder.IsDots() == TRUE )
			continue;

		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();
		strFilename.MakeUpper();

		bool bAccept = FALSE;
		if ( strFilename == "PLUXFER.CSV" )
			bAccept = TRUE;
		else
		{
			if ( strFilename.GetLength() == 27 )
				if ( strFilename.Left(8) == "PLUXFER_" )
					if ( strFilename.Right(4) == ".CSV" )
						if ( strFilename.Mid(16,1) == "_" )
							if ( ::TestNumeric( strFilename.Mid(8,8) ) == TRUE )
								if ( ::TestNumeric( strFilename.Mid(17,6) ) == TRUE )
									bAccept = TRUE;

		}

		if ( TRUE == bAccept )
		{
			CSortedStringItem item;
			item.m_strItem = strFilename.Left( strFilename.GetLength() - 4 );
			arrayFilenames.Consolidate( item );
		}
	}
}

/**********************************************************************/


