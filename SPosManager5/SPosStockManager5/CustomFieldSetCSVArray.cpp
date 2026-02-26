/**********************************************************************/
#include "..\CommonStockTray\CustomField.h"
/**********************************************************************/
#include "CustomFieldSetCSVArray.h"
/**********************************************************************/

CCustomFieldSetCSVRecord::CCustomFieldSetCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CCustomFieldSetCSVRecord::ClearRecord()
{
	m_nNumber = 1;
	m_strName = "";
	m_bIncludeNA = FALSE;
}

/**********************************************************************/

void CCustomFieldSetCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	ClearRecord();

	m_nNumber = csv.GetInt(0);
	m_strName = csv.GetString (1);
	
	switch ( csv.GetInt(2) )
	{
	case 1:	V1ConvertFromCSV ( csv );	break;
	}
}

/**********************************************************************/

void CCustomFieldSetCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	m_bIncludeNA = csv.GetBool(3);
}

/**********************************************************************/
	
void CCustomFieldSetCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_nNumber );
	csv.Add ( m_strName );
	csv.Add ( CUSTOM_FIELD_SET_VERSION );		
	csv.Add ( m_bIncludeNA );
}

/**********************************************************************/

void CCustomFieldSetCSVRecord::SetNumber( int n )
{
	if ( ( n >= 1 ) && ( n <= MAX_CUSTOM_FIELD_SET ) )
		m_nNumber = n;
}

/**********************************************************************/

CCustomFieldSetCSVArray::CCustomFieldSetCSVArray() : CSharedCSVArray()
{
	m_nFieldNo = 1;
}

/**********************************************************************/

void CCustomFieldSetCSVArray::SetFieldNo( int n )
{
	if ( ( n >= 1 ) && ( n <= MAX_ITEM_CUSTOM_FIELDS ) )
		m_nFieldNo = n;
}

/**********************************************************************/

int CCustomFieldSetCSVArray::GetNumber ( int nIndex )
{
	int nNumber = 0;
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nNumber = csv.GetInt ( 0 );
	}

	return nNumber;
}

/**********************************************************************/

const char* CCustomFieldSetCSVArray::GetName ( int nIndex )
{
	m_strName = "";
	CString strTemp = GetAt ( nIndex );
	
	if ( strTemp != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		m_strName = csv.GetString ( 1 );
	}

	return m_strName;
}

/**********************************************************************/

void CCustomFieldSetCSVArray::InitialiseInUseFlags()
{
	for ( int n = 0; n < MAX_CUSTOM_FIELD_SET; n++ )
		m_bInUseFlag[n] = FALSE;

	for ( int x = GetSize() - 1; x >= 0; x-- )
	{
		int nNumber = GetNumber(x);

		if ( ( nNumber < 1 ) || ( nNumber > MAX_CUSTOM_FIELD_SET ) )
			RemoveAt(x);
		else if ( m_bInUseFlag[ nNumber - 1 ] == TRUE )
			RemoveAt(x);
		else
			m_bInUseFlag[ nNumber - 1 ] = TRUE;
	}
}

/**********************************************************************/

void CCustomFieldSetCSVArray::SetInUseFlag( int n, bool bFlag )
{
	if ( ( n >= 1 ) && ( n <= MAX_CUSTOM_FIELD_SET ) )
		m_bInUseFlag[ n - 1 ] = bFlag;
}

/**********************************************************************/

int CCustomFieldSetCSVArray::GetNextFreeNumber()
{
	for ( int n = 0; n < MAX_CUSTOM_FIELD_SET; n++ )
		if ( m_bInUseFlag[n] == FALSE )
			return n + 1;

	return 0;
}

/**********************************************************************/

void CCustomFieldSetCSVArray::CopyArray( CCustomFieldSetCSVArray& source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < source.GetSize(); n++ )
	{
		CCustomFieldSetCSVRecord record;
		source.GetAt( n, record );
		Add( record );
	}
}

/**********************************************************************/
