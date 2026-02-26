/**********************************************************************/
 
/**********************************************************************/
#include "ServerCSVArray.h"
/**********************************************************************/
#include "ServerPresetArray.h"
/**********************************************************************/

CServerPresetInfo::CServerPresetInfo()
{
	Reset();
}

/**********************************************************************/

void CServerPresetInfo::Reset()
{
	m_nPresetNo = 0;
	m_nServerNoSystem = 0;
	m_nServerNoDatabase = 0;
	m_nServerNoLocation = 0;
	m_nServerNoTerminal = 0;
}

/**********************************************************************/

void CServerPresetInfo::SetPresetNo( int n )
{
	if ( ( n >= 1 ) && ( n <= MAX_SVRPRESETNO ) )
		m_nPresetNo = n;
}

/**********************************************************************/

void CServerPresetInfo::SetServerNoSystem( int n )
{
	if ( ( n >= 0 ) && ( n <= Server::ServerNo.Max ) )
		m_nServerNoSystem = n;
}

/**********************************************************************/

void CServerPresetInfo::SetServerNoDatabase( int n )
{
	if ( ( n >= -1 ) && ( n <= Server::ServerNo.Max ) )
		m_nServerNoDatabase = n;
}

/**********************************************************************/

void CServerPresetInfo::SetServerNoLocation( int n )
{
	if ( ( n >= -1 ) && ( n <= Server::ServerNo.Max ) )
		m_nServerNoLocation = n;
}

/**********************************************************************/

void CServerPresetInfo::SetServerNoTerminal( int n )
{
	if ( ( n >= -1 ) && ( n <= Server::ServerNo.Max ) )
		m_nServerNoTerminal = n;
}

/**********************************************************************/

void CServerPresetInfo::Add (CServerPresetInfo& source )
{
}

/**********************************************************************/

int CServerPresetInfo::Compare( CServerPresetInfo& source, int nHint )
{
	if ( m_nPresetNo != source.m_nPresetNo )
		return ( ( m_nPresetNo > source.m_nPresetNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CServerPresetTable::CServerPresetTable(void)
{
	for ( int n = 1; n <= MAX_SVRPRESETNO; n++ )
		AddPreset(n);
}

/**********************************************************************/

void CServerPresetTable::AddPreset( int nPresetNo )
{
	CServerPresetInfo infoPreset;
	infoPreset.SetPresetNo( nPresetNo );
	infoPreset.SetServerNoSystem(0);
	infoPreset.SetServerNoDatabase(-1);
	infoPreset.SetServerNoLocation(-1);
	infoPreset.SetServerNoTerminal(-1);
	m_arrayPresets.DirectAdd( infoPreset );
}

/**********************************************************************/

void CServerPresetTable::ResetPresetList( int nPresetMode )
{
	switch( nPresetMode )
	{
	case SVRPRESETFILE_SYSTEM:
		for ( int n = 0; n < GetSize(); n++ )
		{
			CServerPresetInfo infoPreset;
			m_arrayPresets.GetAt( n, infoPreset );
			infoPreset.SetServerNoSystem( 0 );
			m_arrayPresets.SetAt( n, infoPreset );
		}
		break;

	case SVRPRESETFILE_DATABASE:
		for ( int n = 0; n < GetSize(); n++ )
		{
			CServerPresetInfo infoPreset;
			m_arrayPresets.GetAt( n, infoPreset );
			infoPreset.SetServerNoDatabase( -1 );
			m_arrayPresets.SetAt( n, infoPreset );
		}
		break;

	case SVRPRESETFILE_LOCATION:
		for ( int n = 0; n < GetSize(); n++ )
		{
			CServerPresetInfo infoPreset;
			m_arrayPresets.GetAt( n, infoPreset );
			infoPreset.SetServerNoLocation( -1 );
			m_arrayPresets.SetAt( n, infoPreset );
		}
		break;

	case SVRPRESETFILE_TERMINAL:
		for ( int n = 0; n < GetSize(); n++ )
		{
			CServerPresetInfo infoPreset;
			m_arrayPresets.GetAt( n, infoPreset );
			infoPreset.SetServerNoTerminal( -1 );
			m_arrayPresets.SetAt( n, infoPreset );
		}
		break;
	}
}

/**********************************************************************/

bool CServerPresetTable::GetPreset( CServerPresetMiniInfo& infoMini )
{
	bool bResult = FALSE;

	CServerPresetInfo infoPreset;
	infoPreset.SetPresetNo( infoMini.m_nPresetNo );

	int nPresetIdx;
	if ( m_arrayPresets.Find( infoPreset, nPresetIdx ) == TRUE )
		bResult = GetPreset( nPresetIdx, infoMini );

	return bResult;
}

/**********************************************************************/

bool CServerPresetTable::GetPreset( int nPresetIdx, CServerPresetMiniInfo& infoMini )
{
	bool bResult = FALSE;

	if ( ( nPresetIdx >= 0 ) && ( nPresetIdx < m_arrayPresets.GetSize() ) )
	{
		CServerPresetInfo infoPreset;
		m_arrayPresets.GetAt( nPresetIdx, infoPreset );

		infoMini.m_nPresetNo = infoPreset.GetPresetNo();
			
		switch( infoMini.m_nType )
		{
		case SVRPRESETFILE_SYSTEM:
			infoMini.m_nServerNo = infoPreset.GetServerNoSystem();
			bResult = TRUE;
			break;

		case SVRPRESETFILE_DATABASE:
			infoMini.m_nServerNo = infoPreset.GetServerNoDatabase();
			bResult = TRUE;
			break;

		case SVRPRESETFILE_LOCATION:
			infoMini.m_nServerNo = infoPreset.GetServerNoLocation();
			bResult = TRUE;
			break;

		case SVRPRESETFILE_TERMINAL:
			infoMini.m_nServerNo = infoPreset.GetServerNoTerminal();
			bResult = TRUE;
			break;
		}
	}
	return bResult;
}

/**********************************************************************/

void CServerPresetTable::UpdatePreset( CServerPresetMiniInfo& infoMini )
{
	CServerPresetInfo infoPreset;
	infoPreset.SetPresetNo( infoMini.m_nPresetNo );

	int nPresetIdx;
	if ( m_arrayPresets.Find( infoPreset, nPresetIdx ) == TRUE )
		UpdatePreset( nPresetIdx, infoMini );
}

/**********************************************************************/

void CServerPresetTable::UpdatePreset( int nPresetIdx, CServerPresetMiniInfo& infoMini )
{
	if ( ( nPresetIdx >= 0 ) && ( nPresetIdx < m_arrayPresets.GetSize() ) )
	{
		CServerPresetInfo infoPreset;
		m_arrayPresets.GetAt( nPresetIdx, infoPreset );

		if ( infoPreset.GetPresetNo() == infoMini.m_nPresetNo )
		{
			switch( infoMini.m_nType )
			{
			case SVRPRESETFILE_SYSTEM:
				infoPreset.SetServerNoSystem( infoMini.m_nServerNo );
				break;

			case SVRPRESETFILE_DATABASE:
				infoPreset.SetServerNoDatabase( infoMini.m_nServerNo );
				break;

			case SVRPRESETFILE_LOCATION:
				infoPreset.SetServerNoLocation( infoMini.m_nServerNo );
				break;

			case SVRPRESETFILE_TERMINAL:
				infoPreset.SetServerNoTerminal( infoMini.m_nServerNo );
				break;
			}

			m_arrayPresets.SetAt( nPresetIdx, infoPreset );
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CServerPresetArray::CServerPresetArray( int nPresetMode, CServerPresetTable& PresetTable ) : CSharedStringArray(), m_PresetTable( PresetTable )
{
	m_nPresetMode = nPresetMode;
}

/**********************************************************************/

int CServerPresetArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );
	
	m_PresetTable.ResetPresetList( m_nPresetMode );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for( int n = 0; n < GetSize(); n++ )
		{
			CCSV csv( GetAt(n) );

			if ( csv.GetInt(0) == 1 )
			{
				CServerPresetMiniInfo infoMini;
				infoMini.m_nPresetNo = csv.GetInt(1);
				infoMini.m_nType = m_nPresetMode;
				infoMini.m_nServerNo = csv.GetInt(2);
				m_PresetTable.UpdatePreset( infoMini );
			}
		}
	}

	RemoveAt( 0, GetSize() );
	return nReply;
}

/**********************************************************************/

void CServerPresetArray::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int nIdx = 0; nIdx < m_PresetTable.GetSize(); nIdx++ )
	{
		CServerPresetMiniInfo infoMini;
		infoMini.m_nType = m_nPresetMode;
		m_PresetTable.GetPreset( nIdx, infoMini );

		CCSV csv;
		csv.Add(1);
		csv.Add(infoMini.m_nPresetNo );
		csv.Add(infoMini.m_nServerNo );
		Add( csv.GetLine() );
	}
}

/**********************************************************************/

void CServerPresetArray::UpdatePreset( CServerPresetMiniInfo& infoMini )
{
	infoMini.m_nType = m_nPresetMode;
	m_PresetTable.UpdatePreset( infoMini );
}

/**********************************************************************/

void CServerPresetArray::UpdatePreset( int nPresetIdx, CServerPresetMiniInfo& infoMini )
{
	infoMini.m_nType = m_nPresetMode;
	m_PresetTable.UpdatePreset( nPresetIdx, infoMini );
}

/**********************************************************************/

void CServerPresetArray::GetPreset( CServerPresetMiniInfo& infoMini )
{
	infoMini.m_nType = m_nPresetMode;
	m_PresetTable.GetPreset( infoMini );
}

/**********************************************************************/

void CServerPresetArray::GetPreset( int nPresetIdx, CServerPresetMiniInfo& infoMini )
{
	infoMini.m_nType = m_nPresetMode;
	m_PresetTable.GetPreset( nPresetIdx, infoMini );
}

/**********************************************************************/

void CServerPresetArray::ResetPresetList()
{
	m_PresetTable.ResetPresetList( m_nPresetMode );
}

/**********************************************************************/
