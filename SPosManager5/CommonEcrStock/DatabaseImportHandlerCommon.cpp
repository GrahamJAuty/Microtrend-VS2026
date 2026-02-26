/**********************************************************************/
#include "ServerCSVArray.h"
#include "ImportDefines.h"
#include "ServerDatabaseHeader.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DatabaseImportHandlerCommon.h"
/**********************************************************************/

CDatabaseImportHandlerCommon::CDatabaseImportHandlerCommon()
{
	m_nImportMethod = IMPORT_METHOD_ALL;
	ResetCounters();
}

/**********************************************************************/

void CDatabaseImportHandlerCommon::ResetCounters()
{
	m_nImportTotalLines = 0;
	m_nImportIgnoredLines = 0;
	m_nImportRecordsAdded = 0;
	m_nImportRecordsModified = 0;
}

/**********************************************************************/

int CDatabaseImportHandlerCommon::ImportServerFile ( const char* szFilename )
{
	ResetCounters();

	CString strDatabaseHeader = ServerDatabaseHeader.GetDatabaseHeader();
	
	CString strFilename = szFilename;
	CSSImportFile fileImport ( strDatabaseHeader );
	if (fileImport.Open(strFilename, "rb") == FALSE)
	{
		return IMPORTERROR_PLU_OPENFILE;
	}

	CString strBuffer;

	StatusProgress.Lock( TRUE, "Importing Server data" );
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( fileImport );

		int nImportPos;
		int nResult = DataManagerNonDb.Server.ImportLine ( m_nImportMethod, nImportPos, fileImport, strBuffer );

		switch( nResult )
		{
		case IMPORT_LINE_ADDED:
			{
				/*
				__int64 nPluNo = DataManager.Plu.GetPluNoInt(nImportPos);
			
				if ( SysInfo.IsValidSSPluNo( nPluNo ) == FALSE )
				{
					DataManager.Plu.RemoveAt( nImportPos );
					nResult = IMPORT_LINE_IGNORED;
				}
				*/
			}
			break;
		}

		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:	
			m_nImportTotalLines++;
			m_nImportRecordsAdded++;
			break;

		case IMPORT_LINE_MODIFIED:
			m_nImportTotalLines++;
			m_nImportRecordsModified++;
			break;
				
		case IMPORT_LINE_IGNORED:
			m_nImportTotalLines++;
			m_nImportIgnoredLines++;
			break;
		}
	}
	StatusProgress.Unlock();
	
	return IMPORTERROR_NONE;
}

/**********************************************************************/

