/**********************************************************************/
#include "SPOSKeyboardImportPluDatabase.h"
/**********************************************************************/

CSPOSKeyboardImportPluCSVRecord::CSPOSKeyboardImportPluCSVRecord()
{
	m_nPluNumber = 0;
	m_nDeptNo = 0;
	m_nCategoryNo = 0;
	m_strEcrText = "";
	m_strKeyText = "";
	m_strRepText = "";
	m_strBasePluEcrText = "";
	m_strImageFilename = "";
	m_nVersion = 0;
}

/**********************************************************************/

void CSPOSKeyboardImportPluCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	m_nPluNumber = 0;
	m_nDeptNo = 0;
	m_nCategoryNo = 0;
	m_strEcrText = "";
	m_strKeyText = "";
	m_strRepText = "";
	m_strBasePluEcrText = "";
	m_strImageFilename = "";
	m_nVersion = 0;
	
	m_nPluNumber = csv.GetInt64(0);
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	case 2:	V2ConvertFromCSV ( csv );		break;
	case 3:	V3To5ConvertFromCSV ( csv, 3 );	break;
	case 4:	V3To5ConvertFromCSV ( csv, 4 );	break;
	case 5:	V3To5ConvertFromCSV ( csv, 5 );	break;
	}
}

/**********************************************************************/

void CSPOSKeyboardImportPluCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	m_strEcrText = csv.GetString(2);
	m_strRepText = csv.GetString(3);
	m_nVersion = 1;
}

/**********************************************************************/

void CSPOSKeyboardImportPluCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	m_strEcrText = csv.GetString(2);
	m_strRepText = csv.GetString(3);
	m_strBasePluEcrText = csv.GetString(4);
	m_nVersion = 2;
}

/**********************************************************************/

void CSPOSKeyboardImportPluCSVRecord::V3To5ConvertFromCSV ( CCSV& csv, int nVer )
{
	int nOffset = 2;

	if ( nVer >= 4 )
	{
		m_nDeptNo = csv.GetInt(nOffset++);
		m_nCategoryNo = csv.GetInt(nOffset++);
	}

	m_strEcrText = csv.GetString(nOffset++);
	m_strKeyText = csv.GetString(nOffset++);
	m_strRepText = csv.GetString(nOffset++);
	m_strBasePluEcrText = csv.GetString(nOffset++);

	if ( nVer >= 5 )
		m_strImageFilename = csv.GetString(nOffset++);

	m_nVersion = nVer;
}

/**********************************************************************/

const char* CSPOSKeyboardImportPluCSVRecord::GetComboOrListText()
{
	CString strPluNo;
	strPluNo.Format( "%I64d", m_nPluNumber );
	int nPluLen = strPluNo.GetLength();

	int nMinDirectLen = 1;
	if ( SPOSKeyboardManager.GetLayoutOptions() -> GetMaxBasePluLen() > 0 )
		nMinDirectLen = SPOSKeyboardManager.GetLayoutOptions() -> GetMaxBasePluLen()+ 2;

	CStringArray arrayTexts;

	if ( ( ( m_nPluNumber % 10 ) == 0 ) || ( nPluLen >= nMinDirectLen ) )
	{
		arrayTexts.Add( m_strRepText );
		arrayTexts.Add( m_strEcrText );
	}
	else
	{
		arrayTexts.Add( m_strEcrText );
		arrayTexts.Add( m_strRepText );
		arrayTexts.Add( m_strBasePluEcrText );
	}
	
	m_strPluText = GetPluText( arrayTexts );
	return m_strPluText;
}

/**********************************************************************/

const char* CSPOSKeyboardImportPluCSVRecord::GetSharpPosDefaultText()
{
	CStringArray arrayTexts;

	switch( SPOSKeyboardManager.GetSPOSVersion() -> GetSPOSVersion())
	{
	case SPOS_V4_1216:
	case SPOS_V4_1300:
	case SPOS_V4_1392:
	case SPOS_V4_1510:
	case SPOS_V4_1519:
	case SPOS_V4_1522:
	case SPOS_V4_1525:
	case SPOS_V4_1734:
	case SPOS_V4_1901:
	case SPOS_V4_1977:
	case SPOS_V4_2012:
	case SPOS_V4_2067:
	case SPOS_V4_2133:
	case SPOS_V4_2161:
	case SPOS_V4_2455:
	case SPOS_V4_2606:
	case SPOS_V4_2_369:
	case SPOS_V4_2_631:
	case SPOS_V4_2_714:
	case SPOS_V4_2_919:
	case SPOS_V4_2_959:
	case SPOS_V4_2_1081:
	case SPOS_V4_2_1087:
	case SPOS_V4_2_1384:
	case SPOS_V4_2_1456:
	case SPOS_V4_2_1530:
	case SPOS_V4_2_1579:
	case SPOS_V4_2_1605:
	case SPOS_V4_2_1618:
	case SPOS_V4_2_1622:
	case SPOS_V4_3_18:
	case SPOS_V4_3_93:
	case SPOS_V4_3_104:
		arrayTexts.Add( m_strKeyText );
		arrayTexts.Add( m_strEcrText );
		m_strPluText = GetPluText( arrayTexts );
		break;

	default:
		m_strPluText = m_strEcrText;
		break;
	}

	return m_strPluText;
}

/**********************************************************************/

const char* CSPOSKeyboardImportPluCSVRecord::GetBackOfficeDefaultText()
{
	CStringArray arrayTexts;

	switch( SPOSKeyboardManager.GetLayoutOptions() -> GetPluTextOrder() )
	{
	case 1:
		arrayTexts.Add( m_strKeyText );
		arrayTexts.Add( m_strRepText );
		arrayTexts.Add( m_strEcrText );
		break;

	case 2:
		arrayTexts.Add( m_strEcrText );
		arrayTexts.Add( m_strKeyText );
		arrayTexts.Add( m_strRepText );
		break;

	case 3:
		arrayTexts.Add( m_strEcrText );
		arrayTexts.Add( m_strRepText );
		arrayTexts.Add( m_strKeyText );
		break;

	case 4:
		arrayTexts.Add( m_strRepText );
		arrayTexts.Add( m_strKeyText );
		arrayTexts.Add( m_strEcrText );
		break;

	case 5:
		arrayTexts.Add( m_strRepText );
		arrayTexts.Add( m_strEcrText );
		arrayTexts.Add( m_strKeyText );
		break;

	case 0:
	default:
		arrayTexts.Add( m_strKeyText );
		arrayTexts.Add( m_strEcrText );
		arrayTexts.Add( m_strRepText );
		break;
	}

	m_strPluText = GetPluText( arrayTexts );
	return m_strPluText;
}

/**********************************************************************/

const char* CSPOSKeyboardImportPluCSVRecord::GetPluText( CStringArray& arrayTexts )
{
	m_strPluText = "";
	
	for ( int n = 0; n < arrayTexts.GetSize(); n++ )
	{
		m_strPluText = arrayTexts.GetAt(n);
		::TrimSpaces( m_strPluText, FALSE );

		if ( m_strPluText != "" )
			break;
	}
		
	if ( m_strPluText == "" )
		m_strPluText.Format ( "Plu %14.14I64d", m_nPluNumber );

	return m_strPluText;
}

/**********************************************************************/

void CSPOSKeyboardImportPluCSVRecord::AddToCombo( CSSComboBox& Combo )
{	
	CString strLine;
	strLine.Format ( "%s %s",
		GetFormattedPluNumber(),
		GetComboOrListText() );

	Combo.AddString( strLine );
}

/**********************************************************************/

void CSPOSKeyboardImportPluCSVRecord::AddToList( CSSListCtrl& List )
{
	CString strLine;
	strLine.Format ( "%s %s",
		GetFormattedPluNumber(),
		GetComboOrListText() );

	List.AddString( strLine );
}

/**********************************************************************/

const char* CSPOSKeyboardImportPluCSVRecord::GetFormattedPluNumber()
{
	m_strPluNumber.Format ( "%14.14I64d", m_nPluNumber );
	return m_strPluNumber;
}

/**********************************************************************/

CSPOSKeyboardImportPluCSVArray::CSPOSKeyboardImportPluCSVArray() : CCSVArray()
{
	m_bImportPlu = FALSE;
}

/**********************************************************************/

void CSPOSKeyboardImportPluCSVArray::Reset() 
{
	RemoveAt( 0, GetSize() );
}

/**********************************************************************/

BOOL CSPOSKeyboardImportPluCSVArray::FindPluItem ( __int64 nPluNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	__int64 nPluNoToFind = nPluNo;
	CSPOSKeyboardImportPluCSVRecord PluRecord;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;
		GetAt ( nIndex, PluRecord );
		__int64 nArrayPluNo = PluRecord.GetPluNumber();
		
		if ( nArrayPluNo < nPluNoToFind )
			nStart = nIndex + 1;

		else if ( nArrayPluNo > nPluNoToFind )
			nEnd = nIndex - 1;

		else
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

BOOL CSPOSKeyboardImportPluCSVArray::Read( const char* szPath )
{
	m_strPath = szPath;
	return CCSVArray::Read(m_strPath);
}

/**********************************************************************/

BOOL CSPOSKeyboardImportPluCSVArray::ReRead()
{
	if ( m_strPath != "" )
		return CCSVArray::Read(m_strPath);
	else
		return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardImportPluCSVArray::GotDeptAndCategory()
{
	bool bResult = FALSE;

	if ( GetSize() != 0 )
	{
		CSPOSKeyboardImportPluCSVRecord PluRecord;
		GetAt( 0, PluRecord );
		bResult = ( PluRecord.GetVersion() >= 4 );
	}

	return bResult;
}

/**********************************************************************/
