/**********************************************************************/
#include "CustomField.h"
/**********************************************************************/

CCustomFieldValueCSVRecord::CCustomFieldValueCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::ClearRecord()
{
	m_strKey = "1";
	m_nNumber = 1;
	m_strName = "";
	m_bProtect = FALSE;
	m_strFieldSetFlags = "";
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strKey );
	csv.Add ( m_nNumber );
	csv.Add ( CUSTOM_FIELD_DB_VERSION );
	csv.Add ( m_strName );
	csv.Add ( m_bProtect );
	csv.Add ( m_strFieldSetFlags );
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	SetKey( csv.GetString(0) );
	SetNumber( csv.GetInt(1) );
	
	switch ( csv.GetInt(2) )
	{
	case 1:	V1ConvertFromCSV( csv );	break;
	}
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int n = 3;
	SetName( csv.GetString(n++) );
	SetProtectFlag( csv.GetBool(n++) );
	SetFieldSetFlags( csv.GetString(n++) );
}

/**********************************************************************/
//	Get header
/**********************************************************************/

CString CCustomFieldValueCSVRecord::GetHeader()
{
	CString strHeader;
	CCSV csv;
	csv.Add ( CustomFieldValue::Key.Label );
	csv.Add ( "" );									//PROTECTED CUSTOM FIELD NUMBER
	csv.Add ( "" );									//PROTECTED VERSION NUMBER FIELD
	csv.Add ( CustomFieldValue::Name.Label );
	csv.Add ( "" );									//PROTECTED DELETE PROTECTION FLAG
	csv.Add ( CustomFieldValue::SetFlags.Label );
	strHeader = csv.GetLine();
	return strHeader;
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::SetNumber ( int nNo )
{
	Set ( m_nNumber, nNo, CustomFieldValue::Number );
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::SetName( const char* szName )
{
	CString strName = StripSpaces ( szName );

	if ( strName == "" )
		strName.Format ( "Value %d", m_nNumber );

	Set ( m_strName, strName, CustomFieldValue::Name );
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::SetKey( const char* szKey )
{
	CString strKey = StripSpaces ( szKey );

	if ( strKey == "" )
		strKey.Format ( "%d", m_nNumber );

	Set ( m_strKey, strKey, CustomFieldValue::Key );
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::SetFieldSetFlags( const char* sz )
{
	Set( m_strFieldSetFlags, sz, CustomFieldValue::SetFlags );
}

/**********************************************************************/
//	Merge
/**********************************************************************/

CString CCustomFieldValueCSVRecord::StripSpaces ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	return strText;
}

/**********************************************************************/

void CCustomFieldValueCSVRecord::ImportLine ( CSSImportFile& import, CCSV& csv )
{
	int n = 0;
	
	if ( import.IsRequired(n) == FALSE )
		return;

	CString strKey = StripSpaces( csv.GetString(n) );
	if ( strKey == "" )
		return;

	Import ( strKey, m_strKey, CustomFieldValue::Key ); 
	n++;
	
	n++;	//SKIP PROTECTED CUSTOM FIELD NUMBER
	n++;	//SKIP PROTECTED VERSION NUMBER
	
	if ( import.IsRequired(n) == TRUE ) 
		Import ( StripSpaces ( csv.GetString (n) ), m_strName, CustomFieldValue::Name ); 
	n++;
	
	n++;	//SKIP PROTECTED DELETE PROTECTION FLAG

	if ( import.IsRequired(n) == TRUE ) 
		Import ( StripSpaces ( csv.GetString (n) ), m_strFieldSetFlags, CustomFieldValue::SetFlags );
}

/**********************************************************************/

CCustomFieldValueCSVArray::CCustomFieldValueCSVArray() : CSharedCSVArray( ',', '"', FALSE, TRUE, TRUE )
{
	m_bIsValid = TRUE;
	m_nErrorType = 0;
	m_nErrorLine = 0;
	m_nFieldNo = 1;
	m_nNextNumber = 1;
	m_bGlobalMode = FALSE;
}

/**********************************************************************/

void CCustomFieldValueCSVArray::SetFieldNo( int n )
{
	if ( ( n >= 1 ) && ( n <= MAX_ITEM_CUSTOM_FIELDS ) )
		m_nFieldNo = n;
}

/**********************************************************************/

void CCustomFieldValueCSVArray::RemoveAll()
{
	for ( int nIdx = GetSize() - 1; nIdx >= 0; nIdx-- )
		RemoveAt ( nIdx );

	m_arrayNumberMap.RemoveAll();
}

/**********************************************************************/

void CCustomFieldValueCSVArray::PushNextNumberToArray()
{
	CCustomFieldValueCSVRecord Field;
	Field.SetKey( "" );
	Field.SetNumber( m_nNextNumber );
	Field.SetName( "" );
	InsertAt( 0, Field );
}

/**********************************************************************/

void CCustomFieldValueCSVArray::PullNextNumberFromArray()
{
	if ( GetSize() != 0 )
	{
		CCustomFieldValueCSVRecord Field;
		GetAt( 0, Field );
		m_nNextNumber = Field.GetNumber();
		RemoveAt(0);
	}
	else
		m_nNextNumber = 1;
}

/**********************************************************************/

int CCustomFieldValueCSVArray::GetNumber ( int nIdx )
{
	int nResult = 0;

	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
	{
		CCustomFieldValueCSVRecord Value;
		GetAt ( nIdx, Value );
		nResult = Value.GetNumber();
	}

	return nResult;
}

/**********************************************************************/

const char* CCustomFieldValueCSVArray::GetKey ( int nIdx )
{
	m_strTemp = "";
	
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
	{
		CCustomFieldValueCSVRecord Value;
		GetAt ( nIdx, Value );
		m_strTemp = Value.GetKey();
	}

	return m_strTemp;
}

/**********************************************************************/

const char* CCustomFieldValueCSVArray::GetName ( int nIdx )
{
	m_strTemp = "";
	
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
	{
		CCustomFieldValueCSVRecord Value;
		GetAt ( nIdx, Value );
		m_strTemp = Value.GetName();
	}
	
	return m_strTemp;
}

/**********************************************************************/

void CCustomFieldValueCSVArray::AddToNumberMap( CCustomFieldValueNumberInfo& info )
{
	int nStart = 0;
	int nEnd = m_arrayNumberMap.GetSize() - 1;

	while( nStart <= nEnd )
	{
		int nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayNumberMap[ nPos ].m_nNumber > info.m_nNumber )
			nEnd = nPos - 1;
		else if ( m_arrayNumberMap[ nPos ].m_nNumber < info.m_nNumber )
			nStart = nPos + 1;
		else
			return;
	}

	m_arrayNumberMap.InsertAt( nStart, info );
}

/**********************************************************************/

void CCustomFieldValueCSVArray::RemoveFromNumberMap( CCustomFieldValueNumberInfo& info )
{
	int nStart = 0;
	int nEnd = m_arrayNumberMap.GetSize() - 1;

	while( nStart <= nEnd )
	{
		int nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayNumberMap[ nPos ].m_nNumber > info.m_nNumber )
			nEnd = nPos - 1;
		else if ( m_arrayNumberMap[ nPos ].m_nNumber < info.m_nNumber )
			nStart = nPos + 1;
		else
		{
			m_arrayNumberMap.RemoveAt( nPos );
			return;
		}
	}
}

/**********************************************************************/

bool CCustomFieldValueCSVArray::FindValueByNumber( int nNo, int& nIdx )
{
	int nStart = 0;
	int nEnd = m_arrayNumberMap.GetSize() - 1;

	while( nStart <= nEnd )
	{
		int nPos = ( nStart + nEnd ) / 2;

		if ( m_arrayNumberMap[ nPos ].m_nNumber > nNo )
			nEnd = nPos - 1;
		else if ( m_arrayNumberMap[ nPos ].m_nNumber < nNo )
			nStart = nPos + 1;
		else
		{
			nIdx = m_arrayNumberMap[ nPos ].m_nIdx;
			return TRUE;
		}
	}

	nIdx = -1;
	return FALSE;
}

/**********************************************************************/

bool CCustomFieldValueCSVArray::FindValueByKey( const char* szKey, int& nIdx )
{
	CString strKeyToFind = szKey;

	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIdx = ( nStart + nEnd ) / 2;
		CString strArrayKey = GetKey ( nIdx );
		
		if ( strArrayKey < strKeyToFind )
			nStart = nIdx + 1;

		else if ( strArrayKey > strKeyToFind )
			nEnd = nIdx - 1;

		else
			return TRUE;
	}
		
	nIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CCustomFieldValueCSVArray::AddValue( CCustomFieldValueCSVRecord& Value )
{
	CString strKey = Value.GetKey();
	int nNumber = Value.GetNumber();

	//ERROR IF VALUE NUMBER IS ALREADY IN USE
	int nIdx;
	FindValueByNumber( nNumber, nIdx );
	if ( nIdx != -1 )
		return -1;

	int nKeyIndex;
	if ( FindValueByKey ( strKey, nKeyIndex ) == FALSE )
	{
		InsertAt ( nKeyIndex, Value );
		
		CCustomFieldValueNumberInfo info;
		info.m_nNumber = nNumber;
		info.m_nIdx = nKeyIndex;
		AddToNumberMap( info );

		m_nNextNumber = Value.GetNumber() + 2;
		
		return ( nKeyIndex );
	}
	
	return -1;
}

/**********************************************************************/

void CCustomFieldValueCSVArray::DeleteValue ( int nIdx )
{
	if ( ( nIdx >= 0 ) && ( nIdx < GetSize() ) )
	{
		CCustomFieldValueNumberInfo info;
		info.m_nNumber = GetNumber( nIdx );
		RemoveFromNumberMap( info );
		RemoveAt ( nIdx );	
	}
}

/**********************************************************************/

int CCustomFieldValueCSVArray::FindFreeNumber()
{
	int nNumbersTried = 0;
	int nNumberToTry = m_nNextNumber;

	if ( ( nNumberToTry % 2 ) == 0 )
	{
		if ( FALSE == m_bGlobalMode )
			nNumberToTry++;
	}
	else
	{
		if ( TRUE == m_bGlobalMode )
			nNumberToTry++;
	}
	
	if ( nNumberToTry > CustomFieldValue::Number.Max )
		nNumberToTry = ( TRUE == m_bGlobalMode ) ? 2 : 1;

	do
	{
		int nIdx;
		if ( FindValueByNumber( nNumberToTry, nIdx ) == FALSE )
			return nNumberToTry;

		nNumberToTry += 2;
		if ( nNumberToTry > CustomFieldValue::Number.Max )
			nNumberToTry = ( TRUE == m_bGlobalMode ) ? 2 : 1;

		nNumbersTried++;
	}
	while ( nNumbersTried < 1000 );

	return 0;
}

/**********************************************************************/

int CCustomFieldValueCSVArray::ImportLine ( CSSImportFile& import, const char* szImportLine )
{
	CCSV csvImport ( szImportLine );
	CString strKey = csvImport.GetString(0);
	::TrimSpaces ( strKey, FALSE );

	if ( strKey == "" )	
		return IMPORT_LINE_IGNORED;

	int nIdx;
	if ( FindValueByKey ( strKey, nIdx ) == TRUE )	
	{
		CCustomFieldValueCSVRecord NewValue;
		GetAt ( nIdx, NewValue );
		NewValue.ImportLine ( import, csvImport );
		SetAt ( nIdx, NewValue );
		return IMPORT_LINE_MODIFIED;
	}	
	else			
	{	
		CCustomFieldValueCSVRecord NewValue;
		NewValue.SetNumber ( FindFreeNumber() );

		if ( NewValue.GetNumber() <= 0 )
			return IMPORT_LINE_IGNORED;
		else
		{
			NewValue.ImportLine ( import, csvImport );
			AddValue ( NewValue );
			return IMPORT_LINE_ADDED;
		}
	}
}

/**********************************************************************/

bool CCustomFieldValueCSVArray::ValidateFieldValues()
{
	m_bIsValid = TRUE;
	m_nErrorLine = 0;
	m_nErrorType = 0;

	m_arrayNumberMap.RemoveAll();

	if ( GetSize() > MAX_CUSTOM_FIELD_VALUES )
	{
		m_bIsValid = FALSE;
		m_nErrorType = 1;
		return FALSE;
	}

	CString strLastKey = "";
	for ( int nIdx = 0; nIdx < GetSize(); nIdx++ )
	{
		CCustomFieldValueCSVRecord Value;
		GetAt ( nIdx, Value );
		CString strKey = Value.GetKey();
		int nNumber = Value.GetNumber();

		if ( strKey <= strLastKey )
		{
			m_bIsValid = FALSE;
			m_nErrorLine = nIdx;
			m_nErrorType = 2;
			return FALSE;
		}

		strLastKey = strKey;

		if ( ( nNumber < CustomFieldValue::Number.Min ) || ( nNumber > CustomFieldValue::Number.Max ) )
		{
			m_bIsValid = FALSE;
			m_nErrorLine = nIdx;
			m_nErrorType = 3;
			return FALSE;
		}

		int nDummy;
		if ( FindValueByNumber( nNumber, nDummy ) == TRUE )
		{
			m_bIsValid = FALSE;
			m_nErrorLine = nIdx;
			m_nErrorType = 4;
			return FALSE;
		}

		CCustomFieldValueNumberInfo info;
		info.m_nNumber = nNumber;
		info.m_nIdx = nIdx;
		AddToNumberMap( info );
	}

	return TRUE;
}

/**********************************************************************/

void CCustomFieldValueCSVArray::CopyArray ( CCustomFieldValueCSVArray& source )
{
	RemoveAll();
	for ( int nIdx = 0; nIdx < source.GetSize(); nIdx++ )
	{
		CCustomFieldValueCSVRecord Value;
		source.GetAt ( nIdx, Value );
		Add ( Value );
	}
	SetNextNumber( source.GetNextNumber() );
	ValidateFieldValues();
}

/**********************************************************************/
