/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "ServerSelectionList.h"
/**********************************************************************/

CServerSelectionInfo::CServerSelectionInfo()
{
	Reset();
}

/**********************************************************************/

void CServerSelectionInfo::Reset()
{
	int m_nDbNo = 0;
	int m_nNwkNo = 0;
	int m_nLocNo = 0;
	int m_nServerNo = 0;
	CString m_strServerName = "";
	m_dHourlyRate = 0.0;
	m_nDailyHours = 0;
	m_bMidnightShift = FALSE;
	bool m_bSelected = FALSE;
}

/**********************************************************************/

void CServerSelectionInfo::Add( CServerSelectionInfo& source )
{
}

/**********************************************************************/

int CServerSelectionInfo::Compare( CServerSelectionInfo& source, int nHint )
{
	if ( m_nDbNo != source.m_nDbNo )
		return ( ( m_nDbNo > source.m_nDbNo ) ? 1 : -1 );

	if ( m_nNwkNo != source.m_nNwkNo )
		return ( ( m_nNwkNo > source.m_nNwkNo ) ? 1 : -1 );

	if ( m_nLocNo != source.m_nLocNo )
		return ( ( m_nLocNo > source.m_nLocNo ) ? 1 : -1 );

	if ( m_nServerNo != source.m_nServerNo )
		return ( ( m_nServerNo > source.m_nServerNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CServerSelectionList::CServerSelectionList(void)
{
	m_bUnknownServer = TRUE;
}

/**********************************************************************/

void CServerSelectionList::BuildList()
{
	ClearList();

	CDataManagerInfo info;
			
	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_SYS:
		{
			DataManagerNonDb.CloseServer( info );
			DataManagerNonDb.OpenServer( DB_READONLY, -1, -1, info );

			for ( int n = 0; n < DataManagerNonDb.Server.GetSize(); n++ )
			{
				CServerCSVRecord Server;
				DataManagerNonDb.Server.GetAt( n, Server );

				int nServerNo = Server.GetServerNo();

				if ( nServerNo != 0 )
				{
					CServerSelectionInfo infoServer;
					infoServer.m_nDbNo = 0;
					infoServer.m_nNwkNo = 0;
					infoServer.m_nLocNo = 0;
					infoServer.m_nServerNo = nServerNo;
					infoServer.m_strServerName = Server.GetReportName();
					infoServer.m_dHourlyRate = Server.GetHourlyRate();
					infoServer.m_nDailyHours = Server.GetDailyHours();
					infoServer.m_bMidnightShift = Server.GetMidnightShiftFlag();
					infoServer.m_bSelected = TRUE;
					m_arrayServers.Consolidate( infoServer );

					if ( DealerFlags.GetServerConsolidationType() == SERVER_SORT_BYNAME )
					{
						CSortedIntAndStringItem itemServer;
						itemServer.m_nItem = nServerNo;
						itemServer.m_strItem = Server.GetReportName();
						m_arraySortedNames.Consolidate( itemServer );
					}
				}
			}
		}
		break;

	case SERVER_DB:
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CDataManagerInfo info;
			DataManagerNonDb.CloseServer( info );
			DataManagerNonDb.OpenServer( DB_READONLY, nDbIdx, -1, info );

			for ( int n = 0; n < DataManagerNonDb.Server.GetSize(); n++ )
			{
				CServerCSVRecord Server;
				DataManagerNonDb.Server.GetAt( n, Server );

				int nServerNo = Server.GetServerNo();

				if ( nServerNo != 0 )
				{
					CServerSelectionInfo infoServer;
					infoServer.m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
					infoServer.m_nNwkNo = 0;
					infoServer.m_nLocNo = 0;
					infoServer.m_nServerNo = nServerNo;
					infoServer.m_strServerName = Server.GetReportName();
					infoServer.m_dHourlyRate = Server.GetHourlyRate();
					infoServer.m_nDailyHours = Server.GetDailyHours();
					infoServer.m_bMidnightShift = Server.GetMidnightShiftFlag();
					infoServer.m_bSelected = TRUE;
					m_arrayServers.Consolidate( infoServer );

					if ( DealerFlags.GetServerConsolidationType() == SERVER_SORT_BYNAME )
					{
						CSortedIntAndStringItem itemServer;
						itemServer.m_nItem = nServerNo;
						itemServer.m_strItem = Server.GetReportName();
						m_arraySortedNames.Consolidate( itemServer );
					}
				}
			}
		}
		break;

	case SERVER_LOC:
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			CArray<int,int> arrayLocIdx;
			dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdx );

			for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
			{
				int nLocIdx = arrayLocIdx.GetAt(n);

				CDataManagerInfo info;
				DataManagerNonDb.CloseServer( info );
				DataManagerNonDb.OpenServer( DB_READONLY, nDbIdx, nLocIdx, info );

				for ( int n = 0; n < DataManagerNonDb.Server.GetSize(); n++ )
				{
					CServerCSVRecord Server;
					DataManagerNonDb.Server.GetAt( n, Server );

					int nServerNo = Server.GetServerNo();

					if ( nServerNo != 0 )
					{
						CServerSelectionInfo infoServer;
						infoServer.m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
						infoServer.m_nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
						infoServer.m_nLocNo = dbLocation.GetLocNo( nLocIdx );
						infoServer.m_nServerNo = nServerNo;
						infoServer.m_strServerName = Server.GetReportName();
						infoServer.m_dHourlyRate = Server.GetHourlyRate();
						infoServer.m_nDailyHours = Server.GetDailyHours();
						infoServer.m_bMidnightShift = Server.GetMidnightShiftFlag();
						infoServer.m_bSelected = TRUE;
						m_arrayServers.Consolidate( infoServer );
						
						if ( DealerFlags.GetServerConsolidationType() == SERVER_SORT_BYNAME )
						{
							CSortedIntAndStringItem itemServer;
							itemServer.m_nItem = nServerNo;
							itemServer.m_strItem = Server.GetReportName();
							m_arraySortedNames.Consolidate( itemServer );
						}
					}
				}
			}
		}
		break;
	}

	DataManagerNonDb.CloseServer( info );
}

/**********************************************************************/

void CServerSelectionList::ClearList()
{
	m_arrayServers.RemoveAll();
	m_arraySortedNames.RemoveAll();
}

/**********************************************************************/

bool CServerSelectionList::FindServer( CServerSelectionInfo& info, int& nPos )
{
	return m_arrayServers.Find( info, nPos );
}

/**********************************************************************/

void CServerSelectionList::GetServerStatus( int nPos, CServerSelectionInfo& info )
{
	if ((nPos >= 0) && (nPos < m_arrayServers.GetSize()))
	{
		m_arrayServers.GetAt(nPos, info);
	}
	else
	{
		info.m_nDbNo = 0;
		info.m_nNwkNo = 0;
		info.m_nLocNo = 0;
		info.m_nServerNo = 0;
		info.m_strServerName = "";
		info.m_dHourlyRate = 0.0;
		info.m_nDailyHours = 0;
		info.m_bMidnightShift = FALSE;
		info.m_bSelected = FALSE;
	}
}

/**********************************************************************/

void CServerSelectionList::SetServerStatus( int nPos, CServerSelectionInfo& info )
{
	if ((nPos >= 0) && (nPos < m_arrayServers.GetSize()))
	{
		m_arrayServers.SetAt(nPos, info);
	}
}

/**********************************************************************/

void CServerSelectionList::GetServerStatus( CServerSelectionInfo& info )
{
	info.m_bSelected = FALSE;

	int nPos;
	if (FindServer(info, nPos) == TRUE)
	{
		m_arrayServers.GetAt(nPos, info);
	}
	else
	{
		info.m_strServerName = "Unknown Server";
		info.m_bSelected = m_bUnknownServer;
	}
}

/**********************************************************************/

void CServerSelectionList::SetServerStatus( CServerSelectionInfo& info )
{
	int nPos;
	if ( FindServer( info, nPos ) == TRUE )
	{
		CServerSelectionInfo infoArray;
		m_arrayServers.GetAt( nPos, infoArray );
		infoArray.m_bSelected = info.m_bSelected;
		m_arrayServers.SetAt( nPos, infoArray );
	}
}

/**********************************************************************/

int CServerSelectionList::FindSortedName( int nServerNo, const char* szName )
{
	CSortedIntAndStringItem item;
	item.m_nItem = nServerNo;
	item.m_strItem = szName;

	int nPos;
	if (m_arraySortedNames.Find(item, nPos) == TRUE)
	{
		return nPos;
	}
	else
	{
		return 999999;
	}
}

/**********************************************************************/

const char* CServerSelectionList::GetSortedName( int nNameIdx )
{
	m_strServerName = "Unknown";

	if ( ( nNameIdx >= 0 ) && ( nNameIdx < m_arraySortedNames.GetSize() ) && ( nNameIdx < 999999 ) )
	{
		CSortedIntAndStringItem item;
		m_arraySortedNames.GetAt( nNameIdx, item );
		m_strServerName = item.m_strItem;
	}

	return m_strServerName;
}

/**********************************************************************/

void CServerSelectionList::GetServerStatus( CServerSelectionInfo& info, int nDbNo, int nNwkNo, int nLocNo, int nServerNo )
{
	int nNwkToCheck, nLocToCheck, nDbToCheck;
			
	switch( EcrmanOptions.GetFeaturesServerSettingsType() )
	{
	case SERVER_LOC:
		nNwkToCheck = nNwkNo;
		nLocToCheck = nLocNo;
		nDbToCheck = nDbNo;
		break;

	case SERVER_DB:
		nNwkToCheck = 0;
		nLocToCheck = 0;
		nDbToCheck = nDbNo;
		break;

	case SERVER_SYS:
	default:
		nNwkToCheck = 0;
		nLocToCheck = 0;
		nDbToCheck = 0;
		break;
	}

	info.m_nDbNo = nDbToCheck;
	info.m_nNwkNo = nNwkToCheck;
	info.m_nLocNo = nLocToCheck;
	info.m_nServerNo = nServerNo;
	info.m_bSelected = FALSE;
	GetServerStatus( info );
}

/**********************************************************************/

int CServerSelectionList::GetServerNameIdx( int nLocIdx, int nServer )
{
	CServerSelectionInfo info;
	int nDbNo = dbLocation.GetDbNo( nLocIdx );
	int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
	int nLocNo = dbLocation.GetLocNo( nLocIdx );
	GetServerStatus( info, nDbNo, nNwkNo, nLocNo, nServer );
	return FindSortedName( nServer, info.m_strServerName );
}

/**********************************************************************/

void CServerSelectionList::GetServerConsolidationString( int nDbIdx, int nLocIdx, int nServerNo, CString& strServer )
{
	switch( DealerFlags.GetServerConsolidationType() )
	{
	case SERVER_SORT_BYCODE:
		strServer.Format( "%4.4d", nServerNo );
		break;

	case SERVER_SORT_BYNAME:
		{
			CServerSelectionInfo info;
			int nDbNo = dbLocation.GetDbNo( nLocIdx );
			int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
			int nLocNo = dbLocation.GetLocNo( nLocIdx );
			GetServerStatus( info, nDbNo, nNwkNo, nLocNo, nServerNo );

			strServer.Format( "%4.4d%6.6d", 
				nServerNo,
				FindSortedName( nServerNo, info.m_strServerName ) );
		}
		break;

	case SERVER_SORT_BYFILE:
	default:
		switch( EcrmanOptions.GetFeaturesServerSettingsType() )
		{
		case SERVER_DB:		strServer.Format ( "%6.6d%4.4d", nDbIdx, nServerNo );	break;
		case SERVER_LOC:	strServer.Format ( "%6.6d%4.4d", nLocIdx, nServerNo );	break;
		case SERVER_NONE:
		case SERVER_SYS:
		default:			strServer.Format( "%4.4d", nServerNo );					break;
		}
	}
}

/**********************************************************************/

void CServerSelectionList::ProcessServerLabel( CString& strLabel, int& nServerNo, CString& strServerName )
{
	switch( DealerFlags.GetServerConsolidationType() )
	{
	case SERVER_SORT_BYCODE:
		nServerNo = atoi( strLabel );
		strServerName.Format( "Server %d", nServerNo );
		break;

	case SERVER_SORT_BYNAME:
		nServerNo = atoi( strLabel.Left(4) );
		strServerName = GetSortedName( atoi( strLabel.Right(6) ) );
		break;

	case SERVER_SORT_BYFILE:
	default:
		switch( EcrmanOptions.GetFeaturesServerSettingsType() )
		{
		case SERVER_DB:
			{
				nServerNo = atoi( strLabel.Right(4) );
				
				int nDbIdx = atoi( strLabel.Left(6) );
				int nDbNo = dbDatabase.GetDbNo( nDbIdx );

				CServerSelectionInfo info;
				GetServerStatus( info, nDbNo, 0, 0, nServerNo );

				strServerName = info.m_strServerName;
			}
			break;

		case SERVER_LOC:
			{
				nServerNo = atoi( strLabel.Right(4) );
				
				int nLocIdx = atoi( strLabel.Left(6) );
				int nDbNo = dbLocation.GetDbNo( nLocIdx );
				int nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
				int nLocNo = dbLocation.GetLocNo( nLocIdx );

				CServerSelectionInfo info;
				GetServerStatus( info, nDbNo, nNwkNo, nLocNo, nServerNo );

				strServerName = info.m_strServerName;
			}
			break;

		case SERVER_SYS:
			{
				nServerNo = atoi( strLabel );
				
				CServerSelectionInfo info;
				GetServerStatus( info, 0, 0, 0, nServerNo );

				strServerName = info.m_strServerName;
			}
			break;
	
		case SERVER_NONE:
		default:
			nServerNo = atoi( strLabel );
			strServerName.Format( "Server %d", nServerNo );
			break;
		}
	}
}

/**********************************************************************/

const char* CServerSelectionList::GetServerName( int nConLevel, int nEntityIdx, int nServerNo, bool bDeferredOnly )
{
	m_strServerName = "";

	if ( ( nServerNo != 0 ) || ( TRUE == bDeferredOnly ) )
	{
		switch ( nConLevel )
		{
		case NODE_SYSTEM:
			{
				switch( EcrmanOptions.GetFeaturesServerSettingsType() )
				{
				case SERVER_DB:
					m_strServerName = "Database specific";
					break;

				case SERVER_LOC:
					m_strServerName = "Location specific";
					break;

				case SERVER_SYS:
					if ( FALSE == bDeferredOnly )
					{
						CServerSelectionInfo infoServer;
						DataManagerNonDb.ServerNameTable.GetServerStatus( infoServer, 0, 0, 0, nServerNo );
						m_strServerName = infoServer.m_strServerName;
					}
					break;
				}
			}
			break;

		case NODE_DATABASE:
			{
				switch( EcrmanOptions.GetFeaturesServerSettingsType() )
				{
				case SERVER_LOC:
					m_strServerName = "Location specific";
					break;

				case SERVER_DB:
					if ( FALSE == bDeferredOnly )
					{
						CServerSelectionInfo infoServer;
						int nDbNo = dbDatabase.GetDbNo( nEntityIdx );
						DataManagerNonDb.ServerNameTable.GetServerStatus( infoServer, nDbNo, 0, 0, nServerNo );
						m_strServerName = infoServer.m_strServerName;
					}
					break;

				case SERVER_SYS:
					if ( FALSE == bDeferredOnly )
					{
						CServerSelectionInfo infoServer;
						DataManagerNonDb.ServerNameTable.GetServerStatus( infoServer, 0, 0, 0, nServerNo );
						m_strServerName = infoServer.m_strServerName;
					}
					break;
				}
			}
			break;

		case NODE_LOCATION:
		case NODE_LOCATION_TERMINAL:
			{
				if ( FALSE == bDeferredOnly )
				{
					switch( EcrmanOptions.GetFeaturesServerSettingsType() )
					{
					case SERVER_LOC:
						{
							CServerSelectionInfo infoServer;
							int nDbNo = dbLocation.GetDbNo( nEntityIdx );
							int nNwkNo = dbLocation.GetNetworkNo( nEntityIdx );
							int nLocNo = dbLocation.GetLocNo( nEntityIdx );
							DataManagerNonDb.ServerNameTable.GetServerStatus( infoServer, nDbNo, nNwkNo, nLocNo, nServerNo );
							m_strServerName = infoServer.m_strServerName;
						}
						break;

					case SERVER_DB:
						{
							CServerSelectionInfo infoServer;
							int nDbNo = dbLocation.GetDbNo( nEntityIdx );
							DataManagerNonDb.ServerNameTable.GetServerStatus( infoServer, nDbNo, 0, 0, nServerNo );
							m_strServerName = infoServer.m_strServerName;
						}
						break;

					case SERVER_SYS:
						{
							CServerSelectionInfo infoServer;
							DataManagerNonDb.ServerNameTable.GetServerStatus( infoServer, 0, 0, 0, nServerNo );
							m_strServerName = infoServer.m_strServerName;
						}
						break;
					}
				}
			}
			break;
		}
	}

	return m_strServerName;
}

/**********************************************************************/


