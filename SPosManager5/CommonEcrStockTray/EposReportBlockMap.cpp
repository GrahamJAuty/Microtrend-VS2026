/**********************************************************************/
#include "Consolidation.h"
#include "DatabaseCSVArray.h"
#include "LocationCSVArray.h"
#include "NodeTypes.h"
#include "ReportpointCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
/**********************************************************************/
#include "EposReportBlockMap.h"
/**********************************************************************/

CEposReportBlockMapTempIndex::CEposReportBlockMapTempIndex()
{
	Reset();
}

/**********************************************************************/

void CEposReportBlockMapTempIndex::Reset()
{
	m_nDbIdx = 0;
	m_nRpIdx = 0;
	m_nLocIdx = 0;
	m_nTermIdx = 0;
	m_nSelArrayIdx = 0;
}

/**********************************************************************/

void CEposReportBlockMapTempIndex::Add ( CEposReportBlockMapTempIndex& source )
{
}

/**********************************************************************/

int CEposReportBlockMapTempIndex::Compare( CEposReportBlockMapTempIndex& source, int nHint )
{
	if ( m_nDbIdx != source.m_nDbIdx )
		return ( m_nDbIdx > source.m_nDbIdx ? 1 : -1 );

	if ( m_nRpIdx != source.m_nRpIdx )
		return ( m_nRpIdx > source.m_nRpIdx ? 1 : -1 );

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( m_nLocIdx > source.m_nLocIdx ? 1 : -1 );

	if ( m_nTermIdx != source.m_nTermIdx )
		return ( m_nTermIdx > source.m_nTermIdx ? 1 : -1 );

	if ( m_nSelArrayIdx != source.m_nSelArrayIdx )
		return ( m_nSelArrayIdx > source.m_nSelArrayIdx ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

void CEposReportBlockMapEntity::Copy( CEposReportBlockMapEntity& source )
{
	m_nEntityType = source.m_nEntityType;
	m_nDbIdx = source.m_nDbIdx;
	m_nLocIdx = source.m_nLocIdx;
	m_nRpIdx = source.m_nRpIdx;
	m_nTermIdx = source.m_nTermIdx;
	m_bAddToTotal = source.m_bAddToTotal;
}

/**********************************************************************/

void CEposReportBlockMapEntity::GetHeaderText( CString& strHeader, bool bFullLine )
{
	strHeader = bFullLine ? "<..>" : "";

	switch( m_nEntityType )
	{
	case NODE_LOCATION_TERMINAL:

		if ( dbLocation.IsPMSPseudoLocation( m_nLocIdx ) == TRUE )
			strHeader += "Guest Accounts";
		else
		{
			strHeader += dbLocation.GetName( m_nLocIdx );
			strHeader += ", ";
			strHeader += dbLocation.GetTerminalName( m_nLocIdx, m_nTermIdx );
		}
		break;

	case NODE_LOCATION:
		if ( dbLocation.IsPMSPseudoLocation( m_nLocIdx ) == TRUE )
			strHeader += "Guest Accounts";
		else
			strHeader += dbLocation.GetName( m_nLocIdx );
		break;

	case NODE_REPORTPOINT:
		if ( m_nRpIdx >= 0 )
			strHeader = dbReportpoint.GetName( m_nRpIdx );
		else
			strHeader = "No Report Point";
		break;

#ifdef STOCKMAN_SYSTEM

	case NODE_STOCKPOINT:
		if ( m_nRpIdx >= 0 )
			strHeader = dbStockpoint.GetName( m_nRpIdx );
		else
			strHeader = "No Stock Point";
		break;

#endif

	case NODE_DATABASE:
		strHeader += dbDatabase.GetName( m_nDbIdx );
		break;

	case NODE_SYSTEM:
		strHeader += "System";
		break;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportBlockMapSortInfo::CEposReportBlockMapSortInfo()
{
	Reset();
}

/**********************************************************************/

void CEposReportBlockMapSortInfo::Reset()
{
	m_bIsSystemBlock = FALSE;
	m_nDbSortIdx = 0;
	m_bIsDbBlock = 0;
	m_nRpSortIdx = 0;
	m_nLocSortIdx = 0;
	m_nMapIdx = 0;
}

/**********************************************************************/

void CEposReportBlockMapSortInfo::Add ( CEposReportBlockMapSortInfo& source )
{
}

/**********************************************************************/

int CEposReportBlockMapSortInfo::Compare( CEposReportBlockMapSortInfo& source, int nHint )
{
	if ( m_bIsSystemBlock != source.m_bIsSystemBlock )
		return ( ( m_bIsSystemBlock ) ? 1 : -1 );

	if ( m_nDbSortIdx != source.m_nDbSortIdx )
		return ( ( m_nDbSortIdx > source.m_nDbSortIdx ) ? 1 : -1 );

	if ( m_bIsDbBlock != source.m_bIsDbBlock )
		return ( ( m_bIsDbBlock ) ? 1 : -1 );

	if ( m_nRpSortIdx != source.m_nRpSortIdx )
		return ( ( m_nRpSortIdx > source.m_nRpSortIdx ) ? 1 : -1 );

	if ( m_nLocSortIdx != source.m_nLocSortIdx )
		return ( ( m_nLocSortIdx > source.m_nLocSortIdx ) ? 1 : -1 );

	if ( m_nMapIdx != source.m_nMapIdx )
		return ( ( m_nMapIdx > source.m_nMapIdx ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportBlockMap::CEposReportBlockMap(void)
{
	m_bConsolidateByTerminal = FALSE;
	m_bConsolidateByLocation = FALSE;
	m_bConsolidateByDatabase = FALSE;
	m_bConsolidateBySystem = FALSE;
	
	if ( EcrmanOptions.GetReportsGroupReportpointFlag() == TRUE )
		m_nSpecialMode = BLOCKMAP_SPECIAL_REPPOINT;
	else
		m_nSpecialMode = BLOCKMAP_SPECIAL_NONE;
}

/**********************************************************************/

CEposReportBlockMap::~CEposReportBlockMap(void)
{
}

/**********************************************************************/

void CEposReportBlockMap::SetDorbiereMode()
{
#ifdef STOCKMAN_SYSTEM
	m_nSpecialMode = BLOCKMAP_SPECIAL_DORBIERE;
#endif
}

/**********************************************************************/

void CEposReportBlockMap::BuildMap( CEposSelectArray& SelectArray )
{
	dbLocation.ResetRpNoReport();
	m_arrayTempIndex.RemoveAll();
	m_mapTerminalToBlock.RemoveAll();
	m_arrayBlockMapEntity.RemoveAll();
	m_arrayDuplicateBlockCheck.RemoveAll();

	for ( int nIdx = 0; nIdx < SelectArray.GetListSize(); nIdx++ )
	{
		CEposSelectArrayListItem ListItem;
		SelectArray.GetListItem( nIdx, ListItem );

		CEposReportBlockMapTempIndex info;
		info.m_nDbIdx = ListItem.m_nDbIdx;
		info.m_nLocIdx = ListItem.m_nLocIdx;
		info.m_nTermIdx = ListItem.m_nTermIdx;
		info.m_nSelArrayIdx = nIdx;

		switch( m_nSpecialMode )
		{

#ifdef STOCKMAN_SYSTEM

		case BLOCKMAP_SPECIAL_DORBIERE:
			{
				int nDbNo = dbDatabase.GetDbNo( ListItem.m_nDbIdx );
				int nSpNo = dbLocation.GetSpNo( ListItem.m_nLocIdx, ListItem.m_nTermIdx );
			
				if ( dbStockpoint.FindStockpointByNumber( nDbNo, nSpNo, info.m_nRpIdx ) == FALSE )
					info.m_nRpIdx = -1;
			}
			break;

#endif

		case BLOCKMAP_SPECIAL_REPPOINT:
			{
				int nDbNo = dbDatabase.GetDbNo( ListItem.m_nDbIdx );
				int nRpNo = dbLocation.GetRpNoReport( ListItem.m_nLocIdx, ListItem.m_nTermIdx );
			
				if ( dbReportpoint.FindReportpointByNumber( nDbNo, nRpNo, info.m_nRpIdx ) == FALSE )
					info.m_nRpIdx = -1;
			}
			break;

		case BLOCKMAP_SPECIAL_NONE:
		default:
			info.m_nRpIdx = -1;
			break;
		}
		
		m_arrayTempIndex.Consolidate(info);
		m_mapTerminalToBlock.Add( "" );
	}

	m_nTempDbStart = 0;
	m_nTempRpStart = 0;
	m_nTempLocStart = 0;
	int nCurrentDb = -1;
	int nCurrentRp = -2;
	int nCurrentLoc = -1;
	
	for ( m_nTempIdx = 0; m_nTempIdx < m_arrayTempIndex.GetSize(); m_nTempIdx++ )
	{
		CEposReportBlockMapTempIndex info;
		m_arrayTempIndex.GetAt( m_nTempIdx, info);

		switch( m_nSpecialMode )
		{
		case BLOCKMAP_SPECIAL_REPPOINT:
			if ( info.m_nRpIdx != nCurrentRp )
			{
				if ( ( nCurrentRp != -2 ) && ( TRUE == m_bConsolidateByLocation ) )
					LinkTerminalsToBlock( NODE_REPORTPOINT );

				m_nTempRpStart = m_nTempIdx;
				nCurrentRp = info.m_nRpIdx;
			}
			break;

#ifdef STOCKMAN_SYSTEM

		case BLOCKMAP_SPECIAL_DORBIERE:
			if ( info.m_nRpIdx != nCurrentRp )
			{
				if ( ( nCurrentRp != -2 ) && ( TRUE == m_bConsolidateByLocation ) )
					LinkTerminalsToBlock( NODE_STOCKPOINT );

				m_nTempRpStart = m_nTempIdx;
				nCurrentRp = info.m_nRpIdx;
			}
			break;

#endif

		case BLOCKMAP_SPECIAL_NONE:
		default:
			if ( info.m_nLocIdx != nCurrentLoc )
			{
				if ( ( nCurrentLoc != -1 ) && ( TRUE == m_bConsolidateByLocation ) )
					LinkTerminalsToBlock( NODE_LOCATION );

				m_nTempLocStart = m_nTempIdx;
				nCurrentLoc = info.m_nLocIdx;
			}
			break;
		}

		if ( info.m_nDbIdx != nCurrentDb )
		{
			if ( ( nCurrentDb != -1 ) && ( TRUE == m_bConsolidateByDatabase ) )
				LinkTerminalsToBlock( NODE_DATABASE );

			m_nTempDbStart = m_nTempIdx;
			nCurrentDb = info.m_nDbIdx;
		}

		if ( TRUE == m_bConsolidateByTerminal )
			LinkTerminalsToBlock( NODE_LOCATION_TERMINAL );
	}

	switch( m_nSpecialMode )
	{

#ifdef STOCKMAN_SYSTEM

	case BLOCKMAP_SPECIAL_DORBIERE:
		if ( ( nCurrentRp != -2 ) && ( TRUE == m_bConsolidateByLocation ) )
			LinkTerminalsToBlock( NODE_STOCKPOINT );
		break;

#endif

	case BLOCKMAP_SPECIAL_REPPOINT:
		if ( ( nCurrentRp != -2 ) && ( TRUE == m_bConsolidateByLocation ) )
			LinkTerminalsToBlock( NODE_REPORTPOINT );
		break;
	
	case BLOCKMAP_SPECIAL_NONE:
	default:
		if ( ( nCurrentLoc != -1 ) && ( TRUE == m_bConsolidateByLocation ) )
			LinkTerminalsToBlock( NODE_LOCATION );
		break;
	}

	if ( ( nCurrentDb != -1 ) && ( TRUE == m_bConsolidateByDatabase ) )
		LinkTerminalsToBlock( NODE_DATABASE );
	
	if ( TRUE == m_bConsolidateBySystem )
		LinkTerminalsToBlock( NODE_SYSTEM );

	m_arrayTempIndex.RemoveAll();

	//WORK OUT WHICH BLOCKS SHOULD CONTRIBUTE TO THE
	//GRAND TOTAL SECTION FOR PLU REPORTS
	for ( int n = 0; n < m_arrayBlockMapEntity.GetSize(); n++ )
	{
		CEposReportBlockMapEntity entity1 = m_arrayBlockMapEntity.GetAt( n );
		entity1.m_bAddToTotal = TRUE;
		m_arrayBlockMapEntity.SetAt( n, entity1 );
		
		switch( entity1.m_nEntityType )
		{
		case NODE_REPORTPOINT:
		case NODE_STOCKPOINT:
			for ( int x = n - 1; x >= 0; x-- )
			{
				CEposReportBlockMapEntity entity2 = m_arrayBlockMapEntity.GetAt(x);
				
				if ( entity2.m_nRpIdx == entity1.m_nRpIdx )
				{
					entity2.m_bAddToTotal = FALSE;
					m_arrayBlockMapEntity.SetAt( x, entity2 );
				}
				else
					break;
			}
			break;

		case NODE_LOCATION:
			for ( int x = n - 1; x >= 0; x-- )
			{
				CEposReportBlockMapEntity entity2 = m_arrayBlockMapEntity.GetAt(x);
				
				if ( entity2.m_nLocIdx == entity1.m_nLocIdx )
				{
					entity2.m_bAddToTotal = FALSE;
					m_arrayBlockMapEntity.SetAt( x, entity2 );
				}
				else
					break;
			}
			break;

		case NODE_DATABASE:
			for ( int x = n - 1; x >= 0; x-- )
			{
				CEposReportBlockMapEntity entity2 = m_arrayBlockMapEntity.GetAt(x);
				
				if ( entity2.m_nDbIdx == entity1.m_nDbIdx )
				{
					entity2.m_bAddToTotal = FALSE;
					m_arrayBlockMapEntity.SetAt( x, entity2 );
				}
				else
					break;
			}
			break;
		}
	}
}

/**********************************************************************/

void CEposReportBlockMap::LinkTerminalsToBlock( int nNodeType )
{
	int nStartIdx, nEndIdx;

	switch( nNodeType )
	{
	case NODE_LOCATION_TERMINAL:
		nStartIdx = m_nTempIdx;
		nEndIdx = m_nTempIdx;
		break;

	case NODE_REPORTPOINT:
	case NODE_STOCKPOINT:
		nStartIdx = m_nTempRpStart;
		nEndIdx = m_nTempIdx - 1;
		break;

	case NODE_LOCATION:
		nStartIdx = m_nTempLocStart;
		nEndIdx = m_nTempIdx - 1;
		break;

	case NODE_DATABASE:
		nStartIdx = m_nTempDbStart;
		nEndIdx = m_nTempIdx - 1;
		break;

	case NODE_SYSTEM:
		nStartIdx = 0;
		nEndIdx = m_nTempIdx - 1;
		break;

	default:
		return;
	}

	CArray<int,int> arraySelArrayIdx;
	for ( int nSortIdx = nStartIdx; nSortIdx <= nEndIdx; nSortIdx++ )
	{
		CEposReportBlockMapTempIndex info;
		m_arrayTempIndex.GetAt( nSortIdx, info );

		int nSelArrayIdx = info.m_nSelArrayIdx;
		if ( ( nSelArrayIdx >= 0 ) && ( nSelArrayIdx < m_mapTerminalToBlock.GetSize() ) )
			arraySelArrayIdx.Add( nSelArrayIdx );
	}

	{
		for ( int n = 0; n < arraySelArrayIdx.GetSize(); n++ )
		{
			int nSelArrayIdx = arraySelArrayIdx.GetAt(n);

			CCSV csv( m_mapTerminalToBlock.GetAt( nSelArrayIdx ) );
			csv.Add( m_arrayBlockMapEntity.GetSize() );
			m_mapTerminalToBlock.SetAt( nSelArrayIdx, csv.GetLine() );			
		}

		CEposReportBlockMapTempIndex info;
		m_arrayTempIndex.GetAt( nStartIdx, info );

		switch( nNodeType )
		{
		case NODE_LOCATION_TERMINAL:
			{
				CEposReportBlockMapEntity entity;
				entity.m_nEntityType = NODE_LOCATION_TERMINAL;
				entity.m_nDbIdx = info.m_nDbIdx;
				entity.m_nRpIdx = info.m_nRpIdx;
				entity.m_nLocIdx = info.m_nLocIdx;
				entity.m_nTermIdx = info.m_nTermIdx;
				m_arrayBlockMapEntity.Add( entity );
			}
			break;

		case NODE_LOCATION:
			{
				CEposReportBlockMapEntity entity;
				entity.m_nEntityType = NODE_LOCATION;
				entity.m_nDbIdx = info.m_nDbIdx;
				entity.m_nRpIdx = 0;
				entity.m_nLocIdx = info.m_nLocIdx;
				entity.m_nTermIdx = 0;
				m_arrayBlockMapEntity.Add( entity );
			}
			break;

		case NODE_REPORTPOINT:
		case NODE_STOCKPOINT:
			{
				CEposReportBlockMapEntity entity;
				entity.m_nEntityType = nNodeType;
				entity.m_nDbIdx = info.m_nDbIdx;
				entity.m_nRpIdx = info.m_nRpIdx;
				entity.m_nLocIdx = 0;
				entity.m_nTermIdx = 0;
				m_arrayBlockMapEntity.Add( entity );
			}
			break;

		case NODE_DATABASE:
			{
				CEposReportBlockMapEntity entity;
				entity.m_nEntityType = NODE_DATABASE;
				entity.m_nDbIdx = info.m_nDbIdx;
				entity.m_nRpIdx = 0;
				entity.m_nLocIdx = 0;
				entity.m_nTermIdx = 0;
				m_arrayBlockMapEntity.Add( entity );
			}
			break;

		case NODE_SYSTEM:
			{
				CEposReportBlockMapEntity entity;
				entity.m_nEntityType = NODE_SYSTEM;
				entity.m_nDbIdx = 0;
				entity.m_nRpIdx = 0;
				entity.m_nLocIdx = 0;
				entity.m_nTermIdx = 0;
				m_arrayBlockMapEntity.Add( entity );
			}
			break;
		}
	}
}

/**********************************************************************/

void CEposReportBlockMap::GetBlockMapEntity( int nIdx, CEposReportBlockMapEntity& entity )
{
	if ( ( nIdx >= 0 ) && ( nIdx < m_arrayBlockMapEntity.GetSize() ) )
		entity = m_arrayBlockMapEntity.GetAt( nIdx );
}

/**********************************************************************/

void CEposReportBlockMap::GetBlockListForTerminal( int nSelArrayIdx, CArray<int,int>& BlockList )
{
	BlockList.RemoveAll();
	if ( ( nSelArrayIdx >= 0 ) && ( nSelArrayIdx < m_mapTerminalToBlock.GetSize() ) )
	{
		CCSV csv( m_mapTerminalToBlock.GetAt( nSelArrayIdx ) );

		for ( int n = 0; n < csv.GetSize(); n++ )
			BlockList.Add( csv.GetInt(n) );
	}
}

/**********************************************************************/

void CEposReportBlockMap::BuildSortArray()
{
	m_arraySortInfo.RemoveAll();

	bool bGotDatabase = FALSE;
	for ( int n = 0; n < GetBlockMapEntityCount(); n++ )
	{
		CEposReportBlockMapEntity entity;
		GetBlockMapEntity( n, entity );

		if ( NODE_DATABASE == entity.m_nEntityType )
		{
			bGotDatabase = TRUE;
			break;
		}
	}

	for ( int nEntityIdx = 0; nEntityIdx < GetBlockMapEntityCount(); nEntityIdx++ )
	{
		CEposReportBlockMapEntity entity;
		GetBlockMapEntity( nEntityIdx, entity );

		int nDbSortIdx = dbDatabase.GetSortIdxFromDbIdx( entity.m_nDbIdx );
		int nLocSortIdx = dbLocation.GetSortIdxFromLocIdx( entity.m_nLocIdx );
		int nRpSortIdx = dbReportpoint.GetSortIdxFromRpIdx( entity.m_nRpIdx );

		CEposReportBlockMapSortInfo info;

		switch( entity.m_nEntityType )
		{
		case NODE_SYSTEM:
			info.m_bIsSystemBlock = TRUE;
			info.m_nDbSortIdx = 0;
			info.m_bIsDbBlock = FALSE;
			info.m_nRpSortIdx = -1;
			info.m_nLocSortIdx = 0;
			break;

		case NODE_DATABASE:
			info.m_bIsSystemBlock = FALSE;
			info.m_nDbSortIdx = ( bGotDatabase ) ? nDbSortIdx : 0;
			info.m_bIsDbBlock = TRUE;
			info.m_nRpSortIdx = -1;
			info.m_nLocSortIdx = 0;
			break;

		case NODE_LOCATION:
			info.m_bIsSystemBlock = FALSE;
			info.m_nDbSortIdx = ( bGotDatabase ) ? nDbSortIdx : 0;
			info.m_bIsDbBlock = FALSE;
			info.m_nRpSortIdx = -1;
			info.m_nLocSortIdx = nLocSortIdx;
			break;

		case NODE_REPORTPOINT:
		case NODE_STOCKPOINT:
			info.m_bIsSystemBlock = FALSE;
			info.m_nDbSortIdx = ( bGotDatabase ) ? nDbSortIdx : 0;
			info.m_bIsDbBlock = FALSE;
			info.m_nRpSortIdx = nRpSortIdx;
			info.m_nLocSortIdx = 999999;
			break;

		case NODE_LOCATION_TERMINAL:
			info.m_bIsSystemBlock = FALSE;
			info.m_nDbSortIdx = ( bGotDatabase ) ? nDbSortIdx : 0;
			info.m_bIsDbBlock = FALSE;
			info.m_nRpSortIdx = nRpSortIdx;
			info.m_nLocSortIdx = nLocSortIdx;
			break;
		}

		info.m_nMapIdx = nEntityIdx;
		m_arraySortInfo.Consolidate( info );
	}
}

/**********************************************************************/

int CEposReportBlockMap::GetSortArrayIdx( int nIdx )
{
	int nResult = 0;
	if ( ( nIdx >= 0 ) && ( nIdx < m_arraySortInfo.GetSize() ) )
	{
		CEposReportBlockMapSortInfo info;
		m_arraySortInfo.GetAt( nIdx, info );
		nResult = info.m_nMapIdx;
	}
	return nResult;
}

/**********************************************************************/

void CEposReportBlockMap::WriteToFile()
{
	CSSFile fileOut;
	fileOut.Open( ".\\general.txt", "ab" );

	for ( int n = 0; n < m_mapTerminalToBlock.GetSize(); n++ )
		fileOut.WriteLine( m_mapTerminalToBlock.GetAt(n) );

	fileOut.WriteLine( "" );

	for ( int n = 0; n < m_arrayBlockMapEntity.GetSize(); n++ )
	{
		CEposReportBlockMapEntity entity = m_arrayBlockMapEntity.GetAt( n );

		CCSV csv;
		switch( entity.m_nEntityType )
		{
		case NODE_LOCATION_TERMINAL:
			csv.Add( "Terminal" );
			csv.Add( dbLocation.GetName( entity.m_nLocIdx ) );
			break;

		case NODE_LOCATION:
			csv.Add( "Location" );
			csv.Add( dbLocation.GetName( entity.m_nLocIdx ) );
			break;

		case NODE_REPORTPOINT:
			csv.Add( "Report Point" );
			csv.Add( dbReportpoint.GetName( entity.m_nRpIdx ) );
			break;

#ifdef STOCKMAN_SYSTEM

		case NODE_STOCKPOINT:
			csv.Add( "Stockpoint" );
			csv.Add( dbStockpoint.GetName( entity.m_nRpIdx ) );
			break;

#endif

		case NODE_DATABASE:
			csv.Add( "Database" );
			csv.Add( dbDatabase.GetName( entity.m_nDbIdx ) );
			break;

		case NODE_SYSTEM:
			csv.Add( "System" );
			break;

		default:
			csv.Add( "Unknown" );
			break;
		}

		csv.Add( entity.m_nDbIdx ); 
		csv.Add( entity.m_nLocIdx ); 
		csv.Add( entity.m_nTermIdx ); 

		fileOut.WriteLine( csv.GetLine() );

		fileOut.WriteLine( "" );

	}
}

/**********************************************************************/
