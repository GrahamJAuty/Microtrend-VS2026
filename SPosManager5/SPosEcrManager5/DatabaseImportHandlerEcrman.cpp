/**********************************************************************/
 
/**********************************************************************/
#include "BarcodeCSVArray.h"
//#include "DataManager.h"
//#include "EcrmanOptionsIni.h"
//#include "globalfunctions.h"
#include "ImportDefines.h"
#include "PluDatabaseHeader.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DatabaseImportHandlerEcrman.h"
/**********************************************************************/

CDatabaseImportHandlerEcrman::CDatabaseImportHandlerEcrman()
{
	m_nImportMethodPlu = IMPORT_METHOD_ALL;
	m_nImportMethodBarcode = IMPORT_METHOD_ALL;
	ResetCounters();
}

/**********************************************************************/

void CDatabaseImportHandlerEcrman::ResetCounters()
{
	m_nImportTotalLines = 0;
	m_nImportIgnoredLines = 0;
	m_nImportPluAdded = 0;
	m_nImportPluModified = 0;
	m_nImportBarcodeAdded = 0;
	m_nImportBarcodeModified = 0;
}

/**********************************************************************/

int CDatabaseImportHandlerEcrman::ImportPluFile ( const char* szFilename )
{
	ResetCounters();
	
	CString strFilename = szFilename;
	CSSImportFile fileImport ( PluDatabaseHeader.GetCompatibilityHeader( strFilename ) );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )	
		return IMPORTERROR_PLU_OPENFILE;

	CString strBuffer;

	StatusProgress.Lock( TRUE, "Importing PLU data" );
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( fileImport );

		int nImportPos;
		int nResult = DataManager.Plu.ImportLine ( m_nImportMethodPlu, nImportPos, fileImport, strBuffer );

		switch( nResult )
		{
		case IMPORT_LINE_ADDED:
			{
				__int64 nPluNo = DataManager.Plu.GetPluNoInt(nImportPos);
			
				if ( SysInfo.IsValidSSPluNo( nPluNo ) == FALSE )
				{
					DataManager.Plu.RemoveAt( nImportPos );
					nResult = IMPORT_LINE_IGNORED;
				}
			}
			break;
		}

		switch( nResult )
		{
		case IMPORT_LINE_ADDED:
		case IMPORT_LINE_MODIFIED:
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nImportPos, PluRecord );
				DataManager.Plu.UpdatePluPrice( PluRecord, 0.0, TRUE );
				DataManager.Plu.SetAt( nImportPos, PluRecord );
			}
			break;
		}

		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:	
			m_nImportTotalLines++;
			m_nImportPluAdded++;
			break;

		case IMPORT_LINE_MODIFIED:
			m_nImportTotalLines++;
			m_nImportPluModified++;
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

int CDatabaseImportHandlerEcrman::ImportPluBarcodeFile ( const char* szFilename )
{
	//IMPORT PLU RECORDS
	CString strFilename = szFilename;
	CSSImportFile fileImport ( PluDatabaseHeader.GetCompatibilityHeader( strFilename ) );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )	
		return IMPORTERROR_PLU_OPENFILE;

	StatusProgress.Lock( TRUE, "Importing PLU/Barcode data" );

	CArray<bool,bool> arrayImportLines;

	CString strBuffer;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );
		
		CCSV csv ( strBuffer );
		CString strPluNo = csv.GetString(0);

		int nImportPos;
		int nResult = DataManager.Plu.ImportLine ( m_nImportMethodPlu, nImportPos, fileImport, strBuffer );

		switch( nResult )
		{
		case IMPORT_LINE_ADDED:
			{
				__int64 nPluNo = DataManager.Plu.GetPluNoInt(nImportPos);
			
				if ( SysInfo.IsValidSSPluNo( nPluNo ) == FALSE )
				{
					DataManager.Plu.RemoveAt( nImportPos );
					nResult = IMPORT_LINE_IGNORED;
				}
			}
			break;
		}

		switch( nResult )
		{
		case IMPORT_LINE_ADDED:
		case IMPORT_LINE_MODIFIED:
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nImportPos, PluRecord );
				DataManager.Plu.UpdatePluPrice( PluRecord, 0.0, TRUE );
				DataManager.Plu.SetAt( nImportPos, PluRecord );
			}
			break;
		}

		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:	
			m_nImportTotalLines++;
			m_nImportPluAdded++;
			arrayImportLines.Add( TRUE );
			break;

		case IMPORT_LINE_MODIFIED:
			m_nImportTotalLines++;
			m_nImportPluModified++;
			arrayImportLines.Add( TRUE );
			break;
				
		case IMPORT_LINE_IGNORED:
		default:
			m_nImportTotalLines++;
			m_nImportIgnoredLines++;
			arrayImportLines.Add( FALSE );
			break;
		}
	}

	//IMPORT BARCODE RECORDS
	fileImport.Reset( CBarcodeCSVRecord::GetHeader() );
		
	int nLineNo = -1;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );

		nLineNo++;
		if ( nLineNo >= arrayImportLines.GetSize() )
			break;

		if ( arrayImportLines.GetAt(nLineNo) == FALSE )
			continue;

		int nIndex;
		int nResult = DataManager.Barcode.ImportLine ( IMPORT_METHOD_ALL, nIndex, fileImport, strBuffer );

		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:		
			m_nImportBarcodeAdded++;
			break;

		case IMPORT_LINE_MODIFIED:	
			m_nImportBarcodeModified++;
			break;	
		}
	}

	StatusProgress.Unlock();

	return IMPORTERROR_NONE;
}

/**********************************************************************/

int CDatabaseImportHandlerEcrman::ImportBarcodeFile ( const char* szFilename )
{
	ResetCounters();
	
	CString strFilename = szFilename;
	
	CSSImportFile fileImport ( CBarcodeCSVRecord::GetHeader() );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )	
		return IMPORTERROR_PLU_OPENFILE;

	CString strBuffer;

	StatusProgress.Lock( TRUE, "Importing PLU data" );
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( fileImport );

		int nImportPos;
		int nResult = DataManager.Barcode.ImportLine ( m_nImportMethodBarcode, nImportPos, fileImport, strBuffer );

		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:	
			m_nImportTotalLines++;
			m_nImportBarcodeAdded++;
			break;

		case IMPORT_LINE_MODIFIED:
			m_nImportTotalLines++;
			m_nImportBarcodeModified++;
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
