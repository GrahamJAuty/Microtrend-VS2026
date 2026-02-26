/**********************************************************************/
#include "FileRemover.h"
#include "ImportDefines.h"
#include "LocationCSVArray.h"
#include "KeyboardHelpers.h"
#include "MessageLogger.h"
#include "PriceSetItemManager.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "PluAutoImporterEODPriceSet.h"
/**********************************************************************/

CPluAutoImporterEODPriceSet::CPluAutoImporterEODPriceSet()
{
	m_nLocIdx = 0;

	m_bReadyToGo = FALSE;
}

/**********************************************************************/

void CPluAutoImporterEODPriceSet::CheckLocIdx()
{
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CPluAutoImporterEODPriceSet::GotoNextLoc()
{
	m_nLocIdx++;
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CPluAutoImporterEODPriceSet::ImportPluIdleTime()
{
	if ( m_bReadyToGo == FALSE )
		return;

	CheckLocIdx();

	if ( dbLocation.GetSize() == 0 )
		return;

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo( m_nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		return;

	CArray<CPluAutoImporterEODInfoPriceSet,CPluAutoImporterEODInfoPriceSet> arrayInfo;
	
	CPluAutoImporterEODInfoPriceSet infoFile;
	infoFile.m_nDbIdx = nDbIdx;
	infoFile.m_nLocIdx = m_nLocIdx;
		
	CString strPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( m_nLocIdx );

	CFileFind finder;

	bool bWorking = ( finder.FindFile ( strPath + "\\PS_PROD*.CSV" ) != 0 );
	while (bWorking)   		
	{
		( bWorking = finder.FindNextFile() != 0 );

		if ( finder.IsDots() == TRUE )
			continue;
			
		if ( finder.IsDirectory() == TRUE )
			continue;

		infoFile.m_strFilename = finder.GetFileName();
		arrayInfo.Add( infoFile );
	}

	if ( arrayInfo.GetSize() != 0 )
	{
		for ( int n = 0; n < arrayInfo.GetSize(); n++ )
		{
			CDataManagerInfo infoDb;
			DataManager.OpenDatabaseReadOnly( nDbIdx, infoDb );

			PriceSetItemManager.SelectPriceSet ( m_nLocIdx, 1 );
			PriceSetItemManager.ReadPriceSetFile( TRUE, DataManager.Plu );

			CPluAutoImporterEODInfoPriceSet infoFile = arrayInfo.GetAt( n );
			ImportPluInternal( infoFile );

			DataManager.CloseDatabase();
		}
	}
}

/**********************************************************************/

void CPluAutoImporterEODPriceSet::ImportPluExternal( int nLocIdx )
{
	CArray<CPluAutoImporterEODInfoPriceSet,CPluAutoImporterEODInfoPriceSet> arrayInfo;

	int nDbIdx;
	int nDbNo = dbLocation.GetDbNo( nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		return;

	CPluAutoImporterEODInfoPriceSet infoFile;
	infoFile.m_nDbIdx = nDbIdx;
	infoFile.m_nLocIdx = nLocIdx;
	
	CString strPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( nLocIdx );

	CFileFind finder;

	bool bWorking = ( finder.FindFile ( strPath + "\\PROD*.CSV" ) != 0 );
	
	while (bWorking)   		
	{
		( bWorking = finder.FindNextFile() != 0 );

		if ( finder.IsDots() == TRUE )
			continue;
			
		if ( finder.IsDirectory() == TRUE )
			continue;

		infoFile.m_strFilename = finder.GetFileName();
		arrayInfo.Add( infoFile );
	}

	if ( arrayInfo.GetSize() != 0 )
	{
		for ( int n = 0; n < arrayInfo.GetSize(); n++ )
		{
			CDataManagerInfo infoDb;
			DataManager.OpenDatabaseReadOnly( nDbIdx, infoDb );

			PriceSetItemManager.SelectPriceSet ( nLocIdx, 1 );
			PriceSetItemManager.ReadPriceSetFile( TRUE, DataManager.Plu );

			CPluAutoImporterEODInfoPriceSet infoFile = arrayInfo.GetAt( n );
			ImportPluInternal( infoFile );

			DataManager.CloseDatabase();
		}
	}
}

/**********************************************************************/

void CPluAutoImporterEODPriceSet::ImportPluInternal( CPluAutoImporterEODInfoPriceSet& infoFile )
{
	int nLocIdx = infoFile.m_nLocIdx;
	CString strPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( nLocIdx );

	CString strImportFile = "";
	strImportFile += strPath;
	strImportFile += "\\";
	strImportFile += infoFile.m_strFilename;

	CSSFile fileImport;
	if ( fileImport.Open( strImportFile, "rb" ) == FALSE )
		return;

	int nImportTotalLines = 0;
	int nImportIgnoredLines = 0;
	int nImportPluAdded = 0;

	CEODPluImportInfo ImportInfo;
	ImportInfo.m_bImportIfExists = TRUE;
	ImportInfo.m_nExistingMask = dbLocation.GetProdCSVImportTypeMask( infoFile.m_nLocIdx );
	ImportInfo.m_nFieldMask = dbLocation.GetProdCSVProtectFieldMask( infoFile.m_nLocIdx );
	ImportInfo.m_bImportIfNew = FALSE;
	ImportInfo.m_nImportModifierMode = dbLocation.GetProdCSVImportModifierMode( infoFile.m_nLocIdx );
	
	//CANNOT CREATE NEW ITEMS IN PRICE SET
	if ( ImportInfo.m_nImportModifierMode == 2 )
	{
		ImportInfo.m_nImportModifierMode = 1;
	}

	CString strBuffer;
	while( fileImport.ReadString( strBuffer ) == TRUE )
	{
		nImportTotalLines++;
		
		bool bImportedThisLine = FALSE;
		
		ImportInfo.m_strImportLine = strBuffer;
 		DataManager.Plu.ImportEODPluLine( ImportInfo );

		if ( IMPORT_LINE_IGNORED != ImportInfo.m_nResult )
		{
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( ImportInfo.m_nLastBasePluNo, nPluIdx ) == TRUE )
			{
				PriceSetItemManager.SetPluStatus( nPluIdx, LOCALPLU_LOCAL );
				bImportedThisLine = TRUE;		
			}
		}

		if ( TRUE == bImportedThisLine )
			nImportPluAdded++;
		else
			nImportIgnoredLines++;
	}

	fileImport.Close();

	CFileRemover FileRemover ( strImportFile );
		
	if ( DataManager.Plu.IsModified() == TRUE )	
	{
		CWaitCursor wait;
		PriceSetItemManager.WritePriceSetFile();
	}

	CMessageLoggerInfo infoMsg;
	infoMsg.m_nHideLevel = 0;
	infoMsg.m_bAddNetworkName = FALSE;
	infoMsg.m_strMessage = "PROD.CSV Import";
	infoMsg.m_bLeadingLineSystem = TRUE;
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );

	infoMsg.m_strMessage.Format( "SOURCE: Nwk%3.3d Loc%3.3d (%s)",
		dbLocation.GetNetworkNo( m_nLocIdx ),
		dbLocation.GetLocNo( m_nLocIdx ),
		dbLocation.GetName( m_nLocIdx ) );
	infoMsg.m_bLeadingLineSystem = FALSE;
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );

	infoMsg.m_strMessage.Format( "DEST: Db%3.3d (%s)",
		dbDatabase.GetDbNo( infoFile.m_nDbIdx ),
		dbDatabase.GetName( infoFile.m_nDbIdx ) );
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );
	
	infoMsg.m_strMessage.Format( "RESULT: (%d:%d:%d)",
		nImportTotalLines,
		nImportIgnoredLines,
		nImportPluAdded );

	infoMsg.m_bFollowingLineSystem = TRUE;
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );

	if ( nImportPluAdded != 0 )
		CKeyboardHelpers::RemovePluFile( infoFile.m_nDbIdx );
}

/**********************************************************************/

