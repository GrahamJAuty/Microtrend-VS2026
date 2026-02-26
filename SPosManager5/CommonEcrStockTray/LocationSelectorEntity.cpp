/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "ReportpointCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "LocationSelectorEntity.h"
/**********************************************************************/

CLocationSelectorMiniLookup::CLocationSelectorMiniLookup()
{
	Reset();
}

/**********************************************************************/

void CLocationSelectorMiniLookup::Reset()
{
	m_nConType = NODE_SYSTEM;
	m_nEntityNo = 0;
}

/**********************************************************************/

CLocationSelectorMiniLookup::CLocationSelectorMiniLookup( int nConType, int nEntityNo )
{
	m_nConType = nConType;
	m_nEntityNo = nEntityNo;
}

/**********************************************************************/

CLocationSelectorLookup::CLocationSelectorLookup()
{
	Reset();
}

/**********************************************************************/

void CLocationSelectorLookup::Reset()
{
	m_nConType = NODE_SYSTEM;
	m_nDbNo = 0;
	m_nNwkNo = 0;
	m_nEntityNo = 0;
	m_nTermIdx = 0;
}

/**********************************************************************/

CLocationSelectorLookup::CLocationSelectorLookup( CPosTrayTask& Task )
{
	m_nConType = Task.GetBatchConType();
	m_nDbNo = Task.GetBatchConDbNo();
	m_nNwkNo = Task.GetBatchConNwkNo();
	m_nEntityNo = Task.GetBatchConEntityNo();
	m_nTermIdx = Task.GetBatchConTermIdx();
}

/**********************************************************************/

CLocationSelectorLookup::CLocationSelectorLookup( int nDbIdx, CFNBDatabaseOptions& Options )
{
	m_nConType = Options.GetLocSelType();
	m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
	m_nNwkNo = Options.GetLocSelNwkNo();
	m_nEntityNo = Options.GetLocSelEntityNo();
	m_nTermIdx = 0;
}

/**********************************************************************/

CLocationSelectorEntity::CLocationSelectorEntity()
{
	Reset();
}

/**********************************************************************/

void CLocationSelectorEntity::Reset()
{
	SetSystemNode();
}

/**********************************************************************/

CLocationSelectorEntity::CLocationSelectorEntity( CLocationSelectorLookup& LocSelLookup )
{
	SetInvalidNode();

	int nDbNo = LocSelLookup.GetDbNo();
	int nNwkNo = LocSelLookup.GetNwkNo();
	int nEntityNo = LocSelLookup.GetEntityNo();
	int nTermIdx = LocSelLookup.GetTermIdx();
	int nIdx = 0;
	
	switch( LocSelLookup.GetConType() )
	{
	case NODE_SYSTEM:
		SetSystemNode();
		break;

	case NODE_DATABASE:
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nIdx ) )
			SetDatabaseNode( nIdx );
		break;

	case NODE_LOCATION:
		if ( dbLocation.FindLocationByNumber( nNwkNo, nEntityNo, nIdx ) )
			SetLocationNode( nIdx );
		break;

	case NODE_LOCATION_TERMINAL:
		if ( dbLocation.FindLocationByNumber( nNwkNo, nEntityNo, nIdx ) )
		{
			SetLocationNode( nIdx );
			SetTerminalNode( nTermIdx );
		}
		break;

	case NODE_REPORTPOINT:
		if ( dbReportpoint.FindReportpointByNumber( nDbNo, nEntityNo, nIdx ) )
			SetReportpointNode( nIdx );
		break;

	case NODE_LOCATIONSET:
		if ( dbLocationSet.FindLocationSetByNumber( nDbNo, nEntityNo, nIdx ) )
			SetLocationSetNode( nIdx );
		break;

#ifdef STOCKMAN_SYSTEM
	case NODE_STOCKPOINT:
		if ( dbStockpoint.FindStockpointByNumber( nDbNo, nEntityNo, nIdx ) )
			SetStockpointNode( nIdx );
		break;

		case NODE_SITE:
			if ( dbSite.FindSiteByNumber( nDbNo, nEntityNo, nIdx ) )
				SetSiteNode( nIdx );
		break;
#endif
	}
}

/**********************************************************************/

void CLocationSelectorEntity::SetSystemNode()
{
	m_nConType = NODE_SYSTEM;
	m_nDbNo = 0;
	m_nDbIdx = 0;
	m_nNwkNo = 0;
	m_nEntityNo = 0;
	m_nEntityIdx = 0;
	m_nTermIdx = 0;
	m_strEntityName = "System";
}

/**********************************************************************/

void CLocationSelectorEntity::SetSystemFilterNode()
{
	m_nConType = NODE_SYSTEM_FILTER;
	m_nDbNo = 0;
	m_nDbIdx = 0;
	m_nNwkNo = 0;
	m_nEntityNo = 0;
	m_nEntityIdx = 0;
	m_nTermIdx = 0;
	m_strEntityName = "System Filter";
}

/**********************************************************************/

void CLocationSelectorEntity::SetTimeSettingNode()
{
	m_nConType = NODE_TIME_SETTING;
	m_nDbNo = 0;
	m_nDbIdx = 0;
	m_nNwkNo = 0;
	m_nEntityNo = 0;
	m_nEntityIdx = 0;
	m_nTermIdx = 0;
	m_strEntityName = "Time Settings";
}

/**********************************************************************/

void CLocationSelectorEntity::SetReportListNode()
{
	m_nConType = NODE_REPORT_LIST;
	m_nDbNo = 0;
	m_nDbIdx = 0;
	m_nNwkNo = 0;
	m_nEntityNo = 0;
	m_nEntityIdx = 0;
	m_nTermIdx = 0;
	m_strEntityName = "Report Selection";
}

/**********************************************************************/

void CLocationSelectorEntity::SetDatabaseNode( int nDbIdx )
{
	m_nConType = NODE_DATABASE;
	m_nDbNo = dbDatabase.GetDbNo( nDbIdx );
	m_nDbIdx = nDbIdx;
	m_nNwkNo = 0;
	m_nEntityNo = m_nDbNo;
	m_nEntityIdx = m_nDbIdx;
	m_nTermIdx = 0;
	m_strEntityName = dbDatabase.GetName( nDbIdx );
}

/**********************************************************************/

void CLocationSelectorEntity::SetLocationNode( int nLocIdx )
{
	m_nConType = NODE_LOCATION;
	
	m_nDbNo = dbLocation.GetDbNo( nLocIdx );
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, m_nDbIdx ) == FALSE )
		m_nDbIdx = 0;

	m_nNwkNo = dbLocation.GetNetworkNo( nLocIdx );
	m_nEntityNo = dbLocation.GetLocNo( nLocIdx );
	m_nEntityIdx = nLocIdx;
	m_nTermIdx = 0;
	m_strEntityName = dbLocation.GetName( nLocIdx );
}

/**********************************************************************/

void CLocationSelectorEntity::SetTerminalNode( int nTermIdx )
{
	switch( m_nConType )
	{
	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
		m_nConType = NODE_LOCATION_TERMINAL;
		m_nTermIdx = nTermIdx;
		break;
	}
}

/**********************************************************************/

void CLocationSelectorEntity::SetNetworkNode( int nNwkIdx )
{
	m_nConType = NODE_NETWORK;
	m_nDbNo = 0;
	m_nDbIdx = 0;
	m_nNwkNo = dbNetwork.GetNetworkNo( nNwkIdx );
	m_nEntityNo = dbNetwork.GetNetworkNo( nNwkIdx );
	m_nEntityIdx = nNwkIdx;
	m_nTermIdx = 0;
	m_strEntityName = dbNetwork.GetName( nNwkIdx );
}

/**********************************************************************/

void CLocationSelectorEntity::SetReportpointNode( int nRpIdx )
{
	m_nConType = NODE_REPORTPOINT;
	
	m_nDbNo = dbReportpoint.GetDbNo( nRpIdx );
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, m_nDbIdx ) == FALSE )
		m_nDbIdx = 0;

	m_nNwkNo = 0;
	m_nEntityNo = dbReportpoint.GetRpNo( nRpIdx );
	m_nEntityIdx = nRpIdx;
	m_nTermIdx = 0;
	m_strEntityName = dbReportpoint.GetName( nRpIdx );
}

/**********************************************************************/

void CLocationSelectorEntity::SetLocationSetNode( int nLsIdx )
{
	m_nConType = NODE_LOCATIONSET;
	
	m_nDbNo = dbLocationSet.GetDbNo( nLsIdx );
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, m_nDbIdx ) == FALSE )
		m_nDbIdx = 0;

	m_nNwkNo = 0;
	m_nEntityNo = dbLocationSet.GetLsNo( nLsIdx );
	m_nEntityIdx = nLsIdx;
	m_nTermIdx = 0;
	m_strEntityName = dbLocationSet.GetName( nLsIdx );
}


/**********************************************************************/

void CLocationSelectorEntity::SetKeyboardMapNode( int nMapIdx )
{
	m_nConType = NODE_DBKEYBOARDSET;
	m_nDbIdx = 0;
	m_nNwkNo = 0;
	m_nEntityNo = 0;
	m_nEntityIdx = nMapIdx;
	m_nTermIdx = 0;
	m_strEntityName = "";
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CLocationSelectorEntity::SetStockpointNode( int nSpIdx )
{
	m_nConType = NODE_STOCKPOINT;
	
	m_nDbNo = dbStockpoint.GetDbNo( nSpIdx );
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, m_nDbIdx ) == FALSE )
		m_nDbIdx = 0;
	
	m_nNwkNo = 0;
	m_nEntityNo = dbStockpoint.GetSpNo( nSpIdx );
	m_nEntityIdx = nSpIdx;
	m_nTermIdx = 0;
	m_strEntityName = dbStockpoint.GetName( nSpIdx );		
}

/**********************************************************************/

void CLocationSelectorEntity::SetSiteNode( int nSiteIdx )
{
	m_nConType = NODE_SITE;
	
	m_nDbNo = dbSite.GetDbNo( nSiteIdx );
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, m_nDbIdx ) == FALSE )
		m_nDbIdx = 0;
	
	m_nNwkNo = 0;
	m_nEntityNo = dbSite.GetSiteNo( nSiteIdx );
	m_nEntityIdx = nSiteIdx;
	m_nTermIdx = 0;
	m_strEntityName = dbSite.GetName( nSiteIdx );
}

/**********************************************************************/
#endif
/**********************************************************************/

void CLocationSelectorEntity::SetInvalidNode()
{
	m_nConType = NODE_PLACEHOLDER;
	m_nDbNo = 0;
	m_nDbIdx = 0;
	m_nNwkNo = 0;
	m_nEntityNo = 0;
	m_nEntityIdx = 0;
	m_nTermIdx = 0;
	m_strEntityName = "Invalid";
}

/**********************************************************************/

const char* CLocationSelectorEntity::GetConsolidationTitle ( bool bBrackets )
{
	switch( m_nConType )
	{
	case NODE_SYSTEM:
	case NODE_DATABASE:
	case NODE_NETWORK:
	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
	case NODE_REPORTPOINT:
	case NODE_LOCATIONSET:
	case NODE_STOCKPOINT:
	case NODE_SITE:
		{
			CString strTerminalName = "";

			if ( NODE_LOCATION_TERMINAL == m_nConType )
				strTerminalName.Format ( ", %s", dbLocation.GetTerminalName ( m_nEntityIdx, m_nTermIdx ) );
			
			if ( TRUE == bBrackets )
				m_strConsolidationTitle.Format ( "( %s%s )", (const char*) m_strEntityName, (const char*) strTerminalName );
			else
				m_strConsolidationTitle.Format ( "%s%s", (const char*) m_strEntityName, (const char*) strTerminalName );
		}
		break;

	default:
		m_strConsolidationTitle = "Invalid";
		break;
	}
	
	return m_strConsolidationTitle;
}

/**********************************************************************/

void CLocationSelectorEntity::GetEPOSSelectArray( CEposSelectArray& SelectArray )
{
	SelectArray.RemoveAll();

	switch ( m_nConType )
	{
	case NODE_SYSTEM:
		SelectArray.SelectSystem();
		break;

	case NODE_DATABASE:
		SelectArray.SelectDatabase( m_nEntityIdx );
		break;

	case NODE_LOCATION:
		SelectArray.SelectLocation ( m_nEntityIdx );
		break;

	case NODE_LOCATION_TERMINAL:
		SelectArray.SelectTerminal ( m_nEntityIdx, m_nTermIdx );
		break;

	case NODE_REPORTPOINT:
		{
			if ( dbDatabase.GetDbNo( m_nDbIdx ) == m_nDbNo )
			{
				CArray<int,int> Set;
				dbLocation.GetDatabaseLocIdxSet( m_nDbIdx, Set );

				for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
				{
					int nLocIdx = Set.GetAt ( nIndex );

					for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
						if ( dbLocation.HasRpNoLink( nLocIdx, nTerminalIdx, m_nEntityNo ) == TRUE )
							SelectArray.SelectTerminal ( nLocIdx, nTerminalIdx );
				}
			}
		}
		break;

	case NODE_LOCATIONSET:
		{
			CReportConsolidationArray<CSortedIntItem> LocIdxArray;
			dbLocationSet.TidyNwkLocNoArray( m_nEntityIdx, LocIdxArray );

			for ( int n = 0; n < LocIdxArray.GetSize(); n++ )
			{
				CSortedIntItem item;
				LocIdxArray.GetAt( n, item );
				SelectArray.SelectLocation( item.m_nItem );
			}
		}
		break;

#ifdef STOCKMAN_SYSTEM
		case NODE_STOCKPOINT:
		{
			if ( dbDatabase.GetDbNo( m_nDbIdx ) == m_nDbNo )
			{
				CArray<int,int> Set;
				dbLocation.GetDatabaseLocIdxSet( m_nDbIdx, Set );

				for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
				{
					int nLocIdx = Set.GetAt ( nIndex );

					for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
						if ( dbLocation.GetSpNo ( nLocIdx, nTerminalIdx ) == m_nEntityNo )
							SelectArray.SelectTerminal ( nLocIdx, nTerminalIdx );
				}
			}
		}
		break;
#endif
	}
}

/**********************************************************************/

bool CLocationSelectorEntity::SimpleCompare( CLocationSelectorEntity& other )
{
	return ( ( m_nConType == other.m_nConType ) && ( m_nDbIdx == other.m_nDbIdx ) && ( m_nEntityIdx == other.m_nEntityIdx ) );

}

/**********************************************************************/
