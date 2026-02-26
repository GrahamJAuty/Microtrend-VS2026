/**********************************************************************/
#include "DefTextCategory.h"
/**********************************************************************/
static const bool DEF_CATEGORY_ENABLE = TRUE;
static const int DEF_CATEGORY_TAXBAND = 0;
/**********************************************************************/

int CDefTextCategory::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for ( int i = GetSize(); i < MAX_CATEGORY; i++ )
		{
			CCSV csv;
			csv.Add ( GetDefaultText ( i + 1 ) );
			csv.Add ( DEF_CATEGORY_TAXBAND );
			csv.Add ( DEF_CATEGORY_ENABLE );
			Add ( csv.GetLine() );
		}
	
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;

		if ( GetSize() > MAX_CATEGORY )
			RemoveAt( MAX_CATEGORY, GetSize() - MAX_CATEGORY );
	}

	return nReply;
}

/**********************************************************************/

const char* CDefTextCategory::GetDefaultText ( int nCategory )
{
	int nIndex = nCategory - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < MAX_CATEGORY ) )
	{
		m_strDefText.Format ( "Category %d", nCategory );
		return m_strDefText;
	}

	return "Unknown";
}

/**********************************************************************/

const char* CDefTextCategory::GetText ( int nCategory )
{
	int nIndex = nCategory - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strText = csv.GetString ( 0 );
		return m_strText;
	}

	return "Unknown";
}

/**********************************************************************/

int CDefTextCategory::GetTaxBand( int nCategory )
{
	int nIndex = nCategory - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		return csv.GetInt ( 1 );
	}

	return FALSE;
}

/**********************************************************************/

bool CDefTextCategory::GetReportEnable( int nCategory )
{
	int nIndex = nCategory - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		return csv.GetBool ( 2 );
	}

	return FALSE;
}

/**********************************************************************/

void CDefTextCategory::SetCategory ( int nCategory, const char* szText, int nTaxBand, bool bEnable )
{
	int nIndex = nCategory - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CString strText = szText;
		::TrimSpaces ( strText, FALSE );
		
		if ( strText == "" )
			strText = GetDefaultText ( nCategory );

		CCSV csv;
		csv.Add ( strText );
		csv.Add ( nTaxBand );
		csv.Add ( bEnable );
		SetAt ( nIndex, csv.GetLine() );
	}
}

/**********************************************************************/

void CDefTextCategory::CopyFrom( CDefTextCategory& Source )
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < Source.GetSize(); n++ )
		Add( Source.GetAt(n) );
}

/**********************************************************************/
