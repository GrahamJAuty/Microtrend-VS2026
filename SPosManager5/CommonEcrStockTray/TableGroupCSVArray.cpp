/**********************************************************************/
#include "TableGroupCSVArray.h"
/**********************************************************************/

CTableGroupCSVRecord::CTableGroupCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CTableGroupCSVRecord::ClearRecord()
{
	for( int n = 0; n < 4; n++ )
	{
		m_strGroupName[n] = "";
		m_bShowAsFilter[n] = FALSE;
	}
}

/**********************************************************************/

void CTableGroupCSVRecord::ConvertFromCSV ( CCSV& csv )
{
	switch ( csv.GetInt(0) )
	{
	case 1:	V1ConvertFromCSV ( csv );		break;
	}
}

/**********************************************************************/

void CTableGroupCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{
	int nPos = 1;
	for ( int n = 1; n <= 3; n++ )
	{
		SetGroupName( n, csv.GetString(nPos++) );
		SetShowAsFilterFlag( n, csv.GetBool(nPos++) );
	}
}

/**********************************************************************/

void CTableGroupCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( TABLEGROUP_VERSION );
	
	for ( int n = 1; n <= 3; n++ )
	{
		csv.Add( m_strGroupName[n] );
		csv.Add( m_bShowAsFilter[n] );
	}
}

/**********************************************************************/

const char* CTableGroupCSVRecord::GetGroupName( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_strGroupName[n];
	else
		return "";
}

/**********************************************************************/

bool CTableGroupCSVRecord::GetShowAsFilterFlag( int n )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
		return m_bShowAsFilter[n];
	else
		return FALSE;
}

/**********************************************************************/

void CTableGroupCSVRecord::SetGroupName( int n, const char* sz )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
	{
		Set ( m_strGroupName[n], sz, SPosTableGroup::GroupName );
	}
}

/**********************************************************************/

void CTableGroupCSVRecord::SetShowAsFilterFlag( int n, bool b )
{
	if ( ( n >= 1 ) && ( n <= 3 ) )
	{
		m_bShowAsFilter[n] = b;
	}
}

/**********************************************************************/

const char* CTableGroupCSVRecord::GetTabbedComboText( int nGroupNo, int n )
{
	m_strTabbedComboText = "";
	if ( ( n >= 1 ) && ( n <= 3 ) )
	{
		m_strTabbedComboText.Format( "G%2.2d,%s",
			nGroupNo,
			(const char*) m_strGroupName[n] );
	}

	return m_strTabbedComboText;
}

/**********************************************************************/

const char* CTableGroupCSVRecord::GetPrintReportText( int nGroupNo, int n )
{
	m_strPrintReportText = "";
	if ( ( n >= 1 ) && ( n <= 3 ) )
	{
		m_strPrintReportText.Format( "%2.2d  %s",
			nGroupNo,
			(const char*) m_strGroupName[n] );
	}

	return m_strPrintReportText;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CTableGroupCSVArray::CTableGroupCSVArray() 
{
}

/**********************************************************************/

CTableGroupCSVArray::~CTableGroupCSVArray() 
{
}

/**********************************************************************/

int CTableGroupCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	int nSize = GetSize();
	while ( nSize < MAX_TABLEGROUP )
	{
		CTableGroupCSVRecord Group;

		for ( int n = 1; n <= 3; n++ )
		{
			CString strName;
			strName.Format( "Table Group %d.%d", n, nSize + 1 );

			Group.SetGroupName( n, strName );
			Group.SetShowAsFilterFlag( n, FALSE );
		}

		Add( Group );

		nSize++;
	}

	if ( GetSize() > MAX_TABLEGROUP )
	{
		RemoveAt( MAX_TABLEGROUP, GetSize() - MAX_TABLEGROUP );
	}
	
	return nReply;
}

/**********************************************************************/
