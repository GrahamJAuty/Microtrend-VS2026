/**********************************************************************/
#include "FileRemover.h"
#include "ImportDefines.h"
#include "LocationCSVArray.h"
#include "MessageLogger.h"
#include "PluDatabaseHeader.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PriceSetItemManager.h"
/**********************************************************************/

CPriceSetItemManager::CPriceSetItemManager()
{
	Reset();	
}

/**********************************************************************/

void CPriceSetItemManager::Reset()
{
	SelectPriceSet ( -1, -1 );
	m_nPluIdx = -1;
	m_bIsActive = FALSE;
	m_arrayPluStatus.RemoveAll();
	m_arrayMasterRecord.RemoveAll();
	m_strPriceSetHeader = "";
}

/**********************************************************************/

void CPriceSetItemManager::SelectPriceSet( int nLocIdx, int nSet )
{
	m_arrayFields.RemoveAll();
	m_strPriceSetHeader = "";
	
	m_strFieldFilename = "";
	m_strExcludeFilename = "";
	m_strIncludeFilename = "";
	m_strTempFilename = "";
	m_nLocIdx = nLocIdx;
	m_nPriceSet = nSet;
	m_bPluStatusModified = FALSE;

	bool bIgnoreRequest = FALSE;
	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		bIgnoreRequest = TRUE;
	else if ( EcrmanOptions.GetPluPriceSetsFlag() == FALSE )
		bIgnoreRequest = TRUE;
	else if ( m_nPriceSet < 1 || m_nPriceSet > 3 )
		bIgnoreRequest = TRUE;

	if ( bIgnoreRequest == TRUE )
	{
		m_nLocIdx = -1;
		m_nPriceSet = -1;
		m_strPriceSetNameAndNumber = "";
		return;
	}
	
	int nDbPriceSet = 0;
	switch( m_nPriceSet )
	{
	case 1:	nDbPriceSet = dbLocation.GetDbPriceSet1( m_nLocIdx );	break;
	case 2:	nDbPriceSet = dbLocation.GetDbPriceSet2( m_nLocIdx );	break;
	case 3:	nDbPriceSet = dbLocation.GetDbPriceSet3( m_nLocIdx );	break;
	}

	if ( nDbPriceSet == 0 )
		m_strPriceSetNameAndNumber.Format ( "%s Price Set %d", dbLocation.GetName( m_nLocIdx ), m_nPriceSet );	
	else
		m_strPriceSetNameAndNumber.Format ( "Database Price Set %d", nDbPriceSet );

	m_strFieldFilename = dbLocation.GetFilePathDataLocalPluField( m_nLocIdx, m_nPriceSet );
	m_strExcludeFilename = dbLocation.GetFilePathDataLocalPluExclude( m_nLocIdx );
	m_strIncludeFilename = dbLocation.GetFilePathDataLocalPluInclude( m_nLocIdx );
	m_strLockFilename = m_strFieldFilename + ".XXX";

	CSSFile fileHeader; 
	if ( fileHeader.Open ( m_strFieldFilename, "rb" ) == FALSE )
		return;

	CString strHeader;
	fileHeader.ReadString ( strHeader );
	fileHeader.Close();

	strHeader = PluDatabaseHeader.GetUpdatedHeader( strHeader );

	::TrimSpaces ( strHeader, FALSE );
	CCSV csvIn ( strHeader );
	CCSV csvOut;

	for ( int nIndex = 0; nIndex < csvIn.GetSize(); nIndex++ )
	{
		CString strField = csvIn.GetString(nIndex);
		::TrimSpaces ( strField, FALSE );
		
		if ( strField != "" )
		{
			csvOut.Add ( strField );
			strField.MakeUpper();

			CSortedStringItem item;
			item.m_strItem = strField;
			m_arrayFields.Consolidate ( item );
		}
	}

	m_strPriceSetHeader = csvOut.GetLine();
}

/**********************************************************************/

bool CPriceSetItemManager::CheckField ( const char* szField )
{
	CString strField = szField;
	::TrimSpaces ( strField, FALSE );
	strField.MakeUpper();

	CSortedStringItem item;
	item.m_strItem = strField;

	int nPos = 0;
	return m_arrayFields.Find ( item, nPos );
}

/**********************************************************************/

bool CPriceSetItemManager::IsPluMaster()
{
	return ( ( EcrmanOptions.GetPluPriceSetsFlag() == FALSE ) || ( m_nLocIdx == -1 ) );
}

/**********************************************************************/

bool CPriceSetItemManager::HasLocalFields()
{
	return ( m_arrayFields.GetSize() != 0 );
}

/**********************************************************************/

BYTE CPriceSetItemManager::GetPluStatus ( int nPluIdx )
{
	BYTE byStatus = LOCALPLU_MASTER;
	if ( nPluIdx >= 0 && nPluIdx < m_arrayPluStatus.GetSize() )
		byStatus = m_arrayPluStatus.GetAt ( nPluIdx );

	return byStatus;
}

/**********************************************************************/

void CPriceSetItemManager::SetPluStatus ( int nPluIdx, BYTE byStatus )
{
	if ( nPluIdx >= 0 && nPluIdx < m_arrayPluStatus.GetSize() )
	{
		if ( m_arrayPluStatus.GetAt ( nPluIdx ) != byStatus )
		{
			m_arrayPluStatus.SetAt ( nPluIdx, byStatus );
			m_bPluStatusModified = TRUE;
		}
	}
}

/**********************************************************************/

bool CPriceSetItemManager::GetPluStatusModifiedFlag()
{
	bool bResult = ( ( IsPluMaster() == TRUE ) ? FALSE : m_bPluStatusModified );
	return bResult;
}

/**********************************************************************/

const char* CPriceSetItemManager::GetSavedShelfEdgeLabelsFilename()
{
	m_strShelfListFilename = "";
	if ( IsPluMaster() == TRUE )
	{
		int nDbIdx = DataManager.GetActiveDbIdx();
		if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
		{
			CFilenameUpdater FnUp ( SysFiles::DbSavedShelfLabels, nDbIdx );
			m_strShelfListFilename = FnUp.GetFilenameToUse();
		}
	}
	else
	{
		if ( m_nLocIdx >= 0 && m_nLocIdx < dbLocation.GetSize() )
		{
			CFilenameUpdater FnUp ( SysFiles::LocSavedShelfLabels, m_nLocIdx, m_nPriceSet );
			m_strShelfListFilename = FnUp.GetFilenameToUse();
		}
	}
	
	return m_strShelfListFilename;
}

/**********************************************************************/

const char* CPriceSetItemManager::GetSavedProductLabelsFilename()
{
	m_strBarcodeListFilename = "";
	if ( IsPluMaster() == TRUE )
	{
		int nDbIdx = DataManager.GetActiveDbIdx();
		if ( nDbIdx >= 0 && nDbIdx < dbDatabase.GetSize() )
		{
			CFilenameUpdater FnUp ( SysFiles::DbSavedProductLabels, nDbIdx );
			m_strBarcodeListFilename = FnUp.GetFilenameToUse();
		}
	}
	else
	{
		if ( m_nLocIdx >= 0 && m_nLocIdx < dbLocation.GetSize() )
		{
			CFilenameUpdater FnUp ( SysFiles::LocSavedProductLabels, m_nLocIdx, m_nPriceSet );
			m_strBarcodeListFilename = FnUp.GetFilenameToUse();
		}
	}
	
	return m_strBarcodeListFilename;
}

/**********************************************************************/
#if defined (STOCKMAN_SYSTEM)
/**********************************************************************/
void CPriceSetItemManager::ReadPriceSetFile( bool bForDownload, CPluMonitorStockman& PluMonitor )
/**********************************************************************/
#elif defined (POSTRAY_UTILITY)
/**********************************************************************/
void CPriceSetItemManager::ReadPriceSetFile( bool bForDownload, CPluMonitorPosTray& PluMonitor )
/**********************************************************************/
#else
/**********************************************************************/
void CPriceSetItemManager::ReadPriceSetFile( bool bForDownload, CPluMonitorEcrman& PluMonitor )
/**********************************************************************/
#endif
/**********************************************************************/
{	
	m_bPluStatusModified = FALSE;

	if ( m_strFieldFilename == "" || IsPluMaster() == TRUE ) 
		return;

	CString strHeader = PluDatabaseHeader.GetCompatibilityHeader( m_strFieldFilename );

	m_arrayPluStatus.RemoveAll();
	m_arrayMasterRecord.RemoveAll();

	bool bDefaultToExclude = DealerFlags.GetPriceSetExcludeByDefaultFlag();
	
	if ( PluMonitor.GetSize() > 0 )
	{
		if ( bDefaultToExclude == TRUE )
			m_arrayPluStatus.InsertAt ( 0, LOCALPLU_EXCLUDE, PluMonitor.GetSize() );
		else
			m_arrayPluStatus.InsertAt ( 0, LOCALPLU_MASTER, PluMonitor.GetSize() );

		m_arrayMasterRecord.InsertAt( 0, "", PluMonitor.GetSize() );
	}
	
	CString strFilename;
	int nStatus;

	if ( bDefaultToExclude == TRUE )
	{
		strFilename = m_strIncludeFilename;
		nStatus = LOCALPLU_MASTER;
	}
	else
	{
		strFilename = m_strExcludeFilename;
		nStatus = LOCALPLU_EXCLUDE;
	}

	CSSFile fileIncludeExclude;
	if ( fileIncludeExclude.Open ( strFilename, "rb" ) == TRUE )	
	{
		CString strBuffer;

		StatusProgress.Lock( TRUE, "Reading excluded items list" );
		while ( fileIncludeExclude.ReadString ( strBuffer ) == TRUE )
		{	
			StatusProgress.SetPos ( fileIncludeExclude );
			
			int nPluIdx;
			if ( PluMonitor.FindPluByNumber ( _atoi64(strBuffer), nPluIdx ) == TRUE )
				m_arrayPluStatus.SetAt ( nPluIdx, nStatus );
		}
		StatusProgress.Unlock();
	}
	
	CSSImportFile fileField ( strHeader );
	if ( fileField.Open ( m_strFieldFilename, "rb" ) == TRUE )	
	{
		CString strBuffer;

		StatusProgress.Lock( TRUE, "Reading price set file" );
		while ( fileField.ReadData ( strBuffer ) == TRUE )
		{	
			StatusProgress.SetPos ( fileField );
			
#if defined (STOCKMAN_SYSTEM) || defined (POSTRAY_UTILITY)
			int nImportPos;
			CString strOldRecord;
			if ( PluMonitor.ImportLinePriceSet ( nImportPos, fileField, strBuffer, strOldRecord ) == IMPORT_LINE_MODIFIED )
			{
				if ( nImportPos >= 0 && nImportPos < m_arrayPluStatus.GetSize() )
				{
					if ( m_arrayPluStatus.GetAt( nImportPos ) != LOCALPLU_EXCLUDE )
					{
						m_arrayPluStatus.SetAt ( nImportPos, LOCALPLU_LOCAL );
					
						if ( bForDownload == TRUE )
						{
							CPluCSVRecord PluRecord;
							PluMonitor.GetAt ( nImportPos, PluRecord );
							PluRecord.SetModifiedDateTimePlu();
							PluMonitor.SetAt ( nImportPos, PluRecord );
						}
					}
				}

				if ( nImportPos >= 0 && nImportPos < m_arrayMasterRecord.GetSize() )	
					m_arrayMasterRecord.SetAt( nImportPos, strOldRecord );
			}
#else
			int nImportPos;
			CString strOldRecord;
			if ( PluMonitor.ImportLinePriceSet ( nImportPos, fileField, strBuffer, strOldRecord ) == IMPORT_LINE_MODIFIED )
			{
				if ( nImportPos >= 0 && nImportPos < m_arrayPluStatus.GetSize() )
				{
					if ( m_arrayPluStatus.GetAt( nImportPos ) != LOCALPLU_EXCLUDE )
					{
						m_arrayPluStatus.SetAt ( nImportPos, LOCALPLU_LOCAL );
					
						if ( bForDownload == TRUE )
						{
							CPluCSVRecord PluRecord;
							PluMonitor.GetAt ( nImportPos, PluRecord );
							PluRecord.SetModifiedDateTimePlu();
							PluMonitor.SetAt ( nImportPos, PluRecord );
						}
					}
				}

				if ( nImportPos >= 0 && nImportPos < m_arrayMasterRecord.GetSize() )
					m_arrayMasterRecord.SetAt( nImportPos, strOldRecord );
			}
#endif
		}
		StatusProgress.Unlock();
	}

	m_bIsActive = TRUE;
}

/**********************************************************************/

void CPriceSetItemManager::ConvertIncludeExcludeFiles()
{
	StatusProgress.Lock( TRUE, "Updating excluded items list" );
		
	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

		CArray<int,int> arrayLocIdx;
		dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

		__int64 nProgressTarget = DataManager.Plu.GetSize() * 2 * arrayLocIdx.GetSize();
		__int64 nProgressSoFar = 0;

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);

			CWordArray arrayTempPluStatus;

			int nSpecialStatus;
			CString strSourceFilename, strDestFilename;

			if ( DealerFlags.GetPriceSetExcludeByDefaultFlag() == TRUE )		
			{
				//PREVIOUS STATUS WAS INCLUDE BY DEFAULT
				if ( DataManager.Plu.GetSize() > 0 )
					arrayTempPluStatus.InsertAt ( 0, LOCALPLU_MASTER, DataManager.Plu.GetSize() );
			
				strSourceFilename = dbLocation.GetFilePathDataLocalPluExclude( nLocIdx );
				strDestFilename = dbLocation.GetFilePathDataLocalPluInclude( nLocIdx );
				nSpecialStatus = LOCALPLU_EXCLUDE;
			}
			else
			{
				//PREVIOUS STATUS WAS EXCLUDE BY DEFAULT
				if ( DataManager.Plu.GetSize() > 0 )
					arrayTempPluStatus.InsertAt ( 0, LOCALPLU_EXCLUDE, DataManager.Plu.GetSize() );
			
				strSourceFilename = dbLocation.GetFilePathDataLocalPluInclude( nLocIdx );
				strDestFilename = dbLocation.GetFilePathDataLocalPluExclude( nLocIdx );
				nSpecialStatus = LOCALPLU_MASTER;
			}

			CSSFile fileSource;
			if ( fileSource.Open ( strSourceFilename, "rb" ) == TRUE )	
			{
				CString str;
				str.Format ( "(%s)", dbLocation.GetName( nLocIdx ) );
				StatusProgress.SetDialogText( str );

				CString strBuffer;

				while ( fileSource.ReadString ( strBuffer ) == TRUE )
				{
					__int64 nFileProgress = ( fileSource.GetPercentPosition() * DataManager.Plu.GetSize() ) / 100;
					StatusProgress.SetPos ( nProgressSoFar + nFileProgress, nProgressTarget );
			
					int nPluIdx;
					if ( DataManager.Plu.FindPluByNumber ( _atoi64(strBuffer), nPluIdx ) == TRUE )
						arrayTempPluStatus.SetAt ( nPluIdx, nSpecialStatus );
				}

				fileSource.Close();
			}

			nProgressSoFar += DataManager.Plu.GetSize();
	
			CSSFile fileDest;
			if ( fileDest.Open( strDestFilename, "wb" ) == TRUE )
			{
				int nSize = DataManager.Plu.GetSize();
				for ( int nPluIdx = 0; nPluIdx < nSize; nPluIdx++ )
				{	
					StatusProgress.SetPos( nProgressSoFar + nPluIdx, nProgressTarget );

					if ( arrayTempPluStatus.GetAt ( nPluIdx ) != nSpecialStatus )
					{
						CPluCSVRecord PluRecord;
						DataManager.Plu.GetAt ( nPluIdx, PluRecord );
						fileDest.WriteLine ( PluRecord.GetPluNoString() );
					}
				}

				fileDest.Close();
			}

			nProgressSoFar += DataManager.Plu.GetSize();
			CFileRemover FileRemover( strSourceFilename );
		}		
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CPriceSetItemManager::WritePriceSetFile()
{
	m_bPluStatusModified = FALSE;

	if ( m_strFieldFilename == "" || IsPluMaster() == TRUE ) 
		return;

	int nCount = DataManager.Plu.GetSize();

	if ( HasLocalFields() == TRUE )
	{
		CString strDatabaseHeader = PluDatabaseHeader.GetDatabaseHeader();
	
		CSSExportFilterFile fileExportPlu ( strDatabaseHeader, m_strPriceSetHeader );
		if ( fileExportPlu.Open ( m_strFieldFilename, "wb" ) == FALSE )
		{
			if (DealerFlags.GetLogFileWritesFlag() == TRUE)
			{
				MessageLogger.LogFileWrite(m_strFieldFilename, DATAMANAGER_ERROR_WRITE, GetLastError());
			}

			return;
		}

		fileExportPlu.WriteHeader();
		
		//WRITE THE LIST OF LOCAL PLU RECORDS
		StatusProgress.Lock( TRUE, "Writing price set file" );
		for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
		{	
			StatusProgress.SetPos ( nPluIdx, nCount );	
			
			if ( m_arrayPluStatus.GetAt ( nPluIdx ) == LOCALPLU_LOCAL )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );
				CCSV csv ( ',', '"', FALSE, FALSE, TRUE );
				PluRecord.CreateExportLine ( csv );
				fileExportPlu.WriteData ( csv.GetLine() );
			}
		}
		StatusProgress.Unlock();
		fileExportPlu.Close();

		if ( DealerFlags.GetLogFileWritesFlag() == TRUE )
			MessageLogger.LogFileWrite( m_strFieldFilename, DATAMANAGER_ERROR_NONE, 0 );
	}

	CString strFilename;
	
	if ( DealerFlags.GetPriceSetExcludeByDefaultFlag() == TRUE )
		strFilename = m_strIncludeFilename;
	else
		strFilename = m_strExcludeFilename;
	
	CSSFile fileIncludeExclude;
	if ( fileIncludeExclude.Open ( strFilename, "wb" ) == FALSE )
	{
		if ( DealerFlags.GetLogFileWritesFlag() == TRUE )
			MessageLogger.LogFileWrite( strFilename, DATAMANAGER_ERROR_WRITE, GetLastError() );
		
		return;
	}
	
	//WRITE THE LIST OF EXCLUDED PLU ITEMS
	StatusProgress.Lock( TRUE, "Writing excluded items list" );
	for ( int nPluIdx = 0; nPluIdx < nCount; nPluIdx++ )
	{
		StatusProgress.SetPos ( nPluIdx, nCount );	
		
		if ( DealerFlags.GetPriceSetExcludeByDefaultFlag() == TRUE )
		{
			if ( m_arrayPluStatus.GetAt ( nPluIdx ) != LOCALPLU_EXCLUDE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );
				fileIncludeExclude.WriteLine ( PluRecord.GetPluNoString() );
			}
		}
		else
		{
			if ( m_arrayPluStatus.GetAt ( nPluIdx ) == LOCALPLU_EXCLUDE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );
				fileIncludeExclude.WriteLine ( PluRecord.GetPluNoString() );
			}
		}
	}
	StatusProgress.Unlock();
	fileIncludeExclude.Close();

	if ( DealerFlags.GetLogFileWritesFlag() == TRUE )
		MessageLogger.LogFileWrite( strFilename, DATAMANAGER_ERROR_NONE, 0 );

	if ( DealerFlags.GetPriceSetExcludeByDefaultFlag() == TRUE )
		CFileRemover FileRemover( m_strExcludeFilename );
	else
		CFileRemover FileRemover( m_strIncludeFilename );
}

/**********************************************************************/

void CPriceSetItemManager::GetPriceSetExportInfo( CPriceSetExportInfo& info )
{
	info.m_strLabel = "MASTER";
	info.m_strPath1 = "";
	info.m_strPath2 = "";

	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		return;

	int nDbPriceSet = 0;
	switch( m_nPriceSet )
	{
	case 1:	nDbPriceSet = dbLocation.GetDbPriceSet1( m_nLocIdx );	break;
	case 2:	nDbPriceSet = dbLocation.GetDbPriceSet2( m_nLocIdx );	break;
	case 3:	nDbPriceSet = dbLocation.GetDbPriceSet3( m_nLocIdx );	break;
	}

	if ( nDbPriceSet == 0 )
	{
		info.m_strLabel.Format( "LOCSET%d", m_nPriceSet );
		info.m_strPath1.Format ( "%s\\exports", dbLocation.GetFolderPathSysset( m_nLocIdx ) );
		info.m_strPath2.Format ( "%s\\PRICESET%d", (const char*) info.m_strPath1, m_nPriceSet );
	}
	else
	{
		int nDbIdx = DataManager.GetActiveDbIdx();
		info.m_strLabel.Format( "DBSET%d", nDbPriceSet );
		info.m_strPath1.Format ( "%s\\exports", dbDatabase.GetFolderPathSysset( nDbIdx ) );
		info.m_strPath2.Format ( "%s\\PRICESET%d", (const char*) info.m_strPath1, nDbPriceSet );
	}
}

/**********************************************************************/

void CPriceSetItemManager::RememberMasterRecord()
{
	RememberMasterRecord( m_nPluIdx );
}

/**********************************************************************/

void CPriceSetItemManager::RememberMasterRecord( int nPluIdx )
{
	if ( ( nPluIdx >= 0 ) && ( nPluIdx < DataManager.Plu.GetSize() ) && ( nPluIdx < m_arrayMasterRecord.GetSize() ) )
	{
		CString strRecord = m_arrayMasterRecord.GetAt( nPluIdx );
		if ( strRecord == "" )
		{
			strRecord = DataManager.Plu.GetAt( nPluIdx );
			m_arrayMasterRecord.SetAt( nPluIdx, strRecord );
		}
	}
}

/**********************************************************************/

void CPriceSetItemManager::RestoreMasterRecord()
{
	RestoreMasterRecord( m_nPluIdx );
}

/**********************************************************************/

void CPriceSetItemManager::RestoreMasterRecord( int nPluIdx )
{
	if ( ( nPluIdx >= 0 ) && ( nPluIdx < DataManager.Plu.GetSize() ) && ( nPluIdx < m_arrayMasterRecord.GetSize() ) )
	{
		CString strRecord = m_arrayMasterRecord.GetAt( nPluIdx );
		if ( strRecord != "" )
		{
			CPluCSVRecord PluRecord;
			CCSV csv( strRecord );
			PluRecord.ConvertFromCSV( csv );
			DataManager.Plu.SetAt( nPluIdx, PluRecord );
		}
	}
}

/**********************************************************************/

void CPriceSetItemManager::ReloadMasterRecord( CPluCSVRecord& PluRecord )
{
	if ( ( m_nPluIdx >= 0 ) && ( m_nPluIdx < DataManager.Plu.GetSize() ) && ( m_nPluIdx < m_arrayMasterRecord.GetSize() ) )
	{
		CString strRecord = m_arrayMasterRecord.GetAt( m_nPluIdx );
		if ( strRecord != "" )
		{
			CCSV csv( strRecord );
			PluRecord.ConvertFromCSV( csv );
		}
	}
}

/**********************************************************************/
