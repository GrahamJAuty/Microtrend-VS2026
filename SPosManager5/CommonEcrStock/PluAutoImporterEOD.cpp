/**********************************************************************/
#include "CustomExportManager.h"
#include "FileRemover.h"
#include "ImportDefines.h"
#include "LocationCSVArray.h"
#include "KeyboardHelpers.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "ReportHelpers.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluAutoImporterEOD.h"
/**********************************************************************/

CPluAutoImporterEOD::CPluAutoImporterEOD()
{
	m_nLocIdx = 0;

	m_bReadyToGo = FALSE;
}

/**********************************************************************/

void CPluAutoImporterEOD::CheckLocIdx()
{
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CPluAutoImporterEOD::GotoNextLoc()
{
	m_nLocIdx++;
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		m_nLocIdx = 0;
}

/**********************************************************************/

void CPluAutoImporterEOD::ImportPluIdleTime()
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

	CArray<CPluAutoImporterEODInfo,CPluAutoImporterEODInfo> arrayInfo;
	
	CPluAutoImporterEODInfo infoFile;
	infoFile.m_nDbIdx = nDbIdx;
	infoFile.m_nLocIdx = m_nLocIdx;
		
	CString strPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( m_nLocIdx );

	CFileFind finder;

	bool bWorking = ( finder.FindFile ( strPath + "\\PROD*.CSV" ) != 0 );
	while (bWorking)   		
	{
		( bWorking = finder.FindNextFile() != 0 );

		if ( finder.IsDots() == TRUE )
			continue;
			
		if ( finder.IsDirectory() == TRUE )
			continue;

		CString strFilename = finder.GetFileName();

		infoFile.m_strFilenameOriginal = strFilename;
		infoFile.m_strFilenamePriceSet = "PS_";
		infoFile.m_strFilenamePriceSet += strFilename;
		
		arrayInfo.Add( infoFile );
	}

	if ( arrayInfo.GetSize() != 0 )
	{
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );

#ifdef STOCKMAN_SYSTEM
		arrayReadWrite.Add( NODE_STOCK );
#endif
		
		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_BARCODE );
		arrayReadOnly.Add( NODE_MODIFIER );
		arrayReadOnly.Add( NODE_DEPARTMENT );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
			return;

		for ( int n = 0; n < arrayInfo.GetSize(); n++ )
		{
			CPluAutoImporterEODInfo infoFile = arrayInfo.GetAt( n );
			ImportPluInternal( infoFile );
		}
	
		DataManager.CloseDatabase();
	}
}

/**********************************************************************/

void CPluAutoImporterEOD::ImportPluExternal( int nDbIdx )
{
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );

	CArray<CPluAutoImporterEODInfo,CPluAutoImporterEODInfo> arrayInfo;

	CPluAutoImporterEODInfo infoFile;
	infoFile.m_nDbIdx = nDbIdx;
	
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		if ( dbLocation.GetDbNo( nLocIdx ) == nDbNo )
		{
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

				CString strFilename = finder.GetFileName();

				infoFile.m_strFilenameOriginal = strFilename;
				infoFile.m_strFilenamePriceSet = "PS_";
				infoFile.m_strFilenamePriceSet += strFilename;
		
				arrayInfo.Add( infoFile );
			}
		}
	}

	if ( arrayInfo.GetSize() > 0 )
	{
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );

#ifdef STOCKMAN_SYSTEM
		arrayReadWrite.Add( NODE_STOCK );
#endif
	
		CWordArray arrayReadOnly;
		arrayReadOnly.Add( NODE_BARCODE );
		arrayReadOnly.Add( NODE_MODIFIER );
		arrayReadOnly.Add( NODE_DEPARTMENT );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( nDbIdx, arrayReadWrite, arrayReadOnly, info, FALSE ) == FALSE )
			return;

		for ( int n = 0; n < arrayInfo.GetSize(); n++ )
		{
			CPluAutoImporterEODInfo infoFile = arrayInfo.GetAt( n );
			ImportPluInternal( infoFile );
		}

		DataManager.CloseDatabase();
	}
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CPluAutoImporterEOD::ImportPluInternal( CPluAutoImporterEODInfo& infoFile )
{
	int nLocIdx = infoFile.m_nLocIdx;
	CString strPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( nLocIdx );

	CString strImportFileOriginal = "";
	strImportFileOriginal += strPath;
	strImportFileOriginal += "\\";
	strImportFileOriginal += infoFile.m_strFilenameOriginal;

	CString strImportFilePriceSet = "";
	strImportFilePriceSet += strPath;
	strImportFilePriceSet += "\\";
	strImportFilePriceSet += infoFile.m_strFilenamePriceSet;
	
	CSSFile fileImport;
	if ( fileImport.Open( strImportFileOriginal, "rb" ) == FALSE )
		return;

	int nImportTotalLines = 0;
	int nImportPluInvalid = 0;
	int nImportPluIgnored = 0;
	int nImportPluModified = 0;
	int nImportPluAdded = 0;
	int nImportStockAdded = 0;

	CEODPluImportInfo ImportInfo;
	ImportInfo.m_bImportIfExists = ( dbLocation.GetProdCSVImportExistingMode( nLocIdx ) == 1 );
	ImportInfo.m_nExistingMask = dbLocation.GetProdCSVImportTypeMask(nLocIdx);
	ImportInfo.m_nFieldMask = dbLocation.GetProdCSVProtectFieldMask( nLocIdx );
	ImportInfo.m_nImportModifierMode = dbLocation.GetProdCSVImportModifierMode( nLocIdx );
	ImportInfo.m_bImportIfNew = TRUE;
	
	bool bForwardToPriceSet = ( dbLocation.GetProdCSVImportExistingMode( nLocIdx ) == 2 );
	
	CString strBuffer;
	while( fileImport.ReadString( strBuffer ) == TRUE )
	{
		nImportTotalLines++;

		ImportInfo.m_strImportLine = strBuffer;
		DataManager.Plu.ImportEODPluLine( ImportInfo );

		switch( ImportInfo.m_nResult )
		{
		case IMPORT_LINE_ADDED:		nImportPluAdded++;		break;
		case IMPORT_LINE_MODIFIED:	nImportPluModified++;	break;
		case IMPORT_LINE_IGNORED:	nImportPluIgnored++;	break;
		case IMPORT_LINE_INVALID:	nImportPluInvalid++;	break;
		}

		if ( IMPORT_LINE_ADDED == ImportInfo.m_nResult )
		{
			CPluCSVRecord PluRecord;
			PluRecord.SetPluNo( ImportInfo.m_nLastBasePluNo );
			CString strPluNo = PluRecord.GetPluNoString();

			if ( StockOptions.GetStockAutoCreateFlag() == FALSE )
			{
				CRecipeCSVRecord RecipeRecord;
				RecipeRecord.SetRecipePluNo( strPluNo );
				DataManager.Plu.SetRecipe( -1, RecipeRecord );
			}
			else
			{
				bool bStockTemplate = FALSE;
				__int64 nTemplatePluNo = 0;
				int nTemplatePluIdx = 0;
				bool bTemplateDescription = FALSE;
				bool bTemplateCategory = FALSE;
				bool bTemplateQty = FALSE;
				
				int nDeptIdx;
				if ( DataManager.Department.FindDeptByNumber( ImportInfo.m_nNewPluDeptNo, nDeptIdx ) == TRUE )
				{
					CDepartmentCSVRecord Department;
					DataManager.Department.GetAt( nDeptIdx, Department );
					
					if ( Department.GetStockTemplateFlag() == TRUE )
					{
						CSQLPluNoInfo infoPluNo;
						infoPluNo.m_nEposPluNo = Department.GetSpecimenLearnPlu();
						::ProcessPluNo( infoPluNo );

						nTemplatePluNo = infoPluNo.m_nBasePluNo;
						bStockTemplate =( DataManager.Plu.FindPluByNumber( nTemplatePluNo, nTemplatePluIdx ) );
						bTemplateDescription = Department.GetStockDescriptionFlag();
						bTemplateCategory = Department.GetStockCategoryFlag();
						bTemplateQty = Department.GetStockQtyFlag();
					}
				}
				
				if ( FALSE == bStockTemplate )
				{
					CRecipeCSVRecord RecipeRecord;
					RecipeRecord.SetRecipePluNo ( strPluNo );
					RecipeRecord.AddStockCode ( strPluNo, 1.0 );
					DataManager.Plu.SetRecipe ( -1, RecipeRecord );
		
					//CREATE THE STOCK ITEM IF IT DOES NOT EXIST ALREADY
					CStockCSVRecord StockRecord;
					StockRecord.SetStockCode ( strPluNo );
					StockRecord.SetDescription ( ImportInfo.m_strNewPluText );
					StockRecord.SetCategory ( ImportInfo.m_nNewPluDeptNo );
					
					int nStockIdx;
					if ( DataManager.Stock.InsertRecord ( StockRecord, nStockIdx ) == TRUE )
						nImportStockAdded++;
				}
				else
				{
					CRecipeCSVRecord RecipeTemplate;
					DataManager.Plu.GetRecipe( nTemplatePluIdx, nTemplatePluNo, RecipeTemplate );

					CRecipeCSVRecord RecipeNew;
					RecipeNew.SetRecipePluNo( strPluNo );

					switch ( RecipeTemplate.GetSize() )
					{
					case 0:	
						if ( RecipeTemplate.GetPluStockLinkFlag() == FALSE )
							RecipeNew.DisablePluStockLink();			
						break;
					
					case 1:	
						if ( FALSE == bTemplateQty )
							RecipeNew.AddStockCode( strPluNo, 1.0 );	
						else
							RecipeNew.AddStockCode( strPluNo, RecipeTemplate.GetStockQty(0) );
						break;
					}

					DataManager.Plu.SetRecipe ( -1, RecipeNew );

					if ( RecipeTemplate.GetSize() == 1 )
					{
						CStockCSVRecord StockRecord;

						int nTemplateStockIdx;
						if ( DataManager.Stock.FindStockItemByCode( RecipeTemplate.GetStockCode(0), nTemplateStockIdx ) == TRUE )
							DataManager.Stock.GetAt( nTemplateStockIdx, StockRecord );
						
						StockRecord.SetStockCode ( strPluNo );

						if ( FALSE == bTemplateDescription )
							StockRecord.SetDescription ( ImportInfo.m_strNewPluText );
						
						if ( FALSE == bTemplateCategory )
							StockRecord.SetCategory ( ImportInfo.m_nNewPluDeptNo );
						
						int nStockIdx;
						if ( DataManager.Stock.InsertRecord ( StockRecord, nStockIdx ) == TRUE )
							nImportStockAdded++;
					}
				}
			}
		}
	}

	fileImport.Close();	
	
	if ( TRUE == bForwardToPriceSet )
		rename( strImportFileOriginal, strImportFilePriceSet );
	else
		CFileRemover FileRemover ( strImportFileOriginal );
		
	if ( DataManager.Plu.IsModified() == TRUE || DataManager.Stock.IsModified() == TRUE )
	{
		CWaitCursor wait;

		CDataManagerInfo info;
		DataManager.WritePlu( info );
		DataManager.WriteStock( info, TRUE, TRUE );

		CCustomExportManagerPlu CustomExport;
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

	infoMsg.m_strMessage.Format( "RESULT: (%d:%d:%d:%d:%d:%d)",
		nImportTotalLines,
		nImportPluAdded,
		nImportPluModified,
		nImportPluIgnored,
		nImportPluInvalid,
		nImportStockAdded );

	infoMsg.m_bFollowingLineSystem = TRUE;
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );

	if ( ( nImportPluAdded != 0 ) || ( nImportPluModified != 0 ) )
		CKeyboardHelpers::RemovePluFile( infoFile.m_nDbIdx );
}

/**********************************************************************/
#else
/**********************************************************************/

void CPluAutoImporterEOD::ImportPluInternal( CPluAutoImporterEODInfo& infoFile )
{
	int nLocIdx = infoFile.m_nLocIdx;
	CString strPath = dbLocation.GetFolderPathSyssetPendingProductsLocal( nLocIdx );

	CString strImportFileOriginal = "";
	strImportFileOriginal += strPath;
	strImportFileOriginal += "\\";
	strImportFileOriginal += infoFile.m_strFilenameOriginal;

	CString strImportFilePriceSet = "";
	strImportFilePriceSet += strPath;
	strImportFilePriceSet += "\\";
	strImportFilePriceSet += infoFile.m_strFilenamePriceSet;

	CSSFile fileImport;
	if ( fileImport.Open( strImportFileOriginal, "rb" ) == FALSE )
		return;

	int nImportTotalLines = 0;
	int nImportPluInvalid = 0;
	int nImportPluIgnored = 0;
	int nImportPluAdded = 0;
	int nImportPluModified = 0;

	CEODPluImportInfo ImportInfo;
	ImportInfo.m_bImportIfExists = ( dbLocation.GetProdCSVImportExistingMode( nLocIdx ) == 1 );
	ImportInfo.m_nExistingMask = dbLocation.GetProdCSVImportTypeMask( nLocIdx );
	ImportInfo.m_nFieldMask = dbLocation.GetProdCSVProtectFieldMask( nLocIdx );
	ImportInfo.m_bImportIfNew = TRUE;
	ImportInfo.m_nImportModifierMode = dbLocation.GetProdCSVImportModifierMode( nLocIdx );
	
	bool bForwardToPriceSet = ( dbLocation.GetProdCSVImportExistingMode( nLocIdx ) == 2 );
	
	CString strBuffer;
	while( fileImport.ReadString( strBuffer ) == TRUE )
	{
		nImportTotalLines++;

		ImportInfo.m_strImportLine = strBuffer;
		DataManager.Plu.ImportEODPluLine( ImportInfo );

		switch( ImportInfo.m_nResult )
		{
		case IMPORT_LINE_ADDED:		nImportPluAdded++;		break;
		case IMPORT_LINE_MODIFIED:	nImportPluModified++;	break;
		case IMPORT_LINE_IGNORED:	nImportPluIgnored++;	break;
		case IMPORT_LINE_INVALID:	nImportPluInvalid++;	break;
		}
	}

	fileImport.Close();

	if ( TRUE == bForwardToPriceSet )
		::CopyFile( strImportFileOriginal, strImportFilePriceSet, FALSE );
	
	CFileRemover FileRemover ( strImportFileOriginal );
		
	if ( DataManager.Plu.IsModified() == TRUE )	
	{
		CWaitCursor wait;

		CDataManagerInfo info;
		DataManager.WritePlu( info );

		CCustomExportManagerPlu CustomExport;	
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
	
	infoMsg.m_strMessage.Format( "RESULT: (%d:%d:%d:%d:%d)",
		nImportTotalLines,
		nImportPluAdded,
		nImportPluModified,
		nImportPluIgnored,
		nImportPluInvalid );

	infoMsg.m_bFollowingLineSystem = TRUE;
	MessageLogger.LogSchedulerMessage ( infoMsg, FALSE, TRUE, FALSE );

	if ( ( nImportPluAdded != 0 ) || ( nImportPluModified != 0 ) )
		CKeyboardHelpers::RemovePluFile( infoFile.m_nDbIdx );
}

/**********************************************************************/
#endif
/**********************************************************************/

