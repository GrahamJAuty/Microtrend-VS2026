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
#include "BarcodeAutoImporter.h"
/**********************************************************************/

CBarcodeAutoImporter::CBarcodeAutoImporter()
{
	m_nDbIdx = 0;

	m_bReadyToGo = FALSE;

	m_strImportFile = "";
}

/**********************************************************************/

void CBarcodeAutoImporter::CheckDbIdx()
{
	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		m_nDbIdx = 0;
}

/**********************************************************************/

void CBarcodeAutoImporter::GotoNextDb()
{
	m_nDbIdx++;
	if ( m_nDbIdx < 0 || m_nDbIdx >= dbDatabase.GetSize() )
		m_nDbIdx = 0;
}

/**********************************************************************/

void CBarcodeAutoImporter::ImportBarcodeIdleTime()
{
	if ( m_bReadyToGo == FALSE )
		return;

	if ( EcrmanOptions.GetPluCSVAutoImportFlag() == FALSE )
		return;

	CheckDbIdx();

	if ( dbDatabase.GetSize() == 0 )
		return;
	
	m_strImportFile = dbDatabase.GetFolderPathData( m_nDbIdx );
	m_strImportFile += "\\BarXFer.csv";

	if ( ::FileExists( m_strImportFile ) == FALSE )
		return;
	
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );

#ifdef STOCKMAN_SYSTEM
	arrayReadWrite.Add( NODE_STOCK );
#endif
	
	arrayReadWrite.Add( NODE_BARCODE );

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_MODIFIER );

	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( m_nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		return;
		
	ImportBarcodeInternal( m_nDbIdx );
	DataManager.CloseDatabase();
}

/**********************************************************************/

void CBarcodeAutoImporter::ImportBarcodeExternal( int nDbIdx )
{
	if ( EcrmanOptions.GetPluCSVAutoImportFlag() == FALSE )
		return;

	//THIS FUNCTION IS CALLED FROM OUTSIDE THE CLASS
	//SO WE NEED TO SET THE IMPORT FILE MEMBER AGAIN
	m_strImportFile = dbDatabase.GetFolderPathData( nDbIdx );
	m_strImportFile += "\\BarXFer.csv";

	if ( ::FileExists( m_strImportFile ) == FALSE )
		return;
	
	CWordArray arrayReadWrite;
	arrayReadWrite.Add( NODE_PLU );

#ifdef STOCKMAN_SYSTEM
	arrayReadWrite.Add( NODE_STOCK );
#endif
	
	arrayReadWrite.Add( NODE_BARCODE );

	CWordArray arrayReadOnly;
	arrayReadOnly.Add( NODE_MODIFIER );
	
	CDataManagerInfo info;
	if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
		return;
	
	ImportBarcodeInternal( nDbIdx );
	DataManager.CloseDatabase();
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CBarcodeAutoImporter::ImportBarcodeInternal( int nDbIdx )
{
	CDatabaseImportHandlerStockman ImportHandler;
	ImportHandler.SetPluImportMethod ( IMPORT_METHOD_ALL );
	ImportHandler.SetCreateStockFlag ( ( StockOptions.GetStockAutoCreateFlag() ) );			
	
	if ( ::FileExists( m_strImportFile ) == TRUE )
	{
		ImportHandler.ImportPluBarcodeFile ( m_strImportFile );
			
		CFileRemover FileRemover ( m_strImportFile );
		
		if ( DataManager.Plu.IsModified() == TRUE || DataManager.Stock.IsModified() == TRUE || DataManager.Barcode.IsModified() == TRUE )
		{
			CWaitCursor wait;

			CDataManagerInfo info;
			DataManager.WritePlu( info );
			DataManager.WriteStock( info, TRUE, TRUE );
			DataManager.WriteBarcode( info );

			CCustomExportManagerPlu CustomExport;
		}

		CMessageLoggerInfo info;
		info.m_strMessage.Format ( "Auto Barcode Import Db%3.3d (%s) (1:%d:%d:%d:%d:%d:%d:%d:%d)",
			dbDatabase.GetDbNo( nDbIdx ),
			dbDatabase.GetName( nDbIdx ),
			ImportHandler.GetImportTotalLines(),
			ImportHandler.GetImportIgnoredLines(),
			ImportHandler.GetImportPluAdded(),
			ImportHandler.GetImportPluModified(),
			ImportHandler.GetImportStockAdded(),
			ImportHandler.GetImportStockModified(),
			ImportHandler.GetImportBarcodeAdded(),
			ImportHandler.GetImportBarcodeModified() );
		info.m_nHideLevel = 2;

		MessageLogger.LogSchedulerMessage ( info, FALSE, TRUE, FALSE );
	}
}

/**********************************************************************/
#else
/**********************************************************************/

void CBarcodeAutoImporter::ImportBarcodeInternal( int nDbIdx )
{
	CDatabaseImportHandlerEcrman ImportHandler;
	ImportHandler.SetPluImportMethod ( IMPORT_METHOD_ALL );			
	
	if ( ::FileExists( m_strImportFile ) == TRUE )
	{
		ImportHandler.ImportPluBarcodeFile ( m_strImportFile );		
		CFileRemover FileRemover ( m_strImportFile );
		
		if ( DataManager.Plu.IsModified() == TRUE || DataManager.Barcode.IsModified() == TRUE )	
		{
			CWaitCursor wait;

			CDataManagerInfo info;
			DataManager.WritePlu( info );
			DataManager.WriteBarcode( info );

			CCustomExportManagerPlu CustomExport;	
		}

		CMessageLoggerInfo info;
		info.m_strMessage.Format( "Auto Barcode Import Db%3.3d (%s) (1:%d:%d:%d:%d:%d:%d)",
			dbDatabase.GetDbNo( nDbIdx ),
			dbDatabase.GetName( nDbIdx ),
			ImportHandler.GetImportTotalLines(),
			ImportHandler.GetImportIgnoredLines(),
			ImportHandler.GetImportPluAdded(),
			ImportHandler.GetImportPluModified(),
			ImportHandler.GetImportBarcodeAdded(),
			ImportHandler.GetImportBarcodeModified() );
		info.m_nHideLevel = 2;

		MessageLogger.LogSchedulerMessage ( info, FALSE, TRUE, FALSE );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/




