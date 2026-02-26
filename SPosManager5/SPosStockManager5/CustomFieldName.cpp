/**********************************************************************/
#include "..\CommonStockTray\CustomField.h"
/**********************************************************************/

int CCustomFieldNameArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for ( int nFieldNo = GetSize() + 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
			Add ( GetDefaultName ( nFieldNo ) );
	
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;

		if ( GetSize() > MAX_ITEM_CUSTOM_FIELDS )
			RemoveAt( MAX_ITEM_CUSTOM_FIELDS, GetSize() - MAX_ITEM_CUSTOM_FIELDS );
	}

	return nReply;
}

/**********************************************************************/

const char* CCustomFieldNameArray::GetDefaultName ( int nFieldNo )
{
	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= MAX_ITEM_CUSTOM_FIELDS ) )
		m_strDefName.Format( "Custom Field %d", nFieldNo );
	else
		m_strDefName = "Unknown";

	return m_strDefName;
}

/**********************************************************************/

const char* CCustomFieldNameArray::GetName ( int nFieldNo )
{
	int nIndex = nFieldNo - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strName = csv.GetString ( 0 );
		
		::TrimSpaces( m_strName, FALSE );
		if ( m_strName == "" )
			m_strName = GetDefaultName( nFieldNo );

	}
	else
		m_strName = "Unknown";

	return m_strName;
}

/**********************************************************************/

bool CCustomFieldNameArray::CheckName ( int nFieldNo )
{
	bool bResult = FALSE;

	int nIndex = nFieldNo - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strName = csv.GetString ( 0 );
		
		::TrimSpaces( m_strName, FALSE );
		if ( ( m_strName != "" ) && ( m_strName != GetDefaultName( nFieldNo ) ) )
			bResult = TRUE;

	}
	
	return bResult;
}

/**********************************************************************/

void CCustomFieldNameArray::SetName ( int nFieldNo, const char* szName )
{
	int nIndex = nFieldNo - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < GetSize() ) )
	{
		CString strName = szName;
		::TrimSpaces ( strName, FALSE );
		
		if ( strName == "" )
			strName = GetDefaultName ( nFieldNo );

		SetAt ( nIndex, strName );
	}
}

/**********************************************************************/
