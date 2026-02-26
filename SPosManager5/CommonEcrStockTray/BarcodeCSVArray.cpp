/**********************************************************************/
#include "ImportDefines.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "BarcodeCSVArray.h"
/**********************************************************************/

CBarcodeCSVArray::CBarcodeCSVArray() : CSharedCSVArray()
{
	m_bGotPluBarcodeList = FALSE;
}

/**********************************************************************/

int CBarcodeCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

bool CBarcodeCSVArray::FindBarcodeByNumber ( __int64 nBarcodeNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		__int64 nBarcodeNoArray = GetBarcodeNoInt ( nIndex );
		
		if ( nBarcodeNoArray < nBarcodeNo )
			nStart = nIndex + 1;

		else if ( nBarcodeNoArray > nBarcodeNo )
			nEnd = nIndex - 1;

		else
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

const char* CBarcodeCSVArray::GetBarcodeNoString ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strBarcodeNo = csv.GetString ( 0 );
		::AddLeading ( m_strBarcodeNo, Super.MaxBarcodeLen(), '0' );
		return m_strBarcodeNo;
	}
	return "";
}

/**********************************************************************/

__int64 CBarcodeCSVArray::GetBarcodeNoInt ( int nIndex )
{
	return _atoi64( GetBarcodeNoString( nIndex ) );
}

/**********************************************************************/

void CBarcodeCSVArray::CreatePluBarcodeList()
{
	if ( m_bGotPluBarcodeList == FALSE )
	{
		m_PluBarcodeArray.Rebuild();
		m_bGotPluBarcodeList = TRUE;
	}
}

/**********************************************************************/

void CBarcodeCSVArray::GetBarcodeIdxArray( __int64 nPluNo, CArray<int,int>& BarcodeIdxArray )
{
	BarcodeIdxArray.RemoveAll();

	int nIndex;
	if ( m_PluBarcodeArray.FindPluNo( nPluNo, nIndex ) )
	{
		for ( int nPos = 0; nPos < m_PluBarcodeArray.GetBarcodeNoCount( nIndex ); nPos++ )
		{
			__int64 nBarcodeNo = m_PluBarcodeArray.GetBarcodeNo( nIndex, nPos );

			int nBarcodeIdx;
			if ( FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) )
				BarcodeIdxArray.Add ( nBarcodeIdx );
		}
	}
}

/**********************************************************************/

void CBarcodeCSVArray::GetUnlinkedBarcodeList( CArray<int,int>& BarcodeIdxArray )
{
	BarcodeIdxArray.RemoveAll();

	StatusProgress.Lock( TRUE, "Checking Barcode Links" );

	for ( int nBarcodeIdx = 0; nBarcodeIdx < GetSize(); nBarcodeIdx++ )
	{
		StatusProgress.SetPos( nBarcodeIdx, GetSize() );

		CBarcodeCSVRecord Barcode;
		GetAt( nBarcodeIdx, Barcode );

		int nPluIdx;
		__int64 nPluNo = Barcode.GetPluNoInt();
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
			BarcodeIdxArray.Add( nBarcodeIdx );
	}

	StatusProgress.Unlock();
}

/**********************************************************************/

void CBarcodeCSVArray::DeleteBarcodes( CArray<int,int>& BarcodeIdxArray )
{
	for ( int n = BarcodeIdxArray.GetSize() - 1; n >= 0; n-- )
	{
		int nBarcodeIdx = BarcodeIdxArray.GetAt(n);

		if ( ( nBarcodeIdx >= 0 ) && ( nBarcodeIdx < GetSize() ) )
			RemoveAt( nBarcodeIdx );
	}
}

/**********************************************************************/

void CBarcodeCSVArray::InsertRecord( int nIndex, CBarcodeCSVRecord& BarcodeRecord )
{
	InsertAt( nIndex, BarcodeRecord );
	m_PluBarcodeArray.AddLink( BarcodeRecord.GetPluNoInt(), BarcodeRecord.GetBarcodeNoInt() );
}

/**********************************************************************/

void CBarcodeCSVArray::RemoveRecord( int nIndex )
{
	if ( nIndex >= 0 && nIndex < GetSize() )
	{
		CBarcodeCSVRecord BarcodeRecord;
		GetAt( nIndex, BarcodeRecord );
		RemoveAt( nIndex );
		m_PluBarcodeArray.RemoveLink ( BarcodeRecord.GetPluNoInt(), BarcodeRecord.GetBarcodeNoInt() );
	}
}

/**********************************************************************/

int CBarcodeCSVArray::ImportLine ( int nImportMethod, int& nIndex, CSSImportFile& import, const char* szImportLine )
{
	CCSV csvImport ( szImportLine );
	
	__int64 nBarcodeNo = csvImport.GetInt64(0);
	__int64 nPluNo = csvImport.GetInt64(2);

	if ( ( nBarcodeNo < 1 ) || ( nBarcodeNo > Super.MaxBarcodeInt() ) )
		return IMPORT_LINE_IGNORED;

	if ( ( nPluNo < 1 ) || ( nPluNo > Super.MaxPluInt() ) )
		return IMPORT_LINE_IGNORED;

	if ( FindBarcodeByNumber ( nBarcodeNo, nIndex ) == TRUE )	// plu already exists
	{
		if ( nImportMethod == IMPORT_METHOD_ALL || nImportMethod == IMPORT_METHOD_EXIST )
		{
			CBarcodeCSVRecord NewRecord;
			GetAt ( nIndex, NewRecord );
			NewRecord.ImportLine ( import, csvImport );
			SetAt ( nIndex, NewRecord );
			return IMPORT_LINE_MODIFIED;
		}
	}
	else			//barcodeno not found
	{
		if ( nImportMethod == IMPORT_METHOD_ALL || nImportMethod == IMPORT_METHOD_NEW )
		{
			CBarcodeCSVRecord NewRecord;
			NewRecord.ImportLine ( import, csvImport );
			InsertAt ( nIndex, NewRecord );
			return IMPORT_LINE_ADDED;
		}
	}

	nIndex = -1;
	return IMPORT_LINE_IGNORED;
}

/**********************************************************************/

__int64 CBarcodeCSVArray::GetPluFromBarcode( __int64 nBarcodeNo )
{
	int nBarcodeIdx;
	if ( FindBarcodeByNumber( nBarcodeNo, nBarcodeIdx ) == TRUE )
	{
		CBarcodeCSVRecord Barcode;
		GetAt( nBarcodeIdx, Barcode );
		return Barcode.GetPluNoInt();
	}

	return 0;
}

/**********************************************************************/
