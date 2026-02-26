/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LocationSetCSVArray.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
#include "ReportpointCSVArray.h"
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "..\SPosStockManager5\StockpointCSVArray.h"
#include "SystemMap.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

CLocationSelector::CLocationSelector() 
{
	//INTERNAL CONTROL
	m_nPreviousSelectionTerminal = -1;
	m_nPreviousSelectionTable = -1;
	m_pLocCombo = NULL;
	m_pEposCombo = NULL;
	m_pTableCombo = NULL;
	m_pTableReportHelpers = NULL;
	
	//SPECIAL PROCESSING
	m_nDatabaseMode = NODE_SYSTEM;
	m_bJournalReportMode = FALSE;
	m_bExcludeAllTerminals = FALSE;
	m_bSelectAllTerminals = FALSE;
	m_bExcludeNoComms = FALSE;
	m_bExcludeWebSales = FALSE;
	m_nExcludeDbIdx = -1;
	m_nExcludeSpIdx = -1;
	m_nSingleDbIdx = -1;
	m_nSingleSpIdx = -1;
	m_nSingleLocIdx = -1;
	m_nLocDbNoFilter = 0;

	m_bForceSystem = FALSE;
	m_bForceGroupByDb = FALSE;
	m_bStockTransfer = FALSE;

	m_ImageList.Create ( IDB_ICONS_COMBO, 31, 0, COLORREF(0xFFFFFF) );
}

/**********************************************************************/

void CLocationSelector::SetDatabaseMode ( int nMode )
{
	switch ( nMode )
	{
	case NODE_SYSTEM:
	case NODE_STOCKPOINT:
	case NODE_LOCATION:		m_nDatabaseMode = nMode;		break;	
	default:	break;
	}
}

/**********************************************************************/

void CLocationSelector::BuildList( CWordArray& arrayTypes )
{
	m_arrayListItems.RemoveAll();

	DealerFlags.GetLocationSelectorOrder ( arrayTypes );

	for ( int nIndex = 0; nIndex < arrayTypes.GetSize(); nIndex++ )
	{
		int nType = arrayTypes.GetAt ( nIndex );
		switch ( nType )
		{
		case NODE_DATABASE:					AddDatabases();			break;
		case NODE_LOCATION:					AddLocations();			break;
		case NODE_NETWORK:					AddNetworks();			break;
		case NODE_REPORTPOINT:				AddReportpoints();		break;
		case NODE_LOCATIONSET:				AddLocationSets();		break;
		case NODE_SYSTEM:					AddSystem();			break;
			
#ifdef STOCKMAN_SYSTEM
		case NODE_STOCKPOINT:				AddStockpoints();		break;
		case NODE_SITE:						AddSites();				break;
#endif
		default:	break;
		}
	}
}

/**********************************************************************/

void CLocationSelector::AddToLocCombo ( const char* szText, int nImage )
{
	char* p = (char*) szText;

	COMBOBOXEXITEM item;
	item.mask = CBEIF_DI_SETITEM | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.iItem = m_pLocCombo -> GetCount();
	item.pszText = p;
	item.iImage = nImage;
	item.iSelectedImage = nImage;

	m_pLocCombo -> InsertItem ( &item );
}

/**********************************************************************/

void CLocationSelector::FillLocationCombo ( CSSComboBoxEx* pLocCombo, CSSTabbedComboBox* pEposCombo, CSSTabbedComboBox* pTableCombo, CTableReportHelpers* pTableReportHelpers  )
{
	m_pLocCombo = pLocCombo;
	m_pEposCombo = pEposCombo;
	m_pTableCombo = pTableCombo;
	m_pTableReportHelpers = pTableReportHelpers;

	m_pLocCombo -> ResetContent();
	m_pLocCombo -> SetImageList ( &m_ImageList );
	
	for ( int nPos = 0; nPos < m_arrayListItems.GetSize(); nPos++ )
	{
		CLocationSelectorEntity LocSelEntity = m_arrayListItems.GetAt( nPos );

		switch ( LocSelEntity.GetConType() )
		{
		case NODE_SYSTEM:
			AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_SYSTEM );
			break;

		case NODE_DATABASE:
			AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_DATABASE );
			break;

		case NODE_NETWORK:
			AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_NETWORK );
			break;
	
		case NODE_LOCATION:
			AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_LOCATION );
			break;
	
		case NODE_REPORTPOINT:
			AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_REPPOINT );
			break;

		case NODE_LOCATIONSET:
			AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_LOCSET );
			break;

#ifdef STOCKMAN_SYSTEM
		case NODE_SITE:
			AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_SITE );
			break;

		case NODE_STOCKPOINT:
			{
				int nSpIdx;
				bool bIsWarehouse = FALSE;
				if ( dbStockpoint.FindStockpointByNumber( LocSelEntity.GetDbNo(), LocSelEntity.GetEntityNo(), nSpIdx ) == TRUE )
					bIsWarehouse = dbStockpoint.GetWarehouseFlag( nSpIdx ); 
			
				if ( FALSE == bIsWarehouse )
					AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_STOCKPOINT );
				else
					AddToLocCombo ( LocSelEntity.GetEntityName(), NODE_COMBOICON_WAREHOUSE );
			}
			break;
#endif
		}
	}
	int nCount = m_pLocCombo -> GetCount();
	if ( nCount > 0 )	m_pLocCombo -> SetCurSel(0);
	if ( nCount < 2 )	m_pLocCombo -> EnableWindow ( FALSE );
	FillEposCombo();
	FillTableCombo();
}

/**********************************************************************/

void CLocationSelector::FillEposCombo()
{
	if ( m_pEposCombo == NULL )
		return;

	int nSelection = m_pLocCombo -> GetCurSel();
	if ( ( nSelection < 0 ) || ( nSelection >= m_pLocCombo -> GetCount() ) || ( nSelection == m_nPreviousSelectionTerminal ) )
		return;

	CLocationSelectorEntity LocSelEntity;
	GetSelectedEntity( LocSelEntity );

	m_pEposCombo -> ResetContent();

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(35);
		m_pEposCombo -> SetTabStops( arrayStops );
	}

	int nLocIdx = 0;
	bool bSelectSingleTerminal = FALSE;
	m_bSelectAllTerminals = !m_bExcludeAllTerminals;

	switch( LocSelEntity.GetConType() )
	{
	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
		if ( dbLocation.FindLocationByNumber( LocSelEntity.GetNwkNo(), LocSelEntity.GetEntityNo(), nLocIdx ) == TRUE )
		{
			bSelectSingleTerminal = TRUE;
			m_bSelectAllTerminals &= ( dbLocation.GetTerminalCount( nLocIdx ) > 1 );
		}
		break;
	}
	
	if ( bSelectSingleTerminal == TRUE )
	{
		for ( int nTerminalIdx = 0; nTerminalIdx < dbLocation.GetTerminalCount( nLocIdx ); nTerminalIdx++ )
		{
			CString strItem;
			strItem.Format( "%3.3d,%s",
				dbLocation.GetTNo( nLocIdx, nTerminalIdx ),
				dbLocation.GetTerminalName ( nLocIdx, nTerminalIdx ) );

			m_pEposCombo -> AddItem ( strItem );
		}
	}

	if ( m_bSelectAllTerminals == TRUE ) 
		m_pEposCombo -> AddItem ( ",All Terminals" );
		
	int nCount = m_pEposCombo -> GetCount();
	if ( nCount > 0 ) m_pEposCombo -> SetCurSel( ( m_bSelectAllTerminals ) ? nCount - 1 : 0 );
	m_pEposCombo -> EnableWindow ( nCount >= 2 );

	m_nPreviousSelectionTerminal = nSelection;
}

/**********************************************************************/

void CLocationSelector::FillTableCombo()
{
	if ( m_pTableCombo == NULL )
		return;

	int nSelection = m_pLocCombo -> GetCurSel();
	if ( ( nSelection < 0 ) || ( nSelection >= m_pLocCombo -> GetCount() ) || ( nSelection == m_nPreviousSelectionTable ) )
		return;

	CLocationSelectorEntity LocSelEntity;
	GetSelectedEntity( LocSelEntity );

	m_pTableCombo -> ResetContent();

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(35);
		m_pTableCombo -> SetTabStops( arrayStops );
	}

	int nLocIdx = -1;
	
	switch( LocSelEntity.GetConType() )
	{
	case NODE_LOCATION:
	case NODE_LOCATION_TERMINAL:
		{
			 int nFoundLocIdx = 0;		
			 if ( dbLocation.FindLocationByNumber( LocSelEntity.GetNwkNo(), LocSelEntity.GetEntityNo(), nFoundLocIdx ) == TRUE )			
			 {
				 nLocIdx = nFoundLocIdx;
			 }
		}
		break;
	}

	m_pTableReportHelpers -> SetLocIdx( nLocIdx );
	m_pTableReportHelpers -> BuildTableFilterList();

	for ( int n = 0; n < m_pTableReportHelpers -> GetTableFilterCount(); n++ )
	{
		CTableFilter TableFilter;
		m_pTableReportHelpers -> GetTableFilter( n, TableFilter );

		CString strPrefix = ",";
		switch( TableFilter.m_nType )
		{
		case TABLEFILTER_TABLE:
			strPrefix = "TBL,";
			break;

		case TABLEFILTER_GROUP:
			strPrefix = "GP,";
			break;

		case TABLEFILTER_SET:
			strPrefix = "SET,";
			break;
	
		case TABLEFILTER_ALL:
		default:	
			strPrefix = ",";	
			break;
		}

		m_pTableCombo -> AddItem( strPrefix + TableFilter.m_strName );
	}

	m_pTableCombo -> SetCurSel(0);
	m_pTableCombo -> EnableWindow( m_pTableCombo -> GetCount() > 1 );

	m_nPreviousSelectionTable = nSelection;
}

/**********************************************************************/

void CLocationSelector::SelectTableComboFilter( CTableFilter& Filter )
{
	if ( ( m_pTableCombo == NULL ) || ( m_pTableReportHelpers == NULL ) )
		return;

	if ( m_pTableCombo -> GetCount() != m_pTableReportHelpers -> GetTableFilterCount() )
		return;

	if ( m_pTableCombo -> GetCount() == 0 )
		return;

	m_pTableCombo -> SetCurSel(0);

	for ( int n = 1; n < m_pTableReportHelpers -> GetTableFilterCount(); n++ )
	{
		CTableFilter arrayFilter;
		m_pTableReportHelpers -> GetTableFilter( n, arrayFilter );

		if ( ( Filter.m_nType == arrayFilter.m_nType ) && ( Filter.m_nLevel == arrayFilter.m_nLevel ) && ( Filter.m_nEntityNo == arrayFilter.m_nEntityNo ) )
		{
			m_pTableCombo -> SetCurSel(n);
			break;
		}
	}
}

/**********************************************************************/

bool CLocationSelector::IncludeDbIdx ( int nDbIdx )
{
	if ( m_nExcludeDbIdx == nDbIdx )	
		return FALSE;
	
	if ( ( m_nSingleDbIdx != -1 ) && ( m_nSingleDbIdx != nDbIdx ) )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CLocationSelector::AddDatabases()
{
	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
			AddDatabase( arrayDbIdx.GetAt(n) );
	}
	else
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			AddDatabase( nDbIdx );
	}
}

/**********************************************************************/

void CLocationSelector::AddDatabase( int nDbIdx )
{
	if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
		return;

	bool bAdd = TRUE;
	switch ( m_nDatabaseMode )
	{
	case NODE_LOCATION:
		{
			CArray<int,int> Set;
			dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );
			bAdd = ( Set.GetSize() != 0 );
		}
		break;
	
#ifdef STOCKMAN_SYSTEM
	case NODE_STOCKPOINT:
		{
			int nStartIdx, nEndIdx;
			dbStockpoint.GetSpIdxRange( nDbIdx, nStartIdx, nEndIdx );
			bAdd = ( nEndIdx > nStartIdx );
		}
		break;
#endif
	}

	if ( bAdd == TRUE )	
	{
		CLocationSelectorEntity LocSelEntity;
		LocSelEntity.SetDatabaseNode( nDbIdx );
		m_arrayListItems.Add( LocSelEntity );
	}
}

/**********************************************************************/

void CLocationSelector::AddLocations()
{
	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
	{
		CArray<int,int> arrayLocIdx;
		dbLocation.SortByName( arrayLocIdx, -1, -1, m_bForceGroupByDb );

		for ( int n = 0; n < arrayLocIdx.GetSize(); n++ )
		{
			int nLocIdx = arrayLocIdx.GetAt(n);

			int nDbIdx;
			int nDbNo = dbLocation.GetDbNo( nLocIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == TRUE )
				AddLocation( nLocIdx );
		}
	}
	else
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			if ( IncludeDbIdx ( nDbIdx ) == TRUE )
			{
				CArray<int,int> Set;
				dbLocation.GetDatabaseLocIdxSet ( nDbIdx, Set );

				for ( int nIndex = 0; nIndex < Set.GetSize(); nIndex++ )
				{
					int nLocIdx = Set.GetAt ( nIndex );
					AddLocation( nLocIdx );
				}
			}
		}
	}
}

/**********************************************************************/

void CLocationSelector::AddLocation( int nLocIdx )
{
	if ((m_nSingleLocIdx != -1) && (m_nSingleLocIdx != nLocIdx))
	{
		return;
	}

	if (0 != m_nLocDbNoFilter)
	{
		if (dbLocation.GetDbNo(nLocIdx) != m_nLocDbNoFilter)
		{
			return;
		}
	}
			
	if ( ( m_bExcludeNoComms ) || ( m_bExcludeWebSales ) )
	{
		int nNetworkIdx;
		int nNetworkNo = dbLocation.GetNetworkNo( nLocIdx );
		if ( dbNetwork.FindNetworkByNumber ( nNetworkNo, nNetworkIdx ) == FALSE )
			return;

		bool bExclude = FALSE;
		switch( dbNetwork.GetConnectionType( nNetworkIdx ) )
		{
		case CONNECTION_TYPE_STANDARD_NONE:
			bExclude = m_bExcludeNoComms;
			break;

		case CONNECTION_TYPE_SPTBOOK_WEB:
		case CONNECTION_TYPE_SMARTENT_WEB:
			bExclude = m_bExcludeWebSales;
			break;

		case CONNECTION_TYPE_SPTBOOK_NONE:
		case CONNECTION_TYPE_SMARTENT_NONE:
			bExclude = ( m_bExcludeWebSales || m_bExcludeNoComms );
			break;
		}

		if ( TRUE == bExclude )
			return;
	}
			
	CLocationSelectorEntity LocSelEntity;
	LocSelEntity.SetLocationNode( nLocIdx );
	m_arrayListItems.Add( LocSelEntity );
}

/**********************************************************************/

void CLocationSelector::AddNetworks()
{
	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
	{
		CWordArray arrayNwkIdx;
		dbNetwork.SortByName( arrayNwkIdx );

		for ( int n = 0; n < arrayNwkIdx.GetSize(); n++ )
		{
			int nNwkIdx = arrayNwkIdx.GetAt(n);

			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetNetworkNode( nNwkIdx );
			m_arrayListItems.Add( LocSelEntity );
		}
	}
	else
	{
		for ( int nNwkIdx = 0; nNwkIdx < dbNetwork.GetSize(); nNwkIdx++ )
		{
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetNetworkNode( nNwkIdx );
			m_arrayListItems.Add( LocSelEntity );
		}
	}
}

/**********************************************************************/

void CLocationSelector::AddReportpoints()
{
	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
	{
		CWordArray arrayRpIdx;
		dbReportpoint.SortByName( arrayRpIdx );

		for ( int n = 0; n < arrayRpIdx.GetSize(); n++ )
		{
			int nRpIdx = arrayRpIdx.GetAt(n);

			int nDbIdx;
			int nDbNo = dbReportpoint.GetDbNo( nRpIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
				continue;

			if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
				continue;

			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetReportpointNode( nRpIdx );
			m_arrayListItems.Add( LocSelEntity );
		}
	}
	else
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
				continue;

			int nStartIdx, nEndIdx;
			dbReportpoint.GetRpIdxRange( nDbIdx, nStartIdx, nEndIdx );

			for ( int nRpIdx = nStartIdx; nRpIdx <= nEndIdx; nRpIdx++ )
			{
				CLocationSelectorEntity LocSelEntity;
				LocSelEntity.SetReportpointNode( nRpIdx );
				m_arrayListItems.Add( LocSelEntity );
			}
		}
	}
}

/**********************************************************************/

void CLocationSelector::AddLocationSets()
{
	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
	{
		CWordArray arrayLsIdx;
		dbLocationSet.SortByName( arrayLsIdx );

		for ( int n = 0; n < arrayLsIdx.GetSize(); n++ )
		{
			int nLsIdx = arrayLsIdx.GetAt(n);

			int nDbIdx;
			int nDbNo = dbLocationSet.GetDbNo( nLsIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
				continue;

			if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
				continue;

			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetLocationSetNode( nLsIdx );
			m_arrayListItems.Add( LocSelEntity );
		}
	}
	else
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
				continue;

			int nStartIdx, nEndIdx;
			dbLocationSet.GetLsIdxRange( nDbIdx, nStartIdx, nEndIdx );

			for ( int nLsIdx = nStartIdx; nLsIdx <= nEndIdx; nLsIdx++ )
			{
				CLocationSelectorEntity LocSelEntity;
				LocSelEntity.SetLocationSetNode( nLsIdx );
				m_arrayListItems.Add( LocSelEntity );
			}
		}
	}
}

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

void CLocationSelector::AddStockpoints()
{
	CWordArray arraySpIdx;
	dbStockpoint.SortByName( arraySpIdx );

	for ( int n = 0; n < arraySpIdx.GetSize(); n++ )
	{
		int nSpIdx = arraySpIdx.GetAt(n);

		int nDbIdx;
		int nDbNo = dbStockpoint.GetDbNo( nSpIdx );
		if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
			continue;

		if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
			continue;

		if ( TRUE == m_bStockTransfer )
			if ( dbStockpoint.GetTransferInFlag( nSpIdx ) == FALSE )
				continue;

		if ( ( m_nSingleSpIdx != -1 ) && ( m_nSingleSpIdx != nSpIdx ) )
			continue;

		if ( m_nExcludeSpIdx != nSpIdx )
		{
			CLocationSelectorEntity LocSelEntity;
			LocSelEntity.SetStockpointNode( nSpIdx );
			m_arrayListItems.Add( LocSelEntity );
		}
	}
}

/**********************************************************************/

void CLocationSelector::AddSites()
{
	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
	{
		CWordArray arraySiteIdx;
		dbSite.SortByName( arraySiteIdx );

		for ( int n = 0; n < arraySiteIdx.GetSize(); n++ )
		{
			int nSiteIdx = arraySiteIdx.GetAt(n);

			int nDbIdx;
			int nDbNo = dbSite.GetDbNo( nSiteIdx );
			if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
				continue;

			if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
				continue;

			if ( SystemMapFull.GetSiteSpCount ( nSiteIdx ) >= 2  )
			{
				CLocationSelectorEntity LocSelEntity;
				LocSelEntity.SetSiteNode( nSiteIdx );
				m_arrayListItems.Add( LocSelEntity );
			}
		}
	}
	else
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
		{
			if ( IncludeDbIdx ( nDbIdx ) == FALSE )	
				continue;

			int nStartIdx, nEndIdx;
			dbSite.GetSiteIdxRange( nDbIdx, nStartIdx, nEndIdx );

			for ( int nSiteIdx = nStartIdx; nSiteIdx <= nEndIdx; nSiteIdx++ )
			{
				if ( SystemMapFull.GetSiteSpCount ( nSiteIdx ) >= 2  )
				{
					CLocationSelectorEntity LocSelEntity;
					LocSelEntity.SetSiteNode( nSiteIdx );
					m_arrayListItems.Add( LocSelEntity );
				}
			}
		}
	}
}

/**********************************************************************/
#endif
/**********************************************************************/

void CLocationSelector::AddSystem()
{
	if ( ( dbDatabase.GetSize() >= 2 ) || ( m_bForceSystem == TRUE ) )
	{
		CLocationSelectorEntity LocSelEntity;
		LocSelEntity.SetSystemNode();
		m_arrayListItems.Add( LocSelEntity );
	}
}

/**********************************************************************/

void CLocationSelector::GetSelectedEntity( CLocationSelectorEntity& LocSelEntity )
{
	LocSelEntity.Reset();

	int nSelection = m_pLocCombo -> GetCurSel();
	if ( nSelection < 0 || nSelection >= m_pLocCombo -> GetCount() )
		return;

	LocSelEntity = m_arrayListItems.GetAt( nSelection );

	if ( ( LocSelEntity.GetConType() == NODE_LOCATION ) && ( m_pEposCombo != NULL ) )
	{
		int nTerminalCount = m_pEposCombo -> GetCount();
		int nTerminalSelect = m_pEposCombo -> GetCurSel();

		if ( m_bSelectAllTerminals == TRUE ) 
			nTerminalCount--;
				
		if ( ( nTerminalCount > 1 ) && ( nTerminalSelect < nTerminalCount ) )
			LocSelEntity.SetTerminalNode( nTerminalSelect );
	}
}

/**********************************************************************/

void CLocationSelector::GetListEntry ( int nPos, CLocationSelectorEntity& LocSelEntity )
{
	LocSelEntity.Reset();

	if ( ( nPos >= 0 ) && ( nPos < m_arrayListItems.GetSize() ) )
		LocSelEntity = m_arrayListItems.GetAt( nPos );
}

/**********************************************************************/

void CLocationSelector::ForceSelection ( int nConType, int nIdx )
{
	bool bSelected = FALSE;

	for ( int nPos = 0; nPos < m_pLocCombo -> GetCount(); nPos++ )
	{
		CLocationSelectorEntity LocSelEntity = m_arrayListItems.GetAt( nPos );

		if (	( LocSelEntity.GetConType() == nConType ) && 
				( LocSelEntity.GetEntityIdx() == nIdx ) )
		{
			m_pLocCombo -> SetCurSel ( nPos );
			bSelected = TRUE;
			break;
		}

		if ( ( NODE_DATABASE == nConType ) && ( FALSE == bSelected ) )
		{
			if ( LocSelEntity.GetDbIdx() == nIdx )
			{
				m_pLocCombo -> SetCurSel ( nPos );
				bSelected = TRUE;
			}
		}
	}

	if ( bSelected == TRUE )
		FillEposCombo();
}

/**********************************************************************/

void CLocationSelector::ForceSelection ( CLocationSelectorEntity& LocSelEntity )
{
	bool bSelected = FALSE;

	for ( int nPos = 0; nPos < m_pLocCombo -> GetCount(); nPos++ )
	{
		CLocationSelectorEntity arrayEntity = m_arrayListItems.GetAt( nPos );

		if (	( LocSelEntity.GetConType() == arrayEntity.GetConType() ) && 
				( LocSelEntity.GetDbNo() == arrayEntity.GetDbNo() ) &&
				( LocSelEntity.GetNwkNo() == arrayEntity.GetNwkNo() ) &&
				( LocSelEntity.GetEntityNo() == arrayEntity.GetEntityNo() ) )
		{
			m_pLocCombo -> SetCurSel ( nPos );
			bSelected = TRUE;
			break;
		}

		if ( ( LocSelEntity.GetConType() == NODE_DATABASE ) && ( bSelected == FALSE ) )
		{
			if ( LocSelEntity.GetDbNo() == arrayEntity.GetDbNo() )
			{
				m_pLocCombo -> SetCurSel ( nPos );
				bSelected = TRUE;
			}
		}
	}

	if ( bSelected == TRUE )
		FillEposCombo();
}

/**********************************************************************/

