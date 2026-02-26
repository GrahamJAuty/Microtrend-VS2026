/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/
#include "EposSelectArrayFeeder.h"
/**********************************************************************/

CEposSelectArrayFeeder::CEposSelectArrayFeeder(void)
{
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
	{
		CEposSelectArrayFeederLocInfo info;
		info.m_strOriginal = "";
		info.m_strSelected = "";
		m_arrayLocations.Add( info );
	}

	for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
	{
		CEposSelectArrayFeederDbInfo info;
		info.m_bImport = FALSE;
		info.m_bManual = FALSE;
		m_arrayDatabases.Add( info );
	}

	m_bIsEmpty = TRUE;
}

/**********************************************************************/

bool CEposSelectArrayFeeder::ValidateLocIdx( int nLocIdx )
{
	return ( ( nLocIdx >= 0 ) && ( nLocIdx < m_arrayLocations.GetSize() ) && ( nLocIdx < dbLocation.GetSize() ) );
}

/**********************************************************************/

bool CEposSelectArrayFeeder::ValidateDbIdx( int nDbIdx )
{
	return ( ( nDbIdx >= 0 ) && ( nDbIdx < m_arrayDatabases.GetSize() ) && ( nDbIdx < dbDatabase.GetSize() ) );
}

/**********************************************************************/

void CEposSelectArrayFeeder::RemoveAll()
{
	for ( int n = 0; n < m_arrayLocations.GetSize(); n++ )
	{
		m_arrayLocations[n].m_strOriginal = "";
		m_arrayLocations[n].m_strSelected = "";
	}

#ifdef STOCKMAN_SYSTEM
	for ( int n = 0; n < m_arrayDatabases.GetSize(); n++ )
	{
		m_arrayDatabases[n].m_bImport = FALSE;
		m_arrayDatabases[n].m_bManual = FALSE;
	}
#endif

	m_bIsEmpty = TRUE;
}

/**********************************************************************/

void CEposSelectArrayFeeder::SelectTerminal( int nLocIdx, int nTermIdx )
{
	if ( ValidateLocIdx( nLocIdx ) == TRUE )
	{
		if ( ( nTermIdx >= 0 ) && ( nTermIdx < dbLocation.GetTerminalCount( nLocIdx ) ) )
		{
			::AddTrailing( m_arrayLocations[nLocIdx].m_strSelected, nTermIdx + 1, '0' );
			m_arrayLocations[nLocIdx].m_strSelected.SetAt( nTermIdx, '1' );
			m_bIsEmpty = FALSE;
		}
	}
}

/**********************************************************************/

void CEposSelectArrayFeeder::SelectLocation( int nLocIdx )
{
	if ( ValidateLocIdx( nLocIdx ) == TRUE )
	{
		m_arrayLocations[nLocIdx].m_strSelected = "";
		::AddTrailing( m_arrayLocations[nLocIdx].m_strSelected, dbLocation.GetTerminalCount(nLocIdx), '1' );
			
		if ( m_arrayLocations[nLocIdx].m_strSelected != "" )
		{
			m_bIsEmpty = FALSE;
		}
	}
}

/**********************************************************************/

void CEposSelectArrayFeeder::SelectDatabase( int nDbIdx )
{
	CArray<int,int> Set;
	dbLocation.GetDatabaseLocIdxSet( nDbIdx, Set );

	for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
		SelectLocation ( Set.GetAt( nIndex ) );
}

/**********************************************************************/

void CEposSelectArrayFeeder::SelectSystem()
{
	for ( int nLocIdx = 0; nLocIdx < dbLocation.GetSize(); nLocIdx++ )
		SelectLocation( nLocIdx );
}

/**********************************************************************/

void CEposSelectArrayFeeder::SelectDatabaseForNonEposSales( int nDbIdx, bool bManual, bool bImport )
{
	if ( ValidateDbIdx( nDbIdx ) == TRUE )
	{
		if ( TRUE == bManual )
		{
			m_arrayDatabases[nDbIdx].m_bManual = TRUE;
			m_bIsEmpty = FALSE;
		}

		if ( TRUE == bImport )
		{
			m_arrayDatabases[nDbIdx].m_bImport = TRUE;
			m_bIsEmpty = FALSE;
		}
	}
}

/**********************************************************************/

bool CEposSelectArrayFeeder::GetSelectionStatus( int nLocIdx, int nTermIdx )
{
	bool bResult = FALSE;

	if ( ValidateLocIdx( nLocIdx ) == TRUE )
	{
		CString str = m_arrayLocations[nLocIdx].m_strSelected;	

		if ( ( nTermIdx >= 0 ) && ( nTermIdx < str.GetLength() ) )
			bResult = ( str.Mid( nTermIdx, 1 ) == '1' );
	}

	return bResult;
}

/**********************************************************************/

void CEposSelectArrayFeeder::TagOriginalSelectionStatus( bool bSelectAllTerminals )
{
	for ( int nLocIdx = 0; nLocIdx < GetLocCount(); nLocIdx++ )
	{
		if ( GotTerminals( nLocIdx ) == TRUE )
		{
			m_arrayLocations[nLocIdx].m_strOriginal = m_arrayLocations[nLocIdx].m_strSelected; 

			if ( TRUE == bSelectAllTerminals )
				SelectLocation( nLocIdx );
		}
		else
		{
			m_arrayLocations[nLocIdx].m_strOriginal = "";
		}
	}
}

/**********************************************************************/

bool CEposSelectArrayFeeder::GetOriginalSelectionStatus( int nLocIdx, int nTermIdx )
{
	bool bResult = FALSE;

	if ( ValidateLocIdx( nLocIdx ) == TRUE )
	{
		CString str = m_arrayLocations[nLocIdx].m_strOriginal;	

		if ( ( nTermIdx >= 0 ) && ( nTermIdx < str.GetLength() ) )
			bResult = ( str.Mid( nTermIdx, 1 ) == '1' );
	}

	return bResult;
}

/**********************************************************************/

void CEposSelectArrayFeeder::GetTermIdxArray( int nLocIdx, CArray<int,int>& arrayTermIdx )
{
	arrayTermIdx.RemoveAll();

	if ( ValidateLocIdx( nLocIdx ) == TRUE )
	{
		CString strTerminals = m_arrayLocations[nLocIdx].m_strSelected;
		int nCount = dbLocation.GetTerminalCount( nLocIdx );

		for ( int nTermIdx = 0; ( nTermIdx < strTerminals.GetLength() ) && ( nTermIdx < nCount ); nTermIdx++ )
		{
			if ( strTerminals.GetAt(nTermIdx) == '1' )
			{
				arrayTermIdx.Add(nTermIdx);
			}
		}
	}
}

/**********************************************************************/

bool CEposSelectArrayFeeder::GotTerminals( int nLocIdx )
{
	bool bResult = FALSE;

	if ( ValidateLocIdx( nLocIdx ) == TRUE )
	{
		bResult = ( m_arrayLocations[nLocIdx].m_strSelected.Find( "1", 0 ) >= 0 );
	}

	return bResult;
}

/**********************************************************************/

bool CEposSelectArrayFeeder::GetDbImportFlag( int nDbIdx )
{
	if ( ValidateDbIdx( nDbIdx ) == TRUE )
		return m_arrayDatabases[nDbIdx].m_bImport;
	else
		return FALSE;
}

/**********************************************************************/

bool CEposSelectArrayFeeder::GetDbManualFlag( int nDbIdx )
{
	if ( ValidateDbIdx( nDbIdx ) == TRUE )
		return m_arrayDatabases[nDbIdx].m_bManual;
	else
		return FALSE;
}

/**********************************************************************/

const char* CEposSelectArrayFeeder::GetLocationString( int nLocIdx )
{
	m_strLocation = "";
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < m_arrayLocations.GetSize() ) )
	{
		m_strLocation = m_arrayLocations[nLocIdx].m_strSelected;
	}
	return m_strLocation;
}

/**********************************************************************/

bool CEposSelectArrayFeeder::SetLocationString( int nLocIdx, const char* sz )
{
	bool bResult = FALSE;
	if ( ( nLocIdx >= 0 ) && ( nLocIdx < m_arrayLocations.GetSize() ) )
	{
		CString str = sz;
		bResult = ( str.Find( '1', 0 ) >= 0 );
		m_arrayLocations[nLocIdx].m_strSelected = str;
	}
	return bResult;
}

/**********************************************************************/
