/**********************************************************************/
#include "FileRemover.h"
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "StockImportSheetHelpers.h"
/**********************************************************************/

CStockImportSheetItem::CStockImportSheetItem()
{
	Reset();
}

/**********************************************************************/

void CStockImportSheetItem::Reset()
{
	m_nStockIdx = 0;
	m_nSuppSortIdx = 0;
	m_nCatNo = 0;
	m_strDescription = "";
}

/**********************************************************************/

void CStockImportSheetItem::Add ( CStockImportSheetItem& source )
{
}

/**********************************************************************/

int CStockImportSheetItem::Compare( CStockImportSheetItem& source, int nHint )
{
	switch( nHint & 0xFFFF )
	{	
	case SH_STKLVL_SUPP:
	case SH_STKLVL_SUPP_CAT:

		if ( m_nSuppSortIdx != source.m_nSuppSortIdx )
		{
			return ( m_nSuppSortIdx > source.m_nSuppSortIdx ? 1 : -1 );
		}

		if ( SH_STKLVL_SUPP_CAT == nHint )
		{
			if ( m_nCatNo != source.m_nCatNo )
			{
				return ( m_nCatNo > source.m_nCatNo ? 1 : -1 );
			}
		}

		break;

	case SH_STKLVL_CAT:
	case SH_STKLVL_CAT_SUPP:

		if ( m_nCatNo != source.m_nCatNo )
		{
			return ( m_nCatNo > source.m_nCatNo ? 1 : -1 );
		}

		if ( SH_STKLVL_CAT_SUPP == nHint )
		{
			if ( m_nSuppSortIdx != source.m_nSuppSortIdx )
			{
				return ( m_nSuppSortIdx > source.m_nSuppSortIdx ? 1 : -1 );
			}
		}

		break;
	}

	if ((nHint & 0x10000) != 0)
	{
		if (m_strDescription != source.m_strDescription)
		{
			return (m_strDescription > source.m_strDescription) ? 1 : -1;
		}
	}

	if (m_nStockIdx != source.m_nStockIdx)
	{
		return (m_nStockIdx > source.m_nStockIdx ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CStockImportSheetHelpers::CStockImportSheetHelpers( int nSpIdx, int nStocktakeNo )
{
	m_nSpIdx = nSpIdx;
	m_nStocktakeNo = nStocktakeNo;
	m_nColumnStockCode = -1;
	m_nColumnSUQty = -1;
	m_nColumnDUQty = -1;

	if (m_nStocktakeNo != 0)
	{
		m_strImportSheetFolder.Format("%s\\Stocktake%3.3d_ImportSheets\\",
			(const char*)dbStockpoint.GetFolderPath(m_nSpIdx),
			nStocktakeNo);
	}
	else
	{
		m_strImportSheetFolder.Format("%s\\OpeningStock_ImportSheets\\",
			(const char*)dbStockpoint.GetFolderPath(m_nSpIdx) );
	}

	m_strImportSheetConsolidatedFilename = m_strImportSheetFolder;
	m_strImportSheetConsolidatedFilename += "AllImportSheets.csv";
}

/**********************************************************************/

CStockImportSheetHelpers::~CStockImportSheetHelpers(void)
{
}

/**********************************************************************/

const char* CStockImportSheetHelpers::GetFieldName( int nField, bool bMakeUpper )
{
	switch( nField )
	{
	case STOCK_IMPORTSHEET_STOCKCODE:		m_strFieldName = "StockCode";			break;
	case STOCK_IMPORTSHEET_DESCRIPTION:		m_strFieldName = "Description";			break;
	case STOCK_IMPORTSHEET_CATEGORY:		m_strFieldName = "Category";			break;
	case STOCK_IMPORTSHEET_SUPPLIER:		m_strFieldName = "Supplier";			break;
	case STOCK_IMPORTSHEET_SHEETCODE:		m_strFieldName = "ImportSheetCode";		break;
	case STOCK_IMPORTSHEET_SUNAME:			m_strFieldName = "StockUnitName";		break;
	case STOCK_IMPORTSHEET_SUQTY:			m_strFieldName = "StockUnitQty";		break;
	case STOCK_IMPORTSHEET_DUNAME:			m_strFieldName = "DeliveryUnitName";	break;
	case STOCK_IMPORTSHEET_DUQTY:			m_strFieldName = "DeliveryUnitQty";		break;
	default:								m_strFieldName = "Unknown";				break;
	}

	if ( TRUE == bMakeUpper )
	{
		m_strFieldName.MakeUpper();
	}

	return m_strFieldName;
}

/**********************************************************************/

const char* CStockImportSheetHelpers::GetImportSheetFilename()
{
	m_strImportSheetFilename = "";

	CSSFile file;
	CFilenameUpdater FnUp( SysFiles::LastImportSheetFile, m_nSpIdx );
	if ( file.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		file.ReadString( m_strImportSheetFilename );
		file.Close();
	}

	if ( m_strImportSheetFilename == "" )
	{
		m_strImportSheetFilename = dbStockpoint.GetFolderPath( m_nSpIdx );
		m_strImportSheetFilename += "\\importsheet.csv";
	}

	return m_strImportSheetFilename;
}

/**********************************************************************/

bool CStockImportSheetHelpers::ConsolidateImportSheets()
{
	{
		CFileRemover FileRemover;
		FileRemover.RemoveFile(m_strImportSheetConsolidatedFilename);
	}

	CSSFile fileOutput;
	if (fileOutput.Open(m_strImportSheetConsolidatedFilename, "wb") == FALSE)
	{
		Prompter.Error("Unable to consolidate import sheets");
		return FALSE;
	}

	CStringArray arrayImportFiles;

	CString strFilter = "";

	if (m_nStocktakeNo != 0)
	{
		strFilter.Format("Stocktake%3.3d",
			m_nStocktakeNo);
	}
	else
	{
		strFilter = "OpeningStock";
	}

	strFilter += "_ImportSheet*.csv";

	{
		CFileFind finder;
		BOOL bWorking = finder.FindFile(m_strImportSheetFolder + strFilter);

		while (bWorking == TRUE)
		{
			bWorking = finder.FindNextFile();

			if (finder.IsDots() == TRUE)
			{
				continue;
			}

			if (finder.IsDirectory() == TRUE)
			{
				continue;
			}

			arrayImportFiles.Add(finder.GetFileName());
		}
	}

	bool bGotHeader = FALSE;
	CString strHeader = "";

	for ( int n = 0; n < arrayImportFiles.GetSize(); n++ )
	{
		CString strImportFile = "";
		strImportFile += m_strImportSheetFolder;
		strImportFile += arrayImportFiles.GetAt(n);

		CSSFile fileIn;
		if (fileIn.Open(strImportFile, "rb") == FALSE)
		{
			fileOutput.Close();

			CString strError = "";
			strError += "Unable to open import sheet:\n\n";
			strError += arrayImportFiles.GetAt(n);
			Prompter.Error(strError);

			return FALSE;
		}

		bool bFirstLine = TRUE;
		bool bFirstDataLine = TRUE;
		
		CString strBuffer = "";
		while (fileIn.ReadString(strBuffer) == TRUE)
		{
			if (TRUE == bFirstLine)
			{
				if (FALSE == bGotHeader)
				{
					fileOutput.WriteLine(strBuffer);
					strHeader = strBuffer;
					bGotHeader = TRUE;
				}
				else if ( strHeader != strBuffer )
				{
					fileOutput.Close();

					CString strError = "";
					strError += "Non matching header in import sheet:\n\n";
					strError += arrayImportFiles.GetAt(n);
					Prompter.Error(strError);

					return FALSE;
				}
			}
			else
			{
				if ( (TRUE == bFirstDataLine) && ( n != 0 ) )
				{
					fileOutput.WriteLine("");
				}

				fileOutput.WriteLine(strBuffer);

				bFirstDataLine = FALSE;
			}

			bFirstLine = FALSE;
		}
	}

	fileOutput.Close();
	return TRUE;
}

/**********************************************************************/

void CStockImportSheetHelpers::CreateSheet( CStockFilterArray* pFilterArray, CString& strFields, int nCopies )
{
	CString strFilename = "";
	
	if (1 == nCopies)
	{
		strFilename = GetImportSheetFilename();
	}
	else
	{
		CreateSubdirectory(m_strImportSheetFolder);

		strFilename = m_strImportSheetFolder;

		if (m_nStocktakeNo != 0)
		{
			CString strStocktake = "";
			strStocktake.Format("Stocktake%3.3d",
				m_nStocktakeNo);

			strFilename += strStocktake;
		}
		else
		{
			strFilename = "OpeningStock";
		}

		strFilename += "_ImportSheet01.csv";

		int nDummy = 0;
		CFileRemover FileRemover;
		FileRemover.RemoveFolderContents(m_strImportSheetFolder, nDummy);
	}

	CSSFile fileSheet;
	if ( fileSheet.Open( strFilename, "wb" ) == FALSE )
	{
		Prompter.Error( "Unable to create ImportSheet." );
	}
	else
	{
		int nSortOrder = SH_STKLVL_NONE;
		int nAlphaSort = 0;

		//CREATE SUPPLIER REPORT MAP WITH ALL SUPPLIERS
		{
			CWordArray arraySuppIdx;
			for ( int n = 0; n <= DataManager.Supplier.GetSize(); n++ )
			{
				arraySuppIdx.Add(n);
			}
			m_SupplierReportMap.CreateMap( arraySuppIdx );
		}

		//TAG STOCK CODE AS FIRST FIELD
		BOOL bTagged[ MAX_STOCK_IMPORTSHEET_FIELD + 1 ];
		
		for ( int n = 0; n <= MAX_STOCK_IMPORTSHEET_FIELD; n++ )
		{
			bTagged[n] = ( n == 0 );
		}

		CArray<int,int> arrayFields;
		arrayFields.Add(0);

		//EXTRACT OTHER FIELD AND SORT ORDER FROM LINE
		CCSV csvFields( strFields );
		for ( int nPos = 0; nPos < csvFields.GetSize(); nPos++ )
		{
			int nField = csvFields.GetInt(nPos);
			if ( ( nField >= 1 ) && ( nField <= MAX_STOCK_IMPORTSHEET_FIELD ) )
			{
				if ( bTagged[nField] == FALSE )
				{
					bTagged[nField] = TRUE;
					arrayFields.Add( nField );
				}
			}
			else if ((nField >= SH_STKLVL_NONE) && (nField <= 200))
			{
				nSortOrder = nField;
			}
			else if (301 == nField)
			{
				nAlphaSort = 0x10000;
			}
		}

		//WRITE IMPORT SHEET HEADER
		{
			CCSV csv;
			for ( int n = 0; n < arrayFields.GetSize(); n++ )
			{
				csv.Add( GetFieldName( arrayFields.GetAt(n) ) );
			}

			fileSheet.WriteLine( csv.GetLine() );
		}

		//MAKE SURE THAT SORT ORDER IS A KNOWN TYPE
		switch( nSortOrder )
		{
		case SH_STKLVL_SUPP:		
		case SH_STKLVL_SUPP_CAT:	
		case SH_STKLVL_CAT:			
		case SH_STKLVL_CAT_SUPP:	
			break;	

		default:
			nSortOrder = SH_STKLVL_NONE;
			break;
		}

		CReportConsolidationArray<CStockImportSheetItem> arrayItems( FALSE, nSortOrder | nAlphaSort );

		//SORT ITEMS ACCORDING TO SORT ORDER
		for ( int n = 0; n < pFilterArray -> GetSize(); n++ )
		{
			int nStockIdx = pFilterArray -> GetStockItemIndexAt(n);
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( nStockIdx, StockRecord );

			CStockImportSheetItem item;
			item.m_nStockIdx = nStockIdx;
			item.m_nSuppSortIdx = m_SupplierReportMap.GetSortIdxBySuppNo( StockRecord.GetSuppNo(0) );
			item.m_nCatNo = StockRecord.GetCategory();
			
			if (nAlphaSort != 0)
			{
				item.m_strDescription = StockRecord.GetDescription();
				item.m_strDescription.MakeUpper();
			}
			else
			{
				item.m_strDescription = "";
			}

			arrayItems.Consolidate( item );
		}

		for ( int n = 0; n < arrayItems.GetSize(); n++ )
		{
			CStockImportSheetItem item;
			arrayItems.GetAt( n, item );
			
			CStockCSVRecord StockRecord;
			DataManager.Stock.GetAt( item.m_nStockIdx, StockRecord );

			CCSV csv;
			for ( int x = 0; x < arrayFields.GetSize(); x++ )
			{
				switch( arrayFields.GetAt(x) )
				{
				case STOCK_IMPORTSHEET_STOCKCODE:		
					{
						CString strStockCode = StockRecord.GetStockCode();
						CString strTest = strStockCode.Left(1);
						
						if ( ( ::TestNumeric( strTest ) ) || ( strTest == "#" ) )
						{
							strStockCode.Insert( 0, "#" );
						}

						csv.Add( strStockCode );		
					}
					break;
				
				case STOCK_IMPORTSHEET_DESCRIPTION:		
					csv.Add( StockRecord.GetDescription() );
					break;

				case STOCK_IMPORTSHEET_CATEGORY:	
					csv.Add( DataManager.Category.GetText( item.m_nCatNo ) );
					break;

				case STOCK_IMPORTSHEET_SUPPLIER:
					{
						int nSuppIdx = m_SupplierReportMap.GetSuppIdxBySortIdx( item.m_nSuppSortIdx );

						CString strSuppName = "";

						if ( nSuppIdx == 0 )
						{
							strSuppName = "Unspecified";
						}
						else if ( ( nSuppIdx >= 1 ) && ( nSuppIdx <= DataManager.Supplier.GetSize() ) )
						{
							strSuppName = DataManager.Supplier.GetName( nSuppIdx - 1 );
						}
						else
						{
							strSuppName = "Unknown";
						}

						csv.Add( strSuppName );
					}
					break;
				
				case STOCK_IMPORTSHEET_SHEETCODE:		
					csv.Add( StockRecord.GetImportSheetCode() );			
					break;
				
				case STOCK_IMPORTSHEET_SUNAME:			
					csv.Add( StockRecord.GetStockUnitName() );
					break;
				
				case STOCK_IMPORTSHEET_DUNAME:			
					csv.Add( StockRecord.GetCase(0) );
					break;

				case STOCK_IMPORTSHEET_SUQTY:			
				case STOCK_IMPORTSHEET_DUQTY:			
				default:								
					csv.Add( "" );		
					break;
				}
			}
			fileSheet.WriteLine( csv.GetLine() );
		}

		fileSheet.Close();

		if (nCopies >= 2)
		{
			for (int n = 2; n <= nCopies; n++)
			{
				CString strCopyFile = m_strImportSheetFolder;

				if (m_nStocktakeNo != 0)
				{
					CString strStocktake = "";
					strStocktake.Format("Stocktake%3.3d",
						m_nStocktakeNo);

					strCopyFile += strStocktake;
				}
				else
				{
					strCopyFile += "OpeningStock";
				}

				{
					CString strFileNumber = "";
					strFileNumber.Format("_ImportSheet%2.2d.csv", n);
					strCopyFile += strFileNumber;
				}

				CopyFile(strFilename, strCopyFile, FALSE);
			}
		}

		Prompter.Info( "The ImportSheet was created as requested." );
	}
}

/**********************************************************************/

bool CStockImportSheetHelpers::ProcessHeader( const char* szHeader )
{
	m_nColumnStockCode = -1;
	m_nColumnSUQty = -1;
	m_nColumnDUQty = -1;

	CString strStockCodeField =		GetFieldName( STOCK_IMPORTSHEET_STOCKCODE, TRUE );
	CString strSUQtyField =			GetFieldName( STOCK_IMPORTSHEET_SUQTY, TRUE );
	CString strDUQtyField =			GetFieldName( STOCK_IMPORTSHEET_DUQTY, TRUE );
	
	int nGotFields = 0;

	CCSV csv ( szHeader );

	for ( int n = 0; n < csv.GetSize(); n++ )
	{
		CString strField = csv.GetString(n);
		strField.MakeUpper();

		if ( m_nColumnStockCode == -1 )
		{
			if ( strField == strStockCodeField )
			{
				m_nColumnStockCode = n;
				nGotFields++;
				continue;
			}
		}

		if ( m_nColumnSUQty == -1 )
		{
			if ( strField == strSUQtyField )
			{
				m_nColumnSUQty = n;
				nGotFields++;
				continue;
			}
		}

		if ( m_nColumnDUQty == -1 )
		{
			if ( strField == strDUQtyField )
			{
				m_nColumnDUQty = n;
				nGotFields++;
				continue;
			}
		}

		if ( 3 == nGotFields )
			break;
	}

	if ( m_nColumnStockCode == -1 )
		return FALSE;

	if ( ( m_nColumnSUQty == -1 ) && ( m_nColumnDUQty == -1 ) )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CStockImportSheetHelpers::ProcessLine( const char* szLine, CString& strStockCode, bool& bGotSUQty, double& dSUQty, bool& bGotDUQty, double& dDUQty )
{
	CCSV csv ( szLine );
	strStockCode = ( m_nColumnStockCode == -1 ) ? "" : csv.GetString( m_nColumnStockCode );
	
	if ( strStockCode.Left(1) == "#" )
		strStockCode = strStockCode.Right( strStockCode.GetLength() - 1 );

	bGotSUQty = FALSE;
	bGotDUQty = FALSE;
	dSUQty = 0.0;
	dDUQty = 0.0;

	if ( m_nColumnSUQty != -1 )
	{
		CString str = csv.GetString( m_nColumnSUQty );

		if ( str != "" )
		{
			bGotSUQty = TRUE;
			dSUQty = csv.GetDouble( m_nColumnSUQty );
		}
	}

	if ( m_nColumnDUQty != -1 )
	{
		CString str = csv.GetString( m_nColumnDUQty );

		if ( str != "" )
		{
			bGotDUQty = TRUE;
			dDUQty = csv.GetDouble( m_nColumnDUQty );
		}
	}
}

/**********************************************************************/

bool CStockImportSheetHelpers::CreateReportHeader( const char* szHeader, CReportFile& ReportFile )
{
	CCSV csv( szHeader );

	for ( int n = 0; n < csv.GetSize(); n++ )
	{
		CString strField = csv.GetString(n);
		
		CString strUpper = strField;
		strUpper.MakeUpper();

		bool bQty = FALSE;

		if ( strUpper == GetFieldName( STOCK_IMPORTSHEET_SUQTY, TRUE ) )
			bQty = TRUE;
		else if ( strUpper == GetFieldName( STOCK_IMPORTSHEET_DUQTY, TRUE ) ) 
			bQty = TRUE;

		ReportFile.AddColumn( strField, bQty ? TA_RIGHT : TA_LEFT, 350 );
	}

	return ( csv.GetSize() > 0 );
}

/**********************************************************************/

