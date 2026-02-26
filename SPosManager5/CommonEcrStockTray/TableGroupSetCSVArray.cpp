/**********************************************************************/
#include "TableGroupSetCSVArray.h"
/**********************************************************************/

CTableGroupSetCSVRecord::CTableGroupSetCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CTableGroupSetCSVRecord::ClearRecord()
{
	for ( int n = 0; n < 4; n++ )
	{
		m_strGroupSetName[n] = "";
		m_bShowAsFilter[n] = FALSE;
		m_bIncludeUnknown[n] = FALSE;
		m_bIncludeNonTable[n] = FALSE;
		m_nGroupMask[n] = 0;
	}
}

/**********************************************************************/

void CTableGroupSetCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	}
}

/**********************************************************************/

void CTableGroupSetCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int nPos= 1;

	for ( int n = 1; n <= 3; n++ )
	{
		SetGroupSetName			( n, csv.GetString(nPos++) );
		SetShowAsFilterFlag		( n, csv.GetBool(nPos++) );
		SetIncludeUnknownFlag	( n, csv.GetBool(nPos++) );
		SetIncludeNonTableFlag	( n, csv.GetBool(nPos++) );
		SetGroupMask			( n, csv.GetInt(nPos++) );
	}
}

/**********************************************************************/

void CTableGroupSetCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( TABLEGROUPSET_VERSION );

	for ( int n = 1; n <= 3; n++ )
	{
		csv.Add ( m_strGroupSetName[n] );
		csv.Add ( m_bShowAsFilter[n] );
		csv.Add ( m_bIncludeUnknown[n] );
		csv.Add ( m_bIncludeNonTable[n] );
		csv.Add ( m_nGroupMask[n] );
	}
}

/**********************************************************************/

const char* CTableGroupSetCSVRecord::GetGroupSetName( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_strGroupSetName[n];
	else
		return "";
}

/**********************************************************************/

bool CTableGroupSetCSVRecord::GetShowAsFilterFlag( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_bShowAsFilter[n];
	else
		return FALSE;
}

/**********************************************************************/

bool CTableGroupSetCSVRecord::GetIncludeUnknownFlag( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_bIncludeUnknown[n];
	else
		return FALSE;
}

/**********************************************************************/

bool CTableGroupSetCSVRecord::GetIncludeNonTableFlag( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_bIncludeNonTable[n];
	else
		return FALSE;
}

/**********************************************************************/

int CTableGroupSetCSVRecord::GetGroupMask( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_nGroupMask[n];
	else
		return 0;
}

/**********************************************************************/

void CTableGroupSetCSVRecord::SetGroupSetName( int n, const char* sz )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		Set( m_strGroupSetName[n], sz, SPosTableGroupSet::GroupSetName );
}

/**********************************************************************/

void CTableGroupSetCSVRecord::SetShowAsFilterFlag( int n, bool b )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		m_bShowAsFilter[n] = b;
}

/**********************************************************************/

void CTableGroupSetCSVRecord::SetIncludeUnknownFlag( int n, bool b )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		m_bIncludeUnknown[n] = b;
}

/**********************************************************************/

void CTableGroupSetCSVRecord::SetIncludeNonTableFlag( int n, bool b )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		m_bIncludeNonTable[n] = b;
}

/**********************************************************************/

void CTableGroupSetCSVRecord::SetGroupMask( int n, int nMask )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		Set( m_nGroupMask[n], nMask, SPosTableGroupSet::GroupMask );
}

/**********************************************************************/

bool CTableGroupSetCSVRecord::IsEmpty( int nLevel )
{
	if ( GetIncludeUnknownFlag(nLevel) == TRUE )
		return FALSE;

	if ( GetIncludeNonTableFlag(nLevel) == TRUE )
		return FALSE;

	if ( GetGroupMask(nLevel) != 0 )
		return FALSE;

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTableGroupSetCSVArray::CTableGroupSetCSVArray() 
{
}

/**********************************************************************/

CTableGroupSetCSVArray::~CTableGroupSetCSVArray() 
{
}

/**********************************************************************/

int CTableGroupSetCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int nSize = GetSize();
	while ( nSize < MAX_TABLEGROUPSET)
	{
		CTableGroupSetCSVRecord GroupSet;
		
		for ( int n = 1; n <= 3; n++ )
		{
			CString strName;
			strName.Format( "Table Group Set %d.%d", n, nSize + 1 );
			GroupSet.SetGroupSetName( n, strName );
			GroupSet.SetShowAsFilterFlag( n, FALSE );
			GroupSet.SetGroupMask( n, 0 );
		}

		Add( GroupSet );
		nSize++;
	}

	if ( GetSize() > MAX_TABLEGROUPSET )
	{
		RemoveAt( MAX_TABLEGROUPSET, GetSize() - MAX_TABLEGROUPSET );
	}
	
	return nReply;
}

/**********************************************************************/
