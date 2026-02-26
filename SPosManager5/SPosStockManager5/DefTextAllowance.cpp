/**********************************************************************/
#include "DefTextAllowance.h"
/**********************************************************************/
static const bool DEF_ALLOWANCE_ENABLE = FALSE;
static const int DEF_ALLOWANCE_TAXBAND = 0;
/**********************************************************************/

int CDefTextAllowance::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for ( int i = GetSize(); i < MAX_ALLOWANCES; i++ )
		{
			CCSV csv;
			csv.Add ( GetDefaultText ( i + 1 ) );
			csv.Add ( DEF_ALLOWANCE_TAXBAND );
			csv.Add ( DEF_ALLOWANCE_ENABLE );
			Add ( csv.GetLine() );
		}
	
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;

		if ( GetSize() > MAX_ALLOWANCES )
			RemoveAt( MAX_ALLOWANCES, GetSize() - MAX_ALLOWANCES );
	}

	return nReply;
}

/**********************************************************************/

int CDefTextAllowance::GetEnableCount()
{
	int nResult = 0;

	for ( int nAllowance = 1; nAllowance <= MAX_ALLOWANCES; nAllowance++ )
		if ( GetReportEnable ( nAllowance ) == TRUE )
			nResult++;

	return nResult;
}

/**********************************************************************/

const char* CDefTextAllowance::GetDefaultText ( int nAllowance )
{
	int nIndex = nAllowance - 1;

	if ( ( nIndex >= 0 ) && ( nIndex < MAX_ALLOWANCES ) )
	{
		m_strDefText.Format ( "Allowance %d", nAllowance );
		return m_strDefText;
	}

	return "Unknown";
}

/**********************************************************************/

const char* CDefTextAllowance::GetText ( int nAllowance )
{
	int nIndex = nAllowance - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		m_strText = csv.GetString ( 0 );
		return m_strText;
	}

	return "Unknown";
}

/**********************************************************************/

int CDefTextAllowance::GetTaxBand( int nAllowance )
{
	int nIndex = nAllowance - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		return csv.GetInt ( 1 );
	}

	return 0;
}

/**********************************************************************/

bool CDefTextAllowance::GetReportEnable( int nAllowance )
{
	int nIndex = nAllowance - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CCSV csv ( GetAt ( nIndex ) );
		return csv.GetBool ( 2 );
	}

	return FALSE;
}

/**********************************************************************/

void CDefTextAllowance::SetAllowance ( int nAllowance, const char* szText, int nTaxBand, bool bEnable )
{
	int nIndex = nAllowance - 1;

	if ( ( nIndex >= 0 ) &&  ( nIndex < GetSize() ) )
	{
		CString strText = szText;
		::TrimSpaces ( strText, FALSE );
		
		if ( strText == "" )
			strText = GetDefaultText ( nAllowance );

		CCSV csv;
		csv.Add ( strText );
		csv.Add ( nTaxBand );
		csv.Add ( bEnable );
		SetAt ( nIndex, csv.GetLine() );
	}
}

/**********************************************************************/
