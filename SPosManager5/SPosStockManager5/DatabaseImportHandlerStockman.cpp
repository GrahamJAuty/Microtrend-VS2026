/**********************************************************************/
#include "BarcodeCSVArray.h"
#include "FileRemover.h"
#include "ImportDefines.h"
#include "PluDatabaseHeader.h"
#include "PluMonitorStockman.h"
#include "StatusProgress.h"
#include "StockMonitor.h"
#include "StockOptionsIni.h"
#include "StockPluArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "DatabaseImportHandlerStockman.h"
/**********************************************************************/

CDatabaseImportHandlerStockman::CDatabaseImportHandlerStockman()
{
	m_bCreateStock = FALSE;
	m_nImportMethodPlu = IMPORT_METHOD_ALL;
	m_nImportMethodStock = IMPORT_METHOD_ALL;
	m_nImportMethodBarcode = IMPORT_METHOD_ALL;
	ResetCounters();
}

/**********************************************************************/

void CDatabaseImportHandlerStockman::ResetCounters()
{
	m_nImportTotalLines = 0;
	m_nImportIgnoredLines = 0;
	m_nImportPluAdded = 0;
	m_nImportPluModified = 0;
	m_nImportBarcodeAdded = 0;
	m_nImportBarcodeModified = 0;
	m_nImportStockAdded = 0;
	m_nImportStockModified = 0;
	m_nImportSupplierAdded = 0;
	m_nImportSupplierModified = 0;
}

/**********************************************************************/

int CDatabaseImportHandlerStockman::ImportPluFile ( const char* szFilename )
{
	//IMPORT MAY CREATE NEW PLU AND STOCK RECORDS BUT THEIR LINKS
	//WILL NOT BE LOGGED IN THE STOCK PLU ARRAY 
	DataManager.StockPlu.Invalidate();

	StatusProgress.Lock( TRUE, "Importing PLU data" );
	int nResult = ImportPluFileInternal ( szFilename );
	StatusProgress.Unlock();
	return nResult;
}

/**********************************************************************/

int CDatabaseImportHandlerStockman::ImportPluFileInternal ( const char* szFilename )
{
	CString strFilename = szFilename;
	CSSImportFile fileImport ( PluDatabaseHeader.GetCompatibilityHeader( strFilename ) );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )	
		return IMPORTERROR_PLU_OPENFILE;

	int nOldStockSize = DataManager.Stock.GetSize();

	CString strBuffer;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );
		
		CCSV csv ( strBuffer );
		CString strPluNo = csv.GetString(0);

		int nImportPos;
		int nResult = DataManager.Plu.ImportLine ( m_nImportMethodPlu, nImportPos, fileImport, strBuffer, m_bCreateStock );

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
		default:
			m_nImportTotalLines++;
			m_nImportIgnoredLines++;
			break;
		}
	}
	
	m_nImportStockAdded = DataManager.Stock.GetSize() - nOldStockSize;
	return IMPORTERROR_NONE;
}

/**********************************************************************/

int CDatabaseImportHandlerStockman::ImportPluStockFile ( const char* szFilename )
{
	if (DataManager.Supplier.ValidateSuppliers() == FALSE)
	{
		return IMPORTERROR_SUPPLIER_INVALID1;
	}

	for (int n = 1; n <= MAX_ITEM_CUSTOM_FIELDS; n++)
	{
		if (DataManager.CustomFieldValue[n].ValidateFieldValues() == FALSE)
		{
			return IMPORTERROR_CUSTOM_FIELD_INVALID1 + n - 1;
		}
	}

	DataManager.StockPlu.Invalidate();

	//IMPORT PLU RECORDS
	CString strFilename = szFilename;
	CString strHeader = PluDatabaseHeader.GetCompatibilityHeader( strFilename );
	
	{
		CCSV csvHeader(strHeader);
		csvHeader.InsertAt(1, Stock::StockCode.Label);
		strHeader = csvHeader.GetLine();
	}

	CSSImportFile fileImport ( strHeader );
	if (fileImport.Open(strFilename, "rb") == FALSE)
	{
		return IMPORTERROR_PLU_OPENFILE;
	}

	StatusProgress.Lock( TRUE, "Importing PLU/Stock data" );

	CArray<bool,bool> arrayImportLines;

	CString strBuffer;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );

		int nImportPos;
		int nResult = DataManager.Plu.ImportLine ( m_nImportMethodPlu, nImportPos, fileImport, strBuffer, FALSE, TRUE );

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

	//IMPORT STOCK RECORDS
	int nOldSupplierCount = DataManager.Supplier.GetSize();
	int nItemSupplierCount = StockOptions.GetStockMaxSuppliersPerItem();

	fileImport.Reset( CStockCSVRecord::GetHeader( nItemSupplierCount ) );
		
	int nLineNo = -1;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );

		nLineNo++;
		if ( nLineNo >= arrayImportLines.GetSize() )
			break;

		if ( arrayImportLines.GetAt(nLineNo) == FALSE )
			continue;

		int nResult = DataManager.Stock.ImportLine ( IMPORT_METHOD_ALL, fileImport, strBuffer, nItemSupplierCount );

		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:		
			m_nImportStockAdded++;
			break;

		case IMPORT_LINE_MODIFIED:	
			m_nImportStockModified++;
			break;	
		}
	}

	m_nImportSupplierAdded = DataManager.Supplier.GetSize() - nOldSupplierCount;
	
	//IMPORT RECIPES
	fileImport.Reset ( CRecipeCSVRecord::GetHeader() );

	nLineNo = -1;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{
		StatusProgress.SetPos ( fileImport );

		nLineNo++;
		if ( nLineNo >= arrayImportLines.GetSize() )
			break;

		if ( arrayImportLines.GetAt(nLineNo) == FALSE )
			continue;

		bool bIncompleteLine = TRUE;
		DataManager.Plu.ImportRecipeLine ( fileImport, strBuffer, bIncompleteLine );

		if ( TRUE == bIncompleteLine )
			break;
	}
	
	StatusProgress.Unlock();

	return IMPORTERROR_NONE;
}

/**********************************************************************/

int CDatabaseImportHandlerStockman::ImportPluBarcodeFile ( const char* szFilename )
{
	DataManager.StockPlu.Invalidate();

	//IMPORT PLU RECORDS
	CString strFilename = szFilename;
	CSSImportFile fileImport ( PluDatabaseHeader.GetCompatibilityHeader( strFilename ) );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )	
		return IMPORTERROR_PLU_OPENFILE;

	CArray<bool,bool> arrayImportLines;

	int nOldStockSize = DataManager.Stock.GetSize();

	StatusProgress.Lock( TRUE, "Importing PLU/Barcode data" );

	CString strBuffer;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );
		
		CCSV csv ( strBuffer );
		CString strPluNo = csv.GetString(0);

		int nImportPos;
		int nResult = DataManager.Plu.ImportLine ( m_nImportMethodPlu, nImportPos, fileImport, strBuffer, m_bCreateStock );

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

	m_nImportStockAdded = DataManager.Stock.GetSize() - nOldStockSize;
	
	return IMPORTERROR_NONE;
}

/**********************************************************************/

int CDatabaseImportHandlerStockman::ImportStockFile ( const char* szFilename )
{
	int nOldSupplierCount = DataManager.Supplier.GetSize();
	StatusProgress.Lock( TRUE, "Importing stock data" );
	int nResult = ImportStockFileInternal ( szFilename );
	StatusProgress.Unlock();
	m_nImportSupplierAdded = DataManager.Supplier.GetSize() - nOldSupplierCount;
	return nResult;
}

/**********************************************************************/

int CDatabaseImportHandlerStockman::ImportStockFileInternal ( const char* szFilename )
{
	if ( DataManager.Supplier.ValidateSuppliers() == FALSE )
		return IMPORTERROR_SUPPLIER_INVALID1;

	for ( int n = 1; n <= MAX_ITEM_CUSTOM_FIELDS; n++ )
		if ( DataManager.CustomFieldValue[n].ValidateFieldValues() == FALSE )
			return IMPORTERROR_CUSTOM_FIELD_INVALID1 + n - 1;
			
	int nSupplierCount = StockOptions.GetStockMaxSuppliersPerItem();

	CString strFilename = szFilename;
	CSSImportFile fileImport ( CStockCSVRecord::GetHeader( nSupplierCount ) );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )	
		return IMPORTERROR_STOCK_OPENFILE;

	CString strBuffer;
	while ( fileImport.ReadData ( strBuffer ) == TRUE )
	{	
		StatusProgress.SetPos ( fileImport );
		
		int nResult = DataManager.Stock.ImportLine ( m_nImportMethodStock, fileImport, strBuffer, nSupplierCount );
		
		switch ( nResult )
		{
		case IMPORT_LINE_ADDED:		
			m_nImportTotalLines++;
			m_nImportStockAdded++;
			break;

		case IMPORT_LINE_MODIFIED:	
			m_nImportTotalLines++;
			m_nImportStockModified++;
			break;
				
		case IMPORT_LINE_IGNORED:
			m_nImportTotalLines++;
			m_nImportIgnoredLines++;
			break;
		}
	}
	fileImport.Close();
	return IMPORTERROR_NONE;
}

/**********************************************************************/

int CDatabaseImportHandlerStockman::ImportBarcodeFile ( const char* szFilename )
{
	ResetCounters();
	
	CString strFilename = szFilename;
	
	CSSImportFile fileImport ( CBarcodeCSVRecord::GetHeader() );
	if ( fileImport.Open ( strFilename, "rb" ) == FALSE )	
		return IMPORTERROR_PLU_OPENFILE;

	CString strBuffer;

	StatusProgress.Lock( TRUE, "Importing Barode data" );
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
