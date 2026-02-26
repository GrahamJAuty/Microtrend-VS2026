/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "BarcodeCSVArray.h"
/**********************************************************************/

CBarcodeCSVRecord::CBarcodeCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CBarcodeCSVRecord::ClearRecord()
{
	m_nBarcodeNo = 0;
	m_nPluNo = 0;
	m_nModifier = 0;
	m_bPrintShelf = TRUE;
	m_bPrintProduct = TRUE;
}

/**********************************************************************/

void CBarcodeCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nBarcodeNo );
	csv.Add ( BARCODEDB_VERSION_NO );
	csv.Add ( m_nPluNo );
	csv.Add ( m_nModifier );
	csv.Add ( m_bPrintShelf );
	csv.Add	( m_bPrintProduct );
}

/**********************************************************************/

void CBarcodeCSVRecord::ConvertToCSVForCombinedExport ( CCSV& csv )
{
	csv.Add ( GetBarcodeNoForDownload() );
	csv.Add ( m_bPrintShelf );
	csv.Add	( m_bPrintProduct );
}

/**********************************************************************/

void CBarcodeCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetBarcodeNo ( csv.GetInt64(0) );
	
	switch ( csv.GetInt(1) )			/* version number */
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	case 2:	V2to3ConvertFromCSV ( csv, 2 );	break;
	case 3:	V2to3ConvertFromCSV ( csv, 3 );	break;
	}
}

/**********************************************************************/

void CBarcodeCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	SetPluNo ( csv.GetInt64(2) );
	SetPrintShelfFlag( csv.GetBool(3) );
	SetPrintProductFlag( csv.GetBool(3) );
}

/**********************************************************************/

void CBarcodeCSVRecord::V2to3ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nOffset = 2;
	SetPluNo ( csv.GetInt64(nOffset++) );
	if ( nVer >= 3 ) SetModifier( csv.GetInt( nOffset++ ) );
	SetPrintShelfFlag( csv.GetBool(nOffset++) );
	SetPrintProductFlag( csv.GetBool(nOffset++) );
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CBarcodeCSVRecord::SetBarcodeNo ( __int64 nBarcodeNo )
{
	Set( m_nBarcodeNo, nBarcodeNo, Barcode::BarcodeNo );
}

/**********************************************************************/

void CBarcodeCSVRecord::SetPluNo ( __int64 nPluNo )
{
	Set( m_nPluNo, nPluNo, Plu::PluNo );
}

/**********************************************************************/

void CBarcodeCSVRecord::SetModifier ( int nMod )
{
	Set( m_nModifier, nMod, Barcode::Modifier );
}

/**********************************************************************/

void CBarcodeCSVRecord::SetPrintShelfFlag( bool bFlag )
{
	m_bPrintShelf = bFlag;
}

/**********************************************************************/

void CBarcodeCSVRecord::SetPrintProductFlag( bool bFlag )
{
	m_bPrintProduct = bFlag;
}

/**********************************************************************/

CString CBarcodeCSVRecord::GetHeader()
{
	CString strBuf;
	CCSV csv;

	csv.Add ( Barcode::BarcodeNo.Label );
	csv.Add ( "" );				// protected "VersionNo"
	csv.Add ( Barcode::PluNo.Label );
	csv.Add ( Barcode::Modifier.Label );
	csv.Add ( Barcode::PrintShelf.Label );
	csv.Add ( Barcode::PrintProduct.Label );

	CString strHeader = csv.GetLine();
	return strHeader;
}

/**********************************************************************/

CString CBarcodeCSVRecord::GetHeaderForCombinedExport()
{
	CCSV csv;
	csv.Add ( Barcode::BarcodeNo.Label );
	csv.Add ( Barcode::PrintShelf.Label );
	csv.Add ( Barcode::PrintProduct.Label );

	CString strHeader = csv.GetLine();
	return strHeader;
}

/**********************************************************************/

void CBarcodeCSVRecord::ImportLine ( CSSImportFile& import, CCSV& csv )
{
	int n = 0;

	if ( import.IsRequired(n) == TRUE )	Import( csv.GetString(n), m_nBarcodeNo, Barcode::BarcodeNo );	n++;

	n++;		// protected "VersionNo"
	
	if ( import.IsRequired(n) == TRUE )	Import( csv.GetString(n), m_nPluNo, Barcode::PluNo );		n++;
	if ( import.IsRequired(n) == TRUE )	Import( csv.GetString(n), m_nModifier, Barcode::Modifier );	n++;
	if ( import.IsRequired(n) == TRUE )	Import ( csv.GetString(n), m_bPrintShelf );	n++;
	if ( import.IsRequired(n) == TRUE )	Import ( csv.GetString(n), m_bPrintProduct ); n++;
}

/**********************************************************************/

const char* CBarcodeCSVRecord::GetPrintFlagText( bool bShort )
{
	m_strPrintFlagText = "";

	int nType = 0;
	if ( TRUE == m_bPrintShelf )	nType += 1;
	if ( TRUE == m_bPrintProduct )	nType += 2;

	switch( nType )
	{
	case 1:	
		if ( FALSE == bShort )
			m_strPrintFlagText = "Shelf only";
		else
			m_strPrintFlagText = "Shelf";
		break;
	
	case 2:
		if ( FALSE == bShort )
			m_strPrintFlagText = "Product only";
		else
			m_strPrintFlagText = "Product";
		break;
	
	case 3:
		if ( FALSE == bShort )
			m_strPrintFlagText = "Shelf / Product";
		else
			m_strPrintFlagText = "Yes";
		break;
	
	case 0:
	default:
		m_strPrintFlagText = "No";			
		break;
	}

	return m_strPrintFlagText;
}

/**********************************************************************/

const char* CBarcodeCSVRecord::GetBarcodeNoForDownload()
{
	m_strBarcodeNoForDownload.Format( "%I64d", m_nBarcodeNo );
		
	if ( m_strBarcodeNoForDownload != "" )
	{
		int nMinLength = DealerFlags.GetSPosBarcodeLength();

		switch( nMinLength )
		{
		case 0:
			break;

		case 1:	
			switch( m_strBarcodeNoForDownload.GetLength() )
			{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				::AddLeading( m_strBarcodeNoForDownload, 6, '0' );
				break;

			case 7:
				::AddLeading( m_strBarcodeNoForDownload, 8, '0' );
				break;

			case 9:
			case 10:
			case 11:
				::AddLeading( m_strBarcodeNoForDownload, 12, '0' );
				break;
			}
			break;

		default:
			if ( m_strBarcodeNoForDownload.GetLength() < nMinLength )
				::AddLeading( m_strBarcodeNoForDownload, nMinLength, '0' );
			break;
		}
	}

	return m_strBarcodeNoForDownload;
}

/**********************************************************************/

const char* CBarcodeCSVRecord::GetPluNoString()
{
	if ( m_nPluNo != 0 )
		::FormatPluNo( m_nPluNo, m_strPluNoString );	
	else
		m_strPluNoString = "";

	return m_strPluNoString;
}

/**********************************************************************/

const char* CBarcodeCSVRecord::GetBarcodeNoString()
{
	::FormatBarcodeNo( m_nBarcodeNo, m_strBarcodeNoString );
	return m_strBarcodeNoString;
}

/**********************************************************************/
