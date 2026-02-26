/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "OPGroupArray.h"
/**********************************************************************/

COPGroupMiniInfo::COPGroupMiniInfo()
{
	m_nType = 0;
	m_nGroupNo = 0;
	m_strName = "";
	m_bDefer = FALSE;
	m_nFlags = 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

COPGroupInfo::COPGroupInfo()
{
	Reset();
}

/**********************************************************************/

void COPGroupInfo::Reset()
{
	m_nGroupNo = 0;
	m_strName = "";
	m_bDeferDatabase = TRUE;
	m_bDeferLocation = TRUE;
	ResetFlags();
}

/**********************************************************************/

void COPGroupInfo::ResetFlags()
{
	m_nFlagsSystem = 0;
	m_nFlagsDatabase = 0;
	m_nFlagsLocation = 0;
}

/**********************************************************************/

void COPGroupInfo::SetGroupNo( int n )
{
	if ( ( n >= 1 ) && ( n <= 20 ) )
		m_nGroupNo = n;
}

/**********************************************************************/

void COPGroupInfo::SetName( const char* sz )
{
	CString str = sz;
	if ( str.GetLength() <= MAX_LENGTH_OPGROUP_NAME )
		m_strName = str;
}

/**********************************************************************/

void COPGroupInfo::SetEnableSystem( int n, bool b )
{
	if ( ( n >= 0 ) && ( n <= 15 ) )
	{
		int nMask = 1 << n;

		if ( TRUE == b )
			m_nFlagsSystem |= nMask;
		else
			m_nFlagsSystem &= 0xFFFF - nMask;
	}
}

/**********************************************************************/

void COPGroupInfo::SetEnableDatabase( int n, bool b )
{
	if ( ( n >= 0 ) && ( n <= 15 ) )
	{
		int nMask = 1 << n;

		if ( TRUE == b )
			m_nFlagsDatabase |= nMask;
		else
			m_nFlagsDatabase &= 0xFFFF - nMask;
	}
}

/**********************************************************************/

void COPGroupInfo::SetEnableLocation( int n, bool b )
{
	if ( ( n >= 0 ) && ( n <= 15 ) )
	{
		int nMask = 1 << n;

		if ( TRUE == b )
			m_nFlagsLocation |= nMask;
		else
			m_nFlagsLocation &= 0xFFFF - nMask;
	}
}

/**********************************************************************/

bool COPGroupInfo::GetEnableSystem( int n )
{
	if ( ( n >= 0 ) && ( n <= 15 ) )
		return ( ( m_nFlagsSystem & ( 1 << n ) ) != 0 );
	else
		return FALSE;
}

/**********************************************************************/

bool COPGroupInfo::GetEnableDatabase( int n )
{
	if ( ( n >= 0 ) && ( n <= 15 ) )
		return ( ( m_nFlagsDatabase & ( 1 << n ) ) != 0 );
	else
		return FALSE;
}

/**********************************************************************/

bool COPGroupInfo::GetEnableLocation( int n )
{
	if ( ( n >= 0 ) && ( n <= 15 ) )
		return ( ( m_nFlagsLocation & ( 1 << n ) ) != 0 );
	else
		return FALSE;
}

/**********************************************************************/

void COPGroupInfo::Add (COPGroupInfo& source )
{
}

/**********************************************************************/

int COPGroupInfo::Compare( COPGroupInfo& source, int nHint )
{
	if ( m_nGroupNo != source.m_nGroupNo )
		return ( ( m_nGroupNo > source.m_nGroupNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

COPGroupTable::COPGroupTable(void)
{
	for ( int n = 1; n <= MAX_OP_GROUP; n++ )
	{
		CString str;
		str.Format( "OP Group %d", n );
		AddOPGroup( n, str );
	}
}

/**********************************************************************/

void COPGroupTable::AddOPGroup( int nGroupNo, const char* szName )
{
	COPGroupInfo infoOPGroup;
	infoOPGroup.SetGroupNo( nGroupNo );
	infoOPGroup.SetName( szName );
	infoOPGroup.ResetFlags();
	m_arrayOPGroups.DirectAdd( infoOPGroup );
}

/**********************************************************************/

void COPGroupTable::ResetOPGroupList( int nOPGroupMode )
{
	switch( nOPGroupMode )
	{
	case OPGROUP_SYSTEM:
		for ( int n = 0; n < GetSize(); n++ )
		{
			COPGroupInfo infoOPGroup;
			m_arrayOPGroups.GetAt( n, infoOPGroup );
			infoOPGroup.SetEnableFlagsSystem(0);
			m_arrayOPGroups.SetAt( n, infoOPGroup );
		}
		break;

	case OPGROUP_DATABASE:
		for ( int n = 0; n < GetSize(); n++ )
		{
			COPGroupInfo infoOPGroup;
			m_arrayOPGroups.GetAt( n, infoOPGroup );
			infoOPGroup.SetEnableFlagsDatabase(0);
			infoOPGroup.SetDeferFlagDatabase( TRUE );
			m_arrayOPGroups.SetAt( n, infoOPGroup );
		}
		break;

	case OPGROUP_LOCATION:
		for ( int n = 0; n < GetSize(); n++ )
		{
			COPGroupInfo infoOPGroup;
			m_arrayOPGroups.GetAt( n, infoOPGroup );
			infoOPGroup.SetEnableFlagsLocation(0);
			infoOPGroup.SetDeferFlagLocation( TRUE );
			m_arrayOPGroups.SetAt( n, infoOPGroup );
		}
		break;
	}
}

/**********************************************************************/

bool COPGroupTable::GetOPGroup( COPGroupMiniInfo& infoMini )
{
	bool bResult = FALSE;

	COPGroupInfo infoOPGroup;
	infoOPGroup.SetGroupNo( infoMini.m_nGroupNo );

	int nGroupIdx;
	if ( m_arrayOPGroups.Find( infoOPGroup, nGroupIdx ) == TRUE )
		bResult = GetOPGroup( nGroupIdx, infoMini );

	return bResult;
}

/**********************************************************************/

bool COPGroupTable::GetOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini )
{
	bool bResult = FALSE;

	if ( ( nGroupIdx >= 0 ) && ( nGroupIdx < m_arrayOPGroups.GetSize() ) )
	{
		COPGroupInfo infoOPGroup;
		m_arrayOPGroups.GetAt( nGroupIdx, infoOPGroup );

		infoMini.m_nGroupNo = infoOPGroup.GetGroupNo();
		infoMini.m_strName = infoOPGroup.GetName();
			
		switch( infoMini.m_nType )
		{
		case OPGROUP_SYSTEM:
			infoMini.m_bDefer = FALSE;
			infoMini.m_nFlags = infoOPGroup.GetEnableFlagsSystem();
			bResult = TRUE;
			break;

		case OPGROUP_DATABASE:
			infoMini.m_bDefer = infoOPGroup.GetDeferFlagDatabase();
			infoMini.m_nFlags = infoOPGroup.GetEnableFlagsDatabase();
			bResult = TRUE;
			break;

		case OPGROUP_LOCATION:
			infoMini.m_bDefer = infoOPGroup.GetDeferFlagLocation();
			infoMini.m_nFlags = infoOPGroup.GetEnableFlagsLocation();	
			bResult = TRUE;
			break;
		}
	}
	return bResult;
}

/**********************************************************************/

void COPGroupTable::UpdateOPGroup( COPGroupMiniInfo& infoMini )
{
	COPGroupInfo infoOPGroup;
	infoOPGroup.SetGroupNo( infoMini.m_nGroupNo );

	int nGroupIdx;
	if ( m_arrayOPGroups.Find( infoOPGroup, nGroupIdx ) == TRUE )
		UpdateOPGroup( nGroupIdx, infoMini );
}

/**********************************************************************/

void COPGroupTable::UpdateOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini )
{
	if ( ( nGroupIdx >= 0 ) && ( nGroupIdx < m_arrayOPGroups.GetSize() ) )
	{
		COPGroupInfo infoOPGroup;
		m_arrayOPGroups.GetAt( nGroupIdx, infoOPGroup );

		if ( infoOPGroup.GetGroupNo() == infoMini.m_nGroupNo )
		{
			switch( infoMini.m_nType )
			{
			case OPGROUP_SYSTEM:
				infoOPGroup.SetName( infoMini.m_strName );
				infoOPGroup.SetEnableFlagsSystem( infoMini.m_nFlags );
				break;

			case OPGROUP_DATABASE:
				infoOPGroup.SetEnableFlagsDatabase( infoMini.m_nFlags );
				infoOPGroup.SetDeferFlagDatabase( infoMini.m_bDefer );
				break;

			case OPGROUP_LOCATION:
				infoOPGroup.SetEnableFlagsLocation( infoMini.m_nFlags );
				infoOPGroup.SetDeferFlagLocation( infoMini.m_bDefer );
				break;
			}

			m_arrayOPGroups.SetAt( nGroupIdx, infoOPGroup );
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

COPGroupArray::COPGroupArray( int nOPGroupMode, COPGroupTable& OPGroupTable ) : CSharedStringArray(), m_OPGroupTable( OPGroupTable )
{
	m_nOPGroupMode = nOPGroupMode;
}

/**********************************************************************/

int COPGroupArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );
	
	m_OPGroupTable.ResetOPGroupList( m_nOPGroupMode );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for( int n = 0; n < GetSize(); n++ )
		{
			CCSV csv( GetAt(n) );

			if ( csv.GetInt(0) == 1 )
			{
				COPGroupMiniInfo infoMini;
				infoMini.m_nType = m_nOPGroupMode;
				infoMini.m_nGroupNo = csv.GetInt(1);
				infoMini.m_strName = csv.GetString(2);
				infoMini.m_bDefer = csv.GetBool(3);
				infoMini.m_nFlags = csv.GetInt(4);
				m_OPGroupTable.UpdateOPGroup( infoMini );
			}
		}
	}

	RemoveAt( 0, GetSize() );
	return nReply;
}

/**********************************************************************/

void COPGroupArray::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int nIdx = 0; nIdx < m_OPGroupTable.GetSize(); nIdx++ )
	{
		COPGroupMiniInfo infoMini;
		infoMini.m_nType = m_nOPGroupMode;
		m_OPGroupTable.GetOPGroup( nIdx, infoMini );

		CCSV csv;
		csv.Add(1);
		csv.Add(infoMini.m_nGroupNo );
		csv.Add(infoMini.m_strName );
		csv.Add(infoMini.m_bDefer );
		csv.Add(infoMini.m_nFlags );
		Add( csv.GetLine() );
	}
}

/**********************************************************************/

void COPGroupArray::UpdateOPGroup( COPGroupMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPGroupMode;
	m_OPGroupTable.UpdateOPGroup( infoMini );
}

/**********************************************************************/

void COPGroupArray::UpdateOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPGroupMode;
	m_OPGroupTable.UpdateOPGroup( nGroupIdx, infoMini );
}

/**********************************************************************/

void COPGroupArray::GetOPGroup( COPGroupMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPGroupMode;
	m_OPGroupTable.GetOPGroup( infoMini );
}

/**********************************************************************/

void COPGroupArray::GetOPGroup( int nGroupIdx, COPGroupMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPGroupMode;
	m_OPGroupTable.GetOPGroup( nGroupIdx, infoMini );
}

/**********************************************************************/

void COPGroupArray::ResetOPGroupList()
{
	m_OPGroupTable.ResetOPGroupList( m_nOPGroupMode );
}

/**********************************************************************/
