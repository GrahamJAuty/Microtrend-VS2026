 /**********************************************************************/
#include "BarcodeCSVArray.h"
//#include "globalfunctions.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
#include "SysInfo.h"
/**********************************************************************/
#include "BarcodePluNoTable.h"
/**********************************************************************/
static const int MAX_BARCODE_TABLE_SIZE = 14;
/**********************************************************************/

CBarcodePluNoTable::CBarcodePluNoTable()
{
	m_bFatalReadError = FALSE;
}

/**********************************************************************/

bool CBarcodePluNoTable::Read()
{
	RemoveAll();
	m_arrayTable.RemoveAll();
	
	CFilenameUpdater FnUp( SysFiles::BarcodeEntryTable );
	if ( CFileStringArray::Read ( FnUp.GetFilenameToUse(), MAX_BARCODE_TABLE_SIZE ) == FALSE )
	{
		if ( GetLastError() != ERROR_FILE_NOT_FOUND )
		{
			m_bFatalReadError = TRUE;
			return FALSE;
		}
	}

	for ( int n = 0; ( n < MAX_BARCODE_TABLE_SIZE ) && ( n < GetSize() ); n++ )
	{
		CCSV csv( GetAt( n ) );
		
		CBarcodePluNoTableInfo info;
		info.m_strBarcodeFormat = csv.GetString(0);
		info.m_strPluFormat = csv.GetString(1);
		m_arrayTable.Add( info );
	}
	
	while ( m_arrayTable.GetSize() < MAX_BARCODE_TABLE_SIZE )
	{
		CBarcodePluNoTableInfo info ;
		info.m_strBarcodeFormat = GetDefaultBarcodeFormat( m_arrayTable.GetSize() + 1 );
		info.m_strPluFormat = GetDefaultPluFormat( m_arrayTable.GetSize() + 1 );
		m_arrayTable.Add( info );
	}
		
	while ( GetSize() > MAX_BARCODE_TABLE_SIZE )
		m_arrayTable.RemoveAt( m_arrayTable.GetSize() - 1 );

	//APPEND EXTRA '0' AT THE START OF LEGACY 14 DIGIT BARCODES
	for ( int n = 0; n < m_arrayTable.GetSize(); n++ )
	{
		CString strFormat = m_arrayTable[n].m_strBarcodeFormat;
	
		if ( strFormat.GetLength() != Super.MaxBarcodeLen() )
			::AddLeading( strFormat, Super.MaxBarcodeLen(), '0' );

		m_arrayTable[n].m_strBarcodeFormat = strFormat;
	}

	for ( int n = 0; n < m_arrayTable.GetSize() - 1; n++ )
	{
		CString strFormat = m_arrayTable[n].m_strPluFormat;

		if ( strFormat.GetLength() != Super.MaxPluLen() )
			m_arrayTable[n].m_strPluFormat = GetDefaultPluFormat( n + 1 );
	}
	
	return TRUE;
}

/**********************************************************************/

bool CBarcodePluNoTable::Write()
{
	CFilenameUpdater FnUp ( SysFiles::BarcodeEntryTable );
	
	RemoveAll();

	for ( int n = 0; n < m_arrayTable.GetSize(); n++ )
	{
		CCSV csv;
		csv.Add( m_arrayTable[n].m_strBarcodeFormat );
		csv.Add( m_arrayTable[n].m_strPluFormat );
		Add( csv.GetLine() );
	}
	
	return CFileStringArray::Write ( FnUp.GetFilenameToUse() );
}

/**********************************************************************/

void CBarcodePluNoTable::SetBarcodeFormat ( int nBarcodeNoLen, const char* szFormat )
{
	int nIndex = nBarcodeNoLen - 1;
	CString strFormat = szFormat;

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTable.GetSize() ) )
		if ( strFormat.GetLength() == Super.MaxBarcodeLen() )
			m_arrayTable[nIndex].m_strBarcodeFormat = strFormat;
}

/**********************************************************************/

void CBarcodePluNoTable::SetPluFormat ( int nPluNoLen, const char* szFormat )
{
	int nIndex = nPluNoLen - 1;
	CString strFormat = szFormat;

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTable.GetSize() ) )
		if ( strFormat.GetLength() == Super.MaxPluLen() )
			m_arrayTable[nIndex].m_strPluFormat = strFormat;
}

/**********************************************************************/

const char* CBarcodePluNoTable::GetBarcodeFormat ( int nBarcodeNoLen )
{
	int nIndex = nBarcodeNoLen - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTable.GetSize() ) )
	{
		m_strText = m_arrayTable[nIndex].m_strBarcodeFormat;
		return m_strText;
	}

	return "";
}

/**********************************************************************/

const char* CBarcodePluNoTable::GetPluFormat ( int nPluNoLen )
{
	int nIndex = nPluNoLen - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTable.GetSize() ) )
	{
		m_strText = m_arrayTable[nIndex].m_strPluFormat;
		return m_strText;
	}

	return "";
}

/**********************************************************************/

void CBarcodePluNoTable::RestoreDefaults( int nBarcodeAction, int nPluAction )
{
	if ( 1 == nBarcodeAction )
	{
		for ( int nIndex = 0; nIndex < m_arrayTable.GetSize(); nIndex++ )
			m_arrayTable[nIndex].m_strBarcodeFormat = GetDefaultBarcodeFormat(nIndex + 1);
	}

	switch( nPluAction )
	{
	case 1:
		{
			for ( int nIndex = 0; nIndex < m_arrayTable.GetSize(); nIndex++ )
				m_arrayTable[nIndex].m_strPluFormat = GetDefaultPluFormat(nIndex + 1);	
		}
		break;

	case 2:
		{
			for ( int nIndex = 0; nIndex < m_arrayTable.GetSize(); nIndex++ )
			{
				CString str1 = m_arrayTable[nIndex].m_strBarcodeFormat;
				CString str2 = m_arrayTable[nIndex].m_strPluFormat;

				if ( str1.GetLength() != Super.MaxPluLen() + 1 )
					continue;

				CString strTest = str1.Left(1);
				if ( ( strTest < "0" ) || ( strTest > "9" ) )
					continue;

				m_arrayTable[nIndex].m_strPluFormat = str1.Right( str1.GetLength() - 1 );
			}
		}
		break;
	}
}

/**********************************************************************/

const char* CBarcodePluNoTable::GetDefaultBarcodeFormat ( int nBarcodeNoLen )
{
	int nIndex = nBarcodeNoLen - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < MAX_BARCODE_TABLE_SIZE ) )
	{
		m_strText = "";
		AddTrailing ( m_strText, Super.MaxBarcodeLen() - nBarcodeNoLen, '0' );
		AddTrailing ( m_strText, Super.MaxBarcodeLen(), 'n' );
		return m_strText;
	}
		
	return "";
}

/**********************************************************************/

const char* CBarcodePluNoTable::GetDefaultPluFormat ( int nPluNoLen )
{
	int nIndex = nPluNoLen - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < MAX_BARCODE_TABLE_SIZE ) )
	{
		m_strText = "";
		AddTrailing ( m_strText, Super.MaxPluLen() - nPluNoLen, '0' );
		AddTrailing ( m_strText, Super.MaxPluLen(), 'n' );
		return m_strText;
	}
		
	return "";
}

/**********************************************************************/

bool CBarcodePluNoTable::BarcodeHasCheckDigit ( int nBarcodeNoLen )
{
	int nIndex = nBarcodeNoLen - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTable.GetSize() ) )
	{
		CString strFormat = m_arrayTable[nIndex].m_strBarcodeFormat;

		if ( strFormat.Right ( 1 ) == "c" )
			return TRUE;
	}
		
	return FALSE;	
}

/**********************************************************************/

bool CBarcodePluNoTable::PluHasCheckDigit ( int nPluNoLen )
{
	int nIndex = nPluNoLen - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayTable.GetSize() ) )
	{
		CString strFormat = m_arrayTable[nIndex].m_strPluFormat;

		if ( strFormat.Right ( 1 ) == "c" )
			return TRUE;
	}
		
	return FALSE;	
}

/**********************************************************************/

const char* CBarcodePluNoTable::ConvertBarcode ( const char* szUserBarcode )
{
	CString strUserBarcode = szUserBarcode;
	int nUserBarcodeLen = strUserBarcode.GetLength();
	
	if ( nUserBarcodeLen == 0 )
	{
		m_strText = "";
		return m_strText;
	}

	if ( nUserBarcodeLen >= Super.MaxBarcodeLen() )
	{
		m_strText = strUserBarcode.Left ( Super.MaxBarcodeLen() );
		return m_strText;
	}

	CString strFormat = GetBarcodeFormat ( nUserBarcodeLen );
	if ( strFormat.GetLength() != Super.MaxBarcodeLen() )
		strFormat = GetDefaultBarcodeFormat( nUserBarcodeLen );

	m_strText = strFormat;

	int nInsertPos = m_strText.Find ( 'n' );

	if ( ( nInsertPos >= 0 ) && ( nInsertPos <= m_strText.GetLength() - nUserBarcodeLen ) )
	{
		for ( int n = 0; n < nUserBarcodeLen; n++ )
			m_strText.SetAt( nInsertPos + n, strUserBarcode.GetAt(n) );
	}

	if ( BarcodeHasCheckDigit ( nUserBarcodeLen ) == TRUE )
	{
		CString strTemp = m_strText.Left ( Super.MaxBarcodeLen() - 1 );
		int nDigit = GetCheckDigit ( (const char*) strTemp );
		m_strText.Format ( "%s%d", strTemp, nDigit );
	}

	return m_strText;
}

/**********************************************************************/

const char* CBarcodePluNoTable::ConvertBarcodeInt( __int64 nBarcodeNo )
{
	CString strBarcodeNo;
	strBarcodeNo.Format( "%I64d", nBarcodeNo );
	return ConvertBarcode( strBarcodeNo );
}

/**********************************************************************/

const char* CBarcodePluNoTable::ConvertPlu( const char* szUserPlu )
{
	CString strUserPlu = szUserPlu;
	int nUserPluLen = strUserPlu.GetLength();
	
	if ( nUserPluLen == 0 )
	{
		m_strText = "";
		return m_strText;
	}

	if ( nUserPluLen >= Super.MaxPluLen() )
	{
		m_strText = strUserPlu.Left ( Super.MaxPluLen() );
		return m_strText;
	}

	CString strFormat = GetPluFormat ( nUserPluLen );
	if ( strFormat.GetLength() != Super.MaxPluLen() )
		strFormat = GetDefaultPluFormat( nUserPluLen );

	m_strText = strFormat;

	int nInsertPos = m_strText.Find ( 'n' );

	if ( ( nInsertPos >= 0 ) && ( nInsertPos <= m_strText.GetLength() - nUserPluLen ) )
	{
		for ( int n = 0; n < nUserPluLen; n++ )
			m_strText.SetAt( nInsertPos + n, strUserPlu.GetAt(n) );
	}

	if ( PluHasCheckDigit ( nUserPluLen ) == TRUE )
	{
		CString strTemp = m_strText.Left ( Super.MaxPluLen() - 1 );
		int nDigit = GetCheckDigit ( (const char*) strTemp );
		m_strText.Format ( "%s%d", strTemp, nDigit );
	}

	if ( ::TestNumeric( m_strText ) == FALSE )
	{
		m_strText = strUserPlu;
		::AddLeading( m_strText, Super.MaxPluLen(), '0' );
		m_strText = strUserPlu.Left ( Super.MaxPluLen() );
	}

	return m_strText;
}

/**********************************************************************/

const char* CBarcodePluNoTable::ConvertPluInt( __int64 nPluNo )
{
	CString strPluNo;
	strPluNo.Format( "%I64d", nPluNo );
	return ConvertPlu( strPluNo );
}

/**********************************************************************/

const char* CBarcodePluNoTable::ConvertShortPlu( const char* szUserPluNo )
{
	int nLen = SysInfo.GetMaxBasePluLen();

	m_strText = szUserPluNo;

	::TrimSpaces ( m_strText, TRUE );
	
	if ( m_strText.GetLength() > nLen )
		m_strText = m_strText.Right ( nLen );

	if ( m_strText.GetLength() > 0 )
		AddLeading ( m_strText, nLen, '0' );

	return m_strText;
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

const char* CBarcodePluNoTable::ConvertStockCode ( const char* szUserStock )
{
	CString strUserStock = szUserStock;
	::TrimSpaces ( strUserStock, FALSE );

	if ( strUserStock.GetLength() == 0 )
	{
		m_strText = "";
		return m_strText;
	}

	if ( ( StockOptions.GetStockAddLeadingZerosFlag() == FALSE ) )
	{
		m_strText = strUserStock;
		return m_strText;
	}

	//no conversion if stock code contains non numeric characters
	for ( int nIndex = 0; nIndex < strUserStock.GetLength(); nIndex++ )
	{
		char c = strUserStock.GetAt ( nIndex );

		if ( c < '0' || c > '9' )
		{
			m_strText = strUserStock;
			return m_strText;
		}
	}
	return ConvertPlu ( strUserStock );
}

/**********************************************************************/
#endif
/**********************************************************************/

int CBarcodePluNoTable::GetCheckDigit( const char* szBarcodeNo )
{
	CString strBarcodeNo = szBarcodeNo;
	
	int nTotal = 0;
	
	for ( int nPos = strBarcodeNo.GetLength() - 1; nPos >=0; nPos -= 2 )
	{
		CString strDigit ( strBarcodeNo.GetAt(nPos) );
		nTotal += atoi ( strDigit ) * 3;
	}
		
	for ( int nPos = strBarcodeNo.GetLength() - 2; nPos >=0; nPos -= 2 )
	{
		CString strDigit ( strBarcodeNo.GetAt(nPos) );
		nTotal += atoi ( strDigit );
	}
	
	return ( ( 10 - ( nTotal % 10 ) ) % 10 );
}

/**********************************************************************/

bool CBarcodePluNoTable::ValidateCheckDigit( const char* szBarcodeNo )
{
	CString strBarcodeNo = szBarcodeNo;
	int nLength = strBarcodeNo.GetLength();

	if ( nLength < 2 )
		return FALSE;

	CString strCurrentCheckDigit = strBarcodeNo.Right ( 1 );
	CString strBeforeCheckDigit = strBarcodeNo.Left ( nLength - 1 );

	CString strCorrectCheckDigit;
	strCorrectCheckDigit.Format ( "%d", GetCheckDigit ( strBeforeCheckDigit ) );

	if ( strCorrectCheckDigit == strCurrentCheckDigit )
		return TRUE;
	else
		return FALSE;
}

/**********************************************************************/

bool CBarcodePluNoTable::IsValidEan( const char* szBarcodeNo )
{
	CString strBarcodeNo = szBarcodeNo;
	::TrimSpaces ( strBarcodeNo, TRUE );
		
	if ( strBarcodeNo.GetLength() > 13 )
		return FALSE;

	if ( strBarcodeNo.GetLength() > 8 && strBarcodeNo.GetLength() < 13 )
		::AddLeading ( strBarcodeNo, 13, '0' );
			
	if ( strBarcodeNo.GetLength() < 8 )
		::AddLeading ( strBarcodeNo, 8, '0' );

	return ValidateCheckDigit ( strBarcodeNo );
}

/**********************************************************************/

bool CBarcodePluNoTable::IsValidEan( __int64 nBarcodeNo )
{
	CString strBarcodeNo;
	strBarcodeNo.Format( "%I64d", nBarcodeNo );

	if ( strBarcodeNo.GetLength() > 13 )
		return FALSE;

	if ( strBarcodeNo.GetLength() > 8 && strBarcodeNo.GetLength() < 13 )
		::AddLeading ( strBarcodeNo, 13, '0' );
			
	if ( strBarcodeNo.GetLength() < 8 )
		::AddLeading ( strBarcodeNo, 8, '0' );

	return ValidateCheckDigit ( strBarcodeNo );
}

/**********************************************************************/

void CBarcodePluNoTable::RememberSettings()
{
	m_arrayMemory.RemoveAll();
	
	for ( int n = 0; n < m_arrayTable.GetSize(); n++ )
	{
		CBarcodePluNoTableInfo info = m_arrayTable.GetAt( n );
		m_arrayMemory.Add( info );
	}
}

/**********************************************************************/

void CBarcodePluNoTable::RestoreSettings()
{
	m_arrayTable.RemoveAll();
	
	for ( int n = 0; n < m_arrayMemory.GetSize(); n++ )
	{
		CBarcodePluNoTableInfo info = m_arrayMemory.GetAt( n );
		m_arrayTable.Add( info );
	}
}

/**********************************************************************/







	

