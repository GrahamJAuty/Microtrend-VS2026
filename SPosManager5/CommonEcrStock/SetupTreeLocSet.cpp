/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DealerFlags.h"
#include "..\CommonEcrStock\DefEditDlg.h"
//#include "EcrmanOptionsIni.h"
#include "NetworkCSVArray.h"
#include "SelectMultipleLocationDlg.h"
#include "LocationCSVArray.h"
#include "LocationSetCSVArray.h"
#include "NodeTypes.h"
 
#include "TreeNode.h"
/**********************************************************************/
#include "PropPageSetupSystem.h"
/**********************************************************************/

void CPropPageSetupSystem::BuildLocSetTree()
{
	HTREEITEM hRoot = AddRoot();

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		for ( int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++ )
			BuildLocSetTreeDatabase( hRoot, nDbIdx );
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName( arrayDbIdx );

		for ( int n = 0; n < arrayDbIdx.GetSize(); n++ )
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			BuildLocSetTreeDatabase( hRoot, nDbIdx );
		}
	}

	m_buttonAction4.ShowWindow( SW_HIDE );
	CollapseLocSetTree();
	m_treeSystem.SelectItem ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::BuildLocSetTreeDatabase( HTREEITEM hRoot, int nDbIdx )
{
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	CString strName = dbDatabase.GetName( nDbIdx );

	HTREEITEM hDbNode = AddDatabaseNode ( hRoot, nDbIdx );

	if ( EcrmanOptions.GetReportsLocationSortType() == 0 )
	{
		int nStartIdx, nEndIdx;
		dbLocationSet.GetLsIdxRange( nDbIdx, nStartIdx, nEndIdx );

		for ( int nLsIdx = nStartIdx; nLsIdx <= nEndIdx; nLsIdx++ )
			BuildLocSetTreeLocSet( hDbNode, nDbIdx, nLsIdx );
	}
	else
	{
		CWordArray arrayLsIdx;
		dbLocationSet.SortByName( arrayLsIdx, nDbIdx );

		for ( int n = 0; n < arrayLsIdx.GetSize(); n++ )
		{
			int nLsIdx = arrayLsIdx.GetAt(n);
			BuildLocSetTreeLocSet( hDbNode, nDbIdx, nLsIdx );
		}
	}
}

/**********************************************************************/

void CPropPageSetupSystem::BuildLocSetTreeLocSet( HTREEITEM hDbNode, int nDbIdx, int nLsIdx )
{	
	int nDbNo = dbDatabase.GetDbNo( nDbIdx );
	int nLsNo = dbLocationSet.GetLsNo( nLsIdx );
	CString strName = dbLocationSet.GetName( nLsIdx );
	HTREEITEM hLsNode = AddLocSetNode ( hDbNode, nLsIdx );
	AddLocSetLocationNodes ( hLsNode, nLsNo, nDbNo );
}

/**********************************************************************/

void CPropPageSetupSystem::AddLocSetLocationNodes ( HTREEITEM hLsNode, int nLsNo, int nDbNo )
{
	HTREEITEM hChild;
	while ( ( hChild = m_treeSystem.GetChildItem ( hLsNode ) ) != NULL )
		DeleteSubTreeNodes ( hChild );
	
	int nLsIdx;
	if ( dbLocationSet.FindLocationSetByNumber( nDbNo, nLsNo, nLsIdx ) == FALSE )
		return;

	CReportConsolidationArray<CSortedIntItem> arrayTemp;
	dbLocationSet.TidyNwkLocNoArray( nLsIdx, arrayTemp );
	
	CArray<int,int> LocIdxArray;
	for ( int n = 0; n < arrayTemp.GetSize(); n++ )
	{
		CSortedIntItem item;
		arrayTemp.GetAt( n, item );
		LocIdxArray.Add( item.m_nItem );
	}

	if ( EcrmanOptions.GetReportsLocationSortType() != 0 )
		dbLocation.SortLocListByName( LocIdxArray );

	for ( int n = 0; n < LocIdxArray.GetSize(); n++ )
		AddLocationNode( hLsNode, LocIdxArray.GetAt(n), SETUP_NODE_LOC_LOCSET, FALSE );
}

/**********************************************************************/

void CPropPageSetupSystem::SelectItemLocSetTree ( int nNodeType )
{
	switch ( nNodeType )
	{
	case SETUP_NODE_SYSTEM:
		SetButtonTexts ( "Add", "Edit", "Delete", "" );
		EnableActionButtons ( FALSE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_DBASE:
		SetButtonTexts ( "Add\nSet", "Edit", "Delete", "" );
		EnableActionButtons ( TRUE, FALSE, FALSE, FALSE );
		break;

	case SETUP_NODE_LOCSET:
		SetButtonTexts ( "Link\nLocations", "Edit\nLoc.Set", "Delete\nLoc.Set", "" );
		EnableActionButtons ( TRUE, TRUE, TRUE, FALSE );	
		break;

	case SETUP_NODE_LOC_LOCSET:
		SetButtonTexts ( "Add", "Edit\nLocation", "Remove\nLink", "" );
		EnableActionButtons( FALSE, TRUE, TRUE, FALSE );
		break;
	}
}

/**********************************************************************/

void CPropPageSetupSystem::Button1LocSetTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_DBASE:		
		AddLocSet();			
		break;

	case SETUP_NODE_LOCSET:		
		AddLocationsToLocSet();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button2LocSetTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_LOCSET:		
		EditLocSet();		
		break;

	case SETUP_NODE_LOC_LOCSET:
		EditLoc( FALSE );
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::Button3LocSetTree()
{
	switch ( GetNodeType( m_hItem ) )
	{
	case SETUP_NODE_LOCSET:		
		DeleteLocSet();	
		break;

	case SETUP_NODE_LOC_LOCSET:	
		RemoveLocationsFromLocSet();	
		break;
	}
	m_treeSystem.SetFocus();
}

/**********************************************************************/

void CPropPageSetupSystem::AddLocSet()
{
	int nLsIdx = dbLocationSet.AddLocationSet ( m_nDbNo );
	if ( nLsIdx == -1 )
	{
		Prompter.Error ( "You have already set up the\nmaximum number of location sets" ); 
		return;
	}

	m_treeSystem.SelectItem ( AddLocSetNode ( m_hItem, nLsIdx ) );
}

/**********************************************************************/

void CPropPageSetupSystem::EditLocSet()
{
	int nLsIdx;
	if ( dbLocationSet.FindLocationSetByNumber ( m_nDbNo, m_nLsNo, nLsIdx ) == FALSE )
		return;
	
	CChangeLocationSetNameDlg dlg ( nLsIdx, this );

	if ( dlg.DoModal() == IDOK )
	{
		dbLocationSet.SetName ( nLsIdx, dlg.m_strName );
		CString strName = dbLocationSet.GetName( nLsIdx );
		m_treeSystem.SetItemText ( m_hItem, strName );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::DeleteLocSet()
{
	int nLsIdx;
	if ( dbLocationSet.FindLocationSetByNumber ( m_nDbNo, m_nLsNo, nLsIdx ) == FALSE )
		return;
	
	if ( Prompter.YesNo ( "Are you sure you wish to delete this location set" ) == IDNO )
		return;

	if ( dbLocationSet.RenameForDeletion( nLsIdx ) == FALSE )
		Prompter.Error( "Unable to delete location set" );
	else
		DeleteSubTreeNodesAndData ( m_hItem );
}

/**********************************************************************/

void CPropPageSetupSystem::AddLocationsToLocSet()
{
	int nLsIdx;
	if ( dbLocationSet.FindLocationSetByNumber( m_nDbNo, m_nLsNo, nLsIdx ) == FALSE )
		return;

	int nDbIdx;
	if ( dbDatabase.FindDatabaseByNumber( m_nDbNo, nDbIdx ) == FALSE )
		return;

	CReportConsolidationArray<CSortedIntItem> arrayLocIdxInSet;
	dbLocationSet.TidyNwkLocNoArray( nLsIdx, arrayLocIdxInSet );

	CArray<int,int> arrayLocIdxInDatabase;
	dbLocation.GetDatabaseLocIdxSet( nDbIdx, arrayLocIdxInDatabase );

	CArray<int,int> arrayLocIdxAvailable;
	for ( int n = 0; n < arrayLocIdxInDatabase.GetSize(); n++ )
	{
		CSortedIntItem item;
		item.m_nItem = arrayLocIdxInDatabase.GetAt(n);

		int nPos;
		if ( arrayLocIdxInSet.Find( item, nPos ) == FALSE )
			arrayLocIdxAvailable.Add( item.m_nItem );
	}
			
	if ( arrayLocIdxAvailable.GetSize() == 0 )
	{
		Prompter.Error ( "There are no more locations available to link" );
		return;
	}

	CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo> arrayLocInfo;
	CSelectMultipleLocationDlg::CreateLocInfoArray( arrayLocIdxAvailable, arrayLocInfo );
	CSelectMultipleLocationDlg dlgSelect( "Add Locations to Location Set", arrayLocInfo, TRUE, this );
	
	if ( dlgSelect.DoModal() == IDOK )
	{
		CDWordArray arrayNwkLocNo;

		for ( int n = 0; n < arrayLocIdxInSet.GetSize(); n++ )
		{
			CSortedIntItem item;
			arrayLocIdxInSet.GetAt(n, item);
			arrayNwkLocNo.Add( dbLocation.GetNetworkLocNo( item.m_nItem ) );
		}

		for ( int n = 0; n < arrayLocInfo.GetSize(); n++ )
			if ( TRUE == arrayLocInfo[n].m_bSelected )
				arrayNwkLocNo.Add( dbLocation.GetNetworkLocNo( arrayLocInfo[n].m_nLocIdx ) );	
		
		dbLocationSet.SetNwkLocNoArray( nLsIdx, arrayNwkLocNo );
		AddLocSetLocationNodes( m_hItem, m_nLsNo, m_nDbNo );
	}
}

/**********************************************************************/

void CPropPageSetupSystem::RemoveLocationsFromLocSet()
{
	HTREEITEM hItem = m_treeSystem.GetSelectedItem();
	RemoveLinks( hItem, SETUP_NODE_LOC_LOCSET );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseLocSetTree()
{
	CollapseLocSetTree ( m_treeSystem.GetRootItem() );
}

/**********************************************************************/

void CPropPageSetupSystem::CollapseLocSetTree( HTREEITEM hItem )
{
	if ( hItem == NULL )
		return;

	int nActionType;
	switch ( GetNodeType( hItem ) )
	{
	case SETUP_NODE_SYSTEM:
	case SETUP_NODE_DBASE:		
		nActionType = TVE_EXPAND;	
		break;

	default:					
		nActionType = TVE_COLLAPSE;	
		break;
	}

	m_treeSystem.Expand ( hItem, nActionType );

	HTREEITEM hChild = m_treeSystem.GetChildItem( hItem );
	while ( hChild != NULL )
	{
		CollapseRepPointTree ( hChild );
		hChild = m_treeSystem.GetNextSiblingItem ( hChild );
	}
}

/**********************************************************************/

